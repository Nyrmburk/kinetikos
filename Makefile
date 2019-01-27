EXECUTABLE ?= kinetikos

BUILD_DIR ?= ./_build
 
VENDOR ?= ./_vendor
REMOTE ?= remote/html
REMOTE_VENDOR ?= $(REMOTE)/_vendor
INCLUDE := $(VENDOR)/include

TOOLCHAIN_DIR ?= ../toolchain

TOOLCHAIN ?= $(shell $(CC) -dumpmachine)
TOOLCHAIN ?= $(shell $(CXX) -dumpmachine)

TARGET ?= local
ifeq ($(TARGET),arm)
$(eval ARMTC = $(shell pwd)/$(TOOLCHAIN_DIR)/arm/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64)
	CC = $(ARMTC)/bin/arm-linux-gnueabihf-gcc
	CXX = $(ARMTC)/bin/arm-linux-gnueabihf-g++
	CPATH = $(ARMTC)/arm-linux-gnueabihf/libc/usr/include:$(ARMTC)/arm-linux-gnueabihf/libc/include/arm-linux-gnyeabihf
endif

RELEASE ?= debug

OUT := $(BUILD_DIR)/$(TOOLCHAIN)/$(RELEASE)

RUNNER :=

ARGS :=

SRCS := $(shell find -name "*.cpp" -or -name "*.c" | grep -v $(VENDOR) | grep -v $(TOOLCHAIN))
OBJS = $(SRCS:%=$(OUT)/%.o)
DEPS = $(OBJS:.o=.d)

REMOTE_OUTS := \
	$(OUT)/$(REMOTE)/index.html \
	$(OUT)/$(REMOTE)/min.js \
	$(OUT)/$(REMOTE)/style.css \
	$(OUT)/$(REMOTE)/favicon.png

INC_DIRS := $(shell find -type d | grep -v $(BUILD_DIR) | grep -v $(VENDOR) | grep -v .git) $(VENDOR)/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LIB_NAMES := libuWS.so
BIN ?= $(OUT)/bin
LIBRARIES = $(LIB_NAMES:%=$(OUT)/%)
LIBS ?= -L$(BIN) -luWS -lwiringPi -lm -lz -lssl -lcrypto -lpthread -lrt -lcrypt
CFLAGS ?= -std=c99
CXXFLAGS ?= -std=c++11
LDFLAGS ?= -Wl,-rpath,'$$ORIGIN/bin'

ifeq ($(RELEASE),debug)
	CFLAGS += -g
	CXXFLAGS += -g
else ifeq ($(RELEASE),profile)
	CFLAGS += -Ofast -g
	CXXFLAGS += -Ofast -g
else ifeq ($(RELEASE),release)
	CFLAGS += -Ofast
	CXXFLAGS += -Ofast
endif

default: vendor $(OUT)/$(EXECUTABLE) $(OUT)/config $(REMOTE_OUTS)

$(OUT)/$(EXECUTABLE): $(OBJS) $(BIN)/libuWS.so $(BIN)/libwiringPi.so
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

$(OUT)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBS) -c $< -o $@

$(OUT)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(OUT)/config: config
	@mkdir -p $(dir $@)
	cp -r $< $@

%/libuWS.so:
	$(MAKE) -B -C $(VENDOR)/uWebSockets \
		CC=$(CC) \
		CXX=$(CXX) \
		CPATH=$(CPATH)
	mkdir -p $*
	cp $(VENDOR)/uWebSockets/libuWS.so $*/

%/libwiringPi.so:
	$(MAKE) -B -C $(VENDOR)/wiringPi/wiringPi \
		CC=$(CC) \
		CXX=$(CXX) \
		CPATH=$(CPATH)
	mkdir -p $*
	cp $(VENDOR)/wiringPi/wiringPi/libwiringPi.so.* $@

JS ?= \
	$(REMOTE)/_vendor/h264-live-player/vendor/dist/http-live-player.js \
	$(REMOTE)/_vendor/three.js/build/three.js \
	$(REMOTE)/_vendor/three.js/examples/js/WebGL.js \
	$(REMOTE)/_vendor/stats.js/build/stats.min.js \
	$(REMOTE)/_vendor/dat.gui/build/dat.gui.min.js \
	$(REMOTE)/_vendor/THREE.MeshLine/src/THREE.MeshLine.js \
	$(REMOTE)/_vendor/virtualjoystick.js/virtualjoystick.js \
	$(REMOTE)/_vendor/reconnecting-websocket/reconnecting-websocket.js \
	$(REMOTE)/js/dataview2.js \
	$(REMOTE)/js/protocol.js \
	$(REMOTE)/js/robotprotocol.js \
	$(REMOTE)/js/robotgui.js \
	$(REMOTE)/js/client.js \
	$(REMOTE)/js/robot.js \
	$(REMOTE)/js/renderer3d.js \
	$(REMOTE)/js/main.js


$(OUT)/%/min.js: $(JS)
	@mkdir -p $(dir $@)
	uglifyjs -o $@ -c -m -- $^

$(OUT)/%/index.html: $(REMOTE)/index_min.html
	@mkdir -p $(dir $@)
	cp $^ $@

$(OUT)/%/favicon.png: $(REMOTE)/favicon.png
	@mkdir -p $(dir $@)
	cp $^ $@

$(OUT)/%/style.css: $(REMOTE)/style.css
	@mkdir -p $(dir $@)
	cp $^ $@

-include $(DEPS)

.PHONY: run
run: default
	$(RUNNER) $(OUT)/$(EXECUTABLE) $(ARGS)

.PHONY: debug
debug: RUNNER = gdb
debug: run

.PHONY: memcheck
memcheck: RUNNER = valgrind
memcheck: run

.PHONY: push
push: default
	cd $(OUT) && rsync -avR `find -type f -not -name "*.[do]"` kinetikos@kinetikos.local:

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: clean_vendor
clean_vendor:
	$(RM) -r $(VENDOR)
	$(RM) -r $(REMOTE_VENDOR)

.PHONY: vendor
vendor: \
	$(VENDOR)/uWebSockets \
	$(VENDOR)/wiringPi \
	$(VENDOR)/rapidjson \
	$(REMOTE_VENDOR)/three.js \
	$(REMOTE_VENDOR)/THREE.MeshLine \
	$(REMOTE_VENDOR)/virtualjoystick.js \
	$(REMOTE_VENDOR)/h264-live-player \
	$(REMOTE_VENDOR)/dat.gui \
	$(REMOTE_VENDOR)/stats.js \
	$(REMOTE_VENDOR)/reconnecting-websocket \

$(VENDOR)/uWebSockets:
	git clone https://github.com/uNetworking/uWebSockets.git $@
	cd $@ && git checkout origin/v0.14
	mkdir -p $(INCLUDE)/uWS
	cp -l $@/src/*.h $(INCLUDE)/uWS

$(VENDOR)/wiringPi:
	git clone --depth=1 git://git.drogon.net/wiringPi $@
	mkdir -p $(INCLUDE)/wiringPi
	cp -l $@/wiringPi/*.h $(VENDOR)/include/wiringPi

$(VENDOR)/flatbuffers:
	git clone --depth=1 https://github.com/google/flatbuffers.git $@
	cd $@ && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release && make
	cp -l -r $@/include/flatbuffers $(VENDOR)/include/
	#cp -l -r $@/js $(VENDOR)/js

$(VENDOR)/rapidjson:
	git clone --depth=1 https://github.com/Tencent/rapidjson.git $@
	cp -l -r $@/include/ $(VENDOR)/

$(REMOTE_VENDOR)/three.js:
	git clone --depth=1 https://github.com/mrdoob/three.js.git $@

$(REMOTE_VENDOR)/THREE.MeshLine:
	git clone --depth=1 https://github.com/spite/THREE.MeshLine.git $@

$(REMOTE_VENDOR)/virtualjoystick.js:
	git clone --depth=1 https://github.com/jeromeetienne/virtualjoystick.js.git $@

$(REMOTE_VENDOR)/h264-live-player:
	git clone --depth=1 https://github.com/131/h264-live-player.git $@

$(REMOTE_VENDOR)/dat.gui:
	git clone --depth=1 https://github.com/dataarts/dat.gui.git $@

$(REMOTE_VENDOR)/stats.js:
	git clone --depth=1 https://github.com/mrdoob/stats.js.git $@

$(REMOTE_VENDOR)/reconnecting-websocket:
	git clone --depth=1 https://github.com/joewalnes/reconnecting-websocket.git $@

