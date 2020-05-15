CC = gcc
CFLAGS = -std=c99 -Wall -g -pthread

PROGRAM = client

BIN_DIR = bin
SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.c)


$(PROGRAM):
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(PROGRAM) $(SRC)

clean:
	rm -f $(BIN_DIR)/*

run:
	$(BIN_DIR)/$(PROGRAM) $(ARGS)

