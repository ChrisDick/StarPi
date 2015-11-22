#include <stdio.h> // for debug
#include "TelescopeManager.h"
#include <math.h>
#include "HalAccelerometer.h"
#include "TelescopeOrientation.h"
#include "HalGps.h"

TelescopeManager TelescopeManager::Telescope;
double TelescopeManager::RightAscension;         /**< Right ascension */
double TelescopeManager::Declination;            /**< Declination */
double TelescopeManager::TargetRightAscension;   /**< Target right ascension */
double TelescopeManager::TargetDeclination;      /**< Target declination */

/* constructor
*/
TelescopeManager::TelescopeManager()
{
    
}
/*
    CC_TIME_T RA = { 14, 16, 22.1 };
    double Dec = ((14/24) + ((6/60)/24) + ((6.6/3600)/24))*(2*M_PI);
    RA 14h16m22.1s = 
    Dec19d06'06.6"
*/

/* main run function of the telescope manager
*/
void TelescopeManager::Run()
{
    CC_ANGLES_T Angles;
    /*
        Get the Position, Orientation and time of the telescope
    */
    double Pitch = TelescopeOrientation::Orient.TelescopeOrientationGetPitch();
    time_t UnixTime = (time_t)HalGps::Gps.HalGpsGetTime();
    Angles.LongitudeWest = ( HalGps::Gps.HalGpsGetLongitude() / 180 ) * M_PI;
    Angles.Latitude = ( HalGps::Gps.HalGpsGetLatitude() / 180 ) * M_PI;    
    Angles.Azimuth = TelescopeOrientation::Orient.TelescopeOrientationGetHeading();

    if ( Pitch > (M_PI/2) )
    {
        Angles.Altitude = 0;
    }
    else if ( Pitch < 0 )
    {
        Angles.Altitude = (M_PI/2);        
    }
    else
    {
        Angles.Altitude = (M_PI/2) - Pitch;
    }

#if 0
    if (Angles.Altitude > (M_PI/2))
    {
        Angles.Altitude = (M_PI/2);
    }
    else if (Angles.Altitude < 0)
    {
        Angles.Altitude = 0;
    }
    Angles.Altitude = Pitch;
#endif
    Calculator.EquitorialToCelestrial( &Angles, UnixTime );
    RightAscension = Angles.RightAscension;
    Declination = Angles.Declination;

    /*
        Debug Output
    */

    printf ("Pitch: %f ", (180*(Pitch/M_PI))); // debug
    printf ("Azimuth: %f \n", (180*(Angles.Azimuth/M_PI)));
  
//    printf ("Roll %f \n", (180*(Roll/M_PI))); // debug
//    printf ("Lat %f \n", (180*(Angles.Latitude/M_PI)));
//    printf ("long %f \n", (180*(Angles.LongitudeWest/M_PI)));
    printf ("time %f \n", (double)UnixTime);
    printf ("RA %f \n", Angles.RightAscension);
    printf ("Dec %f \n", Angles.Declination);
//    printf ("lst %d:%d:%f or %f\n", Angles.LocalSiderealCCTime.Hours, Angles.LocalSiderealCCTime.Minutes, Angles.LocalSiderealCCTime.Seconds, Angles.LocalSiderealTime );
    
    
    
    CC_TIME_T Temp;
    Calculator.ConvertRadiansToTime( Angles.RightAscension, &Temp );
    printf ("RightAscension %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);
    Calculator.ConvertRadiansToDegrees( Angles.Declination, &Temp );
    printf ("Declination %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);
//    RightAscension = Calculator.DecimaliseTime( RA_time )* (2 * M_PI);
//    Declination = Dec;
    
}


/* interface to set the target
 * @param Ra
 * @param Dec     
 */
void TelescopeManager::SetGotoTarget(double Ra, double Dec)
{
    TargetRightAscension = Ra;
    TargetDeclination = Dec;
}

/* Export the RightAscension and Declination
 */
void TelescopeManager::GetRaDec ( double* Ra, double* Dec )
{
    *Ra = RightAscension;
    *Dec = Declination;
}

#if 0
void TelescopeManager::testCalculator ( void )
{
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
    double DecimalTime;
    CC_TIME_T Temp;
    double JulianDate = 0;
    double Degrees = 0;
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
    
#if 0    
    /* check time maths */
    Calculator.AddTime ( TimeA, TimeB, &Temp );
    printf ("input Time: %d:%d:%f + %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeB.Hours, TimeB.Minutes, TimeB.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.AddTime ( TimeA, TimeA, &Temp );
    printf ("input Time: %d:%d:%f + %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeA.Hours, TimeA.Minutes, TimeA.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.SubtractTime ( TimeA, TimeB, &Temp );
    printf ("input Time: %d:%d:%f - %d:%d:%f = %d:%d:%f\n", TimeA.Hours, TimeA.Minutes, TimeA.Seconds, TimeB.Hours, TimeB.Minutes, TimeB.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
    Calculator.SubtractTime ( TimeB, TimeC, &Temp );
    printf ("input Time: %d:%d:%f - %d:%d:%f = %d:%d:%f\n", TimeB.Hours, TimeB.Minutes, TimeB.Seconds, TimeC.Hours, TimeC.Minutes, TimeC.Seconds, Temp.Hours, Temp.Minutes, Temp.Seconds );
#endif
    /* check time conversion calculation */
    DecimalTime = Calculator.DecimaliseTime( TimeD );
    Calculator.UnDecimaliseTime( DecimalTime, &Temp);
    printf ("input Time: %d:%d:%f or %f\n", TimeD.Hours, TimeD.Minutes, TimeD.Seconds, DecimalTime );
    printf ("input Time: %f or %d:%d:%f\n", DecimalTime, Temp.Hours, Temp.Minutes, Temp.Seconds );
#if 0
    /* check time to angle calculation */
    Degrees = Calculator.ConvertTimeToAngle( Time );
    printf ("Time to Degrees: %d:%d:%f to %f \n", Time.Hours, Time.Minutes, Time.Seconds, Degrees);
    Calculator.ConvertAngleToTime( Degrees, &Time );
    printf ("and back: %f to %d:%d:%f  \n", Degrees, Time.Hours, Time.Minutes, Time.Seconds);
#endif
    /* check Equatorial to Celestrial calculation */
    Calculator.EquitorialToCelestrial( &Angles, Time, Date );
    printf ("HourAngle %f \n", Angles.HourAngle);
    printf ("lst %d:%d:%f or %f\n", Angles.LocalSiderealCCTime.Hours, Angles.LocalSiderealCCTime.Minutes, Angles.LocalSiderealCCTime.Seconds, Angles.LocalSiderealTime );
    printf ("RightAscension %f \n", Angles.RightAscension);
    printf ("Declination %f \n", Angles.Declination);
    Calculator.ConvertRadiansToTime( Angles.RightAscension, &Temp );
    printf ("RightAscension %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);
    Calculator.ConvertRadiansToDegrees( Angles.Declination, &Temp );
    printf ("Declination %d:%d:%f \n", Temp.Hours, Temp.Minutes, Temp.Seconds);

#if 0
 // this bit doesn't give good results    
    CC_ANGLES_T Angles;
    CC_TIME_T Time = { 22, 9, 50 };
    CC_DATE_T Date = { 2015, 6, 13 };
    
    Angles.LongitudeWest = 1.60940/180*M_PI;
    Angles.Latitude = 54.94834/180*M_PI;
    Angles.RightAscension = TargetRightAscension;
    Angles.Declination = TargetDeclination;
    
    printf ("RightAscension %f \n", Angles.RightAscension);
    printf ("Declination %f \n", Angles.Declination);
    
    Calculator.CelestrialToEquitorial( &Angles, Time, Date );
    
    printf ("Azimuth %f \n", Angles.Azimuth);
    printf ("Altitude %f \n", Angles.Altitude);
    printf ("hour angle %f \n", Angles.HourAngle);
    
    Angles.RightAscension = 0;
    Angles.Declination = 0;
    
    Calculator.EquitorialToCelestrial( &Angles, Time, Date );

    printf ("RightAscension %f \n", Angles.RightAscension);
    printf ("Declination %f \n", Angles.Declination);
#endif    
    /* SetRaDec */
    RightAscension = Angles.RightAscension;
    Declination = Angles.Declination;
#endif
}
#endif 
