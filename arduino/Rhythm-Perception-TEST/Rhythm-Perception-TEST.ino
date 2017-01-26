typedef long int int32_t;
typedef long long int int64_t;


// DEBUG: //
void inline DEBUG_Setup(){
    Serial.begin(115200);
}
#define DEBUG   (Serial.print  )
#define DEBUGln (Serial.println)


// RHYTHM: //
typedef unsigned long long int RhythmMark;
//#define Rhythm_TSIZE (sizeof(RhythmMark) * 8)
#define Rhythm_TSIZE (5)
class Rhythm{
    private:
    enum RhythmStatus { Calm, Up, Down };
    
    //
    int lastData = 0;
    
    //long int t_up[Rhythm_TSIZE] = {0}; 
    long int t_signal[Rhythm_TSIZE] = {0}; 
    long int t_lastSignal = 0;
    int signalStep = 0;
    int signalCount = 0;
    
    // reverse a list
    void reverse(long int *t, int a, int b){
        for(int s = a, e = b - 1; s < e; s++, e--){
            long int tmp = t[s];
            t[s] = t[e];
            t[e] = tmp;
        }
    }
    
    // shift array to leftmost
    void shiftToLeftMost(long int *t, int count, int index){
        reverse(t, 0, index);
        reverse(t, index, count);
        reverse(t, 0, count);
    }

    void calcRhythm(){
        long int*t = t_signal;
        int count = signalCount;
        for(int i=0; i<count; i++){ DEBUG(t[i]); DEBUG("; "); }; DEBUGln("");

        const int maxLenRhythm = sizeof(RhythmMark) * 8;
        RhythmMark rhythmMark = 1; //repersent newest signal

        int i = signalStep;
        while(1){
            // for loop
            i--;
            if(i < 0){
                i = signalCount - 1;
            }
            if(i == signalStep){
                break;
            }

            // calc rhythm length, and mark down
            if(t[i] < interval){
                rhythmMark = rhythmMark << 1 | 1;
            }
            else if(t[i] < 2 * interval){
                rhythmMark = rhythmMark << 2 | 1;
            }else{
                break;
            }
        }

        // show rhythm Mark
        for(int i=0; i < maxLenRhythm; i++){
            DEBUG( (int)((rhythmMark >> i) & 1) ? "X" : "_");
        }
        DEBUGln("");
    }


    void Signal(){
        // DEBUG("////////////// Signal: ");
        // DEBUGln(" ");

        
        long int now = millis();
        long int diff = now - t_lastSignal;

        t_lastSignal = now;

        // filter
        if( diff < interval / 2){
            return;
        }

        t_signal[signalStep] = diff;

        if(signalCount < Rhythm_TSIZE)
        signalCount++;
        signalStep++;
        signalStep %= Rhythm_TSIZE;

        calcRhythm();
    }

    //int32_t speedtime
    RhythmStatus lastStatus;
    long int t_firstStatus = 0;
    void Event(RhythmStatus s){
        // prevent repeat
        if(s == lastStatus){
            lastStatus = s;
            return;
        }
        //
        switch(s){
            case RhythmStatus::Calm:
            DEBUGln("/////////////////////////////////////////// Calm");
            break;
            case RhythmStatus::Up:
            DEBUGln("////////////////////////// Up");
            break;
            case RhythmStatus::Down:
            DEBUGln("////////////////////////// Down: ");
            Signal();

            break;
        }
        lastStatus = s;
    }

    public:
    int interval; // 250 (1/4s)
    int dataTolerance; // 100
    int tCalm; //100

    Rhythm(int pInterval, int pCalmTime, int pDataTolerance){
        dataTolerance = pDataTolerance; //100
        tCalm         = pCalmTime;
        interval      = pInterval;
    }

    long int tLastPutData = 0;

    void putData(int data){
        long int tdiff = millis() - tLastPutData;

        int diff = lastData - data;
        lastData = data;

        // DEBUG("DIFF: ");
        // DEBUGln(diff);
        if(diff > dataTolerance){
            //DEBUGln("RhythmStatus::Up");
            this->Event(RhythmStatus::Up);
        }
        else if(diff < -dataTolerance){
            //DEBUGln("RhythmStatus::Down");
            this->Event(RhythmStatus::Down);
        }
        else if(tdiff >= tCalm){
            //DEBUGln("RhythmStatus::Calm");
            this->Event(RhythmStatus::Calm);
        }
        else{
            return;
        }
        tLastPutData += tdiff;
    }
    void getAction(){
        // TODO
    }
};



// MAIN: //
Rhythm rhythm(250, 100, 60);

#define MEANSIZE 3
int data[MEANSIZE] = {0};
int k = 0;

void setup(){
    DEBUG_Setup();
    pinMode(A0, INPUT);
    
    //last_datamean = analogRead(A0);
}

void loop(){
    // PUT MEAN DATA
    int datanow = analogRead(A0);

    data[k++] = datanow;
    k %= MEANSIZE;

    long int datamean = 0;
    for(int i=0; i<MEANSIZE; i++){
        datamean += data[i];
    }
    datamean /= MEANSIZE;

    rhythm.putData(datamean);
    //
    
}
