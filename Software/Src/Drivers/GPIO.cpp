/*
GPIO provides a generic interface to the GPIO
It uses a config file to check if the pins are availiable

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

#include "GPIO.h"
#include "Config.h"
#include "Wiring.h"

GPIO GPIO::gpio;

/* Constructor
 */
GPIO::GPIO( void )
{
}

/* Initialise the Accelerometer
 * @return Status initialisation (true = success)
 */
void GPIO::Init( void )
{
    uint8_t index = 0;
    wiringPiSetup (void);
    for (index = 0; index < NUMBER_OF_GPIO; index++)
    {
        pinMode ( index, INPUT );
    }
}

/* Setup Output
 * @param PinName
 */
void GPIO::SetupOutput( pin_name_t PinName )
{
    pinMode ( PinName, OUTPUT );
}

/* Setup pull up/down mode
 * @param PinName
 */
void GPIO::SetPullMode( pin_name_t PinName, pin_pull_t PullMode )
{
    switch ( PullMode )
    {
        case PULL_UP:
        {
            pullUpDnControl ( PinName, PUD_UP  );
            break;
        }
        case PULL_DOWN:
        {
            pullUpDnControl ( PinName, PUD_DOWN );
            break;
        }
        case PULL_OFF:
        default:
        {
            pullUpDnControl ( PinName, PUD_OFF );
            break;
        }
    }
}

/* Setup PWM 1
 */
void GPIO::SetupPWM1( void )
{
    pinMode ( WIRINGPIN24, PWM_OUTPUT );
}

/* Setup PWM 2
 */
void GPIO::SetupPWM2( void )
{
    pinMode ( WIRINGPIN25, PWM_OUTPUT );
}

/* set PWM mode
 */
void GPIO::SetMode( pwm_mode_t Mode )
{
    switch ( Mode )
    {
        case BALANCED:
        {
            pwmSetMode ( PWM_MODE_BAL  );
            break;
        }
        case MARK_SPACE:
        default:
        {
            pwmSetMode ( PWM_MODE_MS  );
            break;
        }
        
}

/* set PWM range
 */
void GPIO::SetRange( uint16_t Range )
{
    PWMRange = Range;
    pwmSetRange ( Range );
}

/* Get Range
 * @return uint16_t Range of the PWM
 */
uint16_t GPIO::GetRange( void )
{
    return Range;
}

/* set PWM Clock
 */
void GPIO::SetClock( uint16_t Divisor )
{
    pwmSetClock ( Divisor );
}

/* Set PWM 1
 */
void GPIO::SetPWM1( uint16_t Value )
{
    pwmWrite ( WIRINGPIN24, Value ) ;
}

/* Set PWM 2
 * @param Value PWM value range 0-1024
 */
void GPIO::SetPWM2( uint16_t Value )
{
    pwmWrite ( WIRINGPIN25, Value ) ;
}

/* set one of the GPIO pins
 */
bool GPIO::SetPin( pin_name_t PinName, bool State )
{
    digitalWrite ( PinName, State ) ;
}

/* set one of the GPIO pins
 */
bool GPIO::GetPin( pin_name_t PinName )
{
    return digitalRead ( PinName );
}
