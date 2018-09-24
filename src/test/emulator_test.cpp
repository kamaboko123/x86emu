#include <cppunit/extensions/HelperMacros.h>
#include "emulator.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME EmulatorTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_near_jump);
    CPPUNIT_TEST(test_modrm);
    CPPUNIT_TEST(test_call);
    CPPUNIT_TEST(test_c);
    CPPUNIT_TEST(test_arg);
    CPPUNIT_TEST(test_if);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_init();
    void test_near_jump();
    void test_modrm();
    void test_call();
    void test_c();
    void test_arg();
    void test_if();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_init(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EAX]);
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
    emu.load_program("bin/data/near_jump.bin", 0x0200);
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
    emu.load_program("bin/data/modrm-test.bin", 0x0200);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000002, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007bf0, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007bf0, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000007, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000008, emu.registers[EDI]);
}

void FIXTURE_NAME::test_call(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    emu.load_program("bin/data/call-test.bin", 0x0200);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x0000f1, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x00011a, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000029, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}

void FIXTURE_NAME::test_c(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    emu.load_program("bin/data/c-test.bin", 0x0200);
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

void FIXTURE_NAME::test_arg(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    emu.load_program("bin/data/arg-test.bin", 0x0200);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000007, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000002, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}

void FIXTURE_NAME::test_if(){
    emulator emu(1024 * 1024, 0x7c00, 0x7c00);
    emu.load_program("bin/data/if-test.bin", 0x0200);
    while(emu.exec());
    
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000003, emu.registers[EAX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ECX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBX]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x007c00, emu.registers[ESP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EBP]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[ESI]);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0x000000, emu.registers[EDI]);
}
