/*
A module to handle the actual orientation of an alt-azimuth mount telescope

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
#include <math.h>
#include "TelescopeOrientation.h"
#include "HalMagnetometer.h"
#include "HalAccelerometer.h"
#include "HalFilesystem.h"
#include "Config.h"

//#if ( defined CALIBRATE_MAG_DEBUG) || ( defined CALIBRATE_ACC_DEBUG )
#include <stdio.h>
//#endif
#include <unistd.h>
#include <limits.h>
#include <string.h>
#ifdef TIMING
#include "GPIO.h"
#endif

using namespace std;

TelescopeOrientation TelescopeOrientation::Orient;

/* TelescopeOrientation
 *  Constructor
 */
TelescopeOrientation::TelescopeOrientation( void )
{
}

/* initialise the sensors used
 */
bool TelescopeOrientation::Init( void )
{
    HalAccelerometer::Accelerometer.Init();
    HalMagnetometer::Magneto.Init();
    Calibrating = false;
    
    LoadConfig();
    
    /* raw magneto values */
    Mx = 0.0f;
    My = 0.0f;
    Mz = 0.0f;
    /* Raw Accel values */
    Ax = 0.0f;
    Ay = 0.0f;
    Az = 0.0f;

#ifdef TIMING
    GPIO::gpio.SetupOutput( TELESCOPE_ORIENTATION_PIN );
    GPIO::gpio.SetPullMode( TELESCOPE_ORIENTATION_PIN , PULL_UP );
#endif

    return true;
}

/* Handle loading the calibration or load the defaults.
 */
void TelescopeOrientation::LoadConfig( void )
{
    HalFilesystem File;
    char Path[PATH_MAX];
    char Configuration[4096u];
    /* Get Path of StarPi executable */
    getcwd( Path, PATH_MAX );
    strcat( Path, "/Orientation.Conf");
    if ( File.GetConfig( Path, Configuration ) )
    {
        sscanf( Configuration, "MxMax=%f, MxMin=%f, MyMax=%f, MyMin=%f, MyMax=%f, MzMax=%f, MzMin=%f, AxMax=%f, AxMin=%f, AyMax=%f, AyMin=%f, AyMax=%f, AzMax=%f, AzMin=%f, MagneticOffset=%f, AccelOffset=%f", &MxMax, &MxMin, &MyMax, &MyMin, &MyMax, &MzMax, &MzMin, &AxMax, &AxMin, &AyMax, &AyMin, &AyMax, &AzMax, &AzMin, &MagneticOffset, &AccelOffset );
        printf( "Calibrations loaded\n\r" );
    }
    else
    {
        MxMax = CONFIG_MXMAX;
        MxMin = CONFIG_MXMIN;
        MyMax = CONFIG_MYMAX;
        MyMin = CONFIG_MYMIN;
        MzMax = CONFIG_MZMAX;
        MzMin = CONFIG_MZMIN;
        AxMax = CONFIG_AXMAX;
        AxMin = CONFIG_AXMIN;
        AyMax = CONFIG_AYMAX;
        AyMin = CONFIG_AYMIN;
        AzMax = CONFIG_AZMAX;
        AzMin = CONFIG_AZMIN;
        MagneticOffset = 0.0f;
        AccelOffset = 0.0f;
        printf( "Using default calibrations\n\r" );
    }
}


/* Handle saving of the calibration.
 */
void TelescopeOrientation::SaveConfig( void )
{
    HalFilesystem File;
    char Path[PATH_MAX];
    char Configuration[4096u];
    /* Get Path of StarPi executable */
    getcwd( Path, PATH_MAX );
    strcat( Path, "/Orientation.Conf");
    sprintf( Configuration, "MxMax=%f, MxMin=%f, MyMax=%f, MyMin=%f, MyMax=%f, MzMax=%f, MzMin=%f, AxMax=%f, AxMin=%f, AyMax=%f, AyMin=%f, AyMax=%f, AzMax=%f, AzMin=%f, MagneticOffset=%f, AccelOffset=%f", MxMax, MxMin, MyMax, MyMin, MyMax, MzMax, MzMin, AxMax, AxMin, AyMax, AyMin, AyMax, AzMax, AzMin, MagneticOffset, AccelOffset );
    File.SetConfig( Path, Configuration );
}

/* TelescopeOrientationRun
 *  Runs the Accelerometer and the Magnetometer.
 */
void TelescopeOrientation::Run( void )
{
    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_ORIENTATION_PIN , true );
    #endif

    HalMagnetometer::Magneto.Run();
    HalAccelerometer::Accelerometer.Run();

    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_ORIENTATION_PIN , false );
    #endif
}

/*
 *
 */
void TelescopeOrientation::GetOrientation( float* Pitch, float* Roll, float* Heading )
{
    /* magneto values with offset */
    float Mxo = 0.0f;
    float Myo = 0.0f;
    float Mzo = 0.0f;
    /* Accel values with offset */
    float Axo = 0.0f;
    float Ayo = 0.0f;
    float Azo = 0.0f;
    /* Calculation variables */
    float CosRoll = 0.0f;
    float SinRoll = 0.0f;
    float CosPitch = 0.0f;
    float SinPitch = 0.0f;
    float XComponent = 0.0f;
    float YComponent = 0.0f;
    
    /* get filtered sensor data */
    HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
    HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
    /* Calibrate */
    if ( Calibrating )
    {
        Calibration();
    }
    /* remove Hard Iron effects */
    Mxo = Mx - ((MxMin + MxMax)/2.0);
    Myo = My - ((MyMin + MyMax)/2.0);
    Mzo = Mz - ((MzMin + MzMax)/2.0);
    
    /* Normalise */
    Mxo = Mxo / (((MxMin + MxMax)/2.0));
    Myo = Myo / (((MyMin + MyMax)/2.0));
    Mzo = Mzo / (MzMax - ((MzMin + MzMax)/2.0));
#ifdef CALC_DEBUG
    printf ("Mxo: %f Myo: %f Mzo: %f ", Mxo, Myo, Mzo ); // debug
#endif
    
    Axo = Ax / (AxMax - ((AxMin + AxMax)/2.0));
    Ayo = Ay / (AyMax - ((AyMin + AyMax)/2.0));
    Azo = Az / (AzMax - ((AzMin + AzMax)/2.0));
#ifdef CALC_DEBUG
    printf ("Axo: %f Ayo: %f Azo: %f ", Axo, Ayo, Azo ); // debug
#endif

    /* Calculate Pitch */
    *Pitch = asin ( Axo / ( sqrt( (Axo*Axo) + (Ayo*Ayo) + ( Azo*Azo) ) ) );
    CosPitch = cos(*Pitch);
    SinPitch = sin(*Pitch);
    
    /* Calculate Roll */
    *Roll = atan2( Ayo, Azo );
    CosRoll = cos(*Roll);
    SinRoll = sin(*Roll);
    
    /* Calculate X and Y components then heading */
    XComponent = ( Mxo * CosPitch ) + ( Myo * SinRoll * SinPitch ) + ( Mzo * CosRoll * SinPitch );
    YComponent = ( Mzo * SinRoll ) - ( Myo * CosRoll );
    *Heading = atan2( YComponent , XComponent ); 

    *Pitch += (M_PI * ( AccelOffset / 180.0f ));
    *Heading += (M_PI * ( MagneticOffset / 180.0f )); 

    
    if (*Heading > (2*M_PI)) 
    {
        *Heading -= (2*M_PI);
    }
    if (*Heading < 0) 
    {
        *Heading += (2*M_PI);
    }
    
#ifdef CALC_DEBUG
    printf ("heading %f ", *Heading);
    printf ("roll %f ", *Roll);
    printf ("pitch %f\n\r", *Pitch);
#endif
}

/* EnableCalibration
 * @Param Enable or disable calibration 
 */
void TelescopeOrientation::EnableCalibration ( bool Enable )
{
    Calibrating = Enable;
    /* If we are stopping calibration save the values */
    if ( !Enable )
    {
        SaveConfig();
    }
}    

/* Calibration
 */
void TelescopeOrientation::Calibration( void )
{
    /*
       keep track of the Magnetometer calibration values
    */
    if (Mx > MxMax)
    {
        MxMax = Mx;
    }
    if (Mx < MxMin)
    {
        MxMin = Mx;
    }
    if (My > MyMax)
    {
        MyMax = My;
    }
    if (My < MyMin)
    {
        MyMin = My;
    }
    if (Mz > MzMax)
    {
        MzMax = Mz;
    }
    if (Mz < MzMin)
    {
        MzMin = Mz;
    }
#ifdef CALIBRATE_MAG_DEBUG
    printf ("Mx: %f My:%f Mz:%f MxMax: %f MxMin: %f  MyMax: %f  MyMin: %f  MzMax: %f  MzMin: %f  \n\r", Mx, My, Mz, MxMax, MxMin, MyMax, MyMin, MzMax, MzMin ); // debug
#endif
    /*
       keep track of the Accelerometer calibration values
    */
    if (Ax > AxMax)
    {
        AxMax = Ax;
    }
    if (Ax < AxMin)
    {
        AxMin = Ax;
    }
    if (Ay > AyMax)
    {
        AyMax = Ay;
    }
    if (Ay < AyMin)
    {
        AyMin = Ay;
    }
    if (Az > AzMax)
    {
        AzMax = Az;
    }
    if (Az < AzMin)
    {
        AzMin = Az;
    }
#ifdef CALIBRATE_ACC_DEBUG
    printf ("Ax: %f Ay:%f Az:%f AxMax: %f AxMin: %f  AyMax: %f  AyMin: %f  AzMax: %f  AzMin: %f  \n\r", Ax, Ay, Az, AxMax, AxMin, AyMax, AyMin, AzMax, AzMin ); // debug
#endif
}


/* Ax getter
 */
float TelescopeOrientation::GetAx( void )
{
    return Ax;
}
/* Ay getter
 */
float TelescopeOrientation::GetAy( void )
{
    return Ay;
}
/* Az getter
 */
float TelescopeOrientation::GetAz( void )
{
    return Az;
}
/* AxMin getter
 */
float TelescopeOrientation::GetAxMin( void )
{
    return AxMin;
}
/* AyMin getter
 */
float TelescopeOrientation::GetAyMin( void )
{
    return AyMin;
}
/* AzMin getter
 */
float TelescopeOrientation::GetAzMin( void )
{
    return AzMin;
}
/* AxMax getter
 */
float TelescopeOrientation::GetAxMax( void )
{
    return AxMax;
}
/* AyMax getter
 */
float TelescopeOrientation::GetAyMax( void )
{
    return AyMax;
}
/* AzMax getter
 */
float TelescopeOrientation::GetAzMax( void )
{
    return AzMax;
}

/* AxMin Reset
 */
void TelescopeOrientation::ResetAxMin( void )
{
    AxMin = 0.0f;
}
/* AyMin Reset
 */
void TelescopeOrientation::ResetAyMin( void )
{
    AyMin = 0.0f;
}
/* AzMin Reset
 */
void TelescopeOrientation::ResetAzMin( void )
{
    AzMin = 0.0f;
}
/* AxMax Reset
 */
void TelescopeOrientation::ResetAxMax( void )
{
    AxMax = 0.0f;
}
/* AyMax Reset
 */
void TelescopeOrientation::ResetAyMax( void )
{
    AyMax = 0.0f;
}
/* AzMax Reset
 */
void TelescopeOrientation::ResetAzMax( void )
{
    AzMax = 0.0f;
}

/* Mx getter
 */
float TelescopeOrientation::GetMx( void )
{
    return Mx;
}
/* My getter
 */
float TelescopeOrientation::GetMy( void )
{
    return My;
}
/* Mz getter
 */
float TelescopeOrientation::GetMz( void )
{
    return Mz;
}
/* MxMin getter
 */
float TelescopeOrientation::GetMxMin( void )
{
    return MxMin;
}
/* MyMin getter
 */
float TelescopeOrientation::GetMyMin( void )
{
    return MyMin;
}
/* MzMin getter
 */
float TelescopeOrientation::GetMzMin( void )
{
    return MzMin;
}
/* MxMax getter
 */
float TelescopeOrientation::GetMxMax( void )
{
    return MxMax;
}
/* MyMax getter
 */
float TelescopeOrientation::GetMyMax( void )
{
    return MyMax;
}
/* MzMax getter
 */
float TelescopeOrientation::GetMzMax( void )
{
    return MzMax;
}

/* MxMin Reset
 */
void TelescopeOrientation::ResetMxMin( void )
{
    MxMin = 0.0f;
}
/* MyMin Reset
 */
void TelescopeOrientation::ResetMyMin( void )
{
    MyMin = 0.0f;
}
/* MzMin Reset
 */
void TelescopeOrientation::ResetMzMin( void )
{
    MzMin = 0.0f;
}
/* MxMax Reset
 */
void TelescopeOrientation::ResetMxMax( void )
{
    MxMax = 0.0f;
}
/* MyMax Reset
 */
void TelescopeOrientation::ResetMyMax( void )
{
    MyMax = 0.0f;
}
/* MzMax Reset
 */
void TelescopeOrientation::ResetMzMax( void )
{
    MzMax = 0.0f;
}


/* set the offset for the Azimuth
 */
void TelescopeOrientation::SetMagneticOffset( float Offset )
{
    MagneticOffset = Offset;
}

/* get the offset for the Azimuth
 */
float TelescopeOrientation::GetMagneticOffset( void )
{
    return MagneticOffset;
}
/* set the offset for the Altitude
 */
void TelescopeOrientation::SetAccelOffset( float Offset )
{
    AccelOffset = Offset;
}

/* set the offset for the Altitude
 */
float TelescopeOrientation::GetAccelOffset( void )
{
    return AccelOffset;
}