
# ──────────────────── Makefile ──────────────────────
# Ações:
#   make          → compila o executável em build/banco
#   make run      → compila e executa
#   make clean    → remove artefatos de build
#   make rebuild  → clean + make
 
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS :=
 
SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/banco
 
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo ""
	@echo "  Build OK → $(TARGET)"
	@echo ""
 
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
 
run: all
	@echo "─────────────────────────────────────────"
	@./$(TARGET)
 
clean:
	rm -rf $(BUILD_DIR) contas.dat
	@echo "Artefatos removidos."
 
rebuild: clean all
 
.PHONY: all run clean rebuild