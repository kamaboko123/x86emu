#ifndef __INCLUDE_EMULATOR__
#define __INCLUDE_EMULATOR__

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

const int INSTRUCTION_NUM = 256;
const uint32_t CARRY_FLAG = 1;
const uint32_t ZERO_FLAG = (1 << 6);
const uint32_t SIGN_FLAG = (1 << 7);
const uint32_t OVERFLOW_FLAG = (1 << 11);

enum Register{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTERS_COUNT,
    
    AL = EAX,
    CL = ECX,
    DL = EDX,
    BL = EBX,
    AH = AL + 4,
    CH = CL + 4,
    DH = DL + 4,
    BH = BL + 4
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
    uint32_t eflags;
    uint32_t registers[REGISTERS_COUNT];
    void (emulator::*instructions[INSTRUCTION_NUM])();
    
    void _init_instructions();
    
    uint8_t _get_code8(uint32_t index);
    int8_t _get_sign_code8(uint32_t index);
    uint32_t _get_code32(uint32_t index);
    int32_t _get_sign_code32(uint32_t index);
    
    void _parse_modrm(ModRM &modrm);
    void _set_rm32(ModRM &modrm, uint32_t value);
    void _set_rm8(ModRM &modrm, uint8_t value);
    
    void _set_register32(Register reg, uint32_t value);
    void _set_register8(Register reg, uint8_t value);
    void _set_memory8(uint32_t address, uint8_t value);
    void _set_memory32(uint32_t address, uint32_t value);
    uint8_t _get_memory8(uint32_t address);
    uint32_t _get_memory32(uint32_t address);
    
    uint32_t _get_register32(Register reg);
    uint8_t _get_register8(Register reg);
    
    uint32_t _calc_memory_address(ModRM &modrm);
    
    uint32_t _get_r32(ModRM &modrm);
    uint8_t _get_r8(ModRM &modrm);
    void _set_r32(ModRM &modrm, uint32_t value);
    void _set_r8(ModRM &modrm, uint8_t value);
    uint32_t _get_rm32(ModRM &modrm);
    uint8_t _get_rm8(ModRM &modrm);
    
    void _push32(uint32_t value);
    uint32_t _pop32();
    
    void _update_eflags_sub(uint32_t v1, uint32_t v2, uint64_t result);
    
    void _set_carry(int flag);
    void _set_zero(int flag);
    void _set_sign(int flag);
    void _set_overflow(int flag);
    
    bool _is_carry();
    bool _is_zero();
    bool _is_sign();
    bool _is_overflow();
    
    uint8_t _io_in8(uint16_t address);
    void _io_out8(uint16_t address, uint8_t value);
    
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
    void _add_rm32_imm8(ModRM &modrm);
    
    void _code_ff();
    void _inc_rm32(ModRM &modrm);
    
    void _push_r32();
    void _push_imm8();
    void _push_imm32();
    void _pop_r32();
    
    void _call_rel32();
    void _ret();
    
    void _leave();
    
    void _cmp_r32_rm32();
    void _cmp_rm32_imm8(ModRM &modrm);
    
    void _jc();
    void _jz();
    void _js();
    void _jo();
    void _jnc();
    void _jnz();
    void _jns();
    void _jno();
    
    void _jl();
    void _jle();
    
    void _in_al_dx();
    void _out_dx_al();
    
    void _mov_r8_imm8();
    void _cmp_al_imm8();
    void _mov_rm8_r8();
    void _mov_r8_rm8();
    void _inc_r32();
};

#endif
