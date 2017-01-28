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
        }

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
        int diff = new_value - value;
        value += diff;

        // ignore repeat value
        if(diff == 0){
            return 0;
        }
        
        // calc signal
        int signal = diff > 0 ? 1 : -1;
        static int last_signal = signal;

        // ignore repeat signal
        if (last_signal == signal){
            return 0;
        }
        last_signal = signal;

        // ok
        return signal;
    }
    SignalSensor(){

    }
};

////////////////////////////////
// RhymSensor 节拍感受器
// 
// 输入一串信号，以 0 表示静默，其它数字为不同的有效信号。
// 提取首个有效信号，作为节拍的开始，对后续同种信号计算节拍。
// 实时输出节拍。
// 如出现持续静默，则下次需重新提取首个有效信号，作为节拍的开始。
// 现最长支持64个节拍
//
class RhymSensor{
    public:
    unsigned int interval;
    RhymSensor(unsigned int pInterval){
        interval    = pInterval;
    }

    unsigned long long int rhym(unsigned int signal){
        static unsigned long long int rhym_section = 0; //64bit

        // pick first signal
        static long int t_prev_valid_signal;
        static unsigned int first_signal = 0;
        if( first_signal == 0){
            t_prev_valid_signal = millis();
            first_signal  = signal;
            rhym_section  = 1; // ___X
            return rhym_section;
        }

        // no signal
        if(signal == 0){
            return 0;
        }

        // signal delta time
        int diff = millis() - t_prev_valid_signal;
        
        // calc rhym shift
        unsigned int rhym_shift = (diff + interval / 2) / interval;

        // word interval
        if(rhym_shift > 4){
            t_prev_valid_signal = millis();
            first_signal  = signal;
            rhym_section  = 1; // ___X
            return rhym_section;
        }

        // calc rhym
        if( rhym_shift < sizeof(rhym_section) * 8 ){
            
            if(rhym_shift <= 4){ // sentence interval = 4 * interval
                // calc rhym_section
                if(signal == first_signal){
                    DEBUG("diff: "); DEBUGln(diff);
                    rhym_section = rhym_section << rhym_shift | 1; // _X_X

                    t_prev_valid_signal += diff;
                    return rhym_section;
                }
                //DEBUGln("SIGNAL WUGUAN");
            }
        }
        return 0;
    }
};

// MAIN
int OUTPUT_rawdata = 0;
int OUTPUT_dataFilted = 0;
int OUTPUT_signal = 1;
int OUTPUT_rhym = 1;

FilterShake fs = FilterShake(80);
FilterShiftMean fsm = FilterShiftMean();
SignalSensor ss = SignalSensor();
RhymSensor rs = RhymSensor(250); // 1/4s


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
        DEBUGln(fs.N);                                        break;
        case '2':
        fs.N--;
        DEBUG("=========================");
        DEBUGln(fs.N);                                        break;
        case '3':
        DEBUG("=========================");
        //DEBUGln(lastData);                                    break;
        case 'q':
        OUTPUT_dataFilted = OUTPUT_dataFilted xor 1;          break;
        case 'w':
        OUTPUT_signal = OUTPUT_signal xor 1;                  break;
        case 'e':
        OUTPUT_rhym = OUTPUT_rhym xor 1;                  break;
        case 'r':
        OUTPUT_rawdata = OUTPUT_rawdata xor 1;                  break;
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
        DEBUG("datanow"); DEBUGln(datanow);
        DEBUG("dataFilted"); DEBUGln(dataFilted);
        lastDataFilted = dataFilted;
    }

    // signal
    int signal = ss.signal(dataFilted);
    if(OUTPUT_signal && signal != 0){
        DEBUGln( signal > 0 ? "///////////" : "\\\\\\\\\\");
    }

    // rhym
    unsigned long long int rhym = rs.rhym(signal);
    static unsigned long long int lastrhym;
    if(OUTPUT_rhym && rhym != lastrhym){
        lastrhym = rhym;
        // show rhythm Mark
        for(int i = sizeof(rhym) * 8 - 1; i >= 0; i--){
            // 从大端读起
            DEBUG( (int)((rhym >> i) & 1) ? "X" : "_");
        }
        DEBUGln("");
    }
}
