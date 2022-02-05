/*
HalGps provides an interface to retrive the time and location from 
the GPSD deamon.

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
#include "HalGps.h"
#include "Config.h"

#ifdef TIMING
#include "GPIO.h"
#endif

HalGps   HalGps::Gps;
double   HalGps::Longitude;          /**< longitude of the telescope       */
double   HalGps::Latitude;           /**< latitude of the telescope        */
time_t   HalGps::Time;               /**< latest time                      */
double   HalGps::Height;             /**< current height of the Gps        */
uint8_t  HalGps::Mode;               /**< fix mode                         */
uint8_t  HalGps::NumberOfSatellites; /**< the number of satellites in view */
uint16_t HalGps::GpsdPort;           /**< port to connect to GPSD          */
gpsmm* HalGps::gps_ptr;

/* HalGps
 *  Constructor
 */
HalGps::HalGps( void )
{
    // Todo allow networked GPS to be used through Config.h or params
    gps_ptr = new gpsmm("localhost", DEFAULT_GPSD_PORT);
}

/* HalGpsInit
 *  Initialise the GPSD connection
 * @return bool Initialisation status  
 */
bool HalGps::Init( void )
{
    bool result;
 
    result = ( gps_ptr->stream(WATCH_ENABLE|WATCH_JSON) != NULL);

#ifdef TIMING
    GPIO::gpio.SetupOutput( HAL_GPS_PIN );
    GPIO::gpio.SetPullMode( HAL_GPS_PIN , PULL_UP );
#endif

    return result;
}

/* HalGpsRun
 *  Check for any new data and update time and location
 */
void HalGps::Run( void )
{
    #ifdef TIMING
    GPIO::gpio.SetPinState( HAL_GPS_PIN , true );
    #endif
    /*
    Check to see if there is new data, then update if it's relevent.
    */
    struct gps_data_t* NewGpsData;

    if (gps_ptr->waiting(20))
    {
    if ((NewGpsData = gps_ptr->read()) != NULL) 
    {
        if (NewGpsData->set & TIME_SET)
        {
            Time = NewGpsData->fix.time.tv_sec;
        }
        if (NewGpsData->set & LATLON_SET)
        {
            Latitude  = NewGpsData->fix.latitude; 
            Longitude = NewGpsData->fix.longitude;
        }
        if ( NewGpsData->set & ALTITUDE_SET)
        {
            Height = NewGpsData->fix.altitude;
        }
        if ( NewGpsData->set & SATELLITE_SET) 
        {
            NumberOfSatellites =  NewGpsData->satellites_visible;
        }
        if ( NewGpsData->set & MODE_SET)
        {    
            Mode = NewGpsData->fix.mode;
        }
    }
    }
    #ifdef TIMING
    GPIO::gpio.SetPinState( HAL_GPS_PIN , false );
    #endif
}

/* HalGpsGetHeading
 *  Get the Latitude of the Telescope
 * @return double Latitude 
 */
double HalGps::GetLatitude( void )
{
    return Latitude;
}

/* HalGpsGetLongitude
 *  Get the Longitude of the Telescope
 * @return double Longitude 
 */
double HalGps::GetLongitude( void )
{
    return Longitude;
}

/* HalGpsGetTime
 *  Get the current Time
 * @return double Time 
 */
time_t HalGps::GetTime( void )
{
    return Time;
}

/* HalGpsGetMode
 *  Get the fix mode
 * 0 = invalid
 * 1 = GPS fix (SPS)
 * 2 = DGPS fix
 * 3 = PPS fix
 * 4 = Real Time Kinematic
 * 5 = Float RTK
 * 6 = estimated (dead reckoning) (2.3 feature)
 * 7 = Manual input mode
 * 8 = Simulation mode
 * @return uint8_t Mode 
 */
uint8_t HalGps::GetMode( void )
{
    return Mode;
}

/* HalGpsGetfix
 *  Get the current fix status
 * @return bool fix 
 */
bool HalGps::GetFix( void )
{
    /* ToDo: does this need to include more modes? */
    return ( ( Mode == 3 ) || ( Mode == 2 ) );
}

/* HalGpsGetNoOfSatellites
 *  Get the number of satellites in view
 * @return uint8_t Number Of Satellites 
 */
uint8_t HalGps::GetNoOfSatellites( void )
{
    return NumberOfSatellites;
}

/* HalGpsGetHeight
 *  Get the current Height in kilometers
 * @return double Hieght 
 */
double HalGps::GetHeight( void )
{
    return Height;
}

/* HalGpsGetHeightInkm
 *  Get the current Height in kilometers
 * @return double Hieght 
 */
double HalGps::GetHeightInkm( void )
{
    return (Height/1000.0);
}
