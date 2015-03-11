
#include "CelestrialConverter.h"
#include <stdint.h>
#include <math.h>

CelestrialConverter::CelestrialConverter()
{

}
/*
    calculate_local_sidereal_time
    
    /!Param gst grenwich standard time 
    /!Param longitude_west 
    /!Return float time in radians.
    
*/
float CelestrialConverter::calculate_local_sidereal_time( CC_TIME_T gst, float longitude_west)
{
    float result = 0;
    float gst_angle = convert_time_to_angle ( gst );
    result = gst_angle - longitude_west;
    return result;
}


void CelestrialConverter::equitorial_to_celestrial( CC_ANGLES_T* angles, CC_TIME_T grenwich_standard_time )
{
    angles->declination = asin( ( ( sin(angles->altitude)*sin(angles->latitude) ) + ( cos(angles->altitude)*cos(angles->latitude)*cos(angles->azimuth) ) ));
    angles->hour_angle = (-1*sin(angles->azimuth)*cos(angles->altitude))/cos(angles->declination);
    angles->right_ascension = calculate_local_sidereal_time( grenwich_standard_time, angles->longitude_west ) - angles->hour_angle;     
}


void CelestrialConverter::celestrial_to_equitorial( CC_ANGLES_T* angles, CC_TIME_T grenwich_standard_time )
{
    
    
}

/*
    add_time
    
    description: time_a + time_b. result will wrap around 1 day.
*/
CC_TIME_T CelestrialConverter::add_time( CC_TIME_T time_a, CC_TIME_T time_b )
{
    CC_TIME_T result;
    /*
        add seconds, then check for completed minutes
    */
    result.seconds = time_a.seconds + time_b.seconds;
    if ( result.seconds >= 60 )
    {
        result.seconds -= 60;
        result.minutes++;
    }
    /*
        add minutes, then check for completed hours.
    */
    result.minutes += time_a.minutes + time_b.minutes;
    if ( result.minutes >= 60 )
    {
        result.minutes -= 60;
        result.hours++;
    }
    /*
        add hours, then remove completed days.
    */
    result.hours += time_a.hours + time_b.hours;
    if ( result.hours >= 24 )
    {
        result.hours -= 24;
    }
    return result;
}

/*
    subtract_time
    
    description: time_a - time_b. result will wrap around 1 day.
*/
CC_TIME_T CelestrialConverter::subtract_time( CC_TIME_T time_a, CC_TIME_T time_b )
{
    CC_TIME_T result;
    /*
        subtract seconds then check for wrap.
    */
    result.seconds = time_a.seconds - time_b.seconds;
    if ( result.seconds > 60 )
    {
        result.seconds += 60;
        result.minutes--;
    }
    
    /*
        subtract minutes then check for wrap.
    */
    result.minutes += time_a.minutes - time_b.minutes;
    if ( result.minutes > 60 )
    {
        result.minutes += 60;
        result.hours--;
    }
    
    /*
        subtract hours then check for wrap.
    */
    result.hours += time_a.hours - time_b.hours;
    if ( result.hours > 24)
    {
        result.hours += 24;
    }
    
    return result;
}


/*
    decimalise_time
*/
float CelestrialConverter::decimalise_time( CC_TIME_T time )
{
    float dec_time = 0.0;
    dec_time  = time.hours + (time.minutes/60.0) + (time.seconds/3600.0); 
    return dec_time;
}

/*
    un_decimalise_time
*/
CC_TIME_T CelestrialConverter::un_decimalise_time( float time_dec )
{
    CC_TIME_T time;
    /*
        extract the hours.
    */
    time.hours = (uint8_t)time_dec;
    /*
        extract the minutes
    */
    time.minutes = (uint8_t)(( time_dec - time.hours ) * 60);
    /*
         extract the seconds
    */
    time.seconds = (uint8_t)(((( time_dec - time.hours ) * 60) - time.minutes ) * 60 ); 
    
    return time;
}

/*
    time_to_seconds
*/
// do I need these?

/*
    seconds_to_time
*/

/*
    convert_time_to_angle
*/
float CelestrialConverter::convert_time_to_angle( CC_TIME_T time )
{
    float angle = 0;
    float time_dec = 0;
    /*
        first convert time to a decimal.
    */
    time_dec = decimalise_time( time );
    
    /*
        then multiply by 15' and convert to radians
    */
    angle = time_dec * (M_PI/12);
    
    return angle;
}

CC_TIME_T CelestrialConverter::convert_angle_to_time( float angle )
{
    CC_TIME_T time;
    
    
    
   // angle = (time.hours * 15) + ((time.minutes/60)*15) + ((time.seconds/3600)*15); 
    
    return time;
}


