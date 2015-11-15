#ifndef HALCOMPASS_H
#define HALCOMPASS_H

#include <stdint.h>
#include "Runnable.h"

/** HalCompass
 * - Class to provide use to the magnetometer
 */
class HalCompass: public Runnable {
    public:
    /** Constructor
     */
        HalCompass( void );
    
    /** Initialise the Compass
     * @return bool Initialisation status  
     */
        bool HalCompassInit( void );
    /** Set the pitch of the Compass
     * @param NewPitch double  
     */
        void HalCompassSetPitch( double NewPitch );
    /** Set the roll of the Compass
     * @param NewRoll double  
     */
        void HalCompassSetRoll( double NewRoll );
    /** Runs the filter
     */
        void Run( void );
    /** Get the heading of the Compass
     * @return double heading 
     */
        double HalCompassGetHeading( void );
    /** Get the tilt compensated heading of the Compass
     *   see http://ozzmaker.com/2014/12/17/Compass2/#more-3062
     * @return double heading 
     */
        double HalCompassGetTiltCompensatedHeading( void );
        
        static HalCompass Compass;
        
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
        double Pitch;         /**< current pitch of the compass */
        double Roll;          /**< current roll of the compass */
};

#endif /* HALCOMPASS_H */
