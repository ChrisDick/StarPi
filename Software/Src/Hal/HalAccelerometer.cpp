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

#include "LSM303DLHC.h"

LSM303DLHC_Accel Accel;

static float b[] = {1,  -1.4, 1};
static float a[] = {1, -1.3, 0.5};

static float iirfilter(float x1, float* v1m1, float* v2m1) {
    float y1 = 0;
    y1 = ( ( b[0] * x1 ) + *v1m1) / a[0];
    *v1m1 = ( ( b[1] * x1 ) + *v2m1 ) - ( a[1] * y1 );
    *v2m1 = ( b[2] * x1 ) - ( a[2] * y1 );
    return y1;
}

HalAccelerometer HalAccelerometer::Accelerometer;

/* Constructor
 */
HalAccelerometer::HalAccelerometer( void )
{
}

/* Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
bool HalAccelerometer::Init( void )
{
    bool Result = false;    
    Accel.initialize();
    Result = true;
    return Result;
}

/* runs the filter and updates the Roll and Pitch
 */
void HalAccelerometer::Run( void )
{
    static float Xv1m1 = 0;
    static float Xv2m1 = 0;
    static float Yv1m1 = 0;
    static float Yv2m1 = 0;
    static float Zv1m1 = 0;
    static float Zv2m1 = 0;
    int16_t X = 0;
    int16_t Y = 0;
    int16_t Z = 0;
    
    GetRawData( &X, &Y, &Z );
    
    FilterX = iirfilter(((float)X), &Xv1m1, &Xv2m1);
    FilterY = iirfilter(((float)Y), &Yv1m1, &Yv2m1);
    FilterZ = iirfilter(((float)Z), &Zv1m1, &Zv2m1);    
}
    
    
/* Access to the Accelerometer data.
 */
void HalAccelerometer::GetAll( float* Ax, float* Ay, float* Az )
{
    *Ax = FilterX;
    *Ay = FilterY;
    *Az = FilterZ;
}


/* Get the raw value of the Accelerometer
 * This function reads 6 bytes at once over the I2C 
 * instead of 3 transactions.
 * @void
 */
void HalAccelerometer::GetRawData( int16_t* X, int16_t* Y, int16_t* Z )
{
    /*
        get raw data
    */
    int16_t XRaw = 0;
    int16_t YRaw = 0;
    int16_t ZRaw = 0;
    
    Accel.getAcceleration( &XRaw, &YRaw, &ZRaw);
    
#ifdef OBJECTIVE_END_ACCEL_X_PLUS    
    *X = XRaw;
#elif defined OBJECTIVE_END_ACCEL_X_MINUS
    *X = 0 - XRaw;
#elif defined OBJECTIVE_END_ACCEL_Y_PLUS    
    *X = YRaw;
#elif defined OBJECTIVE_END_ACCEL_Y_MINUS
    *X = 0 - YRaw;
#elif defined OBJECTIVE_END_ACCEL_Z_PLUS    
    *X = ZRaw;
#elif defined OBJECTIVE_END_ACCEL_Z_MINUS
    *X = 0 - ZRaw;
#else
    #error y axis not defined
#endif

#ifdef TELESCOPE_RIGHT_ACCEL_X_PLUS
    *Y = XRaw;
#elif defined TELESCOPE_RIGHT_ACCEL_X_MINUS
    *Y = 0 - XRaw;
#elif defined TELESCOPE_RIGHT_ACCEL_Y_PLUS
    *Y = YRaw;
#elif defined TELESCOPE_RIGHT_ACCEL_Y_MINUS
    *Y = 0 - YRaw;
#elif defined TELESCOPE_RIGHT_ACCEL_Z_PLUS
    *Y = ZRaw;
#elif defined TELESCOPE_RIGHT_ACCEL_Z_MINUS
    *Y = 0 - ZRaw;
#else
    #error y axis not defined
#endif

#ifdef UP_ACCEL_X_PLUS
    *Z = XRaw;
#elif defined UP_ACCEL_X_MINUS
    *Z = 0 - XRaw;
#elif defined UP_ACCEL_Y_PLUS
    *Z = YRaw;
#elif defined UP_ACCEL_Y_MINUS
    *Z = 0 - YRaw;
#elif defined UP_ACCEL_Z_PLUS
    *Z = ZRaw;
#elif defined UP_ACCEL_Z_MINUS
    *Z = 0 - ZRaw;
#else
    #error z axis not defined
#endif
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
