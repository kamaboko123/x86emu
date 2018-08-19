#ifndef __INCLUDE_EMULATOR__
#define __INCLUDE_EMULATOR__

#include <cstdio>
#include <cstdint>

#define INSTRUCTION_NUM 256

enum Register{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTERS_COUNT
};

typedef void(*INSTRUCTION)();

class emulator{
private:
    uint8_t *memory;
    uint32_t eip;
    uint32_t registers[REGISTERS_COUNT];
    INSTRUCTION instructions[INSTRUCTION_NUM];

public:
    emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp);
    ~emulator();
    
    void _init();
    
    void load_program(char *filename, uint32_t size);
    bool exec();
};

#endif
