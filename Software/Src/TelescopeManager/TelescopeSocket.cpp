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

#include <string.h>
#include <stdio.h>
#include "TelescopeManager.h"
#include "TelescopeOrientation.h"

#include "TelescopeSocket.h"
TelescopeSocket TelescopeSocket::TeleSocket;



TELEDATA_T TelescopeSocket::TelescopeData[57] =
{
/* Id                     Header  Handler                                                                      */
/* Multi Item            */ { "MULT", &DefaultHandler               }, /**< any debug string                   */
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
/* GpsLatitudeHours      */ { "GLAD", &GpsLatitudeHoursHandler      }, /**< Gps source latitude degrees        */
/* GpsLatitudeMinutes    */ { "GLAm", &GpsLatitudeMinutesHandler    }, /**< Gps source latitude minutes        */
/* GpsLatitudeSeconds    */ { "GLAS", &GpsLatitudeSecondsHandler    }, /**< Gps source latitude seconds        */
/* GpsLongitudeHours     */ { "GLOH", &GpsLongitudeHoursHandler     }, /**< Gps source longitude hours         */
/* GpsLongitudeMinutes   */ { "GLOm", &GpsLongitudeMinutesHandler   }, /**< Gps source longitude minutes       */
/* GpsLongitudeSeconds   */ { "GLOS", &GpsLongitudeSecondsHandler   }, /**< Gps source longitude seconds       */
/* RawAccelerometerX     */ { "RwAx", &RawAccelerometerXHandler     }, /**<                                    */
/* RawAccelerometerY     */ { "RwAy", &RawAccelerometerYHandler     }, /**<                                    */
/* RawAccelerometerZ     */ { "RwAz", &RawAccelerometerZHandler     }, /**<                                    */
/* MinAccelerometerX     */ { "MiAx", &MinAccelerometerXHandler     }, /**<                                    */
/* MinAccelerometerY     */ { "MiAy", &MinAccelerometerYHandler     }, /**<                                    */
/* MinAccelerometerZ     */ { "MiAz", &MinAccelerometerZHandler     }, /**<                                    */
/* MaxAccelerometerX     */ { "MaAx", &MaxAccelerometerXHandler     }, /**<                                    */
/* MaxAccelerometerY     */ { "MaAy", &MaxAccelerometerYHandler     }, /**<                                    */
/* MaxAccelerometerZ     */ { "MaAz", &MaxAccelerometerZHandler     }, /**<                                    */
/* RawMagnetometerX      */ { "RwMx", &RawMagnetometerXHandler      }, /**<                                    */
/* RawMagnetometerY      */ { "RwMy", &RawMagnetometerYHandler      }, /**<                                    */
/* RawMagnetometerZ      */ { "RwMz", &RawMagnetometerZHandler      }, /**<                                    */
/* MinMagnetometerX      */ { "MiMx", &MinMagnetometerXHandler      }, /**<                                    */
/* MinMagnetometerY      */ { "MiMy", &MinMagnetometerYHandler      }, /**<                                    */
/* MinMagnetometerZ      */ { "MiMz", &MinMagnetometerZHandler      }, /**<                                    */
/* MaxMagnetometerX      */ { "MaMx", &MaxMagnetometerXHandler      }, /**<                                    */
/* MaxMagnetometerY      */ { "MaMy", &MaxMagnetometerYHandler      }, /**<                                    */
/* MaxMagnetometerZ      */ { "MaMz", &MaxMagnetometerZHandler      }, /**<                                    */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
};

#define HEADERLENGTH sizeof(TelescopeData[0].Header)
#define NUMBER_OF_HANDLERS  ((uint8_t)(sizeof(TelescopeData)/sizeof(TelescopeData[0])))
TelescopeSocket::TelescopeSocket()
{
    
}

/* callback to handle which message has been sent
 */
void TelescopeSocket::SocketCallback ( char* Buffer )
{
    uint8_t Id = 0;
    for ( Id = 0; Id < NUMBER_OF_HANDLERS; Id++ )
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
    TelescopeData[Id].handler(  Buffer );
}

/* Handler for TargetRightAscension
 */
void TelescopeSocket::RightAscensionHandler( char* Buffer )
{
    printf(" RightAscensionHandler ");
    sprintf( Buffer, "RA  =%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetRightAscension( ));   
}

/* Handler for TargetDeclination
 */
void TelescopeSocket::DeclinationHandler( char* Buffer )
{
    printf(" DeclinationHandler ");
    sprintf( Buffer, "DEC =%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetDeclination( ));   
}

/* Handler for TargetRightAscension
 */
void TelescopeSocket::TargetRightAscensionHandler( char* Buffer )
{
    printf(" TargetRightAscensionHandler ");
    sprintf( Buffer, "TRA =%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetRightAscension( ));   
}

/* Handler for TargetDeclination
 */
void TelescopeSocket::TargetDeclinationHandler( char* Buffer )
{
    printf(" TargetDeclinationHandler ");
    sprintf( Buffer, "TDEC=%f#", (float)TelescopeManager::Telescope.TelescopeManager::GetDeclination( ));   
}

/* Handler for UnixTime
 */
void TelescopeSocket::UnixTimeHandler( char* Buffer )
{
    printf(" UnixTimeHandler ");
    sprintf( Buffer, "Unix=%d#", (int32_t)TelescopeManager::Telescope.TelescopeManager::GetUnixTime( ));   
}

/* Handler for GreenwichMeanTime
 */
void TelescopeSocket::GreenwichMeanTimeDayHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeDayHandler ");
    sprintf( Buffer, "GMTD=%d#", TelescopeManager::Telescope.TelescopeManager::GetDay( ));     
}

/* Handler for GreenwichMeanTimeMon
 */
void TelescopeSocket::GreenwichMeanTimeMonHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeMonHandler ");
    sprintf( Buffer, "GMTM=%d#", TelescopeManager::Telescope.TelescopeManager::GetMonth( ));     
}

/* Handler for GreenwichMeanTimeYear
 */
void TelescopeSocket::GreenwichMeanTimeYearHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeYearHandler ");
    sprintf( Buffer, "GMTY=%d#", TelescopeManager::Telescope.TelescopeManager::GetYear( ));     
}

/* Handler for GreenwichMeanTimeHour
 */
void TelescopeSocket::GreenwichMeanTimeHourHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeHourHandler ");
    sprintf( Buffer, "GMTH=%d#", TelescopeManager::Telescope.TelescopeManager::GetHour( ));     
}

/* Handler for GreenwichMeanTimeMin
 */
void TelescopeSocket::GreenwichMeanTimeMinHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeMinHandler ");
    sprintf( Buffer, "GMTm=%d#", TelescopeManager::Telescope.TelescopeManager::GetMinute( ));     
}

/* Handler for GreenwichMeanTimeSec
 */
void TelescopeSocket::GreenwichMeanTimeSecHandler( char* Buffer )
{
    printf(" GreenwichMeanTimeSecHandler ");
    sprintf( Buffer, "GMTS=%d#", TelescopeManager::Telescope.TelescopeManager::GetSecond( ));     
}

/* Handler for BritishStandardTime
 */
void TelescopeSocket::BritishStandardTimeHandler( char* Buffer )
{
    printf(" BritishStandardTimeHandler ");
    sprintf( Buffer, "BST =%d#", (uint8_t)TelescopeManager::Telescope.TelescopeManager::GetBST( )); 
}

/* Handler for Roll
 */
void TelescopeSocket::RollHandler( char* Buffer )
{
    printf(" RollHandler ");
    sprintf( Buffer, "Roll=%f#", TelescopeManager::Telescope.GetRoll( ));
}
/* Handler for Altitude/Pitch
 */
void TelescopeSocket::AltitudeHandler( char* Buffer )
{
    printf(" AltitudeHandler ");
    sprintf( Buffer, "Pitc=%f#", TelescopeManager::Telescope.GetPitchDegrees( ));    
}

/* Handler for AzimuthHandler
 */
void TelescopeSocket::AzimuthHandler( char* Buffer )
{
    printf(" AzimuthHandler ");
    sprintf( Buffer, "Azim=%f#", (float)TelescopeManager::Telescope.GetAzimuthDegrees( ));
}

/* Handler for MagneticHeading in degrees
 */
void TelescopeSocket::MagneticHeadingHandler( char* Buffer )
{
    printf(" MagneticHeadingHandler ");
    sprintf( Buffer, "MagH=%f#", TelescopeManager::Telescope.GetHeadingDegrees( ));
}

/* Handler for MagneticDeclination
 */
void TelescopeSocket::MagneticDeclinationHandler( char* Buffer )
{
    printf(" MagneticDeclinationHandler ");
    sprintf( Buffer, "MagD=%f#", TelescopeManager::Telescope.GetMagneticDeclination( ));
}

/* Handler for HieghtAboveGround
 */
void TelescopeSocket::HieghtAboveGroundHandler( char* Buffer )
{
    printf(" HieghtAboveGroundHandler ");
    sprintf( Buffer, "High=%f#", TelescopeManager::Telescope.GetHieghtAboveGround( ));
}

/* Handler for RightAscensionHours
 */
void TelescopeSocket::RightAscensionHoursHandler( char* Buffer )
{
    printf(" RightAscensionHoursHandler ");
    sprintf( Buffer, "RAH =%d#", TelescopeManager::Telescope.TelescopeManager::GetRightAscensionHours( ));     
}

/* Handler for RightAscensionMin
 */
void TelescopeSocket::RightAscensionMinHandler( char* Buffer )
{
    printf(" RightAscensionMinHandler ");
    sprintf( Buffer, "RAm =%d#", TelescopeManager::Telescope.TelescopeManager::GetRightAscensionMinutes( ));     
}

/* Handler for RightAscensionSec
 */
void TelescopeSocket::RightAscensionSecHandler( char* Buffer )
{
    printf(" RightAscensionSecHandler ");
    sprintf( Buffer, "RAS =%d#", TelescopeManager::Telescope.TelescopeManager::GetRightAscensionSeconds( ));     
}

/* Handler for DeclinationHours
 */
void TelescopeSocket::DeclinationHoursHandler( char* Buffer )
{
    printf(" DeclinationHoursHandler ");
    sprintf( Buffer, "DECH=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationHours( ));     
}

/* Handler for DeclinationMinutes
 */
void TelescopeSocket::DeclinationMinutesHandler( char* Buffer )
{
    printf(" DeclinationMinutesHandler ");
    sprintf( Buffer, "DECm=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationMinutes( ));     
}

/* Handler for DeclinationSeconds
 */
void TelescopeSocket::DeclinationSecondsHandler( char* Buffer )
{
    printf(" DeclinationSecondsHandler ");
    sprintf( Buffer, "DECS=%d#", TelescopeManager::Telescope.TelescopeManager::GetDeclinationSeconds( ));     
}

/* Handler for Juliandate
 */
void TelescopeSocket::JuliandateHandler( char* Buffer )
{
    printf(" JuliandateHandler ");
    DefaultHandler( Buffer );
}

/* Handler for Gpsmode
 */
void TelescopeSocket::GpsmodeHandler( char* Buffer )
{
    printf(" GpsmodeHandler ");
    sprintf( Buffer, "GPSM=%d#", TelescopeManager::Telescope.TelescopeManager::Getmode( ));     
}
/* Handler for Latitude
 */
void TelescopeSocket::LatitudeHandler( char* Buffer )
{
    printf(" LatitudeHandler ");
    sprintf( Buffer, "Lati=%f#", (float)TelescopeManager::Telescope.GetLatitudeDegrees( ));
}

/* Handler for LongitudeHandler
 */
void TelescopeSocket::LongitudeHandler( char* Buffer )
{
    printf(" LongitudeHandler ");
    sprintf( Buffer, "Long=%f#", (float)TelescopeManager::Telescope.GetLongitudeDegrees( ));
}

/* Handler for latitude hours  
 */
void TelescopeSocket::GpsLatitudeHoursHandler( char* Buffer )
{
//        float TelescopeManager::GetLatitudeDegrees( void );
    
    printf(" GpsLatitudeHoursHandler ");
    DefaultHandler( Buffer );
}

/* Handler for GpsLatitudeMinutes
 */
void TelescopeSocket::GpsLatitudeMinutesHandler( char* Buffer )
{
    printf(" GpsLatitudeMinutesHandler ");
    DefaultHandler( Buffer );
}

/* Handler for GpsLatitudeSeconds
 */
void TelescopeSocket::GpsLatitudeSecondsHandler( char* Buffer )
{
    printf(" GpsLatitudeSecondsHandler ");
    DefaultHandler( Buffer );
}

/* Handler for GpsLongitudeHours
 */
void TelescopeSocket::GpsLongitudeHoursHandler( char* Buffer )
{
//        float TelescopeManager::GetLongitudeDegrees( void );
    printf(" GpsLongitudeHoursHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GpsLongitudeMinutes
 */
void TelescopeSocket::GpsLongitudeMinutesHandler( char* Buffer )
{
    printf(" GpsLongitudeMinutesHandler ");
    DefaultHandler( Buffer );
}

/* Handler for GpsLongitudeMinutes
 */
void TelescopeSocket::GpsLongitudeSecondsHandler( char* Buffer )
{
    printf(" GpsLongitudeSecondsHandler ");
    DefaultHandler( Buffer );
}
/* Handler for LocalSidrealTimeHour
 */
void TelescopeSocket::LocalSidrealTimeHourHandler( char* Buffer )
{
    printf(" LocalSidrealTimeHourHandler ");
    DefaultHandler( Buffer );
}

/* Handler for LocalSidrealTimeMin
 */
void TelescopeSocket::LocalSidrealTimeMinHandler( char* Buffer )
{
    printf(" LocalSidrealTimeMinHandler ");
    DefaultHandler( Buffer );
}

/* Handler for LocalSidrealTimeSec
 */
void TelescopeSocket::LocalSidrealTimeSecHandler( char* Buffer )
{
    printf(" LocalSidrealTimeSecHandler ");
    DefaultHandler( Buffer );
}

/* Handler for RawAccelerometerX
 */
void TelescopeSocket::RawAccelerometerXHandler( char* Buffer )
{
    printf(" RawAccelerometerXHandler ");
    sprintf( Buffer, "RwAx=%f#", TelescopeOrientation::Orient.GetAx());
}

/* Handler for RawAccelerometerY
 */
void TelescopeSocket::RawAccelerometerYHandler( char* Buffer )
{
    printf(" RawAccelerometerYHandler ");
    sprintf( Buffer, "RwAy=%f#", TelescopeOrientation::Orient.GetAy());
}

/* Handler for RawAccelerometerZ
 */
void TelescopeSocket::RawAccelerometerZHandler( char* Buffer )
{
    printf(" RawAccelerometerZHandler ");
    sprintf( Buffer, "RwAz=%f#", TelescopeOrientation::Orient.GetAz());
}

/* Handler for MinAccelerometerX
 */
void TelescopeSocket::MinAccelerometerXHandler( char* Buffer )
{
    printf(" MinAccelerometerXHandler ");
    sprintf( Buffer, "MiAx=%f#", TelescopeOrientation::Orient.GetAxMin());
}
/* Handler for MinAccelerometerY
 */
void TelescopeSocket::MinAccelerometerYHandler( char* Buffer )
{
    printf(" MinAccelerometerYHandler ");
    sprintf( Buffer, "MiAy=%f#", TelescopeOrientation::Orient.GetAyMin());
}

/* Handler for MinAccelerometerZHandler
 */
void TelescopeSocket::MinAccelerometerZHandler( char* Buffer )
{
    printf(" MinAccelerometerZHandler ");
    sprintf( Buffer, "MiAz=%f#", TelescopeOrientation::Orient.GetAzMin());
}

/* Handler for MaxAccelerometerX
 */
void TelescopeSocket::MaxAccelerometerXHandler( char* Buffer )
{
    printf(" MaxAccelerometerXHandler ");
    sprintf( Buffer, "MaAx=%f#", TelescopeOrientation::Orient.GetAxMax());
}

/* Handler for MaxAccelerometerY
 */
void TelescopeSocket::MaxAccelerometerYHandler( char* Buffer )
{
    printf(" MaxAccelerometerYHandler ");
    sprintf( Buffer, "MaAy=%f#", TelescopeOrientation::Orient.GetAyMax());
}

/* Handler for MaxAccelerometerZ
 */
void TelescopeSocket::MaxAccelerometerZHandler( char* Buffer )
{
    printf(" MaxAccelerometerZHandler ");
    sprintf( Buffer, "MaAz=%f#", TelescopeOrientation::Orient.GetAzMax());
}
/* Handler for RawMagnetometerX
 */
void TelescopeSocket::RawMagnetometerXHandler( char* Buffer )
{
    printf(" RawMagnetometerXHandler ");
    sprintf( Buffer, "RwMx=%f#", TelescopeOrientation::Orient.GetMx());
}

/* Handler for RawMagnetometerY
 */
void TelescopeSocket::RawMagnetometerYHandler( char* Buffer )
{
    printf(" RawMagnetometerYHandler ");
    sprintf( Buffer, "RwMy=%f#", TelescopeOrientation::Orient.GetMy());
}

/* Handler for RawMagnetometerZ
 */
void TelescopeSocket::RawMagnetometerZHandler( char* Buffer )
{
    printf(" RawMagnetometerZHandler ");
    sprintf( Buffer, "RwMz=%f#", TelescopeOrientation::Orient.GetMz());
}

/* Handler for MinMagnetometerX
 */
void TelescopeSocket::MinMagnetometerXHandler( char* Buffer )
{
    printf(" MinMagnetometerXHandler ");
    sprintf( Buffer, "MiMx=%f#", TelescopeOrientation::Orient.GetMxMin());
}
/* Handler for MinMagnetometerY
 */
void TelescopeSocket::MinMagnetometerYHandler( char* Buffer )
{
    printf(" MinMagnetometerYHandler ");
    sprintf( Buffer, "MiMy=%f#", TelescopeOrientation::Orient.GetMyMin());
}

/* Handler for MinMagnetometerZHandler
 */
void TelescopeSocket::MinMagnetometerZHandler( char* Buffer )
{
    printf(" MinMagnetometerZHandler ");
    sprintf( Buffer, "MiMz=%f#", TelescopeOrientation::Orient.GetMzMin());
}

/* Handler for MaxMagnetometerX
 */
void TelescopeSocket::MaxMagnetometerXHandler( char* Buffer )
{
    printf(" MaxMagnetometerXHandler ");
    sprintf( Buffer, "MaMx=%f#", TelescopeOrientation::Orient.GetMxMax());
}

/* Handler for MaxMagnetometerY
 */
void TelescopeSocket::MaxMagnetometerYHandler( char* Buffer )
{
    printf(" MaxMagnetometerYHandler ");
    sprintf( Buffer, "MaMy=%f#", TelescopeOrientation::Orient.GetMyMax());
}

/* Handler for MaxMagnetometerZ
 */
void TelescopeSocket::MaxMagnetometerZHandler( char* Buffer )
{
    printf(" MaxMagnetometerZHandler ");
    sprintf( Buffer, "MaMz=%f#", TelescopeOrientation::Orient.GetMzMax());
}

/* Handler for an unknown message
 */
void TelescopeSocket::DefaultHandler( char* Buffer )
{
    printf(" default handler ");
    sprintf( Buffer, "Not Supported#" );
}


