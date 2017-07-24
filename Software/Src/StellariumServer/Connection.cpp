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

#include "Connection.hpp"
#include "Server.hpp"
#include <math.h>
//#include "LogFile.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

/* Connection
 * Constructor
 */
Connection::Connection( Server &server, SOCKET Fd ) : Socket( server, Fd )
{
    ReadBuffEnd = ReadBuff;
    WriteBuffEnd = WriteBuff;
    ServerMinusClientTime = 0x7FFFFFFFFFFFFFFFLL;
}


/* GetServerMinusClientTime
 * A get function for the difference in server times
 * @return int64_t the difference in times
 */
int64_t Connection::GetServerMinusClientTime(void) 
{
    return ServerMinusClientTime;
}

/* PerformReading
 * Receives data from a TCP/IP connection and stores it in the read buffer.
 */
void Connection::PerformReading( void )
{
    const uint16_t ToRead = ReadBuff + sizeof(ReadBuff) - ReadBuffEnd;
    const int16_t Rc = readNonblocking(ReadBuffEnd, ToRead);
    if (Rc < 0)
    {
        if (ERRNO == ECONNRESET)
        {
			//*log_file << Now() << "Connection::performReading: "
			//                      "client has closed the connection" << endl;
            HangUp();
        } 
        else if (ERRNO != EINTR && ERRNO != EAGAIN)
        {
			//*log_file << Now() << "Connection::performReading: readNonblocking failed: "
			//                   << STRERROR(ERRNO) << endl;
            HangUp();
        }
    } 
    else if (Rc == 0)
    {
        if (IsTcpConnection())
        {
			//*log_file << Now() << "Connection::performReading: "
			//                     "client has closed the connection" << endl;
            HangUp();
        }
    }
    else
    {    
        ReadBuffEnd += Rc;
        const uint8_t* BufferPtr = ReadBuff;
        DataReceived( BufferPtr, ReadBuffEnd);
        if (BufferPtr >= ReadBuffEnd)
        {
            // everything handled
            ReadBuffEnd = ReadBuff;
        }
        else if (BufferPtr > ReadBuff)
        {
            // partly handled
            memmove(ReadBuff, BufferPtr, ReadBuffEnd - BufferPtr);
            ReadBuffEnd -= (BufferPtr - ReadBuff);
        }
    }
}

/* PerformWriting
 * Sends the contents of the write buffer over a TCP/IP connection.
 */
void Connection::PerformWriting( void )
{
    const int16_t to_write = WriteBuffEnd - WriteBuff;
    const int16_t Rc = writeNonblocking(WriteBuff, to_write);
    if (Rc < 0)
    {
        if (ERRNO != EINTR && ERRNO != EAGAIN)
        {
			//*log_file << Now() << "Connection::performWriting: writeNonblocking failed: "
			//                   << STRERROR(ERRNO) << endl;
        HangUp();
        }
    }
    else if (Rc > 0)
    {
        if (Rc >= to_write)
        {
            // everything written
            WriteBuffEnd = WriteBuff;
        }
        else
        {
            // partly written
            memmove(WriteBuff, WriteBuff + Rc, to_write - Rc);
            WriteBuffEnd -= Rc;
        }
    }
}

/* PrepareSelectFds
 * Prepare the selected socket
 * @param ReadFds reference
 * @param WriteFds reference
 * @param FdMax reference
 */
void Connection::PrepareSelectFds( fd_set &ReadFds, fd_set &WriteFds, int16_t &FdMax )
{
    if (!IS_INVALID_SOCKET(Fd))
    {
        if (FdMax < (int16_t)Fd)
        {
            FdMax = (int16_t)Fd;
        }
        if (WriteBuffEnd > WriteBuff)
        {
            FD_SET(Fd, &WriteFds);
        }
        FD_SET(Fd, &ReadFds);
    }
}

/* HandleSelectFds
 * handle the selected rw files
 * @param ReadFds reference to the read file
 * @param WriteFds reference to the write file
 */
void Connection::HandleSelectFds( const fd_set &ReadFds, const fd_set &WriteFds )
{
    if (!IS_INVALID_SOCKET(Fd))
    {
        if (FD_ISSET(Fd, const_cast<fd_set *>(&WriteFds)))
        {
            PerformWriting();
        }
        if (!IS_INVALID_SOCKET(Fd) && FD_ISSET(Fd, const_cast<fd_set *>(&ReadFds)))
        {
            PerformReading();
        }
    }
}

    
/* DataReceived
 * Handle the received data
 * @param BufferPtr reference tot he buffer
 * @param ReadBuffEnd pointer to the end of the buffer
 */
void Connection::DataReceived( const uint8_t* &BufferPtr, const uint8_t *ReadBuffEnd )
{
    while ( (ReadBuffEnd - BufferPtr) >= 2 )
    {
        const uint16_t Size = (uint16_t)BufferPtr[0] + ((uint16_t)BufferPtr[1] << 8);
	//	const int Size = (int)( ((unsigned char)(BufferPtr[0])) |
	//	                        (((unsigned int)(unsigned char)(BufferPtr[1])) << 8) );

        if ( ( Size > (int16_t)sizeof(ReadBuff) ) || ( Size < 4 ) )
        {
			//*log_file << Now() << "Connection::dataReceived: "
		    //                          "bad packet size: " << Size << endl;
            HangUp();
            return;
        }
        if (Size > (ReadBuffEnd-BufferPtr) )
        {
            // wait for complete packet
            break;
        }
        const uint16_t type = (uint16_t)BufferPtr[2] + (((uint16_t)BufferPtr[3]) << 8);
        // dispatch:
        switch (type)
        {
            case 0:
            //A "go to" command
            {
                if (Size < 12)
                {
					//*log_file << Now() << "Connection::dataReceived: "
					//                      "type 0: bad packet size: " << Size << endl;
                    HangUp();
                    return;
                }
                const uint64_t ClientMicros = (int64_t)
                               ( ((uint64_t)(BufferPtr[ 4])) |
                                 ((uint64_t)(BufferPtr[ 5]) <<  8) |
                                 ((uint64_t)(BufferPtr[ 6]) << 16) |
                                 ((uint64_t)(BufferPtr[ 7]) << 24) |
                                 ((uint64_t)(BufferPtr[ 8]) << 32) |
                                 ((uint64_t)(BufferPtr[ 9]) << 40) |
                                 ((uint64_t)(BufferPtr[10]) << 48) |
                                 ((uint64_t)(BufferPtr[11]) << 56) );
                ServerMinusClientTime = GetNow() - ClientMicros;
                const uint32_t RAInt =
                                  ((uint32_t)(BufferPtr[12])) |
                                  ((uint32_t)(BufferPtr[13]) <<  8) |
                                  ((uint32_t)(BufferPtr[14]) << 16) |
                                  ((uint32_t)(BufferPtr[15]) << 24);
                const int32_t DecInt =
                        (int32_t)(((uint32_t)(BufferPtr[16])) |
                                  ((uint32_t)(BufferPtr[17]) <<  8) |
                                  ((uint32_t)(BufferPtr[18]) << 16) |
                                  ((uint32_t)(BufferPtr[19]) << 24) );
                server.GotoReceived(RAInt, DecInt);
            }
            break;
            
            default:
            {
                //No other types of commands are acceptable at the moment
				//*log_file << Now()
				//          << "Connection::dataReceived: "
				//             "ignoring unknown packet, type: "
				//          << type
				//          << endl;
                break;
            }
        }  
        BufferPtr += Size;
    }
    return;
}

/* SendPosition
 * Fill the buffer with the Position data
 * @param RAInt int_32_t version of the Right Acension
 * @param DecInt int32_t version of the declination
 * @param Status status of the server
 */
void Connection::SendPosition( uint32_t RAInt, int32_t DecInt, int32_t Status )
{
    if ( !IS_INVALID_SOCKET( Fd ) )
    {
        if ( ( WriteBuffEnd - WriteBuff + 24 ) < ( (uint16_t)sizeof( WriteBuff ) ))
        {
            // length of packet:
            *WriteBuffEnd++ = 24;
            *WriteBuffEnd++ = 0;
            // type of packet:
            *WriteBuffEnd++ = 0;
            *WriteBuffEnd++ = 0;
            // server_micros:
            //long long int Now = GetNow();
            int64_t Now = GetNow();
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now; Now>>=8;
            *WriteBuffEnd++ = Now;
            // ra:
            *WriteBuffEnd++ = RAInt; RAInt>>=8;
            *WriteBuffEnd++ = RAInt; RAInt>>=8;
            *WriteBuffEnd++ = RAInt; RAInt>>=8;
            *WriteBuffEnd++ = RAInt;
            // dec:
            *WriteBuffEnd++ = DecInt; DecInt>>=8;
            *WriteBuffEnd++ = DecInt; DecInt>>=8;
            *WriteBuffEnd++ = DecInt; DecInt>>=8;
            *WriteBuffEnd++ = DecInt;
            // Status:
            *WriteBuffEnd++ = Status; Status>>=8;
            *WriteBuffEnd++ = Status; Status>>=8;
            *WriteBuffEnd++ = Status; Status>>=8;
            *WriteBuffEnd++ = Status;
        }
        else
        {
			//*log_file << Now() << "Connection::sendPosition: "
			//                      "communication is too slow, I will ignore this command"
			//                   << endl;
        }
    }
}

