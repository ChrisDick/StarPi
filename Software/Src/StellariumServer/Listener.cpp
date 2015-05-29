/*
 * The stellarium telescope library helps building
 * telescope server programs, that can communicate with stellarium
 * by means of the stellarium TCP telescope protocol.
 * It also contains smaple server classes (dummy, Meade LX200).
 * 
 * Author and Copyright of this file and of the stellarium telescope library:
 * Johannes Gajdosik, 2006
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Listener.hpp"
#include "Connection.hpp"
#include "Server.hpp"
#include <iostream>
#include <stdlib.h> // exit
using namespace std;

/* Listener
 * Constructor
 */
Listener::Listener( Server &server, int16_t Port ) 
                  : Socket( server, INVALID_SOCKET ),  Port( Port ) 
{
}


/* Checks if the connection is closed.
 * @returns always false, because if Listener can't listen,
 * it causes the program to exit with code 127.
 */
bool Listener::IsClosed( void ) 
{
    return false;
}
    

/* PrepareSelectFds
 *  Prepares TCP/IP communication and tries to start listening.
 *  If it can't listen, it causes the program to exit with code 127.
 * @param ReadFds reference
 * @param WriteFds reference
 * @param FdMax reference
 */
void Listener::PrepareSelectFds( fd_set &ReadFds, fd_set &WriteFds, int16_t &FdMax )
{
    if ( IS_INVALID_SOCKET( Fd ) )
    {
	
        struct sockaddr_in SockAddr;
        Fd = socket( AF_INET, SOCK_STREAM, 0 );
        if ( IS_INVALID_SOCKET( Fd ) )
        {
            exit(127);
        }
        
        int16_t Yes = -1; // all bits set to 1
        if (0 != setsockopt(Fd,
                            SOL_SOCKET,
                            SO_REUSEADDR,
                            reinterpret_cast<const char*>(&Yes),
                            sizeof(int)))
        {
            exit(127);
        }
        
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_addr.s_addr = INADDR_ANY;
        SockAddr.sin_port = htons(Port);
        if (bind(Fd, (struct sockaddr*)(&SockAddr), sizeof(SockAddr)))
        {
            exit(127);
        }

        if (listen(Fd, 10))
        {
            exit(127);
        }        
    }
    else
    {
        if (FdMax < (int16_t)Fd)
            FdMax = (int16_t)Fd;
        FD_SET(Fd, &ReadFds);
    }
}

/* HandleSelectFds
 * Performs TCP/IP communication and handles new connections.
 * If a new connection is established, creates a new Connection object
 * and passes it to the parent Server with Server::addConnection().
 * @param ReadFds reference
 * @param WriteFds reference
 * @param FdMax reference
 */
void Listener::HandleSelectFds( const fd_set &ReadFds, const fd_set &WriteFds )
{
    if (
            ( !IS_INVALID_SOCKET(Fd) )
         && ( FD_ISSET(Fd, const_cast<fd_set *>(&ReadFds)))
        )
    {
        struct sockaddr_in ClientAddr;
        SOCKLEN_T Length = sizeof(ClientAddr);
        const SOCKET ClientSock = accept(Fd, (struct sockaddr*)&ClientAddr, &Length);
        
        if (IS_INVALID_SOCKET(ClientSock))
        {
            close(ClientSock);
        }
        else if (0 != SETNONBLOCK(ClientSock))
        {
            close(ClientSock);
        }
        else
        {
            server.AddConnection(new Connection(server, ClientSock));
        }
    }
}
