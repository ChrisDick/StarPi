#ifndef HAL_ACCELEROMETER_H
#define HAL_ACCELEROMETER_H

#include <stdint.h>





class HalAccelerometer {
    public:
        HalAccelerometer();
    
        bool  HalAccelerometerInit();
        float HalAccelerometerGetRoll();
        float HalAccelerometerGetPitch();
        void  HalAccelerometerRun();

    private:
        int16_t GetXRawAcceleration();
        int16_t GetYRawAcceleration();
        int16_t GetZRawAcceleration();
        void UpdatePitchAndRoll;
        int16_t x_offset;
        int16_t y_offset;
        int16_t z_offset;
        float scaling;
        float pitch;
        float roll;
        float filter_x[5];
        float filter_y[5];
        float filter_z[5];
        uint8_t filter_count;
};



#endif /* HAL_ACCELEROMETER_H */

