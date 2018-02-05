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
#ifndef HALSOCKET_H
#define HALSOCKET_H

#include <stdint.h>
#include "Runnable.h"

/* Configuration */

#define HAL_SOCKET_MAX_CLIENTS 30
#define HAL_SOCKET_BUFFER_SIZE 50
        
/** HalSocket
 * - Class to provide a socket
 */
class HalSocket: public Runnable 
{
    public:
    /** Constructor
     */
        HalSocket( void );
    /** Initialise
     * @return bool Initialisation status  
     */
        void Init( uint16_t Port, void (*callback_function)(char*) );
    /** Runs the filter
     */
        void Run( void );
    /** ThreadRun
     *  Check for any new data store in the input queue
     */

        static HalSocket Socket; /**< We only want one object handling any stdio. */
        static void (*callback)(char*);
    private:
        static int master_socket;                            /**<  */
        static struct sockaddr_in address;                   /**<  */
        static int client_socket[HAL_SOCKET_MAX_CLIENTS];    /**<  */
};

#endif /* HALSOCKET_H */
