cmake_minimum_required(VERSION 3.31.0)
project(digitalEffects)

# Directory to search for headers
include_directories(include)

# Adiciona os arquivos fonte
add_executable(cppEffects
    src/main.cpp
    src/audio/handleAudio.cpp
    src/effects/reverb.cpp
    src/effects/flanger.cpp
    src/effects/tremolo.cpp
)