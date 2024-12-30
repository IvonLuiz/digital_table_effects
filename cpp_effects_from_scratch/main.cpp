#include "reverb.h"
#include "wavHeader.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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

    // Ajusta os tamanhos do cabeçalho
    WavHeader newHeader = header;
    newHeader.dataSize = audioBytes.size();
    newHeader.chunkSize = 36 + newHeader.dataSize;

    outFile.write(reinterpret_cast<const char*>(&newHeader), sizeof(WavHeader));
    outFile.write(reinterpret_cast<const char*>(audioBytes.data()), audioBytes.size());
    outFile.close();
    std::cout << "File saved successfully: " << filename << std::endl;
}

int main() {
    const char* filename = "original.wav";
    const char* outputFilename = "audio_reverb.wav";
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

// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <cstdint>
// #include <cmath>
// #include <algorithm>

// // Implementação manual de clamp
// template <typename T>
// constexpr const T& clamp(const T& value, const T& min, const T& max) {
//     return (value < min) ? min : (value > max ? max : value);
// }


// // Definição do cabeçalho WAV
// struct WavHeader {
//     char riff[4];             // "RIFF"
//     uint32_t chunkSize;       // Tamanho do arquivo - 8 bytes
//     char wave[4];             // "WAVE"
//     char fmt[4];              // "fmt "
//     uint32_t subchunk1Size;   // 16 for PCM
//     uint16_t audioFormat;     // 1 for PCM
//     uint16_t numChannels;     // Número de canais (1 = mono, 2 = stereo)
//     uint32_t sampleRate;      // Taxa de amostragem (ex: 44100 Hz)
//     uint32_t byteRate;        // SampleRate * NumChannels * BitsPerSample/8
//     uint16_t blockAlign;      // NumChannels * BitsPerSample/8
//     uint16_t bitsPerSample;   // Bits por amostra (ex: 16 bits)
//     char data[4];             // "data"
//     uint32_t dataSize;        // Tamanho dos dados de áudio
// };

// // Função para carregar um arquivo WAV
// bool loadWav(const std::string& filename, WavHeader& header, std::vector<int16_t>& samples) {
//     std::ifstream file(filename, std::ios::binary);
//     if (!file.is_open()) {
//         std::cerr << "Erro ao abrir o arquivo " << filename << "\n";
//         return false;
//     }

//     // Ler o cabeçalho
//     file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
//     if (std::strncmp(header.riff, "RIFF", 4) != 0 || std::strncmp(header.wave, "WAVE", 4) != 0) {
//         std::cerr << "Formato de arquivo inválido.\n";
//         return false;
//     }

//     // Verificar suporte para PCM e 16 bits
//     if (header.audioFormat != 1 || header.bitsPerSample != 16) {
//         std::cerr << "Formato não suportado. Apenas PCM com 16 bits é suportado.\n";
//         return false;
//     }

//     // Ler os dados de áudio
//     samples.resize(header.dataSize / sizeof(int16_t));
//     file.read(reinterpret_cast<char*>(samples.data()), header.dataSize);

//     return true;
// }

// // Função para salvar um arquivo WAV
// bool saveWav(const std::string& filename, const WavHeader& header, const std::vector<int16_t>& samples) {
//     std::ofstream file(filename, std::ios::binary);
//     if (!file.is_open()) {
//         std::cerr << "Erro ao criar o arquivo " << filename << "\n";
//         return false;
//     }

//     // Escrever o cabeçalho
//     file.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

//     // Escrever os dados de áudio
//     file.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));

//     return true;
// }

// // Função para converter amostras int16_t para float (-1.0f a 1.0f)
// std::vector<float> unpackSamples(const std::vector<int16_t>& samples) {
//     const float scale = 1.0f / 32768.0f;
//     std::vector<float> floatSamples(samples.size());
//     for (size_t i = 0; i < samples.size(); ++i) {
//         floatSamples[i] = samples[i] * scale;
//     }
//     return floatSamples;
// }

// // Função para converter amostras float (-1.0f a 1.0f) para int16_t
// std::vector<int16_t> packSamples(const std::vector<float>& floatSamples) {
//     const float scale = 32768.0f;
//     std::vector<int16_t> samples(floatSamples.size());
//     for (size_t i = 0; i < floatSamples.size(); ++i) {
//         samples[i] = static_cast<int16_t>(clamp(floatSamples[i], -1.0f, 1.0f) * scale);
//     }
//     return samples;
// }

// int main() {
//     const std::string inputFile = "01.wav";
//     const std::string outputFile = "output.wav";

//     WavHeader header;
//     std::vector<int16_t> samples;

//     // Carregar o arquivo WAV
//     if (!loadWav(inputFile, header, samples)) {
//         return 1;
//     }

//     std::cout << "Samples: " << samples.size() << "\n";

//     // Converter para float
//     auto floatSamples = unpackSamples(samples);

//     // Aplicar processamento (ex.: reduzir o volume)
//     for (auto& sample : floatSamples) {
//         sample *= 0.5f; // Reduzir o volume
//     }

//     // Converter de volta para int16_t
//     auto processedSamples = packSamples(floatSamples);

//     // Salvar o arquivo processado
//     if (!saveWav(outputFile, header, processedSamples)) {
//         return 1;
//     }

//     std::cout << "Processamento concluído. Arquivo salvo como " << outputFile << "\n";
//     return 0;
// }
