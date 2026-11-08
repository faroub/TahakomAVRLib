# Host tests for TahakomAVRLib.
#
# Built with the host g++ (C++11), no AVR toolchain required.  The stub
# headers under stubs/ provide the avr-libc API used by the library; each
# test file defines the device registers it exercises as plain globals.
#
#   make        build all tests
#   make test   build and run all tests
#   make clean  remove build artifacts

CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wno-attributes -D__AVR_ATmega328P__ -DF_CPU=16000000UL -DBAUD=9600

STUBS    := -Istubs
LIB_INC  := -I../../lib/core -I../../lib/io -I../../lib/component -I../../lib/utils
LIB_DIR  := ../../lib

OBJ_DIR  := obj

TESTS := $(OBJ_DIR)/test_WatchdogTimer

.PHONY: all test clean
all: $(TESTS)

test: all
	@set -e; for t in $(TESTS); do echo "=== $$t ==="; ./$$t; done

test_WatchdogTimer_OBJS := $(OBJ_DIR)/WatchdogTimer.o

$(OBJ_DIR)/test_WatchdogTimer: $(OBJ_DIR)/test_WatchdogTimer.o $(test_WatchdogTimer_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# generic rules
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

# library sources compiled for the host
$(OBJ_DIR)/WatchdogTimer.o: $(LIB_DIR)/core/WatchdogTimer.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
