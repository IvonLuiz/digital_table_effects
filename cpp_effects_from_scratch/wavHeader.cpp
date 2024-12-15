#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

struct WavHeader {
    char riff[4];             // "RIFF"
    uint32_t chunkSize;       // Tamanho do arquivo - 8 bytes
    char wave[4];             // "WAVE"
    char fmt[4];              // "fmt "
    uint32_t subchunk1Size;   // 16 for PCM
    uint16_t audioFormat;     // 1 for PCM
    uint16_t numChannels;     // Número de canais (1 = mono, 2 = stereo)
    uint32_t sampleRate;      // Taxa de amostragem (ex: 44100 Hz)
    uint32_t byteRate;        // SampleRate * NumChannels * BitsPerSample/8
    uint16_t blockAlign;      // NumChannels * BitsPerSample/8
    uint16_t bitsPerSample;   // Bits por amostra (ex: 16 bits)
    char data[4];             // "data"
    uint32_t dataSize;        // Tamanho dos dados de áudio
};
