TARGET_DIR = ../../../../../bin/data
TMP_DIR = $(TARGET_DIR)/tmp/while-test
TARGET = $(TARGET_DIR)/while-test.bin
OBJS = crt0.o test.o

CC = gcc
LD = ld
AS = nasm
CFLAGS += -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector -m32
LDFLAGS += --entry=start --oformat=binary -Ttext 0x7c00 -m elf_i386

.PHONY: all
all : $(TARGET)

$(TARGET) : crt0.asm test.c
	mkdir -p $(TARGET_DIR)
	mkdir -p $(TMP_DIR)
	$(CC) $(CFLAGS) -c -o $(TMP_DIR)/test.o test.c
	$(AS) -f elf crt0.asm -o $(TMP_DIR)/crt0.o
	$(LD) $(LDFLAGS) -o $@ $(TMP_DIR)/crt0.o $(TMP_DIR)/test.o

clean:
	rm -rf $(TARGET)
