/**
HalWebsocketd provides an interface to the websocketd. 
This will queue messages from websocketd ready to be used and queue 
messages to be sent. The send queue will update messages in the queue 
with the same Id instead of adding another item to the queue. 
Each time the run function is called, a message will be taken from 
the queue and sent to websocketd

Author and copyright of this file:
Chris Dick, 2016

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <iostream>
#include <sys/poll.h>
#include <string.h>
#include "Config.h"

#ifdef TIMING
#include "GPIO.h"
#endif

#include "HalWebsocketd.h"

HalWebsocketd   HalWebsocketd::Websocket;

/* HalWebsocketd
 *  Constructor
 */
HalWebsocketd::HalWebsocketd( void )
{
}

/* HalWebsocketdInit
 *  Initialise the GPSD connection
 * @return bool Initialisation status  
 */
void HalWebsocketd::Init( void )
{
    /*
        reset all queues and the buffer
    */
    InputQueue.ReadIndex = 0;
    InputQueue.WriteIndex = 0;
    InputQueue.FillLevel = 0;
    OutputQueue.ReadIndex = 0;
    OutputQueue.WriteIndex = 0;
    OutputQueue.FillLevel = 0;
    
#ifdef TIMING
    GPIO::gpio.SetupOutput( HAL_WEBSOCKETD_PIN );
    GPIO::gpio.SetPullMode( HAL_WEBSOCKETD_PIN , PULL_UP );
#endif

}

/* HalWebsocketdRun
 *  Sends a single message from the output queue
 */
void HalWebsocketd::Run( void )
{    
    #ifdef TIMING
    GPIO::gpio.SetPinState( HAL_WEBSOCKETD_PIN , true );
    #endif

    char OutputMessage[DATALENGTH+1] = { 0 };
    ThreadRun();
    /*
        Send a message if any are waiting
    */
    if (!QueueEmpty(&OutputQueue))
    {
        //printf("FillLevel = %d ReadIndex = %d WriteIndex = %d\n", OutputQueue.FillLevel, OutputQueue.ReadIndex, OutputQueue.WriteIndex ); 
        GetNextMessage( &OutputQueue, OutputMessage);
        puts( OutputMessage );   
    }
    #ifdef TIMING
    GPIO::gpio.SetPinState( HAL_WEBSOCKETD_PIN , false );
    #endif
}

/* ThreadRun
 *  Check for any new data store in the input queue
 */
void HalWebsocketd::ThreadRun( void )
{
    static uint8_t BufferIndex = 0;
    static char InputMessage[DATALENGTH+1] = { 0 };
    /*
        Check for new input and add to queue
    */
    struct pollfd fds;
    fds.fd = 0;//STDIN_FILENO ; /* this is STDIN */
    fds.events = POLLIN;
    if ( poll(&fds, 1, 0) == 1)
    {
        while ( poll(&fds, 1, 0) == 1)
        {
            InputMessage[BufferIndex] = getchar();
            if ( InputMessage[BufferIndex] == '\n' )
            {
                InputMessage[BufferIndex] = '\0';
                AddMessage( &InputQueue, InputMessage, 0 );   
                BufferIndex = 0;
            }
            else
            {
                BufferIndex++;
            }
        }
    }
    if ( BufferIndex > DATALENGTH )
    {
        BufferIndex = 0;
    }
}


/* Add a message to the queue
 * This function is not thread safe so must only 
 * be called from the same thread as anything that
 * reads from the output queue.
 * @return bool_t if the message was added
 */
bool HalWebsocketd::SendMessage( char* Message, uint8_t Id )
{
    bool Result = false;
    /*
        check each queue location between read and write indecies
        and update if the message id already exists
    */
    if (!QueueEmpty(&OutputQueue))
    {
        uint8_t Index = OutputQueue.ReadIndex;
        uint8_t I = 0;
        for (I = 0 ; I < OutputQueue.FillLevel; I++)
        {
            if (OutputQueue.Message[Index].Id == Id)
            {   
                strcpy ( OutputQueue.Message[Index].Data, Message );
                Result = true;
                break;
            }
            Index++;
            if (Index == QUEUESIZE)
            {
                Index = 0;
            }
        }
    }

    /*
        If the message Id isn't waiting already
        and there is space on the queue add the new message
        to the queue
    */
    if (
           (!QueueFull(&OutputQueue))
        && ( !Result )
        )
    {
        Result = AddMessage( &OutputQueue, Message, Id );
    }
    return Result;
}

/* Get the next Message in the queue
 * 
 */
bool HalWebsocketd::GetMessage( char* Message )
{
    /*
        if the input queue isn't empty, return the next message
    */
    return GetNextMessage( &InputQueue, Message );   
}
        
/* Is the queue empty
 * @return bool_t true if empty
 */
bool HalWebsocketd::QueueEmpty( MESSAGEQUEUE_T* Queue )  
{
    bool Result = false;
    if ( Queue->FillLevel == 0)
    {
        Result = true; 
    }
    return Result;
}


/* Is the queue full
 * @return bool_t true if full
 */
bool HalWebsocketd::QueueFull( MESSAGEQUEUE_T* Queue )
{
    bool Result = false;
    if ( Queue->FillLevel == (QUEUESIZE))
    {
        Result = true; 
    }
    return Result;
}

/* Get the next message
 * @return bool_t true if successful
 */
bool HalWebsocketd::GetNextMessage( MESSAGEQUEUE_T* Queue, char* Message )    
{
    bool Result = false;
    if (!QueueEmpty(Queue))
    {
        strcpy ( Message, Queue->Message[Queue->ReadIndex].Data);
        Queue->Message[Queue->ReadIndex].Id = 0; // todo - EMPTY(0) is currently at top of the enum, but this may change
        Queue->ReadIndex += 1;
        if ( Queue->ReadIndex >= QUEUESIZE)
        {
           Queue->ReadIndex = 0;
        }
        Result = true;
        /*
            Only adjust the FillLevel when the data has been read 
            to prevent another item being added before were done.
        */
        Queue->FillLevel -= 1;
    }
    return Result;
}

/* Add a message
 * @return bool_t true if successful
 */
bool HalWebsocketd::AddMessage( MESSAGEQUEUE_T* Queue, char* Message, uint8_t Id )    
{
    bool Result = false;   
    if (!QueueFull(Queue))
    {
        strcpy ( Queue->Message[Queue->WriteIndex].Data, Message );
        Queue->Message[Queue->WriteIndex].Id = Id;
        Queue->WriteIndex += 1;
        if ( Queue->WriteIndex >= QUEUESIZE)
        {
           Queue->WriteIndex = 0;
        }
        Result = true;
        /*
            Only adjust the FillLevel when the data has been added 
            to prevent another item being read before were done.
        */
        Queue->FillLevel += 1;
    }
    return Result;
}
