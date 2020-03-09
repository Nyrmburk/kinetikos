Link (or place) your desired toolchain in this directory.

Each folder in `_toolchain` is selected with the `TARGET` environment variable.
In each target, links to `cc` and `cxx` are needed, along with a text file `cpath`

```
_toolchain/target
├── cc -> link/to/gcc # or another c compiler
├── cpath # cpath contents
└── cxx -> link/to/g++ # or another c++ compiler
```

To install the Raspberry Pi toolchain:
```
$ mkdir rpi # call this in the makefile with "make TARGET=rpi"
$ cd rpi
$ git clone https://github.com/raspberrypi/tools.git # download rpi toolchains
$ ln -s tools/arm-bcm2708/arm-linux-gnueabihf/ toolchain # selecting which rpi toolchain
$ ln -s toolchain/bin/arm-linux-gnueabihf-gcc cc
$ ln -s toolchain/bin/arm-linux-gnueabihf-g++ cxx
```

If there are compilation errors for missing libraries, copy them off an existing device.

