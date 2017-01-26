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
        Serial.begin(115200);
    }

// EEPROM: //
    /*
    struct SyncData{
      uint64_t data;
      uint64_t time;
    };
    int relays_btn_state = 0;
    int relaysAddress = 0;
    SyncData relays;
    int relaysAddress = 0;
    void inline EEPROM_load(){
        EEPROM.get(relaysAddress, relays);
    }
    void inline EEPROM_save(){
        EEPROM.put(relaysAddress, relays);
    }
*/

// ESP8266: //


    //sync
    //    function: dataConvert
    //    GET
    //        data from other module
    //    RESPOND
    //        data to other module


// CLIP: C74HC595 //
    //
    /* Connection: 74HC595 -> Arduino
        ST_CP(12)   ->  D7
        SH_CP(11)   ->  D8
        DS(14)      ->  D12
        OE(13)      ->  GND
        MR(10)      ->  5V
    */
 
    #define ST_CP              ( 7)
    #define SH_CP              ( 8)
    #define DS                 (12)
    int C74HC595_data = 0;
    int C74HC595_dirt = 0;
    void C74HC595_Setup()
    {
        pinMode(ST_CP, OUTPUT);
        pinMode(SH_CP, OUTPUT);
        pinMode(DS   , OUTPUT);    }
    void C74HC595_ShiftOut()
    {
        if(!C74HC595_dirt)
            return;
        digitalWrite(ST_CP, LOW);
        shiftOut(DS, SH_CP, MSBFIRST, C74HC595_data);
        digitalWrite(ST_CP, HIGH);
        C74HC595_dirt = 0;
    }


// MODULE: DELAY4 //
    /*
        Connection: DELAY4 -> 74HC595
            IN1 -> Q0(15)
            IN2 -> Q1(1)
            IN3 -> Q2(2)
            IN4 -> Q3(3)
        Connection: DELAY4 -> Arduino
            JD-VCC -> 5V
            VCC    -> D4
    */
    #define DELAY4_VCC   (4) //pin

    #define DELAY4_delay (80) //ms
    int DELAY4_LastUpdate = 0;
    int DELAY4_data = 0; // bit of IN1, IN2, IN3, IN4
    int DELAY4_dirt = 0;
    void DELAY4_Setup(){

        DELAY4_data = ~0 & 0xF;
        DELAY4_dirt = 1;

        while(DELAY4_dirt){
            DELAY4_Update();
        }
        C74HC595_ShiftOut();
        pinMode(DELAY4_VCC, OUTPUT);
        digitalWrite(DELAY4_VCC, HIGH);
    }
    void DELAY4_Update()
    {
        if(!DELAY4_dirt) return;

        // scan update DELAY4
        // (to prevent switch two delay at a time.)

        // delay scan
        int t = millis();
        if( t - DELAY4_LastUpdate < DELAY4_delay)
            return;
        DELAY4_LastUpdate = t;

        // assert
        DELAY4_data &= 0xF;

        // set a bit
        int next_C74HC595_data = C74HC595_data;
        // search pin for cut
        for(int step=0; step<4; step++)
        {
            next_C74HC595_data |= (DELAY4_data &  (1 << step)); // cut!
            if(next_C74HC595_data != C74HC595_data)
                goto needUpdate;
        }
        // search pin for connect
        for(int step=0; step<4; step++)
        {
            next_C74HC595_data &= (DELAY4_data | ~(1 << step)); // connect!
            if(next_C74HC595_data != C74HC595_data)
                goto needUpdate;
        }

        DELAY4_dirt = 0;
        // update
        needUpdate:
        C74HC595_data = next_C74HC595_data;
        C74HC595_dirt = 1;
    }


// MAIN
    void setup()
    {
        pinMode(13, OUTPUT);


        DEBUG_Setup();    // before ALL
        delay(500);



        C74HC595_Setup(); // before DELAY4

        delay(500);

        digitalWrite(13, HIGH);

        DELAY4_Setup();   // after 74HC595
        delay(500);
        digitalWrite(13, LOW);
    }

    int i = 0;
    int lastPlusI = 0;
    void loop()
    {

        DELAY4_data = ~i & 0xF;
        DELAY4_dirt = 1;

        do{
            int t = millis();
            if( t - lastPlusI < 5000)
                break;

            //DEBUG("DELAY4_data: ");
            //DEBUGln(DELAY4_data);

            i++;
            i %= 16;
            lastPlusI = t;
        }while(0);


        DELAY4_Update();
        C74HC595_ShiftOut();
    }