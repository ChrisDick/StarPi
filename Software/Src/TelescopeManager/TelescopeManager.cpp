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
#include "TelescopeOrientation.h"
#include "TelescopeIO.h"
#include "HalGps.h"
//#include "HalSocket.h"
#include "MagModel.h"
#include "erfa.h"
#include "Config.h"

#ifdef TIMING
#include "GPIO.h"
#endif

#include "TelescopeManager.h"

TelescopeManager TelescopeManager::Telescope;
double TelescopeManager::RightAscension;         /**< Right ascension */
double TelescopeManager::Declination;            /**< Declination */
double TelescopeManager::TargetRightAscension;   /**< Target right ascension */
double TelescopeManager::TargetDeclination;      /**< Target declination */
float TelescopeManager::MagneticDeclination;
double TelescopeManager::Azimuth;
double TelescopeManager::Longitude;
double TelescopeManager::Latitude;
float TelescopeManager::HieghtAboveGround;
float TelescopeManager::LatitudeDegrees;
float TelescopeManager::LongitudeDegrees;
float TelescopeManager::HeadingDegrees;
uint16_t TelescopeManager::Year;
time_t TelescopeManager::UnixTime;
int8_t TelescopeManager::DeclinationHours;
int8_t TelescopeManager::DeclinationMinutes;
int8_t TelescopeManager::DeclinationSeconds;
int8_t TelescopeManager::RightAscensionHours;
int8_t TelescopeManager::RightAscensionMinutes;
int8_t TelescopeManager::RightAscensionSeconds;
uint8_t TelescopeManager::mode;
float TelescopeManager::AzimuthDegrees;
struct tm TelescopeManager::gmt;
float TelescopeManager::Pitch;
float TelescopeManager::Heading;
float TelescopeManager::Roll;
float TelescopeManager::PitchDegrees;





/* constructor
*/
TelescopeManager::TelescopeManager()
{
}

/* Telescope initialisation
*/
void TelescopeManager::Init()
{
    RightAscension = 0.0f;
    Declination = 0.0f;
    TargetRightAscension = 0.0f;
    TargetDeclination = 0.0f;
    MagneticDeclination = 0.0f;
    Azimuth = 0.0;
    Longitude = 0.0;
    Latitude = 0.0;
    HieghtAboveGround = 0.0f;
    /* set some defaults for development */
    LatitudeDegrees = 54.9482778f;
    LongitudeDegrees = -1.68f;
    HeadingDegrees = 0.0f;
    Year = 2017u; 
    UnixTime = 0;
    mode = 0;
    AzimuthDegrees = 0.0f;
    Pitch = 0.0f;
    Heading = 0.0f;
    Roll = 0.0f;
    PitchDegrees = 0.0f;

    TelescopeOrientation::Orient.Init();
    HalGps::Gps.Init();
    //HalSocket::Socket.Init( 9999, SocketCallback );
    
#ifdef TIMING
    GPIO::gpio.SetupOutput( TELESCOPE_MANAGER_PIN );
    GPIO::gpio.SetPullMode( TELESCOPE_MANAGER_PIN , PULL_UP );
#endif
    
}


/* main run function of the telescope manager
*/
void TelescopeManager::Run()
{
    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_MANAGER_PIN , true );
    #endif
    /*
        starpi variables
    */
    MagModel MagCorrect;
    erfa era; 
//    SOURCE_T Source = DEFAULT; 
    
    /*
        Get the Position, Orientation and time of the telescope
    */
    TelescopeOrientation::Orient.GetOrientation( &Pitch, &Roll, &Heading );
    PitchDegrees = (180.0f*(Pitch/M_PI));
    
    
//    timeval SysTime;
//    gettimeofday(&SysTime, NULL);
//    TelescopeIO::TeleIO.GetValue( LOCSOURCE, &Source);

//    if ( Source == WEBSITE )
//    {
//        TelescopeIO::TeleIO.GetValue( LATITUDE, &LatitudeDegrees);
//        TelescopeIO::TeleIO.GetValue( LONGITUDE, &LongitudeDegrees);
        /*
            Set values for calculation
        */
//        Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
//        Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;
//        UnixTime = SysTime.tv_sec;
//    }
//    else
    {
        if (HalGps::Gps.GetFix())
        {
            /*
                Update Gps Data 
            */
            UnixTime = HalGps::Gps.GetTime();
            HieghtAboveGround = HalGps::Gps.GetHeightInkm();
            LatitudeDegrees = HalGps::Gps.GetLatitude();
            LongitudeDegrees = HalGps::Gps.GetLongitude();
            Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
            Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;    
            /*
                Update data
            */
            TelescopeIO::TeleIO.UpdateData( HEIGHT, &HieghtAboveGround );
            TelescopeIO::TeleIO.UpdateData( LATITUDE, &LatitudeDegrees );
            TelescopeIO::TeleIO.UpdateData( LONGITUDE, &LongitudeDegrees );
            /*
                Update system time from fix
            */
            
        }
        else
        {
            /* use stored values */
//            TelescopeIO::TeleIO.GetValue( LONGITUDE, &LongitudeDegrees);
//            Longitude = ( LongitudeDegrees / 180.0f ) * M_PI;
//            TelescopeIO::TeleIO.GetValue( LATITUDE, &LatitudeDegrees);
//            Latitude = ( LatitudeDegrees / 180.0f ) * M_PI;
//            TelescopeIO::TeleIO.GetValue( HEIGHT, &HieghtAboveGround);
            /* use the system clock */
            timeval SysTime;
            gettimeofday(&SysTime, NULL);
            UnixTime = SysTime.tv_sec;
        }
    }  
            
    /*
        Convert Time to GMT
    */
    gmtime_r(&UnixTime, &gmt);
    /*
        get compensation for magnetic declination
    */
    if ( HalGps::Gps.GetFix() )
    {
        MagCorrect.SetParams( Latitude, Longitude, HieghtAboveGround, gmt.tm_mday, (gmt.tm_mon + 1), (gmt.tm_year + 1900) );
        MagneticDeclination = MagCorrect.GetDeclination();
        TelescopeIO::TeleIO.UpdateData( MAGDEC, &MagneticDeclination );
    }
    /* Atoc13 function params - do these want expoting */ 
    double utc1, utc2; 
    double xp = 0.0;
    double yp = 0.0;
    double dut1 = 0.0;
    double phpa = 0.0;
    double tc = 0.0;
    double rh = 0.0;
    double wl = 0.0;
    /*
        Perform Calculation for Telescope position
    */
    Azimuth = Heading - ((MagneticDeclination/180.0f)*M_PI); // ToDo make this come from magmodel in radians?
     
    /* calculate utc as two part value */
    (void)era.Dtf2d("UTC", 
        gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday, 
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec, 
        &utc1, &utc2); 
    /* convert az/zen to ra/dec */
    (void)era.Atoc13("A", Azimuth, ((90.0f * ERFA_DD2R) - Pitch),
        utc1, utc2, dut1,
        Longitude, Latitude, (HieghtAboveGround*1000.0f), xp, yp,
        phpa, tc, rh, wl,
        &RightAscension, &Declination);
 
    /*
        Update Data
    */

    Year = (gmt.tm_year + 1900 );
    TelescopeIO::TeleIO.UpdateData( GMTYEAR, &Year );
    TelescopeIO::TeleIO.UpdateData( GMTDAY, &gmt.tm_mday );
    TelescopeIO::TeleIO.UpdateData( GMTMON, &gmt.tm_mon );
    TelescopeIO::TeleIO.UpdateData( GMTHOUR, &gmt.tm_hour );
    TelescopeIO::TeleIO.UpdateData( GMTMIN, &gmt.tm_min );
    TelescopeIO::TeleIO.UpdateData( GMTSEC, &gmt.tm_sec );
    TelescopeIO::TeleIO.UpdateData( BST, &gmt.tm_isdst );
    TelescopeIO::TeleIO.UpdateData( UNIXTIME, &UnixTime );
    HeadingDegrees = (180.0f*(Heading/M_PI));
    TelescopeIO::TeleIO.UpdateData( MAGHEAD, &HeadingDegrees );
    TelescopeIO::TeleIO.UpdateData( TRUEHEAD, &AzimuthDegrees );


    TelescopeIO::TeleIO.UpdateData( ALTITUDE, &PitchDegrees );
    AzimuthDegrees = (180.0f*(Azimuth/M_PI));
    TelescopeIO::TeleIO.UpdateData( AZIMUTH, &AzimuthDegrees );
    
//    TelescopeIO::TeleIO.UpdateData( LSTHOUR, &Angles.LocalSiderealCCTime.Hours );
//    TelescopeIO::TeleIO.UpdateData( LSTMIN, &Angles.LocalSiderealCCTime.Minutes );
//    TelescopeIO::TeleIO.UpdateData( LSTSEC, &Angles.LocalSiderealCCTime.Seconds );
    int idmsf[4];
    char sign;
    /*
        convert RightAscension formaat from radians to hms
        resolution 0 = 0 00 01
    */
    (void)era.A2af(0, RightAscension, &sign, idmsf);
    RightAscensionHours = idmsf[0];
    RightAscensionMinutes = idmsf[1];
    RightAscensionSeconds = idmsf[2];
    if ( sign == '-' )
    {
        RightAscensionHours *= -1;
    }
    /*
        convert RightAscension formaat from radians to hms
    */
    (void)era.A2af(0, Declination, &sign, idmsf);
    DeclinationHours = idmsf[0];
    DeclinationMinutes = idmsf[1];
    DeclinationSeconds = idmsf[2];
    if ( sign == '-' )
    {
        DeclinationHours *= -1;
    }
    
    /*
    CC_TIME_T RaTemp;
    CC_TIME_T DecTemp;
    Calculator.ConvertRadiansToTime( RightAscension, &RaTemp );
    Calculator.ConvertRadiansToDegrees( Declination, &DecTemp );
    TelescopeIO::TeleIO.UpdateData( RAHOURS, &Temp.Hours );
    TelescopeIO::TeleIO.UpdateData( RAMIN, &Temp.Minutes );
    TelescopeIO::TeleIO.UpdateData( RASEC, &Temp.Seconds );
    TelescopeIO::TeleIO.UpdateData( DECHOURS, &Temp.Hours );
    TelescopeIO::TeleIO.UpdateData( DECMIN, &Temp.Minutes );
    TelescopeIO::TeleIO.UpdateData( DECSEC, &Temp.Seconds );
    */
    // one of these causes a bus error
    TelescopeIO::TeleIO.UpdateData( RAHOURS, &RightAscensionHours );
    TelescopeIO::TeleIO.UpdateData( RAMIN, &RightAscensionMinutes );
    TelescopeIO::TeleIO.UpdateData( RASEC, &RightAscensionSeconds );
    TelescopeIO::TeleIO.UpdateData( DECHOURS, &DeclinationHours );
    TelescopeIO::TeleIO.UpdateData( DECMIN, &DeclinationMinutes );
    TelescopeIO::TeleIO.UpdateData( DECSEC, &DeclinationSeconds );
    
    mode = HalGps::Gps.GetMode(); 
    
    TelescopeIO::TeleIO.UpdateData( GPSMODE, &mode );
    
    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_MANAGER_PIN , false );
    #endif
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

/* Export the MagneticDeclination
 */
float TelescopeManager::GetMagneticDeclination( void )
{
    return MagneticDeclination;
}

/* Export the HieghtAboveGround
 */
float TelescopeManager::GetHieghtAboveGround( void )
{
    return HieghtAboveGround;
}

/* Export the LatitudeDegrees
 */
float TelescopeManager::GetLatitudeDegrees( void )
{
    return LatitudeDegrees;
}

/* Export the LongitudeDegrees
 */
float TelescopeManager::GetLongitudeDegrees( void )
{
    return LongitudeDegrees;
}

/* Export the HeadingDegrees
 */
float TelescopeManager::GetHeadingDegrees( void )
{
    return HeadingDegrees;
}

/* Export the Azimuth
 */
double TelescopeManager::GetAzimuth( void )
{
    return Azimuth;
}

/* Export the Longitude
 */
double TelescopeManager::GetLongitude( void )
{
    return Longitude;
}

/* Export the Latitude
 */
double TelescopeManager::GetLatitude( void )
{
    return Latitude;
}


/* Export the DeclinationHours
 */
int8_t TelescopeManager::GetDeclinationHours( void )
{
    return DeclinationHours;
}

/* Export the DeclinationMinutes
 */
int8_t TelescopeManager::GetDeclinationMinutes( void )
{
    return DeclinationMinutes;
}

/* Export the DeclinationSeconds
 */
int8_t TelescopeManager::GetDeclinationSeconds( void )
{
    return DeclinationSeconds;
}

/* Export the RightAscensionHours
 */
int8_t TelescopeManager::GetRightAscensionHours( void )
{
    return RightAscensionHours;
}

/* Export the RightAscensionMinutes
 */
int8_t TelescopeManager::GetRightAscensionMinutes( void )
{
    return RightAscensionMinutes;
}

/* Export the RightAscensionSeconds
 */
int8_t TelescopeManager::GetRightAscensionSeconds( void )
{
    return RightAscensionSeconds;
}

/* Export the mode
 */
int8_t TelescopeManager::Getmode( void )
{
    return mode;
}

/* Export the AzimuthDegrees
 */
float TelescopeManager::GetAzimuthDegrees( void )
{
    return AzimuthDegrees;
}

/* Export the Pitch
 */
float TelescopeManager::GetPitch( void )
{
    return Pitch;
}

/* Export the Heading
 */
float TelescopeManager::GetHeading( void )
{
    return Heading;
}

/* Export the Roll
 */
float TelescopeManager::GetRoll( void )
{
    return Roll;
}

/* Export the PitchDegrees
 */
float TelescopeManager::GetPitchDegrees( void )
{
    return PitchDegrees;
}

/* Export the UnixTime
 */
time_t TelescopeManager::GetUnixTime( void )
{
    return UnixTime;
}

/* Export the Year
 */
uint16_t TelescopeManager::GetYear( void )
{
    return Year;
}

/* Export the Month
 */
uint16_t TelescopeManager::GetMonth( void )
{
    return gmt.tm_mon ;
}

/* Export the Day
 */
uint16_t TelescopeManager::GetDay( void )
{
    return gmt.tm_mday ;
}

/* Export the Hour
 */
uint16_t TelescopeManager::GetHour( void )
{
    return gmt.tm_hour;
}

/* Export the Minute
 */
uint16_t TelescopeManager::GetMinute( void )
{
    return gmt.tm_min;
}

/* Export the Second
 */
uint16_t TelescopeManager::GetSecond( void )
{
    return gmt.tm_sec;
}

/* Export the british summer time
 */
bool TelescopeManager::GetBST( void )
{
    return gmt.tm_isdst;
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
