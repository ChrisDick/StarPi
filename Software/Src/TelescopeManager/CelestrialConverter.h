
#include <stdint.h>

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
    uint8_t Hours;   /**< Number of Hours.   */
    uint8_t Minutes; /**< Number of Minutes. */
    float Seconds; /**< Number of Seconds. */
} CC_TIME_T;

/** CC_ANGLES_T
 * - Structure containing all angles.
 * All angles are in radians.
 */
typedef struct {
    float LongitudeWest;    /**< Longitude of current position.                                        */
    float Latitude;         /**< Latitude of the current position.                                     */
    float Azimuth;          /**< Angle of Azimuth of the telescope.                                    */
    float Altitude;         /**< Angle of Altitude of the telescope.                                   */
    float HourAngle;        /**< Hour Angle of the telescope.                                          */
    float RightAscension;   /**< Right Ascension of the position on star map in view of the telescope. */
    float Declination;      /**< Declination of the position on star map in view of the telescope.     */
    CC_TIME_T LocalSiderealCCTime; /**< debug */
    float LocalSiderealTime; /**< debug */
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
         * @param LongitudeWest - float - Logitude of current position.
         * @return float - Local Sidereal Time in Radians.   
         */
            float CalculateLocalSiderealTime( CC_TIME_T GrenwichMeanTime, CC_DATE_T Date, float LongitudeWest );
        /** Convert equatorial coordinates to celestial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void EquitorialToCelestrial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime, CC_DATE_T Date );
        /** Convert celestial coordinates to equatorial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void CelestrialToEquitorial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime );
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
        /** Convert hours minutes and seconds to hours.
         * @param Time - CC_TIME_T - structure containing all the time info.
         * @return float - Hours in decimal format.
         */
            float DecimaliseTime( CC_TIME_T Time );
        /** Convert hours to hours, minutes and seconds.
         * @param TimeDec - float - hours in decimal format.
         * @param Angle CC_TIME_T* - Structure containing all the time info.
         */
            void UnDecimaliseTime( float TimeDec, CC_TIME_T* Angle );
        /** ConvertRadiansToTime
         * Convert an angle in radians to a time.
         * @param Time structure containing all the time info
         * @return float Radians - The Angle
         */
            void ConvertRadiansToTime( float Radians, CC_TIME_T* Time );
        /** Convert a time to an angle.
         * @param Time - CC_TIME_T - Structure containing all the time info.
         * @return float - Angle.
         */
            float ConvertTimeToRadians( CC_TIME_T Time );
        /** ConvertRadiansToDegrees
         * Convert an angle in radians to Degrees.
         * @param Degrees structure containing the result
         * @param float Radians - The Angle
         */
            void ConvertRadiansToDegrees( float Radians, CC_TIME_T* Degrees );
        /** ConvertDegreesToRadians
         * Convert a time to an angle in radians
         * @param Time structure containing all the angle info
         * @return float The angle
         */
            float ConvertDegreesToRadians( CC_TIME_T Degrees );
        /** ConvertTimeToAngle
         * Convert a time to an angle
         * @param Time structure containing all the time info
         * @return float Angle
         */
            float ConvertTimeToAngle( CC_TIME_T Time ) ;
        /** Convert an angle to a time
         * @param Angle - float - Angle  
         * @param time - CC_TIME_T* - pointer to Structure to contain all the time info
         */
            void ConvertAngleToTime( float Angle, CC_TIME_T* time );

        /** Calculate the Julian date
         * @param Time - CC_TIME_T Time structure for calculations.
         * @param Date - CC_DATE_T Date structure for calculations.
         * @return Julian date as a float.
         */
            float CalculateJulianDate ( CC_TIME_T Time, CC_DATE_T Date );
};