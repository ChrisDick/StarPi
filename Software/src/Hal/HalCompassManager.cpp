
#include "Config.h"
#include "HalCompassManager.h"
#include <math.h>

#ifdef AK8975_MAGNETOMETER
#include "AK8975.h"
AK8975 compass;
#elif defined HMC5843L_MAGNETOMETER
#include "HMC5843L.h"
HMC5843 compass;
#elif defined HMC5883L_MAGNETOMETER
#include "HMC5883L.h"
HMC5883L compass;
#elif defined MPU9150_MAGNETOMETER
#include "MPU9150.h"
MPU9150 compass;
#else
#error no magnetometer defined - please edit your config.h file.
#endif


HalCompassManager::HalCompassManager()
{
    // initialise module stuff!
}

bool HalCompassManager::init()
{
    
    compass.initialize();
    // initialise compasserometer specifics here
#ifdef AK8975_MAGNETOMETER
 
#elif defined HMC5843_MAGNETOMETER
    
#elif defined HMC5883L_MAGNETOMETER
    
#elif MPU9150_MAGNETOMETER
    
#endif
    return true; // todo
}



float HalCompassManager::get_raw_heading()
{
    int16_t heading_x = 0;
    int16_t heading_y = 0;
    float result = 0;
    heading_x = get_x_raw_heading();
    heading_y = get_y_raw_heading();
    result = atan2(heading_y, heading_x);
    if(result < 0)
    {
        result += 2 * M_PI;
    }
    return result;
}

/*
    see http://ozzmaker.com/2014/12/17/compass2/#more-3062
*/
float HalCompassManager::get_tilt_compensated_heading(float roll, float pitch)
{
    float heading_x = 0;
    float heading_y = 0;
    float heading_z = 0;
    float result = 0;
    float x_compnent = 0;
    float y_compnent = 0;

    heading_x = (float)get_x_raw_heading();
    heading_y = (float)get_y_raw_heading();
    heading_z = (float)get_z_raw_heading();
    
    x_compnent = (heading_x*cos(pitch))+(heading_z*sin(pitch));
    y_compnent = (heading_x*sin(roll)*sin(pitch))+(heading_y*cos(roll))-(heading_z*sin(roll)*cos(pitch));

    result = 180*atan2(y_compnent,x_compnent)/M_PI;
    
    return result;
}


int16_t HalCompassManager::get_x_raw_heading()
{
    int16_t result = 0;
#ifdef OBJECTIVE_END_MAGNETOMETER_X_PLUS    
    result = compass.getHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_X_MINUS
    result = 0 - compass.getHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_PLUS    
    result = compass.getHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_MINUS
    result = 0 - compass.getHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_PLUS    
    result = compass.getHeadingZ();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_MINUS
    result = 0 - compass.getHeadingZ();
#else
    #error y axis not defined
#endif
    return result;
}

int16_t HalCompassManager::get_y_raw_heading()
{
    int16_t result = 0;
#ifdef TELESCOPE_RIGHT_MAGNETOMETER_X_PLUS
    result = compass.getHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
    result = 0 - compass.getHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
    result = compass.getHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
    result = 0 - compass.getHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
    result = compass.getHeadingZ();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS
    result = 0 - compass.getHeadingZ();
#else
    #error y axis not defined
#endif
    return result;    
}

int16_t HalCompassManager::get_z_raw_heading()
{
    int16_t result = 0;
#ifdef UP_MAGNETOMETER_X_PLUS
    result = compass.getHeadingX();
#elif defined UP_MAGNETOMETER_X_MINUS
    result = 0 - compass.getHeadingX();
#elif defined UP_MAGNETOMETER_Y_PLUS
    result = compass.getHeadingY();
#elif defined UP_MAGNETOMETER_Y_MINUS
    result = 0 - compass.getHeadingY();
#elif defined UP_MAGNETOMETER_Z_PLUS
    result = compass.getHeadingZ();
#elif defined UP_MAGNETOMETER_Z_MINUS
    result = 0 - compass.getHeadingZ();
#else
    #error z axis not defined
#endif
    return result;        
}








