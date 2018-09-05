#include <cstdlib>
#include "emulator.hpp"

emulator::emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp){
    for(int i = 0; i < REGISTERS_COUNT; i++) registers[i] = 0;
    memory = new uint8_t[memory_size]();
    eip = init_eip;
    registers[ESP] = init_esp;
    
    _init_instructions();
}

emulator::~emulator(){
    delete memory;
}

void emulator::_init_instructions(){
    for(int i = 0; i < 8; i++){
        instructions[0xB8 + i] = &emulator::_mov_r32_imm32;
    }
    instructions[0xE9] = &emulator::_near_jump;
    instructions[0xEB] = &emulator::_short_jump;
};

void emulator::load_program(const char *filename, uint32_t size, uint32_t index){
    FILE *binary;
    
    binary = fopen(filename, "rb");
    if(binary == NULL){
        fprintf(stderr, "error : failed to read program file.\n");
        exit(-1);
    }
    fread(memory + index, 1, size, binary);
    fclose(binary);
}

void emulator::dump_registers(){
    fprintf(stderr, "------[registers]------\n");
    fprintf(stderr, "[EAX] %08x\n", registers[EAX]);
    fprintf(stderr, "[ECX] %08x\n", registers[ECX]);
    fprintf(stderr, "[EDX] %08x\n", registers[EDX]);
    fprintf(stderr, "[EBX] %08x\n", registers[EBX]);
    fprintf(stderr, "[ESP] %08x\n", registers[ESP]);
    fprintf(stderr, "[EBP] %08x\n", registers[EBP]);
    fprintf(stderr, "[ESI] %08x\n", registers[ESI]);
    fprintf(stderr, "[EDI] %08x\n", registers[EDI]);
    fprintf(stderr, "-----------------------\n");
}

bool emulator::exec(){
    uint8_t code = _get_code8(0);
    
    (this->*instructions[code])();
    
    if(eip == 0x00) return false;
    return true;
}

uint8_t emulator::_get_code8(uint32_t index){
    return memory[eip + index];
}

int8_t emulator::_get_sign_code8(uint32_t index){
    return memory[eip + index];
}

uint32_t emulator::_get_code32(uint32_t index){
    uint32_t ret = 0;
    for(int i = 0; i < 4; i++){
        ret += (uint32_t)_get_code8(index + i) << i * 8;
    }
    return ret;
}

int32_t emulator::_get_sign_code32(uint32_t index){
    return _get_code32(index);
}

void emulator::_mov_r32_imm32(){
    uint8_t reg = _get_code8(0) - 0xB8;
    registers[reg] = _get_code32(1);
    eip += 5;
}
void emulator::_short_jump(){
    int8_t diff = _get_sign_code8(1);
    //opecode(1byte) + operand(1byte)
    eip += 2;
    eip += diff;
}

void emulator::_near_jump(){
    int32_t diff = _get_sign_code32(1);
    //opecode(1byte) + operand(4byte)
    eip += 5;
    eip += diff;
}

