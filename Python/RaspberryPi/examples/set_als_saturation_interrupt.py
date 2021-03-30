#-*- coding: utf-8 -*-
""" file set_als_saturation_interrupt.py
  # @brief Turn on the visble light saturation interrupt function
  # @n The experimental phenomena：If the optical data is saturated, the serial port will output a warning
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
    print ('Please check that the IIC device is properly connected.\n')

#Configure the ALS saturation interrupt function of the sensor
TSL2541.set_int_read_clear(mode = True)
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
TSL2541.set_als_saturation_interrupt(mode = True)
print ('If the optical data is saturated, an interrupt is triggered and a warning is printed.\r\n')

def main():
    while True:
        vis = TSL2541.get_visble_data()
        print ('Visble:%d'%vis)
        time.sleep(1)

if __name__ == "__main__":
    main() 
