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
        bool TelescopeOrientationInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the heading of the Telescope
     * @return double heading 
     */
        void GetOrientation( float* Pitch, float* Roll, float* Heading );
    /** Get the Pitch of the Telescope
     */
        static TelescopeOrientation Orient;      /**< Only one is required */  
    private:
        
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
