/*
    StarPi
    Copyright (C) 2018 Chris Dick

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

#pragma once

/** \file starpi.h
    \brief Construct an NDI Telescope device.
    \author Chris Dick

    \note For use with indi_starpi_sk.xml
*/

#include "defaultdevice.h"
#include "inditelescope.h"
//public INDI::DefaultDevice,
class StarPi :  public INDI::Telescope
{
  public:
    StarPi() = default;
    ~StarPi() = default;

    virtual void ISGetProperties(const char *dev);
    virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n);
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n);
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n);
    virtual bool ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[],
                           char *formats[], char *names[], int n);
    // Telescope specific functions
    bool ReadScopeStatus();
    bool Goto(double, double);
    bool Abort();
    void TimerHit();

 // protected:
 //   bool Handshake();
  private:
    const char *getDefaultName();
    virtual bool initProperties();
    virtual bool Connect();
    virtual bool Disconnect();
    bool SendMsg( char * buffer );
    int sockfd;
    double currentRA;
    double currentDEC;
    double targetRA;
    double targetDEC;
    bool socketOpen;
    bool calibrating;
    unsigned int DBG_SCOPE;
    int portno;
    char host[256];
};

