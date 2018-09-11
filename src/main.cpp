#include <cstdio>
#include <cstdlib>
#include "emulator.hpp"

#define BINARY_SIZE 0x200

int main(int argc, char *argv[]){
    
    emulator emu(1024*1024, 0x7c00, 0x7c00);
    if(argc != 2){
        fprintf(stderr, "error : you must specify program filename.\n");
        exit(-1);
    }
    
    emu.load_program(argv[1], BINARY_SIZE);
    
    emu.dump_registers();
    while(emu.exec()){}
    emu.dump_registers();
    
    return 0;
}

