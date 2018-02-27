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
    void (*handler)( char* );
} TELEDATA_T;


class TelescopeSocket 
{
    public:
    /* Constructor
     */
        TelescopeSocket();
    /* callback to handle which message has been sent
     */
        static void SocketCallback ( char* Buffer );
    /** only one instance of this class is required.
     */
        static TelescopeSocket TeleSocket;

        static TELEDATA_T TelescopeData[57];
    /* Handler for RightAscension
     */
        static void RightAscensionHandler( char* Buffer );
    /* Handler for Declination
     */
        static void DeclinationHandler( char* Buffer );
    /* Handler for TargetRightAscension
     */
        static void TargetRightAscensionHandler( char* Buffer );
    /* Handler for TargetDeclination
     */
        static void TargetDeclinationHandler( char* Buffer );
    /* Handler for UnixTime
     */
        static void UnixTimeHandler( char* Buffer );
    /* Handler for GreenwichMeanTime
     */
        static void GreenwichMeanTimeDayHandler( char* Buffer );
    /* Handler for GreenwichMeanTimeMon
     */
        static void GreenwichMeanTimeMonHandler( char* Buffer );
    /* Handler for GreenwichMeanTimeYear
     */
        static void GreenwichMeanTimeYearHandler( char* Buffer );
    /* Handler for GreenwichMeanTimeHour
     */
        static void GreenwichMeanTimeHourHandler( char* Buffer );
    /* Handler for GreenwichMeanTimeMin
     */
        static void GreenwichMeanTimeMinHandler( char* Buffer );
    /* Handler for GreenwichMeanTimeSec
     */
        static void GreenwichMeanTimeSecHandler( char* Buffer );
    /* Handler for BritishStandardTime
     */
        static void BritishStandardTimeHandler( char* Buffer );
    /* Handler for Roll
     */
        static void RollHandler( char* Buffer );
    /* Handler for Altitude/Pitch
     */
        static void AltitudeHandler( char* Buffer );
    /* Handler for AzimuthHandler
     */
        static void AzimuthHandler( char* Buffer );
    /* Handler for MagneticHeading
     */
        static void MagneticHeadingHandler( char* Buffer );
    /* Handler for MagneticDeclination
     */
        static void MagneticDeclinationHandler( char* Buffer );
    /* Handler for HieghtAboveGround
     */
        static void HieghtAboveGroundHandler( char* Buffer );
    /* Handler for LocalSidrealTimeHour
     */
        static void LocalSidrealTimeHourHandler( char* Buffer );
    /* Handler for LocalSidrealTimeMin
     */
        static void LocalSidrealTimeMinHandler( char* Buffer );
    /* Handler for LocalSidrealTimeSec
     */
        static void LocalSidrealTimeSecHandler( char* Buffer );
    /* Handler for RightAscensionHours
     */
        static void RightAscensionHoursHandler( char* Buffer );
    /* Handler for RightAscensionMin
     */
        static void RightAscensionMinHandler( char* Buffer );
    /* Handler for RightAscensionSec
     */
        static void RightAscensionSecHandler( char* Buffer );
    /* Handler for DeclinationHours
     */
        static void DeclinationHoursHandler( char* Buffer );
    /* Handler for DeclinationMinutes
     */
        static void DeclinationMinutesHandler( char* Buffer );
    /* Handler for DeclinationSeconds
     */
        static void DeclinationSecondsHandler( char* Buffer );
    /* Handler for Juliandate
     */
        static void JuliandateHandler( char* Buffer );
    /* Handler for Gpsmode
     */
        static void GpsmodeHandler( char* Buffer );
    /* Handler for Latitude
     */
        static void LatitudeHandler( char* Buffer );
    /* Handler for LongitudeHandler
     */
        static void LongitudeHandler( char* Buffer );
    /* Handler for latitude hours  
     */
        static void GpsLatitudeHoursHandler( char* Buffer );
    /* Handler for GpsLatitudeMinutes
     */
        static void GpsLatitudeMinutesHandler( char* Buffer );
    /* Handler for GpsLatitudeSeconds
     */
        static void GpsLatitudeSecondsHandler( char* Buffer );
    /* Handler for GpsLongitudeHours
     */
        static void GpsLongitudeHoursHandler( char* Buffer );
    /* Handler for GpsLongitudeMinutes
     */
        static void GpsLongitudeMinutesHandler( char* Buffer );
    /* Handler for GpsLongitudeMinutes
     */
        static void GpsLongitudeSecondsHandler( char* Buffer );
    /* Handler for RawAccelerometerX
     */
        static void RawAccelerometerXHandler( char* Buffer );
    /* Handler for RawAccelerometerY
     */
        static void RawAccelerometerYHandler( char* Buffer );
    /* Handler for RawAccelerometerZ
     */
        static void RawAccelerometerZHandler( char* Buffer );
    /* Handler for MinAccelerometerX
     */
        static void MinAccelerometerXHandler( char* Buffer );
    /* Handler for MinAccelerometerY
     */
        static void MinAccelerometerYHandler( char* Buffer );
    /* Handler for MinAccelerometerZHandler
     */
        static void MinAccelerometerZHandler( char* Buffer );
    /* Handler for MaxAccelerometerX
     */
        static void MaxAccelerometerXHandler( char* Buffer );
    /* Handler for MaxAccelerometerY
     */
        static void MaxAccelerometerYHandler( char* Buffer );
    /* Handler for MaxAccelerometerZ
     */
        static void MaxAccelerometerZHandler( char* Buffer );
    /* Handler for RawMagnetometerX
     */
        static void RawMagnetometerXHandler( char* Buffer );
    /* Handler for RawMagnetometerY
     */
        static void RawMagnetometerYHandler( char* Buffer );
    /* Handler for RawMagnetometerZ
     */
        static void RawMagnetometerZHandler( char* Buffer );
    /* Handler for MinMagnetometerX
     */
        static void MinMagnetometerXHandler( char* Buffer );
    /* Handler for MinMagnetometerY
     */
        static void MinMagnetometerYHandler( char* Buffer );
    /* Handler for MinMagnetometerZHandler
     */
        static void MinMagnetometerZHandler( char* Buffer );
    /* Handler for MaxMagnetometerX
     */
        static void MaxMagnetometerXHandler( char* Buffer );
    /* Handler for MaxMagnetometerY
     */
        static void MaxMagnetometerYHandler( char* Buffer );
    /* Handler for MaxMagnetometerZ
     */
        static void MaxMagnetometerZHandler( char* Buffer );
    /* Handler for an unknown message
     */
        static void DefaultHandler( char* Buffer );
        
    private:

};

#endif /* TELESCOPESOCKET_H */