/*
HalAccelerometer provides a generic interface to any accelerometer
supported by the I2Cdevlib library. It uses a config file to select 
which sensor to use. 

Author and copyright of this file:
Chris Dick, 2015

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "HalAccelerometer.h"
#include "Config.h"
#include <math.h>

#ifdef MPU6050_ACCEL
#include "MPU6050.h"
MPU6050 Accel;
#elif defined ADXL345_ACCEL
#include "ADXL345.h"
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
#elif defined LSM303DLHC_ACCEL
#include "LSM303DLHC.h"
LSM303DLHC_Accel Accel;
#else
#error no Accelerometer defined - please edit your config.h file.
#endif

HalAccelerometer HalAccelerometer::Accelerometer;

/* Constructor
 */
HalAccelerometer::HalAccelerometer( void )
{
}

/* Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
bool HalAccelerometer::HalAccelerometerInit( void )
{
    bool Result = false;    
    FilterCount = 0;
    Accel.initialize();
    // initialise Accelerometer specifics here
    Scaling = 32768.0F;
#ifdef MPU6050_ACCEL
    Accel.setAccelXSelfTest(false);
    Accel.setAccelYSelfTest(false);
    Accel.setAccelZSelfTest(false);
    Scaling = 16384.0;
    Result = true;
#elif defined ADXL345_ACCEL
    #error no init code for Accelerometer
#elif defined BMA150_ACCEL
    #error no init code for Accelerometer
#elif L3G4200D_ACCEL
    #error no init code for Accelerometer
#elif defined MPU9150_ACCEL
    #error no init code for Accelerometer
#elif defined LSM303DLHC_ACCEL
// ToDo: configure intial LSM303DLHC settings
    Scaling = 16384.0;
    Result = true;
#endif
    return Result;
}

/* runs the filter and updates the Roll and Pitch
 */
void HalAccelerometer::Run( void )
{    
    int16_t X = 0;
    int16_t Y = 0;
    int16_t Z = 0;
    
    X = GetXRawAcceleration();
    Y = GetYRawAcceleration();
    Z = GetZRawAcceleration();
    
    FilterX[FilterCount] = (((double)X/Scaling));
    FilterY[FilterCount] = (((double)Y/Scaling));
    FilterZ[FilterCount] = (((double)Z/Scaling));
    
    FilterX[4] = (FilterX[0] + FilterX[1] + FilterX[2] + FilterX[3])/4;
    FilterY[4] = (FilterY[0] + FilterY[1] + FilterY[2] + FilterY[3])/4;
    FilterZ[4] = (FilterZ[0] + FilterZ[1] + FilterZ[2] + FilterZ[3])/4;
    
    FilterCount++;
    if (FilterCount == 4)
    {
        FilterCount = 0;  
    }
}
/* Access to the Accelerometer data.
 */
void HalAccelerometer::HalAccelerometerGetAll( float* Ax, float* Ay, float* Az )
{
    *Ax = FilterX[4];
    *Ay = FilterY[4];
    *Az = FilterZ[4];
}


/* Get the X axis raw value of the Accelerometer
 * @return int16_t X axis value
 */
int16_t HalAccelerometer::GetXRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef OBJECTIVE_END_ACCEL_X_PLUS    
    Result = Accel.getAccelerationX();
#elif defined OBJECTIVE_END_ACCEL_X_MINUS
    Result = 0 - ( Accel.getAccelerationX() );
#elif defined OBJECTIVE_END_ACCEL_Y_PLUS    
    Result = Accel.getAccelerationY();
#elif defined OBJECTIVE_END_ACCEL_Y_MINUS
    Result = 0 - ( Accel.getAccelerationY() );
#elif defined OBJECTIVE_END_ACCEL_Z_PLUS    
    Result = Accel.getAccelerationZ();
#elif defined OBJECTIVE_END_ACCEL_Z_MINUS
    Result = 0 - ( Accel.getAccelerationZ() );
#else
    #error y axis not defined
#endif
    return Result;
}

/* Get the Y axis raw value of the Accelerometer
 * @return int16_t Y axis value
 */
int16_t HalAccelerometer::GetYRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef TELESCOPE_RIGHT_ACCEL_X_PLUS
    Result = Accel.getAccelerationX();
#elif defined TELESCOPE_RIGHT_ACCEL_X_MINUS
    Result = 0 - Accel.getAccelerationX();
#elif defined TELESCOPE_RIGHT_ACCEL_Y_PLUS
    Result = Accel.getAccelerationY();
#elif defined TELESCOPE_RIGHT_ACCEL_Y_MINUS
    Result = 0 - Accel.getAccelerationY();
#elif defined TELESCOPE_RIGHT_ACCEL_Z_PLUS
    Result = Accel.getAccelerationZ();
#elif defined TELESCOPE_RIGHT_ACCEL_Z_MINUS
    Result = 0 - Accel.getAccelerationZ();
#else
    #error y axis not defined
#endif
    return Result;    
}

/* Get the Z axis raw value of the Accelerometer
 * @return int16_t Z axis value
 */
int16_t HalAccelerometer::GetZRawAcceleration( void )
{
    int16_t Result = 0;
#ifdef UP_ACCEL_X_PLUS
    Result = Accel.getAccelerationX();
#elif defined UP_ACCEL_X_MINUS
    Result = 0 - Accel.getAccelerationX();
#elif defined UP_ACCEL_Y_PLUS
    Result = Accel.getAccelerationY();
#elif defined UP_ACCEL_Y_MINUS
    Result = 0 - Accel.getAccelerationY();
#elif defined UP_ACCEL_Z_PLUS
    Result = Accel.getAccelerationZ();
#elif defined UP_ACCEL_Z_MINUS
    Result = 0 - Accel.getAccelerationZ();
#else
    #error z axis not defined
#endif
    return Result;        
}
