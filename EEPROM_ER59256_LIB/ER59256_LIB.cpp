//required to include Arduino.h
#include <Arduino.h>
//include our header file
#include <ER59256_LIB.h>


ER59256::ER59256(uint8_t CS, uint8_t CLK, uint8_t DI, uint8_t DO): CS(CS), CLK(CLK), DI(DI),DO(DO){
    pinMode(CS, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(DI, OUTPUT);
    pinMode(DO, INPUT);
    digitalWrite(CS, LOW);
    digitalWrite(CLK, LOW);
}



void ER59256::clockCycle(){
    digitalWrite(CLK, 1);
    digitalWrite(CLK, 0);
}


void ER59256::writeOut(bool bit){
    digitalWrite(DI, bit);
    clockCycle();
}


void ER59256::writeOut(uint8_t bits){
    for(int i=0; i<8; ++i){
        uint8_t temp = bits << i;
        temp = temp >> 7;
        if(temp == 1){
            writeOut(true);
        } else {
            writeOut(false);
        }
    }
}


void ER59256::writeRaw(uint8_t addr, uint16_t value){
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opAddr = 0b0100 << 4;
    opAddr += (addr & 0b00001111);
    writeOut(opAddr);
    uint8_t halfVal = (uint8_t)(value >> 8);
    writeOut(halfVal);
    writeOut((uint8_t)value);
    digitalWrite(DI, 0);
    digitalWrite(CS, 0);
    delay(erase_time);
    digitalWrite(CS, 1);
    clockCycle();
    digitalWrite(CS, 0);
}


bool ER59256::readIn(){
    clockCycle();
    return digitalRead(DO);
}



bool ER59256::write(uint8_t addr, uint16_t *values, uint8_t nBytes){
    for(uint8_t i=0; i<nBytes; ++i){
        if(!write(addr+i, values[i])){
            return 0;
        }
    }
    return 1;
}


bool ER59256::write(uint8_t addr, uint16_t value){
    if(read(addr)==value){
        return 1;
    }
    if(!erase(addr)){
        return 0;
    }
    writeRaw(addr, value);
    if(read(addr)==value){
        return 1;
    } else {
        return 0;
    }
}

bool ER59256::read(uint8_t addr, uint16_t *values, uint8_t nBytes){
    for(uint8_t i=0; i<nBytes; ++i){
        values[i] = read(addr+i);
    }
    return 1;
}


uint16_t ER59256::read(uint8_t addr){
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opAddr = 0b1000 << 4;
    opAddr += (addr & 0b00001111);
    writeOut(opAddr);
    digitalWrite(DI, 0);
    uint16_t tempbytes = 0b0000000000000000;
    for(uint8_t i=16; i>0; --i){
        bool tempbit = readIn();
        tempbytes += (uint16_t)(tempbit << (i-1));
    }
    digitalWrite(CS, 0);
    return tempbytes;
}


bool ER59256::erase(uint8_t addr, uint8_t nBytes){
    for(uint8_t i=0; i<nBytes; ++i){
        if(!erase(addr+i)){
            return 0;
        }
    }
    return 1;
}


bool ER59256::erase(uint8_t addr){
    if(read(addr)==0xFFFF){
        return 1;
    }
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opAddr = 0b1100 << 4;
    opAddr += (addr & 0b00001111);
    writeOut(opAddr);
    digitalWrite(DI, 0);
    digitalWrite(CS, 0);
    delay(erase_time);
    digitalWrite(CS, 1);
    clockCycle();
    digitalWrite(CS, 0);
    if(read(addr)==0xFFFF){
        return 1;
    } else {
        return 0;
    }
}


bool ER59256::erase_all(){
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opcode = 0b0010 << 4;
    writeOut(opcode);
    digitalWrite(CS, 0);
    delay(erase_time);
    digitalWrite(CS, 1);
    clockCycle();
    digitalWrite(CS, 0);
    for(uint8_t i=0; i<16; ++i){
        if(read(i)!=0xFFFF){
            return 0;
        }
    }
    return 1;
}


void ER59256::erase_write_enable(){
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opcode = 0b00110000;
    writeOut(opcode);
    digitalWrite(CS, 0);
}


void ER59256::erase_write_disable(){
    digitalWrite(CS, 1);
    clockCycle();
    writeOut(true);
    uint8_t opcode = 0;
    writeOut(opcode);
    digitalWrite(CS, 0);
}