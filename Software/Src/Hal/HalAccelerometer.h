#ifndef HAL_ACCELEROMETER_H
#define HAL_ACCELEROMETER_H

#include <stdint.h>

/** HalAccelerometer
 * - Class to provide use of the Accelerometer
 */
class HalAccelerometer {
    public:
        HalAccelerometer( void );
    
        bool  HalAccelerometerInit( void );
        float HalAccelerometerGetRoll( void );
        float HalAccelerometerGetPitch( void );
        void  HalAccelerometerRun( void );

    private:
        int16_t GetXRawAcceleration( void );
        int16_t GetYRawAcceleration( void );
        int16_t GetZRawAcceleration( void );
        void UpdatePitchAndRoll( void );
        int16_t XOffset;
        int16_t YOffset;
        int16_t ZOffset;
        float Scaling;
        float GRange;
        float FilterX[5];
        float FilterY[5];
        float FilterZ[5];
        uint8_t FilterCount;
        bool Update;
        float Pitch;
        float Roll;
};

#endif /* HAL_ACCELEROMETER_H */
