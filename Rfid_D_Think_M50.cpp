// Arduino D-Think_M50 模块驱动库
// 2016/08/19
// Author: WangXuan

#include "RFID_D_Think_M50.h"

void Rfid_D_Think_M50::serWrite(uint8_t b){
    if(useHardSerial){
        hser->write(b);
    }else{
        sser->write(b);
    }
}

int Rfid_D_Think_M50::serAvailable(){
    if(useHardSerial){
        return hser->available();
    }else{
        return sser->available();
    }
}

int Rfid_D_Think_M50::serRead(){
    if(useHardSerial){
        return hser->read();
    }else{
        return sser->read();
    }
}

void Rfid_D_Think_M50::serFlush(){
    if(useHardSerial){
        return hser->flush();
    }else{
        return sser->flush();
    }
}

void Rfid_D_Think_M50::sendByte(uint8_t b){
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
}

void Rfid_D_Think_M50::sendByte(uint8_t b, uint8_t &check){
    check ^= b;
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
}

void Rfid_D_Think_M50::sendWord(uint16_t w){
    uint8_t b;
    b = w>>8;
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
    b = w & 0x00ff;
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
}

void Rfid_D_Think_M50::sendWord(uint16_t w, uint8_t &check){
    uint8_t b;
    b = w>>8;
    check ^= b;
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
    b = w & 0x00ff;
    check ^= b;
    serWrite(b);
    if( b == 0xAA )
        serWrite(0x00);
}

void Rfid_D_Think_M50::send(uint16_t id, uint16_t cmd, uint8_t *msg, uint8_t len){
    uint8_t check=0x00 ;
    serWrite(0xAA);
    serWrite(0xBB);
    sendByte( sizeof(id) + sizeof(cmd) + len + sizeof(check) );
    sendByte(0x00);
    sendWord(id, check);
    sendWord(cmd, check);
    for(uint8_t i=0; i<len; i++)
        sendByte(msg[i], check);
    serWrite(check);
}

int16_t Rfid_D_Think_M50::nextByte(){
    long start_t = millis();
    while( !serAvailable() ){
        if( millis() - start_t > TIME_OUT )
            return FAILED;
    }
    return serRead();
}

int16_t Rfid_D_Think_M50::nextParsedByte(){
    int16_t ch = nextByte();
    if( ch == 0xAA )
        if( nextByte() != 0x00 )
            return FAILED;
    return ch;
}

int16_t Rfid_D_Think_M50::nextParsedByte(uint8_t &check){
    int16_t ch = nextByte();
    if( ch == 0xAA )
        if( nextByte() != 0x00 )
            return FAILED;
    check ^= (0xff & ch);
    return ch;
}

int16_t Rfid_D_Think_M50::nextParsedWord(uint16_t &w, uint8_t &check){
    int16_t ch;
    if( ( ch=nextParsedByte(check) ) == FAILED )
        return FAILED;
    w = ch << 8;
    if( ( ch=nextParsedByte(check) ) == FAILED )
        return FAILED;
    w |= (0xff & ch);
    return OK;
}

bool Rfid_D_Think_M50::receive(uint16_t &id, uint16_t &cmd){
    int16_t len;
    uint8_t check = 0x00;
    if( nextByte() != 0xAA ){
        delay(10);
        serFlush();
        return false;
    }
    if( nextByte() != 0xBB ){
        delay(10);
        serFlush();
        return false;
    }
    if( ( len = nextParsedByte() ) == FAILED )
        return false;
    if( nextByte() != 0x00 ){
        delay(10);
        serFlush();
        return false;
    }
    if( nextParsedWord(id, check) == FAILED )
        return false;
    if( nextParsedWord(cmd, check) == FAILED )
        return false;
    if( nextParsedByte() == FAILED ){
        delay(10);
        serFlush();
        return false;
    }
    recvLen = len -  sizeof(id) - sizeof(cmd) - 1 - sizeof(check) ;
    if( recvLen < 0 || recvLen > MAX_RECV_LEN ){
        delay(10);
        serFlush();
        return false;
    }
    for(int16_t i=0; i<recvLen; i++)
        if( ( recvBuf[i] = nextParsedByte(check) ) == FAILED )
            return false;
    if( nextByte() != check ){
        serFlush();
        return false;
    }
    if( serAvailable() ){
        serFlush();
        return false;
    }
    return true;
}

bool Rfid_D_Think_M50::session(uint16_t id, uint16_t cmd, uint8_t *msg, uint8_t len){
    uint16_t recvId, recvCmd;
    send(id, cmd, msg, len);
    if( !receive(recvId, recvCmd) )
        return false;
    if( id != BROADCAST_ID && id != recvId )
        return false;
    if( cmd != recvCmd )
        return false;
    ID = recvId;
    return true;
}

bool Rfid_D_Think_M50::session(uint16_t cmd, uint8_t *msg, uint8_t len){
    return session(ID, cmd, msg, len);
}

Rfid_D_Think_M50::Rfid_D_Think_M50(HardwareSerial &ser_in){
    hser = &ser_in;
    useHardSerial = true;
    hser->begin(DEFAULT_BAUD);
    ID = BROADCAST_ID;
}

Rfid_D_Think_M50::Rfid_D_Think_M50(HardwareSerial &ser_in, uint16_t id){
    hser = &ser_in;
    useHardSerial = true;
    hser->begin(DEFAULT_BAUD);
    ID = id;
}

Rfid_D_Think_M50::Rfid_D_Think_M50(SoftwareSerial &ser_in){
    sser = &ser_in;
    useHardSerial = false;
    sser->begin(DEFAULT_BAUD);
    ID = BROADCAST_ID;
}

Rfid_D_Think_M50::Rfid_D_Think_M50(SoftwareSerial &ser_in, uint16_t id){
    sser = &ser_in;
    useHardSerial = false;
    sser->begin(DEFAULT_BAUD);
    ID = id;
}

// 开启天线，并指定射频工作模式
bool Rfid_D_Think_M50::enableAntenna(uint8_t rfidMode){
    if( !session(0x0801, &rfidMode, 1) )
        return false;
    return recvLen == 0;
}

// 开启天线，并使用上次设置的射频工作模式
bool Rfid_D_Think_M50::enableAntenna(){
    uint8_t sendByte = 0xff;
    if( !session(0x0C01, &sendByte, 1) )
        return false;
    return recvLen == 0;
}

// 关闭天线
bool Rfid_D_Think_M50::disableAntenna(){
    uint8_t sendByte = 0x00;
    if( !session(0x0C01, &sendByte, 1) )
        return false;
    return recvLen == 0;
}

bool Rfid_D_Think_M50::getModelInfo(char *infoBuffer){
    if( !session(0x0401, NULL, 0) )
        return false;
    memcpy(infoBuffer, recvBuf, recvLen);
    infoBuffer[recvLen] = 0;
    return true;
}

bool Rfid_D_Think_M50::setId(uint16_t id){
    uint8_t pack[] = { (uint8_t)(id>>8), (uint8_t)(id&0xff) };
    uint16_t recvId, recvCmd;
    const uint16_t cmd=0x0201;
    send(ID, cmd, pack, sizeof(pack) );
    if( !receive(recvId, recvCmd) )
        return false;
    if( id != recvId )
        return false;
    if( cmd != recvCmd )
        return false;
    ID = id;
    return true;
}

bool Rfid_D_Think_M50::singleInventory(uint8_t &dsfid, uint8_t uid[]){
    if( !session(0x0110, NULL, 0) ) 
        return false;
    if( recvLen != 9 )
        return false;
    dsfid = recvBuf[0];
    memcpy(uid, recvBuf+1, 8);
    return true;
}

bool Rfid_D_Think_M50::multiInventory(uint8_t &cnt, uint8_t dsfids[], uint8_t uids[][8]){
    if( !session(0x0010, NULL, 0) ) 
        return false;
    if( recvLen % 9  != 0 )
        return false;
    cnt = recvLen / 9;
    for(uint8_t i=0; i<cnt; i++){
        dsfids[i] = recvBuf[i*9];
        memcpy(uids[i], recvBuf+i*9+1, 8);
    }
    return true;
}

bool Rfid_D_Think_M50::read(uint8_t uid[], uint8_t blockNo, uint8_t blockCnt, uint8_t data[]){
    uint8_t pack[11];
    pack[0] = 0x00;
    memcpy(pack+1, uid, 8);
    pack[9] = blockNo;
    pack[10]= blockCnt;
    if( !session(0x0510, pack, sizeof(pack) ) ) 
        return false;
    if( recvLen != blockCnt*4 )
        return false;
    memcpy(data, recvBuf, recvLen);
    return true;
}

bool Rfid_D_Think_M50::read(uint8_t uid[], uint8_t blockNo, uint8_t data[]){
    return read(uid, blockNo, 1, data);
}

bool Rfid_D_Think_M50::read(uint8_t blockNo, uint8_t blockCnt, uint8_t data[]){
    uint8_t dsfid, uid[8];
    if( !singleInventory(dsfid, uid) )
        return false;
    return read(uid, blockNo, blockCnt, data);
}

bool Rfid_D_Think_M50::read(uint8_t blockNo, uint8_t data[]){
    return read(blockNo, 1, data);
}

bool Rfid_D_Think_M50::write(uint8_t uid[], uint8_t blockNo, uint8_t data[]){
    uint8_t pack[14];
    pack[0] = 0x00;
    memcpy(pack+1, uid, 8);
    pack[9] = blockNo;
    memcpy(pack+10, data, 4);
    return session(0x0610, pack, sizeof(pack) ); 
}

bool Rfid_D_Think_M50::write(uint8_t blockNo, uint8_t data[]){
    uint8_t dsfid, uid[8];
    if( !singleInventory(dsfid, uid) )
        return false;
    return write(uid, blockNo, data);
}

bool Rfid_D_Think_M50::getLockStatus(uint8_t uid[], uint8_t blockNo, bool &isLocked){
    uint8_t pack[11];
    pack[0] = 0x00;
    memcpy(pack+1, uid, 8);
    pack[9] = blockNo;
    pack[10] = 0x01;
    if( !session(0x0D10, pack, sizeof(pack) ) ) 
        return false;
    if( recvLen != 1 )
        return false;
    if(recvBuf[0]==0){
        isLocked = false;
    }else{
        isLocked = true;
    }
    return true;
}

bool Rfid_D_Think_M50::getLockStatus(uint8_t blockNo, bool &isLocked){
    uint8_t dsfid, uid[8];
    if( !singleInventory(dsfid, uid) )
        return false;
     return getLockStatus(uid, blockNo, isLocked);
}

bool Rfid_D_Think_M50::lock(uint8_t uid[], uint8_t blockNo){
    uint8_t pack[10];
    pack[0] = 0x00;
    memcpy(pack+1, uid, 8);
    pack[9] = blockNo;
    session(0x0710, pack, sizeof(pack) );
    bool isLocked;
    if( !getLockStatus(uid, blockNo, isLocked) )
        return false;
    return isLocked;
}

bool Rfid_D_Think_M50::lock(uint8_t blockNo){
    uint8_t dsfid, uid[8];
    if( !singleInventory(dsfid, uid) )
        return false;
    return lock(uid, blockNo);
}
