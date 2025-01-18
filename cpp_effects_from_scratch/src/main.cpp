#include <fstream>
#include <vector>
#include <iostream>
#include "effects/reverb.h"
#include "effects/pitchShifter.h"
#include "audio/handleAudio.h"

int main() {
    const char* filename = "src/original.wav";
    const char* outputFilename = "src/audio_pitch.wav";
    float wetLevel = 0.5f; // 50% wet signal
    float pitchShift = 10.5f; // Shift pitch by amount of times

    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }

    // Reads WAV header
    WavHeader header;
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE") {
        std::cerr << "Error: Not a valid WAV file.\n";
        return 1;
    }

    std::cout << "Sample Rate: " << header.sampleRate << " Hz" << "\n";
    std::cout << "Channels: " << header.numChannels << "\n";
    std::cout << "Bits per Sample: " << header.bitsPerSample << "\n";
    std::cout << "Data Size: " << header.dataSize << " bytes" << "\n";

    // Reads audio dada to vector of bytes
    std::vector<uint8_t> audioData(header.dataSize);
    inputFile.read(reinterpret_cast<char*>(audioData.data()), header.dataSize);

    if (!inputFile) {
        std::cerr << "Error: Could not read audio data. \n";
        return 1;
    }

    inputFile.close();

    std::vector<float> floatSamples = convertBytesToSamples16(audioData);

    // Applying pitch shifter algorithm
    int sampleRate = header.sampleRate;
    std::vector<float> outputSamples;
    shiftPitch(floatSamples, outputSamples, sampleRate, pitchShift);

    std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(outputSamples);

    // std::vector<float> pitchShiftedSamples;
    // int fftSize = 1024;
    // int hopSize = 256;

    // shiftPitch(floatSamples, pitchShiftedSamples, fftSize, hopSize, header.sampleRate, pitchShift);
    // std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(pitchShiftedSamples);

    // Applying Schroeder reverberation algorithm
    // std::vector<float> reverbSamples = schroederReverb(floatSamples, header.sampleRate);
    // std::vector<float> outputSamplesFloat = mixDryWet(floatSamples, reverbSamples, wetLevel);

    // std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(outputSamplesFloat);


    saveWavFile(outputFilename, header, outputAudioBytes);
    std::cout <<"Pitch Shifting Done!\n";
    return 0;
}
