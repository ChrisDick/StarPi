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

#ifndef _BMA150_H_
#define _BMA150_H_

#include "I2Cdev.h"

#define BMA150_ADDRESS_00           0xA1                // Default Address
#define BMA150_ADDRESS_01           0x38                // Used on the Atmel ATAVRSBIN1
#define BMA150_DEFAULT_ADDRESS      BMA150_ADDRESS_01

#define BMA150_RA_CHIP_ID           0x00
#define BMA150_RA_VERSION           0x01
#define BMA150_RA_X_AXIS_LSB        0x02
#define BMA150_RA_X_AXIS_MSB        0x03
#define BMA150_RA_Y_AXIS_LSB        0x04
#define BMA150_RA_Y_AXIS_MSB        0x05
#define BMA150_RA_Z_AXIS_LSB        0x06
#define BMA150_RA_Z_AXIS_MSB        0x07
#define BMA150_RA_TEMP_RD           0x08
#define BMA150_RA_SMB150_STATUS     0x09
#define BMA150_RA_SMB150_CTRL       0x0a
#define BMA150_RA_SMB150_CONF1      0x0b
#define BMA150_RA_LG_THRESHOLD      0x0c
#define BMA150_RA_LG_DURATION       0x0d
#define BMA150_RA_HG_THRESHOLD      0x0e
#define BMA150_RA_HG_DURATION       0x0f
#define BMA150_RA_MOTION_THRS       0x10
#define BMA150_RA_HYSTERESIS        0x11
#define BMA150_RA_CUSTOMER1         0x12
#define BMA150_RA_CUSTOMER2         0x13
#define BMA150_RA_RANGE_BWIDTH      0x14
#define BMA150_RA_SMB150_CONF2      0x15
#define BMA150_RA_OFFS_GAIN_X       0x16
#define BMA150_RA_OFFS_GAIN_Y       0x17
#define BMA150_RA_OFFS_GAIN_Z       0x18
#define BMA150_RA_OFFS_GAIN_T       0x19
#define BMA150_RA_OFFSET_X          0x1a
#define BMA150_RA_OFFSET_Y          0x1b
#define BMA150_RA_OFFSET_Z          0x1c
#define BMA150_RA_OFFSET_T          0x1d

#define BMA150_CHIP_ID_BIT             2
#define BMA150_CHIP_ID_LENGTH          3

#define BMA150_X_AXIS_LSB_BIT          7
#define BMA150_X_AXIS_LSB_LENGTH       2
#define BMA150_X_NEW_DATA_BIT          0

#define BMA150_Y_AXIS_LSB_BIT          7
#define BMA150_Y_AXIS_LSB_LENGTH       2
#define BMA150_Y_NEW_DATA_BIT          0

#define BMA150_Z_AXIS_LSB_BIT          7
#define BMA150_Z_AXIS_LSB_LENGTH       2
#define BMA150_Z_NEW_DATA_BIT          0

#define BMA150_STATUS_HG_BIT           0
#define BMA150_STATUS_LG_BIT           1
#define BMA150_HG_LATCHED_BIT          2
#define BMA150_LG_LATCHED_BIT          3
#define BMA150_ALERT_PHASE_BIT         4
#define BMA150_ST_RESULT_BIT           7

#define BMA150_SLEEP_BIT               0
#define BMA150_SOFT_RESET_BIT          1
#define BMA150_ST0_BIT                 2
#define BMA150_ST1_BIT                 3
#define BMA150_EEW_BIT                 4
#define BMA150_UPDATE_IMAGE_BIT        5
#define BMA150_RESET_INT_BIT           6

#define BMA150_ENABLE_LG_BIT           0
#define BMA150_ENABLE_HG_BIT           1
#define BMA150_COUNTER_LG_BIT          3
#define BMA150_COUNTER_LG_LENGTH       2
#define BMA150_COUNTER_HG_BIT          5
#define BMA150_COUNTER_HG_LENGTH       2
#define BMA150_ANY_MOTION_BIT          6
#define BMA150_ALERT_BIT               7

#define BMA150_LG_HYST_BIT             2
#define BMA150_LG_HYST_LENGTH          3
#define BMA150_HG_HYST_BIT             5
#define BMA150_HG_HYST_LENGTH          3
#define BMA150_ANY_MOTION_DUR_BIT      7
#define BMA150_ANY_MOTION_DUR_LENGTH   2

#define BMA150_BANDWIDTH_BIT           2
#define BMA150_BANDWIDTH_LENGTH        3
#define BMA150_RANGE_BIT               4
#define BMA150_RANGE_LENGTH            2

#define BMA150_WAKE_UP_BIT             0
#define BMA150_WAKE_UP_PAUSE_BIT       2
#define BMA150_WAKE_UP_PAUSE_LENGTH    2
#define BMA150_SHADOW_DIS_BIT          3
#define BMA150_LATCH_INT_BIT           4
#define BMA150_NEW_DATA_INT_BIT        5
#define BMA150_ENABLE_ADV_INT_BIT      6
#define BMA150_SPI4_BIT                7

/* range and bandwidth */
#define BMA150_RANGE_2G                0
#define BMA150_RANGE_4G                1
#define BMA150_RANGE_8G                2

#define BMA150_BW_25HZ                 0
#define BMA150_BW_50HZ                 1
#define BMA150_BW_100HZ                2
#define BMA150_BW_190HZ                3
#define BMA150_BW_375HZ                4
#define BMA150_BW_750HZ                5
#define BMA150_BW_1500HZ               6

/* mode settings */
#define BMA150_MODE_NORMAL             0
#define BMA150_MODE_SLEEP              1
/** Class Dirver for the Accelerometer
 */
class BMA150 {
    public:
/** Default constructor, uses default I2C address.
 * @see BMA150_DEFAULT_ADDRESS
 */
        BMA150();
/** Specific address constructor.
 * @param address I2C address
 * @see BMA150_DEFAULT_ADDRESS
 * @see BMA150_ADDRESS_00
 */
        BMA150(uint8_t address);
        
/** Power on and prepare for general usage. This sets the full scale range of 
 * the sensor, as well as the bandwidth
 */
        void initialize();
/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
        bool testConnection();

        // CHIP_ID register
/** Get Device ID.
 * This register is used to verify the identity of the device (0b010).
 * @return Device ID (should be 2 dec)
 * @see BMA150_RA_CHIP_ID
 */
        uint8_t getDeviceID();
        
        // VERSION register
/** Get Chip Revision number
 * @return Chip Revision
 * @see BMA150_RA_VERSION
 */
        uint8_t getChipRevision();
        
        // AXIS registers
/** Get 3-axis accelerometer readings.
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 * @see BMA150_RA_Y_AXIS_LSB
 */
        void getAcceleration(int16_t* x, int16_t* y, int16_t* z);
/** Get X-axis accelerometer reading.
 * @return X-axis acceleration measurement in 16-bit 2's complement format
 * @see BMA150_RA_X_AXIS_LSB
 */
        int16_t getAccelerationX();
/** Get Y-axis accelerometer reading.
 * @return Y-axis acceleration measurement in 16-bit 2's complement format
 * @see BMA150_RA_Y_AXIS_LSB
 */
        int16_t getAccelerationY();
/** Get Z-axis accelerometer reading.
 * @return Z-axis acceleration measurement in 16-bit 2's complement format
 * @see BMA150_RA_Z_AXIS_LSB
 */
        int16_t getAccelerationZ();
/** Check for new X axis acceleration data.
 * @return New X-Axis Data Status
 * @see BMA150_RA_X_AXIS_LSB
 */
        bool newDataX();
/** Check for new Y axis acceleration data.
 * @return New Y-Axis Data Status
 * @see BMA150_RA_Y_AXIS_LSB
 */
        bool newDataY();
/** Check for new Z axis acceleration data.
 * @return New Z-Axis Data Status
 * @see BMA150_RA_Z_AXIS_LSB
 */
        bool newDataZ();
                
        // TEMP register
/** Check for current temperature
 * @return Current Temperature in 0.5C increments from -30C at 00h
 * @see BMA150_RA_TEMP_RD
 */
        int8_t getTemperature();
        
        // SMB150 registers
/** Documentation ToDo
 */ 
        bool getStatusHG();
/** Documentation ToDo
 */ 
        bool getStatusLG();
/** Documentation ToDo
 */ 
        bool getHGLatched();
/** Documentation ToDo
 */ 
        bool getLGLatched();
/** Documentation ToDo
 */ 
        bool getAlertPhase();
/** Documentation ToDo
 */ 
        bool getSTResult();
/** Documentation ToDo
 * @param status_hg
 */ 
        void setStatusHG(bool status_hg);
/** Documentation ToDo
 * @param status_lg
 */ 
        void setStatusLG(bool status_lg);
/** Documentation ToDo
 * @param hg_latched
 */ 
        void setHGLatched(bool hg_latched);
/** Documentation ToDo
 * @param lg_latched
 */ 
        void setLGLatched(bool lg_latched);
/** Documentation ToDo
 * @param alert_phase
 */ 
        void setAlertPhase(bool alert_phase);
/** Documentation ToDo
 * @param st_result
 */ 
        void setSTResult(bool st_result);
        
/** Documentation ToDo
 */ 
        bool getSleep();
/** Documentation ToDo
 */ 
        bool getSoftReset();
/** Documentation ToDo
 */ 
        bool getSelfTest0();
/** Documentation ToDo
 */ 
        bool getSelfTest1();
/** Documentation ToDo
 */ 
        bool getEEW();
/** Documentation ToDo
 */ 
        bool getUpdateImage();
/** Documentation ToDo
 */ 
        bool getResetINT();
/** Documentation ToDo
 * @param sleep
 */ 
        void setSleep(bool sleep);
/** Documentation ToDo
 * @param soft_reset
 */ 
        void setSoftReset(bool soft_reset);
/** Documentation ToDo
 * @param st0
 */ 
        void setSelfTest0(bool st0);
/** Documentation ToDo
 * @param st1
 */ 
        void setSelfTest1(bool st1);
/** Documentation ToDo
 * @param eew
 */ 
        void setEEW(bool eew);
/** Documentation ToDo
 * @param update_image
 */ 
        void setUpdateImage(bool update_image);
/** Documentation ToDo
 * @param reset_int
 */ 
        void setResetINT(bool reset_int);
        
/** Documentation ToDo
 */ 
        bool getEnableLG();
/** Documentation ToDo
 */ 
        bool getEnableHG();
/** Documentation ToDo
 */ 
        int8_t getCounterLG();
/** Documentation ToDo
 */ 
        int8_t getCounterHG();
/** Documentation ToDo
 */ 
        bool getAnyMotion();
/** Documentation ToDo
 */ 
        bool getAlert();
/** Documentation ToDo
 * @param enable_lg
 */ 
        void setEnableLG(bool enable_lg);
/** Documentation ToDo
 * @param enable_hg
 */ 
        void setEnableHG(bool enable_hg);
/** Documentation ToDo
 * @param counter_lg
 */ 
        void setCounterLG(int8_t counter_lg);
/** Documentation ToDo
 * @param counter_hg
 */ 
        void setCounterHG(int8_t counter_hg);
/** Documentation ToDo
 * @param any_motion
 */ 
        void setAnyMotion(bool any_motion);
/** Documentation ToDo
 * @param alert
 */ 
        void setAlert(bool alert);
        
/** Documentation ToDo
 */ 
        bool getWakeUp();
/** Documentation ToDo
 */ 
        int8_t getWakeUpPause();
/** Documentation ToDo
 */ 
        bool getShadowDis();
/** Documentation ToDo
 */ 
        bool getLatchInt();
/** Documentation ToDo
 */ 
        bool getNewDataInt();
/** Documentation ToDo
 */ 
        bool getEnableAdvInt();
/** Documentation ToDo
 */ 
        bool getSPI4();
/** Documentation ToDo
 * @param wake_up
 */ 
        void setWakeUp(bool wake_up);
/** Documentation ToDo
 * @param wake_up_pause
 */ 
        void setWakeUpPause(int8_t wake_up_pause);
/** Documentation ToDo
 * @param shadow_dis
 */ 
        void setShadowDis(bool shadow_dis);
/** Documentation ToDo
 * @param latch_int
 */ 
        void setLatchInt(bool latch_int);
/** Documentation ToDo
 * @param new_data_int
 */ 
        void setNewDataInt(bool new_data_int);
/** Documentation ToDo
 * @param enable_adv_int
 */ 
        void setEnableAdvInt(bool enable_adv_int);
/** Documentation ToDo
 * @param spi4
 */ 
        void setSPI4(bool spi4);    
        
        // LG / HG registers
/** Documentation ToDo
 */ 
        uint8_t getLGThreshold();
/** Documentation ToDo
 * @param lgthres
 */ 
        void  setLGThreshold(uint8_t lgthres);

/** Documentation ToDo
 */ 
        uint8_t getLGDuration();
/** Documentation ToDo
 * @param lgdur
 */ 
        void  setLGDuration(uint8_t lgdur);

/** Documentation ToDo
 */ 
        uint8_t getHGThreshold();
/** Documentation ToDo
 * @param hgthres
 */ 
        void  setHGThreshold(uint8_t hgthres);

/** Documentation ToDo
 */ 
        uint8_t getHGDuration();
/** Documentation ToDo
 * @param hgdur
 */ 
        void  setHGDuration(uint8_t hgdur);

        // MOTION_THRS register
/** Documentation ToDo
 */ 
        uint8_t getMotionThreshold();
/** Documentation ToDo
 * @param mot_thres
 */ 
        void  setMotionThreshold(uint8_t mot_thres);
        
        // HYSTERESIS register
/** Documentation ToDo
 */ 
        uint8_t getLGHysteresis();
/** Documentation ToDo
 * @param lg_hys
 */ 
        void  setLGHysteresis(uint8_t lg_hys);
/** Documentation ToDo
 */ 
        uint8_t getHGHysteresis();
/** Documentation ToDo
 * @param hg_hys
 */ 
        void  setHGHysteresis(uint8_t hg_hys);
/** Documentation ToDo
 */ 
        uint8_t getMotionDuration();
/** Documentation ToDo
 * @param mot_dur
 */ 
        void  setMotionDuration(uint8_t mot_dur);
        
        // CUSTOMER registers
/** Documentation ToDo
 */ 
        uint8_t getCustom1();
/** Documentation ToDo 
 * @param custom1
 */ 
        void setCustom1(uint8_t custom1);
/** Documentation ToDo
 */ 
        uint8_t getCustom2();
/** Documentation ToDo
 * @param custom2
 */ 
        void setCustom2(uint8_t custom2);
        
        // RANGE / BANDWIDTH registers
/** Get Sensor Full Range
 * @return Current Sensor Full Scale Range
 * 0 = +/- 2G
 * 1 = +/- 4G
 * 2 = +/- 8G
 * @see BMA150_RA_RANGE_BWIDTH
 * @see BMA150_RANGE_BIT
 * @see BMA150_RANGE_LENGTH
 */
        uint8_t getRange();
/** Set Sensor Full Range
 * @param range New full-scale range value
 * @see getRange()
 * @see BMA150_RA_RANGE_BWIDTH
 * @see BMA150_RANGE_BIT
 * @see BMA150_RANGE_LENGTH
 */
        void setRange(uint8_t range);
/** Get digital filter bandwidth.
 * The bandwidth parameter is used to setup the digital filtering of ADC output data to obtain
 * the desired bandwidth.
 * @return Current Sensor Bandwidth
 * 0 = 25Hz
 * 1 = 50Hz
 * 2 = 100Hz
 * 3 = 190Hz
 * 4 = 375Hz
 * 5 = 750Hz
 * 6 = 1500Hz
 * @see BMA150_RA_RANGE_BWIDTH
 * @see BMA150_RANGE_BIT
 * @see BMA150_RANGE_LENGTH
 */
        uint8_t getBandwidth();
/** Set Sensor Full Range
 * @param bandwidth New bandwidth value
 * @see getBandwidth()
 * @see BMA150_RA_RANGE_BWIDTH
 * @see BMA150_RANGE_BIT
 * @see BMA150_RANGE_LENGTH
 */
        void setBandwidth(uint8_t bandwidth);
        
        // OFFS_GAIN registers
        
        // OFFSET registers
        
        private:
        uint8_t devAddr;   /**< Address of teh device */
        uint8_t buffer[6]; /**< data buffer*/
        uint8_t mode;      /**< mode of the device */
};

#endif /* _BMA150_H_ */