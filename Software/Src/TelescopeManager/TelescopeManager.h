/**
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
#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <sys/time.h>
#include <stdint.h>
#include "Runnable.h"

/** TelescopeManager
 * Class to manage the functionality of the telescope.
 */
class TelescopeManager : public Runnable 
{
    public:
    /** constructor
     */
        TelescopeManager();
    /** Telescope initialisation
     */
        void Init( void );
    /** main run function of the telescope manager
    */
        void Run( void );
    /** interface to set the target
     * @param Ra
     * @param Dec     
     */
        static void SetGotoTarget(double Ra, double Dec);
    /** Export the RightAscension and Declination
     * @param Ra pointer
     * @param Dec pointer
     */
        static void GetRaDec ( double* Ra, double* Dec );
    /** Export the RightAscension
     * @return float RightAscension in radians
     */
        float GetRightAscension( void );
    /** Export the Declination
     * @return float Declination in radians
     */
        float GetDeclination( void );
    /** Export the MagneticDeclination
     * @return float Magnetic declination in degrees
     */
        float GetMagneticDeclination( void );
    /** Export the HieghtAboveGround
     * @return float Hieght above ground in km
     */
        float GetHieghtAboveGround( void );
    /** Export the LatitudeDegrees
     * @return float Latitude in degrees
     */
        float GetLatitudeDegrees( void );
    /** Export the LongitudeDegrees
     * @return float Longitude in degrees
     */
        float GetLongitudeDegrees( void );
    /** Export the HeadingDegrees
     * @return float Heading in degrees
     */
        float GetHeadingDegrees( void );
    /** Export the Azimuth
     * @return double Azimuth in radians
     */
        double GetAzimuth( void );
    /** Export the Longitude
     * @return double Longitude in radians
     */
        double GetLongitude( void );
    /** Export the Latitude
     * @return double Latitude in radians
     */
        double GetLatitude( void );
    /** Export the Year
     * @return uint16_t year
     */
        uint16_t GetYear( void );
    /** Export the UnixTime
     * @return time_t Unixtime
     */
        time_t GetUnixTime( void );
    /** Export the DeclinationHours
     * @return uint8_t Hours part of Declination
     */
        int8_t GetDeclinationHours( void );
    /** Export the DeclinationMinutes
     * @return uint8_t Minutes part of Declination
     */
        int8_t GetDeclinationMinutes( void );
    /** Export the DeclinationSeconds
     * @return uint8_t Seconds part of Declination
     */
        int8_t GetDeclinationSeconds( void );
    /** Export the RightAscensionHours
     * @return uint8_t Hours part of Right Ascension
     */
        int8_t GetRightAscensionHours( void );
    /** Export the RightAscensionMinutes
     * @return uint8_t Minutes part of Right Ascension
     */
        int8_t GetRightAscensionMinutes( void );
    /** Export the RightAscensionSeconds
     * @return uint8_t Seconds part of Right Ascension
     */
        int8_t GetRightAscensionSeconds( void );
    /** Export the LatitudeHours
     * @return uint8_t Hours part of Latitude
     */
        int8_t GetLatitudeHours( void );
    /** Export the LatitudeMinutes
     * @return uint8_t Minutes part of Latitude
     */
        int8_t GetLatitudeMinutes( void );
    /** Export the LatitudeSeconds
     * @return uint8_t Seconds part of Latitude
     */
        int8_t GetLatitudeSeconds( void );
    /** Export the LongitudeHours
     * @return uint8_t Hours part of Longitude
     */
        int8_t GetLongitudeHours( void );
    /** Export the LongitudeMinutes
     * @return uint8_t Minutes part of Longitude
     */
        int8_t GetLongitudeMinutes( void );
    /** Export the LongitudeSeconds
     * @return uint8_t Seconds part of Longitude
     */
        int8_t GetLongitudeSeconds( void );
    /** Export the mode
     * @return int8_t GPS mode
     */
        int8_t Getmode( void );
    /** Export the AzimuthDegrees
     * @return float Azimuth in degrees
     */
        float GetAzimuthDegrees( void );
    /** Export the Pitch in radians
     * @return float Pitch in degrees
     */
        float GetPitch( void );
    /** Export the Heading in radians
     * @return float Heading in radians
     */
        float GetHeading( void );
    /** Export the Roll in radians
     * @return float Roll in radians
     */
        float GetRoll( void );
    /** Export the PitchDegrees
     * @return float Pitch in degrees
     */
        float GetPitchDegrees( void );
    /** Export the Month
     * @return uint16_t Month
     */
        uint16_t GetMonth( void );
    /** Export the Day
     * @return uint16_t Day
     */
        uint16_t GetDay( void );
    /** Export the Hour
     * @return uint16_t Hours
     */
        uint16_t GetHour( void );
    /** Export the Minute
     * @return uint16_t Minutes
     */
        uint16_t GetMinute( void );
    /** Export the Second
     * @return uint16_t Seconds
     */
        uint16_t GetSecond( void );
    /** Export the British summer time
     * @return bool summer time
     */
        bool GetBST( void );
    /** only one instance of this class is required.
     */
        static TelescopeManager Telescope;
        
    private:
        static double RightAscension;         /**< Right ascension in radians        */
        static double Declination;            /**< Declination in radians            */
        static double TargetRightAscension;   /**< Target right ascension in radians */
        static double TargetDeclination;      /**< Target declination in radians     */
        static float MagneticDeclination;     /**< Magnetic Declination in degrees   */
        static double Azimuth;                /**< Azimuth in radians                */
        static double Longitude;              /**< Longitude in radians              */
        static double Latitude;               /**< Latitude in radians               */
        static float HieghtAboveGround;       /**< Altitude in km                    */
        static float LatitudeDegrees;         /**< Latitude in degrees               */
        static float LongitudeDegrees;        /**< Longitude in degrees              */
        static float HeadingDegrees;          /**< Heading in degrees                */
        static uint16_t Year;                 /**< Current year                      */
        static time_t UnixTime;               /**< Unix time                         */
        static int8_t DeclinationHours;       /**< Hours part of Declination         */
        static int8_t DeclinationMinutes;     /**< Minutes part of Declination       */
        static int8_t DeclinationSeconds;     /**< Seconds part of Declination       */
        static int8_t RightAscensionHours;    /**< Hours part of Right Ascension     */
        static int8_t RightAscensionMinutes;  /**< Minutes part of Right Ascension   */
        static int8_t RightAscensionSeconds;  /**< Seconds part of Right Ascension   */
        static int8_t LongitudeHours;         /**< Hours part of Longitude           */
        static int8_t LongitudeMinutes;       /**< Minutes part of Longitude         */
        static int8_t LongitudeSeconds;       /**< Seconds part of Longitude         */
        static int8_t LatitudeHours;          /**< Hours part of Latitude            */
        static int8_t LatitudeMinutes;        /**< Minutes part of Latitude          */
        static int8_t LatitudeSeconds;        /**< Seconds part of Latitude          */
        static uint8_t mode;                  /**< GPS mode                          */
        static float AzimuthDegrees;          /**< Azimuth in degrees                */
        static struct tm gmt;                 /**< gmt time structure                */
        static float Pitch;                   /**< pitch in radians                  */
        static float Heading;                 /**< heading in radians                */
        static float Roll;                    /**< roll in radians                   */
        static float PitchDegrees;            /**< pitch in degrees                  */
};

#endif /* TELESCOPE_MANAGER_H */