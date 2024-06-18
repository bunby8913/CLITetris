
SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%.cxx,$(BUILD_DIR)/%.o,$(SOURCES))
PROGRAM := Tetris

CC := $(shell which clang || which gcc)
CFLAGS := -Wall -W -O -g -fno-exceptions -fno-rtti -I$(HEADER_DIR)
LIBS := stdc++ m
LDFLAGS := $(LIBS:%=-l%)

$(BUILD_DIR)/$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/$(PROGRAM) $(OBJECTS)
	rm -rf $(BUILD_DIR)
