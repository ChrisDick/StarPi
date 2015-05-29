
#include <stdio.h> // for debug
#include "TelescopeManager.h"
TelescopeManager TelescopeManager::Telescope;
float TelescopeManager::RightAscension;         /**< Right ascension */
float TelescopeManager::Declination;            /**< Declination */
float TelescopeManager::TargetRightAscension;   /**< Target right ascension */
float TelescopeManager::TargetDeclination;      /**< Target declination */

/* constructor
*/
TelescopeManager::TelescopeManager()
{
    
}


/* main run function of the telescope manager
*/
void TelescopeManager::Run()
{
    float Pitch = HalAccelerometer::Accelerometer.HalAccelerometerGetPitch();
    float Roll = HalAccelerometer::Accelerometer.HalAccelerometerGetRoll();
    
    HalCompass::Compass.HalCompassSetPitch( Pitch );
    HalCompass::Compass.HalCompassSetRoll ( Roll );
    /*
        temporarily use roll and pitch as Ra and Dec 
    */
    RightAscension = Pitch;
    Declination = Roll;
    //Calculator.step(10000, pitch, HalCompassGetTiltCompensatedHeading() );
    printf ("Pitch %f \n", Pitch); // debug
    printf ("Roll %f \n", Roll); // debug
}


/* interface to set the target
 * @param Ra
 * @param Dec     
 */
void TelescopeManager::SetGotoTarget(float Ra, float Dec)
{
    TargetRightAscension = Ra;
    TargetDeclination = Dec;
}

/* Export the RightAscension and Declination
 */
void TelescopeManager::GetRaDec ( float* Ra, float* Dec )
{
    *Ra = RightAscension;
    *Dec = Declination;
}