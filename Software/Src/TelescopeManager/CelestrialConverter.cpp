
#include "CelestrialConverter.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h> // for debug
/* CelestrialConverter
 * Constructor
 */
CelestrialConverter::CelestrialConverter()
{
}

/* EquitorialToCelestrial
 * convert equatorial coordinates to celestial coordinates 
 * Hour Angle (HA) and Declination (DE) given the Altitude (AL) and Azimuth (AZ) of a star and the observers Latitude (LA) and Longitude (LO) Convert Azimuth (AZ) and Altitude (AL) to decimal degrees. 
 * Compute sin(DE)=(sin (AL)*sin (LA))+(cos(AL)*cos (LA)*cos (AZ)).
 * Take the inverse sine of sin(DE) to get the declination.
 * Compute cos (HA)=(sin (AL)-(sin (LA)*sin(DE)))/(cos (LA)*cos (DE)).
 * Take the inverse cosine of cos (HA).
 * Take the sine of AZ.
 * If it is positive then HA=360-HA.
 * Divide HA by 15.
 * This is the Hour Angle in decimal Hours. 
 * 
 * Hour Angle to Right Ascension Convert Local Sidereal Time and Hour Angle into decimal hours.
 * Subtract Hour Angle from Local Sidereal Time. 
 * If result is negative add 24. 
 * This is the Right Ascension in decimal hours. 
 * 
 * Reference https://www.physicsforums.com/threads/how-can-i-determine-right-ascension-and-declination-of-a-star-in-the-sky.775652/
 * @param Angles structure containing all Angles
 * @param GrenwichStandardTime current time
 */
void CelestrialConverter::EquitorialToCelestrial( CC_ANGLES_T* Angles, time_t UnixTime )
{
    double SineHourAngle = 0;
    double CosHourAngle = 0;
    /*
        sin(DE)=(sin (AL)*sin (LA))+(cos(AL)*cos (LA)*cos (AZ)).
        Take the inverse sine of sin(DE) to get the declination.
    */
    Angles->Declination = asin( ( ( sin(Angles->Altitude)*sin(Angles->Latitude) ) + ( cos(Angles->Altitude)*cos(Angles->Latitude)*cos(Angles->Azimuth) ) ));
    
 //   if ( Angles->Declination > ( M_PI / 2 ) )
 //   {
 //       Angles->Declination -= (2 * M_PI );
 //   }
    /*
        cos (HA)=(sin (AL)-(sin (LA)*sin(DE)))/(cos (LA)*cos (DE))
        Take the inverse cosine of cos (HA).
    */
    SineHourAngle = (-1*sin(Angles->Azimuth)*cos(Angles->Altitude))/cos(Angles->Declination);
    CosHourAngle = ( sin( Angles->Altitude ) - ( sin( Angles->Declination ) * sin( Angles->Latitude ) ) ) / ( cos ( Angles->Declination ) * cos ( Angles->Latitude ) );
    //Angles->HourAngle = asin( SineHourAngle );

    //Angles->HourAngle = acos( CosHourAngle );
    /*
        Take the sine of AZ.
        If it is positive then HA=360-HA.
    */
//    if ( sin(Angles->Azimuth) > 0 )
//    {
//        Angles->HourAngle = ( 2 * M_PI ) - Angles->HourAngle; 
//    }
//    #if 0
    if ( ( SineHourAngle > 0 ) && ( CosHourAngle < 0 ) )
    {
        // flip over 90'
        Angles->HourAngle = M_PI - Angles->HourAngle;
    }
    else if ( ( SineHourAngle < 0 ) && ( CosHourAngle > 0 ) )
    {
        // flip over 270'
        Angles->HourAngle = ( 2 * M_PI ) - ( Angles->HourAngle - M_PI );
    }
    else 
    {
        // leave the angle as is.
    }
//    #endif
    Angles->LocalSiderealTime = CalculateLocalSiderealTime( UnixTime, Angles->LongitudeWest );
    /* debug */
    UnDecimaliseTime( Angles->LocalSiderealTime, &Angles->LocalSiderealCCTime );
    /* */
    Angles->RightAscension = Angles->LocalSiderealTime - Angles->HourAngle;  

    if ( Angles->RightAscension < 0 )
    {
        Angles->RightAscension += (2*M_PI);
    }
    Angles->RightAscension = fmod( Angles->RightAscension, ( 2 * M_PI ));
    Angles->Declination = fmod( Angles->Declination, ( 2 * M_PI ));
    
    /*
     Angle Raplus = AngleHrs(-0,-0,-33.83);
    Angle Decplus = AngleDegs(0,3,40.46);
    */
}

/* CelestrialToEquitorial
 * convert celestial coordinates to equatorial coordinates 
 * @param Angles structure containing all Angles
 * @param GrenwichStandardTime current time
 */
void CelestrialConverter::CelestrialToEquitorial( CC_ANGLES_T* Angles, time_t UnixTime )
{
/*
    Hour Angle = Grenwich Sidreal Time - Observers Longitude - Right Ascension 
    or 
    Hour Angle = Local Sidreal Time - Right Ascension 
    
    tan ( Azimuth ) = sin ( HourAngle ) / ( ( cos ( Hour Angle )*sin ( Observers Latitube ) ) - ( tan ( declination ) * cos ( Observers Latitude ) ) )

    sin ( Altitude ) = ( sin ( Observers Latitude ) * sin ( declination ) ) + ( cos ( Observers Latitude ) * cos ( declination ) * cos ( Hour Angle ) 
*/
    Angles->LocalSiderealTime = CalculateLocalSiderealTime( UnixTime, Angles->LongitudeWest );
    UnDecimaliseTime( Angles->LocalSiderealTime, &Angles->LocalSiderealCCTime );

    Angles->HourAngle = Angles->LocalSiderealTime - Angles->RightAscension;
    
    
    Angles->Azimuth = atan( sin ( Angles->HourAngle ) / ( ( cos ( Angles->HourAngle ) * sin ( Angles->LongitudeWest ) ) - ( tan ( Angles->Declination ) * cos ( Angles->LongitudeWest ) ) ) );
    
    
    Angles->Altitude = asin( ( sin ( Angles->LongitudeWest ) * sin ( Angles->Declination ) ) + ( cos ( Angles->LongitudeWest ) * cos ( Angles->Declination ) * cos ( Angles->HourAngle ) ) );

    Angles->Altitude = fmod( Angles->Altitude, ( 2 * M_PI ) );
    Angles->Azimuth = fmod( Angles->Azimuth, ( 2 * M_PI ) );
    
}

/* DecimaliseTime
 * convert hours minutes and seconds to hours
 * @param time structure containing all the time info
 * @return double hours
 */
double CelestrialConverter::DecimaliseTime( struct tm* Time )
{
    double DecTime = 0.0;
    DecTime  = Time->tm_hour + ((double)Time->tm_min/60.0) + ((double)Time->tm_sec/3600.0); 
    return DecTime;
}

/* UnDecimaliseTime
 * Convert hours to hours, minutes and seconds
 * @param TimeDec hours
 * @return Structure containing all the time info
*/
void CelestrialConverter::UnDecimaliseTime( double TimeDec, CC_TIME_T* Angle )
{
    /*
        extract the hours.
    */
    Angle->Hours = (uint8_t)TimeDec;
    /*
        extract the minutes
    */
    Angle->Minutes = (uint8_t)(( TimeDec - (double)Angle->Hours ) * 60.0);
    /*
         extract the seconds
    */
    Angle->Seconds = ((( TimeDec - (double)Angle->Hours ) * 60.0) - (double)Angle->Minutes ) * 60.0 ; 
    
}
/* Calculate the Julian date
 * To compute the Julian Date:
 * 
 * Convert local time to Greenwich Mean Time
 * Let Y equal the year, M equal the month, D equal the day in decimal form.
 * If M equals 1 or 2 then subtract 1 from Y. and add 12 to M.
 * Compute A. A=INT(Y/100)
 * Compute B. B=2-A+INT(A/4). However, if the date is earlier than October 15, 1582 then B=0.
 * Calculate C. C=INT(365.25*Y). If Y is negative then C=INT((365.25*Y)-.75).
 * Calculate E. E=INT(30.6001*(M+1))
 * Calculate JD (Julian Date). JD=B+C+D+E+1720994.5
 * @param Time - CC_TIME_T Time structure for calculations.
 * @param Date - CC_DATE_T Date structure for calculations.
 * @return Julian date as a double.
 */
double CelestrialConverter::CalculateJulianDate ( struct tm * gmt )
{
    /*
       First ccalculate the number of years and months since 
       the beginning of the Julian calendar. 1 March 4801BC
    */
    uint8_t CorrectionFactor = 0;  /* correction for no 0 B.C. year */   
    uint16_t Years = 0; /* number of years */
    uint8_t Months = 0;  /* number of months */
    /*
        The correction factor will be 1 for January and Febuary and 0 for everything else.
    */    
    CorrectionFactor = ( 14 - gmt->tm_mon ) / 12; /* discard remainder. */

    Years = gmt->tm_year + 1900 + 4800 - CorrectionFactor;
    
    Months = gmt->tm_mon - (12 * CorrectionFactor ) - 3;
    
    /*
       Then we need to calculate the Julian Day number. this relies on integer rounding.
    */
    double JulianDayNumber = 0;
    JulianDayNumber = gmt->tm_mday + ( ( ( 153 * Months ) + 2) / 5 ) +( 365 * Years ) + ( Years / 4 ) - ( Years /100 ) + ( Years / 400 ) - 32045;
    
    /* 
        Now we can use the time to calculate the Date.
    */
    double JulianDate = JulianDayNumber + ( ( (double)gmt->tm_hour - 12.0 ) / 24.0 ) + ( (double)gmt->tm_min / 1440.0 ) + ( gmt->tm_sec / 86400.0 );  
    
    /*
        to test this the Julian date for January 1st 2000 at 12:00:00 is 2451545.0
    */
    
    return JulianDate;
}


/* CalculateLocalSiderealTime
 * calculate the Local sidereal time from the current time and location
 * GMST = 6.697374558 + 0.06570982441908 D0 + 1.00273790935 H + 0.000026 T2 
 ToDo update comments: http://aa.usno.navy.mil/faq/docs/GAST.php
 ToDo Improve accuracy with GAST = GMST + equation of the equinoxes
 * Where D is the Julian date at 2000 January 1, 12h UT, 
 * which is a Julian date of 2451545.0
 * D = JD - 2451545.0
 without equinox correction:
 HourAngle 0.934725
lst 15:36:3.379669 or 15.600939
 RightAscension 2.099843
Declination 0.352585

 * @param GrenwichStandardTime current time 
 * @param LongitudeWest 
 * @return double time in radians. 
GMST = 6.697374558 + 0.06570982441908 D0 + 1.00273790935 H + 0.000026 T2
JD = JD0 + H/24.
 
 */
double CelestrialConverter::CalculateLocalSiderealTime( time_t UnixTime, double LongitudeWest ) // time_t UnixTime  CC_TIME_T GrenwichMeanTime, CC_DATE_T Date
{
    double Result = 0;
    double JulianDate = 0;
    double JulianDate2000 = 0;
    //double JulianDate2015 = 0;
    double GMST = 0;
    double JulianMidnight = 0;
    
    struct tm * gmt;
    gmt = gmtime ( &UnixTime );
    //JulianDate = CalculateJulianDate( gmt );
    JulianDate =  ( UnixTime / 86400.0 ) + 2440587.5; //unix time = (JD − 2440587.5) × 86400
    //printf("julian date %f\n", JulianDate);
    //JulianDate = 2457187.42350;
    JulianDate2000 = JulianDate - 2451545.0;
    //JulianDate2015 = 2457024.0; 
    
    JulianMidnight = JulianDate2000 - ( DecimaliseTime( gmt ) / 24.0 );
    
    GMST = 6.697374558 + ( 0.06570982441908 * JulianMidnight ) + ( 1.00273790935 * DecimaliseTime( gmt ) ) + ( 0.000026 * ( ( (uint32_t)JulianDate2000 / 36525 ) * ( (uint32_t)JulianDate2000 / 36525 ) ) );
    
//    GMST = 18.697374558 + ( 24.06570982441908 * JulianDate2000 ); Not accurate enough!
    
    GMST = fmod ( GMST, 24.0 );
    double eqeq = 0;
    double nutation = 0;
    double obliquity  = 0;
    nutation = -0.000319 * sin ( 125.04 - ( 0.052954 * JulianDate2000 ) ) - ( 0.000024 * sin (2 * (280.47 + ( 0.98565 * JulianDate2000 ))));
    obliquity = 23.4393 - ( 0.0000004 * JulianDate2000);
    eqeq = nutation * cos ( obliquity );
    
    double GAST = GMST + eqeq;
    
    /*
    The correction term is called the nutation in right ascension or the equation of the equinoxes. Thus,

GAST = GMST + eqeq.
eqeq = Δψ cos ε
Δψ ≈ -0.000319 sin ( 125.04 - ( 0.052954 * JulianDate2000 ) ) - 0.000024 sin (2(280.47 + ( 0.98565 * JulianDate2000 )));
ε = 23.4393 - ( 0.0000004 * JulianDate2000)

The equation of the equinoxes is given as eqeq = Δψ cos ε where Δψ, the nutation in longitude, is given in hours approximately by

Δψ ≈ -0.000319 sin Ω - 0.000024 sin 2L

with Ω, the Longitude of the ascending node of the Moon, given as

Ω = 125.04 - 0.052954 D,

and L, the Mean Longitude of the Sun, given as

L = 280.47 + 0.98565 D.

ε is the obliquity and is given as

ε = 23.4393 - 0.0000004 D.

The above expressions for Ω, L, and ε are all expressed in degrees.
    */
    
    
    //Result = GMST - LongitudeWest;
    Result = GAST - LongitudeWest;
    
    return Result;
}

/* ConvertRadiansToTime
 * Convert an angle in radians to a time.
 * @param Time structure containing all the time info
 * @return double Radians - The Angle
 */
void CelestrialConverter::ConvertRadiansToTime( double Radians, CC_TIME_T* Time )
{
    double TimeDec = 0;    
    TimeDec = Radians * ( 12.0 / M_PI ) ; 
    UnDecimaliseTime( TimeDec, Time );
}
/* ConvertRadiansToDegrees
 * Convert an angle in radians to Degrees.
 * @param Degrees structure containing the result
 * @param double Radians - The Angle
 */
void CelestrialConverter::ConvertRadiansToDegrees( double Radians, CC_TIME_T* Degrees )
{
    double DegreesDec = 0;    
    //UnDecimaliseTime( DegreesDec, Degrees );
 
    if (Radians < 0 )
    {
        DegreesDec = (0.0 - Radians) * ( 180.0 / M_PI ); 
        /*
            extract the hours.
        */
        Degrees->Hours = ((uint8_t)DegreesDec);
        /*
            extract the minutes
        */
        Degrees->Minutes =  ((uint8_t)(( DegreesDec - (double)Degrees->Hours ) * 60.0));
        /*
            extract the seconds
        */
        Degrees->Seconds = -1 * ((( DegreesDec - (double)Degrees->Hours ) * 60.0) - (double)Degrees->Minutes ) * 60.0 ; 
        Degrees->Hours = -1 * Degrees->Hours;
        Degrees->Minutes = -1 * Degrees->Minutes;
    }
    else
    {
        DegreesDec = Radians * ( 180.0 / M_PI ); 
        /*
            extract the hours.
        */
        Degrees->Hours = (uint8_t)DegreesDec;
        /*
            extract the minutes
        */
        Degrees->Minutes = (uint8_t)(( DegreesDec - (double)Degrees->Hours ) * 60.0);
        /*
            extract the seconds
        */
        Degrees->Seconds = ((( DegreesDec - (double)Degrees->Hours ) * 60.0) - (double)Degrees->Minutes ) * 60.0 ; 
    }
}

#if 0
/* AddTime   
 *  Add two times together, will wrap around 1 day.
 * @param TimeA
 * @param TimeB
 * @return the sum of the two times 
 */
void CelestrialConverter::AddTime( CC_TIME_T TimeA, CC_TIME_T TimeB, CC_TIME_T* Result )
{
    /*
        add seconds, then check for completed minutes
    */
    Result->Seconds = TimeA.Seconds + TimeB.Seconds;
    Result->Minutes = TimeA.Minutes + TimeB.Minutes;
    Result->Hours = TimeA.Hours + TimeB.Hours;
    
    if ( Result->Seconds >= 60 )
    {
        Result->Seconds -= 60;
        Result->Minutes++;
    }
    /*
        add minutes, then check for completed hours.
    */
    if ( Result->Minutes >= 60 )
    {
        Result->Minutes -= 60;
        Result->Hours++;
    }
    /*
        add hours, then remove completed days.
    */
    if ( Result->Hours >= 24 )
    {
        Result->Hours -= 24;
    }
}

/* SubtractTime
 * subtract two times, result will wrap around 1 day.
 * @param TimeA
 * @param TimeB
 * @return the subtraction of the two times 
 */
void CelestrialConverter::SubtractTime( CC_TIME_T TimeA, CC_TIME_T TimeB, CC_TIME_T* Result )
{
    /*
        subtract seconds then check for wrap.
    */
    Result->Seconds = TimeA.Seconds - TimeB.Seconds;
    Result->Minutes = TimeA.Minutes - TimeB.Minutes;
    Result->Hours = TimeA.Hours - TimeB.Hours;

    if ( Result->Seconds < 0 )
    {
        Result->Seconds += 60;
        Result->Minutes--;
    }
    
    /*
        subtract minutes then check for wrap.
    */
    if ( Result->Minutes > 60 )
    {
        Result->Minutes += 60;
        Result->Hours--;
    }
    
    /*
        subtract hours then check for wrap.
    */
    if ( Result->Hours > 24)
    {
        Result->Hours += 24;
    }
}

/* ConvertTimeToRadians
 * Convert a time to an angle in radians
 * @param Time structure containing all the time info
 * @return double The angle
 */
double CelestrialConverter::ConvertTimeToRadians( CC_TIME_T Time )
{
    double Angle = 0;
    double TimeDec = 0;
    /*
        first convert time to a decimal.
    */
    TimeDec = DecimaliseTime( Time );
    
    /*
        then multiply by 15' and convert to radians
    */
    Angle = TimeDec * 15.0 * (M_PI/180.0);
    
    return Angle;
}


/* ConvertDegreesToRadians
 * Convert a time to an angle in radians
 * @param Time structure containing all the angle info
 * @return double The angle
 */
double CelestrialConverter::ConvertDegreesToRadians( CC_TIME_T Degrees )
{
    double Radians = 0;
    double DegreesDec = 0;
    /*
        first convert time to a decimal.
    */
    DegreesDec = DecimaliseTime( Degrees );
    
    /*
        then multiply by 15' and convert to radians
    */
    Radians = DegreesDec * (M_PI/180.0);
    
    return Radians;
}


/* ConvertTimeToAngle
 * Convert a time to an angle in degrees
 * @param Time structure containing all the time info
 * @return double Angle
 */
double CelestrialConverter::ConvertTimeToAngle( CC_TIME_T Time )
{
    double Angle = 0;
    double TimeDec = 0;
    /*
        first convert time to a decimal.
    */
    TimeDec = DecimaliseTime( Time );
    
    /*
        then multiply by 15'
    */
    Angle = TimeDec * 15.0;
    
    return Angle;
}


/* ConvertAngleToTime
 * Convert an angle to a time
 * @param Angle 
 * @return structure containing all the time info
 */
void CelestrialConverter::ConvertAngleToTime( double Angle, CC_TIME_T* time )
{
   double DecimalTime = 0;
   DecimalTime = Angle / 15.0;
   UnDecimaliseTime( DecimalTime, time);
   // angle = (time.hours * 15) + ((time.minutes/60)*15) + ((time.seconds/3600)*15); 
    
}

#endif
