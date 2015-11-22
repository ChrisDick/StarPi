#ifndef HALMMAGNETOMETER_H
#define HALMMAGNETOMETER_H

#include <stdint.h>
#include "Runnable.h"

/** HalMagnetometer
 * - Class to provide use to the magnetometer
 */
class HalMagnetometer: public Runnable {
    public:
    /** Constructor
     */
        HalMagnetometer( void );
    
    /** Initialise the Compass
     * @return bool Initialisation status  
     */
        bool HalMagnetometerInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** Access to the Magnetometer data.
     */
        void HalMagnetometerGetAll( float* Mx, float* My, float* Mz );

        static HalMagnetometer Magneto;
        
        private:
    /** Get the raw X heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetXRawHeading( void );
    /** Get the raw Y heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetYRawHeading( void );
    /** Get the raw Z heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetZRawHeading( void );
        
        double FilterX[5];    /**< storage for X axis filter data*/
        double FilterY[5];    /**< storage for Y axis filter data*/
        double FilterZ[5];    /**< storage for Z axis filter data*/
        uint8_t FilterCount; /**< counter to keep track of where to store latest data */
        double Scaling;       /**< scaling for the raw data */
};

#endif /* HALMMAGNETOMETER_H */
