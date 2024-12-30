# Digital Table Effects from Scratch in C++

This project implements various digital audio effects from scratch using the C++ language. The primary motivation for using C++ is the ease of translating C++ code to C, compared to other languages like Python.

## Overview

In this project, we have implemented several audio effects, including a Schroeder reverberator, using fundamental digital signal processing techniques. The code is designed to be easily translatable to C, making it suitable for embedded systems and other performance-critical applications.

## How to Build and Run

To test the effects, follow these steps:

1. Change directory to `./cpp_effects_from_scratch`:
    ```sh
    cd cpp_effects_from_scratch/
    ```

2. Generate the build system files using CMake:
    ```sh
    cmake -S . -B build
    ```

3. Build the project:
    ```sh
    cmake --build build
    ```

4. Run the executable:
    ```sh
    ./build/Debug/cppEffects.exe
    ```

## Dependencies

- CMake: A cross-platform build system generator.
- A C++ compiler that supports C++11 or later.

## References

Here are some references used for understanding theory and implementation:

- **Schroeder Reverberator**: 
  - [Natural Sounding Artificial Reverberation](https://www.semanticscholar.org/paper/Nat-u-ral-Sound-i-ng-Artificial-Reverberation-Schroede/48a532a015e3cbb863e28b6cca8ad4ca849065b7?p2df) by Manfred R. Schroeder. This paper discusses the theory behind artificial reverberation and introduces the Schroeder reverberator algorithm.

- **Medium Articles on Reverb Algorithm**:
  - [Coding a Basic Reverb Algorithm: An Introduction to Audio Programming (Part 1)](https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-an-introduction-to-audio-programming-d5d90ad58bde) by The Seekers Project. This article provides a step-by-step guide to coding a basic reverb algorithm.
  - [Coding a Basic Reverb Algorithm (Part 2)](https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325) by The Seekers Project. This continuation delves deeper into the implementation details and optimizations.

- **Lecture on Writing a Reverb**:
  - [Writing a Reverb](https://www.youtube.com/watch?v=6ZK2Goiyotk&ab_channel=ADC-AudioDeveloperConference) by Geraint Luff. This lecture from the Audio Developer Conference provides insights into the practical aspects of writing a reverb algorithm.
