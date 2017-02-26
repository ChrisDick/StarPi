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
#ifndef LM29X_H
#define LM29X_H

#include <stdint.h>

typedef enum
{
    LM29X_MOTOR1,
    LM29X_MOTOR2,
    LM29X_MAX
} lm29x_motor_t;

typedef enum
{
    STOP,
    FORWARD,
    REVERSE
} direction_t;

class LM29x
{
    public:
    /** Constructor
     */
        LM29x( void );
    /** Init
     */
       void Init( void ); 
    /** Set motor value
     * @param Value - float, per unit demand -1 to +1
     * @param Motor - uint8_t, Index of motor, 0 indexed.
     */
        void SetValue( lm29x_motor_t Motor, uint16_t Value );
    /** Direction
     * @param Enable - bool, enable signal.
     * @param Motor - uint8_t, Index of motor, 0 indexed.   
     */
        void Direction( lm29x_motor_t Motor, direction_t Direction );
        
        static LM29x lm29x; /**< Pi can only handle 2 hardware PWMs, so only 2 motors. will need ot change for software PWM, bu this is not supported. */
        
    private:

};


#endif /* LM29X_H */