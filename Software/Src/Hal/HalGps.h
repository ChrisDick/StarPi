/**
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
#ifndef HALGPS_H
#define HALGPS_H

#include <sys/time.h>
#include <stdint.h>
#include "Runnable.h"
#include "libgpsmm.h"
/** HalGps
 * - Class to provide functionality for the GPS
 */
class HalGps: public Runnable 
{
    //gpsmm gps_rec( "localhost", DEFAULT_GPSD_PORT);
    public:
    static gpsmm* gps_ptr;
    /** Constructor
     */
        HalGps( void );
    
    /** Initialise the GPSD connection
     * @return bool Initialisation status  
     */
        bool Init( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the Latitude of the Telescope
     * @return double Latitude 
     */
        double GetLatitude( void );
    /** Get the Latitude of the Telescope
     * @return double Longitude 
     */
        double GetLongitude( void );
    /** Get the time in unix time
     * @return time_t time
     */
        time_t GetTime( void );
    /** HalGpsGetMode
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
        uint8_t GetMode( void );

    /** HalGpsGetfix
     * Get the current fix status
     * @return bool fix 
     */
        bool GetFix( void );
    /** HalGpsGetNoOfSatellites
     * Get the number of satellites in view
     * @return uint8_t Number Of Satellites 
     */
        uint8_t GetNoOfSatellites( void );
    /** Get the height above ground in meters
     * @return double Height
     */
        double GetHeight( void );
    /** Get the height above ground in kilometers.
     * @return double Height
     */    
        double GetHeightInkm( void );
       
        static HalGps Gps;
        
        private:
        static double Longitude;           /**< longitude of the telescope */
        static double Latitude;            /**< latitude of the telescope */
        static time_t Time;                /**< latest time */
        static double Height;              /**< height above sea level of the gps */
        static uint8_t Mode;               /**< fix mode                         */
        static uint8_t NumberOfSatellites; /**< the number of satellites in view */
        static uint16_t GpsdPort;          /**< port to connect to GPSD */
};

#endif /* HALGPS_H */
