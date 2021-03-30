#-*- coding: utf-8 -*-
""" file set_als_interrupt.py
  # @brief Turn on the visble light sense interrupt function to obtain the visble light data within the specified range
  # @n The experimental phenomena：The serial port will output a warning if the optical data exceeds the threshold
  # @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence     The MIT License (MIT)
  # @author      [LuoYufeng]<yufeng.luo@dfrobot.com> 
  # @version  V1.0
  # @date  2021-01-26
  # @get from https://www.dfrobot.com
  # @url https://github.com/DFRobot/DFRobot_TSL2541
"""
import sys
sys.path.append('../')
from DFRobot_TSL2541 import DFRobot_TSL2541
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
TSL2541 = DFRobot_TSL2541(bus = 1)
IO1 = 21#set interrupt pin

def IO1_callback(channel):#callback function
    global TSL2541
    TSL2541.clear_int_flag()
    print("Interrupt is triggered.")
    time.sleep(1)
 
GPIO.setup(IO1, GPIO.IN)
'''
@brief 当IO1从高电平变成低电平，发生FALLING电平跳变时，运行IO1_callback方法
'''
GPIO.add_event_detect(IO1, GPIO.FALLING, callback = IO1_callback)

while(TSL2541.begin() == False ):
  print ('Please check that the IIC device is properly connected')

#Configure the sensor's ADC integration time, device waiting time, and gain
#TSL2541.set_wait_timer(mode = True)
#TSL2541.set_wait_long_time(mode = False)

'''
  #By asserting wlong, in register 0x8D the wait time is given in multiples of 33.4ms (12x).
  #----------------------------------------
  #| wtime | Wait Cycles | Wait Time      |
  #----------------------------------------
  #|  0x00 |      1      | 2.78ms/ 33.4ms |
  #----------------------------------------
  #|  0x01 |      2      | 5.56ms/ 66.7ms |
  #----------------------------------------
  #|  ...  |     ...     |      ...       |
  #----------------------------------------
  #|  0x23 |     36      | 100ms/ 1.20s   |
  #----------------------------------------
  #|  ...  |     ...     |       ...      |
  #----------------------------------------
  #|  0xff |     256     |  711ms/ 8.53s  |
  #----------------------------------------
'''
#TSL2541.set_wait_time(wtime = 0x00)

'''
  #Maximum ALS Value=  min [CYCLES * 1024, 65535]
  #---------------------------------------------------------------------
  #| aTime | Integration Cycles | Integration Time | Maximum ALS Value |
  #---------------------------------------------------------------------
  #|  0x00 |         1          |       2.78ms     |        1023       |
  #---------------------------------------------------------------------
  #|  0x01 |         2          |       5.56ms     |        2047       |
  #---------------------------------------------------------------------
  #|  ...  |        ...         |       ...        |        ...        |
  #---------------------------------------------------------------------
  #|  0x11 |         18         |       50ms       |        18431      |
  #---------------------------------------------------------------------
  #|  0x40 |         65         |       181ms      |        65535      |
  #---------------------------------------------------------------------
  #|  ...  |        ...         |       ...        |        ...        |
  #---------------------------------------------------------------------
  #|  0xff |        256         |       711ms      |        65535      |
  #---------------------------------------------------------------------
'''
TSL2541.set_integration_time(atime=0x23)

'''
  #AGAIN: ALS Gain Control. Sets the gain of the ALS DAC.
  #----------------------------------------------------------
  #| Field Value |            ALS GAIN VALUE                |
  #----------------------------------------------------------
  #|     0       |               1/2X Gain                    |
  #----------------------------------------------------------
  #|     1       |               1X Gain                    |
  #----------------------------------------------------------
  #|     2       |               4X Gain                    |
  #----------------------------------------------------------
  #|     3       |               16X Gain                   |
  #----------------------------------------------------------
  #|     4       |               64X Gain                   |
  #----------------------------------------------------------
  #|     5       |               128X Gain                    |
  #----------------------------------------------------------
'''
TSL2541.set_als_gain(gain=4)

# enable als interrupt
TSL2541.set_als_interrupt(mode = True)
TSL2541.clear_int_flag()
'''
  #                      APERS                              
  #----------------------------------------------------------
  #| Field Value |            Persistence                   |
  #----------------------------------------------------------
  #|     0000    |   Every ALS cycle generates an interrupt |
  #----------------------------------------------------------
  #|     0001    |   Any value outside of threshold range   |
  #----------------------------------------------------------
  #|     0010    |   2 consecutive values out of range      |
  #----------------------------------------------------------
  #|     0011    |   3 consecutive values out of range      |
  #----------------------------------------------------------
  #|     0100    |   5 consecutive values out of range      |
  #----------------------------------------------------------
  #|     0101    |   10 consecutive values out of range     |
  #----------------------------------------------------------
  #|     0110    |   15 consecutive values out of range     |
  #----------------------------------------------------------
  #|     0111    |   20 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1000    |   25 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1001    |   30 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1010    |   35 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1011    |   40 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1100    |   45 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1101    |   50 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1110    |   55 consecutive values out of range     |
  #----------------------------------------------------------
  #|     1111    |   60 consecutive values out of range     |
  #----------------------------------------------------------
'''
TSL2541.set_interrupt_persistence(apers=0x01)

#Set the threshold range(0-65535)
TSL2541.set_interrupt_threshold(ailt=3000,aiht=10000)
print ('If the light data exceeds the threshold, an interrupt is triggered and a warning is printed.\r\n')

def main():
    while True:
        vis = TSL2541.get_visble_data()
        print ('Visble:%d'%vis)
        time.sleep(1)

if __name__ == "__main__":
    main()  
