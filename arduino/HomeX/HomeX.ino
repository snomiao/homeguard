//#include <stdlib.h>


#include "ESP8266.h"
#include <EEPROM.h>

/* Program Struction

HomeX

    DEBUG(macros)
        setup
        macros

    EEPROM
        load
        save

    ESP8266
        setup
        sync
            function: dataConvert
            GET
                data from other module
            RESPOND
                data to other module

    74HC595
        setup
        shift out

    SETUP
        DEBUG -> setup
        74HC595 -> setup
        ESP8266 -> setup

    LOOP
        control: 74HC595
            control: ESP8266(interval limited)

            */


/* Connection: ESP8266 -> Arduino

    ESP8266_TX->RX(D3)
    ESP8266_RX->TX(D2)
    ESP8266_CH_PD->3.3V
    ESP8266_VCC->3.3V
    ESP8266_GND->GND
    */


// User Setting
#define DeviceUUID  "4f5c81ca-bb66-4435-acc5-c95bebfd6f60"
#define SyncInterval 2000 // ms
#define WLANNAME "054"
#define WLANPASS "88647410"



// DEBUG: //

#define DEBUG   (Serial.print  )
#define DEBUGln (Serial.println)
/*
    #define DEBUGOK(string, operation) \
        DEBUG(string);                 \
        if (operation) {               \
            DEBUG("OK\n");             \
        } else {                       \
            DEBUG("FAIL\n");           \
            return 1;                  \
        };
        */
        void inline DEBUG_Setup()
        {
            Serial.begin(9600);
        }

// EEPROM: //
    int inline EEPROM_Get(int addr){
        int data = 0;
        EEPROM.get(addr, data);
        return data;
    }

    void inline EEPROM_Put(int addr, int data){
        EEPROM.put(addr, data);
    }
    

// ESP8266: //


    //sync
    //    function: dataConvert
    //    GET
    //        data from other module
    //    RESPOND
    //        data to other module




















////////////////////////////////
// FilterShake 消除抖动
// 设置一个滤波计数器，将每次采样值与当前有效值比较：
// 如果采样值 == 当前有效值，则计数器清零；
// 如果采样值 != 当前有效值，则计数器 +1，并判断计数器是否>=上限N（溢出）；
// 如果计数器溢出，则将本次值替换当前有效值，并清计数器。
//
// 整理：shenhaiyu 2013-11-01
//
class FilterShake{
    public:
    unsigned int N; // unequal value count
    unsigned int filter(unsigned int new_value){
        static unsigned int value = new_value;
        static int i = 0;

        // if it is not shaking
        if(value == new_value){
            i = 0;
            return value;
        };

        // counting unequal value
        i++;
        if(i > N){
            i = 0;
            value = new_value;
        }
        return value;
    }
    FilterShake(int n){
        N = n;
    }
};

////////////////////////////////
// FilterShiftMean 递推加权均值
//
class FilterShiftMean{
    #define FilterShiftMean_N 12
    private:
    int values[FilterShiftMean_N] = {0};
    public:

    int filter(int new_value){
        // TODO
        // 递推
        for(int i=0; i < FilterShiftMean_N - 1; i++){
            values[i] = values[i + 1];
        }
        // 新数字
        values[FilterShiftMean_N - 1] = new_value;
        // 加权求和
        long int sum = 0;
        for(int i=0; i < FilterShiftMean_N; i++){
            sum += values[i] * (i + 1);
        }

        // 求出加权数
        int div = 0;
        for(int i=0; i < FilterShiftMean_N; i++){
            div += i + 1;
        }

        // 求均值
        return sum / div;
    }
    FilterShiftMean(){

    }
};


////////////////////////////////
// SignalSensor 信号感受器
//
// 把输入的数据流分成 静止、上升沿、下降沿 三种状态
// 分别对应 0, 1, 2 三种不同的信号。
//
class SignalSensor {
    private:
    typedef unsigned int SIGNAL;
    
    int threshold;
    int value;
    SIGNAL lastSignal;
    public:
    SIGNAL getSignal(int new_value){
        // 计算时间差
        int diff = new_value - value;
        value += diff;

        // 忽略变化缓慢的信号
        diff /= threshold; // 附加阈值
        
        if(diff == 0){
            return 0;
        }

        // 计算信号值
        SIGNAL signal = diff > 0 ? 1 : 2;

        // 忽略重复信号
        if (lastSignal == signal){
            return 0;
        }
        lastSignal = signal;

        // 完成
        return signal;
    }
    SignalSensor(int pHitDiff){
        threshold = pHitDiff;
    }
};

////////////////////////////////
// MorseCodeSensor 莫斯电码接收器
//
class MorseCodeSensor{
    private:
    typedef unsigned int SIGNAL;

    char* MORSE = "%%ETIANMSURWDKGOHVF?LAPJBXCYZQ%%54%3% %2 %+ %%%16=/ % % 7 %%8 90            ?_    \"  .    @   '  -        ;! (     ,    :       ";

    char morseChar(long long int code){
        if(code < 129){
            char c = MORSE[code];
            if(c != ' ')
            return c;
        }
        return 0;
    }

    public:
    int interval;  // 间隔
    MorseCodeSensor(int interval){
        this->interval = interval;
    }
    char signal(SIGNAL s) {

        static long int      lastTime = 0;  // 前一个有效信号的时间戳
        static SIGNAL        lastSignal;    // 前一个有效信号
        static SIGNAL        signalUp = 1;   // 定义它为上升沿信号
        static long long int morseCode = 1;  // 空码
        static int           allowSpace = 0;
        
        // 返回字符, 此处用 -1 表示没有字符
        char c = -1;
        // 计算信号长度
        long int diff = millis() - lastTime;
        

        // 如果一个状态保持了很久，那么可以认为这个有效信号是上升沿信号（打破了状态）
        //
        // ``\__________________________/`````\___
        //                             (^)
        //
        // 
        if(diff >= interval * 14){
            signalUp = s;
        };

        // 处理 LOW 心跳信号，和上升沿信号, 输出字符
        //
        // ``````\___________/```````````
        //      (^^^^^^^^^^^^)
        //
        if( (0 == s && lastSignal != signalUp) || s == signalUp ){
            // 停顿 3 个 "." 之后，输出字符
            if( diff >= interval * 3 ){
                if( morseCode != 1 ){
                    allowSpace = 1;
                    
                    c = morseChar(morseCode);
                    morseCode = 1;
                }
            }
            // 停顿 7 个 "." 之后，输出空格
            if( diff >= interval * 7 ){
                if( morseCode != 1 ){
                    allowSpace = 1;
                    
                    c = morseChar(morseCode);
                    morseCode = 1;

                    }else{
                        if(allowSpace){
                            allowSpace = false;

                            c = ' ';
                            morseCode = 1;
                        }
                    }
                }
            };

        // 处理有效信号的时间戳
        //
        // `````\_____/`````\_____/`````\_____
        //     (^)   (^)   (^)   (^)   (^) 
        //
        if(lastSignal == s || 0 == s){
            return c;
        };
        lastSignal = s;
        lastTime = lastTime - -diff;

        // 处理下降沿信号（也就是放开按键）
        //
        // ``````\___________/```````````
        //      (^)
        //
        if(signalUp != s){
            morseCode <<= 1;
            if( diff >= interval * 2 ){ /*取1和3的平均*/
                morseCode |= 1; // -
                }else{
                morseCode |= 0; // .
            }
        }
        return c;
    }
};


// 74HC595芯片
class C74HC595{
    private:
    int ST_CP;
    int SH_CP;
    int DS   ;
    public:
    C74HC595(int pST_CP, int pSH_CP, int pDS){
        ST_CP = pST_CP;
        SH_CP = pSH_CP;
        DS    = pDS   ;

        pinMode(ST_CP, OUTPUT);
        pinMode(SH_CP, OUTPUT);
        pinMode(DS   , OUTPUT);
    };
    void setData(int data){
        static int lastData = 0;
        
        if(data == lastData){
            return;
        }
        lastData = data;


        digitalWrite(ST_CP, LOW);
        shiftOut(DS, SH_CP, MSBFIRST, data);
        digitalWrite(ST_CP, HIGH);
    };
};

// 4路继电器
class Relay4{
    private: 
    int data;
    int VCC;
    int interval;
    public:
    Relay4(int pVCC, int pInterval){
        interval = pInterval;
        VCC = pVCC;

        pinMode(VCC, OUTPUT);
    }
    void switchVCC(int on){
        digitalWrite(VCC, on);
    }
    void xorData(int pData){
        data = (data xor pData) & 0xF;
    }
    void setData(int pData){
        data = pData & 0xF;
    }
    int getRealData(){
        return data;
    }
    int getData(){
        static long int tLastUpdate = 0;
        static int dataOut = 0;
        // 一次只改变一个位，防止电流剧变导致跳闸

        // 计算 2次跳动的时间差
        int diff = millis() - tLastUpdate;
        if(diff < interval)
        return dataOut;
        tLastUpdate += diff;
        
        //
        int originalDataOut = dataOut;

        // 先找出需要关的
        for(int step=0; step<4; step++){
            dataOut |= (data &  (1 << step)); // cut!
            if(dataOut != originalDataOut)
            return dataOut;
        }

        // 然后再找需要开的
        for(int step=0; step<4; step++){
            dataOut &= (data | ~(1 << step)); // connect!
            if(dataOut != originalDataOut)
            return dataOut;
        }
        return dataOut;
    }
};

class CommandParser{
    private:
    char*buf;
    unsigned int  len;
    public:
    unsigned int needRematch;
    CommandParser(unsigned int pLen){
        len = pLen;
        buf = (char*)malloc(len);
        buf[len-1] = 0;
        needRematch = 0;
    }
    ~CommandParser(){
        free(buf);
    }
    void putCh(char c){
        // 无数据，跳过
        if(c == -1){
            return;
        }

        char tmpBuf[2];
        tmpBuf[0] = c;
        tmpBuf[1] = 0;
        DEBUG((const char*)tmpBuf);
        //DEBUGln("");

        // 最后一位是\0，对于有新输入的情况，除了\0全部左移, 倒数第1位作为新字符
        for(int i = 0; i < len - 2; i++){
            buf[i] = buf[i+1];
        }
        buf[len-2] = c;
        needRematch = 1;
    }
    void putStr(char*str){
        unsigned int len = strlen(str);
        
        // TODO: for展开
        for(int i=0; i < len; i++){
            putCh(str[i]);
        }
    }
    int match(const char* cmd){
        int lenCmd = strlen(cmd);
        return !strcmp(cmd, (const char*)(buf + len - lenCmd - 1 ));
    }
    int needMatch(){
        if(needRematch){
            needRematch = 0;
            return 1;
        }
        else{
            return 0;
        }
    }
};


// class MorseSender{
//     MorseSender{

//     }
// }


//long long int 64

// // 250为一个
// class MorseBeeper{
//     private:
//     unsigned long long int beepQueue = 1;
//     unsigned long long int tLastSwitch = 0;
//     unsigned int pinBeeper;
//     unsigned int interval;
//     int state = 0;
//     public:
//     unsigned int needRematch;
//     Beeper(int pPinBeeper){
//         pinBeeper = pPinBeeper;
//         pinMode(pinBeeper, OUTPUT);
//     }
//     void ask(){
//         // 计算上次输出的时间差
//         int diff = millis() - tLastSwitch;
//         if(diff < interval){
//             return;
//         }
//             tLastSwitch += diff;

//         // 从高位往右扫描，找到第1个1，再右边就是要beep的内容
//         int i;
//         int newState;
//         for(i=0; i < sizeof(beepQueue) * 8 - 1; i++){
//             if( beepQueue & (1 << (sizeof(beepQueue) * 8 - i))){
//                 // 把最高位的 1 置 0
//                 beepQueue &= ~(1 << (sizeof(beepQueue) * 8 - i));
//                 // 之前最高位的 1 的右边那位，就是决定现在要不要 beep 的位了
//                 newState = !!(1 << (sizeof(beepQueue) * 8 - i - 1));
//                 // 把之前最高位的 1 的右边那位置 1
//                 beepQueue |= (1 << (sizeof(beepQueue) * 8 - i - 1));

//                 // 处理掉最高位的 1 后跳出
//                 break;
//             }
//         }

//         // 如果 beep 队列是空的，就停止 beep
//         if(i == sizeof(beepQueue) * 8 - 1){
//             newState = 0;
//         }

//         // 输出
//         if(state != newState){
//             digitalWrite(pinBeeper, newState);
//             state = newState;
//         }
//     }
//     void beepMorseChar(char morse char){
//         startTime = millis();
//         morse 
//         for(int i=0; i<; i++){
            
//         }
//     }
//     void beepTime(int time){
//         int n = time / 250; // 相当于 n 个 dot 连起来
//         ..-. => 



//         digitalWrite(pinBeeper, HIGH);
//         stopTime = millis() + time;
//     }
// };





/*
    Connection: RELAY4 -> 74HC595
        IN1 -> Q0(15)
        IN2 -> Q1(1)
        IN3 -> Q2(2)
        IN4 -> Q3(3)

    Connection: RELAY4 -> Arduino
        JD-VCC -> 5V
        VCC    -> D4
        */

/*
    Connection: 74HC595 -> Arduino
        ST_CP(12)   ->  D7
        SH_CP(11)   ->  D8
        DS(14)      ->  D12

        OE(13)      ->  GND                                                                     
        MR(10)      ->  5V
        */



// MAIN

///////////////////////////////////////////////////////////////////////////////
// 输出选项
//
int OUTPUT_rawdata    = 0;
int OUTPUT_dataFilted = 0;
int OUTPUT_signal     = 1;
int OUTPUT_morse      = 1;

///////////////////////////////////////////////////////////////////////////////
// EEPROM
#define ADDRESS_Relays 0

///////////////////////////////////////////////////////////////////////////////
// 用户命令
//
CommandParser cmd = CommandParser(80/*lenbuf*/);

///////////////////////////////////////////////////////////////////////////////    
// 莫斯电码输入（光敏电阻）
const int       morseInPin    = A0;
FilterShake     morseInFs     = FilterShake(80);
FilterShiftMean morseInFsm    = FilterShiftMean();
SignalSensor    morseInSs     = SignalSensor(3/*触发阈值*/);
MorseCodeSensor morseInSensor = MorseCodeSensor(125); // 1/8s

void morseIn(){
    // 收集传感电阻
    int datanow = analogRead(morseInPin);
    if( OUTPUT_rawdata){
        DEBUG("datanow: ");
        DEBUG(datanow);
        for(int i=0; i<datanow / 15; i++){
            DEBUG("/");
        }
        DEBUGln();
    }

    // 对数据进行滤波
    int dataFilted = morseInFs.filter( morseInFsm.filter(datanow) );
    static int lastDataFilted = dataFilted;
    if(OUTPUT_dataFilted && lastDataFilted != dataFilted){
        DEBUG("dataFilted"); DEBUGln(dataFilted);
        lastDataFilted = dataFilted;
    }

    // 解读光敏电阻数据中的信号
    int signal = morseInSs.getSignal(dataFilted);
    if(OUTPUT_signal && signal != 0){
        //DEBUGln( signal > 0 ? "/////" : "\\\\\\\\\\");
        DEBUG("SIGNAL: ");
        DEBUGln(  signal);
    }

    // 解析摩斯密码字符
    char morseChar = morseInSensor.signal(signal);
    static char lastMorseChar;
    if(OUTPUT_morse && morseChar != -1){
        lastMorseChar = morseChar;

        DEBUG("MORSE: ");
        DEBUG(morseChar);
        DEBUGln("");
    }

    // 将摩斯电码输出到命令
    cmd.putCh(morseChar);
}

// 莫斯电码输出（激光头）

// 红外人体感应器
const int       infraredPin = 11;
SignalSensor    infraredSs  = SignalSensor(1/*触发阈值*/);

// 继电器
Relay4 relay = Relay4(4/*pin4*/, 100/*ms*/);
C74HC595 c595 = C74HC595(7/*ST_CP*/, 8/*SH_CP*/, 12/*DS*/);

void handleCommand(){
    if(cmd.needMatch()){
        if(cmd.match("PING")){DEBUGln("PING");}
        else if(cmd.match("/INFRARED TRIG")){DEBUGln("HUMAN!!!!!!!!!!!!!!!!");}
        else if(cmd.match("/s rawdata"   )) { OUTPUT_rawdata    = !OUTPUT_rawdata;    }
        else if(cmd.match("/s dataFilted")) { OUTPUT_dataFilted = !OUTPUT_dataFilted; }
        else if(cmd.match("/s signal"    )) { OUTPUT_signal     = !OUTPUT_signal;     }
        else if(cmd.match("/s morse"     )) { OUTPUT_morse      = !OUTPUT_morse;      }

        else if(cmd.match("/s q")) { OUTPUT_rawdata    = !OUTPUT_rawdata;    }
        else if(cmd.match("/s w")) { OUTPUT_dataFilted = !OUTPUT_dataFilted; }
        else if(cmd.match("/s e")) { OUTPUT_signal     = !OUTPUT_signal;     }
        else if(cmd.match("/s r")) { OUTPUT_morse      = !OUTPUT_morse;      }

        else if(cmd.match("S1")) { relay.xorData(1); }
        else if(cmd.match("S2")) { relay.xorData(2); }
        else if(cmd.match("S3")) { relay.xorData(4); }
        else if(cmd.match("S4")) { relay.xorData(8); }

        else if(cmd.match("IE")) { relay.xorData(1); }
        else if(cmd.match("II")) { relay.xorData(2); }
        else if(cmd.match("IS")) { relay.xorData(4); }
        else if(cmd.match("IH")) { relay.xorData(8); }
        //else if(cmd.match("SOS ")) { beeper.beep(200) }
    }
}

void setup(){
    //////////////////////////////////
    // 调试器
    DEBUG_Setup();

    //////////////////////////////////
    // 光敏电阻输入
    // 
    pinMode(morseInPin, INPUT);

    //TODO: 红外人体感应器

    //////////////////////////////////
    // 调试指示灯
    //
    pinMode(13, OUTPUT);
    

    //////////////////////////////////
    // 继电器初始化
    //
    // 继电器的初始状态：默认全关
    // 输出到595
    c595.setData( 0 | ~(relay.getData()) );
    // 打开继电器开关
    relay.switchVCC(HIGH);

    // 恢复继电器状态
    int initRelay = EEPROM_Get(ADDRESS_Relays);
    relay.setData(initRelay);
}

void loop(){
    /////////////////////////////////////////////////////////////////////
    // 处理命令
    handleCommand();

    // 处理输入莫斯电码
    morseIn();

    /////////////////////////////////////////////////////////////////////
    // 串口的数据输出到命令
    cmd.putCh( Serial.read() );

    /////////////////////////////////////////////////////////////////////
    // 处理红外人体感应信号
    switch( infraredSs.getSignal(digitalRead(infraredPin) ? 1 : 0)){
        case 1:
        cmd.putStr("/INFRARED TRIG");
        break;
    }
    

    /////////////////////////////////////////////////////////////////////
    // 继电器测试
    {
        // static int i = 0;
        // static long int lastPlusI = 0;
        // int diff = millis() - lastPlusI;
        // if( diff > 1000){
        //     lastPlusI += diff;
        //     relay.setData(i++);
        //     i %= 16;
        // }
    }

    // 处理 74HC595
    c595.setData( 0 |
        // 处理继电器
        ~(relay.getData()) );

    static int lastRelayData = -1;
    int relayData = relay.getRealData();
    if(lastRelayData != relayData ){
        lastRelayData = relayData;
        EEPROM_Put(ADDRESS_Relays, relayData);
    }
}