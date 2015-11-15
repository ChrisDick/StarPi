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

#ifndef SERVER_PI_H
#define SERVER_PI_H

#include "Server.hpp"
#include "Runnable.h"
#include "TelescopeManager.h"

/** Class Telescope server.
*/
class ServerPi : public Server, public Runnable
{
    public:
    /** Constructor
     */
        ServerPi(int Port);
    /** 
     */
        void Run ( void );
    /** 
     */
        void SetRaDec (double Ra, double Dec );
    /** perform one step of the server
     * @param timeout_micros
     * @param ra
     * @param dec
     */    
        void Step(int64_t TimeoutMicros);
    
private:
    /** handler for the goto message
     * @param ra_int
     * @param dec_int
     */
    void GotoReceived(uint32_t ra_int, int32_t dec_int);
    /**
     */

    int64_t next_pos_time; /**< variable to prevent over sending of the messages */
    double RightAscension;        /**< Right ascension */
    double Declination;           /**< Declination */
};

#endif /* SERVER_ASTRO_PI_H */
