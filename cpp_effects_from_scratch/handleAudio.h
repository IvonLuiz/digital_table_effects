#include "wavHeader.cpp"

std::vector<float> convertBytesToSamples16(const std::vector<uint8_t>& audioData);
std::vector<uint8_t> convertSamplesToBytes16(const std::vector<float>& samples);
void saveWavFile(const char* filename, const WavHeader& header, const std::vector<uint8_t>& audioBytes);
