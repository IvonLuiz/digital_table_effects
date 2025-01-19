#define _USE_MATH_DEFINES

#include "effects/pitchShifter.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Função para inicializar a janela de Hann
void initializeWindow(int fftSize, std::vector<Sample>& window) {
    window.resize(fftSize);
    for (int i = 0; i < fftSize; ++i) {
        double r = static_cast<double>(i) / fftSize;
        window[i] = 0.5f * (1.0f - cos(2.0f * M_PI * r)); // Janela de Hann
    }
}

// Função para aplicar a janela
void applyWindow(std::vector<Sample>& buffer, const std::vector<Sample>& window) {
    std::transform(buffer.begin(), buffer.end(), window.begin(), buffer.begin(), std::multiplies<Sample>());
}

// Função para implementar a linha de atraso variável com interpolação linear
void variableDelayLine(const std::vector<Sample>& input, std::vector<Sample>& output, const std::vector<Sample>& delay, int sampleRate) {
    int inputSize = input.size();
    output.resize(inputSize);
    
    for (int i = 0; i < inputSize; ++i) {
        int delaySamples = static_cast<int>(delay[i] * sampleRate);

        if (i - delaySamples >= 0 && i - delaySamples < inputSize) {
            output[i] = input[i - delaySamples];
        } else {
            output[i] = 0.0f;
        }
    }
}

// Função para gerar a onda de serra
void sawtoothWave(std::vector<Sample>& wave, float frequency, int sampleRate) {
    int waveSize = wave.size();
    float increment = frequency / sampleRate;

    for (int i = 0; i < waveSize; ++i) {
        wave[i] = fmod(i * increment, 1.0f);
    }
}

// Função para gerar a onda de serra com deslocamento de fase
void sawtoothWaveWithPhase(std::vector<Sample>& wave, float frequency, int sampleRate, float phaseShift) {
    int waveSize = wave.size();
    float increment = frequency / sampleRate;

    for (int i = 0; i < waveSize; ++i) {
        wave[i] = fmod(i * increment + phaseShift, 1.0f);
    }
}

void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int sampleRate, float pitchShift) {
    int inputSize = input.size();
    output.resize(inputSize);

    // Define o tamanho da janela (em samples)
    int windowSize = sampleRate / 30; // Aproximadamente 30 ms
    if (windowSize > inputSize) {
        windowSize = inputSize;
    }

    // Inicializa a janela de Hann
    std::vector<Sample> window;
    initializeWindow(windowSize, window);
    std::cout << "window size: " << window.size() << "\n";
    // Gera a onda de serra para controlar o tempo de atraso
    std::vector<Sample> sawtooth1(inputSize);
    std::vector<Sample> sawtooth2(inputSize);
    float frequency = ((pitchShift - 1.0f) * sampleRate) / windowSize;
    sawtoothWaveWithPhase(sawtooth1, frequency, sampleRate, 0.0f);
    sawtoothWaveWithPhase(sawtooth2, frequency, sampleRate, 0.5f); // Deslocamento de fase de 180 graus
    std::cout << "sawtooth1 size: " << sawtooth1.size() << "\n";
    // Aplica a linha de atraso variável
    std::vector<Sample> delayed1(inputSize);
    std::vector<Sample> delayed2(inputSize);
    variableDelayLine(input, delayed1, sawtooth1, sampleRate);
    variableDelayLine(input, delayed2, sawtooth2, sampleRate);
    std::cout << "delayed1 size: " << delayed1.size() << "\n";
    // Aplica a envolvente para suavizar as descontinuidades
    // applyWindow(delayed1, window);
    // applyWindow(delayed2, window);
    // std::cout << "delayed1 size: " << delayed1.size() << "\n";

    for (int i = 0; i < inputSize; i += windowSize) {
        int blockSize = std::min(windowSize, inputSize - i);
        std::cout << "blockSize: " << blockSize << "\n";
        std::vector<Sample> block1(delayed1.begin() + i, delayed1.begin() + i + blockSize);
        std::vector<Sample> block2(delayed2.begin() + i, delayed2.begin() + i + blockSize);
        applyWindow(block1, window);
        applyWindow(block2, window);
        std::copy(block1.begin(), block1.end(), delayed1.begin() + i);
        std::copy(block2.begin(), block2.end(), delayed2.begin() + i);
    }

    // Combina as duas linhas de delay usando média ponderada
    for (int i = 0; i < inputSize; ++i) {
        float t = static_cast<float>(i % windowSize) / windowSize;
        output[i] = delayed1[i] * (1.0f - t) + delayed2[i] * t;
    }
}