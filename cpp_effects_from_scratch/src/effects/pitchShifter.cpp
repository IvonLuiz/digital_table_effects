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
    int size = wave.size();
    float phaseIncrement = frequency / sampleRate;
    float phase = phaseShift; // Fase inicial

    for (int i = 0; i < size; i++) {
        wave[i] = 2.0f * (phase - std::floor(phase + 0.5f)); // Fórmula da onda de serra
        phase += phaseIncrement;

        if (phase >= 1.0f) {
            phase -= 1.0f; // Mantém a fase no intervalo [0, 1)
        }
    }
}



void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int sampleRate, float pitchShift) {
    int inputSize = input.size();
    output.resize(inputSize);

    // Define o tamanho da janela (em samples) para suavizar as transições
    int windowSize = sampleRate / 10; // Aproximadamente 10 ms
    if (windowSize > inputSize) {
        windowSize = inputSize;
    }

    // Inicializa a janela de Hann
    std::vector<Sample> window;
    initializeWindow(windowSize, window);

    // Gera a onda de serra para controlar o tempo de atraso
    std::vector<Sample> sawtooth1(inputSize);
    std::vector<Sample> sawtooth2(inputSize);
    float frequency = ((pitchShift - 1.0f) * sampleRate) / inputSize; // Corrigido para usar inputSize
    sawtoothWaveWithPhase(sawtooth1, frequency, sampleRate, 0.0f);
    sawtoothWaveWithPhase(sawtooth2, frequency, sampleRate, 0.1f); // Deslocamento de fase de 180 graus

    // Aplica a linha de atraso variável
    std::vector<Sample> delayed1(inputSize);
    std::vector<Sample> delayed2(inputSize);
    variableDelayLine(input, delayed1, sawtooth1, sampleRate);
    variableDelayLine(input, delayed2, sawtooth2, sampleRate);

    // Gera a envolvente de meia onda senoidal
    std::vector<Sample> envelope;
    initializeWindow(windowSize, envelope);

    // Aplica a envolvente para suavizar as descontinuidades e realiza o overlap-add
    std::vector<Sample> tempOutput(inputSize, 0.0f);
    for (int i = 0; i < inputSize; i += windowSize / 2) { // Overlap de 50%
        int blockSize = std::min(windowSize, inputSize - i);
        
        std::vector<Sample> block1(delayed1.begin() + i, delayed1.begin() + i + blockSize);
        std::vector<Sample> block2(delayed2.begin() + i, delayed2.begin() + i + blockSize);
        applyWindow(block1, envelope);
        applyWindow(block2, envelope);

        for (int j = 0; j < blockSize; ++j) {
            tempOutput[i + j] += (1.0f - static_cast<float>(j) / blockSize) * block1[j] + (static_cast<float>(j) / blockSize) * block2[j];
        }
    }

    // Copia o resultado para a saída
    std::copy(tempOutput.begin(), tempOutput.end(), output.begin());
}