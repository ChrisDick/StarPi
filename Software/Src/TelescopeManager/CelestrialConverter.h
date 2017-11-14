/*
A module to convert between different coordinate systems

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
#include <stdint.h>
#include <time.h>       /* time_t, struct tm, time, gmtime */


/** DATE_T
 * - structure containing date information 
 */
typedef struct {
    uint16_t Year; /**< Year of date, 1BC is year 0, therefore 2BC = -1. */
    uint8_t Month; /**< Month of date, January as 1 through December as 12. */
    uint8_t Day;   /**< Day of date. */
} CC_DATE_T;

/** CC_TIME_T
 * - Structure containing time information. 
 */
typedef struct {
    int16_t Hours;   /**< Number of Hours.   */
    int16_t Minutes; /**< Number of Minutes. */
    float Seconds; /**< Number of Seconds. */
} CC_TIME_T;

/** CC_ANGLES_T
 * - Structure containing all angles.
 * All angles are in radians.
 */
typedef struct {
    double Longitude;    /**< Longitude of current position.                                        */
    double Latitude;         /**< Latitude of the current position.                                     */
    double Azimuth;          /**< Angle of Azimuth of the telescope.                                    */
    double Altitude;         /**< Angle of Altitude of the telescope.                                   */
    double HourAngle;        /**< Hour Angle of the telescope.                                          */
    double RightAscension;   /**< Right Ascension of the position on star map in view of the telescope. */
    double Declination;      /**< Declination of the position on star map in view of the telescope.     */
    CC_TIME_T LocalSiderealCCTime; /**< debug */
    double LocalSiderealTime; /**< debug */
} CC_ANGLES_T;


/** Class to convert between different coordinate systems.
 */
class CelestrialConverter {

    public:
        /** Constructor.
         */
            CelestrialConverter();
        /** Calculate the Local sidereal tiem from the current time and location.
         * @param GrenwichStandardTime - CC_TIME_T - Current time.
         * @param Date CC_DATE_T Current date.
         * @param LongitudeWest - double - Logitude of current position.
         * @return double - Local Sidereal Time in Radians.   
         */
            double CalculateLocalSiderealTime( time_t UnixTime, double LongitudeWest );
        /** Convert equatorial coordinates to celestial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void EquitorialToCelestrial( CC_ANGLES_T* Angles, time_t UnixTime );
        /** Convert celestial coordinates to equatorial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void CelestrialToEquitorial( CC_ANGLES_T* Angles, time_t UnixTime );
        /** Convert hours minutes and seconds to hours.
         * @param Time - tm - structure containing all the time info.
         * @return double - Hours in decimal format.
         */
            double DecimaliseTm( struct tm* Time );
        /** Convert hours minutes and seconds to hours.
         * @param Time - CC_TIME_T - structure containing all the time info.
         * @return double - Hours in decimal format.
         */
            double DecimaliseTime( CC_TIME_T Time );
        /** Convert hours to hours, minutes and seconds.
         * @param TimeDec - double - hours in decimal format.
         * @param Angle CC_TIME_T* - Structure containing all the time info.
         */
            void UnDecimaliseTime( double TimeDec, CC_TIME_T* Angle );
        /** ConvertRadiansToTime
         * Convert an angle in radians to a time.
         * @param Time structure containing all the time info
         * @return double Radians - The Angle
         */
            void ConvertRadiansToTime( double Radians, CC_TIME_T* Time );
        /** ConvertRadiansToDegrees
         * Convert an angle in radians to Degrees.
         * @param Degrees structure containing the result
         * @param double Radians - The Angle
         */
            void ConvertRadiansToDegrees( double Radians, CC_TIME_T* Degrees );
        /** ConvertTimeToAngle
         * Convert a time to an angle
         * @param Time structure containing all the time info
         * @return double Angle
         */
            double ConvertTimeToAngle( CC_TIME_T Time ) ;
#if 0
        /** Calculate the Julian date
         * @param Time - CC_TIME_T Time structure for calculations.
         * @param Date - CC_DATE_T Date structure for calculations.
         * @return Julian date as a double.
         */
            double CalculateJulianDate ( struct tm * gmt );
        /** Add two times together, will wrap around 1 day.
        * @param TimeA - CC_TIME_T - Time to add. 
        * @param TimeB - CC_TIME_T - Time to add.
        * @param Result - CC_TIME_T - Result from adding the two times.
        * @return CC_TIME_T - The sum of the two times. 
        */
            void AddTime( CC_TIME_T TimeA, CC_TIME_T TimeB, CC_TIME_T* Result );
        /** Subtract two times, result will wrap around 1 day.
         * @param TimeA - CC_TIME_T - Time to subtract from.
         * @param TimeB - CC_TIME_T - Time to subtract.
        * @param Result - CC_TIME_T - Result from subtracting the two times
         * @return CC_TIME_T - The subtraction of the two times.
         */
            void SubtractTime( CC_TIME_T TimeA, CC_TIME_T TimeB, CC_TIME_T* Result );
        /** Convert a time to an angle.
         * @param Time - CC_TIME_T - Structure containing all the time info.
         * @return double - Angle.
         */
            double ConvertTimeToRadians( CC_TIME_T Time );
        /** ConvertDegreesToRadians
         * Convert a time to an angle in radians
         * @param Time structure containing all the angle info
         * @return double The angle
         */
            double ConvertDegreesToRadians( CC_TIME_T Degrees );
        /** Convert an angle to a time
         * @param Angle - double - Angle  
         * @param time - CC_TIME_T* - pointer to Structure to contain all the time info
         */
            void ConvertAngleToTime( double Angle, CC_TIME_T* time );
#endif
};