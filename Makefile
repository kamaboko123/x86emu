#Makefile

CC = g++
INCLUDE = -I include
CFLAGS = -std=c++11 -g -Wall -O0

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_DIR = bin
TARGET = $(TARGET_DIR)/emu

OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

#test codes
TEST_INCLUDE = $(INCLUDE) -I include/test
TEST_LDFLAGS = -l cppunit
TEST_SRC_DIR = src/test
TEST_SRC = $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ_DIR = .obj/test
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, $(notdir $(TEST_SRC:.cpp=.o)))
TEST_TARGET_DIR = bin
TEST_TARGET = $(TEST_TARGET_DIR)/emu_test
TEST_RESULT_FILE = test_result.xml
#exclude main file for avoid duplicate main fuction with test codes
MAIN_OBJ = $(OBJ_DIR)/main.o

#test assembler codes
AS = nasm
ASM_SRC_DIR = src/test/asm/src
ASM_SRC = $(wildcard $(ASM_SRC_DIR)/*.asm)
ASM_TARGET_DIR = bin/test_bin
ASM_TARGET = $(addprefix $(ASM_TARGET_DIR)/, $(notdir $(ASM_SRC:.asm=.bin)))

.SECONDARY: $(OBJ)

all: $(TARGET) test test_asm

$(TARGET_DIR)/%: $(OBJ)
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

test: $(OBJ) $(TEST_OBJ)
	mkdir -p $(TEST_TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(filter-out $(MAIN_OBJ), $^) $(TEST_LDFLAGS)

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -D RESULT_FILE=\"$(TEST_RESULT_FILE)\" -o $@ -c $<

test_asm: $(ASM_TARGET)

$(ASM_TARGET_DIR)/%.bin: $(ASM_SRC)
	mkdir -p $(ASM_TARGET_DIR)
	$(AS) -o $@ $<

clean:
	rm -rf $(TARGET_DIR) $(OBJ_DIR) $(TEST_TARGET_DIR) $(TEST_OBJ_DIR)


