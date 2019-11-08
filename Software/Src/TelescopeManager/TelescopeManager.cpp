/*
Telescope manager is a higher level system manager

Author and copyright of this file:
Chris Dick, 2018

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
#include "HalGps.h"
//#include "HalWeather.h"
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
int8_t TelescopeManager::LongitudeHours;
int8_t TelescopeManager::LongitudeMinutes;
int8_t TelescopeManager::LongitudeSeconds;
int8_t TelescopeManager::LatitudeHours;
int8_t TelescopeManager::LatitudeMinutes;
int8_t TelescopeManager::LatitudeSeconds;
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
    Year = 2018u; 
    UnixTime = 0;
    mode = 0;
    AzimuthDegrees = 0.0f;
    Pitch = 0.0f;
    Heading = 0.0f;
    Roll = 0.0f;
    PitchDegrees = 0.0f;

    TelescopeOrientation::Orient.Init();
    HalGps::Gps.Init();
//    HalWeather::Weather.Init();
    
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

    MagModel MagCorrect;
    erfa era; 
//    static uint16_t Count = 0u;
//    Count++;
//    if ( Count == 10u )
//    {
//        HalWeather::Weather.Run();
//        Count = 0u;
//        printf("Pressure: %f Humidity:%f", Pressure, Humidity );
//    }
    /*
        Get the Position, Orientation and time of the telescope
    */
    TelescopeOrientation::Orient.GetOrientation( &Pitch, &Roll, &Heading );
    
    PitchDegrees = (180.0f*(Pitch/M_PI));
        
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
            Update system time from fix
        */
        //ToDo
    }
    else
    {
        /* use the system clock */
        timeval SysTime;
        gettimeofday(&SysTime, NULL);
        UnixTime = SysTime.tv_sec;
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
    }
//    Temperature = HalWeather::Weather.getTemperature();
//    Humidity = HalWeather::Weather.getHumidity();
//    Pressure = HalWeather::Weather.getPressure();

    /* Atoc13 function params - ToDo do these want exporting? */ 
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
    if ( Azimuth < 0 )
    {
        Azimuth += ( 2 * M_PI );
    }
    if ( Azimuth > ( 2 * M_PI ) )
    {
        Azimuth -= ( 2 * M_PI );
    }
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
    /* Time Date */
    Year = (gmt.tm_year + 1900 );
//    TelescopeIO::TeleIO.UpdateData( LSTHOUR, &Angles.LocalSiderealCCTime.Hours );
//    TelescopeIO::TeleIO.UpdateData( LSTMIN, &Angles.LocalSiderealCCTime.Minutes );
//    TelescopeIO::TeleIO.UpdateData( LSTSEC, &Angles.LocalSiderealCCTime.Seconds );
    /* Orientation Data */
    HeadingDegrees = (180.0f*(Heading/M_PI));
    AzimuthDegrees = (180.0f*(Azimuth/M_PI));
    /* reformat data */
    int idmsf[4];
    char sign;
    /*
        convert RightAscension format from radians to hms
        resolution 0 = 0 00 01
    */
    (void)era.A2tf(0, RightAscension, &sign, idmsf);
    RightAscensionHours = idmsf[0];
    RightAscensionMinutes = idmsf[1];
    RightAscensionSeconds = idmsf[2];
    if ( sign == '-' )
    {
        RightAscensionHours *= -1;
    }
    /*
        convert Declination format from radians to hms
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
        convert Latitude format from radians to hms 
    */
    (void)era.A2af(0, Latitude, &sign, idmsf);
    LatitudeHours = idmsf[0];
    LatitudeMinutes = idmsf[1];
    LatitudeSeconds = idmsf[2];
    if ( sign == '-' )
    {
        LatitudeHours *= -1;
    }
    
    /*
        convert Longitude format from radians to hms
    */
    (void)era.A2af(0, Longitude, &sign, idmsf);
    LongitudeHours = idmsf[0];
    LongitudeMinutes = idmsf[1];
    LongitudeSeconds = idmsf[2];
    if ( sign == '-' )
    {
        LongitudeHours *= -1;
    }
    /*
        remaining GPS data
    */
    mode = HalGps::Gps.GetMode(); 

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

/* Export the RightAscension
 */
float TelescopeManager::GetRightAscension( void )
{
    return (float)((RightAscension/M_PI)*24.0);
}

/* Export the Declination
 */
float TelescopeManager::GetDeclination( void )
{
    return (float)((Declination/M_PI)*180.0);
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

/* Export the LatitudeHours
 */
int8_t TelescopeManager::GetLatitudeHours( void )
{
    return LatitudeHours;
}

/* Export the LatitudeMinutes
 */
int8_t TelescopeManager::GetLatitudeMinutes( void )
{
    return LatitudeMinutes;
}

/* Export the LatitudeSeconds
 */
int8_t TelescopeManager::GetLatitudeSeconds( void )
{
    return LatitudeSeconds;
}

/* Export the LongitudeHours
 */
int8_t TelescopeManager::GetLongitudeHours( void )
{
    return LongitudeHours;
}

/* Export the LongitudeMinutes
 */
int8_t TelescopeManager::GetLongitudeMinutes( void )
{
    return LongitudeMinutes;
}

/* Export the LongitudeSeconds
 */
int8_t TelescopeManager::GetLongitudeSeconds( void )
{
    return LongitudeSeconds;
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

/* Export the British summer time
 */
bool TelescopeManager::GetBST( void )
{
    return gmt.tm_isdst;
}



#if 0
LocalSidrealTimeHourHandler
LocalSidrealTimeMinHandler
LocalSidrealTimeSecHandler
#endif
