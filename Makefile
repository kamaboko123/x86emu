#Makefile

CC = g++
INCLUDE = -I ./include
CFLAGS = -std=c++11 -g -Wall -O0 -m32

SRC_DIR = ./src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/emu

OBJ_DIR = ./.obj
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

.SECONDARY: $(OBJ)
all: $(TARGET)

$(TARGET_DIR)/%: $(OBJ)
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -rf $(TARGET_DIR) $(OBJ_DIR)


