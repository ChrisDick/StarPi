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

#include "TelescopeSocket.h"
TelescopeSocket TelescopeSocket::TeleSocket;



TELEDATA_T TelescopeSocket::TelescopeData[40] =
{
/* Id                     Header  Handler                                                                      */
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
/* GpsLatitudeMinutes    */ { "GLAM", &GpsLatitudeMinutesHandler    }, /**< Gps source latitude minutes        */
/* GpsLatitudeSeconds    */ { "GLAS", &GpsLatitudeSecondsHandler    }, /**< Gps source latitude seconds        */
/* GpsLongitudeHours     */ { "GLOH", &GpsLongitudeHoursHandler     }, /**< Gps source longitude hours         */
/* GpsLongitudeMinutes   */ { "GLOM", &GpsLongitudeMinutesHandler   }, /**< Gps source longitude minutes       */
/* GpsLongitudeSeconds   */ { "GLOS", &GpsLongitudeSecondsHandler   }, /**< Gps source longitude seconds       */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
/* Default               */ { "DFLT", &DefaultHandler               }, /**< any debug string                   */
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
void TelescopeSocket::TargetRightAscensionHandler( char* Buffer )
{
//    TelescopeManager::GetRaDec ( double* Ra, double* Dec );
    printf(" TargetRightAscensionHandler ");
    DefaultHandler( Buffer );
}

/* Handler for TargetDeclination
 */
void TelescopeSocket::TargetDeclinationHandler( char* Buffer )
{
  //  TelescopeManager::GetRaDec ( double* Ra, double* Dec );
    printf(" TargetDeclinationHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for UnixTime
 */
void TelescopeSocket::UnixTimeHandler( char* Buffer )
{
    //    time_t TelescopeManager::GetUnixTime( void );
    printf(" UnixTimeHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTime
 */
void TelescopeSocket::GreenwichMeanTimeDayHandler( char* Buffer )
{
      //  uint16_t TelescopeManager::GetDay( void );
    printf(" GreenwichMeanTimeDayHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTimeMon
 */
void TelescopeSocket::GreenwichMeanTimeMonHandler( char* Buffer )
{
      //  uint16_t TelescopeManager::GetMonth( void )
    printf(" GreenwichMeanTimeMonHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTimeYear
 */
void TelescopeSocket::GreenwichMeanTimeYearHandler( char* Buffer )
{
      //  uint16_t TelescopeManager::GetYear( void );
    printf(" GreenwichMeanTimeYearHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTimeHour
 */
void TelescopeSocket::GreenwichMeanTimeHourHandler( char* Buffer )
{
     //   uint16_t TelescopeManager::GetHour( void );
    printf(" GreenwichMeanTimeHourHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTimeMin
 */
void TelescopeSocket::GreenwichMeanTimeMinHandler( char* Buffer )
{
     //   uint16_t TelescopeManager::GetMinute( void );
    printf(" GreenwichMeanTimeMinHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for GreenwichMeanTimeSec
 */
void TelescopeSocket::GreenwichMeanTimeSecHandler( char* Buffer )
{
     //   uint16_t TelescopeManager::GetSecond( void );
    printf(" GreenwichMeanTimeSecHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for BritishStandardTime
 */
void TelescopeSocket::BritishStandardTimeHandler( char* Buffer )
{
     //   bool TelescopeManager::GetBST( void );
    printf(" BritishStandardTimeHandler ");
    DefaultHandler( Buffer );
    
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
   //     float TelescopeManager::GetPitchDegrees( void );
    printf(" AltitudeHandler ");
    sprintf( Buffer, "Pitc=%f#", TelescopeManager::Telescope.GetPitchDegrees( ));
    
}

/* Handler for AzimuthHandler
 */
void TelescopeSocket::AzimuthHandler( char* Buffer )
{
   //     float TelescopeManager::GetAzimuthDegrees( void );
    printf(" AzimuthHandler ");
    sprintf( Buffer, "MagH=%f#", TelescopeManager::Telescope.GetHeadingDegrees( ));
    
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

/* Handler for RightAscensionHours
 */
void TelescopeSocket::RightAscensionHoursHandler( char* Buffer )
{
        //int8_t TelescopeManager::GetRightAscensionHours( void );
    printf(" RightAscensionHoursHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for RightAscensionMin
 */
void TelescopeSocket::RightAscensionMinHandler( char* Buffer )
{
    printf(" RightAscensionMinHandler ");
     //   int8_t TelescopeManager::GetRightAscensionMinutes( void );
    DefaultHandler( Buffer );
    
}

/* Handler for RightAscensionSec
 */
void TelescopeSocket::RightAscensionSecHandler( char* Buffer )
{
    //    int8_t TelescopeManager::GetRightAscensionSeconds( void );
    printf(" RightAscensionSecHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for DeclinationHours
 */
void TelescopeSocket::DeclinationHoursHandler( char* Buffer )
{
      //  int8_t TelescopeManager::GetDeclinationHours( void );
    printf(" DeclinationHoursHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for DeclinationMinutes
 */
void TelescopeSocket::DeclinationMinutesHandler( char* Buffer )
{
    //    int8_t TelescopeManager::GetDeclinationMinutes( void );
    printf(" DeclinationMinutesHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for DeclinationSeconds
 */
void TelescopeSocket::DeclinationSecondsHandler( char* Buffer )
{
  //      int8_t TelescopeManager::GetDeclinationSeconds( void );
    printf(" DeclinationSecondsHandler ");
    DefaultHandler( Buffer );
    
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
//        int8_t TelescopeManager::Getmode( void );
    printf(" GpsmodeHandler ");
    DefaultHandler( Buffer );
    
}
/* Handler for Latitude
 */
void TelescopeSocket::LatitudeHandler( char* Buffer )
{
//        double TelescopeManager::GetLatitude( void );
    printf(" LatitudeHandler ");
    DefaultHandler( Buffer );
    
}

/* Handler for LongitudeHandler
 */
void TelescopeSocket::LongitudeHandler( char* Buffer )
{
//        double TelescopeManager::GetLongitude( void );
    printf(" LongitudeHandler ");
    DefaultHandler( Buffer );
    
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

/* Handler for an unknown message
 */
void TelescopeSocket::DefaultHandler( char* Buffer )
{
    printf(" default handler ");
    sprintf( Buffer, "Not Supported#" );
}
        

