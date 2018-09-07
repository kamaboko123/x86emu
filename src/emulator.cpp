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

void emulator::_parse_modrm(ModRM &modrm){
    uint8_t code = _get_code8(0);
    
    memset(&modrm, 0, sizeof(ModRM));
    modrm.mod = (code & 0xC0) >> 6;
    modrm.opecode = (code & 0x38) >> 3;
    modrm.rm = code & 0x07;
    
    eip++;
    
    //SIBがある場合はフェッチ
    if(modrm.mod != 3 && modrm.rm == 4){
        modrm.sib = _get_code8(0);
        eip++;
    }
    
    //ディスプレースメント 32bit
    //(mod == 00, rm == 101)を見落としそうなので注意
    if((modrm.mod == 0 && modrm.rm == 5) || modrm.mod == 2){
        modrm.disp32 = _get_sign_code32(0);
        eip += 4;
    }
    //ディスプレースメント 8bit
    else if(modrm.mod == 1){
        modrm.disp8 = _get_sign_code8(0);
        eip++;
    }
    
    //これ以外はレジスタか、メモリアドレスの間接指定(たぶん)
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

//Mod/RMで指定されたレジスタ・メモリに値を格納する
void emulator::_set_rm32(ModRM &modrm, uint32_t value){
    //レジスタ指定
    if(modrm.mod == 3){
        _set_register32(static_cast<Register>(modrm.rm), value);
    }
    //メモリ指定
    else{
        //Mod/RMをもとにアドレス計算
        uint32_t address = _calc_memory_address(modrm);
        _set_memory32(address, value);
    }
}

void emulator::_set_register32(Register reg, uint32_t value){
    registers[reg] = value;
}

void emulator::_set_memory32(uint32_t address, uint32_t value){
    memory[address] = value;
}

uint32_t emulator::_get_register32(Register reg){
    return registers[reg];
}

uint32_t emulator::_calc_memory_address(ModRM &modrm){
    if(modrm.mod == 0){
        if(modrm.rm == 4){
            fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
            exit(-1);
        }
        else if(modrm.rm == 5){
            return modrm.disp32;
        }
        else{
            return _get_register32(static_cast<Register>(modrm.rm));
        }
    }
    else if(modrm.mod == 1){
        if(modrm.rm == 4){
            fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
            exit(-1);
        }
        else{
            return _get_register32(static_cast<Register>(modrm.rm)) + modrm.disp8;
        }
    }
    else if(modrm.mod == 2){
        if(modrm.rm == 4){
            fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
            exit(-1);
        }
        else{
            return _get_register32(static_cast<Register>(modrm.rm)) + modrm.disp32;
        }
    }
    else{
        fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
        exit(-1);
    }
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

void emulator::_mov_rm32_imm32(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    uint32_t value = _get_code32(0);
    eip += 4;
    
    _set_rm32(modrm, value);
}

