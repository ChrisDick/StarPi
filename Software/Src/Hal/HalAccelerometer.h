/**
HalAccelerometer provides a generic interface to any accelerometer
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
#ifndef HAL_ACCELEROMETER_H
#define HAL_ACCELEROMETER_H

#include <stdint.h>
#include "Runnable.h"

/** HalAccelerometer
 * - Class to provide use of the Accelerometer
 */
class HalAccelerometer: public Runnable {
    public:
        /** Constructor
        */
            HalAccelerometer( void );
        /** Initialise the Accelerometer
         * @return Status initialisation (true = success)
         */    
            bool  Init( void );
        /** runs the filter and updates the Roll and Pitch
         */
            void  Run( void );
        /** Access to the Accelerometer data.
         */
            void GetAll( float* Ax, float* Ay, float* Az );

            static HalAccelerometer Accelerometer; /**< Only one copy of the Acceleromter is required */
        
    private:
        /** Get the raw value of the Accelerometer
         * This function reads 6 bytes at once over the I2C 
         * instead of 3 transactions.
         * @void
         */
            void GetRawData( int16_t* X, int16_t* Y, int16_t* Z );
        /** Get the X axis raw value of the Accelerometer
         * @return int16_t X axis value
         */
            int16_t GetXRawAcceleration( void );
        /** Get the Y axis raw value of the Accelerometer
         * @return int16_t Y axis value
         */
            int16_t GetYRawAcceleration( void );
        /** Get the Z axis raw value of the Accelerometer
         * @return int16_t Z axis value
         */
        int16_t GetZRawAcceleration( void );

        float Scaling;   /**< scaling for the device         */
        float FilterX;   /**< storage for X axis filter data */
        float FilterY;   /**< storage for Y axis filter data */
        float FilterZ;   /**< storage for Z axis filter data */
};

#endif /* HAL_ACCELEROMETER_H */
