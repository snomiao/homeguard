//#include <stdlib.h>

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

// Default Connection

/* Connection: 74HC595 -> Arduino
    ST_CP->D7
    SH_CP->D8
    DS->D12
    OE->GND
    MR->VCC
    GND->GND
*/
#define ST_CP              ( 7)
#define SH_CP              ( 8)
#define DS                 (12)


#define RELAYS             (A0)
#define BTN_RESET_RELAYS   (11)


/* Connection: SENSOR_TEMPERATURE -> Arduino
    
*/
#define SENSOR_TEMPERATURE (A1)
#define SENSOR_INFRARED    (10)
#define LED                (13)



#include "ESP8266.h"
#include <EEPROM.h>


#define DEBUG   (Serial.print  )  
#define DEBUGln (Serial.println)

#define HOST_1 ("192.168.1.254"             )
#define PATH_1 ("/"                         )
#define HOST_2 ("www.snowstarcyan.com"      )
#define PATH_2 ("/homeguard"                )
#define HOST_3 ("homeguard.snowstarcyan.com")
#define PATH_3 ("/"                         )

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
    pinMode(DS   , OUTPUT); // 74HC595
}
// SYNC DATA BEGIN

struct SyncData{
  uint64_t data;
  uint64_t time;
};
int relays_btn_state = 0;
int relaysAddress = 0;
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
    
    pinMode(BTN_RESET_RELAYS, INPUT);  // 重置按钮
    relays_btn_state = digitalRead(BTN_RESET_RELAYS);

    pinMode(RELAYS, OUTPUT); // RELAYS = 继电器开关
    relays_states_load();
    relays_states_update();
    digitalWrite(RELAYS, HIGH);
}

void inline check_relays_btn(){
    int new_state = digitalRead(BTN_RESET_RELAYS);
    // 下降沿检测（放开按钮）
    if(!new_state && relays_btn_state){
        if(relays.data == (uint64_t)0){
            relays.data = ~relays.data; 
        }else{
            relays.data = (uint64_t)0;
        }
    }
    relays_btn_state = new_state;
}
// SYNC DATA END

// DataPacking
    uint8_t hexdigit( const char hex ){
        return (hex <= '9') ? hex - '0' : 
                            toupper(hex) - 'A' + 10 ;
    }

    uint8_t hexubyte( const char* hex )
    {
        return (hexdigit(hex[0]) << 4) | hexdigit(hex[1]) ;
    }

    uint64_t hexuint64( const char* hex )
    {
        uint64_t n = 0;
        for(int b = 0; b < 8; b++ )
        {
            ((uint8_t*)&n)[8-1-b] = hexubyte(hex + (b<<1));
        }
        return n;
    }

    char digithex(const uint8_t digit){
        return digit <= 9 ? '0' + digit :
                            'A' + (digit - 10);
    }

    char* ubytehex(const uint8_t int8, char* hex){
        hex[1] = digithex(int8 & 0xF);
        hex[0] = digithex(int8  >> 4);
        return hex;
    }
    char* uint64hex(const uint64_t pn, char* hex)
    {
        // 小端模式
        for(int b = 0; b < 8; b++ )
        {
            ubytehex(((uint8_t*)&pn)[8-1-b], hex + (b<<1));
        }
        return hex;
    }

// sync_to_server
    uint8_t sync_to_server(){
        uint8_t mux_id = 0;
        DEBUG("Create TCP:");
        if(wifi.createTCP(mux_id, HOST_1, 80) ){
            DEBUG("OK\n");
        }else{
            DEBUG("FAIL\n");
        }

        char buffer[128] = {0};
        char numbuf[17] = {0};

        strcat(buffer, "GET "                                                      );
        strcat(buffer, PATH_1                                                      );
        strcat(buffer, "?action=sync"                                              );
        strcat(buffer, "&version=2016-08-01"                                       );
        strcat(buffer, "&data_relays_time="                                        );
        strcat(buffer, uint64hex(relays.time, numbuf)                              );
        strcat(buffer, "&data_relays_data="                                        );
        strcat(buffer, uint64hex(relays.data, numbuf)                              );
        strcat(buffer, "&data_temperature="                                        );
        strcat(buffer, uint64hex((uint64_t)digitalRead(SENSOR_TEMPERATURE), numbuf));
        strcat(buffer, "&data_infrared="                                           );
        strcat(buffer, uint64hex((uint64_t)digitalRead(SENSOR_INFRARED   ), numbuf));
        strcat(buffer, " HTTP/1.1\n");
        strcat(buffer, "User-Agent: homeguard\n");
        strcat(buffer, "Accept: text/html\n");
        strcat(buffer, "\n");

        int len = strlen(buffer);
        DEBUG("HTTP GET[");
        DEBUG(len);
        DEBUG("]: [[\n");
        for(uint32_t i = 0; i < len; i++) {
            DEBUG((char)buffer[i]);
        }
        DEBUG("\n]]\n");
        wifi.send(mux_id, (const uint8_t*)buffer, len);


        DEBUG("Recving:\n");
        len = wifi.recv(mux_id, (uint8_t*)buffer, sizeof(buffer), 100);

        DEBUG("RECIVED[");
        DEBUG(len);
        DEBUG("]: [[\n");
        for(int i = 0; i < len; i++) {
            DEBUG((char)buffer[i]);
        }
        DEBUG("\n]]\n");

        
        char *buffer_data;
        buffer_data = strstr(buffer, "\n\n");
        if(buffer_data){
            buffer_data+=2;
            DEBUG(buffer_data);
            relays.time = hexuint64(&buffer_data[ 0]);
            relays.data = hexuint64(&buffer_data[16]);
        }else{
            DEBUG("==== WARNNING: RECIVED NOTHING ====");
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

    DEBUGOK("Join AP ----------------------- ", wifi.joinAP(WLANNAME, WLANPASS));
    DEBUG(  "IP: "); DEBUGln(wifi.getLocalIP().c_str());
    DEBUGOK("Enable multiple --------------- ", wifi.enableMUX());
    DEBUGOK("Start TCP Server (8090) ------- ", wifi.startTCPServer(8090));
    DEBUGOK("Set TCP Timeout (10) seconds -- ", wifi.setTCPServerTimeout(10));

    DEBUG(  "Setup end ===================== ====\n");
    return 1;
}
// setup
void setup(void)
{
    C74HC595_setup();
    relays_states_setup();

    pinMode(LED, OUTPUT);               // LED指示灯

    pinMode(SENSOR_INFRARED, INPUT);    // 传感器
    pinMode(SENSOR_TEMPERATURE, INPUT); // A1 温度传感数据

    Serial.begin(9600);


    if(!WiFi_Setup()){

        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }
}

// loop
void loop(void)
{
    sync_to_server();
    check_relays_btn();

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
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    /*
    if( WiFi_Recive() ){
        DEBUG("relays_states=");
        DEBUGln(relays_states);
    }*/
}