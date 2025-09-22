
CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++11
SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp,build/%.o,$(SRC))
BIN=build/app

.PHONY: all run test clean

.PHONY: all run test clean

all: $(BIN)

build/%.o: src/%.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

test: $(BIN) tests/test_basic.sh
	bash tests/test_basic.sh

clean:
	rm -rf build
