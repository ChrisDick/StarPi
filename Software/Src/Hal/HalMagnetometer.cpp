/*
HalMagnetometer provides a generic interface to any Magnetometer
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
#include "HalMagnetometer.h"
#include "Config.h"
#include <math.h>

#include "LSM303DLHC.h"
LSM303DLHC_Mag Magnetomometer;

static float b[] = {1,  -1.4, 1};
static float a[] = {1, -1.3, 0.5};


static float iirfilter(float x1, float* v1m1, float* v2m1) {
    float y1 = 0;
    y1 = ( ( b[0] * x1 ) + *v1m1) / a[0];
    *v1m1 = ( ( b[1] * x1 ) + *v2m1 ) - ( a[1] * y1 );
    *v2m1 = ( b[2] * x1 ) - ( a[2] * y1 );
    return y1;
}


HalMagnetometer HalMagnetometer::Magneto;

/* HalMagnetometer
 *  Constructor
 */
HalMagnetometer::HalMagnetometer( void )
{
}

/* HalMagnetometerInit
 *  Initialise the Magnetomometer
 * @return bool Initialisation status  
 */
bool HalMagnetometer::Init( void )
{
    Magnetomometer.initialize();
    FilterCount = 0; 
    // initialise Magnetoerometer specifics here
    return true; // todo
}


/* HalMagnetometerRun
 *  Runs the filter
 */
void HalMagnetometer::Run( void )
{
    int16_t X = 0;
    int16_t Y = 0;
    int16_t Z = 0;
    static float Xv1m1 = 0, Xv2m1 = 0, Yv1m1 = 0, Yv2m1 = 0, Zv1m1 = 0, Zv2m1 = 0;
    

    GetRawData( &X, &Y, &Z );
    
    FilterX[4] = iirfilter(((float)X), &Xv1m1, &Xv2m1);
    FilterY[4] = iirfilter(((float)Y), &Yv1m1, &Yv2m1);
    FilterZ[4] = iirfilter(((float)Z), &Zv1m1, &Zv2m1);
 
}

/* Access to the Magnetometer data.
 */
void HalMagnetometer::GetAll( float* Mx, float* My, float* Mz )
{
    *Mx = FilterX[4];
    *My = FilterY[4];
    *Mz = FilterZ[4];
}

/* Get the raw value of the Accelerometer
 * This function reads 6 bytes at once over the I2C 
 * instead of 3 transactions.
 * @return int16_t X axis value
 */
void HalMagnetometer::GetRawData( int16_t* X, int16_t* Y, int16_t* Z )
{
    /*
        get raw data
    */
    int16_t XRaw = 0;
    int16_t YRaw = 0;
    int16_t ZRaw = 0;
    
    Magnetomometer.getHeading( &XRaw, &YRaw, &ZRaw);
    
#ifdef OBJECTIVE_END_MAGNETOMETER_X_PLUS    
    *X = XRaw;
#elif defined OBJECTIVE_END_MAGNETOMETER_X_MINUS
    *X = 0 - XRaw;
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_PLUS    
    *X = YRaw;
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_MINUS
    *X = 0 - YRaw;
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_PLUS    
    *X = ZRaw;
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_MINUS
    *X = 0 - ZRaw;
#else
    #error y axis not defined
#endif

#ifdef TELESCOPE_RIGHT_MAGNETOMETER_X_PLUS
    *Y = XRaw;
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
    *Y = 0 - XRaw;
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
    *Y = YRaw;
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
    *Y = 0 - YRaw;
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
    *Y = ZRaw;
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS
    *Y = 0 - ZRaw;
#else
    #error y axis not defined
#endif

#ifdef UP_MAGNETOMETER_X_PLUS
    *Z = XRaw;
#elif defined UP_MAGNETOMETER_X_MINUS
    *Z = 0 - XRaw;
#elif defined UP_MAGNETOMETER_Y_PLUS
    *Z = YRaw;
#elif defined UP_MAGNETOMETER_Y_MINUS
    *Z = 0 - YRaw;
#elif defined UP_MAGNETOMETER_Z_PLUS
    *Z = ZRaw;
#elif defined UP_MAGNETOMETER_Z_MINUS
    *Z = 0 - ZRaw;
#else
    #error z axis not defined
#endif
}


/* GetXRawHeading
 *  Get the raw X heading of the Magneto
 * @return int16_t heading 
 */
int16_t HalMagnetometer::GetXRawHeading( void )
{
    int16_t Result = 0;
#ifdef OBJECTIVE_END_MAGNETOMETER_X_PLUS    
    Result = Magnetomometer.getHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.getHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_PLUS    
    Result = Magnetomometer.getHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.getHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_PLUS    
    Result = Magnetomometer.getHeadingZ();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.getHeadingZ();
#else
    #error y axis not defined
#endif
    return Result;
}

/* GetYRawHeading
 *  Get the raw Y heading of the Magneto
 * @return int16_t heading 
 */
int16_t HalMagnetometer::GetYRawHeading( void )
{
    int16_t Result = 0;
#ifdef TELESCOPE_RIGHT_MAGNETOMETER_X_PLUS
    Result = Magnetomometer.getHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.getHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
    Result = Magnetomometer.getHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.getHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
    Result = Magnetomometer.getHeadingZ();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.getHeadingZ();
#else
    #error y axis not defined
#endif
    return Result;    
}

/* GetZRawHeading
 *  Get the raw Z heading of the Magneto
 * @return int16_t heading 
 */
int16_t HalMagnetometer::GetZRawHeading( void )
{
    int16_t Result = 0;
#ifdef UP_MAGNETOMETER_X_PLUS
    Result = Magnetomometer.getHeadingX();
#elif defined UP_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.getHeadingX();
#elif defined UP_MAGNETOMETER_Y_PLUS
    Result = Magnetomometer.getHeadingY();
#elif defined UP_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.getHeadingY();
#elif defined UP_MAGNETOMETER_Z_PLUS
    Result = Magnetomometer.getHeadingZ();
#elif defined UP_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.getHeadingZ();
#else
    #error z axis not defined
#endif
    return Result;        
}
