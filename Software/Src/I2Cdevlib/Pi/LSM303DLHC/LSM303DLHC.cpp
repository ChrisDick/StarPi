// I2Cdev library collection - HMC5883L I2C device class
// Based on Honeywell HMC5883L datasheet, 10/2010 (Form #900405 Rev B)
// 6/12/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-12 - fixed swapped Y/Z axes
//     2011-08-22 - small Doxygen comment fixes
//     2011-07-31 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

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

#include "LSM303DLHC.h"
/** Default constructor, uses default I2C address.
 */
LSM303DLHC_Accel::LSM303DLHC_Accel()
{
    accel_devAddr = LSM303_ACC;
}


/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
void LSM303DLHC_Accel::initialize()
{
    setReboot( ENABLE );
    setXEnable( ENABLE );
    setYEnable( ENABLE );
    setZEnable( ENABLE );
    setDataRateSelect( FIFTY_HZ );

//    LSM303_write(0x00, CTRL_REG4_A);

}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool LSM303DLHC_Accel::testConnection()
{
    return true;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setXEnable( lsm303lhc_enable_t enable )
{
    ctrl_reg1_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    Reg.bits.XEnable = (uint8_t)enable;
    I2Cdev::writeByte( accel_devAddr, CTRL_REG1_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getXEnable()
{
    ctrl_reg1_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    
    result = (lsm303lhc_enable_t)Reg.bits.XEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setYEnable( lsm303lhc_enable_t enable )
{
    ctrl_reg1_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    Reg.bits.YEnable = (uint8_t)enable;
    I2Cdev::writeByte( accel_devAddr, CTRL_REG1_A, Reg.all);
}


/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getYEnable()
{
    ctrl_reg1_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    result = ((lsm303lhc_enable_t)Reg.bits.YEnable);
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setZEnable( lsm303lhc_enable_t enable)
{
    ctrl_reg1_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    Reg.bits.ZEnable = (uint8_t)enable;
    I2Cdev::writeByte( accel_devAddr, CTRL_REG1_A, Reg.all);
}


/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getZEnable()
{
    ctrl_reg1_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setLowPowerMode(lsm303lhc_accel_power_mode_t mode)
{
    ctrl_reg1_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    Reg.bits.LowPowerMode = (uint8_t)mode;
    I2Cdev::writeByte( accel_devAddr, CTRL_REG1_A, Reg.all);
}


/** Get
 * @return
 */
lsm303lhc_accel_power_mode_t LSM303DLHC_Accel::getLowPowerMode()
{
    ctrl_reg1_a_t Reg;
    lsm303lhc_accel_power_mode_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    result = (lsm303lhc_accel_power_mode_t)Reg.bits.LowPowerMode;
    return result;
}
/** set
 * @param
 */
void LSM303DLHC_Accel::setDataRateSelect( lsm303lhc_accel_data_rate_t rate )
{
    ctrl_reg1_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    Reg.bits.DataRateSelect = (uint8_t)rate;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG1_A, Reg.all);
}


/** Get
 * @return
 */
lsm303lhc_accel_data_rate_t LSM303DLHC_Accel::getDataRateSelect()
{
    ctrl_reg1_a_t Reg;
    lsm303lhc_accel_data_rate_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG1_A, &Reg.all );
    result = (lsm303lhc_accel_data_rate_t)Reg.bits.DataRateSelect;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInterrupt1HighPassEnable( lsm303lhc_enable_t enable)
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.Interrupt1HighPassEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}


/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInterrupt1HighPassEnable()
{
    ctrl_reg2_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Interrupt1HighPassEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInterrupt2HighPassEnable( lsm303lhc_enable_t enable )
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.Interrupt2HighPassEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInterrupt2HighPassEnable()
{
    ctrl_reg2_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Interrupt2HighPassEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickHighPassEnable( lsm303lhc_enable_t enable )
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.ClickHighPassEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getClickHighPassEnable()
{
    ctrl_reg2_a_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Interrupt2HighPassEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFilterDataSelect( lsm303lhc_accel_filtered_data_t filtered)
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.FilterDataSelect = (uint8_t)filtered;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_filtered_data_t LSM303DLHC_Accel::getFilterDataSelect()
{
    ctrl_reg2_a_t Reg;
    lsm303lhc_accel_filtered_data_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    result = (lsm303lhc_accel_filtered_data_t)Reg.bits.FilterDataSelect;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setHighPassCutOff( uint8_t cutoff)
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.HighPassCutOff = cutoff;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getHighPassCutOff()
{
    ctrl_reg2_a_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    result = Reg.bits.HighPassCutOff;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setHighPassMode( lsm303lhc_accel_high_pass_mode_t mode )
{
    ctrl_reg2_a_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG2_A, &Reg.all );
    Reg.bits.HighPassMode = (uint8_t)mode;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG2_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_high_pass_mode_t LSM303DLHC_Accel::getHighPassMode()
{
    ctrl_reg2_a_t Reg;
    lsm303lhc_accel_high_pass_mode_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_accel_high_pass_mode_t)Reg.bits.HighPassMode;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setOverrun( lsm303lhc_enable_t enable)
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.Overrun = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getOverrun()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Overrun;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setWatermark( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.Watermark = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getWatermark()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Watermark;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setDataReady2( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.DataReady2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getDataReady2()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.DataReady2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setDataReady1( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.DataReady1 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getDataReady1()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.DataReady1;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setAOI1Interrupt( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.AOI1Interrupt = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getAOI1Interrupt()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.AOI1Interrupt;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setAOI2Interrupt( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.AOI2Interrupt = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getAOI2Interrupt()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.AOI2Interrupt;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClick( lsm303lhc_enable_t enable )
{
    interrupt1_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    Reg.bits.Click = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG3_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getClick()
{
    interrupt1_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG3_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Click;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setHighResolutionMode( lsm303lhc_enable_t enable)
{
    accelerometer_contorl_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    Reg.bits.HighResolutionMode = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG4_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getHighResolutionMode()
{
    accelerometer_contorl_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.HighResolutionMode;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFullScaleSelection( lsm303lhc_accel_full_scale_t scale )
{
    accelerometer_contorl_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    Reg.bits.FullScaleSelection = (uint8_t)scale;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG4_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_full_scale_t LSM303DLHC_Accel::getFullScaleSelection()
{
    accelerometer_contorl_t Reg;
    lsm303lhc_accel_full_scale_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    result = (lsm303lhc_accel_full_scale_t)Reg.bits.FullScaleSelection;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setEndianess( lsm303lhc_accel_endianess_t endianess )
{
    accelerometer_contorl_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    Reg.bits.Endianess = (uint8_t)endianess;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG4_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_endianess_t LSM303DLHC_Accel::getEndianess()
{
    accelerometer_contorl_t Reg;
    lsm303lhc_accel_endianess_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    result = (lsm303lhc_accel_endianess_t)Reg.bits.Endianess;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setBlockUpdateMode( lsm303lhc_accell_update_mode_t mode )
{
    accelerometer_contorl_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    Reg.bits.BlockUpdateMode = (uint8_t)mode;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG4_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accell_update_mode_t LSM303DLHC_Accel::getBlockUpdateMode()
{
    accelerometer_contorl_t Reg;
    lsm303lhc_accell_update_mode_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG4_A, &Reg.all );
    result = (lsm303lhc_accell_update_mode_t)Reg.bits.BlockUpdateMode;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFourDEnableInt2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.FourDEnableInt2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getFourDEnableInt2()
{
    lsm303_accel_interrupt_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.FourDEnableInt2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setLatchIntRequest2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.LatchIntRequest2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getLatchIntRequest2()
{
    lsm303_accel_interrupt_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.LatchIntRequest2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFourDEnableInt1( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.FourDEnableInt1 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getFourDEnableInt1()
{
    lsm303_accel_interrupt_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.FourDEnableInt1;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setLatchIntRequest1( lsm303lhc_accel_interrupt_latch_t latch )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.LatchIntRequest2 = (uint8_t)latch;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_interrupt_latch_t LSM303DLHC_Accel::getLatchIntRequest1()
{
    lsm303_accel_interrupt_config_t Reg;
    lsm303lhc_accel_interrupt_latch_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    result = (lsm303lhc_accel_interrupt_latch_t)Reg.bits.LatchIntRequest2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFIFOEnable( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.FIFOEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getFIFOEnable()
{
    lsm303_accel_interrupt_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.FIFOEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setReboot( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG5_A, &Reg.all );
    Reg.bits.Reboot = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG5_A, Reg.all);
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setActiveHigh( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.ActiveHigh = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getActiveHigh()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ActiveHigh;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setActiveFunctionPAD2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.ActiveFunctionPAD2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getActiveFunctionPAD2()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ActiveFunctionPAD2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setRebootPAD2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.RebootPAD2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getRebootPAD2()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.RebootPAD2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2PAD2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.Int2PAD2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2PAD2()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Int2PAD2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1PAD2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.Int1PAD2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1PAD2()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.Int1PAD2;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickPAD2( lsm303lhc_enable_t enable )
{
    lsm303_accel_interrupt_config_2_t Reg;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    Reg.bits.ClickPAD2 = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG6_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getClickPAD2()
{
    lsm303_accel_interrupt_config_2_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CTRL_REG6_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ClickPAD2;
    return result;
}

//#define REFERENCE_A 0x26
/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getXDataAvaliable()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.XDataAvaliable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getYDataAvaliable()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.YDataAvaliable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getZDataAvaliable()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.ZDataAvaliable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getAllDataAvaliable()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.AllDataAvaliable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getXOverrun()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.XOverrun;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getYOverrun()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.YOverrun;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getZOverrun()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.ZOverrun;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_avaliable_t LSM303DLHC_Accel::getAllOverrun()
{
    accel_status_t Reg;
    lsm303lhc_accel_avaliable_t result;
    I2Cdev::readByte( accel_devAddr, STATUS_REG_A, &Reg.all );
    result = (lsm303lhc_accel_avaliable_t)Reg.bits.AllOverrun;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setTriggerSelect( lsm303lhc_accel_trigger_mode_t mode )
{
    fifo_control_t Reg;
    I2Cdev::readByte( accel_devAddr, FIFO_CTRL_REG_A, &Reg.all );
    Reg.bits.TriggerSelect = (uint8_t)mode;
    I2Cdev::writeByte(  accel_devAddr, FIFO_CTRL_REG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_trigger_mode_t LSM303DLHC_Accel::getTriggerSelect()
{
    fifo_control_t Reg;
    lsm303lhc_accel_trigger_mode_t result;
    I2Cdev::readByte( accel_devAddr, FIFO_CTRL_REG_A, &Reg.all );
    result = (lsm303lhc_accel_trigger_mode_t)Reg.bits.TriggerSelect;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setFIFOMode( lsm303lhc_accel_fifo_mode_t mode )
{
    fifo_control_t Reg;
    I2Cdev::readByte( accel_devAddr, FIFO_CTRL_REG_A, &Reg.all );
    Reg.bits.FIFOMode = (uint8_t)mode;
    I2Cdev::writeByte(  accel_devAddr, FIFO_CTRL_REG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_accel_fifo_mode_t LSM303DLHC_Accel::getFIFOMode()
{
    fifo_control_t Reg;
    lsm303lhc_accel_fifo_mode_t result;
    I2Cdev::readByte( accel_devAddr, FIFO_CTRL_REG_A, &Reg.all );
    result = (lsm303lhc_accel_fifo_mode_t)Reg.bits.FIFOMode;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1XLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.XLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1XLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1XHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.XHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CTRL_REG4_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1XHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XHighEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1YLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.YLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1YLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1YHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.YHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1YHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YHighEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1ZLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.ZLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1ZLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1ZHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.ZHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1ZHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZHighEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1TypeEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    Reg.bits.TypeEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt1TypeEnable()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT1_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.TypeEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2XLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.XLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2XLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2XHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.XHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2XHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XHighEvent;
    return result;
}


/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2YLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.YLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2YLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2YHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.YHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT1_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2YHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YHighEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2ZLowEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.ZLowEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2ZLowEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZLowEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2ZHighEvent( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.ZHighEvent = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2ZHighEvent()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZHighEvent;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2TypeEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_int_config_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    Reg.bits.TypeEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, INT2_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getInt2TypeEnable()
{
    lsm303lhc_accel_int_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, INT2_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.TypeEnable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1XLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.XLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1XHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.XHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1YLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.YLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1YHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.YHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1ZLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.ZLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1ZHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.ZHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt1InterruptActive()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT1_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.InterruptActive;
    return result;
}


/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2XLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.XLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2XHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.XHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2YLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.YLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2YHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.YHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2ZLow()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.ZLow;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2ZHigh()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.ZHigh;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInt2InterruptActive()
{
    lsm303lhc_accel_int_source_t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, INT2_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.InterruptActive;
    return result;
}


/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1Threshold( uint8_t Threshold )
{
    lsm303lhc_accel_int_threshold_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_THS_A, &Reg.all );
    Reg.bits.Threshold = (uint8_t)Threshold;
    I2Cdev::writeByte(  accel_devAddr, INT1_THS_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getInt1Threshold()
{
    lsm303lhc_accel_int_threshold_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, INT1_THS_A, &Reg.all );
    result = (uint8_t)Reg.bits.Threshold;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2Threshold( uint8_t Threshold )
{
    lsm303lhc_accel_int_threshold_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_THS_A, &Reg.all );
    Reg.bits.Threshold = (uint8_t)Threshold;
    I2Cdev::writeByte(  accel_devAddr, INT2_THS_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getInt2Threshold()
{
    lsm303lhc_accel_int_threshold_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, INT2_THS_A, &Reg.all );
    result = (uint8_t)Reg.bits.Threshold;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickThreshold( uint8_t Threshold )
{
    lsm303lhc_accel_int_threshold_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_THS_A, &Reg.all );
    Reg.bits.Threshold = (uint8_t)Threshold;
    I2Cdev::writeByte(  accel_devAddr, CLICK_THS_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getClickThreshold()
{
    lsm303lhc_accel_int_threshold_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_THS_A, &Reg.all );
    result = (uint8_t)Reg.bits.Threshold;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt1Duration( uint8_t duration )
{
    lsm303lhc_accel_int_duration_t Reg;
    I2Cdev::readByte( accel_devAddr, INT1_DURATION_A, &Reg.all );
    Reg.bits.Duration = (uint8_t)duration;
    I2Cdev::writeByte(  accel_devAddr, INT1_DURATION_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getInt1Duration()
{
    lsm303lhc_accel_int_duration_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, INT1_DURATION_A, &Reg.all );
    result = (uint8_t)Reg.bits.Duration;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setInt2Duration( uint8_t duration )
{
    lsm303lhc_accel_int_duration_t Reg;
    I2Cdev::readByte( accel_devAddr, INT2_DURATION_A, &Reg.all );
    Reg.bits.Duration = (uint8_t)duration;
    I2Cdev::writeByte(  accel_devAddr, INT2_DURATION_A, Reg.all);
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getInt2Duration()
{
    lsm303lhc_accel_int_duration_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, INT2_DURATION_A, &Reg.all );
    result = (uint8_t)Reg.bits.Duration;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setXSingleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.XSingleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getXSingleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XSingleEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setXDoubleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.XDoubleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getXDoubleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.XDoubleEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setYSingleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.YSingleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getYSingleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YSingleEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setYDoubleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.YDoubleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getYDoubleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.YDoubleEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setZSingleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.ZSingleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getZSingleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZSingleEnable;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setZDoubleEnable( lsm303lhc_enable_t enable )
{
    lsm303lhc_accel_click_config_t Reg;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    Reg.bits.ZDoubleEnable = (uint8_t)enable;
    I2Cdev::writeByte(  accel_devAddr, CLICK_CFG_A, Reg.all);
}

/** Get
 * @return
 */
lsm303lhc_enable_t LSM303DLHC_Accel::getZDoubleEnable()
{
    lsm303lhc_accel_click_config_t Reg;
    lsm303lhc_enable_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_CFG_A, &Reg.all );
    result = (lsm303lhc_enable_t)Reg.bits.ZDoubleEnable;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getXClick()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.XClick;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getYClick()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.YClick;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getZClick()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.ZClick;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getSign()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.Sign;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getSingle()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.Single;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getDouble()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.Double;
    return result;
}

/** Get
 * @return
 */
lsm303lhc_accel_event_active_t LSM303DLHC_Accel::getInterruptActive()
{
    lsm303lhc_accel__t Reg;
    lsm303lhc_accel_event_active_t result;
    I2Cdev::readByte( accel_devAddr, CLICK_SRC_A, &Reg.all );
    result = (lsm303lhc_accel_event_active_t)Reg.bits.InterruptActive;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickLimit( uint8_t Limit )
{
    lsm303lhc_accel_time_limit_t Reg;
    Reg.bits.Limit = Limit;
    I2Cdev::writeByte(  accel_devAddr, TIME_LIMIT_A, Reg.all );
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getClickLimit()
{
    lsm303lhc_accel_time_limit_t Reg;
    uint8_t result;
    I2Cdev::readByte( accel_devAddr, TIME_LIMIT_A, &Reg.all );
    result = Reg.bits.Limit;
    return result;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickLatency( uint8_t Latency )
{
    I2Cdev::writeByte(  accel_devAddr, TIME_LATENCY_A, Latency );
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getClickLatency()
{
    uint8_t Reg;
    I2Cdev::readByte( accel_devAddr, TIME_LATENCY_A, &Reg );
    return Reg;
}

/** set
 * @param
 */
void LSM303DLHC_Accel::setClickWindow( uint8_t window )
{
    I2Cdev::writeByte(  accel_devAddr, TIME_WINDOW_A, window );
}

/** Get
 * @return
 */
uint8_t LSM303DLHC_Accel::getClickWindow()
{
    uint8_t Reg;
    I2Cdev::readByte( accel_devAddr, TIME_WINDOW_A, &Reg );
    return Reg;
}

/** Get 3-axis accleration measurements.
 * These six bytes are eight bits each and hold
 * the output data for each axis.
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 */
void LSM303DLHC_Accel::getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
    I2Cdev::readBytes(  accel_devAddr, OUT_X_L_A, 6, buffer);
    *x = (((int16_t)buffer[1]) << 8) | buffer[0];
    *y = (((int16_t)buffer[3]) << 8) | buffer[2];
    *z = (((int16_t)buffer[5]) << 8) | buffer[4];
}

/** Get X-axis accleration measurement.
 * @return 16-bit signed X-axis acceleration value
 */
int16_t LSM303DLHC_Accel::getAccelerationX()
{
    I2Cdev::readBytes(  accel_devAddr, OUT_X_L_A, 2, buffer);
    return (((int16_t)buffer[1]) << 8) | buffer[0];
}

/** Get Y-axis accleration measurement.
 * @return 16-bit signed Y-axis acceleration value
 */
int16_t LSM303DLHC_Accel::getAccelerationY()
{
    I2Cdev::readBytes(  accel_devAddr, OUT_Y_L_A, 2, buffer);
    return (((int16_t)buffer[1]) << 8) | buffer[0];
}

/** Get Z-axis accleration measurement.
 * @return 16-bit signed Z-axis acceleration value
 */
int16_t LSM303DLHC_Accel::getAccelerationZ()
{
    I2Cdev::readBytes(  accel_devAddr, OUT_Z_L_A, 2, buffer);
    return (((int16_t)buffer[1]) << 8) | buffer[0];
}


 /*
===========================================================================================
        Magneto starts here:
 ==========================================================================================
 */


/** Default constructor, uses default I2C address.
 * @see LSM303LHC_DEFAULT_ADDRESS
 */
LSM303DLHC_Mag::LSM303DLHC_Mag()
{
    mag_devAddr = LSM303_MAG;
}

/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for more information).
 */
void LSM303DLHC_Mag::initialize()
{
//    if ( testConnection() )
    {
        setGain( MAG_SCALE_1_9 );
        setMode( CONTINUOUS );
        setDataRate( RATE_220_0_HZ );
        
    }
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool LSM303DLHC_Mag::testConnection()
{
    if (
         ( getIDA() == 0x48 )
       &&( getIDB() == 0x34 )
       &&( getIDC() == 0x33 )
       )
       {
    return true;
    }
    return false;
    //ToDo
}

        // CONFIG_A register
/** set the scale of the measurment.
 * @param Scale desired scale of measurement
 */
void LSM303DLHC_Mag::setGain( lsm303lhc_mag_scale_t Scale )
{
    lsm303lhc_mag_config_b_t Reg;
    I2Cdev::readByte( mag_devAddr, CRB_REG_M, &Reg.all );
    Reg.bits.Gain = (uint8_t)Scale;
    I2Cdev::writeByte( mag_devAddr, CRB_REG_M, Reg.all );
}

/** Get the scale of the measurment..
 * @return lsm303lhc_mag_scale_t the current scale of measurement.
 */
lsm303lhc_mag_scale_t LSM303DLHC_Mag::getGain()
{
    lsm303lhc_mag_config_b_t Reg;
    lsm303lhc_mag_scale_t result;
    I2Cdev::readByte( mag_devAddr, CRB_REG_M, &Reg.all );
    result = (lsm303lhc_mag_scale_t)Reg.bits.Gain;
    return result;
}

/** set the mode of the measurment.
 * @param Mode mode of measurement
 */
void LSM303DLHC_Mag::setMode( lsm303lhc_mag_mode_t Mode )
{
    lsm303lhc_mag_mode_select_t Reg;
    I2Cdev::readByte( mag_devAddr, MR_REG_M, &Reg.all );
    Reg.bits.Mode = (uint8_t)Mode;
    I2Cdev::writeByte( mag_devAddr, MR_REG_M, Reg.all );
}

/** Get the mdoe of the measurment.
 * @return lsm303lhc_mag_mode_t the mode of measurement.
 */
lsm303lhc_mag_mode_t LSM303DLHC_Mag::getMode()
{
    lsm303lhc_mag_mode_select_t Reg;
    lsm303lhc_mag_mode_t result;
    I2Cdev::readByte( mag_devAddr, MR_REG_M, &Reg.all );
    result = (lsm303lhc_mag_mode_t)Reg.bits.Mode;
    return result;
}

/** Get the data ready flag
 * @return bool the data ready flag
 */
bool LSM303DLHC_Mag::getDataReady()
{
    return true;
    //ToDo
}

/** set the update lock flag
 * @param lock enable or disable the lock
 */
void LSM303DLHC_Mag::setUpdateLock( bool lock)
{
    (void)lock;//ToDo
}

/** Get the update lock flag.
 * @return bool update lock flag
 */
bool LSM303DLHC_Mag::getUpdateLock()
{
    return true;
    //ToDo
}

/** Get 3-axis accleration measurements.
 * These six bytes are eight bits each and hold
 * the output data for each axis.
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 */
void LSM303DLHC_Mag::getHeading(int16_t* x, int16_t* y, int16_t* z)
{
    I2Cdev::readBytes( mag_devAddr, OUT_X_H_M, 6, buffer );
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}


/** Get the X measurement.
 * @return int16_t the measurement
 */
int16_t LSM303DLHC_Mag::getHeadingX()
{
    I2Cdev::readBytes( mag_devAddr, OUT_X_H_M, 2, buffer );
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}


/** Get the Y measurement.
 * @return int16_t the measurement
 */
int16_t LSM303DLHC_Mag::getHeadingY()
{
    I2Cdev::readBytes( mag_devAddr, OUT_Y_H_M, 2, buffer );
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}


/** Get the Z measurement.
 * @return int16_t the measurement
 */
int16_t LSM303DLHC_Mag::getHeadingZ()
{
    I2Cdev::readBytes( mag_devAddr, OUT_Z_H_M, 2, buffer );
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}


/** Get the temperature measurement.
 * @return int16_t the measurement
 */
int16_t LSM303DLHC_Mag::getTemperature()
{
    I2Cdev::readBytes( mag_devAddr, TEMP_OUT_H_M, 2, buffer );
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}


// ID_* registers
/** Get identification byte A
 * @return ID_A byte (should be 01001000, ASCII value 'H')
 */
uint8_t LSM303DLHC_Mag::getIDA()
{
    I2Cdev::readByte( mag_devAddr, IRA_REG_M, buffer );
    return buffer[0];
}


/** Get identification byte B
 * @return ID_A byte (should be 00110100, ASCII value '4')
 */
uint8_t LSM303DLHC_Mag::getIDB()
{
    I2Cdev::readByte( mag_devAddr, IRC_REG_M, buffer );
    return buffer[0];
}


/** Get identification byte C
 * @return ID_A byte (should be 00110011, ASCII value '3')
 */
uint8_t LSM303DLHC_Mag::getIDC()
{
    I2Cdev::readByte( mag_devAddr, IRC_REG_M, buffer );
    return buffer[0];
}

lsm303lhc_mag_data_rate_t LSM303DLHC_Mag::getDataRate( void )
{
    lsm303lhc_mag_config_a_t Reg;
    lsm303lhc_mag_data_rate_t result;
    I2Cdev::readByte( mag_devAddr, MR_REG_M, &Reg.all );
    result = (lsm303lhc_mag_data_rate_t)Reg.bits.DataRate;
    return result;
}

void LSM303DLHC_Mag::setDataRate( lsm303lhc_mag_data_rate_t rate )
{
    lsm303lhc_mag_config_a_t Reg;
    I2Cdev::readByte( mag_devAddr, MR_REG_M, &Reg.all );
    Reg.bits.DataRate = rate;
    I2Cdev::writeByte( mag_devAddr, MR_REG_M, Reg.all );
}
