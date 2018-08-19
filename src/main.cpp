#include <cstdio>
#include <cstdlib>
#include "emulator.hpp"

#define BINARY_SIZE 0x200

int main(int argc, char *argv[]){
    
    if(argc != 2){
        fprintf(stderr, "error : you must specify program filename.\n");
        exit(-1);
    }
    
    emulator emu(1024*1024, 0x00, 0x00);
    emu.load_program(argv[1], BINARY_SIZE);
    
    while(emu.exec());
    
    return 0;
}

