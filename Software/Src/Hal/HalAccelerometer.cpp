#include "HalAccelerometer.h"
#include "Config.h"
#include <math.h>

#ifdef MPU6050_ACCEL
#include "MPU6050.h"
MPU6050 Accel;
#elif defined ADXL345_ACCEL
#include "ADXL345"
ADXL345 Accel;
#elif defined BMA150_ACCEL
#include "BMA150.h"
BMA150 Accel;
#elif L3G4200D_ACCEL
#include "L3G4200.h"
L3G4200D Accel;
#elif defined MPU9150_ACCEL
#include "MPU9150.h"
MPU9150 Accel;
#else
#error no Accelerometer defined - please edit your config.h file.
#endif

/** HalAccelerometer
 *  Constructor
 */
HalAccelerometer::HalAccelerometer( void )
{
}

/** HalAccelerometerInit
 *  Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
bool HalAccelerometer::HalAccelerometerInit( void )
{
    bool Result = false;    
    FilterCount = 0;
    Update = false;
    Accel.initialize();
    // initialise Accelerometer specifics here
#ifdef MPU6050_ACCEL
    Accel.setAccelXSelfTest(false);
    Accel.setAccelYSelfTest(false);
    Accel.setAccelZSelfTest(false);
    Scaling = (float)32768.0;
    GRange = 2;
    Result = true;
#elif defined ADXL345_ACCEL
    #error no init code for Accelerometer
#elif defined BMA150_ACCEL
    #error no init code for Accelerometer
#elif L3G4200D_ACCEL
    #error no init code for Accelerometer
#elif defined MPU9150_ACCEL
    #error no init code for Accelerometer
#endif
    XOffset = Accel.getXAccelOffset();
    YOffset = Accel.getYAccelOffset();
    ZOffset = Accel.getZAccelOffset();
    return Result;
}

/** HalAccelerometerGetPitch
 * Get the Pitch of the Accelerometer
 * @return float The Pitch
 */
float HalAccelerometer::HalAccelerometerGetPitch( void )
{
    if ( Update )
    {
        UpdatePitchAndRoll();
    }
    return Pitch;
}


/** HalAccelerometerGetRoll
 *  Get the Roll of the Accelerometer
 * @return float The Roll
 */
float HalAccelerometer::HalAccelerometerGetRoll( void )
{
    if ( Update )
    {
        UpdatePitchAndRoll();
    }
    return Roll;
}

/** HalAccelerometerRun
 *  runs the filter and updates the Roll and Pitch
 */
void HalAccelerometer::HalAccelerometerRun( void )
{    
    FilterX[FilterCount] = (((float)GetXRawAcceleration()/Scaling)*(GRange*9.81));
    FilterY[FilterCount] = (((float)GetYRawAcceleration()/Scaling)*(GRange*9.81));
    FilterZ[FilterCount] = (((float)GetZRawAcceleration()/Scaling)*(GRange*9.81));
    
    FilterX[4] = (FilterX[0] + FilterX[1] + FilterX[2] + FilterX[3])/4;
    FilterY[4] = (FilterY[0] + FilterY[1] + FilterY[2] + FilterY[3])/4;
    FilterZ[4] = (FilterZ[0] + FilterZ[1] + FilterZ[2] + FilterZ[3])/4;
    
    Update = true;
    FilterCount++;
    if (FilterCount)
    {
        FilterCount = 0;  
    }
}

/** UpdatePitchAndRoll
 *  calculates the Roll and Pitch
 * see http://www.st.com/web/en/resource/technical/document/application_note/CD00268887.pdf
 */
void HalAccelerometer::UpdatePitchAndRoll( void )
{
    /*
        Update Pitch
    */
    Pitch = atan ( FilterX[4] / ( sqrt( (FilterY[4]*FilterY[4]) + ( FilterZ[4]*FilterZ[4]) ) ) );
            
    /*
        Update Roll
    */
    Roll = atan ( FilterY[4] / ( sqrt( (FilterX[4]*FilterX[4]) + ( FilterZ[4]*FilterZ[4]) ) ));
}


/** GetXRawAcceleration
 *  Get the X axis raw value of the Accelerometer
 * @return int16_t X axis value
 */
int16_t HalAccelerometer::GetXRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef OBJECTIVE_END_ACCEL_X_PLUS    
    Result = Accel.getAccelerationX() + XOffset;
#elif defined OBJECTIVE_END_ACCEL_X_MINUS
    Result = 0 - ( Accel.getAccelerationX() + XOffset );
#elif defined OBJECTIVE_END_ACCEL_Y_PLUS    
    Result = Accel.getAccelerationY() + YOffset;
#elif defined OBJECTIVE_END_ACCEL_Y_MINUS
    Result = 0 - ( Accel.getAccelerationY() + YOffset );
#elif defined OBJECTIVE_END_ACCEL_Z_PLUS    
    Result = Accel.getAccelerationZ() + ZOffset;
#elif defined OBJECTIVE_END_ACCEL_Z_MINUS
    Result = 0 - ( Accel.getAccelerationZ() + ZOffset );
#else
    #error y axis not defined
#endif
    return Result;
}

/** GetYRawAcceleration
 *  Get the Y axis raw value of the Accelerometer
 * @return int16_t Y axis value
 */
int16_t HalAccelerometer::GetYRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef TELESCOPE_RIGHT_ACCEL_X_PLUS
    Result = Accel.getAccelerationX() + XOffset;
#elif defined TELESCOPE_RIGHT_ACCEL_X_MINUS
    Result = 0 - ( Accel.getAccelerationX() + XOffset );
#elif defined TELESCOPE_RIGHT_ACCEL_Y_PLUS
    Result = Accel.getAccelerationY() + YOffset;
#elif defined TELESCOPE_RIGHT_ACCEL_Y_MINUS
    Result = 0 - ( Accel.getAccelerationY() + YOffset);
#elif defined TELESCOPE_RIGHT_ACCEL_Z_PLUS
    Result = Accel.getAccelerationZ() + ZOffset;
#elif defined TELESCOPE_RIGHT_ACCEL_Z_MINUS
    Result = 0 - ( Accel.getAccelerationZ() + ZOffset );
#else
    #error y axis not defined
#endif
    return Result;    
}

/** GetZRawAcceleration
 *  Get the Z axis raw value of the Accelerometer
 * @return int16_t Z axis value
 */
int16_t HalAccelerometer::GetZRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef UP_ACCEL_X_PLUS
    Result = Accel.getAccelerationX() + XOffset;
#elif defined UP_ACCEL_X_MINUS
    Result = 0 - ( Accel.getAccelerationX() + XOffset );
#elif defined UP_ACCEL_Y_PLUS
    Result = Accel.getAccelerationY() + YOffset;
#elif defined UP_ACCEL_Y_MINUS
    Result = 0 - ( Accel.getAccelerationY() + YOffset );
#elif defined UP_ACCEL_Z_PLUS
    Result = Accel.getAccelerationZ() + ZOffset;
#elif defined UP_ACCEL_Z_MINUS
    Result = 0 - ( Accel.getAccelerationZ() + ZOffset );
#else
    #error z axis not defined
#endif
    return Result;        
}
