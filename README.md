# DFRobot_TCS3430

The TSL2541 is a very-high sensitivity light-to-digital converter that transforms light <br>
intensity into a digital signal output capable of direct I²C interface. The ALS sensor <br>
features 2 output channels, a visible channel and an IR channel. The visible channel has a <br>
photodiode with a UV and IR blocking filter whereas the IR channel has a photodiode with an <br>
IR pass filter. Each channel has a dedicated data converter producing a 16-bit output. This <br>
architecture allows applications to accurately measure ambient light which enables devices <br>
to calculate illuminance to control a display backlight.<br>

## 产品链接（https://www.dfrobot.com/）
    SKU：SEN0426

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)
<snippet>
<content>

## Summary
获取可见光和红外光强度

## Installation

To use this library, download the library file first, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @brief  Initialization function
   * @return return true succeed ;return false failed.
   */
  bool begin();

  /**
   * @brief  Config the wait timer
   * @param  mode  ture : enable ; false : disable
   */
  void setWaitTimer(bool mode = true);

  /**
   * @brief  Set the wait long time
   * @param  mode  ture : enable ; false : disenable
   */
  void setWaitLong(bool mode = true);

  /**
   * @brief  Set the internal integration time of the  four-channel ADCs
   * @param  aTIme  integration time
   */
  void setIntegrationTime(uint8_t aTime);

  /**
   * @brief  设置等待时间 
   * @param  wTime  wait time 
   */
  void setWaitTime(uint8_t wTime);

  /**
   * @brief  设置数据测量增益 
   * @param  aGain  the value of gain
   */
  void setALSGain(uint8_t aGain);

  /**
   * @brief  设置增益中断积分
   * @param  apers :ALS Interrupt Persistence
   */
  void setInterruptPersistence(uint8_t apers);

  /**
   * @brief  清除中断状态
   */
  void clearIntFlag();
  
  /**
   * @brief  获取可见光强度
   * @return  可见光强度
   */
  uint16_t getVisibleData();

  /**
   * @brief  获取红外光强度
   * @return  红外光强度
   */
  uint16_t getIRData();

  /**
   * @brief  If this bit is set, all flag bits in the STATUS register will be reset 
   * @n      whenever the STATUS register is read over I2C.
   * @param  mode  ture : enable ; false : disable
   */
  void setIntReadClear(bool mode = true);

  /**
   * @brief  Turn on sleep after interruption
   * @param  mode  ture : enable ; false : disable
   */
  void setSleepAfterInterrupt(bool mode = true);

  /**
   * @brief  set az mode
   * @param  mode  :0, Always start at zero when searching the best offset value
                   :1, Always start at the previous (offset_c) with the auto-zero mechanism
   */
  void setAutoZeroMode(uint8_t mode);
  
  /**
   * @brief  set as nth iteration type(Run autozero automatically every nth ALS iteration)
   * @param  value :0,never
   * @n            :7,only at first ALS cycle
   * @n            :n, every nth time
   */
  void setAutoZeroNTHIteration(uint8_t value);

  /**
   * @brief  enable ALS saturation interrupt
   * @param  mode  ture : enable ; false : disable
   */
  void setALSSaturationInterrupt(bool mode = true);

  /**
   * @brief  enable ALS interription
   * @param  mode  ture : enable ; false : disable
   */
  void setALSInterrupt(bool mode = true);

  /**
   * @brief  设置可见光中断触发阈值
   * @param  thresholdL :the low 16 bit values
   * @param  thresholdH :the high 16 bit values
   */
  void setVisIntThreshold(uint16_t thresholdL,uint16_t thresholdH);

```


## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno |       √      |             |            | 
FireBeetle esp32 |       √      |             |            | 
FireBeetle esp8266 |       √      |             |            | 
FireBeetle m0 |       √      |             |            | 
Leonardo |       √      |             |            | 
Microbit |       √      |             |            | 
Arduino MEGA2560 | √ | | | 


## History

- Data 2021-03-26
- Version V1.0


## Credits

Written by [LuoYufeng]<yufeng.luo@dfrobot.com>,2021,(Welcome to our [website](https://www.dfrobot.com/))
