CC          = gcc
CFLAGS      = -std=c99 -Wall -Werror -Wextra -Wno-unused -pedantic -g -O3 -D_POSIX_C_SOURCE=2 -lcrypto libs/hash/sha1.c

SRC_DIR     = ./src
BIN_DIR     = ./bin

SOURCES    := $(wildcard $(SRC_DIR)/*.c)
OBJECTS    := $(SOURCES:$(SRC_DIR)%.c=$(BIN_DIR)%)

.PHONY:    all clean

all:       main

main:      $(OBJECTS)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(FLAGS) $< -o $@

clean:
	-rm bin/*

