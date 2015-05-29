#ifndef HAL_ACCELEROMETER_H
#define HAL_ACCELEROMETER_H

#include <stdint.h>
#include "Runnable.h"

/** HalAccelerometer
 * - Class to provide use of the Accelerometer
 */
class HalAccelerometer: public Runnable {
    public:
        /** Constructor
        */
            HalAccelerometer( void );
        /** Initialise the Accelerometer
         * @return Status initialisation (true = success)
         */    
            bool  HalAccelerometerInit( void );
        /** Get the Pitch of the Accelerometer
        * @return float The Pitch
        */
            float HalAccelerometerGetPitch( void );
        /** Get the Roll of the Accelerometer
         * @return float The Roll
         */
           float HalAccelerometerGetRoll( void );
        /** runs the filter and updates the Roll and Pitch
         */
        void  Run( void );

        static HalAccelerometer Accelerometer;
        
    private:
        /** Calculates the Roll and Pitch
         * see http://www.st.com/web/en/resource/technical/document/application_note/CD00268887.pdf
         */
            void UpdatePitchAndRoll( void );
        /** Get the X axis raw value of the Accelerometer
         * @return int16_t X axis value
         */
            int16_t GetXRawAcceleration( void );
        /** Get the Y axis raw value of the Accelerometer
         * @return int16_t Y axis value
         */
            int16_t GetYRawAcceleration( void );
        /** Get the Z axis raw value of the Accelerometer
         * @return int16_t Z axis value
         */
        int16_t GetZRawAcceleration( void );
        int16_t XOffset;    /**< X axis offset value */
        int16_t YOffset;    /**< Y axis offset value */
        int16_t ZOffset;    /**< Z axis offset value */
        float Scaling;      /**< scaling for the device */
        float GRange;       /**< range of the device*/
        float FilterX[5];   /**< storage for X axis filter data*/
        float FilterY[5];   /**< storage for Y axis filter data*/
        float FilterZ[5];   /**< storage for Z axis filter data*/
        uint8_t FilterCount;/**< counter to keep track of where to store latest data */
        bool Update;        /**< Keep track on the need to repeat the calculation */
        float Pitch;        /**< Calculated Pitch */
        float Roll;         /**< Calculated Roll */

};

#endif /* HAL_ACCELEROMETER_H */
