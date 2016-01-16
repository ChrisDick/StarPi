/*
A module to code and decode Messages and inputs tot he system.

Author and copyright of this file
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
#include <string.h>
#include "HalWebsocketd.h"
#include "Config.h"

#include "TelescopeIO.h"
TelescopeIO TelescopeIO::TeleIO;

/* TelescopeIO
 *  Constructor
 */
TelescopeIO::TelescopeIO( void )
{
}

/* initialise the sensors used
 */
bool TelescopeIO::TelescopeIOInit( void )
{
    /*
         Initialise all the data to defaults?
    */
    return true;
}



/* Get any messages recieved from the websocket pass to handler 
 */
void TelescopeIO::TelescopeIOWebRecieve( )
{
    char RecievedMessage[DATALENGTH+HEADERLENGTH];
    while (true)
    {    
        bool MessageAvailiable = false;
        MessageAvailiable = HalWebsocketd::Websocket.HalWebsocketdGetMessage( RecievedMessage );
        if (!MessageAvailiable)
        {
            break;
        }
        TelescopeIOHandleMessage( RecievedMessage );
    }
}

/* Update and send the data
 * @param Id The id of the data to update
 * @param Data The new data to update
 */
bool TelescopeIO::TelescopeIOUpdateData( DATAID_T Id, void* Data )
{
    char Message[DATALENGTH+HEADERLENGTH] = { 0u }; 
    bool SendMessage = false;
    /*
        Format the data into a string
    */
    switch ( TelescopeData[Id].DataFormat )
    {
        case UINT8_2:
        {
            sprintf ( TelescopeData[Id].Data, " %2d", *((uint8_t*)Data) );
            SendMessage = true;
            break;
        }
        case UINT16_4:
        {
            sprintf ( TelescopeData[Id].Data, " %4d", *((uint16_t*)Data) );
            SendMessage = true;
            break;
        }
        case FLOAT:
        case DOUBLE:
        {
            sprintf ( TelescopeData[Id].Data, " %4.4f", *((float*)Data) );
            SendMessage = true;
            break;
        }
        case STRING:
        {
            sprintf ( TelescopeData[Id].Data, " %s", ((char*)Data) );
            SendMessage = true;
            break;
        }        
        default:
        {
            // ignore it.
        }
   }
   
   if ( SendMessage )
   {
        /*
            Add the header and send the message
        */
        strcat(Message, TelescopeData[Id].Header);
        strcat(Message, TelescopeData[Id].Data); 
        HalWebsocketd::Websocket.HalWebsocketdSendMessage(Message, Id );
   }
   return SendMessage;
}


/* Removes the data string and stores it.
 */
void TelescopeIO::TelescopeIOHandleMessage( char* Message )
{
   for (uint8_t Id = 0; Id < NUMBEROFDATA; Id++)
   {
        if ( strncmp (Message, TelescopeData[Id].Header, 4 ) == 0)
        {
            char OutMessage[DATALENGTH+HEADERLENGTH] = {0};
            strcpy(  TelescopeData[Id].Data, &Message[HEADERLENGTH] );
            strcat(OutMessage, TelescopeData[Id].Header);
            strcat(OutMessage, TelescopeData[Id].Data);
            HalWebsocketd::Websocket.HalWebsocketdSendMessage(OutMessage, Id );
        }
    }   
}

/* Gets the value from the string stored in the structure
 */
void TelescopeIO::TelescopeIOGetValue( DATAID_T Id, void* Data )
{
    switch ( TelescopeData[Id].DataFormat )
    {
        case UINT8_2:
        {
            sscanf ( TelescopeData[Id].Data, "%2d", ((int*)Data) );
            break;
        }
        case UINT16_4:
        {
            sscanf ( TelescopeData[Id].Data, "%4d", ((int*)Data) );
            break;
        }
        case FLOAT:
        case DOUBLE:
        {
            sscanf ( TelescopeData[Id].Data, "%f", ((float*)Data) );
            break;
        }
        default:
        {
            // ignore it.
        }
    }
}