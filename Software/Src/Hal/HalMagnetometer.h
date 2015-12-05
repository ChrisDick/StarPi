/**
HalMagnetometer provides a generic interface to any Magnetometer
supported by the I2Cdevlib library. It uses a config file to select 
which sensor to use. 

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
#ifndef HALMMAGNETOMETER_H
#define HALMMAGNETOMETER_H

#include <stdint.h>
#include "Runnable.h"

/** HalMagnetometer
 * - Class to provide use to the magnetometer
 */
class HalMagnetometer: public Runnable {
    public:
    /** Constructor
     */
        HalMagnetometer( void );
    
    /** Initialise the Compass
     * @return bool Initialisation status  
     */
        bool HalMagnetometerInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** Access to the Magnetometer data.
     */
        void HalMagnetometerGetAll( float* Mx, float* My, float* Mz );

        static HalMagnetometer Magneto;
        
        private:
    /** Get the raw X heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetXRawHeading( void );
    /** Get the raw Y heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetYRawHeading( void );
    /** Get the raw Z heading of the Compass
     * @return int16_t heading 
     */
        int16_t GetZRawHeading( void );
        
        double FilterX[5];    /**< storage for X axis filter data*/
        double FilterY[5];    /**< storage for Y axis filter data*/
        double FilterZ[5];    /**< storage for Z axis filter data*/
        uint8_t FilterCount; /**< counter to keep track of where to store latest data */
        double Scaling;       /**< scaling for the raw data */
};

#endif /* HALMMAGNETOMETER_H */
