// I2Cdev library collection - HMC5843 I2C device class header file
// Based on Honeywell HMC5843 datasheet, 6/2010 (Form #900367)
// 8/22/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-08-22 - initial release

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

#ifndef _HMC5843_H_
#define _HMC5843_H_

#include "I2Cdev.h"

#define HMC5843_ADDRESS            0x1E // this device only has one address
#define HMC5843_DEFAULT_ADDRESS    0x1E

#define HMC5843_RA_CONFIG_A        0x00
#define HMC5843_RA_CONFIG_B        0x01
#define HMC5843_RA_MODE            0x02
#define HMC5843_RA_DATAX_H         0x03
#define HMC5843_RA_DATAX_L         0x04
#define HMC5843_RA_DATAY_H         0x05
#define HMC5843_RA_DATAY_L         0x06
#define HMC5843_RA_DATAZ_H         0x07
#define HMC5843_RA_DATAZ_L         0x08
#define HMC5843_RA_STATUS          0x09
#define HMC5843_RA_ID_A            0x0A
#define HMC5843_RA_ID_B            0x0B
#define HMC5843_RA_ID_C            0x0C

#define HMC5843_CRA_RATE_BIT       4
#define HMC5843_CRA_RATE_LENGTH    3
#define HMC5843_CRA_BIAS_BIT       1
#define HMC5843_CRA_BIAS_LENGTH    2

#define HMC5843_RATE_0P5           0x00
#define HMC5843_RATE_1             0x01
#define HMC5843_RATE_2             0x02
#define HMC5843_RATE_5             0x03
#define HMC5843_RATE_10            0x04 // default
#define HMC5843_RATE_20            0x05
#define HMC5843_RATE_50            0x06

#define HMC5843_BIAS_NORMAL        0x00 // default
#define HMC5843_BIAS_POSITIVE      0x01
#define HMC5843_BIAS_NEGATIVE      0x02

#define HMC5843_CRB_GAIN_BIT       7
#define HMC5843_CRB_GAIN_LENGTH    3

#define HMC5843_GAIN_1620          0x00
#define HMC5843_GAIN_1300          0x01 // default
#define HMC5843_GAIN_970           0x02
#define HMC5843_GAIN_780           0x03
#define HMC5843_GAIN_530           0x04
#define HMC5843_GAIN_460           0x05
#define HMC5843_GAIN_390           0x06
#define HMC5843_GAIN_280           0x07 // not recommended

#define HMC5843_MODEREG_BIT        1
#define HMC5843_MODEREG_LENGTH     2

/**
 * During continuous-measurement mode, the device continuously makes
 * measurements and places measured data in data output registers. Settings in
 * the configuration register affect the data output rate (bits DO[n]), the
 * measurement configuration (bits MS[n]), and the gain (bits GN[n]) when in
 * continuous-measurement mode. To conserve current between measurements, the
 * device is placed in a state similar to idle mode, but the mode is not changed
 * to idle mode. That is, MD[n] bits are unchanged. Data can be re-read from the
 * data output registers if necessary; however, if the master does not ensure
 * that the data register is accessed before the completion of the next
 * measurement, the new measurement may be lost. All registers maintain values
 * while in continuous-measurement mode. The I2C bus is enabled for use by other
 * devices on the network in while continuous-measurement mode.
 */
#define HMC5843_MODE_CONTINUOUS    0x00
/**
 * This is the default single supply power-up mode. In dual supply configuration
 * this is the default mode when AVDD goes high. During single-measurement mode,
 * the device makes a single measurement and places the measured data in data
 * output registers. Settings in the configuration register affect the
 * measurement configuration (bits MS[n]), and the gain (bits GN[n]) when in
 * single-measurement mode. After the measurement is complete and output data
 * registers are updated, the device is placed sleep mode, and the mode register
 * is changed to sleep mode by setting MD[n] bits. All registers maintain values
 * while in single-measurement mode. The I2C bus is enabled for use by other
 * devices on the network while in single-measurement mode.
 */
#define HMC5843_MODE_SINGLE        0x01
/**
 * During this mode the device is accessible through the I2C bus, but major
 * sources of power consumption are disabled, such as, but not limited to, the
 * ADC, the amplifier, the SVDD pin, and the sensor bias current. All registers
 * maintain values while in idle mode. The I2C bus is enabled for use by other
 * devices on the network while in idle mode.
 */
#define HMC5843_MODE_IDLE          0x02
/**
 * This is the default dual supply power-up mode when only DVDD goes high and
 * AVDD remains low. During sleep mode the device functionality is limited to
 * listening to the I2C bus. The internal clock is not running and register
 * values are not maintained while in sleep mode. The only functionality that
 * exists during this mode is the device is able to recognize and execute any
 * instructions specific to this device but does not change from sleep mode due
 * to other traffic on the I2C bus. The I2C bus is enabled for use by other
 * devices on the network while in sleep mode. This mode has two practical
 * differences from idle mode. First this state will create less noise on system
 * since the clock is disabled, and secondly this state is a lower current
 * consuming state since the clock is disabled.
 */
#define HMC5843_MODE_SLEEP         0x03

#define HMC5843_STATUS_REN_BIT     2
#define HMC5843_STATUS_LOCK_BIT    1
#define HMC5843_STATUS_READY_BIT   0
/** Class Driver for the Magnetometer
 */
class HMC5843 {
    public:
    
/** Default constructor, uses default I2C address.
 * @see HMC5843_DEFAULT_ADDRESS
 */
        HMC5843();
/** Specific address constructor.
 * @param address I2C address
 * @see HMC5843_DEFAULT_ADDRESS
 * @see HMC5843_ADDRESS
 */
        HMC5843(uint8_t address);
        
/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
        void initialize();
/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
        bool testConnection();

        // CONFIG_A register
/** Get data output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate.
 *
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.5
 * 1     | 1
 * 2     | 2
 * 3     | 5
 * 4     | 10 (Default)
 * 5     | 20
 * 6     | 50
 * 7     | Not used
 *
 * @return Current rate of data output to registers
 * @see HMC5843_RATE_10
 * @see HMC5843_RA_CONFIG_A
 * @see HMC5843_CRA_RATE_BIT
 * @see HMC5843_CRA_RATE_LENGTH
 */
        uint8_t getDataRate();
/** Set data output rate value.
 * @param rate Rate of data output to registers
 * @see getDataRate()
 * @see HMC5843_RATE_10
 * @see HMC5843_RA_CONFIG_A
 * @see HMC5843_CRA_RATE_BIT
 * @see HMC5843_CRA_RATE_LENGTH
 */
        void setDataRate(uint8_t rate);
/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5843_BIAS_NORMAL
 * @see HMC5843_RA_CONFIG_A
 * @see HMC5843_CRA_BIAS_BIT
 * @see HMC5843_CRA_BIAS_LENGTH
 */
        uint8_t getMeasurementBias();
/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5843_BIAS_NORMAL
 * @see HMC5843_RA_CONFIG_A
 * @see HMC5843_CRA_BIAS_BIT
 * @see HMC5843_CRA_BIAS_LENGTH
 */
        void setMeasurementBias(uint8_t bias);

        // CONFIG_B register
/** Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the Gain column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the data output registers (saturation).
 * The data output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.7 Ga  | 1620
 * 1     | +/- 1.0 Ga  | 1300 (Default)
 * 2     | +/- 1.5 Ga  | 970
 * 3     | +/- 2.0 Ga  | 780
 * 4     | +/- 3.2 Ga  | 530
 * 5     | +/- 3.8 Ga  | 460
 * 6     | +/- 4.5 Ga  | 390
 * 7     | +/- 6.5 Ga  | 280 (Not recommended)
 *
 * @return Current magnetic field gain value
 * @see HMC5843_GAIN_1300
 * @see HMC5843_RA_CONFIG_B
 * @see HMC5843_CRB_GAIN_BIT
 * @see HMC5843_CRB_GAIN_LENGTH
 */
        uint8_t getGain();
/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see getGain()
 * @see HMC5843_RA_CONFIG_B
 * @see HMC5843_CRB_GAIN_BIT
 * @see HMC5843_CRB_GAIN_LENGTH
 */
        void setGain(uint8_t gain);

        // MODE register
/** Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the data register. RDY goes high when new data is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * data output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of data output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the data output register
 * and RDY remains high until the data output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5843_MODE_CONTINUOUS
 * @see HMC5843_MODE_SINGLE
 * @see HMC5843_MODE_IDLE
 * @see HMC5843_MODE_SLEEP
 * @see HMC5843_RA_MODE
 * @see HMC5843_MODEREG_BIT
 * @see HMC5843_MODEREG_LENGTH
 */
        uint8_t getMode();
/** Set measurement mode.
 * @param mode New measurement mode
 * @see getMode()
 * @see HMC5843_MODE_CONTINUOUS
 * @see HMC5843_MODE_SINGLE
 * @see HMC5843_MODE_IDLE
 * @see HMC5843_MODE_SLEEP
 * @see HMC5843_RA_MODE
 * @see HMC5843_MODEREG_BIT
 * @see HMC5843_MODEREG_LENGTH
 */
        void setMode(uint8_t mode);

        // DATA* registers
/** Get 3-axis heading measurements.
 * In the event the ADC reading overflows or underflows for the given channel,
 * or if there is a math overflow during the bias measurement, this data
 * register will contain the value -4096. This register value will clear when
 * after the next valid measurement is made. Note that this method automatically
 * clears the appropriate bit in the MODE register if Single mode is active.
 * @param x 16-bit signed integer container for X-axis heading
 * @param y 16-bit signed integer container for Y-axis heading
 * @param z 16-bit signed integer container for Z-axis heading
 * @see HMC5843_RA_DATAX_H
 */
        void getHeading(int16_t *x, int16_t *y, int16_t *z);
/** Get X-axis heading measurement.
 * @return 16-bit signed integer with X-axis heading
 * @see HMC5843_RA_DATAX_H
 */
        int16_t getHeadingX();
/** Get Y-axis heading measurement.
 * @return 16-bit signed integer with Y-axis heading
 * @see HMC5843_RA_DATAY_H
 */
        int16_t getHeadingY();
/** Get Z-axis heading measurement.
 * @return 16-bit signed integer with Z-axis heading
 * @see HMC5843_RA_DATAZ_H
 */
        int16_t getHeadingZ();

        // STATUS register
/** Get regulator enabled status.
 * This bit is set when the internal voltage regulator is enabled. This bit is
 * cleared when the internal regulator is disabled.
 * @return Regulator enabled status
 * @see HMC5843_RA_STATUS
 * @see HMC5843_STATUS_REN_BIT
 */
        bool getRegulatorEnabledStatus();
/** Get data output register lock status.
 * This bit is set when this some but not all for of the six data output
 * registers have been read. When this bit is set, the six data output registers
 * are locked and any new data will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return Data output register lock status
 * @see HMC5843_RA_STATUS
 * @see HMC5843_STATUS_LOCK_BIT
 */
        bool getLockStatus();
/** Get data ready status.
 * This bit is set when data is written to all six data registers, and cleared
 * when the device initiates a write to the data output registers and after one
 * or more of the data output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement data.
 * @return Data ready status
 * @see HMC5843_RA_STATUS
 * @see HMC5843_STATUS_READY_BIT
 */
        bool getReadyStatus();

        // ID_* registers
/** Get identification byte A
 * @return ID_A byte (should be 01001000, ASCII value 'H')
 */
        uint8_t getIDA();
/** Get identification byte B
 * @return ID_A byte (should be 00110100, ASCII value '4')
 */
        uint8_t getIDB();
/** Get identification byte C
 * @return ID_A byte (should be 00110011, ASCII value '3')
 */
        uint8_t getIDC();

    private:
        uint8_t devAddr;   /**< Address of the device */
        uint8_t buffer[6]; /**< Data buffer */ 
        uint8_t mode;      /**< Mode of device */
};

#endif /* _HMC5843_H_ */
