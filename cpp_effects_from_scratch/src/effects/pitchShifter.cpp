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

