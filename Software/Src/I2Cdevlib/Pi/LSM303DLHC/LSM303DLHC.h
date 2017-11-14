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

union ctrl_reg1_a_t
{
    uint8_t all;
    
    struct {
        uint8_t XEnable        :1;
        uint8_t YEnable        :1;
        uint8_t ZEnable        :1;
        uint8_t LowPowerMode   :1;
        uint8_t DataRateSelect :4;
    } bits;
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

union ctrl_reg2_a_t
{
    uint8_t all;
    
    struct {
        uint8_t Interrupt1HighPassEnable    :1;
        uint8_t Interrupt2HighPassEnable    :1;
        uint8_t ClickHighPassEnable         :1;
        uint8_t FilterDataSelect            :1;
        uint8_t HighPassCutOff              :2;
        uint8_t HighPassMode                :2;
    } bits;
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

union interrupt1_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Overrun       :1;
        uint8_t Watermark     :1;
        uint8_t DataReady2    :1;
        uint8_t DataReady1    :1;
        uint8_t AOI1Interrupt :1;
        uint8_t AOI2Interrupt :1;
        uint8_t Click         :1;
    } bits;
};


/* Accelerometer configuration register */
#define CTRL_REG4_A 0x23

union accelerometer_contorl_t
{
    uint8_t all;
    
    struct 
    {
        uint8_t SpiMode              :1;
        uint8_t empty                :2;
        uint8_t HighResolutionMode   :1;
        uint8_t FullScaleSelection   :2;
        uint8_t Endianess            :1;
        uint8_t BlockUpdateMode      :1;
    } bits;
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

union lsm303_accel_interrupt_config_t
{
    uint8_t all;
    
    struct
    {
        uint8_t FourDEnableInt2   :1;
        uint8_t LatchIntRequest2  :1;
        uint8_t FourDEnableInt1   :1;
        uint8_t LatchIntRequest1  :1;
        uint8_t Empty             :2;
        uint8_t FIFOEnable        :1;
        uint8_t Reboot            :1;
    } bits;
};

typedef enum
{
    NOT_LATCHED,
    LATCHED
} lsm303lhc_accel_interrupt_latch_t;

#define CTRL_REG6_A 0x25

union lsm303_accel_interrupt_config_2_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Empty               :1;
        uint8_t ActiveHigh          :1;
        uint8_t Reserved            :1; 
        uint8_t ActiveFunctionPAD2  :1;
        uint8_t RebootPAD2          :1;
        uint8_t Int2PAD2            :1;
        uint8_t Int1PAD2            :1;
        uint8_t ClickPAD2           :1;
    } bits;
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

union accel_status_t
{
    uint8_t all;
    
    struct
    {
        uint8_t XDataAvaliable    :1;
        uint8_t YDataAvaliable    :1;
        uint8_t ZDataAvaliable    :1;
        uint8_t AllDataAvaliable  :1;
        uint8_t XOverrun          :1;
        uint8_t YOverrun          :1;
        uint8_t ZOverrun          :1;
        uint8_t AllOverrun        :1;
    } bits;
};

typedef enum
{
    NOT_AVALIABLE,
    AVAILIABLE
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

union fifo_control_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Reserved      :5;
        uint8_t TriggerSelect :1;
        uint8_t FIFOMode      :1;
    } bits;
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

union lsm303lhc_accel_int_config_t
{
    uint8_t all;
    
    struct
    {
        uint8_t XLowEvent      :1;
        uint8_t XHighEvent     :1;
        uint8_t YLowEvent      :1;
        uint8_t YHighEvent     :1;
        uint8_t ZLowEvent      :1;
        uint8_t ZHighEvent     :1;
        uint8_t TypeEnable     :2;
    } bits;
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
union lsm303lhc_accel_int_source_t
{
    uint8_t all;
    
    struct
    {
        uint8_t XLow             :1 ;
        uint8_t XHigh            :1 ;
        uint8_t YLow             :1 ;
        uint8_t YHigh            :1 ;
        uint8_t ZLow             :1 ;
        uint8_t ZHigh            :1 ;
        uint8_t InterruptActive  :1 ;
        uint8_t Zero             :1 ;
    } bits;
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

union lsm303lhc_accel_int_threshold_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Threshold   :7;
        uint8_t Zero        :1;
    } bits;
};

/* Interrupt duration rgister */
/* Duration of event for interrupt */
#define INT1_DURATION_A 0x33
#define INT2_DURATION_A 0x37

union lsm303lhc_accel_int_duration_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Duration   :7;
        uint8_t Zero        :1;
    } bits;
};

/* Click event config */
#define CLICK_CFG_A    0x38

union lsm303lhc_accel_click_config_t
{
    uint8_t all;
    
    struct
    {
        uint8_t XSingleEnable   :1;
        uint8_t XDoubleEnable   :1;
        uint8_t YSingleEnable   :1;
        uint8_t YDoubleEnable   :1;
        uint8_t ZSingleEnable   :1;
        uint8_t ZDoubleEnable   :1;
        uint8_t Empty   :2;
    } bits;
};

/* Click event source */
#define CLICK_SRC_A    0x39

union lsm303lhc_accel__t
{
    uint8_t all;
    
    struct
    {
        uint8_t XClick            :1;
        uint8_t YClick            :1;
        uint8_t ZClick            :1;
        uint8_t Sign              :1;
        uint8_t Single            :1;
        uint8_t Double            :1;
        uint8_t InterruptActive   :1;
    } bits;
};

/* Time limit between click down and click up register */
#define TIME_LIMIT_A   0x3B
union lsm303lhc_accel_time_limit_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Limit   :7;
        uint8_t Empty   :1;
    } bits;
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
union lsm303lhc_mag_config_a_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Empty                   :2;
        uint8_t DataRate                :3;
        uint8_t Reserved                :2;
        uint8_t TemperatureSensorEnable :1;
    } bits;
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
union lsm303lhc_mag_config_b_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Empty   :5;
        uint8_t Gain    :3;
    } bits;
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
union lsm303lhc_mag_mode_select_t
{
    uint8_t all;
    
    struct
    {
        uint8_t Mode  :2;
        uint8_t Empty :6;
    } bits;
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
union lsm303lhc_mag_sr_config_t
{
    uint8_t all;
    
    struct
    {
        uint8_t DataReady    :1;
        uint8_t UpdateLock   :1;
        uint8_t Empty        :6;
    } bits;
};

/* who am I? */
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C

/* Temperature output registers */
#define TEMP_OUT_H_M 0x31
#define TEMP_OUT_L_M 0x32

union lsm303lhc_mag_temperature_t
{
    uint16_t all;
    
    struct
    {
        uint8_t Lsb;
        uint8_t Msb;
    } byte;
    
    struct 
    {
        uint16_t Empty       :4;
        uint16_t Temperature :12; /**> temperature in 8lsb/degree */
    } bits;
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
 * @param
 */
        void setXEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getXEnable();
/** set 
 * @param
 */
        void setYEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getYEnable();
/** set 
 * @param
 */
        void setZEnable( lsm303lhc_enable_t enable);
/** get 
 * @return
 */
        lsm303lhc_enable_t getZEnable();
/** set 
 * @param
 */
        void setLowPowerMode(lsm303lhc_accel_power_mode_t mode);
/** get 
 * @return
 */
        lsm303lhc_accel_power_mode_t getLowPowerMode();
/** set 
 * @param
 */
        void setDataRateSelect( lsm303lhc_accel_data_rate_t rate );
/** get 
 * @return
 */
        lsm303lhc_accel_data_rate_t getDataRateSelect();
/** set 
 * @param
 */
        void setInterrupt1HighPassEnable( lsm303lhc_enable_t enable);
/** get 
 * @return
 */
        lsm303lhc_enable_t getInterrupt1HighPassEnable();
/** set 
 * @param
 */
        void setInterrupt2HighPassEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInterrupt2HighPassEnable();
/** set 
 * @param
 */
        void setClickHighPassEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getClickHighPassEnable();
/** set 
 * @param
 */
        void setFilterDataSelect( lsm303lhc_accel_filtered_data_t filtered);
/** get 
 * @return
 */
        lsm303lhc_accel_filtered_data_t getFilterDataSelect();
/** set 
 * @param
 */
        void setHighPassCutOff( uint8_t cutoff);
/** get 
 * @return
 */
        uint8_t getHighPassCutOff();
/** set 
 * @param
 */
        void setHighPassMode( lsm303lhc_accel_high_pass_mode_t mode );
/** get 
 * @return
 */
        lsm303lhc_accel_high_pass_mode_t getHighPassMode();
/** set 
 * @param
 */
        void setOverrun( lsm303lhc_enable_t enable);
/** get 
 * @return
 */
        lsm303lhc_enable_t getOverrun();
/** set 
 * @param
 */
        void setWatermark( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getWatermark();
/** set 
 * @param
 */
        void setDataReady2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getDataReady2();
/** set 
 * @param
 */
        void setDataReady1( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getDataReady1();
/** set 
 * @param
 */
        void setAOI1Interrupt( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getAOI1Interrupt();
/** set 
 * @param
 */
        void setAOI2Interrupt( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getAOI2Interrupt();
/** set 
 * @param
 */
        void setClick( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
         lsm303lhc_enable_t getClick( );
/** set 
 * @param
 */
        void setHighResolutionMode( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
         lsm303lhc_enable_t getHighResolutionMode();
/** set 
 * @param
 */
        void setFullScaleSelection( lsm303lhc_accel_full_scale_t scale );
/** get 
 * @return
 */
        lsm303lhc_accel_full_scale_t getFullScaleSelection();
/** set 
 * @param
 */
        void setEndianess( lsm303lhc_accel_endianess_t endianess);
/** get 
 * @return
 */
        lsm303lhc_accel_endianess_t getEndianess();
/** set 
 * @param
 */
        void setBlockUpdateMode( lsm303lhc_accell_update_mode_t mode );
/** get 
 * @return
 */
        lsm303lhc_accell_update_mode_t getBlockUpdateMode();
/** set 
 * @param
 */
        void setBlockUpdateMode();
/** get 
 * @return
 */
        lsm303lhc_enable_t getFourDEnableInt2();
/** set 
 * @param
 */
        void setFourDEnableInt2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getLatchIntRequest2();
/** set 
 * @param
 */
        void setLatchIntRequest2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getFourDEnableInt1();
/** set 
 * @param
 */
        void setFourDEnableInt1( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_accel_interrupt_latch_t getLatchIntRequest1();
/** set 
 * @param
 */
        void setLatchIntRequest1( lsm303lhc_accel_interrupt_latch_t latch );
/** get 
 * @return
 */
        lsm303lhc_enable_t getFIFOEnable();
/** set 
 * @param
 */
        void setFIFOEnable( lsm303lhc_enable_t enable );
/** set 
 * @param
 */
        void setReboot( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getActiveHigh();
/** set 
 * @param
 */
        void setActiveHigh( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getActiveFunctionPAD2();
/** set 
 * @param
 */
        void setActiveFunctionPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getRebootPAD2();
/** set 
 * @param
 */
        void setRebootPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2PAD2();
/** set 
 * @param
 */
        void setInt2PAD2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1PAD2();
/** set 
 * @param
 */
        void setInt1PAD2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getClickPAD2();
/** set 
 * @param
 */
        void setClickPAD2( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getXDataAvaliable();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getYDataAvaliable();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getZDataAvaliable();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getAllDataAvaliable();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getXOverrun();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getYOverrun();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getZOverrun();
/** get 
 * @return
 */
        lsm303lhc_accel_avaliable_t getAllOverrun();
/** get 
 * @return
 */
        lsm303lhc_accel_trigger_mode_t getTriggerSelect();
/** set 
 * @param
 */
        void setTriggerSelect( lsm303lhc_accel_trigger_mode_t mode );
/** get 
 * @return
 */
        lsm303lhc_accel_fifo_mode_t getFIFOMode();
/** set 
 * @param
 */
        void setFIFOMode( lsm303lhc_accel_fifo_mode_t mode );

/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1XLowEvent();
/** set 
 * @param
 */
        void setInt1XLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1XHighEvent();
/** set 
 * @param
 */
        void setInt1XHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1YLowEvent();
/** set 
 * @param
 */
        void setInt1YLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1YHighEvent();
/** set 
 * @param
 */
        void setInt1YHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1ZLowEvent();
/** set 
 * @param
 */
        void setInt1ZLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1ZHighEvent();
/** set 
 * @param
 */
        void setInt1ZHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt1TypeEnable();
/** set 
 * @param
 */
        void setInt1TypeEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1XLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1XHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1YLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1YHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1ZLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1ZHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt1InterruptActive();

/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2XLowEvent();
/** set 
 * @param
 */
        void setInt2XLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2XHighEvent();
/** set 
 * @param
 */
        void setInt2XHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2YLowEvent();
/** set 
 * @param
 */
        void setInt2YLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2YHighEvent();
/** set 
 * @param
 */
        void setInt2YHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2ZLowEvent();
/** set 
 * @param
 */
        void setInt2ZLowEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2ZHighEvent();
/** set 
 * @param
 */
        void setInt2ZHighEvent( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getInt2TypeEnable();
/** set 
 * @param
 */
        void setInt2TypeEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2XLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2XHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2YLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2YHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2ZLow();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2ZHigh();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInt2InterruptActive();        
        
/** get 
 * @return
 */
        uint8_t getInt1Threshold();
/** set 
 * @param
 */
        void setInt1Threshold( uint8_t Threshold );
/** get 
 * @return
 */
        uint8_t getInt2Threshold();
/** set 
 * @param
 */
        void setInt2Threshold( uint8_t Threshold );
/** get 
 * @return
 */
        uint8_t getClickThreshold();
/** set 
 * @param
 */
        void setClickThreshold( uint8_t Threshold );
/** get 
 * @return
 */
        uint8_t getInt1Duration();
/** set 
 * @param
 */
        void setInt1Duration( uint8_t duration );
/** get 
 * @return
 */
        uint8_t getInt2Duration();
/** set 
 * @param
 */
        void setInt2Duration( uint8_t duration );
/** get 
 * @return
 */
        lsm303lhc_enable_t getXSingleEnable();
/** set 
 * @param
 */
        void setXSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getXDoubleEnable();
/** set 
 * @param
 */
        void setXDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getYSingleEnable();
/** set 
 * @param
 */
        void setYSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getYDoubleEnable();
/** set 
 * @param
 */
        void setYDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getZSingleEnable();
/** set 
 * @param
 */
        void setZSingleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_enable_t getZDoubleEnable();
/** set 
 * @param
 */
        void setZDoubleEnable( lsm303lhc_enable_t enable );
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getXClick();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getYClick();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getZClick();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getSign();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getSingle();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getDouble();
/** get 
 * @return
 */
        lsm303lhc_accel_event_active_t getInterruptActive();
/** get 
 * @return
 */
        uint8_t getClickLimit();
/** set 
 * @param
 */
        void setClickLimit( uint8_t limit );
/** get 
 * @return
 */
        uint8_t getClickLatency();
/** set 
 * @param
 */
        void setClickLatency( uint8_t Latency );
/** get 
 * @return
 */
        uint8_t getClickWindow();
/** set 
 * @param
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
        void setDataRate( lsm303lhc_mag_data_rate_t rate );
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
