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

class emulator{
private:
    uint8_t *memory;
    uint32_t eip;
    uint32_t registers[REGISTERS_COUNT];
    void (emulator::*instructions[INSTRUCTION_NUM])();
    
    void _init_instructions();
    
    uint8_t _get_code8(uint32_t index);
    int8_t _get_sign_code8(uint32_t index);
    uint32_t _get_code32(uint32_t index);

public:
    emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp);
    ~emulator();
    
    void dump_registers();
    
    void load_program(char *filename, uint32_t size);
    bool exec();

private:
    //instructions
    void _mov_r32_imm32();
    void _short_jump();
};

#endif
