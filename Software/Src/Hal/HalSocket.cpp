/**
server side code, which passes the received message to a callback.
Handles multiple socket connections with select and fd_set on Linux 
Author and copyright of this file:
Chris Dick, 2017

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
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
    
#include "HalSocket.h"
    
HalSocket          HalSocket::Socket;
int                HalSocket::master_socket;                           /**< master_socket */
int                HalSocket::client_socket[HAL_SOCKET_MAX_CLIENTS];   /**< client_socket */
struct sockaddr_in HalSocket::address;                                 /**< address */
void               (*HalSocket::callback)(char*);
/* HalSocket
 *  Constructor
 */
HalSocket::HalSocket( void )
{
    
}

/* Init
 *  
 */
void HalSocket::Init( uint16_t Port, void (*callback_function)(char*))
{  
    int opt = 1;  
    uint8_t i = 0u;
    HalSocket::callback = callback_function;
    /* initialise all client_socket[] to 0 so not checked */ 
    for (i = 0u; i < HAL_SOCKET_MAX_CLIENTS; i++)  
    {  
        client_socket[i] = 0u;  
    }  
        
    /* create a master socket */ 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    /* set master socket to allow multiple connections */
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    /* type of socket created */
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( Port );  
        
    /* bind the socket */
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", Port);  
        
    /* try to specify maximum of 3 pending connections for the master socket */
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    puts("Waiting for connections ...");  
}
/**
 */
void HalSocket::Run( void )
{  
    static char buffer[HAL_SOCKET_BUFFER_SIZE];
    int max_sd;  
    int16_t new_socket;
    int8_t activity;
    int valread;
    const int addrlen = sizeof(address);  
    int sd; 
    uint8_t i;  
    //set of socket descriptors 
    fd_set readfds;  
    //a message 
    const char* message = "StarPi! \r\n";  
    timeval timeout;
        
    //clear the socket set 
    FD_ZERO(&readfds);  
    
    //add master socket to set 
    FD_SET(master_socket, &readfds);  
    max_sd = master_socket;  
            
    //add child sockets to set 
    for ( i = 0 ; i < HAL_SOCKET_MAX_CLIENTS ; i++)  
    {  
        //socket descriptor 
        sd = client_socket[i];  
                
        //if valid socket descriptor then add to read list 
        if(sd > 0)  
        {
            FD_SET( sd , &readfds);  
        }                
        //highest file descriptor number, need it for the select function 
        if(sd > max_sd)  
        {
            max_sd = sd;  
        }  
    }    
    /* wait for an activity on one of the sockets */ 
    timeout.tv_sec = 0u;
    timeout.tv_usec = 500u;
    activity = select( max_sd + 1 , &readfds , NULL , NULL , &timeout);  
      
    if ((activity < 0) && (errno!=EINTR))  
    {  
        printf("select error");  
    }  
            
    //If something happened on the master socket , 
    //then its an incoming connection 
    if (FD_ISSET(master_socket, &readfds))  
    {  
        if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0 )  
        {  
            perror("accept");  
            exit(EXIT_FAILURE);  
        }  
            
        //inform user of socket number - used in send and receive commands 
        printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
              (address.sin_port));  
      
        //send new connection greeting message 
        //if( send(new_socket, message, strlen(message), 0) != (int)strlen(message) )  
        //{  
        //    perror("send");  
        //}  
            
        //puts("Welcome message sent successfully");  
            
        //add new socket to array of sockets 
        for (i = 0; i < HAL_SOCKET_MAX_CLIENTS; i++)  
        {  
            //if position is empty 
            if( client_socket[i] == 0 )  
            {  
                client_socket[i] = new_socket;  
                printf("Adding to list of sockets as %d\n" , i);  
                    
                break;  
            }
        }
    }
    else
    {
        //else its some IO operation on some other socket
        for (i = 0; i < HAL_SOCKET_MAX_CLIENTS; i++)  
        {
            sd = client_socket[i];  
            if ( sd != 0 )
            {
                if (FD_ISSET( sd , &readfds))  
                {
                    //Check if it was for closing , and also read the 
                    //incoming message 
                    if ((valread = read( sd , buffer, HAL_SOCKET_BUFFER_SIZE)) == 0)  
                    {  
                        //Somebody disconnected , get his details and print 
                        getpeername(sd , (struct sockaddr*)&address , \
                            (socklen_t*)&addrlen);  
                        printf("Host disconnected , ip %s , port %d \n" , 
                            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                            
                        //Close the socket and mark as 0 in list for reuse 
                        close( sd );  
                        client_socket[i] = 0;  
                    }
                    else
                    {
                        /* search the table for command and run callback */
                        HalSocket::callback( buffer );
                        //set the string terminating NULL byte on the end 
                        //of the data read 
                        //txbuffer[valread] = '\0';
                        if ( buffer[0] != NULL )
                        {
                            send(sd , buffer , strlen(buffer) , 0 );  
                        }
                    }
                }
            }
        }
    }
}

