#include <cppunit/extensions/HelperMacros.h>
#include "emulator.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME EmulatorTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_mov_r32_imm32);
    CPPUNIT_TEST(test_near_jump);
    CPPUNIT_TEST(test_modrm);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_mov_r32_imm32();
    void test_near_jump();
    void test_modrm();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_mov_r32_imm32(){
    emulator emu(1024 * 1024, 0x00, 0x7c00);
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
    
    emu.load_program("bin/data/mov_r32_imm32.bin", 0x0200);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000029, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}

void FIXTURE_NAME::test_near_jump(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
    
    emu.load_program("bin/data/near_jump.bin", 0x0200, 0x7c00);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000029, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}

void FIXTURE_NAME::test_modrm(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
    
    emu.load_program("bin/data/modrm-test.bin", 0x0200, 0x7c00);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000002, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007bf0, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007bf0, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000008, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}


