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
    /** Get the time in 
     * @return double time
     */
        double HalGpsGetTime( void );
        
        static HalGps Gps;
        
        private:
        static double Longitude;     /**< longitude of the telescope */
        static double Latitude;      /**< latitude of the telescope */
        static double Time;          /**< latest time */
        static uint16_t GpsdPort;    /**< port to connect to GPSD */
};

#endif /* HALGPS_H */
