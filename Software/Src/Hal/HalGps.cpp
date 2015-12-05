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

HalGps   HalGps::Gps;
double   HalGps::Longitude;         /**< longitude of the telescope */
double   HalGps::Latitude;          /**< latitude of the telescope  */
double   HalGps::Time;              /**< latest time                */
double   HalGps::Height;            /**< current height of the Gps  */
uint16_t HalGps::GpsdPort;          /**< port to connect to GPSD    */
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
bool HalGps::HalGpsInit( void )
{
    bool result;
 
    result = ( gps_ptr->stream(WATCH_ENABLE|WATCH_JSON) != NULL);

    return result;
}

/* HalGpsRun
 *  Check for any new data and update time and location
 */
void HalGps::Run( void )
{    
/*
   Check to see if there is new data, then update if it's relevent.
*/
    struct gps_data_t* NewGpsData;

    if ((NewGpsData = gps_ptr->read()) != NULL) 
    {
        if (NewGpsData->set & TIME_SET)
        {
            Time = NewGpsData->fix.time;
        }
        if (NewGpsData->set & LATLON_SET)
        {
            Latitude  = NewGpsData->fix.latitude, 
            Longitude = NewGpsData->fix.longitude;
        }
        if ( NewGpsData->set & ALTITUDE_SET)
        {
            Height = NewGpsData->fix.altitude;
        }
    }
}

/* HalGpsGetHeading
 *  Get the Latitude of the Telescope
 * @return double Latitude 
 */
double HalGps::HalGpsGetLatitude( void )
{
    return Latitude;
}

/* HalGpsGetLongitude
 *  Get the Longitude of the Telescope
 * @return double Longitude 
 */
double HalGps::HalGpsGetLongitude( void )
{
    return Longitude;
}

/* HalGpsGetTime
 *  Get the current Time
 * @return double Time 
 */
double HalGps::HalGpsGetTime( void )
{
    return Time;
}

/* HalGpsGetHeight
 *  Get the current Height in kilometers
 * @return double Hieght 
 */
double HalGps::HalGpsGetHeight( void )
{
    return Height;
}

/* HalGpsGetHeightInkm
 *  Get the current Height in kilometers
 * @return double Hieght 
 */
double HalGps::HalGpsGetHeightInkm( void )
{
    return (Height/1000.0);
}
