# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the project name and version
project(MyCProj VERSION 1.0 LANGUAGES C)

# Enable strict compilation warnings (optional)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

file(GLOB EFFECTS_SOURCES src/effects/*.c)

# Add the source files
set(SOURCES
    src/file.c
    src/main.c
    ${EFFECTS_SOURCES}
)

# Include directories for headers
include_directories(include)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Specify the C standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
)

# Link the math library (required for trigonometric functions)
target_link_libraries(${PROJECT_NAME} m)
