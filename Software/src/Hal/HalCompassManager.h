#ifndef HAL_COMPASS_MANAGER_H
#define HAL_COMPASS_MANAGER_H
#include "stdint.h"




class HalCompassManager {
    public:
        HalCompassManager();
    
        bool init();
        float get_raw_heading();
        float get_tilt_compensated_heading(float roll, float pitch);
        int16_t get_x_raw_heading();
        int16_t get_y_raw_heading();
        int16_t get_z_raw_heading();
        
};

#endif /* HAL_COMPASS_MANAGER_H */



