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

// Função para recombinar as janelas usando overlap-add
void overlapAdd(const std::vector<Sample>& buffer, std::vector<Sample>& output, int hopSize, int& outputBufferIndex) {
    std::transform(buffer.begin(), buffer.end(), output.begin() + outputBufferIndex, output.begin() + outputBufferIndex, std::plus<Sample>());
    outputBufferIndex += hopSize;
    if (outputBufferIndex >= buffer.size()) {
        outputBufferIndex = 0;
    }
}

// Função principal para aplicar o pitch shifting
void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int fftSize, int hopSize, float sampleRate, float pitchShift) {
    int inputSize = input.size();
    output.resize(inputSize, 0.0f);

    // Inicialize as janelas de entrada e saída
    std::vector<Sample> window;
    initializeHannWindow(fftSize, window);

    std::vector<Sample> buffer(fftSize, 0.0f);
    std::vector<Complex> fftBuffer(fftSize);
    std::vector<Complex> shiftedBuffer(fftSize);

    int outputBufferIndex = 0;

    // Processamento por janelas
    for (int i = 0; i + fftSize <= inputSize; i += hopSize) {
        // Copiar uma janela de dados de entrada
        std::copy(input.begin() + i, input.begin() + i + fftSize, buffer.begin());

        // Aplicar janela de Hann
        applyWindow(buffer, window);

        // FFT
        fft(buffer, fftBuffer);

        // Shift de pitch no domínio da frequência
        for (int k = 0; k < fftSize; ++k) {
            int shiftedIndex = static_cast<int>(k * pitchShift);

            if (shiftedIndex < fftSize) {
                shiftedBuffer[shiftedIndex] = fftBuffer[k];
            }
        }

        // IFFT
        ifft(shiftedBuffer, buffer);

        // Recombinar usando overlap-add
        overlapAdd(buffer, output, hopSize, outputBufferIndex);
    }
}
