#include "ESP8266.h"

SoftwareSerial SerialESP8266(3, 2); /* RX:D3, TX:D2 */
ESP8266 wifi(SerialESP8266);

//#define HOST_NAME   "www.snowstarcyan.com"
//#define HOST_PORT   (80)

#define SSID        "054"
#define PASSWORD    "88647410"

#define HOST_NAME   "192.168.1.254"
#define HOST_PORT   (80)

//undefined reference to `ESP8266::ESP8266(SoftwareSerial&, unsigned long)
void setup(void){
    Serial.begin(9600);
    Serial.print("setup begin\r\n");

    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());

    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    }
    else {
        Serial.print("to station + softap err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");

        Serial.print("IP:");
        Serial.println( wifi.getLocalIP().c_str());       
    } 
    else {
        Serial.print("Join AP failure\r\n");
    }

    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } 
    else {
        Serial.print("single err\r\n");
    }

    Serial.print("setup end\r\n");
}

void loop(void){

    uint8_t buffer[1024] = {0};
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
    } 
    else {
        Serial.print("create tcp err\r\n");
    }

    //uint8_t buffer[1024] = {0};
    char *hello = "GET / HTTP/1.1\r\nHost: 192.168.1.254\r\nConnection: close\r\n\r\n";
    wifi.send((const uint8_t*)hello, strlen(hello));

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    /*
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            //Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }*/
    Serial.print("Received[");
    Serial.print(len);
    Serial.print("]");
    Serial.println("");

    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } 
    else {
        Serial.print("release tcp err\r\n");
    }


    while(1);
}