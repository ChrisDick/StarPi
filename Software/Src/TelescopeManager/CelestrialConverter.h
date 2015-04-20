
#include <stdint.h>

/** CC_TIME_T
 * - Structure containing time information. 
 */
typedef struct{
    uint8_t Hours;   /**< Number of Hours.   */
    uint8_t Minutes; /**< Number of Minutes. */
    uint8_t Seconds; /**< Number of Seconds. */
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
         * @param LongitudeWest - float - Logitude of current position.
         * @return float - Local Sidereal Time in Radians.   
         */
            float CalculateLocalSiderealTime( CC_TIME_T GrenwichStandardTime, float LongitudeWest);
        /** Convert equatorial coordinates to celestial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void EquitorialToCelestrial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime );
        /** Convert celestial coordinates to equatorial coordinates. 
        * @param Angles - CC_ANGLES_T* - structure containing all Angles.
        * @param GrenwichStandardTime - CC_TIME_T - Current time.
        */
            void CelestrialToEquitorial( CC_ANGLES_T* Angles, CC_TIME_T GrenwichStandardTime );
        /** Add two times together, will wrap around 1 day.
        * @param TimeA - CC_TIME_T - Time to add. 
        * @param TimeB - CC_TIME_T - Time to add.
        * @return CC_TIME_T - The sum of the two times. 
        */
            CC_TIME_T AddTime( CC_TIME_T TimeA, CC_TIME_T TimeB );
        /** Subtract two times, result will wrap around 1 day.
         * @param TimeA - CC_TIME_T - Time to subtract from.
         * @param TimeB - CC_TIME_T - Time to subtract.
         * @return CC_TIME_T - The subtraction of the two times.
         */
            CC_TIME_T SubtractTime( CC_TIME_T TimeA, CC_TIME_T TimeB );
        /** Convert hours minutes and seconds to hours.
         * @param Time - CC_TIME_T - structure containing all the time info.
         * @return float - Hours in decimal format.
         */
            float DecimaliseTime( CC_TIME_T Time );
        /** Convert hours to hours, minutes and seconds.
         * @param TimeDec - float - hours in decimal format.
         * @return CC_TIME_T - Structure containing all the time info.
         */
            CC_TIME_T UnDecimaliseTime( float TimeDec );
        /** Convert a time to an angle.
         * @param Time - CC_TIME_T - Structure containing all the time info.
         * @return float - Angle.
         */
            float ConvertTimeToAngle( CC_TIME_T Time );
        /** Convert an angle to a time
         * @param Angle - float - Angle  
         * @return CC_TIME_T - Structure containing all the time info
         */
            CC_TIME_T ConvertAngleToTime( float Angle );
        
};