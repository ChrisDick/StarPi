#include "HalCompass.h"
#include "Config.h"
#include <math.h>

#ifdef AK8975_MAGNETOMETER
#include "AK8975.h"
AK8975 CompassLib;
#elif defined HMC5843L_MAGNETOMETER
#include "HMC5843L.h"
HMC5843 CompassLib;
#elif defined HMC5883L_MAGNETOMETER
#include "HMC5883L.h"
HMC5883L CompassLib;
#elif defined MPU9150_MAGNETOMETER
#include "MPU9150.h"
MPU9150 CompassLib;
#else
#error no magnetometer defined - please edit your config.h file.
#endif


HalCompass HalCompass::Compass;

/* HalCompass
 *  Constructor
 */
HalCompass::HalCompass( void )
{
}

/* HalCompassInit
 *  Initialise the CompassLib
 * @return bool Initialisation status  
 */
bool HalCompass::HalCompassInit( void )
{
    CompassLib.Initialize();
    Roll = 0;
    Pitch = 0;
    FilterCount = 0; 
    // initialise Compasserometer specifics here
#ifdef AK8975_MAGNETOMETER
    #error no init code for Magnetometer
#elif defined HMC5843_MAGNETOMETER
    #error no init code for Magnetometer
#elif defined HMC5883L_MAGNETOMETER
    Scaling = 32767.0; 
#elif MPU9150_MAGNETOMETER
    #error no init code for Magnetometer
#endif
    return true; // todo
}


/* HalCompassSetPitch
 *  Set the pitch of the Compass
 * @param NewPitch float  
 */
void HalCompass::HalCompassSetPitch( float NewPitch )
{
    Pitch = NewPitch;
}

/* HalCompassSetRoll
 *  Set the roll of the Compass
 * @param NewRoll float  
 */
void HalCompass::HalCompassSetRoll( float NewRoll )
{
    Roll = NewRoll;
}


/* HalCompassRun
 *  Runs the filter
 */
void HalCompass::Run( void )
{    
    FilterX[FilterCount] = ((float)GetXRawHeading()/Scaling);
    FilterY[FilterCount] = ((float)GetYRawHeading()/Scaling);
    FilterZ[FilterCount] = ((float)GetZRawHeading()/Scaling);
    
    FilterX[4] = (FilterX[0] + FilterX[1] + FilterX[2] + FilterX[3])/4;
    FilterY[4] = (FilterY[0] + FilterY[1] + FilterY[2] + FilterY[3])/4;
    FilterZ[4] = (FilterZ[0] + FilterZ[1] + FilterZ[2] + FilterZ[3])/4;
    
    FilterCount++;
    if (FilterCount == 4)
    {
        FilterCount = 0;  
    }
}

/* HalCompassGetHeading
 *  Get the heading of the Compass
 * @return float heading 
 */
float HalCompass::HalCompassGetHeading( void )
{
    float Result = 0;
    Result = atan2(FilterY[4], FilterX[4]);
    if(Result < 0)
    {
        Result += 2 * M_PI;
    }
    return Result;
}

/* HalCompassGetHeading
 *  Get the tilt compensated heading of the Compass
 *   see http://ozzmaker.com/2014/12/17/Compass2/#more-3062
 * @return float heading 
 */
float HalCompass::HalCompassGetTiltCompensatedHeading( void )
{
    float Result = 0;
    float XComponent = 0;
    float YComponent = 0;

    XComponent = (FilterX[4]*cos(Pitch))+(FilterZ[4]*sin(Pitch));
    YComponent = (FilterX[4]*sin(Roll)*sin(Pitch))+(FilterY[4]*cos(Roll))-(FilterZ[4]*sin(Roll)*cos(Pitch));

    Result = 180*atan2(YComponent, XComponent)/M_PI;
    
    return Result;
}

/* GetXRawHeading
 *  Get the raw X heading of the Compass
 * @return int16_t heading 
 */
int16_t HalCompass::GetXRawHeading( void )
{
    int16_t Result = 0;
#ifdef OBJECTIVE_END_MAGNETOMETER_X_PLUS    
    Result = CompassLib.GetHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_X_MINUS
    Result = 0 - CompassLib.GetHeadingX();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_PLUS    
    Result = CompassLib.GetHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Y_MINUS
    Result = 0 - CompassLib.GetHeadingY();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_PLUS    
    Result = CompassLib.GetHeadingZ();
#elif defined OBJECTIVE_END_MAGNETOMETER_Z_MINUS
    Result = 0 - CompassLib.GetHeadingZ();
#else
    #error y axis not defined
#endif
    return Result;
}

/* GetYRawHeading
 *  Get the raw Y heading of the Compass
 * @return int16_t heading 
 */
int16_t HalCompass::GetYRawHeading( void )
{
    int16_t Result = 0;
#ifdef TELESCOPE_RIGHT_MAGNETOMETER_X_PLUS
    Result = CompassLib.GetHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
    Result = 0 - CompassLib.GetHeadingX();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
    Result = CompassLib.GetHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
    Result = 0 - CompassLib.GetHeadingY();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
    Result = CompassLib.GetHeadingZ();
#elif defined TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS
    Result = 0 - CompassLib.GetHeadingZ();
#else
    #error y axis not defined
#endif
    return Result;    
}

/* GetZRawHeading
 *  Get the raw Z heading of the Compass
 * @return int16_t heading 
 */
int16_t HalCompass::GetZRawHeading( void )
{
    int16_t Result = 0;
#ifdef UP_MAGNETOMETER_X_PLUS
    Result = CompassLib.GetHeadingX();
#elif defined UP_MAGNETOMETER_X_MINUS
    Result = 0 - CompassLib.GetHeadingX();
#elif defined UP_MAGNETOMETER_Y_PLUS
    Result = CompassLib.GetHeadingY();
#elif defined UP_MAGNETOMETER_Y_MINUS
    Result = 0 - CompassLib.GetHeadingY();
#elif defined UP_MAGNETOMETER_Z_PLUS
    Result = CompassLib.GetHeadingZ();
#elif defined UP_MAGNETOMETER_Z_MINUS
    Result = 0 - CompassLib.GetHeadingZ();
#else
    #error z axis not defined
#endif
    return Result;        
}
