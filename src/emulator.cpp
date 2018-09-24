#include "emulator.hpp"

emulator::emulator(uint32_t memory_size, uint32_t init_eip, uint32_t init_esp){
    for(int i = 0; i < REGISTERS_COUNT; i++) registers[i] = 0;
    memory = new uint8_t[memory_size]();
    eip = init_eip;
    registers[ESP] = init_esp;
    eflags = 0;
    
    _init_instructions();
}

emulator::~emulator(){
    delete memory;
}

void emulator::_init_instructions(){
    for (int i = 0; i < INSTRUCTION_NUM; i++) instructions[i] = 0;
    
    instructions[0x01] = &emulator::_add_rm32_r32;
    instructions[0x3B] = &emulator::_cmp_r32_rm32;
    for(int i = 0; i < 8; i++){
        instructions[0xB8 + i] = &emulator::_mov_r32_imm32;
        instructions[0x50 + i] = &emulator::_push_r32;
        instructions[0x58 + i] = &emulator::_pop_r32;
    }
    instructions[0x6A] = &emulator::_push_imm8;
    instructions[0x68] = &emulator::_push_imm8;
   
    instructions[0x70] = &emulator::_jo;
    instructions[0x71] = &emulator::_jno;
    instructions[0x72] = &emulator::_jc;
    instructions[0x73] = &emulator::_jnc;
    instructions[0x74] = &emulator::_jz;
    instructions[0x75] = &emulator::_jnz;
    instructions[0x78] = &emulator::_js;
    instructions[0x79] = &emulator::_jns;
    instructions[0x7C] = &emulator::_jl;
    instructions[0x7E] = &emulator::_jle;
    
    instructions[0x83] = &emulator::_code_83;
    instructions[0x89] = &emulator::_mov_rm32_r32;
    instructions[0x8B] = &emulator::_mov_r32_rm32;
    instructions[0xC3] = &emulator::_ret;
    instructions[0xC7] = &emulator::_mov_rm32_imm32;
    instructions[0xC9] = &emulator::_leave;
    instructions[0xE8] = &emulator::_call_rel32;
    instructions[0xE9] = &emulator::_near_jump;
    instructions[0xEB] = &emulator::_short_jump;
    instructions[0xEC] = &emulator::_in_al_dx;
    instructions[0xEE] = &emulator::_out_dx_al;
    instructions[0xFF] = &emulator::_code_ff;
};

void emulator::load_program(const char *filename, uint32_t size){
    FILE *binary;
    
    binary = fopen(filename, "rb");
    if(binary == NULL){
        fprintf(stderr, "error : failed to read program file.\n");
        exit(-1);
    }
    fread(memory + 0x7c00, 1, size, binary);
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
    fprintf(stderr, "frags : %c%c%c%c\n",
        _is_carry() ? 'C' : ' ',
        _is_zero() ? 'Z' : ' ',
        _is_sign() ? 'S' : ' ',
        _is_overflow() ? 'O' : ' '
    );
    fprintf(stderr, "-----------------------\n");
    fprintf(stderr, "\n");
}

bool emulator::exec(){
    uint8_t code = _get_code8(0);
    
    void (emulator::*ins)() = instructions[code];
    if(ins == NULL){
        fprintf(stderr, "error : not implemted instruction. code=0x%02x\n", code);
        exit(-1);
    }
    
    //fprintf(stderr, "[exec]code=0x%02x\n", code);
    (this->*ins)();
    
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

void emulator::_update_eflags_sub(uint32_t v1, uint32_t v2, uint64_t result){
    int32_t sign1 = v1 >> 31;
    int32_t sign2 = v2 >> 31;
    
    int signr = (result >> 31) & 1;
    
    _set_carry(result >> 32);
    _set_zero(result == 0);
    _set_sign(signr);
    _set_overflow((sign1 != sign2) && (signr != signr));
}

void emulator::_set_carry(int flag){
    if(flag){
        eflags |= CARRY_FLAG;
    }
    else{
        eflags &= ~CARRY_FLAG;
    }
}

void emulator::_set_zero(int flag){
    if(flag){
        eflags |= ZERO_FLAG;
    }
    else{
        eflags &= ~ZERO_FLAG;
    }
}

void emulator::_set_sign(int flag){
    if(flag){
        eflags |= SIGN_FLAG;
    }
    else{
        eflags &= ~SIGN_FLAG;
    }
}

void emulator::_set_overflow(int flag){
    if(flag){
        eflags |= OVERFLOW_FLAG;
    }
    else{
        eflags &= ~OVERFLOW_FLAG;
    }
}

bool emulator::_is_carry(){
    return((eflags & CARRY_FLAG) != 0);
}
bool emulator::_is_zero(){
    return((eflags & ZERO_FLAG) != 0);
}
bool emulator::_is_sign(){
    return((eflags & SIGN_FLAG) != 0);
}
bool emulator::_is_overflow(){
    return((eflags & OVERFLOW_FLAG) != 0);
}


uint32_t emulator::_get_code32(uint32_t index){
    uint32_t ret = 0;
    for(int i = 0; i < 4; i++){
        ret += (uint32_t)_get_code8(index + i) << (i * 8);
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

void emulator::_set_register8(Register reg, uint8_t value){
    if(reg <= EBX){
        registers[reg] &= 0xFFFFFF00;
        registers[reg] |= (uint32_t)value;
    }
    else{
        registers[reg - 4] &= 0xFFFF00FF;
        registers[reg - 4] |= ((uint32_t)value << 8);
    }
}

void emulator::_set_memory8(uint32_t address, uint8_t value){
    memory[address] = value;
}

void emulator::_set_memory32(uint32_t address, uint32_t value){
    for(int i = 0; i < 4; i++){
        _set_memory8(address + i, (value >> (i * 8)) & 0xFF);
    }
}

uint8_t emulator::_get_memory8(uint32_t address){
    return(memory[address]);
}

uint32_t emulator::_get_memory32(uint32_t address){
    uint32_t value = 0;
    for(int i = 0; i < 4; i++){
        value += (uint32_t)_get_memory8(address + i) << (i * 8);
    }
    return(value);
}

uint32_t emulator::_get_register32(Register reg){
    return registers[reg];
}

uint8_t emulator::_get_register8(Register reg){
    //レジスタのindexはenum Registerの定義を参照
    
    //EAX(=AL), ECX(=CL), EDX(=DL), EBX(=BL)の場合
    //下位1byteを取り出す
    if(reg <= EBX){
        return registers[reg] & 0x000000FF;
    }
    
    //AL, CL, DL, BLの場合
    //8-15bitを取り出し
    return (registers[reg - 4] >> 8) & 0x000000FF;
}

uint32_t emulator::_get_r32(ModRM &modrm){
    return(_get_register32(static_cast<Register>(modrm.reg_index)));
}

void emulator::_set_r32(ModRM &modrm, uint32_t value){
    _set_register32(static_cast<Register>(modrm.reg_index), value);
}

uint32_t emulator::_get_rm32(ModRM &modrm){
    if(modrm.mod == 3){
        return(_get_register32(static_cast<Register>(modrm.rm)));
    }
    else{
        uint32_t address = _calc_memory_address(modrm);
        return(_get_memory32(address));
    }
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

void emulator::_mov_rm32_r32(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    uint32_t value = _get_r32(modrm);
    _set_rm32(modrm, value);
}


void emulator::_mov_r32_rm32(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    uint32_t value = _get_rm32(modrm);
    _set_r32(modrm, value);
}

void emulator::_add_rm32_r32(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    uint32_t rm32 = _get_rm32(modrm);
    uint32_t r32 = _get_r32(modrm);
    
    _set_rm32(modrm, rm32 + r32);
    
    //TODO: implement update eflags
}

void emulator::_sub_rm32_imm8(ModRM &modrm){
    uint32_t rm32 = _get_rm32(modrm);
    uint32_t imm8 = _get_sign_code8(0);
    eip++;
    
    uint64_t result = static_cast<uint64_t>(rm32) - imm8;
    _set_rm32(modrm, result);
    
    _update_eflags_sub(rm32, imm8, result);
}

void emulator::_code_83(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    switch(modrm.opecode){
        case 0:
            _add_rm32_imm8(modrm);
            break;
        case 5:
            _sub_rm32_imm8(modrm);
            break;
        case 7:
            _cmp_rm32_imm8(modrm);
            break;
        default:
            fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
            exit(-1);
    }
}

void emulator::_add_rm32_imm8(ModRM &modrm){
    uint32_t rm32 = _get_rm32(modrm);
    uint32_t imm8 = _get_sign_code8(0);
    eip++;
    
    _set_rm32(modrm, rm32 + imm8);
}

void emulator::_code_ff(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    switch(modrm.opecode){
        case 0:
            _inc_rm32(modrm);
            break;
        default:
            fprintf(stderr, "error : not implemted instruction. ModRM(mod=%d, rm=%d)\n", modrm.mod, modrm.rm);
            exit(-1);
    }
}

void emulator::_inc_rm32(ModRM &modrm){
    uint32_t rm32 = _get_rm32(modrm);
    _set_rm32(modrm, ++rm32);
}

void emulator::_push32(uint32_t value){
    uint32_t address = _get_register32(ESP) - 4;
    _set_register32(ESP, address);
    _set_memory32(address, value);
}

void emulator::_push_r32(){
    Register reg = static_cast<Register>(_get_code8(0) - 0x50);
    eip++;
    uint32_t value = _get_register32(reg);
    _push32(value);
}

void emulator::_push_imm8(){
    uint8_t value = _get_code8(1);
    _push32(value);
    eip += 2;
}

void emulator::_push_imm32(){
    uint32_t value = _get_code32(1);
    _push32(value);
    eip += 5;
}

uint32_t emulator::_pop32(){
    uint32_t address = _get_register32(ESP);
    uint32_t value = _get_memory32(address);
    _set_register32(ESP, address + 4);
    
    return(value);
}

void emulator::_pop_r32(){
    Register reg = static_cast<Register>(_get_code8(0) - 0x58);
    eip++;
    _set_register32(reg, _pop32());
}

void emulator::_call_rel32(){
    uint32_t diff = _get_sign_code32(1);
    _push32(eip + 5);
    eip += (diff + 5);
}

void emulator::_ret(){
    eip = _pop32();
}

void emulator::_leave(){
    //ESPにEBPを格納
    //これでPOPすると保存しておいた、呼び出し元のEBPが取得できるのでEBPに格納
    _set_register32(ESP, _get_register32(EBP));
    _set_register32(EBP, _pop32());
    
    eip++;
}

void emulator::_cmp_r32_rm32(){
    eip++;
    ModRM modrm;
    _parse_modrm(modrm);
    
    uint32_t r32 = _get_r32(modrm);
    uint32_t rm32 = _get_rm32(modrm);
    
    uint64_t result = static_cast<uint64_t>(r32) - static_cast<uint64_t>(rm32);
    
    _update_eflags_sub(r32, rm32, result);
}

//call from _code_83
void emulator::_cmp_rm32_imm8(ModRM &modrm){
    uint32_t rm32 = _get_rm32(modrm);
    uint32_t imm8 = static_cast<uint32_t>(_get_sign_code8(0));
    eip++;
    
    uint64_t result = static_cast<uint64_t>(rm32) - static_cast<uint64_t>(imm8);
    
    _update_eflags_sub(rm32, imm8, result);
}

void emulator::_jc(){
    int32_t diff = _is_sign() ? _get_sign_code8(1) : 0;
    eip += diff + 2;
}
void emulator::_jz(){
    int32_t diff = _is_zero() ? _get_sign_code8(1) : 0;
    eip += diff + 2;
}
void emulator::_js(){
    int32_t diff = _is_sign() ? _get_sign_code8(1) : 0;
    eip += diff + 2;
}
void emulator::_jo(){
    int32_t diff = _is_overflow() ? _get_sign_code8(1) : 0;
    eip += diff + 2;
}

void emulator::_jnc(){
    int32_t diff = _is_sign() ? 0 : _get_sign_code8(1);
    eip += diff + 2;
}
void emulator::_jnz(){
    int32_t diff = _is_zero() ? 0 : _get_sign_code8(1);
    eip += diff + 2;
}
void emulator::_jns(){
    int32_t diff = _is_sign() ? 0 : _get_sign_code8(1);
    eip += diff + 2;
}
void emulator::_jno(){
    int32_t diff = _is_overflow() ? 0 : _get_sign_code8(1);
    eip += diff + 2;
}

void emulator::_jl(){
    int32_t diff = 0;
    if(_is_sign() != _is_overflow()){
        diff = _get_sign_code8(1);
    }
    eip += diff + 2;
}

void emulator::_jle(){
    int32_t diff = 0;
    if(_is_zero() || (_is_sign() != _is_overflow())){
        diff = _get_sign_code8(1);
    }
    eip += diff + 2;
}

void emulator::_in_al_dx(){
    uint16_t address = _get_register32(EDX) & 0x0000FFFF;
    uint8_t value = _io_in8(address);
    _set_register8(AL, value);
    eip++;
}

void emulator::_out_dx_al(){
    uint16_t address = _get_register32(EDX) & 0x0000FFFF;
    uint8_t value = _get_register8(AL);
    _io_out8(address, value);
    eip++;
}

uint8_t emulator::_io_in8(uint16_t address){
    switch(address){
        case 0x03F8:
            return getchar();
            break;
        default:
            return 0;
    }
}

void emulator::_io_out8(uint16_t address, uint8_t value){
    switch(address){
        case 0x03F8:
            putchar(value);
            break;
    }
}
