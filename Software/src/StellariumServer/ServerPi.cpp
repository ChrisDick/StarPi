/*
The stellarium telescope library helps building
telescope server programs, that can communicate with stellarium
by means of the stellarium TCP telescope protocol.
It also contains smaple server classes (dummy, Meade LX200).

Author and Copyright of the stellarium telescope library:
Johannes Gajdosik, 2006

Author and copyright of this file:
Chris Dick, 2015

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

#include "ServerPi.h"
#include "Socket.hpp" // GetNow
#include "TelescopeManager.h"
#include <math.h>

ServerPi::ServerPi(int port)
            :Server(port)
{
    next_pos_time = -0x8000000000000000LL;
}

/*
server->client:
MessageCurrentPosition (type = 0):

LENGTH (2 bytes,integer): length of the message
TYPE   (2 bytes,integer): 0
TIME   (8 bytes,integer): current time on the server computer in microseconds
           since 1970.01.01 UT. Currently unused.
RA     (4 bytes,unsigned integer): right ascension of the telescope (J2000)
           a value of 0x100000000 = 0x0 means 24h=0h,
           a value of 0x80000000 means 12h
DEC    (4 bytes,signed integer): declination of the telescope (J2000)
           a value of -0x40000000 means -90degrees,
           a value of 0x0 means 0degrees,
           a value of 0x40000000 means 90degrees
STATUS (4 bytes,signed integer): status of the telescope, currently unused.
           status=0 means ok, status<0 means some error
*/
void ServerPi::step(long long int timeout_micros, float ra, float dec)
{
    long long int now = GetNow();
    if (now >= next_pos_time)
    {
        next_pos_time = now + 500000;
        /*
            convert to integer format to send to stellarium
        */
        const unsigned int ra_int = (unsigned int)floor(
                                       0.5 +  ra*(((unsigned int)0x80000000)/M_PI));
        const int dec_int = (int)floor(0.5 + dec*(((unsigned int)0x80000000)/M_PI));
        const int status = 0;
        sendPosition(ra_int,dec_int,status);
    }
    Server::step(timeout_micros);
}

/*
client->server:
MessageGoto (type =0)
LENGTH (2 bytes,integer): length of the message
TYPE   (2 bytes,integer): 0
TIME   (8 bytes,integer): current time on the client computer in microseconds
                  since 1970.01.01 UT. Currently unused.
RA     (4 bytes,unsigned integer): right ascension of the telescope (J2000)
           a value of 0x100000000 = 0x0 means 24h=0h,
           a value of 0x80000000 means 12h
DEC    (4 bytes,signed integer): declination of the telescope (J2000)
           a value of -0x40000000 means -90degrees,
           a value of 0x0 means 0degrees,
           a value of 0x40000000 means 90degrees
*/
void ServerPi::gotoReceived(unsigned int ra_int, int dec_int)
{
    float ra_rads = 0; 
    float dec_rads = 0; 
    ra_rads = ra_int*(M_PI/(unsigned int)0x80000000);
    dec_rads = dec_int*(M_PI/(unsigned int)0x80000000);
    TelescopeManager::set_goto_target(ra_rads, dec_rads);
}


