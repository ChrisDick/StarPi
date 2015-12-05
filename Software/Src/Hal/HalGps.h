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
        bool HalGpsInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the Latitude of the Telescope
     * @return double Latitude 
     */
        double HalGpsGetLatitude( void );
    /** Get the Latitude of the Telescope
     * @return double Longitude 
     */
        double HalGpsGetLongitude( void );
    /** Get the time in unix time
     * @return double time
     */
        double HalGpsGetTime( void );
    /** Get the height above ground in meters
     * @return double Height
     */
        double HalGpsGetHeight( void );
    /** Get the height above ground in kilometers.
     * @return double Height
     */    
        double HalGpsGetHeightInkm( void );
       
        static HalGps Gps;
        
        private:
        static double Longitude;     /**< longitude of the telescope */
        static double Latitude;      /**< latitude of the telescope */
        static double Time;          /**< latest time */
        static double Height;        /**< height above sea level of the gps */
        static uint16_t GpsdPort;    /**< port to connect to GPSD */
};

#endif /* HALGPS_H */
