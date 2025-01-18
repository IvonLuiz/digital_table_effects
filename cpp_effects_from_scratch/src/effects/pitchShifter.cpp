#define _USE_MATH_DEFINES

#include "effects/pitchShifter.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Função para inicializar a janela de Hann
void initializeWindow(int fftSize, std::vector<Sample>& window) {
    window.resize(fftSize);
    for (int i = 0; i < fftSize; ++i) {
        window[i] = 0.5f - 0.5f * std::cos(2.0f * M_PI * i / fftSize);
    }
}

// Função para aplicar a janela de Hann
void applyWindow(std::vector<Sample>& buffer, const std::vector<Sample>& window) {
    std::transform(buffer.begin(), buffer.end(), window.begin(), buffer.begin(), std::multiplies<Sample>());
}

// Função para implementar a linha de atraso variável
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

// Função para aplicar a envolvente
void envelope(std::vector<Sample>& buffer, const std::vector<Sample>& env) {
    std::transform(buffer.begin(), buffer.end(), env.begin(), buffer.begin(), std::multiplies<Sample>());
}

// Função principal para aplicar o pitch shifting
void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int sampleRate, float pitchShift) {
    int inputSize = input.size();
    output.resize(inputSize);

    // Inicializa a janela de Hann
    std::vector<Sample> window;
    initializeWindow(inputSize, window);

    // Gera a onda de serra para controlar o tempo de atraso
    std::vector<Sample> sawtooth(inputSize);
    float frequency = (pitchShift - 1.0f) * sampleRate / inputSize;
    sawtoothWave(sawtooth, frequency, sampleRate);

    // Aplica a linha de atraso variável
    std::vector<Sample> delayed(inputSize);
    variableDelayLine(input, delayed, sawtooth, sampleRate);

    // Aplica a envolvente para suavizar as descontinuidades
    envelope(delayed, window);

    // Copia o resultado para a saída
    std::copy(delayed.begin(), delayed.end(), output.begin());
}