#include <iostream>
#include "reverb.h"

// std::vector<float> (std::vector<float> samples, int samplesLength, float delayMs, float decayGain, float sampleRate)
std::vector<int16_t> combFilter(std::vector<int16_t> inputSamples, double delayMs, double decayGain, float sampleRate){
    int delaySamples = (int) delayMs * sampleRate / 1000; 
    std::cout<<"delaysamples";
    std::cout << delaySamples << std::endl;

    // Garantindo que o atraso não é maior que o tamanho do vetor
    if (delaySamples >= inputSamples.size()) {
        std::cerr << "ERROR: Delay is larger than the number of samples.\n";
        return inputSamples; // Retorna o vetor original para evitar problemas
    }

    // Apply comb filter
    std::vector<int16_t> combFilterSamples(inputSamples.size());
    std::copy(std::begin(inputSamples), std::end(inputSamples), std::begin(combFilterSamples));

    for (size_t i = 0; i < inputSamples.size() - delaySamples; ++i) {
        combFilterSamples[i + delaySamples] += combFilterSamples[i] * decayGain;
        std::cout << i << std::endl;
        std::cout << combFilterSamples[i] << std::endl;
        std::cout << inputSamples[i] << std::endl;
    }

    return combFilterSamples;
}


