
#include "CelestrialConverter.h"
#include <stdint.h>
#include <math.h>

/* CelestrialConverter
 * Constructor
 */
CelestrialConverter::CelestrialConverter()
{
}

/* CalculateLocalSiderealTime
 * calculate the Local sidereal tiem from the current time and location
 * @param GrenwichStandardTime current time 
 * @param longitude_west 
 * @return float time in radians.   
 */
float CelestrialConverter::CalculateLocalSiderealTime( CC_TIME_T GrenwichStandardTime, float LongitudeWest)
{
    float Result = 0;
    float GstAngle = ConvertTimeToAngle ( GrenwichStandardTime );
    Result = GstAngle - LongitudeWest;
    return Result;
}

/* EquitorialToCelestrial
 * convert equatorial coordinates to celestial coordinates 
 * @param Angles structure containing all Angles
 * @param GrenwichStandardTime current time
 */
void CelestrialConverter::EquitorialToCelestrial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime )
{
    Angles->Declination = asin( ( ( sin(Angles->Altitude)*sin(Angles->Latitude) ) + ( cos(Angles->Altitude)*cos(Angles->Latitude)*cos(Angles->Azimuth) ) ));
    Angles->HourAngle = (-1*sin(Angles->Azimuth)*cos(Angles->Altitude))/cos(Angles->Declination);
    Angles->RightAscension = CalculateLocalSiderealTime( GrenwichStandardTime, Angles->LongitudeWest ) - Angles->HourAngle;     
}

/* CelestrialToEquitorial
 * convert celestial coordinates to equatorial coordinates 
 * @param Angles structure containing all Angles
 * @param GrenwichStandardTime current time
 */
void CelestrialConverter::CelestrialToEquitorial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime )
{
    
    
}

/* AddTime   
 *  Add two times together, will wrap around 1 day.
 * @param TimeA
 * @param TimeB
 * @return the sum of the two times 
 */
CC_TIME_T CelestrialConverter::AddTime( CC_TIME_T TimeA, CC_TIME_T TimeB )
{
    CC_TIME_T Result;
    /*
        add seconds, then check for completed minutes
    */
    Result.Seconds = TimeA.Seconds + TimeB.Seconds;
    if ( Result.Seconds >= 60 )
    {
        Result.Seconds -= 60;
        Result.Minutes++;
    }
    /*
        add minutes, then check for completed hours.
    */
    Result.Minutes += TimeA.Minutes + TimeB.Minutes;
    if ( Result.Minutes >= 60 )
    {
        Result.Minutes -= 60;
        Result.Hours++;
    }
    /*
        add hours, then remove completed days.
    */
    Result.Hours += TimeA.Hours + TimeB.Hours;
    if ( Result.Hours >= 24 )
    {
        Result.Hours -= 24;
    }
    return Result;
}

/* SubtractTime
 * subtract two times, result will wrap around 1 day.
 * @param TimeA
 * @param TimeB
 * @return the subtraction of the two times 
 */
CC_TIME_T CelestrialConverter::SubtractTime( CC_TIME_T TimeA, CC_TIME_T TimeB )
{
    CC_TIME_T Result;
    /*
        subtract seconds then check for wrap.
    */
    Result.Seconds = TimeA.Seconds - TimeB.Seconds;
    if ( Result.Seconds > 60 )
    {
        Result.Seconds += 60;
        Result.Minutes--;
    }
    
    /*
        subtract minutes then check for wrap.
    */
    Result.Minutes += TimeA.Minutes - TimeB.Minutes;
    if ( Result.Minutes > 60 )
    {
        Result.Minutes += 60;
        Result.Hours--;
    }
    
    /*
        subtract hours then check for wrap.
    */
    Result.Hours += TimeA.Hours - TimeB.Hours;
    if ( Result.Hours > 24)
    {
        Result.Hours += 24;
    }
    
    return Result;
}

/* DecimaliseTime
 * convert hours minutes and seconds to hours
 * @param time structure containing all the time info
 * @return float hours
 */
float CelestrialConverter::DecimaliseTime( CC_TIME_T Time )
{
    float DecTime = 0.0;
    DecTime  = Time.Hours + (Time.Minutes/60.0) + (Time.Seconds/3600.0); 
    return DecTime;
}

/* UnDecimaliseTime
 * Convert hours to hours, minutes and seconds
 * @param TimeDec hours
 * @return Structure containing all the time info
*/
CC_TIME_T CelestrialConverter::UnDecimaliseTime( float TimeDec )
{
    CC_TIME_T Time;
    /*
        extract the hours.
    */
    Time.Hours = (uint8_t)TimeDec;
    /*
        extract the minutes
    */
    Time.Minutes = (uint8_t)(( TimeDec - Time.Hours ) * 60);
    /*
         extract the seconds
    */
    Time.Seconds = (uint8_t)(((( TimeDec - Time.Hours ) * 60) - Time.Minutes ) * 60 ); 
    
    return Time;
}

/* ConvertTimeToAngle
 * Convert a time to an angle
 * @param Time structure containing all the time info
 * @return float Angle
 */
float CelestrialConverter::ConvertTimeToAngle( CC_TIME_T Time )
{
    float Angle = 0;
    float TimeDec = 0;
    /*
        first convert time to a decimal.
    */
    TimeDec = DecimaliseTime( Time );
    
    /*
        then multiply by 15' and convert to radians
    */
    Angle = TimeDec * (M_PI/12);
    
    return Angle;
}

/* ConvertAngleToTime
 * Convert an angle to a time
 * @param Angle 
 * @return structure containing all the time info
 */
CC_TIME_T CelestrialConverter::ConvertAngleToTime( float Angle )
{
    CC_TIME_T Time;
    
   // angle = (time.hours * 15) + ((time.minutes/60)*15) + ((time.seconds/3600)*15); 
    
    return Time;
}

