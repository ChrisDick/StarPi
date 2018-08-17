/**
HalMagnetometer provides a generic interface to any Magnetometer
supported by the I2Cdevlib library. It uses a config file to select 
which sensor to use. 

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
            bool Init( void );
        /** Runs the filter
         */
            void Run( void );
        /** Access to the Magnetometer data.
         * @param Mx Pointer to X axis result
         * @param My Pointer to Y axis result
         * @param Mz Pointer to Z axis result
         */
            void GetAll( float* Mx, float* My, float* Mz );

            static HalMagnetometer Magneto; /**< only one is required */
        
    private:
        /** Get the raw heading of the Compass
         * This function reads 6 bytes at once over the I2C 
         * instead of 3 transactions.
         * @param X Raw X result
         * @param Y Raw Y result
         * @param Z Raw Z result
         */
            void GetRawData( int16_t* X, int16_t* Y, int16_t* Z );
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
            
            double FilterX[5];    /**< storage for X axis filter data                      */
            double FilterY[5];    /**< storage for Y axis filter data                      */
            double FilterZ[5];    /**< storage for Z axis filter data                      */
            uint8_t FilterCount;  /**< counter to keep track of where to store latest data */
            double Scaling;       /**< scaling for the raw data                            */
};

#endif /* HALMMAGNETOMETER_H */
