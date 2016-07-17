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
#include "MPU9150.h"
MPU9150 Magnetomometer;
#elif defined LSM303DLHC_MAGNETOMETER
#include "LSM303DLHC.h"
LSM303DLHC_Mag Magnetomometer;
#else
#error no magnetometer defined - please edit your config.h file.
#endif


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
    Magnetomometer.Initialize();
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
    FilterX[FilterCount] = (float)GetXRawHeading();
    FilterY[FilterCount] = (float)GetYRawHeading();
    FilterZ[FilterCount] = (float)GetZRawHeading();
    
    FilterX[4] = (FilterX[0] + FilterX[1] + FilterX[2] + FilterX[3])/4;
    FilterY[4] = (FilterY[0] + FilterY[1] + FilterY[2] + FilterY[3])/4;
    FilterZ[4] = (FilterZ[0] + FilterZ[1] + FilterZ[2] + FilterZ[3])/4;
    
    FilterCount++;
    if (FilterCount == 4)
    {
        FilterCount = 0;  
    }
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
    Result = Magnetomometer.GetHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.GetHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_PLUS    
    Result = Magnetomometer.GetHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.GetHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_PLUS    
    Result = Magnetomometer.GetHeadingZ();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.GetHeadingZ();
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
    Result = Magnetomometer.GetHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.GetHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
    Result = Magnetomometer.GetHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.GetHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
    Result = Magnetomometer.GetHeadingZ();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.GetHeadingZ();
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
    Result = Magnetomometer.GetHeadingX();
#elif defined UP_MAGNETOMETER_X_MINUS
    Result = 0 - Magnetomometer.GetHeadingX();
#elif defined UP_MAGNETOMETER_Y_PLUS
    Result = Magnetomometer.GetHeadingY();
#elif defined UP_MAGNETOMETER_Y_MINUS
    Result = 0 - Magnetomometer.GetHeadingY();
#elif defined UP_MAGNETOMETER_Z_PLUS
    Result = Magnetomometer.GetHeadingZ();
#elif defined UP_MAGNETOMETER_Z_MINUS
    Result = 0 - Magnetomometer.GetHeadingZ();
#else
    #error z axis not defined
#endif
    return Result;        
}
