#include <ER59256_LIB.h>

ER59256 EEPROM256(2,3,4,5);
uint8_t addr = 0;

void setup(){
    Serial.begin(38400);
}
void loop(){
    Serial.println("\n");
    Serial.println("Start of EEPROM-DUMP:\n");
    for(addr=0; addr<15; addr+=4){
        Serial.print(addr);
        Serial.print("\t");
        for(uint8_t i = 0; i<4 ;++i){
            Serial.print("\t");
            Serial.print(EEPROM256.read(addr + i), HEX);
        }
        Serial.println();
    }
    Serial.println("\nEnd of EEPROM-DUMP.");
    Serial.println("\n");
    delay(5000);
}