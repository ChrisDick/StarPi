/*
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
#include <stdio.h>
#include <string.h>
#include "HalWebsocketd.h"
#include "Config.h"

#include "TelescopeIO.h"
TelescopeIO TelescopeIO::TeleIO;



typedef enum
{
    BOOL,
    UINT8_2,
    INT8_2,
    UINT16_4,
    INT16_4,
    FLOAT,
    DOUBLE,
    STRING,
} FORMAT_T;

typedef struct 
{
    const char* Header;
    bool Web;
    FORMAT_T DataFormat;
    char Data[DATALENGTH];
} TELEDATA_T;

static TELEDATA_T TelescopeData[NUMBEROFDATA] = 
{
/* Id              Header  Web  DataType Data                                            */
/* EMPTY,     */ { "EMPT", false, STRING,   {0} }, /**< Unused slot - reserved (always 0)  */
/* UNIXTIME,  */ { "Unix", false, FLOAT,    {0} }, /**< UnixTime                           */
/* GMTDAY,    */ { "GMTD", true,  UINT8_2,  {0} }, /**< gmt->tm_mday                       */
/* GMTMON,    */ { "GMTM", true,  UINT8_2,  {0} }, /**< gmt->tm_mon                        */
/* GMTYEAR,   */ { "GMTY", true,  UINT16_4, {0} }, /**< (gmt->tm_year + 1900)              */
/* GMTHOUR,   */ { "GMTH", true,  UINT8_2,  {0} }, /**< gmt->tm_hour                       */
/* GMTMIN,    */ { "GMTm", true,  UINT8_2,  {0} }, /**< gmt->tm_min                        */
/* GMTSEC,    */ { "GMTS", true,  UINT8_2,  {0} }, /**< gmt->tm_sec                        */
/* BST        */ { "BST ", true,  UINT8_2,  {0} }, /**< gmt->tm_isdst                      */
/* ALTITUDE,  */ { "Pitc", true,  FLOAT,    {0} }, /**< (180*(Pitch/M_PI))                 */
/* AZIMUTH,   */ { "Azim", true,  FLOAT,    {0} }, /**< (180*(Angles.Azimuth/M_PI))        */
/* LATITUDE,  */ { "Lati", false, FLOAT,    {0} }, /**< (180*(Angles.Latitude/M_PI))       */
/* LONGITUDE, */ { "Long", false, FLOAT,    {0} }, /**< (180*(Angles.LongitudeWest/M_PI))  */
/* LSTHOUR,   */ { "LSTH", true,  INT8_2,  {0} }, /**< Local Sidereal Time Hours          */
/* LSTMIN,    */ { "LSTm", true,  INT8_2,  {0} }, /**< Local Sidereal Time Minutes        */
/* LSTSEC,    */ { "LSTS", true,  FLOAT,    {0} }, /**< Local Sidereal Time Seconds        */
/* MAGDEC,    */ { "MagD", true,  FLOAT,    {0} }, /**< Magnetic Declination               */
/* MAGHEAD,   */ { "MagH", true,  FLOAT,    {0} }, /**< (180*(Heading/M_PI))               */
/* TRUEHEAD,  */ { "TruH", true,  FLOAT,    {0} }, /**< (180*(Angles.Azimuth/M_PI)));      */
/* HEIGHT,    */ { "High", true,  FLOAT,    {0} }, /**< Hieght Above Ground (km)           */
/* RAHOURS,   */ { "RAH ", true,  INT8_2,  {0} }, /**< Right Ascension Degrees            */
/* RAMIN,     */ { "RAm ", true,  INT8_2,  {0} }, /**< Right Ascension Minutes            */
/* RASEC,     */ { "RAS ", true,  FLOAT,    {0} }, /**< Right Ascension Seconds            */
/* DECHOURS,  */ { "DECH", true,  INT8_2,  {0} }, /**< Declination Hours                  */
/* DECMIN,    */ { "DECm", true,  INT8_2,  {0} }, /**< Declination Minutes                */
/* DECSEC,    */ { "DECS", true,  FLOAT,    {0} }, /**< Declination Seconds                */
/* JULIANDATE,*/ { "JDAT", true,  FLOAT,    {0} }, /**< JulianDate                         */
/* GPSSOURCE, */ { "GPSS", true,  UINT8_2,  {0} }, /**< Source of GPS                      */
/* GPSLATD,   */// { "GLAD", true,  INT8_2,  {0} }, /**< Gps source latitude degrees        */
/* GPSLATM,   */// { "GLAM", true,  INT8_2,  {0} }, /**< Gps source latitude minutes        */
/* GPSLATS,   */// { "GLAS", true,  FLOAT,    {0} }, /**< Gps source latitude seconds        */
/* GPSLONH,   */// { "GLOH", true,  INT8_2,  {0} }, /**< Gps source longitude hours         */
/* GPSLONM,   */// { "GLOM", true,  INT8_2,  {0} }, /**< Gps source longitude minutes       */
/* GPSLONS,   */// { "GLOS", true,  FLOAT,    {0} }, /**< Gps source longitude seconds       */
/* GPSHIEGHT, */// { "GHIH", true,  FLOAT,    {0} }, /**< Gps source longitude seconds       */
/* WEBLATD,   */// { "WLAD", false, INT8_2,  {0} }, /**< Web source latitude degrees        */
/* WEBLATM,   */// { "WLAM", false, INT8_2,  {0} }, /**< Web source latitude minutes        */
/* WEBLATS,   */// { "WLAS", false, FLOAT,    {0} }, /**< Web source latitude seconds        */
/* WEBLONH,   */// { "WLOH", false, INT8_2,  {0} }, /**< Web source longitude hours         */
/* WEBLONM,   */// { "WLOM", false, INT8_2,  {0} }, /**< Web source longitude minutes       */
/* WEBLONS,   */// { "WLOS", false, FLOAT,    {0} }, /**< Web source longitude seconds       */
/* WEBHIEGHT, */// { "WHIH", false, FLOAT,    {0} }, /**< Web source longitude seconds       */
/* DEBUG,     */ { "DEBU", true,  STRING,   {0} }, /**< any debug string                   */ 
};

#define HEADERLENGTH sizeof(TelescopeData[0].Header)


/* TelescopeIO
 *  Constructor
 */
TelescopeIO::TelescopeIO( void )
{
}

/* initialise the sensors used
 */
bool TelescopeIO::TelescopeIOInit( void )
{
    /*
         Initialise all the data to zero. (defaults?)
    */
    uint8_t Id = 0;
    int32_t Data = 0;
    while (Id < NUMBEROFDATA)
    {
        switch ( TelescopeData[Id].DataFormat )
        {
            case UINT8_2:
            {
                sprintf ( TelescopeData[Id].Data, " %2d", Data );
                break;
            }
            case UINT16_4:
            {
                sprintf ( TelescopeData[Id].Data, " %4d", Data );
                break;
            }
            case INT8_2:
            {
                sprintf ( TelescopeData[Id].Data, " %2d", Data );
                break;
            }
            case INT16_4:
            {
                sprintf ( TelescopeData[Id].Data, " %4d", Data );
                break;
            }
            case FLOAT:
            case DOUBLE:
            {
                sprintf ( TelescopeData[Id].Data, " %4f", ((float)Data) );
                break;
            }
            case STRING:
            {
                sprintf ( TelescopeData[Id].Data, " %s", ((char*)Data) );
                break;
            }        
            default:
            {
                // ignore it.
            }
       }
       Id++;
    }
    
    return true;
}



/* Get any messages recieved from the websocket pass to handler 
 */
void TelescopeIO::TelescopeIOWebRecieve( )
{
    char RecievedMessage[DATALENGTH+HEADERLENGTH];
    while (true)
    {    
        bool MessageAvailiable = false;
        MessageAvailiable = HalWebsocketd::Websocket.HalWebsocketdGetMessage( RecievedMessage );
        if (!MessageAvailiable)
        {
            break;
        }
        TelescopeIOHandleMessage( RecievedMessage );
    }
}

/* Update and send the data
 * @param Id The id of the data to update
 * @param Data The new data to update
 */
bool TelescopeIO::TelescopeIOUpdateData( DATAID_T Id, void* Data )
{
    char Message[DATALENGTH+HEADERLENGTH] = { 0u }; 
    bool SendMessage = false;
    /*
        Format the data into a string
    */
    switch ( TelescopeData[Id].DataFormat )
    {
        case UINT8_2:
        {
            sprintf ( TelescopeData[Id].Data, " %2d", (int32_t)*((uint8_t*)Data) );
            SendMessage = true;
            break;
        }
        case UINT16_4:
        {
            sprintf ( TelescopeData[Id].Data, " %4d", (uint32_t)*((uint16_t*)Data) );
            SendMessage = true;
            break;
        }
        case INT8_2:
        {
            sprintf ( TelescopeData[Id].Data, " %2d", (uint32_t)*((int8_t*)Data) );
            SendMessage = true;
            break;
        }
        case INT16_4:
        {
            sprintf ( TelescopeData[Id].Data, " %4d", (uint32_t)*((int16_t*)Data) );
            SendMessage = true;
            break;
        }
        case FLOAT:
        case DOUBLE:
        {
            sprintf ( TelescopeData[Id].Data, " %4f", *((float*)Data) );
            SendMessage = true;
            break;
        }
        case STRING:
        {
            sprintf ( TelescopeData[Id].Data, " %s", ((char*)Data) );
            SendMessage = true;
            break;
        }        
        default:
        {
            // ignore it.
        }
   }
   
   if ( SendMessage )
   {
        /*
            Add the header and send the message
        */
        strcat(Message, TelescopeData[Id].Header);
        strcat(Message, TelescopeData[Id].Data); 
        HalWebsocketd::Websocket.HalWebsocketdSendMessage(Message, Id );
   }
   return SendMessage;
}


/* Removes the data string and stores it.
 */
void TelescopeIO::TelescopeIOHandleMessage( char* Message )
{
   for (uint8_t Id = 0; Id < NUMBEROFDATA; Id++)
   {
        if ( strncmp (Message, TelescopeData[Id].Header, 4 ) == 0)
        {
            char OutMessage[DATALENGTH+HEADERLENGTH] = {0};
            strcpy(  TelescopeData[Id].Data, &Message[HEADERLENGTH] );
            //strcat(OutMessage, TelescopeData[Id].Header);
            //strcat(OutMessage, TelescopeData[Id].Data);
            //HalWebsocketd::Websocket.HalWebsocketdSendMessage(OutMessage, Id );
        }
    }   
}

/* Gets the value from the string stored in the structure
 */
void TelescopeIO::TelescopeIOGetValue( DATAID_T Id, void* Data )
{
    switch ( TelescopeData[Id].DataFormat )
    {
        case UINT8_2:
        {
            sscanf ( TelescopeData[Id].Data, "%2d", (int32_t*)((uint8_t*)Data) );
            break;
        }
        case UINT16_4:
        {
            sscanf ( TelescopeData[Id].Data, "%4d", (int32_t*)((uint16_t*)Data) );
            break;
        }
        case INT8_2:
        {
            sscanf ( TelescopeData[Id].Data, "%2d", (int32_t*)((uint8_t*)Data) );
            break;
        }
        case INT16_4:
        {
            sscanf ( TelescopeData[Id].Data, "%4d", (int32_t*)((uint16_t*)Data) );
            break;
        }
        case FLOAT:
        case DOUBLE:
        {
            sscanf ( TelescopeData[Id].Data, "%4f", ((float*)Data) );
            break;
        }
        default:
        {
            // ignore it.
        }
    }
}