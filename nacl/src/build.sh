#!/bin/bash

echo "These are only notes to be read, not executed"
exit 1

# For me:
PPAPI=/home/mag/Downloads/nacl_sdk/pepper_32
TOOLCHAIN="$PPAPI/toolchain/linux_pnacl/bin64"
export CC="$TOOLCHAIN/pnacl-clang"
export CXX="$TOOLCHAIN/pnacl-clang++"
export AR="$TOOLCHAIN/pnacl-ar"
export RANLIB="$TOOLCHAIN/pnacl-ranlib"
./waf configure --ppapi=$PPAPI -o build-pnacl
./waf build_debug --targets=platform-pnacl_unstripped.bc
"$TOOLCHAIN/pnacl-finalize" \
	-o build-pnacl/debug/src/platform-pnacl/platform-pnacl.pexe \
	build-pnacl/debug/src/platform-pnacl/platform-pnacl_unstripped.bc
./nacl/index.js


# Successful build for those guys:
CC=/home/mag/Downloads/nacl_sdk/pepper_32/toolchain/linux_pnacl/bin/pnacl-clang
CXX=/home/mag/Downloads/nacl_sdk/pepper_32/toolchain/linux_pnacl/bin/pnacl-clang++
FINALIZE=/home/mag/Downloads/nacl_sdk/pepper_32/toolchain/linux_pnacl/bin/pnacl-finalize

mkdir -p pnacl/Release/

$CC \
	-o pnacl/Release/hello_world_pnacl.o \
	-c hello_world.c \
	-g -O2 -pthread -MMD -DNDEBUG -Wall \
	-I/home/mag/Downloads/nacl_sdk/pepper_32/include \
	-I/home/mag/Downloads/nacl_sdk/pepper_32/include/pnacl \
	-Wno-long-long -Werror

$CXX \
	-o pnacl/Release/using_ppapi_simple_unstripped.bc \
	./pnacl/Release/hello_world_pnacl.o \
	-O2 \
	-L/home/mag/Downloads/nacl_sdk/pepper_32/lib/pnacl/Release \
	-lppapi_simple -lnacl_io -lppapi_cpp -lppapi 

$FINALIZE \
	-o pnacl/Release/using_ppapi_simple_unstripped.pexe \
	pnacl/Release/using_ppapi_simple_unstripped.bc

cp pnacl/Release/using_ppapi_simple_unstripped.pexe pnacl/Release/using_ppapi_simple.pexe
