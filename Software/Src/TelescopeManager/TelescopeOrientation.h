/**
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
#ifndef TELESCOPEORIENTATION_H
#define TELESCOPEORIENTATION_H

#include <stdint.h>
#include "Runnable.h"

/** TelescopeOrientation
 * - Class to provide use to the magnetometer
 */
class TelescopeOrientation: public Runnable {
    public:
    /** Constructor
     */
        TelescopeOrientation( void );
    
    /** Initialise the Orientation
     * @return bool Initialisation status  
     */
        bool Init( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the heading of the Telescope
     * @return double heading 
     */
        void GetOrientation( float* Pitch, float* Roll, float* Heading );
    /** EnableCalibration
     * @Param Enable or disable calibration 
     */
        void EnableCalibration ( bool Enable );
    /** Ax getter
     */
        float GetAx( void );
    /** Ay getter
     */
        float GetAy( void );
    /** Az getter    
     */
        float GetAz( void );
    /** AxMin getter
     */
        float GetAxMin( void );
    /** AyMin getter
     */
        float GetAyMin( void );
    /** AzMin getter
     */
        float GetAzMin( void );
    /** AxMax getter
     */
        float GetAxMax( void );
    /** AyMax getter
     */
        float GetAyMax( void );
    /** AzMax getter
     */
        float GetAzMax( void );
    /** AxMin Reset
     */
        void ResetAxMin( void );
    /** AyMin Reset
     */
        void ResetAyMin( void );
    /** AzMin Reset
     */
        void ResetAzMin( void );
    /** AxMax Reset
     */
        void ResetAxMax( void );
    /** AyMax Reset
     */
        void ResetAyMax( void );
    /** AzMax Reset
     */
        void ResetAzMax( void );
    /** Mx getter
     */
        float GetMx( void );
    /** My getter
     */
        float GetMy( void );
    /** Mz getter
     */
        float GetMz( void );
    /** MxMim getter
     */
        float GetMxMin( void );
    /** MyMin getter
     */
        float GetMyMin( void );
    /** MzMin getter
     */
        float GetMzMin( void );
    /** MxMax getter
     */
        float GetMxMax( void );
    /** MyMax getter
     */
        float GetMyMax( void );
    /** Az getter
     */
        float GetMzMax( void );
    /** MxMin Reset
     */
        void ResetMxMin( void );
    /** MyMin Reset
     */
        void ResetMyMin( void );
    /** MzMin Reset
     */
        void ResetMzMin( void );
    /** MxMax Reset
     */
        void ResetMxMax( void );
    /** MyMax Reset
     */
        void ResetMyMax( void );
    /** MzMax Reset
     */
        void ResetMzMax( void );
    /** set the offset for the Azimuth
     */
        void SetMagneticOffset( float Offset );
    /** get the offset for the Azimuth
     */
        float GetMagneticOffset( void );
    /** set the offset for the Altitude
     */
        void SetAccelOffset( float Offset );
    /** set the offset for the Altitude
     */
        float GetAccelOffset( void );

        static TelescopeOrientation Orient;      /**< Only one is required */  
    private:
    /** Calibration
     */
        void Calibration( void );
        bool Calibrating;        
        float MagneticOffset;
        float AccelOffset;

    /** raw magneto values */
        float Mx;
        float My;
        float Mz;
    /** Raw Accel values */
        float Ax;
        float Ay;
        float Az;       
    /** Calibration Values
     */
        float MxMax;
        float MxMin;
        float MyMax;
        float MyMin;
        float MzMax;
        float MzMin;
        float AxMax;
        float AxMin;
        float AyMax;
        float AyMin;
        float AzMax;
        float AzMin;

};

#endif /* TELESCOPEORIENTATION_H */
