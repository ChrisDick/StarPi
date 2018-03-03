/*
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
    const char Header[5];
    uint8_t (*handler)( char* );
} TELEDATA_T;


class TelescopeSocket 
{
    public:
    /** Constructor
     */
        TelescopeSocket();
    /** callback to handle which message has been sent
     */
        static void SocketCallback ( char* Buffer );
    /** only one instance of this class is required.
     */
        static TelescopeSocket TeleSocket;

        static TELEDATA_T TelescopeData[60];
    /** Handler for an multiple message
     */
        static uint8_t MultiHandler( char* Buffer );
    /** Handler for RightAscension
     */
        static uint8_t RightAscensionHandler( char* Buffer );
    /** Handler for Declination
     */
        static uint8_t DeclinationHandler( char* Buffer );
    /** Handler for TargetRightAscension
     */
        static uint8_t TargetRightAscensionHandler( char* Buffer );
    /** Handler for TargetDeclination
     */
        static uint8_t TargetDeclinationHandler( char* Buffer );
    /** Handler for UnixTime
     */
        static uint8_t UnixTimeHandler( char* Buffer );
    /** Handler for GreenwichMeanTime
     */
        static uint8_t GreenwichMeanTimeDayHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeMon
     */
        static uint8_t GreenwichMeanTimeMonHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeYear
     */
        static uint8_t GreenwichMeanTimeYearHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeHour
     */
        static uint8_t GreenwichMeanTimeHourHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeMin
     */
        static uint8_t GreenwichMeanTimeMinHandler( char* Buffer );
    /** Handler for GreenwichMeanTimeSec
     */
        static uint8_t GreenwichMeanTimeSecHandler( char* Buffer );
    /** Handler for BritishStandardTime
     */
        static uint8_t BritishStandardTimeHandler( char* Buffer );
    /** Handler for Roll
     */
        static uint8_t RollHandler( char* Buffer );
    /** Handler for Altitude/Pitch
     */
        static uint8_t AltitudeHandler( char* Buffer );
    /** Handler for AzimuthHandler
     */
        static uint8_t AzimuthHandler( char* Buffer );
    /** Handler for MagneticHeading
     */
        static uint8_t MagneticHeadingHandler( char* Buffer );
    /** Handler for MagneticDeclination
     */
        static uint8_t MagneticDeclinationHandler( char* Buffer );
    /** Handler for HieghtAboveGround
     */
        static uint8_t HieghtAboveGroundHandler( char* Buffer );
    /** Handler for LocalSidrealTimeHour
     */
        static uint8_t LocalSidrealTimeHourHandler( char* Buffer );
    /** Handler for LocalSidrealTimeMin
     */
        static uint8_t LocalSidrealTimeMinHandler( char* Buffer );
    /** Handler for LocalSidrealTimeSec
     */
        static uint8_t LocalSidrealTimeSecHandler( char* Buffer );
    /** Handler for RightAscensionHours
     */
        static uint8_t RightAscensionHoursHandler( char* Buffer );
    /** Handler for RightAscensionMin
     */
        static uint8_t RightAscensionMinHandler( char* Buffer );
    /** Handler for RightAscensionSec
     */
        static uint8_t RightAscensionSecHandler( char* Buffer );
    /** Handler for DeclinationHours
     */
        static uint8_t DeclinationHoursHandler( char* Buffer );
    /** Handler for DeclinationMinutes
     */
        static uint8_t DeclinationMinutesHandler( char* Buffer );
    /** Handler for DeclinationSeconds
     */
        static uint8_t DeclinationSecondsHandler( char* Buffer );
    /** Handler for Juliandate
     */
        static uint8_t JuliandateHandler( char* Buffer );
    /** Handler for Gpsmode
     */
        static uint8_t GpsmodeHandler( char* Buffer );
    /** Handler for Latitude
     */
        static uint8_t LatitudeHandler( char* Buffer );
    /** Handler for LongitudeHandler
     */
        static uint8_t LongitudeHandler( char* Buffer );
    /** Handler for latitude hours  
     */
        static uint8_t GpsLatitudeHoursHandler( char* Buffer );
    /** Handler for GpsLatitudeMinutes
     */
        static uint8_t GpsLatitudeMinutesHandler( char* Buffer );
    /** Handler for GpsLatitudeSeconds
     */
        static uint8_t GpsLatitudeSecondsHandler( char* Buffer );
    /** Handler for GpsLongitudeHours
     */
        static uint8_t GpsLongitudeHoursHandler( char* Buffer );
    /** Handler for GpsLongitudeMinutes
     */
        static uint8_t GpsLongitudeMinutesHandler( char* Buffer );
    /** Handler for GpsLongitudeMinutes
     */
        static uint8_t GpsLongitudeSecondsHandler( char* Buffer );
    /** Handler for RawAccelerometerX
     */
        static uint8_t RawAccelerometerXHandler( char* Buffer );
    /** Handler for RawAccelerometerY
     */
        static uint8_t RawAccelerometerYHandler( char* Buffer );
    /** Handler for RawAccelerometerZ
     */
        static uint8_t RawAccelerometerZHandler( char* Buffer );
    /** Handler for MinAccelerometerX
     */
        static uint8_t MinAccelerometerXHandler( char* Buffer );
    /** Handler for MinAccelerometerY
     */
        static uint8_t MinAccelerometerYHandler( char* Buffer );
    /** Handler for MinAccelerometerZHandler
     */
        static uint8_t MinAccelerometerZHandler( char* Buffer );
    /** Handler for MaxAccelerometerX
     */
        static uint8_t MaxAccelerometerXHandler( char* Buffer );
    /** Handler for MaxAccelerometerY
     */
        static uint8_t MaxAccelerometerYHandler( char* Buffer );
    /** Handler for MaxAccelerometerZ
     */
        static uint8_t MaxAccelerometerZHandler( char* Buffer );
    /** Handler for RawMagnetometerX
     */
        static uint8_t RawMagnetometerXHandler( char* Buffer );
    /** Handler for RawMagnetometerY
     */
        static uint8_t RawMagnetometerYHandler( char* Buffer );
    /** Handler for RawMagnetometerZ
     */
        static uint8_t RawMagnetometerZHandler( char* Buffer );
    /** Handler for MinMagnetometerX
     */
        static uint8_t MinMagnetometerXHandler( char* Buffer );
    /** Handler for MinMagnetometerY
     */
        static uint8_t MinMagnetometerYHandler( char* Buffer );
    /** Handler for MinMagnetometerZHandler
     */
        static uint8_t MinMagnetometerZHandler( char* Buffer );
    /** Handler for MaxMagnetometerX
     */
        static uint8_t MaxMagnetometerXHandler( char* Buffer );
    /** Handler for MaxMagnetometerY
     */
        static uint8_t MaxMagnetometerYHandler( char* Buffer );
    /** Handler for MaxMagnetometerZ
     */
        static uint8_t MaxMagnetometerZHandler( char* Buffer );
    /** Handler for Calibration Enable
     */
        static uint8_t CalibrationEnableHandler( char* Buffer );
    /** Handler for Magnetic Offset
     */
        static uint8_t MagneticOffsetHandler( char* Buffer );
    /** Handler for Accel Offset
     */
        static uint8_t AccelOffsetHandler( char* Buffer );
    /** Handler for an unknown message
     */
        static uint8_t DefaultHandler( char* Buffer );
        
    private:

};

#endif /* TELESCOPESOCKET_H */