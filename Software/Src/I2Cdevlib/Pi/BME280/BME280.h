// I2Cdev library collection - BMA150 I2C device class header file
// Based on BMA150 datasheet, 29/05/2008
// 01/18/2012 by Brian McCain <bpmccain@gmail.com>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-01-18 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef BMA280_H
#define BMA280_H

#include "I2Cdev.h"

#define BME280_DEFAULT_ADDRESS           (0x76)  //?? or just 0x76 << 1

#define BME280_CHIP_ID                   0xD0u
#define BME280_RESET                     0xE0u
#define BME280_RESET_CMD                 0xB6u

#define BME280_CAL_HUM1                  0xA1u
#define BME280_CAL_HUM2                  0xE1u
#define BME280_CSL_TEMP                  0x88u
#define BME280_CSL_PRESS                 0x8Eu


#define BME280_CTRL_HUM                  0xF2u
/* Changes to this register only become effective after a write operation to BME280_CTRL_MEAS.*/

union bme280_ctrl_hum_t
{
    uint8_t all;
    
    struct 
    {
        uint8_t  osrs_h  :3u;
        uint8_t  unsed   :5u;
    } bits;
};

#define BME280_STATUS                    0xF3u
union bme280_status_t
{
    uint8_t all;
    
    struct 
    {
        uint8_t measuring :1u;
        uint8_t unused    :1u;
        uint8_t im_update :1u;
        uint8_t unused2   :5u;
    }bits;
};

#define BME280_CTRL_MEAS                 0xF4u
union bme280_ctrl_meas_t
{
    uint8_t all;
    
    struct 
    {
        uint8_t mode      :2u;
        uint8_t osrs_p    :3u;
        uint8_t osrs_t    :3u;
    }bits;
};

typedef enum 
{
    SLEEP_MODE,
    FORCED_MODE,
    FORCED_MODE2,
    NORMAL_MODE,
} bme280_mode_t;

#define BME280_CONFIG                    0xF5u
union bme280_config_t
{
    uint8_t all;
    
    struct 
    {
        uint8_t spi3w_en   :1u;
        uint8_t unused     :1u;
        uint8_t filter     :3u;
        uint8_t t_sb       :3u;
    }bits;
};

typedef enum{
    FILTER_OFF,
    FILTER_X2,
    FILTER_X4,
    FILTER_X8,
    FILTER_X16
} bme280_filter_t;

typedef enum
{
    TIME_0_5,
    TIME_62_5,
    TIME_125,
    TIME_250,
    TIME_500,
    TIME_1000,
    TIME_10,
    TIME_20
} bme280_standby_t;

#define BME280_PRESS_MSB                 0xF7u
#define BME280_PRESS_LSB                 0xF8u
#define BME280_PRESS_XLSB                0xF9u

#define BME280_TEMP_MSB                  0xFAu
#define BME280_TEMP_LSB                  0xFBu
#define BME280_TEMP_XLSB                 0xFCu

#define BME280_HUM_MSB                   0xFDu
#define BME280_HUM_LSB                   0xFEu

typedef enum
{
    SKIPPED,
    OVERSAMPLING_X1,
    OVERSAMPLING_X2,
    OVERSAMPLING_X4,
    OVERSAMPLING_X8,
    OVERSAMPLING_X16
} bme280_osrs_t;

/** Class Driver for the BME280
 */
class BME280 {
    public:
/** Default constructor, uses default I2C address.
 * @see BMA150_DEFAULT_ADDRESS
 */
        BME280();
/** Specific address constructor.
 * @param address I2C address
 * @see BMA150_DEFAULT_ADDRESS
 * @see BMA150_ADDRESS_00
 */
        BME280(uint8_t address);
/** Power on and prepare for general usage. This sets the full scale range of 
 * the sensor, as well as the bandwidth
 */
        void initialize();
/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
        bool testConnection();
/** Get Device ID.
 * This register is used to verify the identity of the device (0b010).
 * @return Device ID (should be 2 dec)
 * @see BMA150_RA_CHIP_ID
 */
        uint8_t getDeviceID();
/** Reset
 * This register performs a soft reset on the device
 * @see BMA150_RESET
 */
        void Reset();
/** Get the temperature measurement.
 * @return float Temperature.
 */
        float getTemperature();
/** Get the pressure measurement.
 * @return float pressure.
 */
        float getPressure();
/** Get the humidity measurement.
 * @return float humidity.
 */
        float getHumidity();
/** Set humidity sensor oversampling
 * @param sampling oversampling rate
 */
        void setHumidityOversampling( bme280_osrs_t sampling );
/** Get humidity sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
        bme280_osrs_t getHumidityOversampling( void );
/** Get measurement active flag
 * @return bool measurement active
 */
        bool getMeasurementActive( void );
/** Get memory active flag
 * @return bool memory active
 */
        bool getMemoryActive( void );
/** Set the measurement mode
 * @param Mode measurement mode
 */
        void setMode( bme280_mode_t Mode );
/** Get the measurement mode
 * @return bme280_mode_t measurement mode
 */
        bme280_mode_t getMode( void );
/** Set pressure sensor oversampling
 * @param sampling oversampling rate
 */
        void setPressureOversampling( bme280_osrs_t sampling );
/** Get pressure sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
        bme280_osrs_t getPressureOversampling( void );
/** Set temperature sensor oversampling
 * @param sampling oversampling rate
 */
        void setTemperatureOversampling( bme280_osrs_t sampling );
/** Get temperature sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
        bme280_osrs_t getTemperatureOversampling( void );
/** Set SPI enable flag
 * @param Enable 3-wire SPI interface enabled
 */
        void setEnableSpi( bool Enable );
/** Get SPI enable flag
 * @return bool 3-wire SPI interface enabled
 */
        bool getSpiEnable ( void );
/** Set filter coefficient
 * @param filter coefficient
 */
        void setFilter( bme280_filter_t filter );
/** Get filter coefficient
 * @return bme280_filter_t filter coefficient
 */
        bme280_filter_t getFilter( void );
/** Set Standby Time
 * @param Time standby time
 */
        void setStandbyTime ( bme280_standby_t Time );
/** Get Standby Time
 * @return bme280_standby_t standby time
 */
        bme280_standby_t getStandbyTime( void );
     
    private:
        uint8_t devAddr;   /**< Address of the device */
        uint8_t buffer[20]; /**< data buffer*/
        uint16_t    dig_T1;
        int16_t     dig_T2;
        int16_t     dig_T3;
        uint16_t    dig_P1;
        int16_t     dig_P2;
        int16_t     dig_P3;
        int16_t     dig_P4;
        int16_t     dig_P5;
        int16_t     dig_P6;
        int16_t     dig_P7;
        int16_t     dig_P8;
        int16_t     dig_P9;
        uint16_t    dig_H1;
        uint16_t    dig_H3;
        int16_t     dig_H2;
        int16_t     dig_H4;
        int16_t     dig_H5;
        int16_t     dig_H6;
        int32_t     t_fine;
};

#endif /* _BMA150_H_ */