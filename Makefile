EXECUTABLE ?= kinetikos

BUILD_DIR ?= ./_build
OUT := $(BUILD_DIR)/.make
 
VENDOR ?= ./_vendor
TOOLCHAIN_DIR ?= ../toolchain
TOOLCHAIN ?= $(shell $(CC) -dumpmachine)
TOOLCHAIN ?= $(shell $(CXX) -dumpmachine)

EXCLUDE := "control/PiPololuMotorControl.cpp"

SRCS := $(shell find -name "*.cpp" -or -name "*.c" | grep -v $(addprefix -e ,$(EXCLUDE)) | grep -v $(VENDOR) | grep -v $(TOOLCHAIN))
OBJS = $(SRCS:%=$(BUILD_DIR)/.make/%.o)
DEPS = $(OBJS:.o=.d)

LIBRARIES = wiringPi uWebSockets

INC_DIRS := $(shell find -type d | grep -v $(BUILD_DIR)) $(addprefix $(VENDOR)/,$(LIBRARIES))
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LIBS ?= -lm
CFLAGS ?= -std=c99
CXXFLAGS ?= -std=c++11

.PHONY: default
default: target $(OUT)/$(EXECUTABLE)

$(OUT)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

$(OUT)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBS) -c $< -o $@

$(OUT)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

.PHONY: target
target:
	@mkdir -p $(BUILD_DIR)/$(TOOLCHAIN)
	@ln -sfn $(TOOLCHAIN) $(OUT)

-include $(DEPS)

.PHONY: run
run: default
	@$(OUT)/$(EXECUTABLE)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: setup
setup:
	git clone https://github.com/uNetworking/uWebSockets.git $(VENDOR)/uWebSockets
	git clone git://git.drogon.net/wiringPi $(VENDOR)/wiringPi

# platform targets
.PHONY: arm
arm: ARMTC = $(TOOLCHAIN_DIR)/arm/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
arm: CC = $(ARMTC)/arm-linux-gnueabihf-gcc
arm: CXX = $(ARMTC)/arm-linux-gnueabihf-g++
arm: target
arm: $(OUT)/./control/PiPololuMotorControl.cpp.o
arm: $(OUT)/$(EXECUTABLE)

