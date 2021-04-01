#-*- coding: utf-8 -*-
""" file DFRobot_TSL2541.py
  # DFRobot_TSL2541 Class infrastructure, implementation of underlying methods
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author      [LuoYufeng]<yufeng.luo@dfrobot.com> 
  @version  V1.0
  @date  2021-01-26
  @get from https://www.dfrobot.com
  @url https://github.com/DFRobot/DFRobot_TSL2541
"""
import sys
sys.path.append('../')
import smbus
import time

class DFRobot_TSL2541:
    DFRobot_TSL2541_IIC_ADDR           = 0x39
    
    '''
     * Enable Register (ENABLE 0x80)
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |                  reversed                 |   WEN    | reserved |    AEN    |   PON    |
     * ------------------------------------------------------------------------------------------
     *
    '''
    TSL2541_REG_ENABLE                 = 0x80
    TSL2541_REG_ATIME                  = 0x81
    TSL2541_REG_WTIME                  = 0x83
    TSL2541_REG_AILTL                  = 0x84
    TSL2541_REG_AILTH                  = 0x85
    TSL2541_REG_AIHTL                  = 0x86
    TSL2541_REG_AIHTH                  = 0x87
    TSL2541_REG_PERS                   = 0x8C
    '''
     * CFG0 Register (0x8D)
     * ------------------------------------------------------------------------------------------------
     * |    b7       |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------------
     * | reversed<1> |                  reversed                    |   WLONG  |       reversed       |
     * ------------------------------------------------------------------------------------------------
     * Reserved: Write as 0.
     * Reserved<1>: Write as 1.
     * Wait Long: When asserted, the wait cycles are increased by a factor 12× from that programmed in the WTIME register.
    '''
    TSL2541_REG_CFG0                   = 0x8D
    '''
     * CFG1 Register (0x90) 
     * ---------------------------------------------------------------------------------------------
     * |    b7    |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ---------------------------------------------------------------------------------------------
     * |                               reversed                             |         AGAIN        |
     * ---------------------------------------------------------------------------------------------
     * Reserved: Write as 0.
     * AMUX:ALS Multiplexer. Sets the CH3 input. Default = 0 (X Channel). Set to 1 to read IR2.
     * AGAIN: ALS Gain Control. Sets the gain of the ALS DAC.
     * ----------------------------------------------------------
     * | Field Value |            ALS GAIN VALUE                |
     * ----------------------------------------------------------
     * |     00      |               1X Gain                    |
     * ----------------------------------------------------------
     * |     01      |               4X Gain                    |
     * ----------------------------------------------------------
     * |     10      |               16X Gain                   |
     * ----------------------------------------------------------
     * |     11      |               64X Gain                   |
     * ----------------------------------------------------------
    '''
    TSL2541_REG_CFG1                   = 0x90
    
    TSL2541_REG_REVID                  = 0x91
    TSL2541_REG_ID                     = 0x92
    '''
     * Status Register (STATUS 0x93)
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |   ASAT   |       Reserved      |    AINT  |   CINT   |       Reserved                  |
     * ------------------------------------------------------------------------------------------
     * ASAT: ALS Saturation. This flag is set for analog saturation, writing a 1 will clear this status flag.
     * Reserved:Reserved.
     * AINT:ALS Interrupt. Indicates that the device is asserting an ALS interrupt, writing a 1 will clear this status flag.
     * CINT:The Calibration Interrupt flag indicates that calibration has completed.
    '''
    TSL2541_REG_STATUS                 = 0x93
    TSL2541_REG_VISDATAL               = 0x94
    TSL2541_REG_VISDATAH               = 0x95
    TSL2541_REG_IRDATAL                = 0x96
    TSL2541_REG_IRDATAH                = 0x97
    TSL2541_REG_REVID2                 = 0x9E
    '''
     * CFG2 Register (0x9F)
     * ---------------------------------------------------------------------------------------------
     * |    b7    |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ---------------------------------------------------------------------------------------------
     * |           Reserved(000)           | AGAINMAX | Reserved |  AGAINL  |     Reserved(00)     |
     * ---------------------------------------------------------------------------------------------
     * The ALS gain can be adjusted by setting the two AGAIN bits as well as the AGAINMAX and AGAINL bits 
     * which yields an overall range from 1/2x to 128x.
    '''
    TSL2541_REG_CFG2                   = 0x9F
    '''
     * CFG3 Register (0xAB)
     * ----------------------------------------------------------------------------------------------------
     * |        b7          |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ----------------------------------------------------------------------------------------------------
     * |   INT_READ_CLEAR   |     Reserved(0)     |    SAI   |           Reserved(any value)              |
     * ----------------------------------------------------------------------------------------------------
     * INT_READ_CLEAR:If this bit is set, all flag bits in the STATUS register will be reset whenever the STATUS register is read over I2C.
     * SAI:Sleep After Interrupt. Power down the device at the end of the ALS cycle if an interrupt has been generated.
    '''
    TSL2541_REG_CFG3                   = 0xAB
    '''
     * AZ_CONFIG Register (0xD6)
     * ---------------------------------------------------------------------------------------------
     * |     b7    |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0   |
     * ---------------------------------------------------------------------------------------------
     * |  AZ_MODE  |                                 AZ_NTH_ITERATION                              |
     * ---------------------------------------------------------------------------------------------
     * AZ_MODE: 0, Always start at zero when searching the best offset value
     *          1, Always start at the previous (offset_c) with the auto-zero mechanism
     * AZ_NTH_ITERATION : Run autozero automatically every nth ALS iteration
     *                    (0=never, 7Fh=only at first ALS cycle, n=every nth time)
    '''
    TSL2541_REG_AZ_CONFIG              = 0xD6
    
    '''
     * INTENAB Register (0xDD)
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |  ASIEN   |     Reserved(0)     |   AIEN   |                 Reserved(0)                |
     * ------------------------------------------------------------------------------------------
     * ASIEN: Writing '1' to this bit enables A
     
     
     
     
     SAT interrupt.
     * AIEN: Writing '1' to this bit enables ALS interrupt.
    '''
    TSL2541_REG_INTENAB                = 0xDD
    TSL2541_REG_VISADJ                 = 0xE6
    
    DEVICE_ID                          = 0xE4
    REVISION_ID                        = 0x61
    CONFIG_NO_WLONG                    = 0x80
    CONFIG_WLONG                       = 0x84
    
    ''' 
        @brief  Module init
        @param  bus  Set to IICBus
    '''
    def __init__(self,bus = 1):
        self.i2cbus=smbus.SMBus(bus)
        self.i2c_addr = self.DFRobot_TSL2541_IIC_ADDR
    
    ''' 
        @brief  Initialize the device and turn it on
        @return  Whether the device is on or not. True succeed, False failed 
    '''
    def begin(self):
        self._soft_reset()
        self._set_power_als_on()
        device_id = self._get_device_id()
        revision_id =self._get_revision_id()
        if device_id != self.DEVICE_ID and revision_id != self.REVISION_ID :
            self._set_device_adc(False)
            self._set_device_power(False)
            return False
        return True 
    
    ''' 
        @brief  Config the wait timer 
        @param  mode  set wait-timer,True enable False disenable
    '''
    def set_wait_timer(self,mode=True):
        if mode==True:
            buf = self._read_reg(self.TSL2541_REG_ENABLE)|0x02
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
        if mode==False:
            buf = self._read_reg(self.TSL2541_REG_ENABLE)&0xFE
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
    
    ''' 
        @brief  Set the internal integration time
        @param  atime  the internal integration time(range: 0x00 -0xff)
    '''
    def set_integration_time(self,atime):
        atime = atime & 0xFF
        self._atime = atime
        self._write_reg(self.TSL2541_REG_ATIME, atime)
    
    ''' 
        @brief  Set wait time 
        @param  wtime  wait time(range: 0x00 -0xff)
    '''
    def set_wait_time(self,wtime):
        wtime = wtime & 0xFF
        self._wtime = wtime
        self._write_reg(self.TSL2541_REG_WTIME, wtime)
    
    ''' 
        @brief  Set the channel 0 interrupt threshold
        @param  ailt   the low 16 bit values(range: 0x0000 -0xffff)
        @param  aiht   the high 16 bit values(range: 0x0000 -0xffff)
    '''
    def set_interrupt_threshold(self,ailt,aiht):
        ailtl = ailt & 0xFF
        ailth = (ailt>>8) & 0xFF
        aihtl = aiht & 0xFF
        aihth = (aiht>>8) & 0xFF
        self._write_reg(self.TSL2541_REG_AILTL, ailtl)
        self._write_reg(self.TSL2541_REG_AILTH, ailth)
        self._write_reg(self.TSL2541_REG_AIHTL, aihtl)
        self._write_reg(self.TSL2541_REG_AIHTH, aihth)
    
    ''' 
        @brief  Set the channel 0 interrupt Persistence
        @param  apers  Interrupt Persistence(range: 0x00 -0x0f)
    '''
    def set_interrupt_persistence(self,apers):
        apers = apers & 0xFF
        self._write_reg(self.TSL2541_REG_PERS, apers)
    
    ''' 
        @brief  使能等待长
        @param  mode  True enable 等待时间将增加12倍  False disenable
    '''
    def set_wait_long_time(self,mode=True):
        if mode == True:
            self._write_reg(self.TSL2541_REG_CFG0, self.CONFIG_NO_WLONG)
            self._wlong = 1
        if mode == False:
            self._write_reg(self.TSL2541_REG_CFG0, self.CONFIG_WLONG)
            self._wlong = 0
    
    ''' 
        @brief  Set the ALS gain 
        @param  gain  the value of gain(range: 0x00 -0x03)
    '''
    def set_als_gain(self,gain):
        if gain > 0 and gain < 5:
            buf1 = self._read_reg(self.TSL2541_REG_CFG1) & 0xFC
            gain = gain - 1
            buf1 = buf1 | gain
            self._write_reg(self.TSL2541_REG_CFG1, buf1)
            buf2 = self._read_reg(self.TSL2541_REG_CFG2) & 0xEF
            buf2 = buf2 & 0x04
            self._write_reg(self.TSL2541_REG_CFG2, buf2)
        elif gain == 0:
            buf1 = self._read_reg(self.TSL2541_REG_CFG1) & 0xFC
            self._write_reg(self.TSL2541_REG_CFG1, buf1)
            buf2 = self._read_reg(self.TSL2541_REG_CFG2) & 0xEB
            self._write_reg(self.TSL2541_REG_CFG2, buf2)
        elif gain == 5:
            buf1 = self._read_reg(self.TSL2541_REG_CFG1) | 0x03
            self._write_reg(self.TSL2541_REG_CFG1, buf1)
            buf2 = self._read_reg(self.TSL2541_REG_CFG2) | 0x14
            self._write_reg(self.TSL2541_REG_CFG2, buf2)
    
    ''' 
        @brief 获取可见光强度的值
        @return  the visble data
    '''
    def get_visble_data(self):
        dataL = self._read_reg(self.TSL2541_REG_VISDATAL)
        dataH = self._read_reg(self.TSL2541_REG_VISDATAH)
        return dataL | (dataH << 8)
    
    ''' 
        @brief 获取红外光强度的值
        @return  the IR data
    '''
    def get_IR_data(self):
        dataL = self._read_reg(self.TSL2541_REG_IRDATAL)
        dataH = self._read_reg(self.TSL2541_REG_IRDATAH)
        return dataL | (dataH << 8)
    
    '''
        @brief  If this bit is set, all flag bits in the STATUS register will be reset whenever the STATUS register is read over I2C. 
        @param  mode  True enable  False disenable
    '''
    def set_int_read_clear(self,mode=True):
        if mode == True:
            buf = self._read_reg(self.TSL2541_REG_CFG3)|0x80
            self._write_reg(self.TSL2541_REG_CFG3, buf)
        if mode == False:
            buf = self._read_reg(self.TSL2541_REG_CFG3)&0x7F
            self._write_reg(self.TSL2541_REG_CFG3, buf)
    
    '''
        @brief  Config the function of 'sleep after interruption'
        @param  mode  True enable  False disenable
    '''
    def set_sleep_after_interrupt(self,mode=True):
        if mode == True:
            buf = self._read_reg(self.TSL2541_REG_CFG3)|0x10
            self._write_reg(self.TSL2541_REG_CFG3, buf)
        if mode == False:
            buf = self._read_reg(self.TSL2541_REG_CFG3)&0xEF
            self._write_reg(self.TSL2541_REG_CFG3, buf)
    
    '''
        @brief  set aotuzero mode
        @param  mode  0,Always start at zero when searching the best offset value  1,Always start at the previous (offset_c) with the auto-zero mechanism
    '''
    def set_auto_zero_mode(self,mode=0):
        if(mode==1):
            buf = self._read_reg(self.TSL2541_REG_AZ_CONFIG)|0x80
            self._write_reg(self.TSL2541_REG_AZ_CONFIG, buf)
        if(mode==0):
            buf = self._read_reg(self.TSL2541_REG_AZ_CONFIG)&0x7F
            self._write_reg(self.TSL2541_REG_AZ_CONFIG, buf)
    
    ''' 
        @brief  set autozero automatically every nth ALS iteration)
        @param  iteration_type  0,never  7F,only at first ALS cycle  n, every nth time
    '''
    def set_auto_zero_nth_iteration(self,iteration_type):
        iteration_type = iteration_type & 0x7F
        buf = self._read_reg(self.TSL2541_REG_AZ_CONFIG)|iteration_type
        self._write_reg(self.TSL2541_REG_AZ_CONFIG, buf)
    
    '''
        @brief  Config the ambient light sensing interruption
        @param  mode  True enable  False disenable
    '''
    def set_als_interrupt(self,mode=True):
        self.set_int_read_clear(True)
        if(mode==True):
            buf = self._read_reg(self.TSL2541_REG_INTENAB)|0x10
            self._write_reg(self.TSL2541_REG_INTENAB, buf)
        if(mode==False):
            buf = self._read_reg(self.TSL2541_REG_ENABLE)&0xEF
            self._write_reg(self.TSL2541_REG_INTENAB, buf)
    
    '''
        @brief  Config the ALS saturation interruption
        @param  mode  True enable  False disenable
    '''
    def set_als_saturation_interrupt(self,mode=True):
        self.set_int_read_clear(True);
        if(mode==True):
            buf = self._read_reg(self.TSL2541_REG_INTENAB)|0x80
            self._write_reg(self.TSL2541_REG_INTENAB, buf)
        if(mode==False):
            buf = self._read_reg(self.TSL2541_REG_INTENAB)&0x7F
            self._write_reg(self.TSL2541_REG_INTENAB, buf)
    
    '''
        @brief  Get the status of the device
    '''
    def clear_int_flag(self):
        self._read_reg(self.TSL2541_REG_STATUS)
    
    '''
        @brief  Activating the internal oscillator to permit the timers and ADC channels to operate ,and activing the ALS function
    '''
    def _set_power_als_on(self):
        self._write_reg(self.TSL2541_REG_ENABLE, 0x0B)
    
    '''
        @brief  Activating the internal oscillator to permit the timers and ADC channels to operate
        @param  mode  True enable  False disenable
    '''
    def _set_device_power(self,mode=True):
        if(mode==True):
            buf = self._read_reg(self.TSL2541_REG_ENABLE)|0x01
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
        if(mode==False):
            buf = self._read_reg(self.TSL2541_REG_ENABLE)&0xFE
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
    
    '''
        @brief  Activating the four-channel ADC
        @param  mode  True enable  False disenable
    '''
    def _set_device_adc(self,mode=True):
        if(mode==True):
            buf = self._read_reg(self.TSL2541_REG_ENABLE)|0x02
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
        if(mode==False):
            buf = self._read_reg(self.TSL2541_REG_ENABLE)&0xFD
            self._write_reg(self.TSL2541_REG_ENABLE, buf)
    
    '''
        @brief  get the revision id
        @return  the revision id
    '''
    def _get_revision_id(self):
        return self._read_reg(self.TSL2541_REG_REVID)
    
    '''
        @brief  get the device id
        @return  the device id
    '''
    def _get_device_id(self):
        return self._read_reg(self.TSL2541_REG_ID)
    
    '''
        @brief  Initializes all registers of the device
    '''
    def _soft_reset(self):
        self.set_wait_timer(False)
        self.set_integration_time(0x23)
        self.set_wait_time(0)
        self.set_wait_long_time(False)
        self.set_als_gain(4)
        self.set_int_read_clear(False)
        self.set_sleep_after_interrupt(False)
        self.set_auto_zero_mode(0)
        self.set_auto_zero_nth_iteration(0x7F)
        self.set_als_saturation_interrupt(False)
        self.set_als_interrupt(False)

    def _write_reg(self, reg, buff):
        self.i2cbus.write_byte_data(self.i2c_addr, reg, buff)
        

    def _read_reg(self, reg):
        return self.i2cbus.read_byte_data(self.i2c_addr, reg) 

    def scan(self):
        try:
            self.i2cbus.write_quick(self.i2c_addr)
            return True
        except:
            print("I2C init fail")
            return False
