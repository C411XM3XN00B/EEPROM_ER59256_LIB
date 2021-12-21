#include <ER59256_LIB.h>

ER59256 EEPROM256(2,3,4,5);     //Create ER59256 Object with Pins: CS=2, CLK=3, DI=4, DO=5

uint8_t addr = 0;
uint16_t testVal = 0;

void setup(){
    Serial.begin(38400);


    Serial.println("\n");
    Serial.println("Start of EEPROM-DUMP before Writing:\n");
    for(addr=0; addr<15; addr+=4){
        Serial.print(addr);
        Serial.print("\t");
        for(uint8_t i = 0; i<4 ;++i){
            Serial.print("\t");
            Serial.print(EEPROM256.read(addr + i), HEX);    //print contents of chip at the address (addr + i) in HEX format
        }
        Serial.println();
    }
    Serial.println("\nEnd of EEPROM-DUMP before Writing.");
    Serial.println("\n");

    delay(500);

    EEPROM256.erase_write_enable();         //Enables erasing and writing modes in chip itself
    Serial.println("\n");
    Serial.println("Writing to EEPROM:\n");
    for(addr=0; addr<15; addr+=4){
        Serial.print(addr);
        Serial.print("\t");
        for(uint8_t i = 0; i<4 ;++i){
            Serial.print("\t");
            testVal = ((addr+i)*0x1111);
            if(EEPROM256.write(addr+i, testVal)){           //attempt to write the current test value at the address (addr + i) 
                Serial.print(EEPROM256.read(addr + i), HEX);    //if successfull, read back from the same address and print it
            }
        }
        Serial.println();
    }
    Serial.println("\nEnd of Writing to EEPROM.");
    Serial.println("\n");
    EEPROM256.erase_write_disable();        //Disables erasing and writing modes in chip itself

    delay(500);

    Serial.println("\n");
    Serial.println("Start of EEPROM-DUMP after Writing:\n");
    for(addr=0; addr<15; addr+=4){
        Serial.print(addr);
        Serial.print("\t");
        for(uint8_t i = 0; i<4 ;++i){
            Serial.print("\t");
            Serial.print(EEPROM256.read(addr + i), HEX);
        }
        Serial.println();
    }
    Serial.println("\nEnd of EEPROM-DUMP after Writing.");
    Serial.println("\n");
}
void loop(){
    delay(5000);
}