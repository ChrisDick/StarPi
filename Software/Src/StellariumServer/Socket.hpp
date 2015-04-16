/**
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

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> // strerror

#define ERRNO errno
#define SETNONBLOCK(s) fcntl(s,F_SETFL,O_NONBLOCK)
#define SOCKLEN_T socklen_t
#define SOCKET int
#define IS_INVALID_SOCKET(Fd) (Fd<0)
#define INVALID_SOCKET (-1)
#define STRERROR(x) strerror(x)

int64_t GetNow(void);

class Server;

class Socket
{
    public:
        virtual ~Socket( void ) { HangUp(); }
        void HangUp( void );
        virtual void PrepareSelectFds(fd_set &ReadFds, fd_set &WriteFds, int16_t &FdMax) = 0;
        virtual void HandleSelectFds(const fd_set &ReadFds, const fd_set &WriteFds) = 0;
        virtual bool IsClosed( void )
        {
            return IS_INVALID_SOCKET(Fd);
        }
        virtual bool IsTcpConnection(void) const { return false; }
        virtual void SendPosition( uint16_t RAInt, int32_t DecInt, int32_t Status) {}
        
    protected:
        Socket(Server &server, SOCKET Fd) : server(server), Fd(Fd) {}
        Server & server;
        
        int readNonblocking(void *buf, int count)
        {
            return read(Fd, buf, count);
        }
        int writeNonblocking(const void *buf, int count) {
            return write(Fd, buf, count);
        }
    
    protected:
        SOCKET Fd;
        
    private:
        // no copying
        Socket(const Socket&);
        const Socket &operator=(const Socket&);
};

#endif /* SOCKET_HPP */
