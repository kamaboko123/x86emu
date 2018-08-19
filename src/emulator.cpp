#include <cstdlib>
#include "emulator.hpp"

emulator::emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp){
    memory = new uint8_t[memory_size]();
    eip = init_eip;
    registers[ESP] = init_esp;
}

emulator::~emulator(){
    delete memory;
}

void _init(){
    
};

void emulator::load_program(char *filename, uint32_t size){
    FILE *binary;
    
    binary = fopen(filename, "rb");
    if(binary == NULL){
        fprintf(stderr, "error : failed to read program file.\n");
        exit(-1);
    }
    fread(memory, 1, size, binary);
    fclose(binary);
}

bool emulator::exec(){
    printf("[EXEC]\n");
    if(*(memory + eip) == 0x00){
        return false;
    }
    eip++;
    
    return true;
}

