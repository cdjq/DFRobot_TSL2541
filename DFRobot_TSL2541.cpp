/*!
 * @file DFRobot_TSL2541.cpp
 * @brief Define the basic structure of class DFRobot_TSL2541
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2021-3-24
 * @url https://github.com/DFRobot/DFRobot_TSL2541
 */

#include <DFRobot_TSL2541.h>

DFRobot_TSL2541::~DFRobot_TSL2541() {
    _cfg3Reg.reservedBit5_6 = 2;
    _cfg3Reg.reservedBit0_3 = 12;
}

bool DFRobot_TSL2541::begin(void)
{
    _pWire->begin();
    delay(100);
    _pWire->beginTransmission(_deviceAddr);
    if((getDeviceID()!=TSL2541_ID) || (getRevisionID() != TSL2541_REVISION_ID)){
        setDeviceALSADC(false);
        setDevicePower(false);
        return false;
    }
    softReset();
    setPowerALSADC();
    readReg(TSL2541_REG_VISADJ, &adjFac, 1);
    return true;
}

void DFRobot_TSL2541::setIntegrationTime(uint8_t aTime)
{
    writeReg(TSL2541_REG_ATIME, &aTime, 1);
}

void DFRobot_TSL2541::setWaitTime(uint8_t wTime)
{
    writeReg(TSL2541_REG_WTIME, &wTime, 1);
}

void DFRobot_TSL2541::setPowerALSADC()
{
    _enableReg.pon = 1;
    _enableReg.aen = 1;
    writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
}

void DFRobot_TSL2541::setDevicePower(bool mode)
{
    if(mode == true){
        _enableReg.pon = 1;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
    else{
        _enableReg.pon = 0;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
}

void DFRobot_TSL2541::setDeviceALSADC(bool mode)
{
    if(mode == true){
        _enableReg.aen = 1;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
    else{
        _enableReg.aen = 0;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
}

void DFRobot_TSL2541::setWaitTimer(bool mode)
{
    if(mode == true){
        _enableReg.wen = 1;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
    else{
        _enableReg.wen = 0;
        writeReg(TSL2541_REG_ENABLE, &_enableReg, 1);
    }
}

void DFRobot_TSL2541::setInterruptPersistence(uint8_t apers)
{
    writeReg(TSL2541_REG_PERS, &apers, 1);
}

void DFRobot_TSL2541::setWaitLong(bool mode)
{
    uint8_t wlong = 0x80;
    uint8_t nlong = 0x84;
    if(mode){
        writeReg(TSL2541_REG_CFG0, &wlong, 1);
    
    }else{
        writeReg(TSL2541_REG_CFG0, &nlong, 1);
    }
}

void DFRobot_TSL2541::setALSGain(uint8_t aGain)
{
    if(aGain > 0 && aGain < 5){
        _cfg1Reg.again=aGain - 1;
        _cfg2Reg.againl = 1;
        _cfg2Reg.againMax = 0;
        writeReg(TSL2541_REG_CFG1, &_cfg1Reg, 1);
        writeReg(TSL2541_REG_CFG2, &_cfg2Reg, 1);
    }else if(aGain == 0){
        _cfg1Reg.again=0;
        _cfg2Reg.againl = 0;
        _cfg2Reg.againMax = 0;
        writeReg(TSL2541_REG_CFG1, &_cfg1Reg, 1);
        writeReg(TSL2541_REG_CFG2, &_cfg2Reg, 1);
    }else if(aGain == 5){
        _cfg1Reg.again=3;
        _cfg2Reg.againl = 1;
        _cfg2Reg.againMax = 1;
        writeReg(TSL2541_REG_CFG1, &_cfg1Reg, 1);
        writeReg(TSL2541_REG_CFG2, &_cfg2Reg, 1);
    }
}

uint8_t DFRobot_TSL2541::getRevisionID()
{
    uint8_t buf;
    readReg(TSL2541_REG_REVID, &buf, 1);
    return buf;
}

uint8_t DFRobot_TSL2541::getDeviceID()
{
    uint8_t buf;
    readReg(TSL2541_REG_ID, &buf, 1);
    return buf;
}

void DFRobot_TSL2541::clearIntFlag()
{
    uint8_t buf;
    readReg(TSL2541_REG_STATUS, &buf, 1);
}

uint16_t DFRobot_TSL2541::getVisibleData()
{
    uint8_t buf[2];
    readReg(TSL2541_REG_VISDATAL, buf, 2);
    uint16_t vis = (uint16_t)buf[0] | (uint16_t)(buf[1]<<8);
    if (adjFac < 0x80){
        vis = vis*((float)adjFac / 100 + 1);
    }else{
        vis = vis*(1 - (((float)adjFac - 0x80) / 100));
    }
    return vis;
}

uint16_t DFRobot_TSL2541::getIRData()
{
    uint8_t buf[2];
    readReg(TSL2541_REG_IRDATAL, buf, 2);
    return (uint16_t)buf[0] | (uint16_t)(buf[1]<<8);
}

void DFRobot_TSL2541::setIntReadClear(bool mode)
{
    if (mode){
        _cfg3Reg.intReadClear = 1;
        writeReg(TSL2541_REG_CFG3, &_cfg3Reg, 1);
    }else{
        _cfg3Reg.intReadClear = 0;
        writeReg(TSL2541_REG_CFG3, &_cfg3Reg, 1);
    }
}


void DFRobot_TSL2541::setSleepAfterInterrupt(bool mode)
{
    if (mode){
        _cfg3Reg.sai = 1;
        writeReg(TSL2541_REG_CFG3, &_cfg3Reg, 1);
    }else{
        _cfg3Reg.sai = 0;
        writeReg(TSL2541_REG_CFG3, &_cfg3Reg, 1);
    }
}

void DFRobot_TSL2541::setAutoZeroMode(uint8_t mode)
{
    if(mode == 0){
        _AZCfgReg.azMode = 0;
    }
    if(mode == 1){
        _AZCfgReg.azMode = 1;
    }
    writeReg(TSL2541_REG_AZ_CONFIG, &_AZCfgReg, 1);
}


void DFRobot_TSL2541::setAutoZeroNTHIteration(uint8_t value)
{
    _AZCfgReg.azNTHIteration = value &0x7F;
    writeReg(TSL2541_REG_AZ_CONFIG, &_AZCfgReg, 1);
}

void DFRobot_TSL2541::setALSSaturationInterrupt(bool mode )
{
    setIntReadClear(true);
    if(mode){
        _intEnabReg.asien = 1;
        writeReg(TSL2541_REG_INTENAB, &_intEnabReg, 1);
    }else{
        _intEnabReg.asien = 0;
        writeReg(TSL2541_REG_INTENAB, &_intEnabReg, 1);
    }
}

void DFRobot_TSL2541::setALSInterrupt(bool mode )
{
    setIntReadClear(true);
    if(mode){
        _intEnabReg.aien = 1;
        writeReg(TSL2541_REG_INTENAB, &_intEnabReg, 1);
    }else{
        _intEnabReg.aien = 0;
        writeReg(TSL2541_REG_INTENAB, &_intEnabReg, 1);
    }
}

void DFRobot_TSL2541::setVisIntThreshold(uint16_t thresholdL,uint16_t thresholdH)
{
    uint8_t buf[4] = {thresholdL&0x00FF, (thresholdL&0xFF00)>>8, thresholdH&0x00FF, (thresholdH&0xFF00)>>8};
    writeReg(TSL2541_REG_AILTL, buf, 4);
}

void DFRobot_TSL2541::softReset()
{
    setWaitTimer(false);
    setIntegrationTime(0x23);
    setWaitTime(0);
    setWaitLong(false);
    setALSGain(4);
    setIntReadClear(false);
    setSleepAfterInterrupt(false);
    setAutoZeroMode(0);
    setAutoZeroNTHIteration(0x7f);
    setALSInterrupt(false);
    setALSSaturationInterrupt(false);
}

bool DFRobot_TSL2541::writeReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    for(uint16_t i = 0; i < size; i++){
        _pWire->write(_pBuf[i]);
    }
    if( _pWire->endTransmission() != 0){
        return 0;
    }else{
        return 1;
    }
}

uint8_t DFRobot_TSL2541::readReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    if( _pWire->endTransmission() != 0){
        return 0;
    }

    _pWire->requestFrom(_deviceAddr, (uint8_t) size);
    for(uint16_t i = 0; i < size; i++){
        _pBuf[i] = _pWire->read();
    }
    return size;
}
