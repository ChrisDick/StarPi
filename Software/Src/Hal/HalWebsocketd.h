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
#ifndef HALWEBSOCKETD_H
#define HALWEBSOCKETD_H

#include <stdint.h>
#include "Runnable.h"
        
#define QUEUESIZE  ((uint8_t)100u)
#define DATALENGTH ((uint8_t)50u)


/** HalWebsocketd
 * - Class to provide interface to websocketd
 */
class HalWebsocketd: public Runnable 
{
    public:
    /** Constructor
     */
        HalWebsocketd( void );
     /** Structure for the messages and Ids
     */   
        typedef struct 
        {
            char Data[DATALENGTH+1];         /**< data */
            uint8_t Id;                      /**< data Id */
        } MESSAGE_T;
    /** Structure for the queues.  
     */        
        typedef struct
        {
            MESSAGE_T Message[QUEUESIZE];    /**< data queue*/     
            uint8_t ReadIndex;               /**< Read location in queue */
            uint8_t WriteIndex;              /**< Write location in queue */
            uint8_t FillLevel;               /**< Number of items currently queued */
        } MESSAGEQUEUE_T;
    /** Initialise the GPSD connection
     * @return bool Initialisation status  
     */
        void HalWebsocketdInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** ThreadRun
     *  Check for any new data store in the input queue
     */
        void ThreadRun( void );
    /** Add a message to the queue
     * @param Message pointer to the message to be altered or added to the send queue.
     * @param Id identifier for the message.
     * @return bool true if item is successfully added to the queue
     */
        bool HalWebsocketdSendMessage( char* Message, uint8_t Id );
    /** Get the next Message in the queue
     * @param Message pointer to the message to be altered or added to the send queue.
     */
        bool HalWebsocketdGetMessage( char* Message );

        static HalWebsocketd Websocket; /**< We only want one object handling any stdio. */
        
    /** Is the queue empty
     * @param Queue pointer to the queue to check
     * @return bool_t true if empty
     */
        bool HalWebsocketdQueueEmpty( MESSAGEQUEUE_T* Queue );    
    /** Is the queue full
     * @param Queue pointer to the queue to check
     * @return bool_t true if full
     */
        bool HalWebsocketdQueueFull( MESSAGEQUEUE_T* Queue );    
    /** Get the next message
     * @param Queue pointer to the queue to retrieve from
     * @param Message pointer to storage fro message
     * @return bool_t true if successful
     */
        bool HalWebsocketdGetNextMessage( MESSAGEQUEUE_T* Queue, char* Message );   
    /** Get the next message
     * @param Queue pointer to the queue to retrieve from
     * @param Message pointer to storage fro message
     * @param Id identifier for the message.
     * @return bool_t true if successful
     */
        bool HalWebsocketdAddMessage( MESSAGEQUEUE_T* Queue, char* Message, uint8_t Id );
 
        private:
        MESSAGEQUEUE_T InputQueue;     /**< The Input queue  */
        MESSAGEQUEUE_T OutputQueue;    /**< The Output queue */
        char Buffer[DATALENGTH];
};

#endif /* HALWEBSOCKETD_H */
