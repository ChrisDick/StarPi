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
#include <math.h>
#include <stdio.h> // for debug

#include "ServerPi.h"
#include "Socket.hpp" // GetNow
#include "TelescopeManager.h"
#include "Config.h"

#ifdef TIMING
#include "GPIO.h"
#endif

/* Constructor
*/
ServerPi::ServerPi(int Port)
            :Server(Port)
{
#if 0
    current_pos[0] = desired_pos[0] = 1.0;
    current_pos[1] = desired_pos[1] = 0.0;
    current_pos[2] = desired_pos[2] = 0.0;
#endif

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
void ServerPi::Step(int64_t TimeoutMicros)
{
#if 0
    int64_t now = GetNow();
    if (now >= next_pos_time)
    {
        next_pos_time = now + 500000;
        current_pos[0] = 3*current_pos[0] + desired_pos[0];
        current_pos[1] = 3*current_pos[1] + desired_pos[1];
        current_pos[2] = 3*current_pos[2] + desired_pos[2];
        double h = current_pos[0]*current_pos[0]
                 + current_pos[1]*current_pos[1]
                + current_pos[2]*current_pos[2];
        if (h > 0.0)
        {
            h = 1.0 / sqrt(h);
            current_pos[0] *= h;
            current_pos[1] *= h;
            current_pos[2] *= h;
        }
        else
        {
            current_pos[0] = desired_pos[0];
            current_pos[1] = desired_pos[1];
            current_pos[2] = desired_pos[2];
        }
        
        const double ra = atan2(current_pos[1],current_pos[0]);
        const double dec = atan2(current_pos[2],
                                 sqrt(current_pos[0]*current_pos[0]+current_pos[1]*current_pos[1]));
        const uint32_t ra_int = (uint32_t)floor(
                                       0.5 +  ra*(((uint32_t)0x80000000)/M_PI));
        const int32_t dec_int = (int32_t)floor(0.5 + dec*(((uint32_t)0x80000000)/M_PI));
        const int32_t status = 0;
        SendPosition(ra_int,dec_int,status);
    }
    
    Server::Step(TimeoutMicros);
#endif
    int64_t now = GetNow();
    if (now >= next_pos_time)
    {
        next_pos_time = now + 500000;
        /*
            convert to integer format to send to stellarium
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
// ToDo; check this, doesn't match the above.
//        const unsigned int ra_int = (unsigned int)floor(
//                                       (0.5 +  RightAscension)*(((unsigned int)0x80000000)/M_PI));
//        const int dec_int = (int)floor((0.5 + Declination)*(((unsigned int)0x80000000)/M_PI));
        const unsigned int ra_int = (unsigned int)((RightAscension/(2.0*M_PI))*0xFFFFFFFF);
        const int dec_int = (int)((Declination/(M_PI/2.0))*1073741824.0);
        const int status = 0;
        SendPosition(ra_int,dec_int,status);
    }
    Server::Step(TimeoutMicros);
}

void ServerPi::SetRaDec (double Ra, double Dec )
{
    RightAscension = Ra;
    Declination = Dec;
}

void ServerPi::Run( void )
{
    #ifdef TIMING
    GPIO::gpio.SetPinState( SERVER_PI_PIN , true );
    #endif

    TelescopeManager::GetRaDec( &RightAscension, &Declination );
    //SetRaDec( RightAscension, Declination );
    Step( 10000 );

    #ifdef TIMING
    GPIO::gpio.SetPinState( SERVER_PI_PIN , false );
    #endif
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
void ServerPi::GotoReceived(uint32_t ra_int, int32_t dec_int)
{
    double ra_rads = 0; 
    double dec_rads = 0; 
    ra_rads = ra_int*(M_PI/(unsigned int)0x80000000);
    dec_rads = dec_int*(M_PI/(unsigned int)0x80000000);
    TelescopeManager::SetGotoTarget(ra_rads, dec_rads);
#if 0
    const double ra = ra_int*(M_PI/(uint32_t)0x80000000);
    const double dec = dec_int*(M_PI/(uint32_t)0x80000000);
    const double cdec = cos(dec);
    desired_pos[0] = cos(ra)*cdec;
    desired_pos[1] = sin(ra)*cdec;
    desired_pos[2] = sin(dec);
#endif
}


