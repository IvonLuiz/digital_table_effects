#include "reverb.cpp"
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

int main() {
    const char* filename = "Lagrima.wav";
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

    std::cout << "First 10 Samples: " << std::endl;
    for (size_t i = 0; i < 10 && i < samples.size(); ++i) {
        std::cout << "Sample " << i << ": " << samples[i] << std::endl;
    }

    return 0;
}
