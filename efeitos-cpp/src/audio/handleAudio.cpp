#include "audio/handleAudio.h"
#include <iostream>

// Convertes array of bytes into array of samples (16-bit PCM signed)
std::vector<float> convertBytesToSamples16(const std::vector<uint8_t>& audioData) {
    std::vector<float> samples;
    for (size_t i = 0; i < audioData.size(); i += 2) {
        int16_t sample = audioData[i] | (audioData[i + 1] << 8);
        samples.push_back(static_cast<float>(sample) / 32768.0f); // Normalize to interval [-1, 1]
    }
    return samples;
}

// Converts float samples back to bytes (Little Endian)
std::vector<uint8_t> convertSamplesToBytes16(const std::vector<float>& samples) {
    std::vector<uint8_t> audioBytes;
    audioBytes.reserve(samples.size() * 2);

    for (float sample : samples) {
        int16_t intSample = static_cast<int16_t>(std::round(sample * 32768.0f)); // Denormalize to interval [-32768, 32767]
        audioBytes.push_back(intSample & 0xFF);        // First byte (least significant)
        audioBytes.push_back((intSample >> 8) & 0xFF); // Second byte (most significant)
    }
    return audioBytes;
}

void saveWavFile(const char* filename, const WavHeader& header, const std::vector<uint8_t>& audioBytes) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // Adjusts size from header
    WavHeader newHeader = header;
    newHeader.dataSize = audioBytes.size();
    newHeader.chunkSize = 36 + newHeader.dataSize;

    outFile.write(reinterpret_cast<const char*>(&newHeader), sizeof(WavHeader));
    outFile.write(reinterpret_cast<const char*>(audioBytes.data()), audioBytes.size());
    outFile.close();
    std::cout << "File saved successfully: " << filename << std::endl;
}

