cmake_minimum_required(VERSION 3.31.0)
project(digitalEffects)

add_executable(reverb main.cpp reverb.cpp handleAudio.cpp wavHeader.cpp)
