#ifndef PITCHSHIFTER_H
#define PITCHSHIFTER_H

#include <vector>
#include <complex>

using Sample = float;
using Complex = std::complex<Sample>;

// Função para inicializar a janela de Hann
void initializeWindow(int fftSize, std::vector<Sample>& window);

// Função para aplicar a janela de Hann
void applyWindow(std::vector<Sample>& buffer, const std::vector<Sample>& window);

// Função para implementar a linha de atraso variável
void variableDelayLine(const std::vector<Sample>& input, std::vector<Sample>& output, const std::vector<Sample>& delay, int sampleRate);

// Função para gerar a onda de serra
void sawtoothWave(std::vector<Sample>& wave, float frequency, int sampleRate);

// Função para aplicar a envolvente
void envelope(std::vector<Sample>& buffer, const std::vector<Sample>& env);

// Função principal para aplicar o pitch shifting
void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int sampleRate, float pitchShift);

#endif // PITCHSHIFTER_H