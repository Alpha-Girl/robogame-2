// Arduino D-Think_M50 模块驱动库
// 2016/08/19
// Author: WangXuan


#ifndef  _RFID_D_THINK_M50_H_
#define  _RFID_D_THINK_M50_H_


#include <Arduino.h>
#include <SoftwareSerial.h>


// Rfid 类，用于控制一个 D-Think_M50 模块
class Rfid_D_Think_M50{
public:
    // 射频工作模式
    const uint8_t ISO15693  = '1';
    
    
    
    // ----------------------------构造函数--------------------------------------
    // 构造函数，不知道模块ID，使用广播ID
    Rfid_D_Think_M50(HardwareSerial &ser_in);
    // 构造函数，并设置已知的模块地址
    Rfid_D_Think_M50(HardwareSerial &ser_in, uint16_t id);
    // 构造函数，不知道模块ID，使用广播ID
    Rfid_D_Think_M50(SoftwareSerial &ser_in);
    // 构造函数，并设置已知的模块地址
    Rfid_D_Think_M50(SoftwareSerial &ser_in, uint16_t id);
    // ----------------------------------------------------------------------------

    
    
    // ----------------------关于操作读卡器的函数----------------------------------
    // 使能天线并设置工作模式
    bool enableAntenna(uint8_t rfidMode);
    // 使能天线，使用以往设置好的工作模式
    bool enableAntenna();
    // 关闭天线
    bool disableAntenna();
    // 获取卡片型号、版本字符串，infoBuffer至少要有16B的空间
    bool getModelInfo(char *infoBuffer);
    // 设置读卡器ID，用于一个串口上挂载多个读卡器
    // 读卡器出厂ID都为0x1112，但你可以修改两个读卡器的ID使它们不同，这样这两个读卡器就能挂载在同一个串口上
    // 若要区分两个ID不同，挂载在同一个串口上的读卡器，请在构造函数里
    bool setId(uint16_t id);
    // ----------------------------------------------------------------------------

    
    
    // -----------------------------关于操作卡片的函数------------------------------
    // ISO_15693 单卡发现
    bool singleInventory(uint8_t &dsfid, uint8_t uid[]);
    // ISO_15693 多卡发现
    bool multiInventory(uint8_t &cnt, uint8_t dsfids[], uint8_t uids[][8]);
    // ISO_15693 读多块操作，已发现卡片（已知卡片UID）
    bool read(uint8_t uid[], uint8_t blockNo, uint8_t blockCnt, uint8_t data[]);
    // ISO_15693 读单块操作，已发现卡片（已知卡片UID）
    bool read(uint8_t uid[], uint8_t blockNo, uint8_t data[]);
    // ISO_15693 读多块操作，自动发现卡片（未知卡片UID）
    bool read(uint8_t blockNo, uint8_t blockCnt, uint8_t data[]);
    // ISO_15693 读单块操作，自动发现卡片（未知卡片UID）
    bool read(uint8_t blockNo, uint8_t data[]);
    // ISO_15693 写单块操作，已发现卡片（已知卡片UID）
    bool write(uint8_t uid[], uint8_t blockNo, uint8_t data[]);
    // ISO_15693 写单块操作，自动发现卡片（未知卡片UID）
    bool write(uint8_t blockNo, uint8_t data[]);
    // ISO_15693 读单块锁状态操作，已发现卡片（已知卡片UID）
    bool getLockStatus(uint8_t uid[], uint8_t blockNo, bool &isLocked);
    // ISO_15693 读单块锁状态操作，自动发现卡片（未知卡片UID）
    bool getLockStatus(uint8_t blockNo, bool &isLocked);
    // ISO_15693 锁单块操作，已发现卡片（已知卡片UID）
    bool lock(uint8_t uid[], uint8_t blockNo);
    // ISO_15693 锁单块操作，自动发现卡片（未知卡片UID）
    bool lock(uint8_t blockNo);
    // ----------------------------------------------------------------------------

    
    
    
    
    
    
    
    
private:
    // 指示函数返回状态，失败
    static const int16_t  FAILED  = 0xffff;
    // 指示函数返回状态，成功
    static const int16_t  OK =      0x0000;
    
    // 广播ID
    const uint16_t BROADCAST_ID = 0x0000;
    // 模块ID
    uint16_t ID;

    // 模块上电后的默认波特率
    static const long DEFAULT_BAUD = 19200;
    // 模块使用的硬件UART
    HardwareSerial *hser;
    // 模块使用的软件件UART
    SoftwareSerial *sser;
    // 指示模块使用的硬件还是软件UART
    bool useHardSerial;

    void serWrite(uint8_t b);
    int serAvailable();
    int serRead();
    void serFlush();
    
    void sendByte(uint8_t  b);
    void sendByte(uint8_t  b, uint8_t &check);
    void sendWord(uint16_t w);
    void sendWord(uint16_t w, uint8_t &check);
    
    int16_t nextByte();
    int16_t nextParsedByte();
    int16_t nextParsedByte(uint8_t &check);
    int16_t nextParsedWord(uint16_t &w, uint8_t &check);

    // 串口接收超时时间(ms)
    static const long    TIME_OUT     = 64;
    // 链路层帧的最大载荷长度
    static const uint8_t MAX_RECV_LEN = 64;
    // 链路层载荷的接收缓冲区
    uint8_t recvBuf[MAX_RECV_LEN];
    // 接收到的链路层载荷的实际长度
    uint8_t recvLen;

    // 函数: send
    // 功能: 按模块通信协议封装并发送数据帧（链路层发送）
    // 参数:
    //       id:   模块的识别号
    //       cmd:  命令码
    //       msg:  待封装的字节数组的首地址
    //       len:  待封装的字节数组的长度
    void send(uint16_t id, uint16_t cmd, uint8_t *msg, uint8_t len);
    // 链路层接收，接收到的载荷放在 recvBuf
    bool receive(uint16_t &id, uint16_t &cmd);
    // 链路层单次会话（先发后收），接收到的载荷放在 recvBuf
    bool session(uint16_t id, uint16_t cmd, uint8_t *msg, uint8_t len);
    // 使用已知的模块ID进行链路层单次会话（先发后收），接收到的载荷放在 recvBuf
    bool session(uint16_t cmd, uint8_t *msg, uint8_t len);
    
};


#endif
