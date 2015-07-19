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

#include "ServerPi.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include "HalAccelerometer.h"
#include "HalCompass.h"
#include "TelescopeManager.h"
#include "TTC_Sched_Pi_Impl.h"

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
        {
           continue_looping = false;

            printf ("End.\n");
            break;
        } 
        default:
    //        just ignore
        break;
    }
}

int main(int argc, char *argv[])
{
    //cout << "This is " << argv[0] << ", built at "
    //     << __DATE__ << ", " << __TIME__ << endl;
    // SIGPIPE is normal operation when we send while the other side
    // has already closed the socket. We must ignore it:

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);

    // maybe the user wants to continue after SIGHUP ?
    signal(SIGHUP,signal_handler);

    int Port;
    if ((argc < 2 || argc > 4) ||
        1 != sscanf(argv[1], "%d", &Port) ||
        Port < 0 || Port > 0xFFFF)
    {
        cout << "Usage: " << argv[0] << " port" << endl;
        return 126;
    }
    
    TTC_Sched_Pi_Impl   Scheduler;
    ServerPi PiServer( Port );
    
    Scheduler.Init();   // call first to reset task table and configure timer.
    printf ("scheduler initialised.\n");
    HalAccelerometer::Accelerometer.HalAccelerometerInit();
    HalAccelerometer::Accelerometer.SetDelay(0);    // no offset is set here.
    HalAccelerometer::Accelerometer.SetPeriod(4);   // run every 4 ticks (1 tick == 500us).
    
    HalCompass::Compass.HalCompassInit();
    HalCompass::Compass.SetDelay(1); // run one tick after Reader run.
    HalCompass::Compass.SetPeriod(4); // also run every 4 ticks.
       
    TelescopeManager::Telescope.SetDelay(0); 
    TelescopeManager::Telescope.SetPeriod(200);
    
    PiServer.SetDelay(1);
    PiServer.SetPeriod(10);
    
    printf ("tasks configured.\n");

    Scheduler.AddTask(&PiServer);
    Scheduler.AddTask(&HalAccelerometer::Accelerometer);
    Scheduler.AddTask(&TelescopeManager::Telescope);
    uint8_t error = Scheduler.AddTask(&HalCompass::Compass);    
    printf ("tasks added = %d.\n", error);
    
    Scheduler.Start();

    printf ("scheduler started.\n");
    
    /* Do busy work. */
    while (continue_looping)
    {
        Scheduler.DispatchTasks();
    }
    
    printf ("End.\n");
}
