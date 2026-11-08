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

TESTS := $(OBJ_DIR)/test_WatchdogTimer $(OBJ_DIR)/test_ExternInterrupt $(OBJ_DIR)/test_ADC $(OBJ_DIR)/test_ServoMotor

.PHONY: all test clean
all: $(TESTS)

test: all
	@set -e; for t in $(TESTS); do echo "=== $$t ==="; ./$$t; done

test_WatchdogTimer_OBJS := $(OBJ_DIR)/WatchdogTimer.o
test_ExternInterrupt_OBJS := $(OBJ_DIR)/ExternInterrupt.o
test_ADC_OBJS := $(OBJ_DIR)/ADC.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o
test_ServoMotor_OBJS := $(OBJ_DIR)/ServoMotor.o $(OBJ_DIR)/TimerCounter1.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o

$(OBJ_DIR)/test_WatchdogTimer: $(OBJ_DIR)/test_WatchdogTimer.o $(test_WatchdogTimer_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ExternInterrupt: $(OBJ_DIR)/test_ExternInterrupt.o $(test_ExternInterrupt_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ADC: $(OBJ_DIR)/test_ADC.o $(test_ADC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ServoMotor: $(OBJ_DIR)/test_ServoMotor.o $(test_ServoMotor_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# generic rules
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

# library sources compiled for the host
$(OBJ_DIR)/WatchdogTimer.o: $(LIB_DIR)/core/WatchdogTimer.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/ExternInterrupt.o: $(LIB_DIR)/core/ExternInterrupt.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/ADC.o: $(LIB_DIR)/core/ADC.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/MCU.o: $(LIB_DIR)/core/MCU.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/Pin.o: $(LIB_DIR)/io/Pin.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/ServoMotor.o: $(LIB_DIR)/component/ServoMotor.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/TimerCounter1.o: $(LIB_DIR)/core/TimerCounter1.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
