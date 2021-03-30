/*!
 * @file getVisbleIRData.ino
 * @brief 获取可见光数据和红外数据
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-01-26
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_TSL2541
 */
#include <DFRobot_TSL2541.h>

DFRobot_TSL2541 TSL2541;
void setup() {
  Serial.begin(9600);

  while(!TSL2541.begin()){
    Serial.println("Please check that the IIC device is properly connected");
    delay(1000);
  }
}

void loop() {
  Serial.print("Visble:");//可见光强度
  Serial.print(TSL2541.getVisibleData());
  Serial.print(" IR:");//红外光强度
  Serial.println(TSL2541.getIRData());
  delay(1000);
}
