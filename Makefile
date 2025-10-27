
CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++11
# Find all .cpp files recursively under src
SRC:=$(shell find src -name '*.cpp')
# Map src/<path>.cpp -> build/<path>.o
OBJ=$(patsubst src/%.cpp,build/%.o,$(SRC))
BIN=build/app

.PHONY: all run test clean

.PHONY: all run test clean

all: $(BIN)

build/%.o: src/%.cpp | build
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

OBJDIRS := $(sort $(dir $(OBJ)))

build:
	mkdir -p $(OBJDIRS)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

test: $(BIN) tests/test_basic.sh
	bash tests/test_basic.sh

clean:
	rm -rf build
