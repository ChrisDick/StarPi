/*
Telescope manager is a higher level system manager

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
#include <stdio.h> 
#include <math.h>
#include <sys/time.h>
#include "TelescopeOrientation.h"
#include "TelescopeIO.h"
#include "HalGps.h"
#include "MagModel.h"
#include "erfa.h"

#include "TelescopeManager.h"

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

/* Telescope initialisation
*/
void TelescopeManager::TelescopeManagerInit()
{
    RightAscension = 0.0f;
    Declination = 0.0f;
    TargetRightAscension = 0.0f;
    TargetDeclination = 0.0f;
    TelescopeOrientation::Orient.TelescopeOrientationInit();
    HalGps::Gps.HalGpsInit();
}


/* main run function of the telescope manager
*/
void TelescopeManager::Run()
{
    /*
        starpi variables
    */
    time_t UnixTime = 0;
    struct tm gmt;
    float MagneticDeclination = 0.0f;
    MagModel MagCorrect;
    double Azimuth = 0.0;
    double Longitude = 0.0;
    double Latitude = 0.0;
    float HieghtAboveGround = 0.0f;
    float LatitudeDegrees = 0.0f;
    float LongitudeDegrees = 0.0f;
    SOURCE_T Source = DEFAULT; 
    float HeadingDegrees = 0.0f;
    uint16_t Year = 0.0f;
    float AzimuthDegrees = 0.0f;
    timeval SysTime;
    /* Atoc13 function params */
    erfa era; 
    int res; 
    double utc1, utc2; 
    double xp = 0.0;
    double yp = 0.0;
    double dut1 = 0.0;
    double phpa = 0.0;
    double tc = 0.0;
    double rh = 0.0;
    double wl = 0.0;
    /*
        Perfrom user tasks
    */
    
    /*
        Get the Position, Orientation and time of the telescope
    */
    float Pitch = 0.0f;
    float Heading = 0.0f;
    float Roll = 0.0f;
    TelescopeOrientation::Orient.GetOrientation( &Pitch, &Roll, &Heading );
    float PitchDegrees = (180.0f*(Pitch/M_PI));
    
    
    gettimeofday(&SysTime, NULL);
    TelescopeIO::TeleIO.TelescopeIOGetValue( LOCSOURCE, &Source);

//    if ( Source == WEBSITE )
//    {
//        TelescopeIO::TeleIO.TelescopeIOGetValue( LATITUDE, &LatitudeDegrees);
//        TelescopeIO::TeleIO.TelescopeIOGetValue( LONGITUDE, &LongitudeDegrees);
        /*
            Set values for calculation
        */
//        Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
//        Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;
//        UnixTime = SysTime.tv_sec;
//    }
//    else
    {
        if (HalGps::Gps.HalGpsGetFix())
        {
            /*
                Update Gps Data 
            */
            UnixTime = HalGps::Gps.HalGpsGetTime();
            HieghtAboveGround = HalGps::Gps.HalGpsGetHeightInkm();
            LatitudeDegrees = HalGps::Gps.HalGpsGetLatitude();
            LongitudeDegrees = HalGps::Gps.HalGpsGetLongitude();
            Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
            Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;    
            /*
                Update data
            */
            TelescopeIO::TeleIO.TelescopeIOUpdateData( HEIGHT, &HieghtAboveGround );
            TelescopeIO::TeleIO.TelescopeIOUpdateData( LATITUDE, &LatitudeDegrees );
            TelescopeIO::TeleIO.TelescopeIOUpdateData( LONGITUDE, &LongitudeDegrees );
            /*
                Update system time from fix
            */
            
        }
        else
        {
            /* use stored values */
            TelescopeIO::TeleIO.TelescopeIOGetValue( LONGITUDE, &LongitudeDegrees);
            Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
            TelescopeIO::TeleIO.TelescopeIOGetValue( LATITUDE, &LatitudeDegrees);
            Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;
            TelescopeIO::TeleIO.TelescopeIOGetValue( HEIGHT, &HieghtAboveGround);
            UnixTime = SysTime.tv_sec;
        }
    }  
//    LatitudeDegrees = 54.9482778;
//    LongitudeDegrees = -1.68;
            
    /*
        Convert Time to GMT
    */
    gmtime_r(&UnixTime, &gmt);
    /*
        get compensation for magnetic declination
    */
    if ( HalGps::Gps.HalGpsGetFix() )
    {
        MagCorrect.MagModelSetParams( Latitude, Longitude, HieghtAboveGround, gmt.tm_mday, (gmt.tm_mon + 1), (gmt.tm_year + 1900) );
        MagneticDeclination = MagCorrect.MagModelGetDeclination();
        TelescopeIO::TeleIO.TelescopeIOUpdateData( MAGDEC, &MagneticDeclination );
    }
    /*
        Perform Calculation for Telescope position
    */
    Azimuth = Heading + ((MagneticDeclination/180.0f)*M_PI); // ToDo make this come from magmodel in radians?
     
    /* calculate utc as two part value */
    res = era.Dtf2d("UTC", 
        gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday, 
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec, 
        &utc1, &utc2); 
    /* convert az/zen to ra/dec */
    res = era.Atoc13("A", Azimuth, ((90.0f * ERFA_DD2R) - Pitch),
        utc1, utc2, dut1,
        Longitude, Latitude, (HieghtAboveGround*1000.0f), xp, yp,
        phpa, tc, rh, wl,
        &RightAscension, &Declination);
    (void)res; 
    /*
        Update Data
    */

    Year = (gmt.tm_year + 1900 );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTYEAR, &Year );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTDAY, &gmt.tm_mday );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTMON, &gmt.tm_mon );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTHOUR, &gmt.tm_hour );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTMIN, &gmt.tm_min );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( GMTSEC, &gmt.tm_sec );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( BST, &gmt.tm_isdst );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( UNIXTIME, &UnixTime );
    HeadingDegrees = (180.0f*(Heading/M_PI));
    TelescopeIO::TeleIO.TelescopeIOUpdateData( MAGHEAD, &HeadingDegrees );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( TRUEHEAD, &AzimuthDegrees );


    TelescopeIO::TeleIO.TelescopeIOUpdateData( ALTITUDE, &PitchDegrees );
    AzimuthDegrees = (180.0f*(Azimuth/M_PI));
    TelescopeIO::TeleIO.TelescopeIOUpdateData( AZIMUTH, &AzimuthDegrees );
    
//    TelescopeIO::TeleIO.TelescopeIOUpdateData( LSTHOUR, &Angles.LocalSiderealCCTime.Hours );
//    TelescopeIO::TeleIO.TelescopeIOUpdateData( LSTMIN, &Angles.LocalSiderealCCTime.Minutes );
//    TelescopeIO::TeleIO.TelescopeIOUpdateData( LSTSEC, &Angles.LocalSiderealCCTime.Seconds );

    CC_TIME_T Temp;
    Calculator.ConvertRadiansToTime( RightAscension, &Temp );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( RAHOURS, &Temp.Hours );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( RAMIN, &Temp.Minutes );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( RASEC, &Temp.Seconds );
    Calculator.ConvertRadiansToDegrees( Declination, &Temp );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( DECHOURS, &Temp.Hours );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( DECMIN, &Temp.Minutes );
    TelescopeIO::TeleIO.TelescopeIOUpdateData( DECSEC, &Temp.Seconds );

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
