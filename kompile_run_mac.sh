#!/bin/sh

clang++ -std=c++20 -Flibs -march=haswell -flto -funroll-loops -fomit-frame-pointer -DNDEBUG src/run.cpp src/tinyfiledialogs.cpp src/pdfgen.cpp -o run -Flibs -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer -Wl,-rpath,@executable_path/libs -framework OpenGL -lm -DGL_SILENCE_DEPRECATION -w #-Ofast