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
/** Class handler for the sockets
 */
class Socket
{
    public:
/** Destructor
 */
        virtual ~Socket( void ) { HangUp(); }
/** Close connection
 */
        void HangUp( void );
    /** PrepareSelectFds
     * Performs TCP/IP communication and handles new connections.
     * If a new connection is established, creates a new Connection object
     * and passes it to the parent Server with Server::addConnection().
     * @param ReadFds reference
     * @param WriteFds reference
     * @param FdMax reference
     */
        virtual void PrepareSelectFds(fd_set &ReadFds, fd_set &WriteFds, int16_t &FdMax) = 0;
    /** HandleSelectFds
     * handle the selected rw files
     * @param ReadFds reference to the read file
     * @param WriteFds reference to the write file
     */
        virtual void HandleSelectFds(const fd_set &ReadFds, const fd_set &WriteFds) = 0;
    /** Check status of socket
     */
        virtual bool IsClosed( void )
        {
            return IS_INVALID_SOCKET(Fd);
        }
    /** Check to see if the connection is a TCP socket.
     */    
        virtual bool IsTcpConnection(void) const { return false; }
    /** SendPosition
     * Composes a "MessageCurrentPosition" in the write buffer.
     * This is a Stellarium telescope control protocol message containing
     * the current right ascension, declination and status of the telescope mount.
     * @param RAInt uint32_T version of the Right Ascension
     * @param DecInt int32_t version of the Declination
     * @param Status int32_t version of the 
     */
        virtual void SendPosition( uint32_t /* RAInt */, int32_t /* DecInt */, int32_t /* Status */) {}
        
    protected:
    /** Constructor
     * @param server object for the server class
     * @param Fd file id for the socket
     */
        Socket(Server &server, SOCKET Fd) : server(server), Fd(Fd) {}
        Server & server; /**< reference tot he server */
        
    /** perform a non blocking read of the socket.
     * @param buf pointer tot he data buffer
     * @param count length of read.
     */
        int readNonblocking(void *buf, int count)
        {
            return read(Fd, buf, count);
        }
    /** Perform a non blocking write.
     * @param buf pointer tot he data buffer
     * @param count length of read.
     */
        int writeNonblocking(const void *buf, int count) {
            return write(Fd, buf, count);
        }
    
    protected:
        SOCKET Fd; /**< instance of the socket */
        
    private:
        // no copying
        Socket(const Socket&); 
        const Socket &operator=(const Socket&);
};

#endif /* SOCKET_HPP */
