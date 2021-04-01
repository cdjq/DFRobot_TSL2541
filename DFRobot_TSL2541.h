/*!
 * @file DFRobot_TSL2541.h
 * @brief Define the basic structure of class DFRobot_TSL2541 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2021-3-24
 * @url https://github.com/DFRobot/DFRobot_TSL2541
 */

#ifndef _DFRobot_TSL2541_H
#define _DFRobot_TSL2541_H

#include "Arduino.h"
#include <Wire.h>




//Open this macro to see the detailed running process of the program 
//#define ENABLE_DBG
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_TSL2541
{
    /*I2C ADDRESS*/
    #define TSL2541_IIC_ADDRESS         0x39
    
    #define TSL2541_REG_ENABLE          0x80
    #define TSL2541_REG_ATIME           0x81
    #define TSL2541_REG_WTIME           0x83
    #define TSL2541_REG_AILTL           0x84
    #define TSL2541_REG_AILTH           0x85
    #define TSL2541_REG_AIHTL           0x86
    #define TSL2541_REG_AIHTH           0x87
    #define TSL2541_REG_PERS            0x8C
    #define TSL2541_REG_CFG0            0x8D
    #define TSL2541_REG_CFG1            0x90
    #define TSL2541_REG_REVID           0x91
    #define TSL2541_REG_ID              0x92
    #define TSL2541_REG_STATUS          0x93
    #define TSL2541_REG_VISDATAL        0x94
    #define TSL2541_REG_VISDATAH        0x95
    #define TSL2541_REG_IRDATAL         0x96
    #define TSL2541_REG_IRDATAH         0x97
    #define TSL2541_REG_REVID2          0x9E
    #define TSL2541_REG_CFG2            0x9F
    #define TSL2541_REG_CFG3            0xAB
    #define TSL2541_REG_AZ_CONFIG       0xD6
    #define TSL2541_REG_INTENAB         0xDD
    #define TSL2541_REG_VISADJ          0xE6
    
    #define TSL2541_ID                  0xE4
    #define TSL2541_REVISION_ID         0x61
    
    /* 
     * CFG0 Register (0x8D)
     * ------------------------------------------------------------------------------------------------
     * |    b7       |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------------
     * | reversed<1> |                  reversed                    |   WLONG  |       reversed       |
     * ------------------------------------------------------------------------------------------------
     * Reserved: Write as 0.
     * Reserved<1>: Write as 1.
     * Wait Long: When asserted, the wait cycles are increased by a factor 12× from that programmed in the WTIME register.
     */
    
    /* 
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
    */
    typedef struct { 
        uint8_t   again: 2; 
        uint8_t   reservedBit2_7: 6; 
    } __attribute__ ((packed)) sCFG1Reg_t;
    
    /*
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
     */
    #define TSL2541_CONFIG_NO_WLONG             0x80
    #define TSL2541_CONFIG_WLONG                0x84
    
    /* 
     * CFG2 Register (0x9F)
     * ---------------------------------------------------------------------------------------------
     * |    b7    |       b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ---------------------------------------------------------------------------------------------
     * |           Reserved(000)           | AGAINMAX | Reserved |  AGAINL  |     Reserved(00)     |
     * ---------------------------------------------------------------------------------------------
     * The ALS gain can be adjusted by setting the two AGAIN bits as well as the AGAINMAX and AGAINL bits 
     * which yields an overall range from 1/2x to 128x.
     */
    typedef struct { 
        uint8_t   reservedBit0_1: 2;
        uint8_t   againl: 1;
        uint8_t   reservedBit3: 1;
        uint8_t   againMax: 1; 
        uint8_t   reservedBit5_7: 3; 
    } __attribute__ ((packed)) sCFG2Reg_t;

    /*
     * Enable Register (ENABLE 0x80)
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |                  reversed                 |   WEN    | reserved |    AEN    |   PON    |
     * ------------------------------------------------------------------------------------------
     *
     */
    typedef struct {
        uint8_t   pon: 1; /* Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate. 
                            Writing a one activates the oscillator. 
                            Writing a zero disables the oscillator. */
        uint8_t   aen: 1; /* ALS Enable. This bit actives the ALS function.
                            Set aen=1 and pon=1 in the same command to ensure autozero function is run prior to the first measurement.*/
        uint8_t   reservedBit2: 1; /* Reserved. */
        uint8_t   wen: 1; /* Wait Enable. This bit activates the wait feature. 
                            Writing a 1 activates the wait timer.
                            Writing a 0 disables the wait timer. */
        uint8_t   reservedBit4_7: 4; /* Reserved. */
    } __attribute__ ((packed)) sEnableReg_t;

    /*
     * CFG3 Register (0xAB)
     * ----------------------------------------------------------------------------------------------------
     * |        b7          |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ----------------------------------------------------------------------------------------------------
     * |   INT_READ_CLEAR   |     Reserved(0)     |    SAI   |           Reserved(any value)              |
     * ----------------------------------------------------------------------------------------------------
     * INT_READ_CLEAR:If this bit is set, all flag bits in the STATUS register will be reset whenever the STATUS register is read over I2C.
     * SAI:Sleep After Interrupt. Power down the device at the end of the ALS cycle if an interrupt has been generated.
     */
    typedef struct { 
        uint8_t   reservedBit0_3: 4; 
        uint8_t   sai: 1; 
        uint8_t   reservedBit5_6: 2;
        uint8_t   intReadClear: 1; 
    } __attribute__ ((packed)) sCFG3Reg_t;
    
    /* 
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
     */
    typedef struct { 
        uint8_t   azNTHIteration: 7;
        uint8_t   azMode: 1; 
    } __attribute__ ((packed)) sAZCFGReg_t;
    
    /*
     * INTENAB Register (0xDD)
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |  ASIEN   |     Reserved(0)     |   AIEN   |                 Reserved(0)                |
     * ------------------------------------------------------------------------------------------
     * ASIEN: Writing '1' to this bit enables A
     
     
     
     
     SAT interrupt.
     * AIEN: Writing '1' to this bit enables ALS interrupt.
     */
    typedef struct { 
        uint8_t   reservedBit0_3: 4; 
        uint8_t   aien: 1; 
        uint8_t   reservedBit5_6: 2;
        uint8_t   asien: 1; 
    } __attribute__ ((packed)) sINTENABReg_t;
    
public:


    /**
     * @brief Constructor 
     * @param Input Wire address
     */
    DFRobot_TSL2541(TwoWire *pWire = &Wire){_pWire = pWire;};
    ~DFRobot_TSL2541();
    /*!
     *@brief Init chip 
     *@return True means IIC communication succeeds, false means it fails.
     */
    bool begin(void);

    /**
     * @brief  Config the wait timer 
     * @param  mode  ture : enable ; false : disable
     */
    void setWaitTimer(bool mode = true);
    
    /**
     * @brief  声明“等待长”后，WTIME设置的等待时间将增加12倍。
     * @param  mode  ture : enable ; false : disable
     */
    void setWaitLong(bool mode = true);
    
    /**
     * @brief  设置ADC的积分时间
     * @param  aTIme  integration time (0x00-0xFF)
     * Maximum ALS Value=  min [CYCLES * 1024, 65535]
     * ---------------------------------------------------------------------
     * | aTime | Integration Cycles | Integration Time | Maximum ALS Value |
     * ---------------------------------------------------------------------
     * |  0x00 |         1          |       2.78ms     |        1023       |
     * ---------------------------------------------------------------------
     * |  0x01 |         2          |       5.56ms     |        2047       |
     * ---------------------------------------------------------------------
     * |  ...  |        ...         |       ...        |        ...        |
     * ---------------------------------------------------------------------
     * |  0x11 |         18         |       50ms       |        18431      |
     * ---------------------------------------------------------------------
     * |  0x40 |         65         |       181ms      |        65535      |
     * ---------------------------------------------------------------------
     * |  ...  |        ...         |       ...        |        ...        |
     * ---------------------------------------------------------------------
     * |  0xff |        256         |       711ms      |        65535      |
     * ---------------------------------------------------------------------
     */
    void setIntegrationTime(uint8_t aTime);
    
    /**
     * @brief  设置等待时间
     * @param  wTime  wait time (0x00-0xFF)
     * By asserting wlong, in register 0x8D the wait time is given in multiples of 33.4ms (12x).
     * ----------------------------------------
     * | wtime | Wait Cycles | Wait Time      |
     * ----------------------------------------
     * |  0x00 |      1      | 2.78ms/ 33.4ms |
     * ----------------------------------------
     * |  0x01 |      2      | 5.56ms/ 66.7ms |
     * ----------------------------------------
     * |  ...  |     ...     |      ...       |
     * ----------------------------------------
     * |  0x23 |     36      | 100ms/ 1.20s   |
     * ----------------------------------------
     * |  ...  |     ...     |       ...      |
     * ----------------------------------------
     * |  0xff |     256     |  711ms/ 8.53s  |
     * ----------------------------------------
     */
    void setWaitTime(uint8_t wTime);
    
    /**
     * @brief  设置数据测量增益 
     * @param  aGain  the value of gain (0-5)
     * AGAIN: ALS Gain Control. Sets the gain of the ALS DAC.
     * ----------------------------------------------------------
     * | Field Value |            ALS GAIN VALUE                |
     * ----------------------------------------------------------
     * |     0       |               1/2X Gain                  |
     * ----------------------------------------------------------
     * |     1       |               1X Gain                    |
     * ----------------------------------------------------------
     * |     2       |               4X Gain                    |
     * ----------------------------------------------------------
     * |     3       |               16X Gain                   |
     * ----------------------------------------------------------
     * |     4       |               64X Gain                   |
     * ----------------------------------------------------------
     * |     5       |               128X Gain                  |
     * ----------------------------------------------------------
     */
    void setALSGain(uint8_t aGain);
    
    /**
     * @brief  设置饱和中断阈值，当计数器读到的值超出阈值时产生中断
     * @param  apers :ALS Interrupt Persistence 阈值
     */
    void setInterruptPersistence(uint8_t apers);
    
    /**
     * @brief  读取一次状态寄存器后清除中断标志
     */
    void clearIntFlag();
    
    /**
     * @brief  获取可见光强度
     * @return  可见光强度
     */
    float getVisibleData();
    
    /**
     * @brief  获取红外光强度
     * @return  红外光强度
     */
    uint16_t getIRData();
    
    /**
     * @brief  使能清除中断标志的功能
     * @n      whenever the status register is read over I2C.
     * @param  mode  ture : enable ; false : disable
     */
    void setIntReadClear(bool mode = true);
    
    /**
     * @brief  使能中断后睡眠功能
     * @param  mode  ture : enable ; false : disable
     */
    void setSleepAfterInterrupt(bool mode = true);
    
    /**
     * @brief  set as nth iteration type(Run auto-zero automatically every nth ALS iteration)
     * @param  value :0,never
     * @n            :7,only at first ALS cycle
     * @n            :n,every nth time
     */
    void setAutoZeroNTHIteration(uint8_t value);
    
    /**
     * @brief  Config the ALS saturation interruption
     * @param  mode  ture : enable ; false : disable
     */
    void setALSSaturationInterrupt(bool mode = true);
    
    /**
     * @brief  Config the ALS interruption
     * @param  mode  ture : enable ; false : disable
     */
    void setALSInterrupt(bool mode = true);
    
    /**
     * @brief  设置可见光中断触发阈值
     * @param  thresholdL :the low 16 bit values
     * @param  thresholdH :the high 16 bit values
     */
    void setVisIntThreshold(uint16_t thresholdL,uint16_t thresholdH);
    
private:
    
    /**
     * @brief  get the revision id
     * @return the revision id
     */
    uint8_t getRevisionID();
  
    /**
     * @brief  get the device id
     * @return  the device id
     */
    uint8_t getDeviceID();
  
    /**
     * @brief  Activating the internal oscillator to permit the timers and ADC channels to operate ,and activate the ALS function
     */
    void setPowerALSADC();

    /**
     * @brief  enable or disable the oscillator and puts the part into a low power sleep mode
     * @param  mode:true means enable, false means disable
     */
    void setDevicePower(bool mode);

    /**
     * @brief  enable or disable the four-channel ADC
     * @param  mode:true means enable, false means disable
     */
    void setDeviceALSADC(bool mode);

    /**
     * @brief  Initializes all registers of the device
     */
    void softReset();
    
    
    virtual bool writeReg(uint8_t reg, const void* pBuf, size_t size);
    virtual uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
    
    


private:
    TwoWire *_pWire;
    uint8_t _deviceAddr = TSL2541_IIC_ADDRESS;
    sEnableReg_t _enableReg;
    sCFG1Reg_t _cfg1Reg;
    sCFG2Reg_t _cfg2Reg;
    sCFG3Reg_t _cfg3Reg;
    sAZCFGReg_t _AZCfgReg;
    sINTENABReg_t _intEnabReg;
    uint8_t adjFac;
    uint8_t _wlong;
};

#endif
