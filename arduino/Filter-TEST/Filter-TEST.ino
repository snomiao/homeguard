typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned int uint;


// DEBUG: //
void inline DEBUG_Setup(){
    Serial.begin(115200);
}

#define DEBUG   (Serial.print  )
#define DEBUGln (Serial.println)



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
// 分别对应 0, 1, -1 三种不同的信号。
//
class SignalSensor{
    public:
    int signal(unsigned int new_value){

        // calc diff
        static unsigned int value = new_value;
        static unsigned int hitDiff = 6;
        
        int diff = new_value - value;
        value += diff;

        // 忽略变化缓慢的信号
        if(diff / hitDiff == 0){
            return 0;
        }


        
        // calc signal
        int signal = diff > 0 ? 1 : -1;
        static int lastSignal = signal;

        // ignore repeat signal
        if (lastSignal == signal){
            return 0;
        }
        lastSignal = signal;

        // ok
        return signal;
    }
    SignalSensor(){

    }
};

////////////////////////////////
// MorseCodeSensor 莫斯电码接收器
//
class MorseCodeSensor{
    private:
    typedef int SIGNAL;

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
        static long long int morseCode = 1;  // 空信号
        static int           allowSpace = 0;
        
        // 返回字符, 此处用 0 表示没有字符
        char c = 0;
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
                        
                        c = '_';
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

// MAIN
int OUTPUT_rawdata    = 0;
int OUTPUT_dataFilted = 0;
int OUTPUT_signal     = 0;
int OUTPUT_morse      = 1;


FilterShake     fs  = FilterShake(80);
FilterShiftMean fsm = FilterShiftMean();
SignalSensor    ss  = SignalSensor();
MorseCodeSensor mcs = MorseCodeSensor(250); // 1/4s


void setup(){
    DEBUG_Setup();
    pinMode(A0, INPUT);
}
void loop(){

    //command control
    switch (Serial.read()) {
        case '1':
        fs.N++;
        DEBUG("=========================");
        DEBUGln(fs.N);                                          break;
        case '2':
        fs.N--;
        DEBUG("=========================");
        DEBUGln(fs.N);                                          break;
        case '3':
        DEBUG("=========================");
        //DEBUGln(lastData);                                    break;
        case 'q':
        OUTPUT_rawdata = OUTPUT_rawdata xor 1;                  break;
        case 'w':
        OUTPUT_dataFilted = OUTPUT_dataFilted xor 1;            break;
        case 'e':
        OUTPUT_signal = OUTPUT_signal xor 1;                    break;
        case 't':
        OUTPUT_morse = OUTPUT_morse xor 1;                      break;
    }

    // PUT MEAN DATA
    int datanow = analogRead(A0);
    if( OUTPUT_rawdata){
        DEBUG("datanow: ");
        DEBUG(datanow);
        for(int i=0; i<datanow / 15; i++){
            DEBUG("/");
        }
        DEBUGln();
    }

    // data
    int dataFilted = fs.filter( fsm.filter(datanow) );
    static int lastDataFilted = dataFilted;
    if(OUTPUT_dataFilted && lastDataFilted!=dataFilted){
        DEBUG("dataFilted"); DEBUGln(dataFilted);
        lastDataFilted = dataFilted;
    }

    // signal
    int signal = ss.signal(dataFilted);
    if(OUTPUT_signal && signal != 0){
        DEBUGln( signal > 0 ? "///////////" : "\\\\\\\\\\");
    }

    // morse code
    unsigned char morseChar = mcs.signal(signal);
    static unsigned char lastMorseChar;
    if(OUTPUT_morse && lastMorseChar != morseChar && morseChar != 0){
        lastMorseChar = morseChar;
        // show rhythm Mark
        #define MORSEBUFLEN 32
        static unsigned char c[MORSEBUFLEN] = "|||||||||||||||||||||||||||||||";
        for(int i=0; i < MORSEBUFLEN - 2; i++){
            c[i] = c[i+1];
        }
        c[MORSEBUFLEN - 2] = morseChar;

        DEBUG((const char*)c);
        DEBUGln("");
    }
}
 