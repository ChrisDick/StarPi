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
LM29x LM29x::lm29x; 

/* Constructor
 */
LM29x( void )
{
            
}

/* Init
 */
void LM29x::Init( void )
{
    #ifdef LM29X1
    #ifndef LM29XIN1
    #error lm29x input 1 pin not defined
    #endif
    #ifndef LM29XIN2
    #error lm29x input 2 pin not defined
    #endif
    GPIO::gpio.SetupPWM1()
    GPIO::gpio.SetupOutput( LM29XIN1 )
    GPIO::gpio.SetupOutput( LM29XIN2 )
    #endif
    #ifdef LM29X2
    #ifndef LM29XIN3
    #error lm29x input 3 pin not defined
    #endif
    #ifndef LM29XIN4
    #error lm29x input 4 pin not defined
    #endif
    GPIO::gpio.SetupPWM2()
    GPIO::gpio.SetupOutput( LM29XIN3 )
    GPIO::gpio.SetupOutput( LM29XIN4 )
    #endif
}
    
/* Set motor value
 * @param Value - uint16_t, value of PWM range 0-1024
 * @param Motor - uint8_t, Index of motor, 0 indexed.
 */
void LM29x::SetValue( lm29x_motor_t Motor, unit16_t Value )
{
    if ( Value < gpio.GetRange() )
    {
        switch ( Motor )
        {
            case LM29X_MOTOR1:
            {
                GPIO::gpio.SetPWM1( Value );
                break;
            }
            case LM29X_MOTOR1:
            {
                GPIO::gpio.SetPWM2( Value ); 
                break;
            }
            default:
            {
                
            }
        }
    }
}

/* Direction
 * @param Enable - bool, enable signal.
 * @param Motor - uint8_t, Index of motor, 0 indexed.   
 */
void LM29x::Direction( lm29x_motor_t Motor, direction_t Direction )
{
    switch ( Motor )
    {
        case LM29X_MOTOR1:
        {
            switch ( Direction )
            {
                case FORWARD:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN1, true );
                    GPIO::gpio.GPIOSetPinState( LM29XIN2, false );
                    break;
                }
                case REVERSE:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN1, false );
                    GPIO::gpio.GPIOSetPinState( LM29XIN2, true );
                    break;
                }
                case STOP:
                default:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN1, false );
                    GPIO::gpio.GPIOSetPinState( LM29XIN2, false );
                    break;
                }
            }
            break;
        }
        case LM29X_MOTOR2:
        {
            switch ( Direction )
            {
                case FORWARD:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN3, true );
                    GPIO::gpio.GPIOSetPinState( LM29XIN4, false );
                    break;
                }
                case REVERSE:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN3, false );
                    GPIO::gpio.GPIOSetPinState( LM29XIN4, true );
                    break;
                }
                case STOP:
                default:
                {
                    GPIO::gpio.GPIOSetPinState( LM29XIN3, false );
                    GPIO::gpio.GPIOSetPinState( LM29XIN4, false );
                    break;
                }
            }
            break;
        }
        default:
        {
            
        }
    }
}

    