/*!
 * @file setALSSaturationInterrupt.ino
 * @brief Turn on the visble light saturation interrupt function
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng]<yufeng.luo@dfrobot.com> 
 * @version  V1.0
 * @date  2021-03-26
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_TSL2541
 */
#include <DFRobot_TSL2541.h>


DFRobot_TSL2541 TSL2541;

volatile int state = 0;

void interrupt(){
    Serial.println("WARNING: Visble light saturation");
    state = 1;
}

void setup() {
    Serial.begin(9600);
    
    while(!TSL2541.begin()){
        Serial.println("Please check that the IIC device is properly connected");
        delay(1000);
    }
    
    /*
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
    TSL2541.setIntegrationTime(/*aTime=*/0x23);
    TSL2541.setIntReadClear(/*mode*/true);
    //mode = true ： enable ALS Saturation Interrupt
    TSL2541.setALSSaturationInterrupt(/*mode*/true); 
    
    Serial.println("If the optical data is saturated, an interrupt is triggered and a warning is printed.");
    
    #if defined(ESP32) || defined(ESP8266)||defined(ARDUINO_SAM_ZERO)
    attachInterrupt(digitalPinToInterrupt(D6)/*Query the interrupt number of the D6 pin*/,interrupt,FALLING);
    #else
    /*    The Correspondence Table of AVR Series Arduino Interrupt Pins And Terminal Numbers
    * ---------------------------------------------------------------------------------------
    * |                                        |  DigitalPin  | 2  | 3  |                   |
    * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  |                   |
    * |-------------------------------------------------------------------------------------|
    * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
    * |               Mega2560                 |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
    * |-------------------------------------------------------------------------------------|
    * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
    * |    Leonardo, other 32u4-based          |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
    * |--------------------------------------------------------------------------------------
    */
    /*                      The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
    * ---------------------------------------------------------------------------------------------------------------------------------------------
    * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
    * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
    * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
    * |-------------------------------------------------------------------------------------------------------------------------------------------|
    */
    attachInterrupt(/*Interrupt No*/0,interrupt,FALLING);//Open the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
    //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
    #endif
}

void loop() {
    if (state == 1){
        state =0;
        TSL2541.clearIntFlag(); 
    }else{
    Serial.println(TSL2541.getVisibleData());
    delay(1000);
    }
}
