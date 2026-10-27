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
#include <Eigen/LU> /* needed for .inverse */
#include <Eigen/Dense>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#ifdef TIMING
#include "GPIO.h"
#endif

using namespace Eigen;
using namespace std;

TelescopeOrientation TelescopeOrientation::Orient;

#define ACCELXUPCAL   ((uint16_t) 0x0001)
#define ACCELXDOWNCAL ((uint16_t) 0x0002)
#define ACCELYUPCAL   ((uint16_t) 0x0004)
#define ACCELYDOWNCAL ((uint16_t) 0x0008)
#define ACCELZUPCAL   ((uint16_t) 0x0010)
#define ACCELZDOWNCAL ((uint16_t) 0x0020)
#define MAGZDOWNCAL   ((uint16_t) 0x0040)
#define MAGYDOWNCAL   ((uint16_t) 0x0080)
#define MAGXDOWNCAL   ((uint16_t) 0x0100)
#define ALLCAL        ((uint16_t) 0x01FF)


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
    XDownCalEnable = false; 
    XUpCalEnable = false;   
    YDownCalEnable = false; 
    YUpCalEnable = false;   
    ZdownCalEnable = false; 
    ZUpCalEnable = false;   
    XbDownCalEnable = false;
    YbDownCalEnable = false;
    ZbDownCalEnable = false;
    DDDCalEnable = false;   
    
   // printf( "Ax, Ay, Az, Mx, My, Mz, Mcx, Mcy, Mcz\n" );
//    printf( "Ax, Ay, Az, Mx, My, Mz\n" );
//    printf( "Ax, Ay, Az, Mx, My, Mz, Mcx, Mcy, Mcz\n" );
    /* Calibration flag */
    CalFlag = 0u;
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
        float ACC1, ACC2, ACC3, ACC4, ACC5, ACC6, ACC7, ACC8, ACC9, ACC10, ACC11, ACC12;
        float MSC1, MSC2, MSC3, MSC4, MSC5, MSC6, MSC7, MSC8, MSC9, MO1, MO2, MO3;
        sscanf( Configuration, 
                                "ACC11=%f, ACC12=%f, ACC13=%f, ACC10=%f, ACC21=%f, ACC22=%f, ACC23=%f, ACC20=%f, ACC31=%f, ACC32=%f, ACC33=%f, ACC30=%f, MSC11=%f, MSC12=%f, MSC13=%f, MSC21=%f, MSC22=%f, MSC23=%f, MSC31=%f, MSC32=%f, MSC33=%f, MO1=%f, MO2=%f, MO3=%f, MagneticOffset=%f, AccelOffset=%f", 
                                 &ACC1, &ACC2,  &ACC3,  &ACC4,
                                 &ACC5, &ACC6,  &ACC7,  &ACC8,
                                 &ACC9, &ACC10, &ACC11, &ACC12,
                                 &MSC1, &MSC2,  &MSC3,
                                 &MSC4, &MSC5,  &MSC6,
                                 &MSC7, &MSC8,  &MSC9,
                                 &MO1,  &MO2,   &MO3,
                                 &MagneticOffset, &AccelOffset );
        ACC(0,0) = ACC1;
        ACC(1,0) = ACC2;
        ACC(2,0) = ACC3;
        ACC(3,0) = ACC4;
        ACC(0,1) = ACC5;
        ACC(1,1) = ACC6;
        ACC(2,1) = ACC7;
        ACC(3,1) = ACC8;
        ACC(0,2) = ACC9;
        ACC(1,2) = ACC10;
        ACC(2,2) = ACC11;
        ACC(3,2) = ACC12;
        MSC(0,0) = MSC1;
        MSC(0,1) = MSC2;
        MSC(0,2) = MSC3;
        MSC(1,0) = MSC4;
        MSC(1,1) = MSC5;
        MSC(1,2) = MSC6;
        MSC(2,0) = MSC7;
        MSC(2,1) = MSC8;
        MSC(2,2) = MSC9;
        M_O(0,0) = MO1;
        M_O(1,0) = MO2;
        M_O(2,0) = MO3;
//        printf( "Calibrations loaded\n\r" );
    }
    else
    {
        MagneticOffset = 0.0f;
        AccelOffset = 0.0f;
//        printf( "Using default calibrations\n\r" );
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
    sprintf( Configuration, 
                                "ACC11=%f, ACC12=%f, ACC13=%f, ACC10=%f, ACC21=%f, ACC22=%f, ACC23=%f, ACC20=%f, ACC31=%f, ACC32=%f, ACC33=%f, ACC30=%f, MSC11=%f, MSC12=%f, MSC13=%f, MSC21=%f, MSC22=%f, MSC23=%f, MSC31=%f, MSC32=%f, MSC33=%f, MO1=%f, MO2=%f, MO3=%f, MagneticOffset=%f, AccelOffset=%f", 
                                 ACC(0,0), ACC(1,0), ACC(2,0), ACC(3,0),
                                 ACC(0,1), ACC(1,1), ACC(2,1), ACC(3,1),
                                 ACC(0,2), ACC(1,2), ACC(2,2), ACC(3,2),
                                 MSC(0,0), MSC(1,0), MSC(2,0),
                                 MSC(0,1), MSC(1,1), MSC(2,1),
                                 MSC(0,2), MSC(1,2), MSC(2,2),
                                 M_O(0,0), M_O(1,0), M_O(2,0),
                                 MagneticOffset, AccelOffset );        
    File.SetConfig( Path, Configuration );
    printf( "Calibrations saved\n\r" );
}

/* TelescopeOrientationRun
 *  Runs the Accelerometer and the Magnetometer.
 */
void TelescopeOrientation::Run( void )
{
    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_ORIENTATION_PIN , true );
    #endif

    bool NewMagnetoData = HalMagnetometer::Magneto.Run();
    bool NewAccelerData = HalAccelerometer::Accelerometer.Run();
    /* Accelerometer Calibration data collection */
    if ( NewAccelerData )
    {
        if ( XDownCalEnable )
        {
            if ( XbDownCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                XbDownX[ XbDownCount ] = Ax;
                XbDownY[ XbDownCount ] = Ay;
                XbDownZ[ XbDownCount ] = Az;
                XbDownCount++;
            }
        }
        if ( XUpCalEnable )
        {
            if ( XbUpCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                XbUpX[ XbUpCount ] = Ax;
                XbUpY[ XbUpCount ] = Ay;
                XbUpZ[ XbUpCount ] = Az;
                XbUpCount++;
            }
        }
        if ( YDownCalEnable )
        {
            if ( YbDownCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                YbDownX[ YbDownCount ] = Ax;
                YbDownY[ YbDownCount ] = Ay;
                YbDownZ[ YbDownCount ] = Az;
                YbDownCount++;
            }
        }
        if ( YUpCalEnable )
        {
            if ( YbUpCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                YbUpX[ YbUpCount ] = Ax;
                YbUpY[ YbUpCount ] = Ay;
                YbUpZ[ YbUpCount ] = Az;
                YbUpCount++;
            }
        }
        if ( ZdownCalEnable )
        {
            if ( ZbDownCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                ZbDownX[ ZbDownCount ] = Ax;
                ZbDownY[ ZbDownCount ] = Ay;
                ZbDownZ[ ZbDownCount ] = Az;
                ZbDownCount++;
            }
        }
        if ( ZUpCalEnable )
        {
            if ( ZbUpCount < MAXARRAYSIZE )
            {
                HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
                ZbUpX[ ZbUpCount ] = Ax;
                ZbUpY[ ZbUpCount ] = Ay;
                ZbUpZ[ ZbUpCount ] = Az;
                ZbUpCount++;
            }
        }
    }
    /* Magnetometer Calibration data collection */
    if ( NewMagnetoData )
    {
        if ( XbDownCalEnable )
        {
            if ( XMagDownCount < MAXARRAYSIZE )
            {
                HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
                MagXDownX[ XMagDownCount ] = Mx;
                MagXDownY[ XMagDownCount ] = My;
                MagXDownZ[ XMagDownCount ] = Mz;
                XMagDownCount++;
            }
        }    
        if ( YbDownCalEnable )
        {
            if ( YMagDownCount < MAXARRAYSIZE )
            {
                HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
                MagYDownX[ YMagDownCount ] = Mx;
                MagYDownY[ YMagDownCount ] = My;
                MagYDownZ[ YMagDownCount ] = Mz;
                YMagDownCount++;
            }
        }    
        if ( ZbDownCalEnable )
        {
            if ( ZMagDownCount < MAXARRAYSIZE )
            {
                HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
                MagZDownX[ ZMagDownCount ] = Mx;
                MagZDownY[ ZMagDownCount ] = My;
                MagZDownZ[ ZMagDownCount ] = Mz;
                ZMagDownCount++;
            }
        }    
        /* not sure this is needed */
        if ( DDDCalEnable )
        {
            if ( DDDMagCount < MAXARRAYSIZE )
            {
                HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
                MagDDownX[ DDDMagCount ] = Mx;
                MagDDownY[ DDDMagCount ] = My;
                MagDDownZ[ DDDMagCount ] = Mz;
                DDDMagCount++;
            }
        }
        /* update minimums and maximums */
        CheckMaxMin( Mx, &MxMax, &MxMin );
        CheckMaxMin( My, &MyMax, &MyMin );
        CheckMaxMin( Mz, &MzMax, &MzMin );
    }
    #ifdef TIMING
    GPIO::gpio.SetPinState( TELESCOPE_ORIENTATION_PIN , false );
    #endif
}

/*
 *
 */
void TelescopeOrientation::CheckMaxMin( float M, float* Max, float* Min )
{
    if ( M > *Max )
    {
        *Max = M;
    }
    if ( M < *Min )
    {
        *Min = M;
    }
}

/*
 *
 */
void TelescopeOrientation::GetOrientation( float* Pitch, float* Roll, float* Heading )
{
    float CosRoll = 0.0f;
    float SinRoll = 0.0f;
    float CosPitch = 0.0f;
    float SinPitch = 0.0f;
    Matrix14f RawAcc;
    Matrix13f A;
        
    /* get filtered sensor data */
    HalAccelerometer::Accelerometer.GetAll( &Ax, &Ay, &Az );
    HalMagnetometer::Magneto.GetAll( &Mx, &My, &Mz );
//    printf( "%f, %f, %f, ", Mx, My, Mz );
  //  printf( "%f, %f, %f, %f, %f, %f\n", Ax, Ay, Az, Mx, My, Mz);
//    printf( "%f, %f, %f, ", Ax, Ay, Az );
    /* fill raw accelerometer matrix*/
    RawAcc(0,0) = Ax;
    RawAcc(0,1) = Ay;
    RawAcc(0,2) = Az;
    /* Apply calibration */
    A = RawAcc*ACC;
    /* Calculate Pitch */
    *Pitch = asin ( A(0,0) );
    CosPitch = cos(*Pitch);
    SinPitch = sin(*Pitch);
    
    /* Calculate Roll */
    *Roll = asin( A(0,1) / CosPitch );
    CosRoll = cos(*Roll);
    SinRoll = sin(*Roll);

    /* magneto values with correction */
    Matrix31f RawM;
    Matrix31f Mo;
    Matrix31f Mc;
    Matrix31f Mctemp;
    Matrix31f MNorm;
/* temp */
//    MNorm(0, 0) = ( MxMax - MxMin ) / 2.0f;
//    MNorm(1, 0) = ( MyMax - MyMin ) / 2.0f;
//    MNorm(2, 0) = ( MzMax - MzMin ) / 2.0f;

    
    RawM(0, 0) = Mx;//  / MxNorm;
    RawM(1, 0) = My;//  / MyNorm;
    RawM(2, 0) = Mz;//  / MzNorm;
    Mo = RawM - M_O;
    Mctemp = MSC * Mo;
    Mc(0, 0) = Mctemp(0, 0);// / MNorm(0, 0);
    Mc(1, 0) = Mctemp(1, 0);// / MNorm(1, 0);
    Mc(2, 0) = Mctemp(2, 0);// / MNorm(2, 0);
    /* ellipsoid vs Sphere  */
//    std::cout << Mc << std::endl;
 //   printf( "%f, %f, %f, \n", Mc(0, 0), Mc(1, 0), Mc(2, 0) );
    /* Calculation variables */
    float XComponent = 0.0f;
    float YComponent = 0.0f;
        
    /* Calculate X and Y components then heading */
    XComponent = ( Mc(0, 0) * CosPitch ) + ( Mc(2, 0) * SinPitch );
    YComponent = ( Mc(0, 0) * SinRoll * SinPitch ) + ( Mc(1, 0) * CosRoll ) - ( Mc(2, 0) * SinRoll * CosPitch );
    /* Use atan2 because of the sign ambiguity of atan */
    *Heading = atan2( -YComponent , XComponent ); 
    
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
    /* shut up warning until this is removed */
    (void)Enable;
}    


/* SetAccAxDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAxDown( bool Enable )
{
    XDownCalEnable= false;
    if ( Enable )
    {
        XDownCalEnable= true;
        XUpCalEnable = false;
        YDownCalEnable = false;
        YUpCalEnable = false;
        ZdownCalEnable = false; 
        ZUpCalEnable = false;
        CalFlag = CalFlag | ACCELXDOWNCAL;
        printf("Accelerometer X down enabled\n\r");
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
        printf("Accelerometer X down disabled\n\r");
    }
    printf("CalFlag = %d ", CalFlag);
    printf("XbDownCount = %d\n\r", XbDownCount );
}

/* SetAccAxUp
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAxUp( bool Enable )
{
    XUpCalEnable = false;
    if ( Enable )
    {
        XDownCalEnable= false; 
        XUpCalEnable = true;
        YDownCalEnable = false;
        YUpCalEnable = false;
        ZdownCalEnable = false; 
        ZUpCalEnable = false;
        CalFlag = CalFlag | ACCELXUPCAL;
        printf("Accelerometer X up enabled\n\r");
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
        printf("Accelerometer X down disabled\n\r");
    } 
    printf("CalFlag = %d ", CalFlag);
    printf("XbUpCount = %d", XbUpCount );
}

/* SetAccAyDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAyDown( bool Enable )
{          
    YDownCalEnable = false;
    if ( Enable )
    {
        XDownCalEnable= false; 
        XUpCalEnable = false;
        YDownCalEnable = true;
        YUpCalEnable = false;
        ZdownCalEnable = false; 
        ZUpCalEnable = false;
        CalFlag = CalFlag |  ACCELYDOWNCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }
    printf("YbDownCount = %d", YbDownCount );
}

/* SetAccAyUp
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAyUp( bool Enable )
{       
    YUpCalEnable = false;
    if ( Enable )
    {
        XDownCalEnable= false; 
        XUpCalEnable = false;
        YDownCalEnable = false;
        YUpCalEnable = true;
        ZdownCalEnable = false; 
        ZUpCalEnable = false;
        CalFlag = CalFlag |  ACCELYUPCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }  
    printf("YbUpCount = %d", YbUpCount );
}

/* SetAccAzDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAzDown( bool Enable )
{
    ZdownCalEnable = false; 
    if ( Enable )
    {
        XDownCalEnable= false; 
        XUpCalEnable = false;
        YDownCalEnable = false;
        YUpCalEnable = false;
        ZdownCalEnable = true; 
        ZUpCalEnable = false;
        CalFlag = CalFlag |  ACCELZDOWNCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }
    printf("ZbDownCount = %d", ZbDownCount );
}

/* SetAccAzUp
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetAccAzUp( bool Enable )
{ 
    ZUpCalEnable= false; 
    if ( Enable )
    {
        XDownCalEnable= false; 
        XUpCalEnable = false;
        YDownCalEnable = false;
        YUpCalEnable = false;
        ZdownCalEnable = false; 
        ZUpCalEnable = true;
        CalFlag = CalFlag |  ACCELZUPCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    } 
    printf("ZbUpCount = %d", ZbUpCount );
}

/* SetMagXbDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetMagXbDown( bool Enable )
{
       
    XbDownCalEnable = false;
    if ( Enable )
    {
        XbDownCalEnable = true;
        YbDownCalEnable = false;
        ZbDownCalEnable = false;
        CalFlag = CalFlag |  MAGXDOWNCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }
    printf("XMagDownCount = %d", XMagDownCount );
}

/* SetMagYbDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetMagYbDown( bool Enable )
{
    YbDownCalEnable = false;
    if ( Enable )
    {
        XbDownCalEnable = false;
        YbDownCalEnable = true;
        ZbDownCalEnable = false;
        CalFlag = CalFlag |  MAGYDOWNCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }
    printf("YMagDownCount = %d", YMagDownCount );
}

/* SetMagZbDown
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetMagZbDown( bool Enable )
{         
    ZbDownCalEnable = false;
    if ( Enable )
    {
        XbDownCalEnable = false;
        YbDownCalEnable = false;
        ZbDownCalEnable = true;
        CalFlag = CalFlag |  MAGZDOWNCAL;
    }
    else if ( ( CalFlag & ALLCAL ) == ALLCAL )
    {
        Calibration();
    }    
    printf("ZMagDownCount = %d", ZMagDownCount );
}
/* SetMagDDD
 * @param Enable or disable calibration 
 */
void TelescopeOrientation::SetMagDDD( bool Enable )
{
    DDDCalEnable = Enable;               
//    XbDownCalEnable = !Enable;
//    YbDownCalEnable = !Enable;
//    ZbDownCalEnable = !Enable;
}

/* Calibration
 */
void TelescopeOrientation::Calibration( void )
{
    uint32_t arraycount = 0;
    uint32_t rows = ZbDownCount + ZbUpCount + YbDownCount + YbUpCount + XbDownCount + XbUpCount;
    Eigen::MatrixXf Y(rows,3);   /**< Accelerometer least squares matrix */
    Eigen::MatrixXf W(rows,4);   /**< Accelerometer least squares matrix */

  //  MxNorm = ( MxMax - MxMin ) / 2.0f;
  //  MyNorm = ( MyMax - MyMin ) / 2.0f;
  //  MzNorm = ( MzMax - MzMin ) / 2.0f;
    
    /* fill Accelerometer Arrays with all six sets of measurements */
    for ( uint16_t i = 0; i < ZbDownCount; i++ )
    {
        W(arraycount, 0) = ZbDownX[i];
        W(arraycount, 1) = ZbDownY[i];
        W(arraycount, 2) = ZbDownZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = 0;
        Y(arraycount, 1) = 0;
        Y(arraycount, 2) = 1;
        arraycount++;
    }
    for ( uint16_t i = 0; i < ZbUpCount; i++ )
    {
        W(arraycount, 0) = ZbUpX[i];
        W(arraycount, 1) = ZbUpY[i];
        W(arraycount, 2) = ZbUpZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = 0;
        Y(arraycount, 1) = 0;
        Y(arraycount, 2) = -1;
        arraycount++;
    }
    
    for ( uint16_t i = 0; i < YbDownCount; i++ )
    {
        W(arraycount, 0) = YbDownX[i];
        W(arraycount, 1) = YbDownY[i];
        W(arraycount, 2) = YbDownZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = 0;
        Y(arraycount, 1) = 1;
        Y(arraycount, 2) = 0;
        arraycount++;
    }
    
    for ( uint16_t i = 0; i < YbUpCount; i++ )
    {
        W(arraycount, 0) = YbUpX[i];
        W(arraycount, 1) = YbUpY[i];
        W(arraycount, 2) = YbUpZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = 0;
        Y(arraycount, 1) = -1;
        Y(arraycount, 2) = 0;
        arraycount++;
    }

    for ( uint16_t i = 0; i < XbDownCount; i++ )
    {
        W(arraycount, 0) = XbDownX[i];
        W(arraycount, 1) = XbDownY[i];
        W(arraycount, 2) = XbDownZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = 1;
        Y(arraycount, 1) = 0;
        Y(arraycount, 2) = 0;
        arraycount++;
    }
    
    for ( uint16_t i = 0; i < XbUpCount; i++ )
    {
        W(arraycount, 0) = XbUpX[i];
        W(arraycount, 1) = XbUpY[i];
        W(arraycount, 2) = XbUpZ[i];
        W(arraycount, 3) = 1;
        Y(arraycount, 0) = -1;
        Y(arraycount, 1) = 0;
        Y(arraycount, 2) = 0;
        arraycount++;
    }
    /* Calculate the accelerometer factors */
    Eigen::MatrixXf Wt;  /**< Accelerometer least squares matrix */
    Matrix44f WWT;
    Eigen::MatrixXf WInv;
    Eigen::MatrixXf WTWW;
//     std::cout << "Here is the matrix w:\n" << W << std::endl;
//     std::cout << "Here is the matrix Y:\n" << Y << std::endl;
    Wt = W.transpose();
//     std::cout << "Here is the matrix wt:\n" << Wt << std::endl;
    WWT = Wt*W;
//     std::cout << "Here is the matrix WWT:\n" << WWT << std::endl;
    WInv = WWT.inverse();
//     std::cout << "Here is the matrix WInv:\n" << WInv << std::endl;
    WTWW = WInv*Wt;
//     std::cout << "Here is the matrix WTWW:\n" << WTWW << std::endl;
    ACC = WTWW*Y;
//     std::cout << "Here is the matrix ACC:\n" << ACC << std::endl;

    /* Fill the Matrices for the magneto offset and scaling calibration */
    arraycount = 0;
    rows = XMagDownCount + YMagDownCount + ZMagDownCount;
    Eigen::MatrixXf WM(rows, 1);
    Eigen::MatrixXf H(rows, 6);
    for ( uint16_t i = 0; i < XMagDownCount; i++ )
    {
        WM(arraycount, 0) = MagXDownX[i] * MagXDownX[i];
        H(arraycount, 0) = MagXDownX[i];
        H(arraycount, 1) = MagXDownY[i];
        H(arraycount, 2) = MagXDownZ[i];
        H(arraycount, 3) = -1 * ( MagXDownY[i] * MagXDownY[i] );
        H(arraycount, 4) = -1 * ( MagXDownZ[i] * MagXDownZ[i] );
        H(arraycount, 5) = 1;
        arraycount++;
    }
    for ( uint16_t i = 0; i < YMagDownCount; i++ )
    {
        WM(arraycount, 0) = MagYDownX[i] * MagYDownX[i];
        H(arraycount, 0) = MagYDownX[i];
        H(arraycount, 1) = MagYDownY[i];
        H(arraycount, 2) = MagYDownZ[i];
        H(arraycount, 3) = -1 * ( MagYDownY[i] * MagYDownY[i] );
        H(arraycount, 4) = -1 * ( MagYDownZ[i] * MagYDownZ[i] );
        H(arraycount, 5) = 1;
        arraycount++;
    }
    for ( uint16_t i = 0; i < ZMagDownCount; i++ )
    {
        WM(arraycount, 0) = MagZDownX[i] * MagZDownX[i];
        H(arraycount, 0) = MagZDownX[i];
        H(arraycount, 1) = MagZDownY[i];
        H(arraycount, 2) = MagZDownZ[i];
        H(arraycount, 3) = -1 * ( MagZDownY[i] * MagZDownY[i] );
        H(arraycount, 4) = -1 * ( MagZDownZ[i] * MagZDownZ[i] );
        H(arraycount, 5) = 1;
        arraycount++;
    }
    Eigen::MatrixXf HT(6,rows);
    Eigen::MatrixXf HHT;
    Eigen::MatrixXf HInv;
    Eigen::MatrixXf HTHH;
    Eigen::MatrixXf X;
//     std::cout << "Here is the matrix H:\n" << H << std::endl;
//     std::cout << "Here is the matrix WM:\n" << WM << std::endl;
    HT = H.transpose();
//     std::cout << "Here is the matrix HT:\n" << HT << std::endl;
    HHT = HT*H;
//     std::cout << "Here is the matrix HHT:\n" << HHT << std::endl;
    HInv = HHT.inverse();
//     std::cout << "Here is the matrix HInv:\n" << HInv << std::endl;
    HTHH = HInv*HT;
//     std::cout << "Here is the matrix HTHH:\n" << HTHH << std::endl;
    X = HTHH*WM;
//     std::cout << "Here is the matrix X:\n" << X << std::endl;
    
    float M_OSx = X(0, 0) / 2.0f;
    float M_OSy = X(1, 0) / ( 2.0f * X(3, 0) );
    float M_OSz = X(2, 0) / ( 2.0f * X(4, 0) );
//    printf("M_OS %f, %f, %f\n\r",  M_OSx, M_OSy, M_OSz);
    
    float A = X(5, 0) + ( M_OSx * M_OSx ) + X(3, 0) + ( M_OSy * M_OSy ) + X(4, 0) + ( M_OSz * M_OSz );
    float B = A / X(3, 0);
    float C = A / X(4, 0);
//    printf("ABC %f, %f, %f\n\r",  A, B, C);
    float M_SCX = sqrt( A );
    float M_SCY = sqrt( B );
    float M_SCZ = sqrt( C );
//    printf("M_SC %f, %f, %f\n\r",  M_SCX, M_SCY, M_SCZ);
    /* Fill the matrices with the compensated magnetometer readings */
    Eigen::MatrixXf HMZb(ZMagDownCount,3);
    Eigen::MatrixXf HMYb(YMagDownCount,3);
    Eigen::MatrixXf HMXb(XMagDownCount,3);
    Eigen::MatrixXf WMZb(ZMagDownCount,1);
    Eigen::MatrixXf WMYb(YMagDownCount,1);
    Eigen::VectorXf WMXb(XMagDownCount,1);
    
    for ( uint16_t i = 0; i <  ZMagDownCount; i++ )
    {
        float xx = MagZDownX[i] - M_OSx;
        float yy = MagZDownY[i] - M_OSy;
        float zz = MagZDownZ[i] - M_OSz;
        float xxx = xx / M_SCX;
        float yyy = yy / M_SCY;
        float zzz = zz / M_SCZ;
        HMZb( i, 0 ) = xxx;
        HMZb( i, 1 ) = yyy;
        HMZb( i, 2 ) = zzz;
        WMZb( i, 0 ) = sqrt( ( xxx * xxx ) + ( yyy * yyy ) + ( zzz * zzz ) );
    }
    
    for ( uint16_t i = 0; i <  YMagDownCount; i++ )
    {
        float xx = MagYDownX[i] - M_OSx;
        float yy = MagYDownY[i] - M_OSy;
        float zz = MagYDownZ[i] - M_OSz;
        float xxx = xx / M_SCX;
        float yyy = yy / M_SCY;
        float zzz = zz / M_SCZ;
        HMYb( i, 0 ) = xxx;
        HMYb( i, 1 ) = yyy;
        HMYb( i, 2 ) = zzz;
        WMYb( i, 0 ) = sqrt( ( xxx * xxx ) + ( yyy * yyy ) + ( zzz * zzz ) );
    }
    
    for ( uint16_t i = 0; i <  XMagDownCount; i++ )
    {
        float xx = MagXDownX[i] - M_OSx;
        float yy = MagXDownY[i] - M_OSy;
        float zz = MagXDownZ[i] - M_OSz;
        float xxx = xx / M_SCX;
        float yyy = yy / M_SCY;
        float zzz = zz / M_SCZ;
        HMXb( i, 0 ) = xxx;
        HMXb( i, 1 ) = yyy;
        HMXb( i, 2 ) = zzz;
        WMXb( i, 0 ) = sqrt( ( xxx * xxx ) + ( yyy * yyy ) + ( zzz * zzz ) );
    }

    /* Calculate the Zb down misalignment matrix */
    Eigen::MatrixXf HMZbT(3,ZMagDownCount);
    Eigen::MatrixXf XMz;
    Eigen::MatrixXf Hz;
    Eigen::MatrixXf HzInv;
    Eigen::MatrixXf HiHz;
//     std::cout << "Here is the matrix WMZb:\n" << WMZb << std::endl;
//     std::cout << "Here is the matrix HMZb:\n" << HMZb << std::endl;
    HMZbT = HMZb.transpose();
//     std::cout << "Here is the matrix HMZbT:\n" << HMZbT << std::endl;
    Hz = HMZbT * HMZb;
//     std::cout << "Here is the matrix Hz:\n" << Hz << std::endl;
    HzInv = Hz.inverse();
//     std::cout << "Here is the matrix HzInv:\n" << HzInv << std::endl;
    HiHz = HzInv * HMZbT;
//     std::cout << "Here is the matrix HiHz:\n" << HiHz << std::endl;
    XMz = HiHz * WMZb;
//     std::cout << "Here is the matrix XMz:\n" << XMz << std::endl;
    
    /* Calculate the Yb down misalignment matrix */
    Eigen::MatrixXf HMYbT(3,YMagDownCount);
    Eigen::MatrixXf XMy;
    Eigen::MatrixXf Hy;
    Eigen::MatrixXf HyInv;
    Eigen::MatrixXf HiHy;
//     std::cout << "Here is the matrix HMYb:\n" << HMYb << std::endl;
//     std::cout << "Here is the matrix WMYb:\n" << WMYb << std::endl;
    HMYbT = HMYb.transpose();
//     std::cout << "Here is the matrix HMYbT:\n" << HMYbT << std::endl;
    Hy = HMYbT * HMYb;
//     std::cout << "Here is the matrix Hy:\n" << Hy << std::endl;
    HyInv = Hy.inverse();
//     std::cout << "Here is the matrix HyInv:\n" << HyInv << std::endl;
    HiHy = HyInv * HMYbT;
//     std::cout << "Here is the matrix HiHy:\n" << HiHy << std::endl;
    XMy = HiHy * WMYb;
//     std::cout << "Here is the matrix XMy:\n" << XMy << std::endl;
    
    /* Calculate the Xb down misalignment matrix */
    Eigen::MatrixXf HMXbT(3,XMagDownCount);
    Eigen::MatrixXf XMx;
    Eigen::MatrixXf Hx;
    Eigen::MatrixXf HxInv;
    Eigen::MatrixXf HiHx;
//     std::cout << "Here is the matrix HMXb:\n" << HMXb << std::endl;
//     std::cout << "Here is the matrix WMXb:\n" << WMXb << std::endl;
    HMXbT = HMXb.transpose();
//     std::cout << "Here is the matrix HMXbT:\n" << HMXbT << std::endl;
    Hx = HMXbT * HMXb;
//     std::cout << "Here is the matrix Hx:\n" << Hx << std::endl;
    HxInv = Hx.inverse();
//     std::cout << "Here is the matrix HxInv:\n" << HxInv << std::endl;
    HiHx = HxInv * HMXbT;
//     std::cout << "Here is the matrix HiHx:\n" << HiHx << std::endl;
    XMx = HiHx * WMXb;
//     std::cout << "Here is the matrix XMx:\n" << XMx << std::endl;
    
    /* normalise and combine the three misalignment matrices to give M_m matrix */
    float Magnitude; 
    Magnitude = sqrt ( ( XMz(0, 0) * XMz(0, 0) ) + ( XMz(1, 0) * XMz(1, 0) ) + ( XMz(2, 0) * XMz(2, 0) ) );
    XMz(0, 0) = XMz(0, 0) / Magnitude;
    XMz(1, 0) = XMz(1, 0) / Magnitude;
    XMz(2, 0) = XMz(2, 0) / Magnitude;
    Magnitude = sqrt ( ( XMy(0, 0) * XMy(0, 0) ) + ( XMy(1, 0) * XMy(1, 0) ) + ( XMy(2, 0) * XMy(2, 0) ) );
    XMy(0, 0) = XMy(0, 0) / Magnitude;
    XMy(1, 0) = XMy(1, 0) / Magnitude;
    XMy(2, 0) = XMy(2, 0) / Magnitude;
    Magnitude = sqrt ( ( XMx(0, 0) * XMx(0, 0) ) + ( XMx(1, 0) * XMx(1, 0) ) + ( XMx(2, 0) * XMx(2, 0) ) );
    XMx(0, 0) = XMx(0, 0) / Magnitude;
    XMx(1, 0) = XMx(1, 0) / Magnitude;
    XMx(2, 0) = XMx(2, 0) / Magnitude;

    /* combine the hard and soft iron matrices, assume no soft iron and M_si is an identity matrix */
    Matrix33f M_m;
    Matrix33f M_sc;
    Matrix31f M_os;
    M_m(0, 0) = XMx(0, 0);
    M_m(1, 0) = XMx(1, 0);
    M_m(2, 0) = XMx(2, 0);
    M_m(0, 1) = XMy(0, 0);
    M_m(1, 1) = XMy(1, 0);
    M_m(2, 1) = XMy(2, 0);
    M_m(0, 2) = XMz(0, 0);
    M_m(1, 2) = XMz(1, 0);
    M_m(2, 2) = XMz(2, 0);
    M_sc(0, 0) = 1/M_SCX;
    M_sc(0, 1) = 0;
    M_sc(0, 2) = 0;
    M_sc(1, 0) = 0;
    M_sc(1, 1) = 1/M_SCY;
    M_sc(1, 2) = 0;
    M_sc(2, 0) = 0;
    M_sc(2, 1) = 0;
    M_sc(2, 2) = 1/M_SCZ;
    M_os(0, 0) = M_OSx;
    M_os(1, 0) = M_OSy;
    M_os(2, 0) = M_OSz;
//     std::cout << "Here is the matrix M_m:\n" << M_m << std::endl;
//     std::cout << "Here is the matrix M_sc:\n" << M_sc << std::endl;
    /* combine misalignment and offset matrices  */
    Matrix33f M_si;
    M_si(0, 0) = 1;
    M_si(0, 1) = 0;
    M_si(0, 2) = 0;
    M_si(1, 0) = 0;
    M_si(1, 1) = 1;
    M_si(1, 2) = 0;
    M_si(2, 0) = 0;
    M_si(2, 1) = 0;
    M_si(2, 2) = 1;
    MSC = M_m * M_sc;
    M_O = M_si * M_os;
 //    std::cout << "Here is the matrix M_os:\n" << M_os << std::endl;
 //    std::cout << "Here is the matrix M_si:\n" << M_si << std::endl;
 //    std::cout << "Here is the matrix M_O:\n" << M_O << std::endl;
    /* Save the calibration values */
    SaveConfig();
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
/* ToDo clean up unused functions */
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
    AxMin = +1000.0f;
}
/* AyMin Reset
 */
void TelescopeOrientation::ResetAyMin( void )
{
    AyMin = +1000.0f;
}
/* AzMin Reset
 */
void TelescopeOrientation::ResetAzMin( void )
{
    AzMin = +1000.0f;
}
/* AxMax Reset
 */
void TelescopeOrientation::ResetAxMax( void )
{
    AxMax = -1000.0f;
}
/* AyMax Reset
 */
void TelescopeOrientation::ResetAyMax( void )
{
    AyMax = -1000.0f;
}
/* AzMax Reset
 */
void TelescopeOrientation::ResetAzMax( void )
{
    AzMax = -1000.0f;
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
    MxMin = +1000.0f;
}
/* MyMin Reset
 */
void TelescopeOrientation::ResetMyMin( void )
{
    MyMin = +1000.0f;
}
/* MzMin Reset
 */
void TelescopeOrientation::ResetMzMin( void )
{
    MzMin = +1000.0f;
}
/* MxMax Reset
 */
void TelescopeOrientation::ResetMxMax( void )
{
    MxMax = -1000.0f;
}
/* MyMax Reset
 */
void TelescopeOrientation::ResetMyMax( void )
{
    MyMax = -1000.0f;
}
/* MzMax Reset
 */
void TelescopeOrientation::ResetMzMax( void )
{
    MzMax = -1000.0f;
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
