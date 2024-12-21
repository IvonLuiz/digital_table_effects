#include "reverb.h"
#include "wavHeader.cpp"

// Função para converter array de bytes em um array de amostras (16-bit PCM signed)
std::vector<int16_t> convertBytesToSamples16(const std::vector<uint8_t>& audioData) {
    std::vector<int16_t> samples;
    for (size_t i = 0; i < audioData.size(); i += 2) {
        // Reconstruct sample based on 2 bytes (little-endian)
        int16_t sample = audioData[i] | (audioData[i + 1] << 8);
        samples.push_back(sample);
    }
    return samples;
}

// Converte int16_t samples de volta para bytes (Little Endian)
std::vector<uint8_t> convertSamplesToBytes16(const std::vector<int16_t>& samples) {
    std::vector<uint8_t> audioBytes;
    audioBytes.reserve(samples.size() * 2);

    for (int16_t sample : samples) {
        // Little endian: LSB (least significant byte) first
        audioBytes.push_back(sample & 0xFF);        // First byte (lest significant 8 bits)
        audioBytes.push_back((sample >> 8) & 0xFF); // Segundo byte (most significant 8 bits)
    }
    return audioBytes;
}

void saveWavFile(const char* filename, const WavHeader& header, const std::vector<uint8_t>& audioBytes) {
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        std::cerr << "Error: Could not open output file for writing: " << filename << std::endl;
        return;
    }

    // Escreve o cabeçalho original (com ajustes no tamanho dos dados)
    WavHeader newHeader = header;
    newHeader.dataSize = audioBytes.size();
    newHeader.chunkSize = 36 + newHeader.dataSize;

    outFile.write(reinterpret_cast<const char*>(&newHeader), sizeof(WavHeader));

    // Escreve os dados de áudio convertidos (em bytes)
    outFile.write(reinterpret_cast<const char*>(audioBytes.data()), audioBytes.size());

    outFile.close();
    std::cout << "File saved successfully: " << filename << std::endl;
}



int main() {
    const char* filename = "Lagrima.wav";
    // double delayMs = 80;
    double delayMs = .040; // Teste com 1 milissegundo de atraso
    double decayGain = 0.85;
    float sampleRate = 48000;

    std::ifstream inputFile(filename, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Read WAV header
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

    // Read audio data in a vector of bytes
    std::vector<uint8_t> audioData(header.dataSize);
    inputFile.read(reinterpret_cast<char*>(audioData.data()), header.dataSize);

    if (!inputFile) {
        std::cerr << "Error: Could not read audio data." << std::endl;
        return 1;
    }

    inputFile.close();

    std::cout << "Audio data successfully read into a byte array!" << std::endl;

    // Convert to samples
    std::vector<int16_t> samples = convertBytesToSamples16(audioData);
    std::cout << "size of samples: " << samples.size() << std::endl;

    std::cout << "First 10 Samples: " << std::endl;
    for (size_t i = 0; i < 10 && i < samples.size(); ++i) {
        std::cout << "Sample " << i << ": " << samples[i] << std::endl;
    }

    std::vector<int16_t> delayed = combFilter(samples, delayMs, decayGain, sampleRate);
    std::vector<uint8_t> outputBytes = convertSamplesToBytes16(delayed);

    saveWavFile("output_reverb.wav", header, outputBytes);
    
    return 0;
}
