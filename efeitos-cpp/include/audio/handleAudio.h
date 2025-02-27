#ifndef HANDLEAUDIO_H
#define HANDLEAUDIO_H

#include "audio/wavHeader.h"
#include <fstream>
#include <vector>
#include <cstdint>

std::vector<float> convertBytesToSamples16(const std::vector<uint8_t>& audioData);
std::vector<uint8_t> convertSamplesToBytes16(const std::vector<float>& samples);
void saveWavFile(const char* filename, const WavHeader& header, const std::vector<uint8_t>& audioBytes);

#endif // HANDLEAUDIO_H