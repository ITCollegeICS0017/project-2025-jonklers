CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++11
SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=app

.PHONY: all run test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

test: $(BIN) tests/test_basic.sh
	bash tests/test_basic.sh

clean:
	rm -f $(BIN) src/*.o
