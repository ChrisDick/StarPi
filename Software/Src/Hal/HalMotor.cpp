/*
HalMotor provides a generic interface to the motor drivers
It uses a config file to determine motor type
currently supported types:
DC motor

ToDo:
Stepper motors

Author and copyright of this file:
Chris Dick, 2016

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

#include "HalMotor.h"
#include "GPIO.h"
#include "Config.h"

HalMotor HalMotor::Motors;
lm29x_motor_t  HalMotor::Motors[CONFIG_NUMBER_OF_MOTORS]; /**< indcies of motors  */

/* Constructor
 */
HalMotor::HalMotor( void )
{
    
}

/* Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
void HalMotor::Init( void )
{
#ifdef LM29X   
    LM29x::lm29x.Init();
#if (MOTOR_ONE == MOTOR_TWO)
#error both motors cannot be set to the same driver - update config.h
#endif
#if (MOTOR_ONE == LM29X1)
    Motors[0] = LM29X_MOTOR1;
#endif
#ifdef (MOTOR_ONE == LM29x2)
    Motors[0] = LM29X_MOTOR2;
#endif
#if (MOTOR_TWO == LM29X1)
    Motors[1] = LM29X_MOTOR1;
#endif
#ifdef (MOTOR_TWO == LM29x2)
    Motors[1] = LM29X_MOTOR2;
#endif
#else
#error no motors defined
#endif    
}

/* Set motor value
 * @param Value - float, per unit demand -1 to +1
 * @param Motor - motor_index_t, Index of motor, 0 indexed.
 */
void HalMotor::SetValue( float Value, motor_index_t Motor )
{
    #ifdef LM29X
    if ( Value < 0 )
    {
        LM29x::lm29x.Direction( Motors[Motor], REVERSE )
    }
    else
    {
        LM29x::lm29x.Direction( Motors[Motor], FORWARD )
    }
    LM29x::lm29x.Value( Motors[Motor], (uint16_t)(abs( Value ) * GPIO::gpio.GetRange() ));
    #endif
}

/* Enable
 * @param Enable - bool, enable signal.
 * @param Motor - motor_index_t, Index of motor, 0 indexed.
 */
void HalMotor::Enable( bool Enable, motor_index_t Motor );
{
    LM29x::lm29x.Value( Motors[Motor], 0);
    LM29x::lm29x.Direction( Motors[Motor], OFF );
}