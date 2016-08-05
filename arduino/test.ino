
// dataPacking
    uint8_t hexdigit( const char hex ){
        return (hex <= '9') ? hex - '0' : 
                            toupper(hex) - 'A' + 10 ;
    }

    uint8_t hexbyte( const char* hex ){
        return (hexdigit(hex[0]) << 4) | hexdigit(hex[1]) ;
    }

    uint64_t hexint64( const char* hex ){
        uint64_t result = 0;
        ((uint8_t*)&result)[0] = hexbyte(hex + 14);
        ((uint8_t*)&result)[1] = hexbyte(hex + 14);
        ((uint8_t*)&result)[2] = hexbyte(hex + 14);
        ((uint8_t*)&result)[3] = hexbyte(hex + 14);
        ((uint8_t*)&result)[4] = hexbyte(hex + 14);
        ((uint8_t*)&result)[5] = hexbyte(hex + 14);
        ((uint8_t*)&result)[6] = hexbyte(hex + 14);
        ((uint8_t*)&result)[7] = hexbyte(hex + 14);
        return result;
    }

    char digithex(uint8_t digit){
        return digit <= 9 ? '0' + digit :
                            'A' + (digit - 10);
    }

    char* bytehex(uint8_t int8, char* buf){
        buf[1] = digithex(int8 & 0xF);
        buf[0] = digithex(int8  >> 4);
        return buf;
    }
    char* uint64hex(uint64_t *n, char* buf){
        // 小端模式
        for(int b = 0; b < 8; b++ ){
            bytehex(((uint8_t*)&n)[8-1-b], buf + (b<<1));
        }
        return buf;
    }
//

void setup(){
    char numbuf[17] = {0};
    uint64_t num = 0x1234567887654321;
    Serial.begin(9600);
    //Serial.println(num); //305441741
    
    Serial.println(uint64hex(&num, numbuf)); //112210F4B2D230A2
    //Serial.println(((uint32_t)0x4996<<16) | 0x02D2); //305441741
    //Serial.println(hexint64("1234ABCD")); //305441741
    //Serial.println(int64hex(1234567891011121314, numbuf)); //499602D2
}
void loop(){

}