#include "reverb.h"
#include "wavHeader.cpp"

int main() {
    const char* filename = "Lagrima.wav";
    std::ifstream inputFile(filename, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Ler o cabeçalho WAV
    WavHeader header;
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    // Validar o cabeçalho
    if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE") {
        std::cerr << "Error: Not a valid WAV file." << std::endl;
        return 1;
    }

    std::cout << "Sample Rate: " << header.sampleRate << " Hz" << std::endl;
    std::cout << "Channels: " << header.numChannels << std::endl;
    std::cout << "Bits per Sample: " << header.bitsPerSample << std::endl;
    std::cout << "Data Size: " << header.dataSize << " bytes" << std::endl;

    // Ler os dados de áudio em um vetor de bytes
    std::vector<uint8_t> audioData(header.dataSize);
    inputFile.read(reinterpret_cast<char*>(audioData.data()), header.dataSize);

    if (!inputFile) {
        std::cerr << "Error: Could not read audio data." << std::endl;
        return 1;
    }

    inputFile.close();

    std::cout << "Audio data successfully read into a byte array!" << std::endl;

    // Exemplo: Printar os primeiros 10 bytes
    for (size_t i = 0; i < 10 && i < audioData.size(); ++i) {
        std::cout << "Byte " << i << ": " << static_cast<int>(audioData[i]) << std::endl;
    }

    return 0;
}
