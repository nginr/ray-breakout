#!/usr/bin/bash

set -xe 

RAYLIB_SRC="./thirdparty/raylib-5.0_linux/include"
RAYLIB_LIB="./thirdparty/raylib-5.0_linux/lib"

mkdir -p out

BIN_PATH="out/breakout"

gcc -Wall -Wextra -Werror -O2 -std=c99 -pedantic -I${RAYLIB_SRC} src/main.c -o $BIN_PATH -lm -L${RAYLIB_LIB} -l:libraylib.a
