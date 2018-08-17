/*
Telescope Socket is a protocol handler for HalSocket

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
#include <stdio.h>
#include <string.h>
#include "TelescopeManager.h"
#include "TelescopeOrientation.h"

#include "TelescopeSocket.h"
TelescopeSocket TelescopeSocket::TeleSocket;

#define BUFFER_SIZE 256
/*
    Command table - Each callback must update the return buffer and return how much data has been added.
*/
TELEDATA_T TelescopeSocket::TelescopeData[60] =
{
/* Id                     Header  Handler                                                                      */
/* Multi Item            */ { "MULT", &MultiHandler                 }, /**< any debug string                   */
/* RightAscension        */ { "RA  ", &RightAscensionHandler        }, /**< Target Declination                 */
/* Declination           */ { "DEC ", &DeclinationHandler           }, /**< Target Declination                 */
/* TargetRightAscension  */ { "TRA ", &TargetRightAscensionHandler  }, /**< Target Declination                 */
/* TargetDeclination     */ { "TDEC", &TargetDeclinationHandler     }, /**< Target Declination                 */
/* Unixtime              */ { "Unix", &UnixTimeHandler              }, /**< UnixTime                           */
/* GreenwichMeanTimeDay  */ { "GMTD", &GreenwichMeanTimeDayHandler  }, /**< gmt->tm_mday                       */
/* GreenwichMeanTimeMon  */ { "GMTM", &GreenwichMeanTimeMonHandler  }, /**< gmt->tm_mon                        */
/* GreenwichMeanTimeYear */ { "GMTY", &GreenwichMeanTimeYearHandler }, /**< (gmt->tm_year + 1900)              */
/* GreenwichMeanTimeHour */ { "GMTH", &GreenwichMeanTimeHourHandler }, /**< gmt->tm_hour                       */
/* GreenwichMeanTimeMin  */ { "GMTm", &GreenwichMeanTimeMinHandler  }, /**< gmt->tm_min                        */
/* GreenwichMeanTimeSec  */ { "GMTS", &GreenwichMeanTimeSecHandler  }, /**< gmt->tm_sec                        */
/* BritishStandardTime   */ { "BST ", &BritishStandardTimeHandler   }, /**< gmt->tm_isdst                      */
/* Altitude              */ { "Pitc", &AltitudeHandler              }, /**< AltitudePitch in degrees           */
/* Azimuth               */ { "Azim", &AzimuthHandler               }, /**< Azimuth/Heading in degrees         */
/* Roll                  */ { "Roll", &RollHandler                  }, /**< Roll in degrees                    */
/* Height                */ { "High", &HieghtAboveGroundHandler     }, /**< Hieght Above Ground (km)           */
/* MagneticDeclination   */ { "MagD", &MagneticDeclinationHandler   }, /**< Magnetic Declination               */
/* MagneticHeading       */ { "MagH", &MagneticHeadingHandler       }, /**< (180*(Heading/M_PI))               */
/* LocalSidrealTimehour  */ { "LSTH", &LocalSidrealTimeHourHandler  }, /**< Local Sidereal Time Hours          */
/* LocalSidrealTimemin   */ { "LSTm", &LocalSidrealTimeMinHandler   }, /**< Local Sidereal Time Minutes        */
/* LocalSidrealTimesec   */ { "LSTS", &LocalSidrealTimeSecHandler   }, /**< Local Sidereal Time Seconds        */
/* RightAscensionHours   */ { "RAH ", &RightAscensionHoursHandler   }, /**< Right Ascension Degrees            */
/* RightAscensionMin     */ { "RAm ", &RightAscensionMinHandler     }, /**< Right Ascension Minutes            */
/* RightAscensionSec     */ { "RAS ", &RightAscensionSecHandler     }, /**< Right Ascension Seconds            */
/* DeclinationHours      */ { "DECH", &DeclinationHoursHandler      }, /**< Declination Hours                  */
/* DeclinationMinutes    */ { "DECm", &DeclinationMinutesHandler    }, /**< Declination Minutes                */
/* DeclinationSeconds    */ { "DECS", &DeclinationSecondsHandler    }, /**< Declination Seconds                */
/* Juliandate            */ { "JDAT", &JuliandateHandler            }, /**< JulianDate                         */
/* Gpsmode               */ { "GPSM", &GpsmodeHandler               }, /**< Gps Fix Mode                       */
/* Latitude              */ { "Lati", &LatitudeHandler              }, /**< (180*(Angles.Latitude/M_PI))       */
/* Longitude             */ { "Long", &LongitudeHandler             }, /**< (180*(Angles.LongitudeWest/M_PI))  */
/* GpsLatitudeHours      */ { "GLAH", &GpsLatitudeHoursHandler      }, /**< Gps source latitude degrees        */
/* GpsLatitudeMinutes    */ { "GLAm", &GpsLatitudeMinutesHandler    }, /**< Gps source latitude minutes        */
/* GpsLatitudeSeconds    */ { "GLAS", &GpsLatitudeSecondsHandler    }, /**< Gps source latitude seconds        */
/* GpsLongitudeHours     */ { "GLOH", &GpsLongitudeHoursHandler     }, /**< Gps source longitude hours         */
/* GpsLongitudeMinutes   */ { "GLOm", &GpsLongitudeMinutesHandler   }, /**< Gps source longitude minutes       */
/* GpsLongitudeSeconds   */ { "GLOS", &GpsLongitudeSecondsHandler   }, /**< Gps source longitude seconds       */
/* RawAccelerometerX     */ { "RwAx", &RawAccelerometerXHandler     }, /**< Raw data from the Accelerometer    */
/* RawAccelerometerY     */ { "RwAy", &RawAccelerometerYHandler     }, /**< Raw data from the Accelerometer    */
/* RawAccelerometerZ     */ { "RwAz", &RawAccelerometerZHandler     }, /**< Raw data from the Accelerometer    */
/* MinAccelerometerX     */ { "MiAx", &MinAccelerometerXHandler     }, /**< Min data from the Accelerometer    */
/* MinAccelerometerY     */ { "MiAy", &MinAccelerometerYHandler     }, /**< Min data from the Accelerometer    */
/* MinAccelerometerZ     */ { "MiAz", &MinAccelerometerZHandler     }, /**< Min data from the Accelerometer    */
/* MaxAccelerometerX     */ { "MaAx", &MaxAccelerometerXHandler     }, /**< Max data from the Accelerometer    */
/* MaxAccelerometerY     */ { "MaAy", &MaxAccelerometerYHandler     }, /**< Max data from the Accelerometer    */
/* MaxAccelerometerZ     */ { "MaAz", &MaxAccelerometerZHandler     }, /**< Max data from the Accelerometer    */
/* RawMagnetometerX      */ { "RwMx", &RawMagnetometerXHandler      }, /**< Raw data from the Magnetometer     */
/* RawMagnetometerY      */ { "RwMy", &RawMagnetometerYHandler      }, /**< Raw data from the Magnetometer     */
/* RawMagnetometerZ      */ { "RwMz", &RawMagnetometerZHandler      }, /**< Raw data from the Magnetometer     */
/* MinMagnetometerX      */ { "MiMx", &MinMagnetometerXHandler      }, /**< Min data from the Magnetometer     */
/* MinMagnetometerY      */ { "MiMy", &MinMagnetometerYHandler      }, /**< Min data from the Magnetometer     */
/* MinMagnetometerZ      */ { "MiMz", &MinMagnetometerZHandler      }, /**< Min data from the Magnetometer     */
/* MaxMagnetometerX      */ { "MaMx", &MaxMagnetometerXHandler      }, /**< Max data from the Magnetometer     */
/* MaxMagnetometerY      */ { "MaMy", &MaxMagnetometerYHandler      }, /**< Max data from the Magnetometer     */
/* MaxMagnetometerZ      */ { "MaMz", &MaxMagnetometerZHandler      }, /**< Max data from the Magnetometer     */
/* Calibration Enable    */ { "CALE", &CalibrationEnableHandler     }, /**< Calibration enable                 */
/* MagneticOffset        */ { "MAGO", &MagneticOffsetHandler        }, /**< user offset to Azimuth             */
/* AccelOffset           */ { "ACCO", &AccelOffsetHandler           }, /**< user offsset to Altitude           */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
};

#define HEADERLENGTH sizeof(TelescopeData[0].Header)
#define NUMBER_OF_HANDLERS  ((uint8_t)(sizeof(TelescopeData)/sizeof(TelescopeData[0])))
/* Constructor
 */
TelescopeSocket::TelescopeSocket()
{
    
}

/* callback to handle which message has been sent
 */
void TelescopeSocket::SocketCallback ( char* Buffer )
{
    uint8_t Id = 0u;
    /* Look for the callback */
    for ( Id = 0u; Id < NUMBER_OF_HANDLERS; Id++ )
    {
        if ( strncmp ( Buffer, TelescopeData[Id].Header, 4 ) == 0)
        {
            break;
        }
    }
    if (Id >= NUMBER_OF_HANDLERS )
    {
        Id = NUMBER_OF_HANDLERS - 1;
    }

    (void)TelescopeData[Id].handler(  Buffer );
}


/* Handler for an multiple message
 */
uint8_t TelescopeSocket::MultiHandler( char* Buffer )
{
    char ReturnBuffer[BUFFER_SIZE];
    uint8_t ReturnIndex = 0u;
    uint8_t Id = 0u;
    /*
       we need to iterate through the received string and pick out each command
    */
    bool looking = true;
    /* we can ignore the "MULT" command */
    uint8_t Index = sizeof( TelescopeData[0].Header );
    while ( looking )
    {
        /* look for a matching command */
        for ( Id = 0u; Id < NUMBER_OF_HANDLERS; Id++ )
        {
            if ( strncmp ( &Buffer[Index], TelescopeData[Id].Header, 4 ) == 0)
            {
                break;
            }
        }
        /* if no handler is found stop looking */
        if (Id >= NUMBER_OF_HANDLERS )
        {
            looking = false;
            /* If this was the first time call the default callback */
            if( Index == sizeof( TelescopeData[0].Header ) )
            {
                ReturnIndex += TelescopeData[(NUMBER_OF_HANDLERS - 1)].handler( &ReturnBuffer[ReturnIndex] );
            }
        }
        else
        {
            /* copy the command to the return buffer in case it's a setter */
            strncpy( &ReturnBuffer[ReturnIndex], &Buffer[Index], strcspn( &Buffer[Index], "#" ) );
            /* call the callback */
            ReturnIndex += TelescopeData[Id].handler( &ReturnBuffer[ReturnIndex] );
            /* increase the index to the next command, including the # */
            Index += strcspn( &Buffer[Index], "#" ) + 1u; 
        }
        if ( ReturnIndex >= BUFFER_SIZE - sizeof( TelescopeData[0].Header ) )
        {
            looking = false;
        }
    }
    /* check for too much data */
    if ( ReturnIndex < BUFFER_SIZE - sizeof( TelescopeData[0].Header )  )
    {
        strncpy ( &Buffer[sizeof( TelescopeData[0].Header )], ReturnBuffer, ReturnIndex );
    }
    else
    {
        sprintf( &Buffer[sizeof( TelescopeData[0].Header )], "Not Supported#" );
    }
    return ReturnIndex;
}

/* Handler for TargetRightAscension
 */
uint8_t TelescopeSocket::RightAscensionHandler( char* Buffer )
{
    sprintf( Buffer, "RA__=%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetRightAscension( ));   
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for TargetDeclination
 */
uint8_t TelescopeSocket::DeclinationHandler( char* Buffer )
{
    sprintf( Buffer, "DEC_=%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetDeclination( ));   
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for TargetRightAscension
 */
uint8_t TelescopeSocket::TargetRightAscensionHandler( char* Buffer )
{
    sprintf( Buffer, "TRA =%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetRightAscension( ));   
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for TargetDeclination
 */
uint8_t TelescopeSocket::TargetDeclinationHandler( char* Buffer )
{
    sprintf( Buffer, "TDEC=%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetDeclination( ));   
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for UnixTime
 */
uint8_t TelescopeSocket::UnixTimeHandler( char* Buffer )
{
    sprintf( Buffer, "Unix=%d#", (int32_t)TelescopeManager::Telescope.TelescopeManager::GetUnixTime( ));   
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTime
 */
uint8_t TelescopeSocket::GreenwichMeanTimeDayHandler( char* Buffer )
{
    sprintf( Buffer, "GMTD=%d#", TelescopeManager::Telescope.TelescopeManager::GetDay( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTimeMon
 */
uint8_t TelescopeSocket::GreenwichMeanTimeMonHandler( char* Buffer )
{
    sprintf( Buffer, "GMTM=%d#", TelescopeManager::Telescope.TelescopeManager::GetMonth( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTimeYear
 */
uint8_t TelescopeSocket::GreenwichMeanTimeYearHandler( char* Buffer )
{
    sprintf( Buffer, "GMTY=%d#", TelescopeManager::Telescope.TelescopeManager::GetYear( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTimeHour
 */
uint8_t TelescopeSocket::GreenwichMeanTimeHourHandler( char* Buffer )
{
    sprintf( Buffer, "GMTH=%d#", TelescopeManager::Telescope.TelescopeManager::GetHour( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTimeMin
 */
uint8_t TelescopeSocket::GreenwichMeanTimeMinHandler( char* Buffer )
{
    sprintf( Buffer, "GMTm=%d#", TelescopeManager::Telescope.TelescopeManager::GetMinute( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GreenwichMeanTimeSec
 */
uint8_t TelescopeSocket::GreenwichMeanTimeSecHandler( char* Buffer )
{
    sprintf( Buffer, "GMTS=%d#", TelescopeManager::Telescope.TelescopeManager::GetSecond( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for BritishStandardTime
 */
uint8_t TelescopeSocket::BritishStandardTimeHandler( char* Buffer )
{
    sprintf( Buffer, "BST =%d#", (uint8_t)TelescopeManager::Telescope.TelescopeManager::GetBST( )); 
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Roll
 */
uint8_t TelescopeSocket::RollHandler( char* Buffer )
{
    sprintf( Buffer, "Roll=%f#", TelescopeManager::Telescope.GetRoll( ));
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for Altitude/Pitch
 */
uint8_t TelescopeSocket::AltitudeHandler( char* Buffer )
{
    sprintf( Buffer, "Pitc=%f#", TelescopeManager::Telescope.GetPitchDegrees( ));    
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for AzimuthHandler
 */
uint8_t TelescopeSocket::AzimuthHandler( char* Buffer )
{
    sprintf( Buffer, "Azim=%f#", (float)TelescopeManager::Telescope.GetAzimuthDegrees( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MagneticHeading in degrees
 */
uint8_t TelescopeSocket::MagneticHeadingHandler( char* Buffer )
{
    sprintf( Buffer, "MagH=%f#", TelescopeManager::Telescope.GetHeadingDegrees( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MagneticDeclination
 */
uint8_t TelescopeSocket::MagneticDeclinationHandler( char* Buffer )
{
    sprintf( Buffer, "MagD=%f#", TelescopeManager::Telescope.GetMagneticDeclination( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for HieghtAboveGround
 */
uint8_t TelescopeSocket::HieghtAboveGroundHandler( char* Buffer )
{
    sprintf( Buffer, "High=%f#", TelescopeManager::Telescope.GetHieghtAboveGround( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RightAscensionHours
 */
uint8_t TelescopeSocket::RightAscensionHoursHandler( char* Buffer )
{
    sprintf( Buffer, "RAH =%d#", TelescopeManager::Telescope.GetRightAscensionHours( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RightAscensionMin
 */
uint8_t TelescopeSocket::RightAscensionMinHandler( char* Buffer )
{
    sprintf( Buffer, "RAm =%d#", TelescopeManager::Telescope.GetRightAscensionMinutes( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RightAscensionSec
 */
uint8_t TelescopeSocket::RightAscensionSecHandler( char* Buffer )
{
    sprintf( Buffer, "RAS =%d#", TelescopeManager::Telescope.GetRightAscensionSeconds( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for DeclinationHours
 */
uint8_t TelescopeSocket::DeclinationHoursHandler( char* Buffer )
{
    sprintf( Buffer, "DECH=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationHours( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for DeclinationMinutes
 */
uint8_t TelescopeSocket::DeclinationMinutesHandler( char* Buffer )
{
    sprintf( Buffer, "DECm=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationMinutes( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for DeclinationSeconds
 */
uint8_t TelescopeSocket::DeclinationSecondsHandler( char* Buffer )
{
    sprintf( Buffer, "DECS=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationSeconds( ));     
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Juliandate
 */
uint8_t TelescopeSocket::JuliandateHandler( char* Buffer )
{
    DefaultHandler( Buffer );
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Gpsmode
 */
uint8_t TelescopeSocket::GpsmodeHandler( char* Buffer )
{
    sprintf( Buffer, "GPSM=%d#", TelescopeManager::Telescope.TelescopeManager::Getmode( ));     
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for Latitude
 */
uint8_t TelescopeSocket::LatitudeHandler( char* Buffer )
{
    sprintf( Buffer, "Lati=%f#", (float)TelescopeManager::Telescope.GetLatitudeDegrees( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for LongitudeHandler
 */
uint8_t TelescopeSocket::LongitudeHandler( char* Buffer )
{
    sprintf( Buffer, "Long=%f#", (float)TelescopeManager::Telescope.GetLongitudeDegrees( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for latitude hours  
 */
uint8_t TelescopeSocket::GpsLatitudeHoursHandler( char* Buffer )
{
    sprintf( Buffer, "GLAH=%d#", TelescopeManager::Telescope.GetLatitudeHours( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GpsLatitudeMinutes
 */
uint8_t TelescopeSocket::GpsLatitudeMinutesHandler( char* Buffer )
{
    sprintf( Buffer, "GLAm=%d#", TelescopeManager::Telescope.GetLatitudeMinutes( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GpsLatitudeSeconds
 */
uint8_t TelescopeSocket::GpsLatitudeSecondsHandler( char* Buffer )
{
    sprintf( Buffer, "GLAS=%d#", TelescopeManager::Telescope.GetLatitudeSeconds( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GpsLongitudeHours
 */
uint8_t TelescopeSocket::GpsLongitudeHoursHandler( char* Buffer )
{
    sprintf( Buffer, "GLOH=%d#", TelescopeManager::Telescope.GetLongitudeHours( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GpsLongitudeMinutes
 */
uint8_t TelescopeSocket::GpsLongitudeMinutesHandler( char* Buffer )
{
    sprintf( Buffer, "GLOm=%d#", TelescopeManager::Telescope.GetLongitudeMinutes( ));
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for GpsLongitudeMinutes
 */
uint8_t TelescopeSocket::GpsLongitudeSecondsHandler( char* Buffer )
{
    sprintf( Buffer, "GLOS=%d#", TelescopeManager::Telescope.GetLongitudeSeconds( ));
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for LocalSidrealTimeHour
 */
uint8_t TelescopeSocket::LocalSidrealTimeHourHandler( char* Buffer )
{
    DefaultHandler( Buffer );
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for LocalSidrealTimeMin
 */
uint8_t TelescopeSocket::LocalSidrealTimeMinHandler( char* Buffer )
{
    DefaultHandler( Buffer );
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for LocalSidrealTimeSec
 */
uint8_t TelescopeSocket::LocalSidrealTimeSecHandler( char* Buffer )
{
    DefaultHandler( Buffer );
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RawAccelerometerX
 */
uint8_t TelescopeSocket::RawAccelerometerXHandler( char* Buffer )
{
    sprintf( Buffer, "RwAx=%f#", TelescopeOrientation::Orient.GetAx());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RawAccelerometerY
 */
uint8_t TelescopeSocket::RawAccelerometerYHandler( char* Buffer )
{
    sprintf( Buffer, "RwAy=%f#", TelescopeOrientation::Orient.GetAy());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RawAccelerometerZ
 */
uint8_t TelescopeSocket::RawAccelerometerZHandler( char* Buffer )
{
    sprintf( Buffer, "RwAz=%f#", TelescopeOrientation::Orient.GetAz());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MinAccelerometerX
 */
uint8_t TelescopeSocket::MinAccelerometerXHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiAxReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAxMin();
    }
    sprintf( Buffer, "MiAx=%f#", TelescopeOrientation::Orient.GetAxMin());
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for MinAccelerometerY
 */
uint8_t TelescopeSocket::MinAccelerometerYHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiAyReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAyMin();
    }
    sprintf( Buffer, "MiAy=%f#", TelescopeOrientation::Orient.GetAyMin());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MinAccelerometerZHandler
 */
uint8_t TelescopeSocket::MinAccelerometerZHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiAzReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAzMin();
    }
    sprintf( Buffer, "MiAz=%f#", TelescopeOrientation::Orient.GetAzMin());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxAccelerometerX
 */
uint8_t TelescopeSocket::MaxAccelerometerXHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaAxReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAxMax();
    }
    sprintf( Buffer, "MaAx=%f#", TelescopeOrientation::Orient.GetAxMax());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxAccelerometerY
 */
uint8_t TelescopeSocket::MaxAccelerometerYHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaAyReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAyMax();
    }
    sprintf( Buffer, "MaAy=%f#", TelescopeOrientation::Orient.GetAyMax());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxAccelerometerZ
 */
uint8_t TelescopeSocket::MaxAccelerometerZHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaAzReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetAzMax();
    }
    sprintf( Buffer, "MaAz=%f#", TelescopeOrientation::Orient.GetAzMax());
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for RawMagnetometerX
 */
uint8_t TelescopeSocket::RawMagnetometerXHandler( char* Buffer )
{
    sprintf( Buffer, "RwMx=%f#", TelescopeOrientation::Orient.GetMx());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RawMagnetometerY
 */
uint8_t TelescopeSocket::RawMagnetometerYHandler( char* Buffer )
{
    sprintf( Buffer, "RwMy=%f#", TelescopeOrientation::Orient.GetMy());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for RawMagnetometerZ
 */
uint8_t TelescopeSocket::RawMagnetometerZHandler( char* Buffer )
{
    sprintf( Buffer, "RwMz=%f#", TelescopeOrientation::Orient.GetMz());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MinMagnetometerX
 */
uint8_t TelescopeSocket::MinMagnetometerXHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiMxReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMxMin();
    }
    sprintf( Buffer, "MiMx=%f#", TelescopeOrientation::Orient.GetMxMin());
    return ( strcspn (Buffer, "#") + 1u );
}
/* Handler for MinMagnetometerY
 */
uint8_t TelescopeSocket::MinMagnetometerYHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiMyReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMyMin();
    }
    sprintf( Buffer, "MiMy=%f#", TelescopeOrientation::Orient.GetMyMin());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MinMagnetometerZHandler
 */
uint8_t TelescopeSocket::MinMagnetometerZHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MiMzReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMzMin();
    }
    sprintf( Buffer, "MiMz=%f#", TelescopeOrientation::Orient.GetMzMin());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxMagnetometerX
 */
uint8_t TelescopeSocket::MaxMagnetometerXHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaMxReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMxMax();
    }
    sprintf( Buffer, "MaMx=%f#", TelescopeOrientation::Orient.GetMxMax());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxMagnetometerY
 */
uint8_t TelescopeSocket::MaxMagnetometerYHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaMyReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMyMax();
    }
    sprintf( Buffer, "MaMy=%f#", TelescopeOrientation::Orient.GetMyMax());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for MaxMagnetometerZ
 */
uint8_t TelescopeSocket::MaxMagnetometerZHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MaMzReset", 9 ) == 0 )
    {
        TelescopeOrientation::Orient.ResetMzMax();
    }
    sprintf( Buffer, "MaMz=%f#", TelescopeOrientation::Orient.GetMzMax());
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Calibration Enable
 */
uint8_t TelescopeSocket::CalibrationEnableHandler( char* Buffer )
{
    if ( strncmp( Buffer, "CALE=Enable", 11 ) == 0 )
    {
        sprintf( Buffer, "CALE=%s#", "Enabled");
        TelescopeOrientation::Orient.EnableCalibration(true);
    }
    else
    {
        sprintf( Buffer, "CALE=%s#", "Disabled");
        TelescopeOrientation::Orient.EnableCalibration(false);
    }
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Magnetic Offset
 */
uint8_t TelescopeSocket::MagneticOffsetHandler( char* Buffer )
{
    if ( strncmp( Buffer, "MAGO=", 5 ) == 0 )
    {
        float Offset = 0.0f;
        sscanf( Buffer, "MAGO=%f#", &Offset );
        TelescopeOrientation::Orient.SetMagneticOffset( Offset );
        sprintf( Buffer, "MAGO=%f#", TelescopeOrientation::Orient.GetMagneticOffset());
    }
    else
    {
        sprintf( Buffer, "MAGO=%f#", TelescopeOrientation::Orient.GetMagneticOffset());
    }
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for Accel Offset
 */
uint8_t TelescopeSocket::AccelOffsetHandler( char* Buffer )
{
    if ( strncmp( Buffer, "ACCO=", 5 ) == 0 )
    {
        float Offset = 0.0f;
        sscanf( Buffer, "ACCO=%f#", &Offset );
        TelescopeOrientation::Orient.SetAccelOffset( Offset );
        sprintf( Buffer, "ACCO=%f#", TelescopeOrientation::Orient.GetAccelOffset());
    }
    else
    {
        sprintf( Buffer, "ACCO=%f#", TelescopeOrientation::Orient.GetAccelOffset());
    }    
    return ( strcspn (Buffer, "#") + 1u );
}

/* Handler for an unknown message
 */
uint8_t TelescopeSocket::DefaultHandler( char* Buffer )
{
    sprintf( Buffer, "Not Supported#" );
    return ( strcspn (Buffer, "#") + 1u );
}
