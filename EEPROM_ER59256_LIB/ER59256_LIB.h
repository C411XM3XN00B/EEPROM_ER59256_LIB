#if !defined(ER59256_LIB_h)
#define ER59256_LIB_h

//required to include Arduino.h
#include <Arduino.h>


class ER59256 {
    private:
        uint8_t CS;
        uint8_t CLK;
        uint8_t DI;
        uint8_t DO;
        const uint8_t erase_time = 35;    //Chips needs a maximum of 30ms after erasing before continued operation
        void writeOut(bool bit);
        void writeOut(uint8_t bits);
        void writeRaw(uint8_t addr, uint16_t value);
        bool readIn();
        void clockCycle();
    public:
        ER59256(uint8_t CS, uint8_t CLK, uint8_t DI, uint8_t DO);
        bool write(uint8_t addr, uint16_t *values, uint8_t nBytes);
        bool write(uint8_t addr, uint16_t value);
        bool read(uint8_t addr, uint16_t *values, uint8_t nBytes);
        uint16_t read(uint8_t addr);
        bool erase(uint8_t addr, uint8_t nBytes);
        bool erase(uint8_t addr);
        bool erase_all();
        void erase_write_enable();
        void erase_write_disable();
};






#endif // ER59256_LIB_h