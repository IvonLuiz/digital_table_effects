# Digital Table Effects from Scratch in C++

This project implements various digital audio effects from scratch using C++. The primary motivation for using C++ is its ease of translation to C, compared to other languages like Python.

## Features

- **Reverb**: Schroeder reverberator implementation
- **Pitch Shifter**: Time-domain pitch shifting 
- **Tremolo**: Amplitude modulation effect
- **Flanger**: Classic modulated delay effect

## Overview

This project implements several audio effects, including a Schroeder reverberator, using fundamental digital signal processing techniques. The code is designed to be easily translatable to C, making it suitable for embedded systems and other performance-critical applications.

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


- [The Scientist and Engineer's Guide to Digital Signal Processing](https://www.dspguide.com/pdfbook.htm)

* **Reverb**:
  - [Natural Sounding Artificial Reverberation](https://www.semanticscholar.org/paper/Nat-u-ral-Sound-i-ng-Artificial-Reverberation-Schroede/48a532a015e3cbb863e28b6cca8ad4ca849065b7?p2df) by Manfred R. Schroeder. This paper discusses the theory behind artificial reverberation and introduces the Schroeder reverberator algorithm.

  - [Coding a Basic Reverb Algorithm: An Introduction to Audio Programming (Part 1)](https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-an-introduction-to-audio-programming-d5d90ad58bde) by The Seekers Project. This article provides a step-by-step guide to coding a basic reverb algorithm.
  - [Coding a Basic Reverb Algorithm (Part 2)](https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325) by The Seekers Project. This continuation delves deeper into the implementation details and optimizations.

  - [Writing a Reverb](https://www.youtube.com/watch?v=6ZK2Goiyotk&ab_channel=ADC-AudioDeveloperConference) by Geraint Luff. This lecture from the Audio Developer Conference provides insights into the practical aspects of writing a reverb algorithm.

* **Pitch Shifter**:
  - [Pitch shifting](https://msp.ucsd.edu/techniques/v0.11/book-html/node115.html) by Miller Puckette. Online book with a lot of theory. This page talks about the pitch shifting implementation using variable delay lines using Doppler effect. 
  - [Pitch-shifting algorithm design and applications in music](https://kth.diva-portal.org/smash/get/diva2:1381398/FULLTEXT01.pdf) by Théo Royer

- **Tremolo**:
  - [Tremolo Effect](https://www.youtube.com/watch?v=cj8E1ivCCEo&t=200s&ab_channel=Phil%E2%80%99sLab) by Phil’s Lab. A video about the theory and how to implement a tremolo in multiple ways.

* **Flanger**:
  - [DIY Synth Flange Effect](https://blog.demofox.org/2015/03/16/diy-synth-flange-effect) by DemoFox. A blog post that discusses the implementation of a flanger.