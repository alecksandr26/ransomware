C_WIN = i686-w64-mingw32-cc
C_DEBUG_FLAGS = -pedantic -Wall -ggdb
C_COMPILE_FLAGS = -O2 -DNDEBUG -fno-stack-protector -no-pie
C_FLAGS = $(C_DEBUG_FLAGS)
C_WIN_LIB_FLAGS =  -lshlwapi 

SRC_DIR = src
OUT_DIR = build
BIN = $(addprefix $(OUT_DIR)/, ransomware.exe)

OBJ_DIR = $(addprefix $(OUT_DIR)/, obj)
OBJS  = $(addprefix $(OBJ_DIR)/, main.o bit.o des.o hash.o encipher.o file_encipher.o key.o)

all: $(OUT_DIR) $(OBJ_DIR) $(BIN)

$(OUT_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(C_WIN) $(C_FLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(C_WIN) $(C_FLAGS) $^ -o $@ $(C_WIN_LIB_FLAGS)

clean:
	rm -rf $(OUT_DIR)


compile: C_FLAGS = $(C_COMPILE_FLAGS)
compile: clean all

