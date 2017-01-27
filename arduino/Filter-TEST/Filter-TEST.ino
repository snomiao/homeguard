typedef long int int32_t;
typedef long long int int64_t;


// DEBUG: //
void inline DEBUG_Setup(){
    Serial.begin(115200);
}

#define DEBUG   (Serial.print  )
#define DEBUGln (Serial.println)



// Filter 消除抖动
class FilterShake{
    private:
    int i = 0;
    unsigned int value; // TODO init
    public:
    unsigned int N;
    FilterShake(int n){
        N = n;
    }
    void inline init(unsigned int init_value){
        value = init_value;
    }
    unsigned int filter(unsigned int new_value){
        if(value != new_value){
            i++;
            if(i>N){
                i=0;
                value = new_value;
            }
        }
        else{
            i=0;
        }
        return value;
    }
};

// Filter 加权递推
class FilterShiftMean{
    private:
    #define FILTER_N 12
    int coe[FILTER_N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int sum_coe = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12;
    int filter_buf[FILTER_N + 1];

    public:

    int filter(int new_value){
        int i;
        int filter_sum = 0;
        filter_buf[FILTER_N] = new_value;
        for(i = 0; i < FILTER_N; i++) {
            filter_buf[i] = filter_buf[i + 1];
            filter_sum += filter_buf[i] * coe[i];
        }
        filter_sum /= sum_coe;
        return filter_sum;
    }

    FilterShiftMean(){

    }
};

class SignalSensor{
    private:
    unsigned int value; //not inited, so first signal is not valid
    int last_signal = 0;

    public:
    int signal(unsigned int new_value){
        int diff = new_value - value;
        value += diff;

        // ignore no diff
        if(diff == 0){
            return 0;
        }
        
        // calc signal
        int signal = diff > 0 ? 1 : -1;

        // ignore repeat signal
        if (last_signal == signal){
            return 0;
        }

        // output
        last_signal = signal;
        return signal;
    }
    SignalSensor(){

    }
};

// calss RhymSensor{
//     private:
//     unsigned int value:
// }
// MAIN

FilterShake fs = FilterShake(10);
SignalSensor ss = SignalSensor();
int lastData = 0;

void setup(){
    DEBUG_Setup();
    pinMode(A0, INPUT);
    
    fs.init(analogRead(A0));
}
void loop(){

    //command control
    switch (Serial.read()) {
        case '1':
        fs.N++;
        DEBUG("=========================");
        DEBUGln(fs.N);
        break;
        case '2':
        fs.N--;
        DEBUG("=========================");
        DEBUGln(fs.N);
        break;
        case '3':
        DEBUG("=========================");
        DEBUGln(lastData);
        break;
    }


    // PUT MEAN DATA
    int datanow = analogRead(A0);
    

    // data
    int dataFilted = fs.filter(datanow);
    if(lastData!=dataFilted){
        DEBUGln(dataFilted);
        lastData = dataFilted;
    }
    // signal
    int signal = ss.signal(dataFilted);
    if( signal != 0){
        DEBUGln( signal > 0 ? "///////////" : "\\\\\\\\\\");
    }
    
}
