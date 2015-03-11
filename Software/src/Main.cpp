/*
 * Author and Copyright of this file and of the telescope manager:
 * Chris Dick, 2015
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "server_astro_pi.h"
#include "LogFile.hpp"
#include <signal.h>

#include <iostream>
using namespace std;


static volatile bool continue_looping = true;

static void signal_handler(int signum)
{
    switch (signum)
    {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            continue_looping = false;
            break;
        default:
            // just ignore
            break;
    }
}


int main(int argc, char *argv[])
{
//    cout << "This is " << argv[0] << ", built at "
//         << __DATE__ << ", " << __TIME__ << endl;
    // SIGPIPE is normal operation when we send while the other side
    // has already closed the socket. We must ignore it:
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);
    // maybe the user wants to continue after SIGHUP ?
    //signal(SIGHUP,signal_handler);

    int port;
    if ((argc < 2 || argc > 4) ||
        1 != sscanf(argv[1], "%d", &port) ||
        port < 0 || port > 0xFFFF)
    {
        cout << "Usage: " << argv[0] << " port [ignored_arg logfile]" << endl;
        return 126;
    }
    if (argc > 3)
    {
        SetLogFile(argv[3]);
        *log_file << Now() << "This is " << argv[0] << ", built on "
                  << __DATE__ << ", " << __TIME__ << endl;
    }
    
    server_astro_pi server(port);
    while (continue_looping)
    {
        server.step(10000);
    }

    *log_file << Now() << "bye." << endl;
    return 0;
}
