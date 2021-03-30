/*!
 * @file setALSInterrupt.ino
 * @brief Turn on the visble light sense interrupt function to obtain the visble light data within the specified range
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

int LEDpin = 12;

volatile int state = 0;

void handleInterrupt(){

  Serial.println("WARNING:The data obtained exceeds the threshold");
  state = 1;
}

void setup() {
    Serial.begin(9600);
    
    while(!TSL2541.begin()){
        Serial.println("Please check that the IIC device is properly connected");
        delay(1000);
    }
    
    // Configure the sensor's ADC integration time, device waiting time, and gain
    
    //TSL2541.setWaitTimer(true);
    //TSL2541.setWaitLong(false);
    /*
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
    //TSL2541.setWaitTime(/*wTime=*/0x00);
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
    /*
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
    TSL2541.setALSGain(/*aGian=*/4);
    
    /* Turn on the ALS interrupt function of the device */
    
    //mode = true : enable ALS Interrupt
    TSL2541.setALSInterrupt(/*mode*/true);
    
    /*
     *                       APERS                              
     * ----------------------------------------------------------
     * | Field Value |            Persistence                   |
     * ----------------------------------------------------------
     * |     0x00    |   Every ALS cycle generates an interrupt |
     * ----------------------------------------------------------
     * |     0x01    |   Any value outside of threshold range   |
     * ----------------------------------------------------------
     * |     0x02    |   2 consecutive values out of range      |
     * ----------------------------------------------------------
     * |     0x03    |   3 consecutive values out of range      |
     * ----------------------------------------------------------
     * |     0x04    |   5 consecutive values out of range      |
     * ----------------------------------------------------------
     * |     0x05    |   10 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x06    |   15 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x07    |   20 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x08    |   25 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x09    |   30 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0A    |   35 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0B    |   40 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0C    |   45 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0D    |   50 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0E    |   55 consecutive values out of range     |
     * ----------------------------------------------------------
     * |     0x0F    |   60 consecutive values out of range     |
     * ----------------------------------------------------------
     */
    TSL2541.setInterruptPersistence(/*apers=*/0x05);
    
    // Set the threshold range(0-65535)
    TSL2541.setVisIntThreshold(/*thresholdL=*/5000,/*thresholdH=*/10000);
    
    Serial.println("If the light data exceeds the threshold, an interrupt is triggered and a warning is printed.");

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
    delay(100);
  }
}
