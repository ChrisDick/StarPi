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

#include "BME280.h"
#include "stdio.h"
/** Default constructor, uses default I2C address.
 * @see BME280_DEFAULT_ADDRESS
 */
BME280::BME280() {
    devAddr = BME280_DEFAULT_ADDRESS;
}

/** Specific address constructor.
 * @param address I2C address
 * @see BME280_DEFAULT_ADDRESS
 * @see BME280_ADDRESS_00
 */
BME280::BME280(uint8_t address) {
    devAddr = address;
}

/** Power on and prepare for general usage. This sets the full scale range of 
 * the sensor, as well as the bandwidth
 */
void BME280::initialize() 
{
  //  Reset();

    /* read out calibrations */
    I2Cdev::readBytes( devAddr, BME280_CSL_TEMP, 6, buffer );
    dig_T1 = (buffer[1] << 8) | buffer[0];
    dig_T2 = (buffer[3] << 8) | buffer[2];
    dig_T3 = (buffer[5] << 8) | buffer[4];
 
    I2Cdev::readBytes( devAddr, BME280_CSL_PRESS, 18, buffer );
    dig_P1 = (buffer[ 1] << 8) | buffer[ 0];
    dig_P2 = (buffer[ 3] << 8) | buffer[ 2];
    dig_P3 = (buffer[ 5] << 8) | buffer[ 4];
    dig_P4 = (buffer[ 7] << 8) | buffer[ 6];
    dig_P5 = (buffer[ 9] << 8) | buffer[ 8];
    dig_P6 = (buffer[11] << 8) | buffer[10];
    dig_P7 = (buffer[13] << 8) | buffer[12];
    dig_P8 = (buffer[15] << 8) | buffer[14];
    dig_P9 = (buffer[17] << 8) | buffer[16];

    I2Cdev::readByte( devAddr, BME280_CAL_HUM1, buffer );
    I2Cdev::readBytes( devAddr, BME280_CAL_HUM2, 7, &buffer[1] );
    dig_H1 = buffer[0];
    dig_H2 = (buffer[2] << 8) | buffer[1];
    dig_H3 = buffer[3];
    dig_H4 = (buffer[4] << 4) | (buffer[5] & 0x0f);
    dig_H5 = (buffer[6] << 4) | ((buffer[5]>>4) & 0x0f);
    dig_H6 = buffer[7];
    
    
    printf("H1: %d H2: %d H3: %d H4: %d H5: %d H6: %d \n\r", dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6 ); 
    printf("T1: %d T2: %d T3: %d  \n\r", dig_T1, dig_T2, dig_T3 ); 
    printf("P1: %d P2: %d P3: %d P4: %d P5: %d P6: %d P7: %d  P8: %d  P9: %d \n\r", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9 ); 
    
    
    
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool BME280::testConnection() {
    return getDeviceID() == 0x60u;
}

/** Get Device ID.
 * This register is used to verify the identity of the device (0b010).
 * @return Device ID (should be 2 dec)
 * @see BMA150_RA_CHIP_ID
 */
uint8_t BME280::getDeviceID() {
    I2Cdev::readByte(devAddr, BME280_CHIP_ID, buffer);
    return buffer[0];
}

/** Reset
 * This register performs a soft reset on the device
 * @see BMA150_RESET
 */
void BME280::Reset()
{
    buffer[0] = BME280_RESET_CMD;
    I2Cdev::writeByte(devAddr, BME280_RESET, buffer[0]);
}

/** Get the temperature measurement.
 * @return float Temperature.
 */
float BME280::getTemperature()
{
    uint32_t temp_raw;
    int32_t temp;
    float tempf;
 
    I2Cdev::readBytes(devAddr, BME280_TEMP_MSB, 3, buffer);
    temp_raw = (((uint32_t)buffer[1]) << 12) | (((uint16_t)buffer[2]) << 4) | (buffer[3] >> 4);

    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14);
 
    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;
 
    return (tempf/100.0f);
}
 
/** Get the pressure measurement.
 * @return float pressure.
 */
float BME280::getPressure()
{
    uint32_t press_raw;
    int32_t var1, var2;
    uint32_t press;
    float pressf;
 
    I2Cdev::readBytes(devAddr, BME280_PRESS_MSB, 3, buffer);
    press_raw = (((uint32_t)buffer[1]) << 12) | (((uint16_t)buffer[2]) << 4) | (buffer[3] >> 4);
 
    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 1);
    var2 = (var2 >> 2) + (dig_P4 << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * dig_P1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
    }
    var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
    press = (press + ((var1 + var2 + dig_P7) >> 4));
 
    pressf = (float)press;
    return (pressf/100.0f);
}

/** Get the humidity measurement.
 * @return float humidity.
 */
float BME280::getHumidity()
{
    uint32_t hum_raw;
    float humf;
    int32_t v_x1; 
 
    I2Cdev::readBytes(devAddr, BME280_HUM_MSB, 2, buffer);
    hum_raw = (((uint16_t)buffer[1]) << 8) | buffer[2];
 
    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
 
    humf = (float)(v_x1 >> 12);
 
    return (humf/1024.0f);
}

/** Set humidity sensor oversampling
 * @param sampling oversampling rate
 */
void BME280::setHumidityOversampling( bme280_osrs_t sampling )
{
    bme280_ctrl_hum_t hum_ctrl;
    hum_ctrl.bits.osrs_h = (uint8_t)sampling; 
    I2Cdev::writeByte(devAddr, BME280_CTRL_HUM, hum_ctrl.all );
}

/** Get humidity sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
bme280_osrs_t BME280::getHumidityOversampling( void )
{
    bme280_ctrl_hum_t hum_ctrl;
    I2Cdev::readByte(devAddr, BME280_CTRL_HUM, buffer);
    hum_ctrl.all = buffer[0];
    return (bme280_osrs_t)hum_ctrl.bits.osrs_h;
}

/** Get measurement active flag
 * @return bool measurement active
 */
bool BME280::getMeasurementActive( void )
{
    bme280_status_t status;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    status.all = buffer[0];
    return status.bits.measuring;
}

/** Get memory active flag
 * @return bool memory active
 */
bool BME280::getMemoryActive( void )
{
    bme280_status_t status;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    status.all = buffer[0];
    return status.bits.im_update;
}

/** Set the measurement mode
 * @param Mode measurement mode
 */
void BME280::setMode( bme280_mode_t Mode )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    meas_ctrl.bits.mode = (uint8_t)Mode; 
    buffer[0] = meas_ctrl.all;
    I2Cdev::writeByte(devAddr, BME280_STATUS, buffer[0]);
}

/** Get the measurement mode
 * @return bme280_mode_t measurement mode
 */
bme280_mode_t BME280::getMode( void )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    return (bme280_mode_t)meas_ctrl.bits.mode;
}

/** Set pressure sensor oversampling
 * @param sampling oversampling rate
 */
void BME280::setPressureOversampling( bme280_osrs_t sampling )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    meas_ctrl.bits.osrs_p = (uint8_t)sampling; 
    buffer[0] = meas_ctrl.all;
    I2Cdev::writeByte(devAddr, BME280_STATUS, buffer[0]);
}

/** Get pressure sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
bme280_osrs_t BME280::getPressureOversampling( void )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    return (bme280_osrs_t)meas_ctrl.bits.osrs_p;
}

/** Set temperature sensor oversampling
 * @param sampling oversampling rate
 */
void BME280::setTemperatureOversampling( bme280_osrs_t sampling )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    meas_ctrl.bits.osrs_t = (uint8_t)sampling; 
    buffer[0] = meas_ctrl.all;
    I2Cdev::writeByte(devAddr, BME280_STATUS, buffer[0]);
}

/** Get temperature sensor oversampling
 * @return bme280_osrs_t oversampling rate
 */
bme280_osrs_t BME280::getTemperatureOversampling( void )
{
    bme280_ctrl_meas_t meas_ctrl;
    I2Cdev::readByte(devAddr, BME280_STATUS, buffer);
    meas_ctrl.all = buffer[0];
    return (bme280_osrs_t) meas_ctrl.bits.osrs_t;
}

/** Set SPI enable flag
 * @param Enable 3-wire SPI interface enabled
 */
void BME280::setEnableSpi( bool Enable )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    config.bits.spi3w_en = (uint8_t)Enable; 
    buffer[0] = config.all;
    I2Cdev::writeByte(devAddr, BME280_CONFIG, buffer[0]);
}

/** Get SPI enable flag
 * @return bool 3-wire SPI interface enabled
 */
bool BME280::getSpiEnable ( void )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    return config.bits.spi3w_en;
}

/** Set filter coefficient
 * @param filter coefficient
 */
void BME280::setFilter( bme280_filter_t filter )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    config.bits.filter = (uint8_t)filter; 
    buffer[0] = config.all;
    I2Cdev::writeByte(devAddr, BME280_CONFIG, buffer[0]);
}

/** Get filter coefficient
 * @return bme280_filter_t filter coefficient
 */
bme280_filter_t BME280::getFilter( void )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    return (bme280_filter_t)config.bits.filter;
}

/** Set Standby Time
 * @param Time standby time
 */
void BME280::setStandbyTime( bme280_standby_t Time )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    config.bits.t_sb = (uint8_t)Time; 
    buffer[0] = config.all;
    I2Cdev::writeByte(devAddr, BME280_CONFIG, buffer[0]);
}

/** Get Standby Time
 * @return bme280_standby_t standby time
 */
bme280_standby_t BME280::getStandbyTime( void )
{
    bme280_config_t config;
    I2Cdev::readByte(devAddr, BME280_CONFIG, buffer);
    config.all = buffer[0];
    return (bme280_standby_t)config.bits.t_sb;
}
