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
#define RhythmMarkMaxLen (sizeof(RhythmMark)*8)
class Rhythm{
    private:


    enum RhythmStatus { Calm, Up, Down };
    int lastData = 0;

    //
    
    
    //long int t_up[Rhythm_TSIZE] = {0}; 
    // long int t_signal[Rhythm_TSIZE] = {0}; 
    // long int t_lastSignal = 0;
    // int signalStep = 0;
    // int signalCount = 0;
    RhythmMark rhym = 1;




    void matchAction(){
        // TODO
        //rhym == 
        //rhym &
        switch(rhym){

            case 7645: // XXX_XXX_XXX_X
            DEBUGln("ACTION! ONE!");
            rhym = 0;
            break;
            
            case 173: // X_X_XX_X
            DEBUGln("ACTION! TWO!");
            rhym = 0;
            break;

            default:
            //none

            break;
        }
    }

    int rhymAt = 0;
    long int tLastDown = 0;
    long int tLastUp   = 0;
    void up(){
        long int t = millis();
        int diff = t - tLastDown;
        tLastDown = t;

        int rhymShift = diff / interval + 1;
        //if(rhymShift < RhythmMarkMaxLen){
            if(rhymShift <= 4){
                if(rhymAt == 2){
                    rhym = rhym << rhymShift | 1;
            } //dn
        }
        else{
            rhym = 1;
            rhymAt = 2; //dn
        }
    }
    void dn(){

        long int t = millis();
        int diff = t - tLastDown;
        tLastDown = t;

        int rhymShift = diff / interval + 1;
        // if(rhymShift < RhythmMarkMaxLen)
        if(rhymShift <= 4){
            if(rhymAt == 1){
                rhym = rhym << rhymShift | 1;
            } //dn
        }
        else{
            rhym = 1;
            rhymAt = 1; //dn
        }
    }
    void clam(){
        // show rhythm Mark
        for(int i = sizeof(rhym) * 8 - 1; i >= 0; i--){
            // 从大端读起
            DEBUG( (int)((rhym >> i) & 1) ? "X" : "_");
        }
        DEBUG("//");
        DEBUG( (unsigned long int)( (rhym >> 32) & 0xFFFFFFFF) );
        DEBUG(",");
        DEBUG( (unsigned long int)( (rhym      ) & 0xFFFFFFFF) );
        DEBUG("// rhymAt:");
        DEBUG( rhymAt);
        DEBUGln();

        matchAction();
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
            clam();
            break;
            case RhythmStatus::Up:
            DEBUGln("////////////////////////// Up");
            up();
            break;
            case RhythmStatus::Down:
            DEBUGln("////////////////////////// Down: ");
            dn();
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


        if(tdiff < interval / 8 ){
            return;
        }

        //DEBUG("DATA: "); DEBUGln(data);
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
            return;
        }
        else{
            return;
        }
        tLastPutData += tdiff;
    }
};



// MAIN: //
Rhythm rhythm(250, 100, 80);

#define MEANSIZE 10
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

    // data[k++] = datanow;
    // k %= MEANSIZE;

    // long int datamean = 0;
    // for(int i=0; i<MEANSIZE; i++){
    //     datamean += data[i];
    // }
    // datamean /= MEANSIZE;

    rhythm.putData(datanow);
    //
    
}
