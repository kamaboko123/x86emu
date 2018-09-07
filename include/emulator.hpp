#ifndef __INCLUDE_EMULATOR__
#define __INCLUDE_EMULATOR__

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

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

typedef struct{
    uint8_t mod;
    union {
        uint8_t opecode;
        uint8_t reg_index;
    };
    uint8_t rm;
    
    uint8_t sib;
    union{
        int8_t disp8;
        uint32_t disp32;
    };
} ModRM;

class emulator{
friend class EmulatorTest;
private:
    uint8_t *memory;
    uint32_t eip;
    uint32_t registers[REGISTERS_COUNT];
    void (emulator::*instructions[INSTRUCTION_NUM])();
    
    void _init_instructions();
    
    uint8_t _get_code8(uint32_t index);
    int8_t _get_sign_code8(uint32_t index);
    uint32_t _get_code32(uint32_t index);
    int32_t _get_sign_code32(uint32_t index);
    
    void _parse_modrm(ModRM &modrm);

public:
    emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp);
    ~emulator();
    
    void dump_registers();
    
    void load_program(const char *filename, uint32_t size, uint32_t index = 0);
    bool exec();

private:
    //instructions
    void _mov_r32_imm32();
    void _short_jump();
    void _near_jump();
};

#endif
