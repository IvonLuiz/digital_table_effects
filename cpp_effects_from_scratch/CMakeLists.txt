cmake_minimum_required(VERSION 3.31.0)
project(digitalEffects)
# set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib/cmake/opencv4")

# Packages

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Subdirectories for each algorithm 

# target_link_libraries(SobelModule ${OpenCV_LIBS})

add_executable(reverb main.cpp reverb.cpp wavHeader.cpp)
