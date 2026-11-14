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
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wno-attributes -D__AVR_ATmega328P__ -DF_CPU=16000000UL -DBAUD=9600 -DSPI_TIMEOUT_CYCLES=200000000U

STUBS    := -Istubs
LIB_INC  := -I../../lib/core -I../../lib/io -I../../lib/component -I../../lib/utils
LIB_DIR  := ../../lib

OBJ_DIR  := obj

# each test lists the library objects it needs
TESTS := $(OBJ_DIR)/test_WatchdogTimer $(OBJ_DIR)/test_ExternInterrupt $(OBJ_DIR)/test_ADC $(OBJ_DIR)/test_ServoMotor $(OBJ_DIR)/test_USART0 $(OBJ_DIR)/test_SPI $(OBJ_DIR)/test_StepperMotor $(OBJ_DIR)/test_LDR $(OBJ_DIR)/test_map $(OBJ_DIR)/test_Organ $(OBJ_DIR)/test_PushButton $(OBJ_DIR)/test_Buzzer $(OBJ_DIR)/test_TWI $(OBJ_DIR)/test_PIDController $(OBJ_DIR)/test_RotaryEncoder

.PHONY: all test clean
all: $(TESTS)

test: all
	@failed=""; \
	for t in $(TESTS); do \
		echo "=== $$t ==="; \
		if ! ./$$t; then \
			failed="$$failed $$t"; \
		fi; \
	done; \
	if [ -n "$$failed" ]; then \
		echo "FAILED TEST BINARIES:$$failed"; \
		exit 1; \
	fi

test_WatchdogTimer_OBJS := $(OBJ_DIR)/WatchdogTimer.o
test_ExternInterrupt_OBJS := $(OBJ_DIR)/ExternInterrupt.o
test_ADC_OBJS := $(OBJ_DIR)/ADC.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o
test_ServoMotor_OBJS := $(OBJ_DIR)/ServoMotor.o $(OBJ_DIR)/TimerCounter1.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o

test_USART0_OBJS := $(OBJ_DIR)/USART0.o $(OBJ_DIR)/MCU.o

test_SPI_OBJS := $(OBJ_DIR)/SPI.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o

test_StepperMotor_OBJS := $(OBJ_DIR)/StepperMotor.o $(OBJ_DIR)/Pin.o
test_LDR_OBJS := $(OBJ_DIR)/LDR.o $(OBJ_DIR)/ADC.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o
test_map_OBJS := $(OBJ_DIR)/utils_m328p.o
test_Organ_OBJS :=
test_PushButton_OBJS := $(OBJ_DIR)/PushButton.o $(OBJ_DIR)/Pin.o
test_Buzzer_OBJS := $(OBJ_DIR)/Buzzer.o $(OBJ_DIR)/TimerCounter1.o $(OBJ_DIR)/MCU.o $(OBJ_DIR)/Pin.o
test_TWI_OBJS := $(OBJ_DIR)/TWI.o $(OBJ_DIR)/MCU.o
test_PIDController_OBJS := $(OBJ_DIR)/PIDController.o
test_RotaryEncoder_OBJS := $(OBJ_DIR)/RotaryEncoder.o $(OBJ_DIR)/Pin.o

$(OBJ_DIR)/test_WatchdogTimer: $(OBJ_DIR)/test_WatchdogTimer.o $(test_WatchdogTimer_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ExternInterrupt: $(OBJ_DIR)/test_ExternInterrupt.o $(test_ExternInterrupt_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ADC: $(OBJ_DIR)/test_ADC.o $(test_ADC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_ServoMotor: $(OBJ_DIR)/test_ServoMotor.o $(test_ServoMotor_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_USART0: $(OBJ_DIR)/test_USART0.o $(test_USART0_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# test_SPI needs pthreads for the thread that runs the blocking transfer
$(OBJ_DIR)/test_SPI: $(OBJ_DIR)/test_SPI.o $(test_SPI_OBJS)
	$(CXX) $(CXXFLAGS) -pthread -o $@ $^

$(OBJ_DIR)/test_StepperMotor: $(OBJ_DIR)/test_StepperMotor.o $(test_StepperMotor_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_LDR: $(OBJ_DIR)/test_LDR.o $(test_LDR_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_map: $(OBJ_DIR)/test_map.o $(test_map_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_PushButton: $(OBJ_DIR)/test_PushButton.o $(test_PushButton_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_Buzzer: $(OBJ_DIR)/test_Buzzer.o $(test_Buzzer_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_TWI: $(OBJ_DIR)/test_TWI.o $(test_TWI_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_PIDController: $(OBJ_DIR)/test_PIDController.o $(test_PIDController_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/test_RotaryEncoder: $(OBJ_DIR)/test_RotaryEncoder.o $(test_RotaryEncoder_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# pure-arithmetic characterization: no library objects needed
$(OBJ_DIR)/test_Organ: $(OBJ_DIR)/test_Organ.o $(test_Organ_OBJS)
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

$(OBJ_DIR)/USART0.o: $(LIB_DIR)/io/USART0.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/SPI.o: $(LIB_DIR)/io/SPI.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/StepperMotor.o: $(LIB_DIR)/component/StepperMotor.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/LDR.o: $(LIB_DIR)/component/LDR.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/PushButton.o: $(LIB_DIR)/component/PushButton.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/Buzzer.o: $(LIB_DIR)/component/Buzzer.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/utils_m328p.o: $(LIB_DIR)/utils/utils_m328p.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/TWI.o: $(LIB_DIR)/core/TWI.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/PIDController.o: $(LIB_DIR)/utils/PIDController.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR)/RotaryEncoder.o: $(LIB_DIR)/component/RotaryEncoder.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(STUBS) $(LIB_INC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
