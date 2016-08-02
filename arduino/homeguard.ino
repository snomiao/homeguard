//#include <stdlib.h>
#include "ESP8266.h"
#include <EEPROM.h>
#define SPOTNAME "054"
#define PASSWORD "88647410"

#define ST_CP 7
#define SH_CP 8
#define DS 12

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

// SYNC DATA BEGIN
uint8_t states;

// SYNC DATA END

uint8_t sync_to_server(){
    uint8_t mux_id = 0;
    DEBUG("Create TCP:");
    if(wifi.createTCP(mux_id, HOST_1, 80) ){
        DEBUG("OK\n");
    }else{
        DEBUG("FAIL\n");
    }

    states=1;

    char buffer[128] = "";
    strcat(buffer, "GET ");
    strcat(buffer, PATH_1);
    strcat(buffer, "?action=sync");
    strcat(buffer, "&version=2016-08-01");
    strcat(buffer, "&data_states=");
    char numbuf[12];
    itoa(states, numbuf, 10)
    strcat(buffer, numbuf);
    strcat(buffer, "&data_states=");
    
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

    pinMode(ST_CP, OUTPUT); // 74HC595
    pinMode(SH_CP, OUTPUT); // 74HC595
    pinMode(DS, OUTPUT); // 74HC595
    pinMode(13, OUTPUT); // LED指示灯
    pinMode(11, INPUT);  // 重置按钮
    pinMode(10, INPUT);  // 传感器
    pinMode(A0, OUTPUT); // A0 = 继电器开关
    pinMode(A7, OUTPUT); // RST, 重启

    // 继电器状态

    states = EEPROM.read(0);
    digitalWrite(ST_CP, LOW);
    shiftOut(DS, SH_CP, MSBFIRST, ~states);
    digitalWrite(ST_CP, HIGH);

    digitalWrite(A0, HIGH);


    Serial.begin(9600);

    digitalWrite(A7, HIGH);

    if(!WiFi_Setup()){

        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
        delay(100);

        digitalWrite(A7, LOW);
    }
}


void loop(void)
{
    sync_to_server();


    // if(digitalRead(11)){
    //     EEPROM.write(0, 0);

    //     // 继电器状态

    //     states = EEPROM.read(0);
    //     digitalWrite(ST_CP, LOW);
    //     shiftOut(DS, SH_CP, MSBFIRST, ~states);
    //     digitalWrite(ST_CP, HIGH);

    //     //digitalWrite(A0, HIGH);
    // }

    digitalWrite(13, HIGH);
    delay(20);
    digitalWrite(13, LOW);
    /*
    if( WiFi_Recive() ){
        DEBUG("states=");
        DEBUGln(states);
    }*/
}
