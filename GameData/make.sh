#!/bin/sh

# Optional helper build script for Anarch.
# by drummyfish, released under CC0 1.0, public domain
#
# usage:
#
# ./make.sh platform [compiler]

if [ $# -lt 1 ]; then
  echo "need a parameter (sdl, pokitto, gb, emscripten, ...)"
  exit 0
fi

clear; clear; 

C_FLAGS="-std=c99 -Wall -Wextra -pedantic -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -o anarch"

COMPILER='gcc'

if [ $# -eq 2 ]; then
  COMPILER=$2

  if [ $2 = "tcc" ]; then # you'll probably want to modify this
    C_FLAGS="${C_FLAGS} -L/usr/lib/x86_64-linux-gnu/pulseaudio/ 
      -I/home/tastyfish/git/tcc/tcc-0.9.27/include
      -I/usr/lib/gcc/x86_64-linux-gnu/8/include/"
  fi
fi

echo "compiling"

if [ $1 = "sdl" ]; then
echo "SDL HIT"
  # PC SDL build, requires:
  # - g++
  # - SDL2 (dev) package

  SDL_FLAGS=`sdl2-config --cflags --libs --static-libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_sdl.c -I/usr/local/include ${SDL_FLAGS}"

  echo ${COMMAND}

  ${COMMAND}
elif [ $1 = "saf" ]; then
  # SAF build using SDL, requires:
  # - saf.h
  # - SDL2 (dev) package

  SDL_FLAGS=`sdl2-config --cflags --libs --static-libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_saf.c -I/usr/local/include ${SDL_FLAGS}"

  echo ${COMMAND}

  ${COMMAND}
elif [ $1 = "terminal" ]; then
  # PC terminal build, requires:
  # - g++

  COMMAND="${COMPILER} ${C_FLAGS} main_terminal.c"

  echo ${COMMAND}

  ${COMMAND}
elif [ $1 = "csfml" ]; then
  # csfml build, requires:
  # - csfml

  COMMAND="${COMPILER} ${C_FLAGS} main_csfml.c -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio"

  echo ${COMMAND}

  ${COMMAND}
elif [ $1 = "test" ]; then
  # test build, requires:
  # - g++

  COMMAND="${COMPILER} ${C_FLAGS} main_test.c"

  echo ${COMMAND}

  ${COMMAND}
elif [ $1 = "pokitto" ]; then
  # Pokitto build, requires:
  # - PokittoLib, in this folder create a symlink named "PokittoLib" to the 
  #   "Pokitto" subfolder of PokittoLib
  # - Pokitto Makefile
  # - GNU embedded toolchain, in this folder create a symlink named "gtc" to the
  #   "bin" subfolder
  # - files like My_settings.h required by Pokitto

  make
  ./PokittoEmu BUILD/firmware.bin 
elif [ $1 = "emscripten" ]; then
  # emscripten (browser Javascript) build, requires:
  # - emscripten

  ../emsdk/upstream/emscripten/emcc ./main_sdl.c -s USE_SDL=2 -O3 -lopenal --shell-file HTMLshell.html -o anarch.html -s EXPORTED_FUNCTIONS='["_main","_webButton"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'
else
  echo "unknown parameter: $1"
fi
