#-*- coding: utf-8 -*-
""" file get_visble_IR_data.py
  # @brief 获取可见光和红外强度
  # @n The experimental phenomena：The serial port outputs the optical data obtained once a second
  # @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence     The MIT License (MIT)
  # @author      [LuoYufeng]<yufeng.luo@dfrobot.com> 
  # @version  V1.0
  # @date  2021-03-30
  # @get from https://www.dfrobot.com
  # @url https://github.com/DFRobot/DFRobot_TSL2541
"""
import sys
sys.path.append('../')
from DFRobot_TSL2541 import DFRobot_TSL2541
import time

TSL2541 = DFRobot_TSL2541(bus = 1)
while(TSL2541.begin() == False ):
    print ('Please check that the IIC device is properly connected')

def main():
    while True:
        vis = TSL2541.get_visble_data()
        IR = TSL2541.get_IR_data()
        print ('Visble:%d'%vis,' IR:%d'%IR)
        time.sleep(1)

if __name__ == "__main__":
    main()
