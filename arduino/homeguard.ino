//#include <stdlib.h>
#include "ESP8266.h"
#include <EEPROM.h>
#define SPOTNAME "054"
#define PASSWORD "88647410"

#define ST_CP 7
#define SH_CP 8
#define DS 12
#define RELAYS A0
#define BTN_RESET_RELAYS 11
#define SENSOR_TEMPERATURE A1
#define SENSOR_INFRARED 10
#define LED 13


#define DEBUG Serial.print
#define DEBUGln Serial.println

#define HOST_1 "192.168.1.254"
#define PATH_1 "/"
#define HOST_2 "www.snowstarcyan.com"
#define PATH_2 "/homeguard"
#define HOST_3 "homeguard.snowstarcyan.com"
#define PATH_3 "/"

SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

// 74HC595

void inline C74HC595_OUTPUT(uint8_t value){
    digitalWrite(ST_CP, LOW);
    shiftOut(DS, SH_CP, MSBFIRST, ~value);
    digitalWrite(ST_CP, HIGH);
}

void inline C74HC595_setup(){
    pinMode(ST_CP, OUTPUT); // 74HC595
    pinMode(SH_CP, OUTPUT); // 74HC595
    pinMode(DS, OUTPUT); // 74HC595
}
// SYNC DATA BEGIN

struct SyncData{
  uint32_t data;
  uint32_t time;
};
uint relaysAddress = 0
SyncData relays;
void inline relays_states_load(){
    EEPROM.get(relaysAddress, relays);
}
void inline relays_states_save(){
    EEPROM.put(relaysAddress, relays);
}
void inline relays_states_update(){
    C74HC595_OUTPUT(~relays.data);
}
void inline relays_states_setup(){
    pinMode(RELAYS, OUTPUT); // RELAYS = 继电器开关
    relays_states_load();
    relays_states_update();
    digitalWrite(RELAYS, HIGH);
}

// SYNC DATA END

inline char* int32_to_hex(int32_t n, char* buf8byte)
{
    sprintf(buf8byte, "%04X%04X", *((int16_t*)(&n) + 1), *((int16_t*)(&n) + 0) );
    return buf8byte;
}

inline int32_t hex_to_int32(char* buf8byte)
{
    int32_t n;
    sscanf(buf8byte, "%04X%04X", ((int16_t)&n) + 1, ((int16_t)&n) + 0);
    return n;
}

uint8_t sync_to_server(){
    uint8_t mux_id = 0;
    DEBUG("Create TCP:");
    if(wifi.createTCP(mux_id, HOST_1, 80) ){
        DEBUG("OK\n");
    }else{
        DEBUG("FAIL\n");
    }

    char buffer[128] = "";
    char numbuf[8];
    strcat(buffer, "GET ");
    strcat(buffer, PATH_1);
    strcat(buffer, "?action=sync");
    strcat(buffer, "&version=2016-08-01");
    strcat(buffer, "&data_relays_data=");
    strcat(buffer, itoa(relays.data, numbuf, 16));
    strcat(buffer, "&data_relays_data=");
    strcat(buffer, itoa(relays.time, numbuf, 16));
    strcat(buffer, "&data_temperature=");
    strcat(buffer, itoa(digitalRead(SENSOR_TEMPERATURE), numbuf, 16));
    strcat(buffer, "&data_infrared=");
    strcat(buffer, itoa(digitalRead(SENSOR_INFRARED), numbuf, 16));
    
    strcat(buffer, " HTTP/1.1\n");

    strcat(buffer, "User-Agent: homeguard\n");
    strcat(buffer, "Accept: text/html\n");

    DEBUG("HTTP GET: [[\n");
    uint32_t len = strlen(buffer);
    for(uint32_t i = 0; i < len; i++) {
        DEBUG((char)buffer[i]);
    }
    DEBUG("\n]]\n");
    wifi.send(mux_id, (const uint8_t*)buffer, len);




    DEBUG("Recving:\n");
    len = wifi.recv(mux_id, (uint8_t*)buffer, sizeof(buffer), 100);

    DEBUG("RECIVED: [[\n");
    for(uint32_t i = 0; i < len; i++) {
        DEBUG((char)buffer[i]);
    }
    DEBUG("\n]]\n");
    
    //uint32_t data = atoi()

    char *p;
    p = strstr(buffer, "\n\n");
    if(p){
        p+=2;
        DEBUG(p);
    }


    DEBUG("release tcp");
    DEBUG(mux_id);
    if (wifi.releaseTCP(mux_id)) {
        DEBUGln(" ok");
    } else {
        DEBUGln(" err");
    }
    return 0;
}



// WIFI_SETUP : 
    #define DEBUGOK(string, operation) \
        DEBUG(string);                 \
        if (operation) {               \
            DEBUG("OK\n");             \
        } else {                       \
            DEBUG("FAIL\n");           \
            return 0;                  \
        }
    uint8_t WiFi_Setup(){
        DEBUG(  "Setup begin =================== ====\n");

        DEBUG("FW Version:");
        DEBUGln(wifi.getVersion().c_str());
        DEBUGOK("To station + softap ----------- ", wifi.setOprToStationSoftAP());

        DEBUGOK("Join AP ----------------------- ", wifi.joinAP(SPOTNAME, PASSWORD));
        DEBUG(  "IP: "); DEBUGln(wifi.getLocalIP().c_str());    
        DEBUGOK("Enable multiple --------------- ", wifi.enableMUX());
        DEBUGOK("Start TCP Server (8090) ------- ", wifi.startTCPServer(8090));
        DEBUGOK("Set TCP Timeout (10) seconds -- ", wifi.setTCPServerTimeout(10));

        DEBUG(  "Setup end ===================== ====\n");
        return 1;
    }

void setup(void)
{
    C74HC595_setup();
    relays_states_setup();

    pinMode(LED, OUTPUT); // LED指示灯
    pinMode(BTN_RESET_RELAYS, INPUT);  // 重置按钮
    pinMode(RELAYS, OUTPUT); // RELAYS = 继电器开关

    pinMode(SENSOR_INFRARED, INPUT);  // 传感器
    pinMode(SENSOR_TEMPERATURE, INPUT); // A1 温度传感数据

    Serial.begin(9600);


    if(!WiFi_Setup()){

        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);

        digitalWrite(A7, LOW);
    }
}


void loop(void)
{
    sync_to_server();


    // if(digitalRead(BTN_RESET_RELAYS)){
    //     EEPROM.write(0, 0);

    //     // 继电器状态

    //     relays_states = EEPROM.read(0);
    //     digitalWrite(ST_CP, LOW);
    //     shiftOut(DS, SH_CP, MSBFIRST, ~relays_states);
    //     digitalWrite(ST_CP, HIGH);

    //     //digitalWrite(RELAYS, HIGH);
    // }

    digitalWrite(LED, HIGH);
    delay(20);
    digitalWrite(LED, LOW);
    /*
    if( WiFi_Recive() ){
        DEBUG("relays_states=");
        DEBUGln(relays_states);
    }*/
}
