/*
 * The stellarium telescope library helps building
 * telescope server programs, that can communicate with stellarium
 * by means of the stellarium TCP telescope protocol.
 * It also contains sample server classes (dummy, Meade LX200).
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

#include "Server.hpp"
#include "Socket.hpp"
#include "Listener.hpp"
 
/* Server 
 * Constructor
 */
//Server::Server( void )
//{
//}

/* Server
 * Constructor
 * @param Port 
 */
Server::Server( int16_t Port )
{
    Socket* ListenerPtr = new Listener( *this, Port );
    ListOfSockets.push_back( ListenerPtr );
}

/* ~Server
 * Destructor
 */     
Server::~Server( void )
{
}

/* Step
 * main run function
 * @param TimeoutMicros
 */
void Server::Step( int64_t TimeoutMicros )
{
	fd_set ReadFds, WriteFds;
    FD_ZERO(&ReadFds);
    FD_ZERO(&WriteFds);
    int16_t FdMax = -1;
    
    for (
         SocketList::const_iterator It( ListOfSockets.begin() );
         It != ListOfSockets.end();
         It++ 
         )
    {
        (*It)->PrepareSelectFds( ReadFds, WriteFds, FdMax);
    }
    
    struct timeval Tv;
    if (TimeoutMicros < 0)
    {
        TimeoutMicros = 0;
    }
    Tv.tv_sec = TimeoutMicros / 1000000;
    Tv.tv_usec = TimeoutMicros % 1000000;
    const int16_t SelectRc = select( FdMax+1, &ReadFds, &WriteFds, 0, &Tv );
    if (SelectRc > 0)
    {
        SocketList::iterator It( ListOfSockets.begin() );
        while ( It != ListOfSockets.end() )
        {
            (*It)->HandleSelectFds( ReadFds, WriteFds );
            if ( (*It)->IsClosed() )
            {
                SocketList::iterator Tmp( It );
                It++;
                delete ( *Tmp );
                ListOfSockets.erase( Tmp );
            }
            else
            {
                It++;
            }
        }
    }
}

/* SendPosition
 * Send the current position of the telescope to the client
 * @param RAInt uint32_t format of the Right Ascension
 * @param DecInt uint32_t format of the Declination
 * @param Status uint32_t system status
 */
void Server::SendPosition( uint32_t RAInt, int32_t DecInt, int32_t Status )
{
    for ( 
         SocketList::const_iterator It( ListOfSockets.begin() );
         It != ListOfSockets.end();
         It++
         )
    {
        (*It)->SendPosition( RAInt, DecInt, Status );
    }
}

/* AddConnection
 * Adds this object to the list of connections maintained by this server.
 * This method is called by Listener.
 * @param SocketPtr can be anything that inherits Socket, including Listener,
 * Connection or any custom class that implements a serial port
 * connection (such as Lx200Connection and NexStarConnection).
 */
void Server::AddConnection( Socket* SocketPtr )
{
    if ( SocketPtr )
    {
        ListOfSockets.push_back( SocketPtr );
    }
}


/* CloseAcceptedConnections
 * Close all connections in list
 */
void Server::CloseAcceptedConnections( void )
{
    for (
         SocketList::iterator It( ListOfSockets.begin() );
         It != ListOfSockets.end();
         It++
         )
    {
        if ( (*It)->IsTcpConnection() )
        {
            (*It)->HangUp();
        }
    }
}

/*  ~SocketList
 * Destructor
 */
Server::SocketList::~SocketList ( void )
{
    Clear();
}

/* Clear
 * Empty the list
 */
void Server::SocketList::Clear( void )
{
    for (const_iterator It(begin()); It != end(); It++)
    {
        delete (*It);
    }
    list<Socket*>::clear();
}
