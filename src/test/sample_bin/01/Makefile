AS = nasm
SRC = test.asm
TARGET_DIR = bin
TARGET = ${TARGET_DIR}/test.bin

all:
	mkdir -p $(TARGET_DIR)
	$(AS) -o $(TARGET) $(SRC)

clean:
	rm -rf $(TARGET_DIR)
