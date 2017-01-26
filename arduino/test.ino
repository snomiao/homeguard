
// dataPacking
    uint8_t hexdigit( const char hex ){
        return (hex <= '9') ? hex - '0' : 
                            toupper(hex) - 'A' + 10 ;
    }

    uint8_t hexbyte( const char* hex )
    {
        return (hexdigit(hex[0]) << 4) | hexdigit(hex[1]) ;
    }

    uint64_t hexint64( const char* hex )
    {
        uint64_t n = 0;
        for(int b = 0; b < 8; b++ )
        {
            ((uint8_t*)&n)[8-1-b] = hexbyte(hex + (b<<1));
        }
        return n;
    }

    char digithex(uint8_t digit){
        return digit <= 9 ? '0' + digit :
                            'A' + (digit - 10);
    }

    char* bytehex(uint8_t int8, char* hex){
        hex[1] = digithex(int8 & 0xF);
        hex[0] = digithex(int8  >> 4);
        return hex;
    }
    char* uint64hex(uint64_t *pn, char* hex)
    {
        // 小端模式
        for(int b = 0; b < 8; b++ )
        {
            bytehex(((uint8_t*)pn)[8-1-b], hex + (b<<1));
        }
        return hex;
    }
//

void setup(){
    char numbuf[17] = {0};
    uint64_t num = 0;
    
    num = hexint64("1234567887654321");
    Serial.begin(9600);
    //Serial.println(num); //305441741
    
    


    Serial.println(uint64hex(&num, numbuf)); //112210F4B2D230A2
    //Serial.println(((uint32_t)0x4996<<16) | 0x02D2); //305441741
    //Serial.println(hexint64("1234ABCD")); //305441741
    //Serial.println(int64hex(1234567891011121314, numbuf)); //499602D2
}
void loop(){

}