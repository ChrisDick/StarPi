/**
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
#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <sys/time.h>
#include <stdint.h>
#include "Runnable.h"
//#include "CelestrialConverter.h"

/** TelescopeManager
 * Class to manage the functionality of the telescope.
 */
class TelescopeManager : public Runnable 
{
    public:
       // CelestrialConverter Calculator;

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

    /**
     */
        static char* SocketCallback ( char* Buffer );

    /** Export the RightAscension and Declination
     */
        static void GetRaDec ( double* Ra, double* Dec );
    /* Export the RightAscension
     */
        float GetRightAscension( void );
    /* Export the Declination
     */
        float GetDeclination( void );
    /** Export the MagneticDeclination
     */
        float GetMagneticDeclination( void );
    /** Export the HieghtAboveGround
     */
        float GetHieghtAboveGround( void );
    /** Export the LatitudeDegrees
     */
        float GetLatitudeDegrees( void );
    /** Export the LongitudeDegrees
     */
        float GetLongitudeDegrees( void );
    /** Export the HeadingDegrees
     */
        float GetHeadingDegrees( void );
    /** Export the Azimuth
     */
        double GetAzimuth( void );
    /** Export the Longitude
     */
        double GetLongitude( void );
    /** Export the Latitude
     */
        double GetLatitude( void );
    /** Export the Year
     */
        uint16_t GetYear( void );
    /** Export the UnixTime
     */
        time_t GetUnixTime( void );
    /** Export the DeclinationHours
     */
        int8_t GetDeclinationHours( void );
    /** Export the DeclinationMinutes
     */
        int8_t GetDeclinationMinutes( void );
    /** Export the DeclinationSeconds
     */
        int8_t GetDeclinationSeconds( void );
    /** Export the RightAscensionHours
     */
        int8_t GetRightAscensionHours( void );
    /** Export the RightAscensionMinutes
     */
        int8_t GetRightAscensionMinutes( void );
    /** Export the RightAscensionSeconds
     */
        int8_t GetRightAscensionSeconds( void );
    /** Export the mode
     */
        int8_t Getmode( void );
    /** Export the AzimuthDegrees
     */
        float GetAzimuthDegrees( void );
    /** Export the Pitch in radians
     */
        float GetPitch( void );
    /** Export the Heading in radians
     */
        float GetHeading( void );
    /** Export the Roll in radians
     */
        float GetRoll( void );
    /** Export the PitchDegrees
     */
        float GetPitchDegrees( void );
    /** Export the Month
     */
        uint16_t GetMonth( void );
    /** Export the Day
     */
        uint16_t GetDay( void );
    /** Export the Hour
     */
        uint16_t GetHour( void );
    /** Export the Minute
     */
        uint16_t GetMinute( void );
    /** Export the Second
     */
        uint16_t GetSecond( void );
    /** Export the british summer time
     */
        bool GetBST( void );
    /** only one instance of this class is required.
     */
        static TelescopeManager Telescope;
        
       // static void testCalculator ( void );
    private:
        static double RightAscension;         /**< Right ascension */
        static double Declination;            /**< Declination */
        static double TargetRightAscension;   /**< Target right ascension */
        static double TargetDeclination;      /**< Target declination */
        static float MagneticDeclination;
        static double Azimuth;
        static double Longitude;
        static double Latitude;
        static float HieghtAboveGround;
        static float LatitudeDegrees;
        static float LongitudeDegrees;
        static float HeadingDegrees;
        static uint16_t Year;
        static time_t UnixTime;
        static int8_t DeclinationHours;
        static int8_t DeclinationMinutes;
        static int8_t DeclinationSeconds;
        static int8_t RightAscensionHours;
        static int8_t RightAscensionMinutes;
        static int8_t RightAscensionSeconds;
        static uint8_t mode;
        static float AzimuthDegrees;
        static struct tm gmt;
        static float Pitch;
        static float Heading;
        static float Roll;
        static float PitchDegrees;

};

#endif /* TELESCOPE_MANAGER_H */