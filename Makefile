
SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%.cxx,$(BUILD_DIR)/%.o,$(SOURCES))
PROGRAM := Tetris

CC := $(shell which clang || which gcc)
CFLAGS := -Wall -W -O -fno-exceptions -fno-rtti -I$(HEADER_DIR)
LIBS := stdc++ m
LDFLAGS := $(LIBS:%=-l%)

$(BUILD_DIR)/$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cxx $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/$(PROGRAM) $(OBJECTS)
