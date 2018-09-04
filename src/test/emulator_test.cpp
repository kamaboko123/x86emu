#include <cppunit/extensions/HelperMacros.h>
#include "emulator.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME EmulatorTest

const uint32_t MEMORY_SIZE = 1024 * 1024;
const uint32_t INIT_EIP = 0x00;
const uint32_t INIT_ESP = 0x7c00;

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_mov_r32_imm32);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_mov_r32_imm32();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_mov_r32_imm32(){
   emulator emu(MEMORY_SIZE, INIT_EIP, INIT_ESP);
   
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

