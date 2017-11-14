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

#ifdef AK8975_MAGNETOMETER
#include "AK8975.h"
AK8975 Magnetomometer;
#elif defined HMC5843L_MAGNETOMETER
#include "HMC5843L.h"
HMC5843 Magnetomometer;
#elif defined HMC5883L_MAGNETOMETER
#include "HMC5883L.h"
HMC5883L Magnetomometer;
#elif defined MPU9150_MAGNETOMETER
#error todo MPU9150 has function named getHeading
#include "MPU9150.h"
MPU9150 Magnetomometer;
#elif defined LSM303DLHC_MAGNETOMETER
#include "LSM303DLHC.h"
LSM303DLHC_Mag Magnetomometer;
#else
#error no magnetometer defined - please edit your config.h file.
#endif

static float b[] = {1,  -1.4, 1};
static float a[] = {1, -1.3, 0.5};
static float Xv1m1 = 0, Xv2m1 = 0, Yv1m1 = 0, Yv2m1 = 0, Zv1m1 = 0, Zv2m1 = 0, v1m, v2m;


 static float iirfilter(float x1, float v1m1, float v2m1) {
    float y1 = 0;
    y1 = (b[0] * x1 + v1m1) / a[0];
    v1m = (b[1] * x1 + v2m1) - a[1] * y1;
    v2m = b[2] * x1 - a[2] * y1;
    v1m1 = v1m;
    v2m1 = v2m;
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
bool HalMagnetometer::HalMagnetometerInit( void )
{
    Magnetomometer.initialize();
    FilterCount = 0; 
    // initialise Magnetoerometer specifics here
#ifdef AK8975_MAGNETOMETER
    #error no init code for Magnetometer
#elif defined HMC5843_MAGNETOMETER
    #error no init code for Magnetometer
#elif defined HMC5883L_MAGNETOMETER
    Scaling = 1; 
#elif defined MPU9150_MAGNETOMETER
    #error no init code for Magnetometer
#elif defined LSM303DLHC_MAGNETOMETER
    Scaling = 1; 
#endif
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
    
    X = GetXRawHeading();
    Y = GetYRawHeading();
    Z = GetZRawHeading();

    FilterX[4] = iirfilter(((float)X), Xv1m1, Xv2m1);
    FilterY[4] = iirfilter(((float)Y), Yv1m1, Yv2m1);
    FilterZ[4] = iirfilter(((float)Z), Zv1m1, Zv2m1);
 
}

/* Access to the Magnetometer data.
 */
void HalMagnetometer::HalMagnetometerGetAll( float* Mx, float* My, float* Mz )
{
    *Mx = FilterX[4];
    *My = FilterY[4];
    *Mz = FilterZ[4];
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
