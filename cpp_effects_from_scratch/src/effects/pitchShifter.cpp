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
    int numFrames = input.size() / hopSize;
    output.resize(input.size());
    std::cout << "output: " << output.size() << "\n";

    std::vector<Sample> window;
    initializeHannWindow(fftSize, window);
    std::cout << "window: " << window.size() << "\n";

    std::vector<Complex> prevPhases(fftSize / 2 + 1, Complex(0, 0));
    std::vector<Complex> shiftedFftBuffer(fftSize / 2 + 1); // Altere para Complex
    int outputBufferIndex = 0;

    for (int frame = 0; frame < numFrames; ++frame) {
        // Extrai uma janela de entrada
        std::vector<Sample> inputFrame(input.begin() + frame * hopSize, input.begin() + frame * hopSize + fftSize);

        // Aplica a janela de Hann
        applyWindow(inputFrame, window);

        // Calcula a FFT da janela de entrada
        std::vector<Complex> fftBuffer;
        fft(inputFrame, fftBuffer);

        for (size_t k = 0; k < fftSize / 2 + 1; ++k) {
            float magnitude = std::abs(fftBuffer[k]); // Calcula a magnitude
            float phase = std::arg(fftBuffer[k]);     // Extrai a fase atual

            // Calcula o delta de fase
            float deltaPhase = phase - std::arg(prevPhases[k]);
            prevPhases[k] = fftBuffer[k]; // Atualiza a fase anterior

            // Normaliza o delta de fase no intervalo -pi a pi
            deltaPhase -= 2 * M_PI * hopSize * k / fftSize;
            while (deltaPhase > M_PI) deltaPhase -= 2 * M_PI;
            while (deltaPhase < -M_PI) deltaPhase += 2 * M_PI;

            // Ajusta a frequência para o pitch shift
            float trueFreq = 2 * M_PI * k / fftSize + deltaPhase / hopSize;

            // Aplica o pitch shifting
            phase += trueFreq * hopSize * pitchShift;

            // Reconstroi o buffer com a nova fase e magnitude
            shiftedFftBuffer[k] = Complex(magnitude * std::cos(phase), magnitude * std::sin(phase));
        }

        // Calcula a IFFT da janela modificada
        ifft(shiftedFftBuffer, inputFrame); // Agora shiftedFftBuffer é do tipo correto

        // Recombina as janelas usando overlap-add
        overlapAdd(inputFrame, output, hopSize, outputBufferIndex);
    }
}

