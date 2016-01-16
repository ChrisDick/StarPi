/*
A module to store any data that's being passed around the telescope software
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
#ifndef TELESCOPEDATA_H
#define TELESCOPEDATA_H

#include "HalWebsocketd.h"
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
    DEBUG,
    NUMBEROFDATA      // this line must always be last in the enum
} DATAID_T;

typedef enum
{
    BOOL,
    UINT8_2,
    UINT16_4,
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
/* Id              Header  Web  DataType Data                                          */
/* EMPTY,     */ { "EMPT", false, STRING, {0} }, /* (double)UnixTime                   */
/* UNIXTIME,  */ { "Unix", true, FLOAT,    {0} }, /* (double)UnixTime                   */
/* GMTDAY,    */ { "GMTD", true, UINT8_2,  {0} }, /* gmt->tm_mday                       */
/* GMTMON,    */ { "GMTM", true, UINT8_2,  {0} }, /* gmt->tm_mon                        */
/* GMTYEAR,   */ { "GMTY", true, UINT16_4, {0} }, /* (gmt->tm_year + 1900)              */
/* GMTHOUR,   */ { "GMTH", true, UINT8_2,  {0} }, /* gmt->tm_hour                       */
/* GMTMIN,    */ { "GMTm", true, UINT8_2,  {0} }, /* gmt->tm_min                        */
/* GMTSEC,    */ { "GMTS", true, UINT8_2,  {0} }, /* gmt->tm_sec                        */
/* BST        */ { "BST ", true, UINT8_2,  {0} }, /* gmt->tm_isdst                      */
/* ALTITUDE,  */ { "Pitc", true, FLOAT,    {0} }, /* (180*(Pitch/M_PI))                 */
/* AZIMUTH,   */ { "Azim", true, FLOAT,    {0} }, /* (180*(Angles.Azimuth/M_PI))        */
/* LATITUDE,  */ { "Lati", true, FLOAT,    {0} }, /* (180*(Angles.Latitude/M_PI))       */
/* LONGITUDE, */ { "Long", true, FLOAT,    {0} }, /* (180*(Angles.LongitudeWest/M_PI))  */
/* LSTHOUR,   */ { "LSTH", true, UINT8_2,  {0} }, /* Angles.LocalSiderealCCTime.Hours   */
/* LSTMIN,    */ { "LSTm", true, UINT8_2,  {0} }, /* Angles.LocalSiderealCCTime.Minutes */
/* LSTSEC,    */ { "LSTS", true, FLOAT,    {0} }, /* Angles.LocalSiderealCCTime.Seconds */
/* MAGDEC,    */ { "MagD", true, FLOAT,    {0} }, /* MagneticDeclination                */
/* MAGHEAD,   */ { "MagH", true, FLOAT,    {0} }, /* (180*(Heading/M_PI))               */
/* TRUEHEAD,  */ { "TruH", true, FLOAT,    {0} }, /* (180*(Angles.Azimuth/M_PI)));      */
/* HEIGHT,    */ { "High", true, FLOAT,    {0} }, /* HieghtAboveGround                  */
/* RAHOURS,   */ { "RAH ", true, UINT8_2,  {0} }, /* Temp.Hours                         */
/* RAMIN,     */ { "RAm ", true, UINT8_2,  {0} }, /* Temp.Minutes                       */
/* RASEC,     */ { "RAS ", true, FLOAT,    {0} }, /* Temp.Seconds                       */
/* DECHOURS,  */ { "DECH", true, UINT8_2,  {0} }, /* Temp.Hours                         */
/* DECMIN,    */ { "DECm", true, UINT8_2,  {0} }, /* Temp.Minutes                       */
/* DECSEC,    */ { "DECS", true, FLOAT,    {0} }, /* Temp.Seconds                       */
/* JULIANDATE,*/ { "JDAT", true, FLOAT,    {0} }, /* JulianDate                         */
/* DEBUG,     */ { "DEBU", true, STRING,   {0} }, /* any debug string                   */ 
};

#define HEADERLENGTH sizeof(TelescopeData[0].Header)

#endif /* TELESCOPEDATA_H */