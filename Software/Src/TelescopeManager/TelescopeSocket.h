/**
Telescope Socket is a protocol handler for HalSocket

Author and copyright of this file:
Chris Dick, 2018

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option); any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef TELESCOPESOCKET_H
#define TELESCOPESOCKET_H

#include <stdint.h>

typedef struct
{
    const char Header[5];        /**< Command string                            */
    uint8_t (*handler)( char* ); /**< Pointer to function to handle the message */
} TELEDATA_T;

/** TelescopeSocket
 * - Class to handle the socket interface to the telescope.
 */
class TelescopeSocket 
{
    public:
    /** Constructor
     */
        TelescopeSocket();
    /** callback to handle which message has been sent
     * @param Buffer pointer to the data buffer
     */
        static void SocketCallback ( char* Buffer );
    /** only one instance of this class is required.
     */
        static TelescopeSocket TeleSocket;

        static TELEDATA_T TelescopeData[60]; /**< data buffer */
    /** Handler for an multiple message
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MultiHandler( char* Buffer );
    /** Handler for RightAscension
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RightAscensionHandler( char* Buffer );
    /** Handler for Declination
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t DeclinationHandler( char* Buffer );
    /** Handler for TargetRightAscension
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t TargetRightAscensionHandler( char* Buffer );
    /** Handler for TargetDeclination
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t TargetDeclinationHandler( char* Buffer );
    /** Handler for UnixTime
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t UnixTimeHandler( char* Buffer );
    /** Handler for GreenwichMeanTime
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeDayHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeMon
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeMonHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeYear
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeYearHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeHour
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeHourHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeMin
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeMinHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeSec
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GreenwichMeanTimeSecHandler( char* Buffer );
    /** Handler for BritishStandardTime
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t BritishStandardTimeHandler( char* Buffer );
    /** Handler for Roll
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RollHandler( char* Buffer );
    /** Handler for Altitude/Pitch
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t AltitudeHandler( char* Buffer );
    /** Handler for AzimuthHandler
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t AzimuthHandler( char* Buffer );
    /** Handler for MagneticHeading
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MagneticHeadingHandler( char* Buffer );
    /** Handler for MagneticDeclination
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MagneticDeclinationHandler( char* Buffer );
    /** Handler for HieghtAboveGround
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t HieghtAboveGroundHandler( char* Buffer );
    /** Handler for LocalSidrealTimeHour
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t LocalSidrealTimeHourHandler( char* Buffer );
    /** Handler for LocalSidrealTimeMin
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t LocalSidrealTimeMinHandler( char* Buffer );
    /** Handler for LocalSidrealTimeSec
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t LocalSidrealTimeSecHandler( char* Buffer );
    /** Handler for RightAscensionHours
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RightAscensionHoursHandler( char* Buffer );
    /** Handler for RightAscensionMin
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RightAscensionMinHandler( char* Buffer );
    /** Handler for RightAscensionSec
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RightAscensionSecHandler( char* Buffer );
    /** Handler for DeclinationHours
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t DeclinationHoursHandler( char* Buffer );
    /** Handler for DeclinationMinutes
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t DeclinationMinutesHandler( char* Buffer );
    /** Handler for DeclinationSeconds
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t DeclinationSecondsHandler( char* Buffer );
    /** Handler for Juliandate
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t JuliandateHandler( char* Buffer );
    /** Handler for Gpsmode
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsmodeHandler( char* Buffer );
    /** Handler for Latitude
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t LatitudeHandler( char* Buffer );
    /** Handler for LongitudeHandler
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t LongitudeHandler( char* Buffer );
    /** Handler for latitude hours  
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLatitudeHoursHandler( char* Buffer );
    /** Handler for GpsLatitudeMinutes
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLatitudeMinutesHandler( char* Buffer );
    /** Handler for GpsLatitudeSeconds
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLatitudeSecondsHandler( char* Buffer );
    /** Handler for GpsLongitudeHours
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLongitudeHoursHandler( char* Buffer );
    /** Handler for GpsLongitudeMinutes
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLongitudeMinutesHandler( char* Buffer );
    /** Handler for GpsLongitudeMinutes
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t GpsLongitudeSecondsHandler( char* Buffer );
    /** Handler for RawAccelerometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawAccelerometerXHandler( char* Buffer );
    /** Handler for RawAccelerometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawAccelerometerYHandler( char* Buffer );
    /** Handler for RawAccelerometerZ
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawAccelerometerZHandler( char* Buffer );
    /** Handler for MinAccelerometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinAccelerometerXHandler( char* Buffer );
    /** Handler for MinAccelerometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinAccelerometerYHandler( char* Buffer );
    /** Handler for MinAccelerometerZHandler
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinAccelerometerZHandler( char* Buffer );
    /** Handler for MaxAccelerometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxAccelerometerXHandler( char* Buffer );
    /** Handler for MaxAccelerometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxAccelerometerYHandler( char* Buffer );
    /** Handler for MaxAccelerometerZ
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxAccelerometerZHandler( char* Buffer );
    /** Handler for RawMagnetometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawMagnetometerXHandler( char* Buffer );
    /** Handler for RawMagnetometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawMagnetometerYHandler( char* Buffer );
    /** Handler for RawMagnetometerZ
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t RawMagnetometerZHandler( char* Buffer );
    /** Handler for MinMagnetometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinMagnetometerXHandler( char* Buffer );
    /** Handler for MinMagnetometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinMagnetometerYHandler( char* Buffer );
    /** Handler for MinMagnetometerZHandler
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MinMagnetometerZHandler( char* Buffer );
    /** Handler for MaxMagnetometerX
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxMagnetometerXHandler( char* Buffer );
    /** Handler for MaxMagnetometerY
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxMagnetometerYHandler( char* Buffer );
    /** Handler for MaxMagnetometerZ
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MaxMagnetometerZHandler( char* Buffer );
    /** Handler for Calibration Enable
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t CalibrationEnableHandler( char* Buffer );
    /** Handler for Magnetic Offset
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t MagneticOffsetHandler( char* Buffer );
    /** Handler for Accel Offset
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t AccelOffsetHandler( char* Buffer );
    /** Handler for an unknown message
     * @param Buffer pointer to data buffer
     * @return uint8_t length of string
     */
        static uint8_t DefaultHandler( char* Buffer );
        
    private:

};

#endif /* TELESCOPESOCKET_H */