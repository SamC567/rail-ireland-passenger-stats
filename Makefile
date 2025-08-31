CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -O2
INCLUDES=-Iinclude
LDFLAGS=

SRC=src/main.c src/passenger.c src/login.c src/fileio.c src/utils.c
OBJ=$(SRC:.c=.o)
BIN=rail_ireland

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
