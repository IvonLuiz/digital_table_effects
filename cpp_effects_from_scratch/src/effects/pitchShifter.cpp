#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include "effects/pitchShifter.h"
#include <iostream>

void initializeHannWindow(int fftSize, std::vector<Sample>& window) {
    window.resize(fftSize);

    for (int i = 0; i < fftSize; ++i) {
        window[i] = 0.5f - 0.5f * std::cos(2.0f * M_PI * i / fftSize);
    }
}

void applyWindow(std::vector<Sample>& buffer, const std::vector<Sample>& window) {
    std::transform(buffer.begin(), buffer.end(), window.begin(), buffer.begin(), std::multiplies<Sample>());
}


void fft(const std::vector<Sample>& input, std::vector<Complex>& output) {
    int n = input.size();
    output.resize(n);

    for (int k = 0; k < n; ++k) {
        output[k] = {0, 0};
        for (int t = 0; t < n; ++t) {
            double angle = 2 * M_PI * t * k / n;
            output[k] += input[t] * Complex(std::cos(angle), -std::sin(angle));
        }
    }
}


void ifft(const std::vector<Complex>& input, std::vector<Sample>& output) {
    int n = input.size();
    output.resize(n);
    for (int t = 0; t < n; ++t) {
        Complex sum = {0, 0};
        for (int k = 0; k < n; ++k) {
            double angle = 2 * M_PI * t * k / n;
            sum += input[k] * Complex(std::cos(angle), std::sin(angle));
        }
        output[t] = sum.real() / n;
    }
}

