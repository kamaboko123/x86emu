TARGET_DIR = ../../../../../bin/data
TARGET = $(TARGET_DIR)/c-test.bin
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
	mkdir -p $(TARGET_DIR)/tmp
	$(CC) $(CFLAGS) -c -o $(TARGET_DIR)/tmp/test.o test.c
	$(AS) -f elf crt0.asm -o $(TARGET_DIR)/tmp/crt0.o
	$(LD) $(LDFLAGS) -o $@ $(TARGET_DIR)/tmp/crt0.o $(TARGET_DIR)/tmp/test.o

clean:
	rm -rf $(TARGET)
