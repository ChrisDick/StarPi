#ifndef HAL_ACCELEROMETER_MANAGER_H
#define HAL_ACCELEROMETER_MANAGER_H

#include <stdint.h>





class HalAccelerometerManager {
    public:
        HalAccelerometerManager();
    
        bool init();
        float elevation_angle();
        bool get_roll_and_pitch(float* roll, float* pitch);
        int16_t get_x_raw_acceleration();
        int16_t get_y_raw_acceleration();
        int16_t get_z_raw_acceleration();
        
};



#endif /* HAL_ACCELEROMETER_MANAGER_H */

