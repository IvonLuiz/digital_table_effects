#include <fstream>
#include <vector>
#include <iostream>
#include "effects/reverb.h"
#include "effects/pitchShifter.h"
#include "effects/tremolo.h"
#include "effects/flanger.h"
#include "audio/handleAudio.h"
#include <cmath>
#include <stdexcept>


int main() {
    const char* filename = "src/audioFiles/original.wav";
    const char* outputFilename = "src/audioFiles/audioOut.wav";
    float wetLevel = 0.5f; // 50% wet signal
    float pitchShift = -10.f;

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

    std::vector<float> samples = convertBytesToSamples16(audioData);

    // Applying pitch shifter algorithm
    int sampleRate = header.sampleRate;
    std::vector<float> outputSamples;
    
    // Applying tremolo effect
    // tremolo(samples, header.sampleRate, 500);

    // Apply flanger effect 
    // flanger(samples, header.sampleRate, 5.0f, 2.0f, 0.25f); // Example: 5ms delay, 2ms depth, 0.25Hz rate

    shiftPitch(samples, outputSamples, sampleRate, pitchShift);
    // std::cout <<"Pitch Shifting Done!\n";

    // Applying Schroeder reverberation algorithm
    // std::vector<float> reverbSamples = schroederReverb(outputSamples, header.sampleRate);
    // std::vector<float> outputSamplesReverb = mixDryWet(outputSamples, reverbSamples, wetLevel);

    
    std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(outputSamples);
    saveWavFile(outputFilename, header, outputAudioBytes);


    // float reverbTime = 4.0f;
    // applyPitchedReverb(samples, sampleRate, 125, reverbTime, wetLevel);
    // std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(samples);
    // saveWavFile(outputFilename, header, outputAudioBytes);



    return 0;
}
