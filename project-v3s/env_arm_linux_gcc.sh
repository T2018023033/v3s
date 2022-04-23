#! /bin/bash

export PATH=/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/:$PATH
export CC="arm-linux-gnueabihf-gcc  "
export CXX="arm-linux-gnueabihf-g++"
export CPP="arm-linux-gnueabihf-gcc -E "
export AS="arm-linux-gnueabihf-as"
export LD="arm-linux-gnueabihf-ld"
export GDB=arm-linux-gnueabihf-gdb
export STRIP=arm-linux-gnueabihf-strip
export RANLIB=arm-linux-gnueabihf-ranlib
export OBJCOPY=arm-linux-gnueabihf-objcopy
export OBJDUMP=arm-linux-gnueabihf-objdump
export AR=arm-linux-gnueabihf-ar
export NM=arm-linux-gnueabihf-nm
export M4=m4
export TARGET_PREFIX=arm-linux-gnueabihf-
export CFLAGS=" -O2 -pipe -g"
export CXXFLAGS=" -O2 -pipe -g"
export LDFLAGS=""
export CPPFLAGS=""
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

