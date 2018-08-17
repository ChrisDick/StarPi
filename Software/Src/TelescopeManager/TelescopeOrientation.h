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
     * @param Enable or disable calibration 
     */
        void EnableCalibration ( bool Enable );
    /** Ax getter
     * @return float Raw Accelerometer X reading
     */
        float GetAx( void );
    /** Ay getter
     * @return float Raw Accelerometer Y reading
     */
        float GetAy( void );
    /** Az getter    
     * @return float Raw Accelerometer Z reading
     */
        float GetAz( void );
    /** AxMin getter
     * @return float Accelerometer X Minimum
     */
        float GetAxMin( void );
    /** AyMin getter
     * @return float Accelerometer Y Minimum
     */
        float GetAyMin( void );
    /** AzMin getter
     * @return float Accelerometer Z Minimum
     */
        float GetAzMin( void );
    /** AxMax getter
     * @return float Accelerometer X Maximum
     */
        float GetAxMax( void );
    /** AyMax getter
     * @return float Accelerometer Y Maximum
     */
        float GetAyMax( void );
    /** AzMax getter
     * @return float Accelerometer Z Maximum
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
     * @return float Raw Magnetometer X reading
     */
        float GetMx( void );
    /** My getter
     * @return float Raw Magnetometer Y reading
     */
        float GetMy( void );
    /** Mz getter
     * @return float Raw Magnetometer Z reading
     */
        float GetMz( void );
    /** MxMim getter
     * @return float Magnetometer X Minimum
     */
        float GetMxMin( void );
    /** MyMin getter
     * @return float Magnetometer Y Minimum
     */
        float GetMyMin( void );
    /** MzMin getter
     * @return float Magnetometer Z Minimum
     */
        float GetMzMin( void );
    /** MxMax getter
     * @return float Magnetometer X Maximum
     */
        float GetMxMax( void );
    /** MyMax getter
     * @return float Magnetometer Y Maximum
     */
        float GetMyMax( void );
    /** MzMax getter
     * @return float Magnetometer Z Maximum
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
     * @param Offset New offset value
     */
        void SetMagneticOffset( float Offset );
    /** get the offset for the Azimuth
     * @return float the current offset value
     */
        float GetMagneticOffset( void );
    /** set the offset for the Altitude
     * @param Offset New offset value
     */
        void SetAccelOffset( float Offset );
    /** set the offset for the Altitude
     * @return float the current offset value
     */
        float GetAccelOffset( void );
    /** Handle loading the calibration or load the defaults.
     */
        void LoadConfig( void );
    /** Handle saving of the calibration.
     */
        void SaveConfig( void );
        
        static TelescopeOrientation Orient;      /**< Only one is required */  
    private:
    /** Calibration
     */
        void Calibration( void );
     
        bool Calibrating;     /**< Current calibration state             */
        float MagneticOffset; /**< Offset for correcting compass heading */
        float AccelOffset;    /**< Offset for correcting pitch           */
        float Mx;             /**< raw magneto X value                   */
        float My;             /**< raw magneto Y value                   */
        float Mz;             /**< raw magneto Z value                   */
        float Ax;             /**< raw Accelerometer X value             */
        float Ay;             /**< raw Accelerometer Y value             */
        float Az;             /**< raw Accelerometer Z value             */
        float MxMax;         /**< Maximum Magneto X Value                */
        float MxMin;         /**< Minimum Magneto X Value                */
        float MyMax;         /**< Maximum Magneto Y Value                */
        float MyMin;         /**< Minimum Magneto Y Value                */
        float MzMax;         /**< Maximum Magneto Z Value                */
        float MzMin;         /**< Minimum Magneto Z Value                */
        float AxMax;         /**< Maximum Accelerometer X Value          */
        float AxMin;         /**< Minimum Accelerometer X Value          */
        float AyMax;         /**< Maximum Accelerometer Y Value          */
        float AyMin;         /**< Minimum Accelerometer Y Value          */
        float AzMax;         /**< Maximum Accelerometer Z Value          */
        float AzMin;         /**< Minimum Accelerometer Z Value          */

};

#endif /* TELESCOPEORIENTATION_H */
