/** LSM303LHC I2C device class header file intended to be part 
 * of the I2Cdevlib, but I've not follwed the same format entirely.
 * Based on LSM303LHC datasheet
 * 14/05/2016 by Chris Dick
 * Changelog:
 * ============================================
 * 
 * device library code is placed under the MIT license
 * Copyright (c) 2016 Chris Dick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ===============================================
 */

#ifndef LSM303LHC_H
#define LSM303LHC_H

#include "I2Cdev.h"

/* 
    LSM303LHC Address definitions 
    7bit addresses, datasheet gives 8bit with read and write bits
    Mag = 0x3C and Accel 0x32
*/
#define LSM303_MAG  0x1E  
#define LSM303_ACC  0x19  


/* Linear acceleration register definitions */
/* Accelerometer control register */
#define CTRL_REG1_A 0x20

union ctrl_reg1_a_t /** Accelerometer Control Register 1 */
{
    uint8_t all; /**< Access to entire structure */
    
    struct {
        uint8_t XEnable        :1; /**< Enable X axis */
        uint8_t YEnable        :1; /**< Enable Y Axis */
        uint8_t ZEnable        :1; /**< Enable Z Axis */
        uint8_t LowPowerMode   :1; /**< Enable Low power mode */
        uint8_t DataRateSelect :4; /**< Select Data rate */
    } bits; /**< Access to individual bits */
};


typedef enum 
{
    DISABLE,
    ENABLE
} lsm303lhc_enable_t;

typedef enum
{
    NORMAL,
    LOW_POWER
} lsm303lhc_accel_power_mode_t;

typedef enum
{
    OFF,
    ONE_HZ,
    TEN_HZ,
    TWENTY_FIVE_HZ,
    FIFTY_HZ,
    ONE_HUNDRED_HZ,
    TWO_HUNDRED_HZ,
    FOUR_HUNDRED_HZ,
    LOW_POWER_MODE,
    NORMAL_MODE,
    MAX_DATA_RATE  /** this one should always be last */
} lsm303lhc_accel_data_rate_t;



/* Accelerometer filter control register */
#define CTRL_REG2_A 0x21

union ctrl_reg2_a_t /**< Accelerometer Filter Control Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct {
        uint8_t Interrupt1HighPassEnable    :1; /**< Interrupt 1 high pass enable bit */
        uint8_t Interrupt2HighPassEnable    :1; /**< Interrupt 2 high pass enable bit */
        uint8_t ClickHighPassEnable         :1; /**< click high pass enable bit       */
        uint8_t FilterDataSelect            :1; /**< Filter data select bit           */
        uint8_t HighPassCutOff              :2; /**< High pass cut off                */
        uint8_t HighPassMode                :2; /**< High pass mode                   */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    NO_FILTER,
    FILTERED
} lsm303lhc_accel_filtered_data_t;

typedef enum
{
    NORMAL_MODE_REset,
    REFERENCE_SIGNAL,
    NORMAL_PASS_MODE,
    AUTOREset
} lsm303lhc_accel_high_pass_mode_t;

/* Accelerometer interrupt 1 control register */
#define CTRL_REG3_A 0x22

union interrupt1_t /**< Accelerometer Interrupt 1 Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Overrun       :1; /**< enable overrun bit                */
        uint8_t Watermark     :1; /**< enable watermark interrupt bit    */
        uint8_t DataReady2    :1; /**< data ready interrupt 1 enable bit */
        uint8_t DataReady1    :1; /**< data ready interrupt 2 enable bit */
        uint8_t AOI1Interrupt :1; /**< AOI interrupt 1 enable bit        */
        uint8_t AOI2Interrupt :1; /**< AOI interrupt 2 enable bit        */
        uint8_t Click         :1; /**< Click interrupt enable bit        */
    } bits; /**< Access to individual bits */
};


/* Accelerometer configuration register */
#define CTRL_REG4_A 0x23

union accelerometer_control_t /**< Accelerometer Control Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct 
    {
        uint8_t SpiMode              :1; /**< SPI mode configuration bit */
        uint8_t empty                :2; /**< Not used                   */
        uint8_t HighResolutionMode   :1; /**< High resolution mode bit   */
        uint8_t FullScaleSelection   :2; /**< Full scale selection bits  */
        uint8_t Endianess            :1; /**< Endianess selection bits   */
        uint8_t BlockUpdateMode      :1; /**< Block update mode bits     */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    BIG,
    LITTLE
} lsm303lhc_accel_endianess_t;

typedef enum
{
    TWO_G,
    FOUR_G,
    EIGHT_G,
    SIXTEEN_G
} lsm303lhc_accel_full_scale_t; 

typedef enum
{
    CONTINUOS_ACCEL,
    READING_ACCEL
} lsm303lhc_accell_update_mode_t;

/* Accelerometer interrupt config register */
#define CTRL_REG5_A 0x24

union lsm303_accel_interrupt_config_t /**< Accelerometer Interrupt 1 Configuration Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t FourDEnableInt2   :1; /**< Four D interrupt 2 enable bit */
        uint8_t LatchIntRequest2  :1; /**< Latch interrupt 2 enable bit  */
        uint8_t FourDEnableInt1   :1; /**< Four D enable interrupt 1 bit */
        uint8_t LatchIntRequest1  :1; /**< Latch interrupt 1 enable bit  */
        uint8_t Empty             :2; /**< not used                      */
        uint8_t FIFOEnable        :1; /**< FIFO enable bit               */
        uint8_t Reboot            :1; /**< Reboot device bit             */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    NOT_LATCHED,
    LATCHED
} lsm303lhc_accel_interrupt_latch_t;

#define CTRL_REG6_A 0x25

union lsm303_accel_interrupt_config_2_t /**< Accelerometer interrupt 2 Configuration Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Empty               :1; /**< not used                    */
        uint8_t ActiveHigh          :1; /**< Active high enable bit      */
        uint8_t Reserved            :1; /**< not used                    */
        uint8_t ActiveFunctionPAD2  :1; /**< Active function PAD2 bit    */
        uint8_t RebootPAD2          :1; /**< reboot PAD2 bit             */
        uint8_t Int2PAD2            :1; /**< Interrupt 2 PAD2 enable bit */
        uint8_t Int1PAD2            :1; /**< Interrupt 1 PAD2 enable bit */
        uint8_t ClickPAD2           :1; /**< Click OAD2 enable bit       */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    ACTIVEHIGH,
    ACTIVELOW
} lsm303lhc_accell_interrupt_active_t;

/* Refernce value for Interrupt generation */
#define REFERENCE_A 0x26
/* Accelerometer Status Register */
#define STATUS_REG_A 0x27

union accel_status_t /**< Accelerometer Status Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t XDataAvaliable    :1; /**< Data for X axis available   */
        uint8_t YDataAvaliable    :1; /**< Data for Y axis available   */
        uint8_t ZDataAvaliable    :1; /**< Data for Z axis available   */
        uint8_t AllDataAvaliable  :1; /**< Data for all axes available */
        uint8_t XOverrun          :1; /**< X axis overrun              */
        uint8_t YOverrun          :1; /**< Y axis overrun              */
        uint8_t ZOverrun          :1; /**< Z axis overrun              */
        uint8_t AllOverrun        :1; /**< All axes overrun              */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    NOT_AVAILABLE,
    AVAILABLE
} lsm303lhc_accel_avaliable_t;

/* Accelerometer output registers */
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D

/* FIFO control register */
#define FIFO_CTRL_REG_A 0x2E

union fifo_control_t /**< Accelerometer FIFO control Register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Reserved      :5; /**< Not used              */
        uint8_t TriggerSelect :1; /**< Trigger select bits   */
        uint8_t FIFOMode      :1; /**< FIFO mode select bits */
    } bits; /**< Access to individual bits */
};

typedef enum
{
    BYPASS,
    FIFO,
    STREAM,
    TRIGGER
} lsm303lhc_accel_fifo_mode_t;

typedef enum
{
    INTERRUPT_ONE,
    INTERRUPT_TWO
} lsm303lhc_accel_trigger_mode_t;

/* data sheet is missing info on this one?? */
#define FIFO_SRC_REG_A 0x2F

/* Interrupt 1 config register */
#define INT1_CFG_A 0x30
#define INT2_CFG_A 0x34

union lsm303lhc_accel_int_config_t /**< Union to access the Interrupt 1 configuration register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t XLowEvent      :1; /**< Enable X Low Event  */
        uint8_t XHighEvent     :1; /**< Enable X High Event */
        uint8_t YLowEvent      :1; /**< Enable Y Low Event  */
        uint8_t YHighEvent     :1; /**< Enable Y High Event */
        uint8_t ZLowEvent      :1; /**< Enable Z Low Event  */
        uint8_t ZHighEvent     :1; /**< Enable Z High Event */
        uint8_t TypeEnable     :2; /**< Enable type         */
    } bits; /**< Access to bits */
};

typedef enum
{
    OR_COMBINATION,
    SIX_D_DIRECTION,
    AND_COMBINATION,
    SIX_D_POSITION
} lsm303lhc_accel_interrupt_mode_t;

/* Interrupt source register */
#define INT1_SRC_A 0x31
#define INT2_SRC_A 0x35
union lsm303lhc_accel_int_source_t /**< Accelerometer interrupt source */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t XLow             :1 ; /**< Accelerometer interrupt came from X Low  */
        uint8_t XHigh            :1 ; /**< Accelerometer interrupt came from X High */
        uint8_t YLow             :1 ; /**< Accelerometer interrupt came from Y Low  */
        uint8_t YHigh            :1 ; /**< Accelerometer interrupt came from Y High */
        uint8_t ZLow             :1 ; /**< Accelerometer interrupt came from Z Low  */
        uint8_t ZHigh            :1 ; /**< Accelerometer interrupt came from Z High */
        uint8_t InterruptActive  :1 ; /**< Interrupt active status                  */ 
        uint8_t Zero             :1 ; /**< Must be 0                                */
    } bits; /**< Access to bits */
};

typedef enum
{
    NO_EVENT,
    EVENT_ACTIVE
} lsm303lhc_accel_event_active_t;

/* interrupt 1 threshold register */
#define INT1_THS_A  0x32
#define INT2_THS_A  0x36
/* Click event threshold */
#define CLICK_THS_A 0x3A

union lsm303lhc_accel_int_threshold_t /**< Accelerometer interrupt threshold configuration */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Threshold   :7; /**< Interrupt threshold */
        uint8_t Zero        :1; /**< Must be 0           */
    } bits; /**< Access to bits */
};

/* Interrupt duration rgister */
/* Duration of event for interrupt */
#define INT1_DURATION_A 0x33
#define INT2_DURATION_A 0x37

union lsm303lhc_accel_int_duration_t /**< Accelerometer interrupt duration configuration */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Duration    :7; /**< Interrupt duration */ 
        uint8_t Zero        :1; /**< Must be 0          */
    } bits; /**< Access to bits */
};

/* Click event config */
#define CLICK_CFG_A    0x38

union lsm303lhc_accel_click_config_t  /**< Accelerometer click configuration */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t XSingleEnable   :1; /**< X axis single click enable */
        uint8_t XDoubleEnable   :1; /**< X axis double click enable */
        uint8_t YSingleEnable   :1; /**< Y axis single click enable */
        uint8_t YDoubleEnable   :1; /**< Y axis double click enable */
        uint8_t ZSingleEnable   :1; /**< Z axis single click enable */
        uint8_t ZDoubleEnable   :1; /**< Z axis double click enable */
        uint8_t Empty           :2; /**< Not used                   */
    } bits; /**< Access to bits */
};

/* Click event source */
#define CLICK_SRC_A    0x39

union lsm303lhc_accel__t /**< Accelerometer Event register */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t XClick            :1; /**< X click interrupt active */
        uint8_t YClick            :1; /**< Y click interrupt active */
        uint8_t ZClick            :1; /**< Z click interrupt active */
        uint8_t Sign              :1; /**< Sign interrupt active    */
        uint8_t Single            :1; /**< single interrupt active  */
        uint8_t Double            :1; /**< double interrupt active  */
        uint8_t InterruptActive   :1; /**< interrupt active         */
    } bits; /**< Access to bits */
};

/* Time limit between click down and click up register */
#define TIME_LIMIT_A   0x3B
union lsm303lhc_accel_time_limit_t  /**< Accelerometer interrupt time limit configuration */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Limit   :7; /**< time limit */
        uint8_t Empty   :1; /**< not used   */
    } bits; /**< Access to bits */
};

/* Time latency register */
/* for use in doubel click detection */
#define TIME_LATENCY_A 0x3C

/* Time window register */
/* for use in doubel click detection */
#define TIME_WINDOW_A  0x3D

/*
    Magnetic field sensing registers
*/

/* Megneto configuration register A */
#define CRA_REG_M 0x00
union lsm303lhc_mag_config_a_t  /**< Magnetometer configuration A */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Empty                   :2; /**< Not used                  */
        uint8_t DataRate                :3; /**< Data rate                 */
        uint8_t Reserved                :2; /**< Not used                  */
        uint8_t TemperatureSensorEnable :1; /**< Temperature sensor enable */
    } bits; /**< Access to bits */
};

typedef enum
{
    RATE_0_75_HZ,
    RATE_1_5_HZ,
    RATE_3_0_HZ,
    RATE_7_5_HZ,
    RATE_15_0_HZ,
    RATE_30_0_HZ,
    RATE_70_0_HZ,
    RATE_220_0_HZ
} lsm303lhc_mag_data_rate_t;

/* Megneto configuration register B */
#define CRB_REG_M 0x01
union lsm303lhc_mag_config_b_t  /**< Magnetometer configuration B */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Empty   :5; /**< Not used     */
        uint8_t Gain    :3; /**< Gain setting */
    } bits; /**< Access to bits */
};

typedef enum
{                         //full-scale 
    MAG_SCALE_1_3, /**> +/-1.3Gauss */
    MAG_SCALE_1_9, /**> +/-1.9Gauss */
    MAG_SCALE_2_5, /**> +/-2.5Gauss */
    MAG_SCALE_4_0, /**> +/-4.0Gauss */
    MAG_SCALE_4_7, /**> +/-4.7Gauss */
    MAG_SCALE_5_6, /**> +/-5.6Gauss */
    MAG_SCALE_8_1, /**> +/-8.1Gauss */
} lsm303lhc_mag_scale_t;

/* Mode select register */
#define MR_REG_M 0x02
union lsm303lhc_mag_mode_select_t /**< Magnetometer mode select */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Mode  :2; /**< Mode selection bits */
        uint8_t Empty :6; /**< Not used            */
    } bits; /**< Access to bits */
};

typedef enum
{
    CONTINUOUS,
    SINGLE,
    SLEEP,
} lsm303lhc_mag_mode_t;

/* Magneto output registers */
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Y_H_M 0x07
#define OUT_Y_L_M 0x08
#define OUT_Z_H_M 0x05
#define OUT_Z_L_M 0x06

/* output register config */
#define SR_REG_M 0x09
union lsm303lhc_mag_sr_config_t /**< Magnetometer configuration */
{
    uint8_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t DataReady    :1; /**< Data ready enable  */
        uint8_t UpdateLock   :1; /**< enable update lock */
        uint8_t Empty        :6; /**< Not used           */
    } bits; /**< Access to bits */
};

/* who am I? */
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C

/* Temperature output registers */
#define TEMP_OUT_H_M 0x31
#define TEMP_OUT_L_M 0x32

union lsm303lhc_mag_temperature_t /**< Temperature measurement */
{
    uint16_t all; /**< Access to entire structure */
    
    struct
    {
        uint8_t Lsb; /**< Access to least significant byte */
        uint8_t Msb; /**< Access to most significant byte  */
    } byte; /**< Access to bytes */
    
    struct 
    {
        uint16_t Empty       :4;  /**< Not used                   */
        uint16_t Temperature :12; /**> temperature in 8lsb/degree */
    } bits; /**< Access to bits */
};


/** Class Driver for the Magnetometer
 */
class LSM303DLHC_Accel {

    public:
/** Default constructor, uses default I2C address.
 * @see LSM303LHC_DEFAULT_ADDRESS
 */
        LSM303DLHC_Accel();
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

/** set 
 * @param enable X axis
 */
        void setXEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t X axis enable
 */
        lsm303lhc_enable_t getXEnable();
/** set 
 * @param enable Y axis
 */
        void setYEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t Y axis enable
 */
        lsm303lhc_enable_t getYEnable();
/** set 
 * @param enable Z axis
 */
        void setZEnable( lsm303lhc_enable_t enable);
/** get 
 * @return lsm303lhc_enable_t Z axis enable
 */
        lsm303lhc_enable_t getZEnable();
/** set 
 * @param mode desired power mode
 */
        void setLowPowerMode(lsm303lhc_accel_power_mode_t mode);
/** get 
 * @return lsm303lhc_accel_power_mode_t the current power mode
 */
        lsm303lhc_accel_power_mode_t getLowPowerMode();
/** set 
 * @param rate set the accelerometer data rate
 */
        void setDataRateSelect( lsm303lhc_accel_data_rate_t rate );
/** get 
 * @return lsm303lhc_accel_data_rate_t the current accelerometer data rate
 */
        lsm303lhc_accel_data_rate_t getDataRateSelect();
/** set 
 * @param enable the high pass filter interrupt
 */
        void setInterrupt1HighPassEnable( lsm303lhc_enable_t enable);
/** get 
 * @return lsm303lhc_enable_t the current high pass interrupt state
 */
        lsm303lhc_enable_t getInterrupt1HighPassEnable();
/** set 
 * @param enable the high pass interrupt 2
 */
        void setInterrupt2HighPassEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the High pass interrupt state
 */
        lsm303lhc_enable_t getInterrupt2HighPassEnable();
/** set 
 * @param enable the click high pass interrupt
 */
        void setClickHighPassEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the click high pass interrupt state
 */
        lsm303lhc_enable_t getClickHighPassEnable();
/** set 
 * @param filtered enable the data filter
 */
        void setFilterDataSelect( lsm303lhc_accel_filtered_data_t filtered);
/** get 
 * @return lsm303lhc_accel_filtered_data_t the data filter state
 */
        lsm303lhc_accel_filtered_data_t getFilterDataSelect();
/** set 
 * @param cutoff enable the high pass cutoff
 */
        void setHighPassCutOff( uint8_t cutoff);
/** get 
 * @return uint8_t the high pass cutoff 
 */
        uint8_t getHighPassCutOff();
/** set 
 * @param mode the high pass mode
 */
        void setHighPassMode( lsm303lhc_accel_high_pass_mode_t mode );
/** get 
 * @return lsm303lhc_accel_high_pass_mode_t the current high pass mode
 */
        lsm303lhc_accel_high_pass_mode_t getHighPassMode();
/** set 
 * @param enable the overrun
 */
        void setOverrun( lsm303lhc_enable_t enable);
/** get 
 * @return lsm303lhc_enable_t the current overrun state
 */
        lsm303lhc_enable_t getOverrun();
/** set 
 * @param enable the watermark
 */
        void setWatermark( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current watermark state
 */
        lsm303lhc_enable_t getWatermark();
/** set 
 * @param enable the data ready 2
 */
        void setDataReady2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the data ready 2 state
 */
        lsm303lhc_enable_t getDataReady2();
/** set 
 * @param enable the data ready 1
 */
        void setDataReady1( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the data ready 1 state
 */
        lsm303lhc_enable_t getDataReady1();
/** set 
 * @param enable the AOI 1 interrupt
 */
        void setAOI1Interrupt( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the AOI 1 interrupt state
 */
        lsm303lhc_enable_t getAOI1Interrupt();
/** set 
 * @param enable the AOI 2 interrupt
 */
        void setAOI2Interrupt( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current AOI 2 state
 */
        lsm303lhc_enable_t getAOI2Interrupt();
/** set 
 * @param enable the click detection
 */
        void setClick( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current click detection state
 */
         lsm303lhc_enable_t getClick( );
/** set 
 * @param enable the high resolution mode
 */
        void setHighResolutionMode( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current high resolution mode
 */
         lsm303lhc_enable_t getHighResolutionMode();
/** set 
 * @param scale the full scale selection
 */
        void setFullScaleSelection( lsm303lhc_accel_full_scale_t scale );
/** get 
 * @return lsm303lhc_accel_full_scale_t the current full scale selection
 */
        lsm303lhc_accel_full_scale_t getFullScaleSelection();
/** set 
 * @param endianess the endianess
 */
        void setEndianess( lsm303lhc_accel_endianess_t endianess);
/** get 
 * @return lsm303lhc_accel_endianess_t the current endianess
 */
        lsm303lhc_accel_endianess_t getEndianess();
/** set 
 * @param mode the block update mode
 */
        void setBlockUpdateMode( lsm303lhc_accell_update_mode_t mode );
/** get 
 * @return lsm303lhc_accell_update_mode_t the current block update mode
 */
        lsm303lhc_accell_update_mode_t getBlockUpdateMode();
/** get 
 * @return lsm303lhc_enable_t the current 4D interrupt 2
 */
        lsm303lhc_enable_t getFourDEnableInt2();
/** set 
 * @param enable the 4D interrupt 2
 */
        void setFourDEnableInt2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the latch interrupt 2
 */
        lsm303lhc_enable_t getLatchIntRequest2();
/** set 
 * @param enable the latch interrupt 2
 */
        void setLatchIntRequest2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the 4D interrupt 1
 */
        lsm303lhc_enable_t getFourDEnableInt1();
/** set 
 * @param enable the 4D latch interrupt 1
 */
        void setFourDEnableInt1( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_accel_interrupt_latch_t the current Latch interrupt request
 */
        lsm303lhc_accel_interrupt_latch_t getLatchIntRequest1();
/** set 
 * @param latch interrupt 1
 */
        void setLatchIntRequest1( lsm303lhc_accel_interrupt_latch_t latch );
/** get 
 * @return lsm303lhc_enable_t the current FIFO state
 */
        lsm303lhc_enable_t getFIFOEnable();
/** set 
 * @param enable the FIFO
 */
        void setFIFOEnable( lsm303lhc_enable_t enable );
/** set 
 * @param enable the reboot
 */
        void setReboot( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t Active high
 */
        lsm303lhc_enable_t getActiveHigh();
/** set 
 * @param enable the active high
 */
        void setActiveHigh( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current sctive function state
 */
        lsm303lhc_enable_t getActiveFunctionPAD2();
/** set 
 * @param enable the active function pad 2
 */
        void setActiveFunctionPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the current reboot state
 */
        lsm303lhc_enable_t getRebootPAD2();
/** set 
 * @param enable the reboot pad 2
 */
        void setRebootPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the PAD2 interrupt 2 state
 */
        lsm303lhc_enable_t getInt2PAD2();
/** set 
 * @param enable PAD2 interrupt
 */
        void setInt2PAD2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the PAD2 interrupt 1 state
 */
        lsm303lhc_enable_t getInt1PAD2();
/** set 
 * @param enable the PAD2 interrupt 1
 */
        void setInt1PAD2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the click PAD2
 */
        lsm303lhc_enable_t getClickPAD2();
/** set 
 * @param lsm303lhc_enable_t the current click PAD2 state
 */
        void setClickPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_accel_avaliable_t the X data available 
 */
        lsm303lhc_accel_avaliable_t getXDataAvaliable();
/** get 
 * @return lsm303lhc_accel_avaliable_t the Y data available 
 */
        lsm303lhc_accel_avaliable_t getYDataAvaliable();
/** get 
 * @return lsm303lhc_accel_avaliable_t the Z data available 
 */
        lsm303lhc_accel_avaliable_t getZDataAvaliable();
/** get 
 * @return lsm303lhc_accel_avaliable_t all data available 
 */
        lsm303lhc_accel_avaliable_t getAllDataAvaliable();
/** get 
 * @return lsm303lhc_accel_avaliable_t the X overrun available 
 */
        lsm303lhc_accel_avaliable_t getXOverrun();
/** get 
 * @return lsm303lhc_accel_avaliable_t the Y overrun available 
 */
        lsm303lhc_accel_avaliable_t getYOverrun();
/** get 
 * @return lsm303lhc_accel_avaliable_t the Z overrun available 
 */
        lsm303lhc_accel_avaliable_t getZOverrun();
/** get 
 * @return lsm303lhc_accel_avaliable_t all overrun available 
 */
        lsm303lhc_accel_avaliable_t getAllOverrun();
/** get 
 * @return lsm303lhc_accel_trigger_mode_t the current trigger select
 */
        lsm303lhc_accel_trigger_mode_t getTriggerSelect();
/** set 
 * @param mode the desired trigger selection
 */
        void setTriggerSelect( lsm303lhc_accel_trigger_mode_t mode );
/** get 
 * @return lsm303lhc_accel_fifo_mode_t the current FIFO mode
 */
        lsm303lhc_accel_fifo_mode_t getFIFOMode();
/** set 
 * @param mode the desired FIFO mode
 */
        void setFIFOMode( lsm303lhc_accel_fifo_mode_t mode );

/** get 
 * @return lsm303lhc_enable_t the X Low event interrupt state
 */
        lsm303lhc_enable_t getInt1XLowEvent();
/** set 
 * @param enable the X low event interrupt
 */
        void setInt1XLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the X High event interrupt state
 */
        lsm303lhc_enable_t getInt1XHighEvent();
/** set 
 * @param enable the X High event interrupt
 */
        void setInt1XHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the Y Low event interrupt state
 */
        lsm303lhc_enable_t getInt1YLowEvent();
/** set 
 * @param enable the Y low event interrupt
 * @param
 */
        void setInt1YLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the Y High event interrupt state
 */
        lsm303lhc_enable_t getInt1YHighEvent();
/** set 
 * @param enable the Y High event interrupt
 */
        void setInt1YHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the Z Low event interrupt state
 */
        lsm303lhc_enable_t getInt1ZLowEvent();
/** set 
 * @param enable the Z low event interrupt
 */
        void setInt1ZLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t the Z High event interrupt state
 */
        lsm303lhc_enable_t getInt1ZHighEvent();
/** set 
 * @param enable the Z High event interrupt
 */
        void setInt1ZHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t Interrupt 1 type enable
 */
        lsm303lhc_enable_t getInt1TypeEnable();
/** set 
 * @param enable the Interrupt 1 type
 */
        void setInt1TypeEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 X low event active
 */
        lsm303lhc_accel_event_active_t getInt1XLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 X high event active
 */
        lsm303lhc_accel_event_active_t getInt1XHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 Y low event active
 */
        lsm303lhc_accel_event_active_t getInt1YLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 Y high event active
 */
        lsm303lhc_accel_event_active_t getInt1YHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 Z low event active
 */
        lsm303lhc_accel_event_active_t getInt1ZLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 Z high event active
 */
        lsm303lhc_accel_event_active_t getInt1ZHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 1 active
 */
        lsm303lhc_accel_event_active_t getInt1InterruptActive();

/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 X low state
 */
        lsm303lhc_enable_t getInt2XLowEvent();
/** set 
 * @param enable the X low interrupt 2
 */
        void setInt2XLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 X high state
 */
        lsm303lhc_enable_t getInt2XHighEvent();
/** set 
 * @param enable the X high event interrupt 2 
 */
        void setInt2XHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 Y low state
 */
        lsm303lhc_enable_t getInt2YLowEvent();
/** set 
 * @param enable the Y low interrupt 2
 */
        void setInt2YLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 Y high state 
 */
        lsm303lhc_enable_t getInt2YHighEvent();
/** set 
 * @param enable the Y high interrupt 2
 */
        void setInt2YHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 Z low state
 */
        lsm303lhc_enable_t getInt2ZLowEvent();
/** set 
 * @param enable the Z low interrupt 2
 */
        void setInt2ZLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 Z high state
 */
        lsm303lhc_enable_t getInt2ZHighEvent();
/** set 
 * @param enable the Z high interrupt 2 
 */
        void setInt2ZHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t current Interrupt 2 Z high state
 */
        lsm303lhc_enable_t getInt2TypeEnable();
/** set 
 * @param enable the type interrupt 2
 */
        void setInt2TypeEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 X low event active
 */
        lsm303lhc_accel_event_active_t getInt2XLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 X high event active
 */
        lsm303lhc_accel_event_active_t getInt2XHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 Y low event active
 */
        lsm303lhc_accel_event_active_t getInt2YLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 Y high event active
 */
        lsm303lhc_accel_event_active_t getInt2YHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 Z low event active
 */
        lsm303lhc_accel_event_active_t getInt2ZLow();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 Z low event active
 */
        lsm303lhc_accel_event_active_t getInt2ZHigh();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt 2 active
 */
        lsm303lhc_accel_event_active_t getInt2InterruptActive();        
        
/** get 
 * @return uint8_t Interrupt 1 threshold
 */
        uint8_t getInt1Threshold();
/** set 
 * @param Threshold interrupt 1 threshold
 */
        void setInt1Threshold( uint8_t Threshold );
/** get 
 * @return uint8_t initerrupt 2 threshold
 */
        uint8_t getInt2Threshold();
/** set 
 * @param Threshold interrupt 2 threshold
 */
        void setInt2Threshold( uint8_t Threshold );
/** get 
 * @return uint8_t click threshold
 */
        uint8_t getClickThreshold();
/** set 
 * @param Threshold click interrupt threshold
 */
        void setClickThreshold( uint8_t Threshold );
/** get 
 * @return uint8_t interrupt 1 duration
 */
        uint8_t getInt1Duration();
/** set 
 * @param duration interrupt 1 duration
 */
        void setInt1Duration( uint8_t duration );
/** get 
 * @return uint8_t interrupt 2 duration
 */
        uint8_t getInt2Duration();
/** set 
 * @param duration interrupt 2 duration
 */
        void setInt2Duration( uint8_t duration );
/** get 
 * @return lsm303lhc_enable_t X axis single enable
 */
        lsm303lhc_enable_t getXSingleEnable();
/** set 
 * @param enable X axis single
 */
        void setXSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return  lsm303lhc_enable_t X axis double enable
 */
        lsm303lhc_enable_t getXDoubleEnable();
/** set 
 * @param enable X axis double
 */
        void setXDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t Y axis single enable
 */
        lsm303lhc_enable_t getYSingleEnable();
/** set 
 * @param enable Y axis single
 */
        void setYSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return  lsm303lhc_enable_t Y axis double enable
 */
        lsm303lhc_enable_t getYDoubleEnable();
/** set 
 * @param enable Y axis double
 */
        void setYDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_enable_t Z axis single enable
 */
        lsm303lhc_enable_t getZSingleEnable();
/** set 
 * @param enable Z axis single
 */
        void setZSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return  lsm303lhc_enable_t Z axis double enable
 */
        lsm303lhc_enable_t getZDoubleEnable();
/** set 
 * @param enable Z axis double
 */
        void setZDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return lsm303lhc_accel_event_active_t X click event active
 */
        lsm303lhc_accel_event_active_t getXClick();
/** get 
 * @return lsm303lhc_accel_event_active_t Y click event active
 */
        lsm303lhc_accel_event_active_t getYClick();
/** get 
 * @return lsm303lhc_accel_event_active_t Z click event active
 */
        lsm303lhc_accel_event_active_t getZClick();
/** get 
 * @return lsm303lhc_accel_event_active_t Sign event active
 */
        lsm303lhc_accel_event_active_t getSign();
/** get 
 * @return lsm303lhc_accel_event_active_t Single event active
 */
        lsm303lhc_accel_event_active_t getSingle();
/** get 
 * @return lsm303lhc_accel_event_active_t Double event active
 */
        lsm303lhc_accel_event_active_t getDouble();
/** get 
 * @return lsm303lhc_accel_event_active_t Interrupt event active
 */
        lsm303lhc_accel_event_active_t getInterruptActive();
/** get 
 * @return uint8_t Click limit
 */
        uint8_t getClickLimit();
/** set 
 * @param limit click limit
 */
        void setClickLimit( uint8_t limit );
/** get 
 * @return uint8_t click latency
 */
        uint8_t getClickLatency();
/** set 
 * @param Latency click Latency
 */
        void setClickLatency( uint8_t Latency );
/** get 
 * @return uint8_t click window
 */
        uint8_t getClickWindow();
/** set 
 * @param window click window
 */
        void setClickWindow( uint8_t window );

/** get 3-axis accleration measurements.
 * These six bytes are eight bits each and hold
 * the output data for each axis.
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 */
        void getAcceleration(int16_t* x, int16_t* y, int16_t* z);
/** get X-axis accleration measurement.
 * @return 16-bit signed X-axis acceleration value
 */
        int16_t getAccelerationX();
/** get Y-axis accleration measurement.
 * @return 16-bit signed Y-axis acceleration value
 */
        int16_t getAccelerationY();
/** get Z-axis accleration measurement.
 * @return 16-bit signed Z-axis acceleration value
 */
        int16_t getAccelerationZ();
        
    private:
        uint8_t accel_devAddr;   /**< Address of the device. */
        uint8_t buffer[6]; /**< Data buffer */
};




/** Class Driver for the Magnetometer
 */
class LSM303DLHC_Mag {

    public:
/** Default constructor, uses default I2C address.
 * @see LSM303LHC_DEFAULT_ADDRESS
 */
        LSM303DLHC_Mag();
        
/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for more information).
 */
        void initialize();
/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
        bool testConnection();

        // CONFIG_A register
/** set the scale of the measurment.
 * @param Scale desired scale of measurement
 */
        void setGain( lsm303lhc_mag_scale_t Scale);

/** get the scale of the measurment..
 * @return lsm303lhc_mag_scale_t the current scale of measurement.
 */
        lsm303lhc_mag_scale_t getGain();

/** set the mode of the measurment.
 * @param Mode mode of measurement
 */
        void setMode( lsm303lhc_mag_mode_t Mode);

/** get the mode of the measurment.
 * @return lsm303lhc_mag_mode_t the mode of measurement.
 */
        lsm303lhc_mag_mode_t getMode();
 //todo doxygen header
/** set the data rate
 * @param rate the data rate 
 */
        void setDataRate( lsm303lhc_mag_data_rate_t rate );
/** get the current data rate
 * @return lsm303lhc_mag_data_rate_t the data rate
 */
        lsm303lhc_mag_data_rate_t getDataRate( void );   
/** get the data ready flag
 * @return bool the data ready flag
 */
        bool getDataReady();

/** set the update lock flag
 * @param lock enable or disable the lock
 */
        void setUpdateLock( bool lock);

/** get the update lock flag.
 * @return bool update lock flag
 */
        bool getUpdateLock();
        
        void getHeading(int16_t* x, int16_t* y, int16_t* z);
/** get the X measurement.
 * @return int16_t the measurement
 */
        int16_t getHeadingX();        

/** get the Y measurement.
 * @return int16_t the measurement
 */
        int16_t getHeadingY();
        
/** get the Z measurement.
 * @return int16_t the measurement
 */
        int16_t getHeadingZ();
        
/** get the temperature measurement.
 * @return int16_t the measurement
 */
        int16_t getTemperature();
        
// ID_* registers
/** get identification byte A
 * @return ID_A byte (should be 01001000, ASCII value 'H')
 */
        uint8_t getIDA();
        
/** get identification byte B
 * @return ID_A byte (should be 00110100, ASCII value '4')
 */
        uint8_t getIDB();
        
/** get identification byte C
 * @return ID_A byte (should be 00110011, ASCII value '3')
 */
        uint8_t getIDC();

    private:
        uint8_t mag_devAddr;   /**< Address of the device. */
        uint8_t buffer[6]; /**< Data buffer */
};

#endif /* _LSM303LHC_H_ */
