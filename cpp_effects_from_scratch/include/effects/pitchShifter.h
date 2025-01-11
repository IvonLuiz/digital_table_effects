#ifndef PITCHSHIFTER_H
#define PITCHSHIFTER_H

#include <vector>
#include <complex>

using Sample = float;
using Complex = std::complex<Sample>;

int fftSize;
int hopSize;
float sampleRate;
std::vector<float> windows;
std::vector<Complex> prevPhases;
std::vector<Sample> outputBuffers;

void processPitchShift(const std::vector<Sample>& input, std::vector<Sample>& output, int fftSize, int hopSize, float sampleRate);

void applyWindow(std::vector<Sample>& buffer);
void fft(const std::vector<Sample>& input, std::vector<Complex>& output);
void ifft(const std::vector<Complex>& input, std::vector<Sample>& output);
void overlapAdd(const std::vector<Sample>& buffer, std::vector<Sample>& output);

#endif // PITCHSHIFTER_H