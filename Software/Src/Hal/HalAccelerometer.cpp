
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

/** HalAccelerometer
 *  Constructor
 */
HalAccelerometer::HalAccelerometer()
{
    // initialise module stuff!
}

/** HalAccelerometerInit
 *  Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
bool HalAccelerometer::HalAccelerometerInit( void )
{
    bool result = false;    
    filter_count = 0;
    accel.initialize();
    // initialise accelerometer specifics here
#ifdef MPU6050_ACCEL
    accel.setAccelXSelfTest(false);
    accel.setAccelYSelfTest(false);
    accel.setAccelZSelfTest(false);
    scaling = (float)32768.0;
    g_range = 2;
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

/** HalAccelerometerGetPitch
 * Get the pitch of the accelerometer
 * @return float The Pitch
 */
float HalAccelerometer::HalAccelerometerGetPitch( void )
{
    return pitch;
}


/** HalAccelerometerGetRoll
 *  Get the roll of the accelerometer
 * @return float The roll
 */
float HalAccelerometer::HalAccelerometerGetRoll( void )
{
    return roll;
}

/** HalAccelerometerRun
 *  runs the filter and updates the roll and pitch
 */
void HalAccelerometer::HalAccelerometerRun( void )
{    
    filter_x[filter_count] = (((float)GetXRawAcceleration()/scaling)*(g_range*9.81));
    filter_y[filter_count] = (((float)GetYRawAcceleration()/scaling)*(g_range*9.81));
    filter_z[filter_count] = (((float)GetZRawAcceleration()/scaling)*(g_range*9.81));
    
    filter_x[4] = (filter_x[0] + filter_x[1] + filter_x[2] + filter_x[3])/4;
    filter_y[4] = (filter_y[0] + filter_y[1] + filter_y[2] + filter_y[3])/4;
    filter_z[4] = (filter_z[0] + filter_z[1] + filter_z[2] + filter_z[3])/4;
    
    filter_count++;
    if (filter_count)
    {
        filter_count = 0;  
    }
    UpdatePitchAndRoll();
}

/** UpdatePitchAndRoll
 *  calculates the roll and pitch
 * see http://www.st.com/web/en/resource/technical/document/application_note/CD00268887.pdf
 */
void HalAccelerometer::UpdatePitchAndRoll( void )
{
    /*
        Update pitch
    */
    pitch = atan ( filter_x / ( sqrt( (filter_y[4]*filter_y[4]) + ( filter_z[4]*filter_z[4]) ) ) );
            
    /*
        Update roll
    */
    roll = atan ( filter_y[4] / ( sqrt( (filter_x[4]*filter_x[4]) + ( filter_z[4]*filter_z[4]) ) ));
}


/** GetXRawAcceleration
 *  Get the X axis raw value of the accelerometer
 * @return int16_t X axis value
 */
int16_t HalAccelerometer::GetXRawAcceleration()
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

/** GetYRawAcceleration
 *  Get the Y axis raw value of the accelerometer
 * @return int16_t Y axis value
 */
int16_t HalAccelerometer::GetYRawAcceleration( void )
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

/** GetZRawAcceleration
 *  Get the Z axis raw value of the accelerometer
 * @return int16_t Z axis value
 */
int16_t HalAccelerometer::GetZRawAcceleration( void )
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








