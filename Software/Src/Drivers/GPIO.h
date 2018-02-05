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
#ifndef GPIO_H
#define GPIO_H
#include "Config.h"
#include <stdint.h>
typedef enum
{
/*
    WiringPi numbering
*/
    WIRINGPIN00 = 0,
    WIRINGPIN01 = 1,
    WIRINGPIN02 = 2,
    WIRINGPIN03 = 3,
    WIRINGPIN04 = 4,
    WIRINGPIN05 = 5,
    WIRINGPIN06 = 6,
    WIRINGPIN07 = 7,
//#ifdef REV1
//#ifndef I2C0
//#else
//#ifndef I2C1
//#endif
    WIRINGPIN08 = 8,
    WIRINGPIN09 = 9,
//#endif
//#ifndef CS0
    WIRINGPIN10 = 10,
//#endif
//#ifndef CS1
    WIRINGPIN11 = 11,
//#endif
//#ifndef SPI
    WIRINGPIN12 = 12,
    WIRINGPIN13 = 13,
    WIRINGPIN14 = 14,
//#endif
//#ifndef SERIAL
    WIRINGPIN15 = 15,
    WIRINGPIN16 = 16,
//#endif
//#ifdef REV1
//#ifndef I2C1
    WIRINGPIN17 = 17,
    WIRINGPIN18 = 18,
//#endif
    WIRINGPIN19 = 19,
    WIRINGPIN20 = 20,
//#endif
    WIRINGPIN21 = 21,
    WIRINGPIN22 = 22,
    WIRINGPIN23 = 23,
//#ifndef PWM1
    WIRINGPIN24 = 24,
//#endif
//#ifndef PWM2
    WIRINGPIN25 = 25,
//#endif
    WIRINGPIN26 = 26,
    WIRINGPIN27 = 27,
    WIRINGPIN28 = 28,
    WIRINGPIN29 = 29,
//#ifndef REV1
//#ifndef I2C0
    WIRINGPIN30 = 30,
    WIRINGPIN31 = 31,
//#endif
//#endif
    PIN_MAX = 32,
} pin_name_t;
/*
    BCM GPIO to WiringPi numbering
*/
//#ifdef REV1
//#ifndef I2C0
//#else
//#ifndef I2C1
//#endif
#define BCM_GPIO00  WIRINGPIN08
#define BCM_GPIO01  WIRINGPIN09
//#else
#define BCM_GPIO02  WIRINGPIN08
#define BCM_GPIO03  WIRINGPIN09
//#endif
//#endif
#define BCM_GPIO04  WIRINGPIN07
#define BCM_GPIO05  WIRINGPIN21
#define BCM_GPIO06  WIRINGPIN22
//#ifndef CS1
#define BCM_GPIO07  WIRINGPIN11
//#endif
//#ifndef CS0
#define BCM_GPIO08  WIRINGPIN10
//#endif
//#ifndef SPI
#define BCM_GPIO09  WIRINGPIN13
#define BCM_GPIO10  WIRINGPIN12
#define BCM_GPIO11  WIRINGPIN14
//#endif
#define BCM_GPIO12  WIRINGPIN26
#define BCM_GPIO13  WIRINGPIN23
//#ifndef SERIAL
#define BCM_GPIO14  WIRINGPIN14
#define BCM_GPIO15  WIRINGPIN15
//#endif
#define BCM_GPIO16  WIRINGPIN27
#define BCM_GPIO17  WIRINGPIN00
#define BCM_GPIO18  WIRINGPIN01
//#ifndef PWM1
#define BCM_GPIO19  WIRINGPIN24
//#endif
#define BCM_GPIO20  WIRINGPIN28
#define BCM_GPIO21  WIRINGPIN02
#define BCM_GPIO22  WIRINGPIN03
#define BCM_GPIO23  WIRINGPIN04
#define BCM_GPIO24  WIRINGPIN05
#define BCM_GPIO25  WIRINGPIN06
//#ifndef PWM2
#define BCM_GPIO26  WIRINGPIN25
//#endif
//#ifdef REV1
#define BCM_GPIO27  WIRINGPIN02
//#else
//    BCM_GPIO27 = WIRINGPIN29,
//#endif
//#ifndef I2C1
#define BCM_GPIO28  WIRINGPIN17
#define BCM_GPIO29  WIRINGPIN18
//#endif
#define BCM_GPIO30  WIRINGPIN19
#define BCM_GPIO31  WIRINGPIN20
/*
    Header position to WiringPi numbering
*/
//#ifdef REV1
//#ifndef I2C0
//#else
//#ifndef I2C1
//#endif
#define PIN_1_03  WIRINGPIN08 
#define PIN_1_05  WIRINGPIN09 
//#endif
#define PIN_1_07  WIRINGPIN07 
//#ifndef SERIAL
#define PIN_1_08  WIRINGPIN15 
#define PIN_1_10  WIRINGPIN16 
//#endif
#define PIN_1_11  WIRINGPIN00 
#define PIN_1_12  WIRINGPIN18 
#define PIN_1_13  WIRINGPIN02 
#define PIN_1_15  WIRINGPIN03 
#define PIN_1_16  WIRINGPIN04 
#define PIN_1_18  WIRINGPIN05
//#ifndef SPI    
#define PIN_1_19  WIRINGPIN12 
#define PIN_1_21  WIRINGPIN13
//#endif
#define PIN_1_22  WIRINGPIN06 
//#ifndef SPI
#define PIN_1_23  WIRINGPIN14
//#endif
//#ifndef CS0
#define PIN_1_24  WIRINGPIN10 
//#endif
//#ifndef CS1
#define PIN_1_26  WIRINGPIN11 
//#endif
//#ifndef REV1
//#ifndef I2C0
#define PIN_1_27  WIRINGPIN30 
#define PIN_1_28  WIRINGPIN31 
//#endif
#define PIN_1_29  WIRINGPIN21 
#define PIN_1_31  WIRINGPIN22 
#define PIN_1_32  WIRINGPIN26 
#define PIN_1_33  WIRINGPIN23 
//#ifndef PWM1
#define PIN_1_35  WIRINGPIN24 
//#endif
#define PIN_1_36  WIRINGPIN27 
//#ifndef PWM2
#define PIN_1_37  WIRINGPIN25 
//#endif
#define PIN_1_38  WIRINGPIN28 
#define PIN_1_40  WIRINGPIN29 
//#else
//#ifndef I2C1
#define PIN2_03  WIRINGPIN17 
#define PIN2_04  WIRINGPIN18 
//#endif
#define PIN2_05  WIRINGPIN19 
#define PIN2_06  WIRINGPIN20  
//#endif
//#endif

/** pin_pull_t - gpio pin pull up/down configuration 
 */
typedef enum
{
    PULL_OFF,
    PULL_DOWN,
    PULL_UP 
} pin_pull_t;

typedef enum
{
    MARK_SPACE,
    BALANCED
} pwm_mode_t;

/** GPIO
 * - Class to provide access to GPIO
 */
class GPIO
{
    public:
    /** Constructor
     */
        GPIO( void );

    /** Initialise the GPIO to all inputs
     * @return bool Initialisation status
     */
        void Init( void );
    /** Setup PWM 1
     */
        void SetupPWM1( void );
    /** Setup PWM 2
     */
        void SetupPWM2( void );
    /** Setup Output
     * @param PinName
     */
        void SetupOutput( pin_name_t PinName );
    /** Setup pull up/down mode
     * @param PinName
     */
        void SetPullMode( pin_name_t PinName, pin_pull_t PullMode );
    /** set PWM mode
     * @param Mode mode of PWM, balanced or mark space mode.
     */
        void SetMode( pwm_mode_t Mode );
    /** set PWM Range
     * @param Range uin16_t range of the pwm.
     */
        void SetRange( uint16_t Range );
    /** Get Range
     * @return uint16_t Range of the PWM
     */
        uint16_t GetRange( void );
    /** set PWM Clock
     * @param Divisor uint16_t divider value for the PWM clock scaling
     */
        void SetClock( uint16_t Divisor );
    /** Set PWM 1
     * @param Value uint16_t sets the percentage on of PWM
     */
        void SetPWM1( uint16_t Value );
    /** Set PWM 1
     * @param Value uint16_t sets the percentage on of PWM
     */
        void SetPWM2( uint16_t Value );
    /** Get the state of an GPIO pin
     * @param PinName name of the pin to get
     * @return bool state of pin
     */
        bool GetPinState( pin_name_t PinName );
    /** Set the state of an output pin
     * @param PinName name of the pin to set
     */
        void SetPinState( pin_name_t PinName, bool state );

        static GPIO gpio;

    private:
        uint16_t PWMRange; /**< range of the PWM */
};

#endif /* GPIO_H */
