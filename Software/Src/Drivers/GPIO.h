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
    WIRINGPIN0,
    WIRINGPIN1,
    WIRINGPIN2,
    WIRINGPIN3,
    WIRINGPIN4,
    WIRINGPIN5,
    WIRINGPIN6,
    WIRINGPIN7,
#ifdef REV1
#ifndef I2C0
#else
#ifndef I2C1
#endif
    WIRINGPIN8,
    WIRINGPIN9,
#endif
#ifndef CS0
    WIRINGPIN10,
#endif
#ifndef CS1
    WIRINGPIN11,
#endif
#ifndef SPI
    WIRINGPIN12,
    WIRINGPIN13,
    WIRINGPIN14,
#endif
#ifndef SERIAL
    WIRINGPIN15,
    WIRINGPIN16,
#endif
#ifdef REV1
#ifndef I2C1
    WIRINGPIN17,
    WIRINGPIN18,
#endif
    WIRINGPIN19,
    WIRINGPIN20,
#endif
    WIRINGPIN21,
    WIRINGPIN22,
    WIRINGPIN23,
#ifndef PWM1
    WIRINGPIN24,
#endif
#ifndef PWM2
    WIRINGPIN25,
#endif
    WIRINGPIN26,
    WIRINGPIN27,
    WIRINGPIN28,
    WIRINGPIN29,
#ifndef REV1
#ifndef I2C0
    WIRINGPIN30,
    WIRINGPIN31,
#endif
#endif
    PIN_MAX,
/*
    BCM GPIO to WiringPi numbering
*/
#ifdef REV1
#ifndef I2C0
#else
#ifndef I2C1
#endif
    BCM_GPIO00 = WIRINGPIN8,
    BCM_GPIO01 = WIRINGPIN9,
#else
    BCM_GPIO02 = WIRINGPIN8,
    BCM_GPIO03 = WIRINGPIN9,
#endif
#endif
    BCM_GPIO04 = WIRINGPIN7,
    BCM_GPIO05 = WIRINGPIN21,
    BCM_GPIO06 = WIRINGPIN22,
#ifndef CS1
    BCM_GPIO07 = WIRINGPIN11,
#endif
#ifndef CS0
    BCM_GPIO08 = WIRINGPIN10,
#endif
#ifndef SPI
    BCM_GPIO09 = WIRINGPIN13,
    BCM_GPIO10 = WIRINGPIN12,
    BCM_GPIO11 = WIRINGPIN14,
#endif
    BCM_GPIO12 = WIRINGPIN26,
    BCM_GPIO13 = WIRINGPIN23,
#ifndef SERIAL
    BCM_GPIO14 = WIRINGPIN14,
    BCM_GPIO15 = WIRINGPIN15,
#endif
    BCM_GPIO16 = WIRINGPIN27,
    BCM_GPIO17 = WIRINGPIN00,
    BCM_GPIO18 = WIRINGPIN01,
#ifndef PWM1
    BCM_GPIO19 = WIRINGPIN24,
#endif
    BCM_GPIO20 = WIRINGPIN28,
    BCM_GPIO21 = WIRINGPIN02,
    BCM_GPIO22 = WIRINGPIN03,
    BCM_GPIO23 = WIRINGPIN04,
    BCM_GPIO24 = WIRINGPIN05,
    BCM_GPIO25 = WIRINGPIN06,
#ifndef PWM2
    BCM_GPIO26 = WIRINGPIN25,
#endif
#ifdef REV1
    BCM_GPIO27 = WIRINGPIN02,
#else
    BCM_GPIO27 = WIRINGPIN29,
#endif
#ifndef I2C1
    BCM_GPIO28 = WIRINGPIN17,
    BCM_GPIO29 = WIRINGPIN18,
#endif
    BCM_GPIO30 = WIRINGPIN19,
    BCM_GPIO31 = WIRINGPIN20,
/*
    Header position to WiringPi numbering
*/
#ifdef REV1
#ifndef I2C0
#else
#ifndef I2C1
#endif
    PIN_1_03 = WIRINGPIN08, 
    PIN_1_05 = WIRINGPIN09, 
#endif
    PIN_1_07 = WIRINGPIN07, 
#ifndef SERIAL
    PIN_1_08 = WIRINGPIN15, 
    PIN_1_10 = WIRINGPIN16, 
#endif
    PIN_1_11 = WIRINGPIN00, 
    PIN_1_12 = WIRINGPIN18, 
    PIN_1_13 = WIRINGPIN02, 
    PIN_1_15 = WIRINGPIN03, 
    PIN_1_16 = WIRINGPIN04, 
    PIN_1_18 = WIRINGPIN05,
#ifndef SPI    
    PIN_1_19 = WIRINGPIN12, 
    PIN_1_21 = WIRINGPIN13,
#endif
    PIN_1_22 = WIRINGPIN06, 
#ifndef SPI
    PIN_1_23 = WIRINGPIN14, 
#endif
#ifndef CS0
    PIN_1_24 = WIRINGPIN10, 
#endif
#ifndef CS1
    PIN_1_26 = WIRINGPIN11, 
#endif
#ifndef REV1
#ifndef I2C0
    PIN_1_27 = WIRINGPIN30, 
    PIN_1_28 = WIRINGPIN31, 
#endif
    PIN_1_29 = WIRINGPIN21, 
    PIN_1_31 = WIRINGPIN22, 
    PIN_1_32 = WIRINGPIN26, 
    PIN_1_33 = WIRINGPIN23, 
#ifndef PWM1
    PIN_1_35 = WIRINGPIN24, 
#endif
    PIN_1_36 = WIRINGPIN27, 
#ifndef PWM2
    PIN_1_37 = WIRINGPIN25, 
#endif
    PIN_1_38 = WIRINGPIN28, 
    PIN_1_40 = WIRINGPIN29, 
#else
#ifndef I2C1
    PIN2_03 = WIRINGPIN17, 
    PIN2_04 = WIRINGPIN18, 
#endif
    PIN2_05 = WIRINGPIN19, 
    PIN2_06 = WIRINGPIN20,  
#endif
#endif
} pin_name_t;
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
