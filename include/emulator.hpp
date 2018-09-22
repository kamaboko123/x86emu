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
    void _set_rm32(ModRM &modrm, uint32_t value);
    
    void _set_register32(Register reg, uint32_t value);
    void _set_memory8(uint32_t address, uint8_t value);
    void _set_memory32(uint32_t address, uint32_t value);
    uint8_t _get_memory8(uint32_t address);
    uint32_t _get_memory32(uint32_t address);
    
    uint32_t _get_register32(Register reg);
    
    uint32_t _calc_memory_address(ModRM &modrm);
    
    uint32_t _get_r32(ModRM &modrm);
    void _set_r32(ModRM &modrm, uint32_t value);
    uint32_t _get_rm32(ModRM &modrm);
    
    void _push32(uint32_t value);
    uint32_t _pop32();
    
public:
    emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp);
    ~emulator();
    
    void dump_registers();
    
    void load_program(const char *filename, uint32_t size);
    bool exec();

private:
    //instructions
    void _mov_r32_imm32();
    void _short_jump();
    void _near_jump();
    
    //mov with ModRM
    void _mov_rm32_imm32();
    void _mov_rm32_r32();
    void _mov_r32_rm32();
    
    void _add_rm32_r32();
    
    void _code_83();
    void _sub_rm32_imm8(ModRM &modrm);
    
    void _code_ff();
    void _inc_rm32(ModRM &modrm);
    
    void _push_r32();
    void _pop_r32();
    
    void _call_rel32();
    void _ret();
    
    void _leave();
};

#endif
