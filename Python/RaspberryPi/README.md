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

```
python get_xyz_ir_data.py
```

## Methods

```python
  ''' 
    @brief  Set temperature and humidity
    @return  equipment condition, True succeed, False failed 
  '''
  begin(self)

  ''' 
    @brief  enable wait timer 
    @param  mode  set wait-timer,True enable False disenable
  '''
  set_wait_timer(self,mode=True)

  '''
    @brief  声明“等待长”后，WTIME设置的等待时间将增加12倍
    @param  mode  True enable  False disenable
  '''
  def set_wait_long_time(self,mode=True)

  ''' 
    @brief  Set the internal integration time
    @param  atime  the internal integration time(range: 0x00 -0xff)
  '''
  set_integration_time(self,atime)

  ''' 
    @brief  Set wait time 
    @param  wtime  wait time(range: 0x00 -0xff)
  '''
  set_wait_time(self,wtime)

  ''' 
    @brief  设置可见光中断触发阈值
    @param  ailt   the low 16 bit values(range: 0x0000 -0xffff)
    @param  aiht   the high 16 bit values(range: 0x0000 -0xffff)
  '''
  set_interrupt_threshold(self,ailt,aiht)

  ''' 
    @brief  计数超出阈值限制的连续可见通道结果的频率。用这个数值与设置的阈值比较，产生中断
    @param  apers  Interrupt Persistence(range: 0x00 -0x0f)
  '''
  set_interrupt_persistence(self,apers)

  ''' 
    @brief  Set the wait long time
    @param  mode  True enable  False disenable
  '''
  set_wait_long_time(self,mode=True)

  ''' 
    @brief  Set the ALS gain 
    @param  gain  the value of gain(range: 0 - 5)
  '''
  set_als_gain(self,gain)

  ''' 
    @brief 获取可见光
    @return  可见光原始数据
  '''
  get_visble_data(self)
        
  ''' 
    @brief 获取红外光
    @return  红外光原始数据
  '''
  get_IR_data(self)

  '''
    @brief  If this bit is set, all flag bits in the STATUS register will be reset whenever the STATUS register is read over I2C. 
    @param  mode  True enable  False disenable
  '''
  set_int_read_clear(self,mode=True)

  '''
    @brief  Turn on sleep after interruption
    @param  mode  True enable  False disenable
  '''
  set_sleep_after_interrupt(self,mode=True)

  '''
    @brief  set az mode
    @param  mode  0,Always start at zero when searching the best offset value  1,Always start at the previous (offset_c) with the auto-zero mechanism
  '''
  set_auto_zero_mode(self,mode=0)

  ''' 
    @brief  set az nth iteration type(Run autozero automatically every nth ALS iteration)
    @param  iteration_type  0,never  7F,only at first ALS cycle  n, every nth time
  '''
  set_auto_zero_nth_iteration(self,iteration_type)

  '''
    @brief  enable ambient light sensing interrupt
    @param  mode  True enable  False disenable
  '''
  set_als_interrupt(self,mode=True)

  '''
    @brief  enable ALS saturation interription
    @param  mode  True enable  False disenable
  '''
  set_als_saturation_interrupt(self,mode=True)

  '''
    @brief  清除中断标志
  '''
  clear_int_flag(self)

```

## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## History

- Data 2021-03-26
- Version V1.0


## Credits

Written by [LuoYufeng]<yufeng.luo@dfrobot.com>,2021,(Welcome to our [website](https://www.dfrobot.com/))(https://www.dfrobot.com/))
