/**
GPIO provides an interface to retrive the time and location from 
the GPSD deamon.

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
#ifndef HALMOTOR_H
#define HALMOTOR_H

#include "config.h"

typedef enum
{
    MOTOR1,
    MOTOR2,
    MAX_MOTORS
} motor_index_t;

class HalMotor
{
    public:
    /** Constructor
     */
        Halmotor( void );
    /** Init
     */
       void Init( void ); 
    /** Set motor value
     * @param Value - float, per unit demand -1 to +1
     * @param Motor - uint8_t, Index of motor, 0 indexed.
     */
        void SetValue( float Value, motor_index_t Motor );
    /** Enable
     * @param Enable - bool, enable signal.
     * @param Motor - uint8_t, Index of motor, 0 indexed.
     */
        void Enable( bool Enable, motor_index_t Motor );
        
        static HalMotor Motor;
        
    private:
    #ifdef LM29X
        lm29x_motor_t Motors[CONFIG_NUMBER_OF_MOTORS];
    #endif
};


#endif