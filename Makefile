EXECUTABLE ?= kinetikos

BUILD_DIR ?= ./_build

SOURCE ?= source
VENDOR ?= ./vendor
REMOTE ?= remote/html
INCLUDE := $(VENDOR)/include

TOOLCHAIN_DIR ?= $(shell pwd)/_toolchain

TOOLCHAIN ?= $(shell $(CC) -dumpmachine)
TOOLCHAIN ?= $(shell $(CXX) -dumpmachine)

ifdef TARGET
    CC = $(TOOLCHAIN_DIR)/$(TARGET)/cc
    CXX = $(TOOLCHAIN_DIR)/$(TARGET)/cxx
    CPATH = $(shell cat $(TOOLCHAIN_DIR)/$(TARGET)/cpath)
endif

RELEASE ?= debug

OUT := $(BUILD_DIR)/$(TOOLCHAIN)/$(RELEASE)

RUNNER :=

ARGS :=

SRCS := $(shell find $(SOURCE) -name "*.cpp" -or -name "*.c")
OBJS = $(SRCS:%=$(OUT)/%.o)
DEPS = $(OBJS:.o=.d)

CONFIG_OUTS := $(shell find config/ -type f)
CONFIG_OUTS := $(addprefix $(OUT)/,$(CONFIG_OUTS))

REMOTE_OUTS := \
	index.html \
	min.js \
	style.css \
	favicon.png
REMOTE_OUTS := $(addprefix $(OUT)/$(REMOTE)/,$(REMOTE_OUTS))

INC_DIRS := $(shell find $(SOURCE) -type d) $(VENDOR)/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LIB_NAMES := libuWS.so
BIN ?= $(OUT)/bin
LIBRARIES = $(LIB_NAMES:%=$(OUT)/%)
LIBS ?= -L$(BIN) -luWS -lm -lz -lssl -lcrypto -lpthread -lrt -lcrypt
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

default: $(OUT)/$(EXECUTABLE) $(CONFIG_OUTS) $(REMOTE_OUTS)

$(OUT)/$(EXECUTABLE): $(OBJS) $(BIN)/libuWS.so
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

$(OUT)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBS) -c $< -o $@

$(OUT)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(OUT)/config/%: config/%
	@mkdir -p $(dir $@)
	cp $< $@

%/libuWS.so:
	$(MAKE) -B -C $(VENDOR)/uWebSockets \
		CC=$(CC) \
		CXX=$(CXX) \
		CPATH=$(CPATH)
	mkdir -p $*
	mv $(VENDOR)/uWebSockets/libuWS.so $*/

JS ?= \
	$(VENDOR)/h264-live-player/vendor/dist/http-live-player.js \
	$(VENDOR)/three.js/build/three.js \
	$(VENDOR)/three.js/examples/js/WebGL.js \
	$(VENDOR)/stats.js/build/stats.min.js \
	$(VENDOR)/dat.gui/build/dat.gui.min.js \
	$(VENDOR)/THREE.MeshLine/src/THREE.MeshLine.js \
	$(VENDOR)/virtualjoystick.js/virtualjoystick.js \
	$(VENDOR)/reconnecting-websocket/reconnecting-websocket.js \
	js/dataview2.js \
	js/protocol.js \
	js/robotprotocol.js \
	js/robotgui.js \
	js/client.js \
	js/robot.js \
	js/renderer3d.js \
	js/main.js
JS := $(addprefix $(REMOTE)/,$(JS))

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
	cd $(OUT) && rsync --ignore-times -avR `find -type f -not -name "*.[do]"` $(HOST):$(DEST)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

