#include <fstream>
#include <vector>
#include <iostream>
#include "effects/reverb.h"
#include "audio/handleAudio.h"

int main() {
    const char* filename = "src/original.wav";
    const char* outputFilename = "src/audio_reverb.wav";
    float wetLevel = 0.5f; // 50% wet signal

    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Reads WAV header
    WavHeader header;
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE") {
        std::cerr << "Error: Not a valid WAV file." << std::endl;
        return 1;
    }

    std::cout << "Sample Rate: " << header.sampleRate << " Hz" << std::endl;
    std::cout << "Channels: " << header.numChannels << std::endl;
    std::cout << "Bits per Sample: " << header.bitsPerSample << std::endl;
    std::cout << "Data Size: " << header.dataSize << " bytes" << std::endl;

    // Reads audio dada to vector of bytes
    std::vector<uint8_t> audioData(header.dataSize);
    inputFile.read(reinterpret_cast<char*>(audioData.data()), header.dataSize);

    if (!inputFile) {
        std::cerr << "Error: Could not read audio data." << std::endl;
        return 1;
    }

    inputFile.close();

    std::vector<float> floatSamples = convertBytesToSamples16(audioData);

    // Applying Schroeder reverberation algorithm
    std::vector<float> reverbSamples = schroederReverb(floatSamples, header.sampleRate);
    std::vector<float> outputSamplesFloat = mixDryWet(floatSamples, reverbSamples, wetLevel);

    std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(outputSamplesFloat);

    saveWavFile(outputFilename, header, outputAudioBytes);

    return 0;
}
