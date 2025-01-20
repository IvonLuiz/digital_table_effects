#include <fstream>
#include <vector>
#include <iostream>
#include "effects/reverb.h"
#include "effects/pitchShifter.h"
#include "effects/pitchedReverb.h"
#include "audio/handleAudio.h"
#include <cmath>
#include <stdexcept>



int main() {
    const char* filename = "src/audioFiles/original.wav";
    const char* outputFilename = "src/audioFiles/audio_pitch.wav";
    float wetLevel = 0.5f; // 50% wet signal
    int toneToShift = 2; // Example: 2 semitones (equivalent to Dó -> Ré)
    float pitchShift = pow(2.0f, toneToShift / 12.0f);
    //  pitchShift = 1.5f; // 1.5 semitones

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
    
    // float reverbTime = 4.0f;
    // applyPitchedReverb(samples, sampleRate, 125, reverbTime, wetLevel);
    // std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(samples);
    // saveWavFile(outputFilename, header, outputAudioBytes);
    shiftPitch(samples, outputSamples, sampleRate, pitchShift);

    // Applying Schroeder reverberation algorithm
    std::vector<float> reverbSamples = schroederReverb(outputSamples, header.sampleRate);
    std::vector<float> outputSamplesReverb = mixDryWet(outputSamples, reverbSamples, wetLevel);

    std::vector<uint8_t> outputAudioBytes = convertSamplesToBytes16(outputSamplesReverb);


    saveWavFile(outputFilename, header, outputAudioBytes);
    std::cout <<"Pitch Shifting Done!\n";
    return 0;
}
