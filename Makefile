EXECUTABLE ?= kinetikos

BUILD_DIR ?= ./_build
OUT := $(BUILD_DIR)/.make
 
VENDOR ?= ./_vendor
INCLUDE := $(VENDOR)/include

TOOLCHAIN_DIR ?= ../toolchain
TOOLCHAIN ?= $(shell $(CC) -dumpmachine)
TOOLCHAIN ?= $(shell $(CXX) -dumpmachine)

EXCLUDE := "control/PiPololuMotorControl.cpp"

SRCS := $(shell find -name "*.cpp" -or -name "*.c" | grep -v $(addprefix -e ,$(EXCLUDE)) | grep -v $(VENDOR) | grep -v $(TOOLCHAIN))
OBJS = $(SRCS:%=$(OUT)/%.o)
DEPS = $(OBJS:.o=.d)

INC_DIRS := $(shell find -type d | grep -v $(BUILD_DIR) | grep -v $(VENDOR) | grep -v .git) $(VENDOR)/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LIB_NAMES := libuWS.so
BIN ?= $(OUT)/bin
LIBRARIES = $(LIB_NAMES:%=$(OUT)/%)
LIBS ?= -L$(BIN) -lm -lz -lssl -luWS
CFLAGS ?= -std=c99
CXXFLAGS ?= -std=c++11

.PHONY: default
default: target $(OUT)/$(EXECUTABLE)

$(OUT)/$(EXECUTABLE): $(OBJS) $(BIN)/libuWS.so
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

$(OUT)/%.c.o: %.c $(BIN)/libuWS.so
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBS) -c $< -o $@

$(OUT)/%.cpp.o: %.cpp $(BIN)/libuWS.so
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

.PHONY: target
target:
	@mkdir -p $(BUILD_DIR)/$(TOOLCHAIN)
	@ln -sfn $(TOOLCHAIN) $(OUT)

%/libuWS.so:
	$(MAKE) -B -C $(VENDOR)/uWebSockets \
		CC=$(CC) \
		CXX=$(CXX) \
		CPATH=$(CPATH)
	mkdir -p $*
	cp $(VENDOR)/uWebSockets/libuWS.so $*/
	ls $*

-include $(DEPS)

.PHONY: run
run: default
	@export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)/$(BIN) && $(OUT)/$(EXECUTABLE)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: setup
setup:
	git clone https://github.com/uNetworking/uWebSockets.git $(VENDOR)/uWebSockets
	mkdir -p $(INCLUDE)/uWS
	cp -l $(VENDOR)/uWebSockets/src/*.h $(INCLUDE)/uWS
	git clone git://git.drogon.net/wiringPi $(VENDOR)/wiringPi
	mkdir -p $(INCLUDE)/wiringPi
	cp -l $(VENDOR)/wiringPi/wiringPi/*.h $(VENDOR)/include/wiringPi
	git clone https://github.com/google/flatbuffers.git $(VENDOR)/flatbuffers
	cd $(VENDOR)/flatbuffers && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release && make
	cp -l -r $(VENDOR)/flatbuffers/include/flatbuffers $(VENDOR)/include/
	#cp -l -r $(VENDOR)/flatbuffers/js $(VENDOR)/js

# platform targets
.PHONY: arm
arm:
	$(eval ARMTC = $(shell pwd)/$(TOOLCHAIN_DIR)/arm/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64)
	$(eval CC = $(ARMTC)/bin/arm-linux-gnueabihf-gcc)
	$(eval CXX = $(ARMTC)/bin/arm-linux-gnueabihf-g++)
	$(eval CPATH = $(ARMTC)/arm-linux-gnueabihf/libc/usr/include:$(ARMTC)/arm-linux-gnueabihf/libc/include/arm-linux-gnyeabihf)

.PHONY: rpi
rpi: arm
rpi: target
rpi: $(OUT)/./control/PiPololuMotorControl.cpp.o
rpi: $(OUT)/$(EXECUTABLE)

