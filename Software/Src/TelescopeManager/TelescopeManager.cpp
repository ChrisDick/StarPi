
#include <stdio.h> // for debug
#include "TelescopeManager.h"
#include <math.h>
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
//#if 0
    float Pitch = HalAccelerometer::Accelerometer.HalAccelerometerGetPitch();
    float Roll = HalAccelerometer::Accelerometer.HalAccelerometerGetRoll();
    float Heading = 0;
    float TiltHeading = 0;
    HalCompass::Compass.HalCompassSetPitch( Pitch );
    HalCompass::Compass.HalCompassSetRoll ( Roll );
    /*
        Temporarily use roll and pitch as Ra and Dec 
    */
    RightAscension = Pitch;
    Declination = Roll;
    Heading = HalCompass::Compass.HalCompassGetHeading();
    TiltHeading = HalCompass::Compass.HalCompassGetTiltCompensatedHeading();
    //Calculator.step(10000, pitch, HalCompassGetTiltCompensatedHeading() );
    printf ("Pitch %f \n", (180*(Pitch/M_PI))); // debug
    printf ("Roll %f \n", (180*(Roll/M_PI))); // debug
    printf ("Heading %f \n", Heading);
    printf ("TiltHeading %f \n", TiltHeading);
//#endif
    /*
 test calculator
taken from :http://xjubier.free.fr/en/site_pages/astronomy/coordinatesConverter.html
Equatorial Geocentric Coordinates (J2000)
  
Right Ascension  8h  6m  50.13s 
Declination 20° 14' 47.16" 
Date and Time in Coordinated Universal Time (UTC)
 13Day  6Month  2015Year 
 22Hour 9Minute 50.3Second 
Coordinates of the Observer
Latitude   54°   59.90033'  N   —> 54.94834°  
Longitude   1°   36.56400'  W   —> -1.60940°  

Horizontal Geocentric Coordinates   
Altitude    5.23°   5.39°(with refraction)
Azimuth 298.25° 298.25°(with refraction)
 
Horizontal Topocentric Coordinates  (stars)
Altitude (with refraction)      5.39°
Azimuth                         298.25°
 
Julian Date 2457187.42350    
Mean Greenwich Sideral Time 15.62395
    15h37m26.21s
Local Sideral Time  15.51665
    15h30m59.95s 

*/
#if 0
    CC_DATE_T Date = { 2015, 6, 13 };
    CC_ANGLES_T Angles;
    CC_TIME_T Time = { 22, 9, 50.3 };
    float DecimalTime;
    CC_TIME_T Temp;
    float JulianDate = 0;
    float Degrees = 0;
    CC_TIME_T TimeA = { 12, 35, 46 };
    CC_TIME_T TimeB = { 1, 26, 54 };
    CC_TIME_T TimeC = { 2, 46, 21 };
    CC_TIME_T TimeD = { 15, 30, 59.95 };
    
    Angles.LongitudeWest = 1.60940/180*M_PI;
    Angles.Latitude = 54.94834/180*M_PI;
    Angles.Azimuth = 298.25/180*M_PI;
    Angles.Altitude = 5.23/180*M_PI;
    Angles.HourAngle = 0;
    Angles.RightAscension = 0;
    Angles.Declination = 0;
    /* check Julian date calculation */
    JulianDate = Calculator.CalculateJulianDate( Time, Date );
    printf ( "Gegorian Date to Julain Date: %d:%d:%d at %d:%d:%f gives %f \n", Date.Year, Date.Month, Date.Day, Time.Hours, Time.Minutes, Time.Seconds, JulianDate );
    
    /* check time maths */
    Calculator.AddTime ( TimeA, TimeB, &Temp );
    printf ("input Time: %d:%d:%f + %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeB.Hours, TimeB.Minutes, TimeB.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.AddTime ( TimeA, TimeA, &Temp );
    printf ("input Time: %d:%d:%f + %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeA.Hours, TimeA.Minutes, TimeA.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.SubtractTime ( TimeA, TimeB, &Temp );
    printf ("input Time: %d:%d:%f - %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeB.Hours, TimeB.Minutes, TimeB.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.SubtractTime ( TimeB, TimeC, &Temp );
    printf ("input Time: %d:%d:%f - %d:%d:%f = %d:%d:%f\n", TimeB.Hours, TimeB.Minutes, TimeB.Seconds, TimeC.Hours, TimeC.Minutes, TimeC.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );

    
    /* check time conversion calculation */
    DecimalTime = Calculator.DecimaliseTime( TimeD );
    Calculator.UnDecimaliseTime( DecimalTime, &Temp);
    printf ("input Time: %d:%d:%f or %f\n", TimeD.Hours, TimeD.Minutes, TimeD.Seconds, DecimalTime );
    printf ("input Time: %f or %d:%d:%f\n", DecimalTime, Temp.Hours, Temp.Minutes, Temp.Seconds );
    
    /* check time to angle calculation */
    Degrees = Calculator.ConvertTimeToAngle( Time );
    printf ("Time to Degrees: %d:%d:%f to %f \n", Time.Hours, Time.Minutes, Time.Seconds, Degrees);
    Calculator.ConvertAngleToTime( Degrees, &Time );
    printf ("and back: %f to %d:%d:%f  \n", Degrees, Time.Hours, Time.Minutes, Time.Seconds);
    
    /* LST */
//    Angles.LocalSiderealTime = Calculator.CalculateLocalSiderealTime( Time, Date, Angles.LongitudeWest );
//    printf ("lst %f\n ", Angles.LocalSiderealTime );     
    /* check Equatorial calculation */
    Calculator.EquitorialToCelestrial( &Angles, Time, Date );
    printf ("HourAngle %f \n", Angles.HourAngle);
    printf ("lst %d:%d:%f or %f\n", Angles.LocalSiderealCCTime.Hours, Angles.LocalSiderealCCTime.Minutes, Angles.LocalSiderealCCTime.Seconds, Angles.LocalSiderealTime );
    printf ("RightAscension %f \n", Angles.RightAscension);
    printf ("Declination %f \n", Angles.Declination);
    Calculator.ConvertRadiansToTime( Angles.RightAscension, &Temp );
    printf ("RightAscension %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);
    Calculator.ConvertRadiansToDegrees( Angles.Declination, &Temp );
    printf ("Declination %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);
    
    //const unsigned int ra_int = (unsigned int)floor( 0.5 +  Angles.RightAscension*(((unsigned int)0x80000000)/M_PI));
    //const int dec_int = (int)floor(0.5 + Angles.Declination*(((unsigned int)0x80000000)/M_PI));
    //const int status = 0;
    //sendPosition(ra_int,dec_int,status);
//SetRaDec
    RightAscension = Angles.RightAscension;
    Declination = Angles.Declination;
#endif
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