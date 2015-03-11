
#include "Config.h"
#include "HalAccelerometerManager.h"
#include <math.h>

#ifdef MPU6050_ACCEL
#include "MPU6050.h"
MPU6050 accel;
#elif defined ADXL345_ACCEL
#include "ADXL345"
ADXL345 accel;
#elif defined BMA150_ACCEL
#include "BMA150.h"
BMA150 accel;
#elif L3G4200D_ACCEL
#include "L3G4200.h"
L3G4200D accel;
#elif defined MPU9150_ACCEL
#include "MPU9150.h"
MPU9150 accel;
#else
#error no accelerometer defined - please edit your config.h file.
#endif

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t z_offset = 0;

HalAccelerometerManager::HalAccelerometerManager()
{
    // initialise module stuff!
}

bool HalAccelerometerManager::init()
{
    bool result = false;    
    accel.initialize();
    // initialise accelerometer specifics here
#ifdef MPU6050_ACCEL
    accel.setAccelXSelfTest(false);
    accel.setAccelYSelfTest(false);
    accel.setAccelZSelfTest(false);
    result = true;
#elif defined ADXL345_ACCEL
    #error no init code for accelerometer
#elif defined BMA150_ACCEL
    #error no init code for accelerometer
#elif L3G4200D_ACCEL
    #error no init code for accelerometer
#elif defined MPU9150_ACCEL
    #error no init code for accelerometer
#endif
    x_offset = accel.getXAccelOffset();
    y_offset = accel.getYAccelOffset();
    z_offset = accel.getZAccelOffset();
    return result;
}




float HalAccelerometerManager::elevation_angle(){
    float result = 0;
    int16_t obj_accel = 0;
    obj_accel = get_x_raw_acceleration();
    result = asin((obj_accel)/9.81);
    return result;
}

/*
    see http://www.st.com/web/en/resource/technical/document/application_note/CD00268887.pdf
*/
bool HalAccelerometerManager::get_roll_and_pitch(float* roll, float* pitch)
{
    float obj_accel = 0;
    float side_accel = 0;
    float up_accel = 0;
    
    obj_accel  = (float)get_x_raw_acceleration();
    side_accel = (float)get_y_raw_acceleration();
    up_accel   = (float)get_z_raw_acceleration();
    
    /*
        Pitch
    */
    *pitch = atan ( obj_accel / ( sqrt( (side_accel*side_accel) + ( up_accel*up_accel) ) ) );
        
    /*
        Roll
    */
    *roll = atan ( side_accel / ( sqrt( (obj_accel*obj_accel) + ( up_accel*up_accel) ) ));
        
    return true;
}

int16_t HalAccelerometerManager::get_x_raw_acceleration()
{
    int16_t result = 0;
#ifdef OBJECTIVE_END_ACCEL_X_PLUS    
    result = accel.getAccelerationX() + x_offset;
#elif defined OBJECTIVE_END_ACCEL_X_MINUS
    result = 0 - ( accel.getAccelerationX() + x_offset );
#elif defined OBJECTIVE_END_ACCEL_Y_PLUS    
    result = accel.getAccelerationY() + y_offset;
#elif defined OBJECTIVE_END_ACCEL_Y_MINUS
    result = 0 - ( accel.getAccelerationY() + y_offset );
#elif defined OBJECTIVE_END_ACCEL_Z_PLUS    
    result = accel.getAccelerationZ() + z_offset;
#elif defined OBJECTIVE_END_ACCEL_Z_MINUS
    result = 0 - ( accel.getAccelerationZ() + z_offset );
#else
    #error y axis not defined
#endif
    return result;
}

int16_t HalAccelerometerManager::get_y_raw_acceleration()
{
    int16_t result = 0;
#ifdef TELESCOPE_RIGHT_ACCEL_X_PLUS
    result = accel.getAccelerationX() + x_offset;
#elif defined TELESCOPE_RIGHT_ACCEL_X_MINUS
    result = 0 - ( accel.getAccelerationX() + x_offset );
#elif defined TELESCOPE_RIGHT_ACCEL_Y_PLUS
    result = accel.getAccelerationY() + y_offset;
#elif defined TELESCOPE_RIGHT_ACCEL_Y_MINUS
    result = 0 - ( accel.getAccelerationY() + y_offset);
#elif defined TELESCOPE_RIGHT_ACCEL_Z_PLUS
    result = accel.getAccelerationZ() + z_offset;
#elif defined TELESCOPE_RIGHT_ACCEL_Z_MINUS
    result = 0 - ( accel.getAccelerationZ() + z_offset );
#else
    #error y axis not defined
#endif
    return result;    
}

int16_t HalAccelerometerManager::get_z_raw_acceleration()
{
    int16_t result = 0;
#ifdef UP_ACCEL_X_PLUS
    result = accel.getAccelerationX() + x_offset;
#elif defined UP_ACCEL_X_MINUS
    result = 0 - ( accel.getAccelerationX() + x_offset );
#elif defined UP_ACCEL_Y_PLUS
    result = accel.getAccelerationY() + y_offset;
#elif defined UP_ACCEL_Y_MINUS
    result = 0 - ( accel.getAccelerationY() + y_offset );
#elif defined UP_ACCEL_Z_PLUS
    result = accel.getAccelerationZ() + z_offset;
#elif defined UP_ACCEL_Z_MINUS
    result = 0 - ( accel.getAccelerationZ() + z_offset );
#else
    #error z axis not defined
#endif
    return result;        
}








