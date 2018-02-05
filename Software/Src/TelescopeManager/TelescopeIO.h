/**
A module to code and decode Messages and inputs tot he system.

Author and copyright of this file
Chris Dick, 2016

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
#ifndef TELESCOPEIO_H
#define TELESCOPEIO_H

#include <stdint.h>
//#include "TelescopeData.h"

typedef enum
{
    EMPTY,      //Empty slot for Queue
    UNIXTIME,   //(double)UnixTime
    GMTDAY,     //gmt->tm_mday 
    GMTMON,     //gmt->tm_mon
    GMTYEAR,    //(gmt->tm_year + 1900)
    GMTHOUR,    //gmt->tm_hour
    GMTMIN,     //gmt->tm_min
    GMTSEC,     //gmt->tm_sec
    BST,         //gmt->tm_isdst
    ALTITUDE,   //(180*(Pitch/M_PI))
    AZIMUTH,    //(180*(Angles.Azimuth/M_PI))
    LATITUDE,   //(180*(Angles.Latitude/M_PI))
    LONGITUDE,  //(180*(Angles.LongitudeWest/M_PI))
    LSTHOUR,    //Angles.LocalSiderealCCTime.Hours
    LSTMIN,     //Angles.LocalSiderealCCTime.Minutes
    LSTSEC,     //Angles.LocalSiderealCCTime.Seconds
    MAGDEC,     //MagneticDeclination 
    MAGHEAD,    //(180*(Heading/M_PI))
    TRUEHEAD,   //(180*(Angles.Azimuth/M_PI)));    
    HEIGHT,     //HieghtAboveGround
    RAHOURS,    //Temp.Hours
    RAMIN,      //Temp.Minutes
    RASEC,      //Temp.Seconds
    DECHOURS,   //Temp.Hours
    DECMIN,     //Temp.Minutes
    DECSEC,     //Temp.Seconds
    JULIANDATE, // Julian Data
    LOCSOURCE,  // the source of gps data, default 
    GPSMODE,    //GPS fix mode
    //GPSLATD,
    //GPSLATM,
    //GPSLATS,
    //GPSLONH,
    //GPSLONM,
    //GPSLONS,
    //GPSHIEGHT,
    WEBLATD,
    WEBLATM,
    WEBLATS,
    WEBLONH,
    WEBLONM,
    WEBLONS,
    WEBHIEGHT,
    DEBUG,
    NUMBEROFDATA      // this line must always be last in the enum
} DATAID_T;

typedef enum
{
    DEFAULT,
    WEBSITE,
	NO_OF_SOURCES
} SOURCE_T;

/** TelescopeIO
 * - Class to provide use to the magnetometer
 */
class TelescopeIO
{
    public:
    /** Constructor
     */
        TelescopeIO( void );
    /**  House keeping function
     */
    /** Initialise the Orientation
     * @return bool Initialisation status  
     */
        bool Init( void );
    /** Get any messages recieved from the websocket pass to handler 
     */
        void WebRecieve( void );
    /** Get the heading of the Telescope
     * @param Id The id of the data to update
     * @param Data The new data to update
     */
        bool UpdateData(  DATAID_T Id, void* Data );
    /** Removes the data string and stores it.
     * @param Message tHE MESSAGE TO BE HANDELED.
     */
        void HandleMessage( char* Message );
    /** Gets the value from the string stored in the structure
     * @param Id The id of the data to get.
     * @param Data pointer to get the data to.
     */
        void GetValue( DATAID_T Id, void* Data );
        
        static TelescopeIO TeleIO;
        
};

#endif /* TELESCOPEIO_H */
