#if 0
starpi indi driver
Copyright (C) 2018 Chris Dick

This library is free software;
you can redistribute it and / or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation;
either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY;
without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library;
if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301  USA

#endif

/** \file starpi.cpp
    \brief Construct an INDI Telescope device.
    \author Chris Dick

    \note For use with indi_starpi_sk.xml
*/

#include "starpi.h"
#include "config.h"
#include "indicom.h"
#include "indidriver.h"
#include "connectiontcp.h"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <sys/stat.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


/* Our auto pointer */
std::unique_ptr<StarPi> starpi(new StarPi());

//const int POLLMS = 500; /* Period of update, 1 second. */

/**************************************************************************************
**
***************************************************************************************/
void ISGetProperties(const char *dev)
{
    starpi->ISGetProperties(dev);
}

/**************************************************************************************
**
***************************************************************************************/
void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    starpi->ISNewSwitch(dev, name, states, names, n);
}

/**************************************************************************************
**
***************************************************************************************/
void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    starpi->ISNewText(dev, name, texts, names, n);
}

/**************************************************************************************
**
***************************************************************************************/
void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    starpi->ISNewNumber(dev, name, values, names, n);
}

/**************************************************************************************
**
***************************************************************************************/
void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[],
               char *names[], int n)
{
    starpi->ISNewBLOB(dev, name, sizes, blobsizes, blobs, formats, names, n);
}
/**************************************************************************************
**
***************************************************************************************/
void ISSnoopDevice(XMLEle *root)
{
    INDI_UNUSED(root);
}

/**************************************************************************************
** Initialize all properties & set default values.
**************************************************************************************/
bool StarPi::initProperties()
{
    setVersion(STARPI_VERSION_MAJOR, STARPI_VERSION_MINOR);
    INDI::Telescope::initProperties();
    /*
         This is the default driver skeleton file location
         Convention is: drivername_sk_xml
         Default location is /usr/share/indi
    */
    const char *skelFileName = "/usr/share/indi/indi_starpi_sk.xml";
    struct stat st;

    char *skel = getenv("INDISKEL");

    if (skel != nullptr)
    {
        buildSkeleton(skel);
    }
    else if (stat(skelFileName, &st) == 0)
    {
        buildSkeleton(skelFileName);
    }
    else
    {
        IDLog(
            "No skeleton file was specified. Set environment variable INDISKEL to the skeleton path and try again.\n");
    }
#if 0
    std::vector<INDI::Property *> *pAll = getProperties();
    /* Let's print a list of all device properties */
    for (int i = 0; i < (int)pAll->size(); i++)
    {
        IDLog("Property #%d: %s\n", i, pAll->at(i)->getName());
    }
#endif
    ITextVectorProperty *tvp = getText((char*)"DEVICE_TCP_ADDRESS");
    if ( tvp != NULL )
    {
        if (( strcmp(tvp->tp[0].text, "ADDRESS" ) != 0 )
          &&(strcmp(tvp->tp[0].text, "PORT" ) != 0 ))
        {
        }
    }
    
            strcpy(host, "localhost");
            portno = 9999;
    
    return true;
}

/**************************************************************************************
** Define Basic properties to clients.
***************************************************************************************/
void StarPi::ISGetProperties(const char *dev)
{
    static int configLoaded = 0;

    /* Ask the default driver first to send properties. */
    INDI::Telescope::ISGetProperties(dev);
   
    /* If no configuration is load before, then load it now. */
    if (configLoaded == 0)
    {
        loadConfig();
        configLoaded = 1;
    }
        
}

/**************************************************************************************
** Process Text properties
***************************************************************************************/
bool StarPi::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    bool result;   
    ITextVectorProperty *tvp = getText(name);
    /* Ignore if not ours */
    if (dev != nullptr && strcmp(dev, getDeviceName()) != 0)
    {
        result = false;
    }
    else
    {
        IUUpdateText( tvp, texts, names, n);
        IDSetText(tvp, nullptr);
        result = true;
    }
    return result;
}

/**************************************************************************************
**
***************************************************************************************/
bool StarPi::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    bool result = false;
    INumberVectorProperty *nvp = getNumber(name);
    /* Ignore if not ours */
    if (dev != nullptr && strcmp(dev, getDeviceName()) != 0)
    {
        result = false;
    }
    else if (nvp == nullptr)
    {
        result = false;
    }
    else
    {
        if (!isConnected())
        {
            nvp->s = IPS_ALERT;
            IDSetNumber(nvp, "Cannot change property while device is disconnected.");
            result = false;
        }
        else if ( strncmp( nvp->name, "ACCEL_OFF", 9 ) == 0)
        {
            /* Set the offset for the Pitch */
            char buffer[256];
            float temp = values[0];
            sprintf( buffer, "ACCO=%f#", temp);
            if ( SendMsg ( buffer ) )
            {
                float Offset = 0.0f;
                if ( sscanf( buffer, "ACCO=%f#", &Offset ) == 1 )
                {
                    nvp->np[0].value = Offset;
                    nvp->s = IPS_OK;
                    result = true;
                }
                else
                {
                    nvp->s = IPS_ALERT;
                    result = false;
                }
                IUUpdateNumber(nvp, values, names, n);
                IDSetNumber(nvp, nullptr);
            }
        }
        else if ( strncmp(nvp->name, "MAG_OFF", 7) == 0 )
        {
            /* Set the offset for the heading */
            char buffer[256];
            float temp = values[0];
            sprintf( buffer, "MAGO=%f#", temp );
            if ( SendMsg ( buffer ) )
            {
                float Offset = 0.0f;
                if ( sscanf( buffer, "MAGO=%f#", &Offset ) == 1 )
                {
                    nvp->np[0].value = Offset;
                    nvp->s = IPS_OK;
                    result = true;
                }
                else
                {
                    nvp->s = IPS_ALERT;
                    result = false;
                }
                IUUpdateNumber(nvp, values, names, n);
                IDSetNumber(nvp, nullptr);
            }
        }
        else
        {
            IUUpdateNumber(nvp, values, names, n);
            nvp->s = IPS_OK;
            IDSetNumber(nvp, nullptr);
            result = true;
        }  
    }
    return ( result );
}

/**************************************************************************************
**
***************************************************************************************/
bool StarPi::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    bool result = false;
    int lightState = 0;
    int lightIndex = 0;
    ISwitchVectorProperty *svp = nullptr;
    char buffer[256];
    /* ignore if not ours */
    if ( ( dev != nullptr ) && ( strcmp(dev, getDeviceName()) != 0) )
    {
        result = false;
    }
    else if (INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n))
    {
        result = true;
    }
    else
    {
        svp = getSwitch(name);
        if (svp == nullptr)
        {
            result = false;
        }
        else if (!isConnected())
        {
            svp->s = IPS_ALERT;
            IDSetSwitch(svp, "Cannot change property while device is disconnected.");
            result = false;
        }
        else
        {
            if (strcmp(svp->name, "CAL_ON") == 0)
            {
                uint8_t index = IUFindOnSwitchIndex(svp);
                
                IUUpdateSwitch(svp, states, names, n);
                    
                if( index == 0u )
                {                
                    /* turn off calibration */
                    strcpy ( buffer, "CALE=Disable#" );
                    if ( SendMsg( buffer) )
                    {
                        if ( strncmp( buffer, "CALE=Disabled", 13 ) == 0 )
                        {
                            svp->s = IPS_IDLE;
                            calibrating = false;
                            result = true;
                        }
                        else
                        {
                            svp->s = IPS_ALERT;
                            calibrating = false;
                            result = false;
                        }
                    }
                    else
                    {
                        svp->s = IPS_ALERT;
                        calibrating = false;
                        result = false;
                    }
                }
                else if (index == 1u )
                {
                    /* turn on calibration */
                    strcpy ( buffer, "CALE=Enable#" );
                    if ( SendMsg( buffer) )
                    {
                        if ( strncmp( buffer, "CALE=Enabled", 12 ) == 0 )
                        {
                            svp->s = IPS_OK;
                            calibrating = true;
                            result = true;
                        }
                        else
                        {
                            svp->s = IPS_ALERT;
                            calibrating = false;
                            result = false;
                        }
                    }
                    else
                    {
                        svp->s = IPS_ALERT;
                        calibrating = false;
                        result = false;
                    }
                    svp->s = IPS_OK;
                    result = true;
                }
                else
                {
                    /* shouldn't be here */
                    svp->s = IPS_ALERT ;
                    result = false;
                }
                IDSetSwitch( svp, nullptr );
            }
            else if (strcmp(svp->name, "RESET") == 0)
            {
                uint8_t index = IUFindOnSwitchIndex(svp);
                IUUpdateSwitch(svp, states, names, n);
                    
                if( index == 0u )
                {                
                    /* reset magnetometer */
                    strcpy ( buffer, "MULT#MiMxReset#MaMxReset#MiMyReset#MaMyReset#MiMzReset#MaMzReset#" );
                    if ( SendMsg( buffer) )
                    {
                        svp->s = IPS_OK;
                        result = true;
                    }
                    else
                    {
                        svp->s = IPS_ALERT;
                        result = false;
                    }
                }
                else if (index == 1u )
                {
                    /* Reset Accelerometer */
                    strcpy ( buffer, "MULT#MiAxReset#MaAxReset#MiAyReset#MaAyReset#MiAzReset#MaAzReset#" );
                    if ( SendMsg( buffer) )
                    {
                        svp->s = IPS_OK;
                        result = true;
                    }
                    else
                    {
                        svp->s = IPS_ALERT;
                        calibrating = false;
                        result = false;
                    }
                }                
                else
                {
                    /* shouldn't be here */
                    svp->s = IPS_ALERT ;
                    result = false;
                }
                IDSetSwitch( svp, nullptr );
            }
            
            else
            {
                /* switch not supported */
            }
        }
    }
    return result;
}

bool StarPi::ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[],
                               char *formats[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) != 0)
        return false;

    IBLOBVectorProperty *bvp = getBLOB(name);

    if (bvp == nullptr)
        return false;

    if (!isConnected())
    {
        bvp->s = IPS_ALERT;
        IDSetBLOB(bvp, "Cannot change property while device is disconnected.");
        return false;
    }

    if (strcmp(bvp->name, "BLOB Test") == 0)
    {
        IUUpdateBLOB(bvp, sizes, blobsizes, blobs, formats, names, n);

        IBLOB *bp = IUFindBLOB(bvp, names[0]);

        if (bp == nullptr)
            return false;

        IDLog("Received BLOB with name %s, format %s, and size %d, and bloblen %d\n", bp->name, bp->format, bp->size,
              bp->bloblen);

        char *blobBuffer = new char[bp->bloblen + 1];
        strncpy(blobBuffer, ((char *)bp->blob), bp->bloblen);
        blobBuffer[bp->bloblen] = '\0';

        IDLog("BLOB Content:\n##################################\n%s\n##################################\n",
              blobBuffer);

        delete[] blobBuffer;

        bp->size = 0;
        bvp->s   = IPS_OK;
        IDSetBLOB(bvp, nullptr);
    }

    return true;
}

/**************************************************************************************
**
***************************************************************************************/
bool StarPi::Connect()
{
    /* open port to connect to StarPi */
    /* todo make stuff configurable */
//    int portno = 9999;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    socketOpen = true;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        IDLog("ERROR opening socket");
        socketOpen = false;
    }
    server =  gethostbyname(host);
  //  server =  gethostbyname("localhost");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    {
        IDLog("ERROR connecting\n");
        socketOpen = false;
    }    
    if ( socketOpen )
    {
        /* setup timer */
        SetTimer(200);
    }    
    return socketOpen;
}

bool StarPi::Disconnect()
{
    /* close port to StarPi */
    if (socketOpen)
    {
        close(sockfd);
    }
    return true;
}

const char *StarPi::getDefaultName()
{
    return "StarPi";
}

void StarPi::TimerHit()
{
    /* cycle through data */
    /* ToDo just send 1 big message to read all the data at once to increase update frequency */
    static uint8_t DataType = 0u;
    char buffer[256];
    switch ( DataType )
    {
        case 0:
        {
            /* RAW DATA */
            strcpy ( buffer, "MULT#Pitc#Roll#MagH#MagD#Azim#" );
            break;
        }
        
        case 2:
        {
            /* RAW_VALUES */
            strcpy ( buffer, "MULT#RwAx#RwAy#RwAz#RwMx#RwMy#RwMz#" );
            break;
        }
        
        case 3:
        {
            /* MAG_CAL */
            strcpy ( buffer, "MULT#MiMx#MaMx#MiMy#MaMy#MiMz#MaMz#" );
            break;
        }
        
        case 4:
        {
            /* ACCEL_CAL */
            strcpy ( buffer, "MULT#MiAx#MaAx#MiAy#MaAy#MiAz#MaAz#" );
            break;
        }
        
        default:
        {
            /* Every other read is for position */
            strcpy ( buffer, "MULT#RA  #DEC #" );
            break;
        }
    }
    
    if ( SendMsg( buffer ) )
    {
        switch ( DataType )
        {
            case 0:
            {
                float Pitch;
                float Azimuth;
                float Roll;
                float MagDec;
                float MagHead;
                if ( sscanf( buffer, "MULT#Pitc=%f#Roll=%f#MagH=%f#MagD=%f#Azim=%f#", &Pitch, &Roll, &MagHead, &MagDec, &Azimuth ) == 5 )
                {
                    INumberVectorProperty *RawData = nullptr;
                    RawData = starpi->getNumber("RAW DATA");
                    RawData->np[0].value = Pitch;
                    RawData->np[1].value = Roll;
                    RawData->np[2].value = MagHead;
                    RawData->np[3].value = MagDec;
                    RawData->np[4].value = Azimuth;
                    RawData->s = IPS_OK;
                    IDSetNumber( RawData, NULL );

                    INumberVectorProperty *HorzCoord = nullptr;
                    HorzCoord = starpi->getNumber("HORIZONTAL_COORD");
                    HorzCoord->np[0].value = Pitch;
                    HorzCoord->np[1].value = Azimuth;
                    HorzCoord->s = IPS_OK;
                    IDSetNumber( HorzCoord, NULL );
                    
                    INumberVectorProperty *CalPitch = nullptr;
                    CalPitch = starpi->getNumber("CALPITCH");
                    CalPitch->np[0].value = Pitch;
                    CalPitch->s = IPS_OK;
                    IDSetNumber( CalPitch, NULL );
                    
                    INumberVectorProperty *CalMag = nullptr;
                    CalMag = starpi->getNumber("CALMAG");
                    CalMag->np[0].value = MagHead;
                    CalMag->s = IPS_OK;
                    IDSetNumber( CalMag, NULL );
                    
                }
                break;
            }
            case 2:
            {
                /* RAW_VALUES */
                float RwAx;
                float RwAy;
                float RwAz;
                float RwMx;
                float RwMy;
                float RwMz;
                if ( sscanf( buffer, "MULT#RwAx=%f#RwAy=%f#RwAz=%f#RwMx=%f#RwMy=%f#RwMz=%f#", &RwAx, &RwAy, &RwAz, &RwMx, &RwMy, &RwMz) == 6 )
                {
                    INumberVectorProperty *RawValues = nullptr;
                    RawValues = starpi->getNumber("RAW_VALUES");
                    RawValues->np[0].value = RwAx;
                    RawValues->np[1].value = RwAy;
                    RawValues->np[2].value = RwAz;
                    RawValues->np[3].value = RwMx;
                    RawValues->np[4].value = RwMy;
                    RawValues->np[5].value = RwMz;
                    RawValues->s = IPS_OK;
                    IDSetNumber( RawValues, NULL );
                }
                break;
            }
            
            case 3:
            {
                /* MAG_CAL */
                float MiMx;
                float MaMx;
                float MiMy;
                float MaMy;
                float MiMz;
                float MaMz;
                if ( sscanf( buffer, "MULT#MiMx=%f#MaMx=%f#MiMy=%f#MaMy=%f#MiMz=%f#MaMz=%f#", &MiMx, &MaMx, &MiMy, &MaMy, &MiMz, &MaMz) == 6 )
                {
                    INumberVectorProperty *MagCal = nullptr;
                    MagCal = starpi->getNumber("MAG_CAL");
                    MagCal->np[0].value = MiMx;
                    MagCal->np[1].value = MaMx;
                    MagCal->np[2].value = MiMy;
                    MagCal->np[3].value = MaMy;
                    MagCal->np[4].value = MiMz;
                    MagCal->np[5].value = MaMz;
                    MagCal->s = IPS_OK;
                    IDSetNumber( MagCal, NULL );
                }
                break;
            }
            
            case 4:
            {
                /* ACCEL_CAL */
                float MiAx;
                float MaAx;
                float MiAy;
                float MaAy;
                float MiAz;
                float MaAz;
                if ( sscanf( buffer, "MULT#MiAx=%f#MaAx=%f#MiAy=%f#MaAy=%f#MiAz=%f#MaAz=%f#", &MiAx, &MaAx, &MiAy, &MaAy, &MiAz, &MaAz) == 6 )
                {
                    INumberVectorProperty *AccelCal = nullptr;
                    AccelCal = starpi->getNumber("ACCEL_CAL");
                    AccelCal->np[0].value = MiAx;
                    AccelCal->np[1].value = MaAx;
                    AccelCal->np[2].value = MiAy;
                    AccelCal->np[3].value = MaAy;
                    AccelCal->np[4].value = MiAz;
                    AccelCal->np[5].value = MaAz;
                    AccelCal->s = IPS_OK;
                    IDSetNumber( AccelCal, NULL );
                }
                break;
            }
            
            /* takes place of case 1 */
            default:
            {
                float tempra;
                float tempdec;
                if ( sscanf( buffer, "MULT#RA__=%f#DEC_=%f", &tempra, &tempdec) == 2 )
                {                
                    currentRA = tempra;
                    currentDEC = tempdec;
                    NewRaDec(currentRA, currentDEC);
                }
                break;
            }
        }
        DataType++;
        /* check if calibration is enabled */
        if ( calibrating )
        {
            /* only update calibration tab*/
            if ( DataType > 4u )
            {
                DataType = 1u;
            }
        }        
        else
        {
            /* update everything else */
            if ( DataType > 1u )
            {
                DataType = 0u;
            }
        }
    }
    
    /* Clean the buffer */
    bzero(buffer,256);
    SetTimer(200); /* This isn't updating at 200ms rate */
}

bool StarPi::SendMsg( char * buffer )
{
//    static bool expecting = false;
    bool expecting = false;
    static uint16_t timeout = 1000u;
    bool result = false;
    int16_t n;
    if ( !expecting )
    {
        n = write( sockfd, buffer, strlen( buffer ));
        if (n < 0) 
        {
            IDLog("ERROR writing to socket");
        }
        timeout = 1000u;
    }
    
    bzero(buffer,256);
    n = read( sockfd, buffer, 255 );
    if (n < 0) 
    {
         IDLog("ERROR reading from socket");
    }
    else if (n == 0)
    {
        timeout--;
        if ( timeout == 0u )
        {
            expecting = false;
        }
    }
    else 
    {
        expecting = false;
        result = true;
    }
    return ( result );
}

/**************************************************************************************
** Client is asking us to slew to a new position
***************************************************************************************/
bool StarPi::Goto(double ra, double dec)
{
    targetRA  = ra;
    targetDEC = dec;
    char RAStr[64]={0}, DecStr[64]={0};

    // Parse the RA/DEC into strings
    fs_sexa(RAStr, targetRA, 2, 3600);
    fs_sexa(DecStr, targetDEC, 2, 3600);

    // Mark state as slewing
    TrackState = SCOPE_SLEWING;

    // Inform client we are slewing to a new position
    LOGF_INFO("Slewing to RA: %s - DEC: %s", RAStr, DecStr);

    // Success!
    return true;
}

/**************************************************************************************
** Client is asking us to abort our motion
***************************************************************************************/
bool StarPi::Abort()
{
    return true;
}

/**************************************************************************************
** Client is asking us to report telescope status
***************************************************************************************/
bool StarPi::ReadScopeStatus()
{
#if 0
    static struct timeval ltv { 0, 0 };
    struct timeval tv { 0, 0 };
    double dt = 0, da_ra = 0, da_dec = 0, dx = 0, dy = 0;
    int nlocked;

    /* update elapsed time since last poll, don't presume exactly POLLMS */
    gettimeofday(&tv, nullptr);

    if (ltv.tv_sec == 0 && ltv.tv_usec == 0)
        ltv = tv;

    dt  = tv.tv_sec - ltv.tv_sec + (tv.tv_usec - ltv.tv_usec) / 1e6;
    ltv = tv;

    // Calculate how much we moved since last time
    da_ra  = SLEW_RATE * dt;
    da_dec = SLEW_RATE * dt;

    /* Process per current state. We check the state of EQUATORIAL_EOD_COORDS_REQUEST and act acoordingly */
    switch (TrackState)
    {
        case SCOPE_SLEWING:
            // Wait until we are "locked" into positon for both RA & DEC axis
            nlocked = 0;

            // Calculate diff in RA
            dx = targetRA - currentRA;

            // If diff is very small, i.e. smaller than how much we changed since last time, then we reached target RA.
            if (fabs(dx) * 15. <= da_ra)
            {
                currentRA = targetRA;
                nlocked++;
            }
            // Otherwise, increase RA
            else if (dx > 0)
                currentRA += da_ra / 15.;
            // Otherwise, decrease RA
            else
                currentRA -= da_ra / 15.;

            // Calculate diff in DEC
            dy = targetDEC - currentDEC;

            // If diff is very small, i.e. smaller than how much we changed since last time, then we reached target DEC.
            if (fabs(dy) <= da_dec)
            {
                currentDEC = targetDEC;
                nlocked++;
            }
            // Otherwise, increase DEC
            else if (dy > 0)
                currentDEC += da_dec;
            // Otherwise, decrease DEC
            else
                currentDEC -= da_dec;

            // Let's check if we recahed position for both RA/DEC
            if (nlocked == 2)
            {
                // Let's set state to TRACKING
                TrackState = SCOPE_TRACKING;

                LOG_INFO("Telescope slew is complete. Tracking...");
            }
            break;

        default:
            break;
    }

    char RAStr[64]={0}, DecStr[64]={0};

    // Parse the RA/DEC into strings
    fs_sexa(RAStr, currentRA, 2, 3600);
    fs_sexa(DecStr, currentDEC, 2, 3600);

    DEBUGF(DBG_SCOPE, "Current RA: %s Current DEC: %s", RAStr, DecStr);

//    NewRaDec(currentRA, currentDEC);
#endif
    return true;
}
