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

/** Class Telescope server.
*/
class ServerPi : public Server
{
    public:
    /** Constructor
     */
        ServerPi(int port);
    /** perform one step of the server
     * @param timeout_micros
     * @param ra
     * @param dec
     */    
    void step(long long int timeout_micros, float ra, float dec);
    
private:
    /** handler for the goto message
     * @param ra_int
     * @param dec_int
     */
    void gotoReceived(unsigned int ra_int,int dec_int);
    long long int next_pos_time; /**< variable to prevent over sending of the messages */
};

#endif /* SERVER_ASTRO_PI_H */
