#include <vector>
#include "effects/pitchShifter.h"

void generateHannWindow(std::vector<float>& window, int fftSize) {
    window.resize(fftSize);
    for (int i = 0; i < fftSize; ++i) {
        window[i] = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / fftSize));
        window[i] = 0.5f  - 0.5f * std::cos(2.0f * M_PI * n++ / fftSize));
    }
}

void shiftPitch(const std::vector<Sample>& input, std::vector<Sample>& output, int fftSize, int hopSize, float sampleRate) {
    window.resize(fftSize);
    prevPhase.resize(fftSize / 2 + 1);
    outputBuffer.resize(fftSize);

    // Create a Hann window
    std::generate(window.begin(), window.end(), [n = 0, fftSize]() mutable {
    });
}

void processPitchShift(const std::vector<Sample>& input, std::vector<Sample>& output, int fftSize, int hopSize, float sampleRate) {
    int numFrames = input.size() / hopSize;
    output.resize(input.size());

    windows.resize(fftSize);
    prevPhases.resize(fftSize / 2 + 1);
    outputBuffers.resize(fftSize);

    // Create a Hann window
    std::generate(windows.begin(), windows.end(), [n = 0, fftSize]() mutable {
        return 0.5f * (1.0f - std::cos(2.0f * M_PI * n++ / fftSize));
    });

    for (int frame = 0; frame < numFrames; ++frame) {
        std::vector<Sample> inputFrame(input.begin() + frame * hopSize, input.begin() + frame * hopSize + fftSize);
        applyWindow(inputFrame);

        std::vector<Complex> fftBuffer;
        fft(inputFrame, fftBuffer);

        std::vector<Complex> shiftedFftBuffer(fftBuffer.size());
        std::transform(fftBuffer.begin(), fftBuffer.end(), prevPhases.begin(), shiftedFftBuffer.begin(),
            [hopSize, fftSize](Complex& bin, Complex& prev) {
                float magnitude = std::abs(bin);
                float phase = std::arg(bin);
                float phaseDiff = phase - std::arg(prev);
                float trueFreq = phaseDiff / (2.0f * M_PI * hopSize / fftSize);
                float newPhase = std::arg(prev) + 2.0f * M_PI * trueFreq * pitchShift;
                prev = bin;
                return Complex(magnitude * std::cos(newPhase), magnitude * std::sin(newPhase));
            });

        ifft(shiftedFftBuffer, inputFrame);
        overlapAdd(inputFrame, output);
    }
}

class OverlapAddStretch {
public:
	using Sample = double;

	OverlapAddStretch(bool isSpectral=false) : isSpectral(isSpectral) {}
	
	void configure(int channels, int blockSamples, int intervalSamples, int maxExtraInput=0) {
		this->channels = channels;
		this->blockSamples = blockSamples;
		this->intervalSamples = intervalSamples;
		this->maxSurplusInputSamples = maxExtraInput;

		inputHistory.resize(channels, blockSamples + maxExtraInput);
		summedOutput.resize(channels, blockSamples);
		blockBuffers.resize(blockSamples*channels);
		window.resize(blockSamples);
		if (isSpectral) {
			// Kaiser's a good window for spectral stuff, but not so great for time-domain
			auto kaiser = signalsmith::windows::Kaiser::withBandwidth(blockSamples*1.0/intervalSamples, true);
			kaiser.fill(window, blockSamples);
		} else {
			for (int i = 0; i < blockSamples; ++i) {
				double r = (i + 0.5)/blockSamples;
				window[i] = std::sin(r*M_PI); // sine window, becomes Hann when applied twice
			}
		}
		// Makes it add up nicely to 1 when applied twice
		signalsmith::windows::forcePerfectReconstruction(window, blockSamples, intervalSamples);
		
		intervalCounter = 0;
	}
	
	void reset() {
		inputHistory.reset();
		summedOutput.reset();
		intervalCounter = 0;
	}
	
	void setRate(double rate) {
		invTimeFactor = rate;
	}
	void setTimeFactor(double timeFactor) {
		invTimeFactor = 1/timeFactor;
	}
	/// How many input samples do we need to get this much output?
	int samplesForOutput(int outputSamples) const {
		double inputSamples = outputSamples*invTimeFactor - surplusInputSamples;
		return int(std::ceil(inputSamples));
	}
	
	void process(const Sample * const *inputs, int inputSamples, Sample **outputs, int outputSamples) {
		int inputFilledTo = 0;
		for (int o = 0; o < outputSamples; ++o) {
			if (++intervalCounter >= intervalSamples) {
				intervalCounter = 0;
				// Fill the block from the input
				int inputStart = int(std::round(o*invTimeFactor - surplusInputSamples - blockSamples));
				// For safety: don't go past the end of the block, or too far in the past
				inputStart = std::max(std::min(inputStart, inputSamples - blockSamples), -maxSurplusInputSamples - blockSamples);
				for (int c = 0; c < channels; ++c) {
					// Make sure we have enough input history
					auto input = inputs[c];
					auto history = inputHistory[c];
					for (int i = inputFilledTo; i < inputStart + blockSamples; ++i) {
						history[i] = input[i];
					}
					// Fill the block from history
					Sample *blockBuffer = channelBlock(c);
					for (int i = 0; i < blockSamples; ++i) {
						blockBuffer[i] = history[inputStart + i]*window[i];
					}
				}
				
				processBlock(inputStart - prevInputIndex);
				prevInputIndex = inputStart;
				
				// Add the block to the summed output
				for (int c = 0; c < channels; ++c) {
					Sample *blockBuffer = channelBlock(c);
					auto output = summedOutput[c];
					for (int i = 0; i < blockSamples; ++i) {
						output[i] += blockBuffer[i]*window[i];
					}
				}
			}
			for (int c = 0; c < channels; ++c) {
				outputs[c][o] = summedOutput[c][0];
				summedOutput[c][0] = 0;
			}
			++summedOutput;
		}
		
		// Copy in remaining input
		for (int c = 0; c < channels; ++c) {
			auto input = inputs[c];
			auto history = inputHistory[c];
			for (int i = inputFilledTo; i < inputSamples; ++i) {
				history[i] = input[i];
			}
		}
		inputHistory += inputSamples;
		prevInputIndex -= inputSamples;
		surplusInputSamples += inputSamples - outputSamples*invTimeFactor;
	}
	
	int inputLatency() const {
		return blockSamples/2;
	}
	int outputLatency() const {
		return blockSamples - inputLatency();
	}

protected:
	int channels = 0, blockSamples = 0;
	int intervalSamples = 0, intervalCounter = 0;
	double invTimeFactor = 1;
	
	Sample * channelBlock(int channel) {
		return blockBuffers.data() + channel*blockSamples;
	}
	
	virtual void processBlock(int inputIntervalSamples) {
		// Alter the blocks (for each channel) if we want to
		(void)inputIntervalSamples;
	}

	void scheduleNextBlock(int interval) {
		intervalCounter = intervalSamples - interval;
	}
private:
	bool isSpectral;

	// Multi-channel circular buffers
	signalsmith::delay::MultiBuffer<Sample> inputHistory, summedOutput;
	std::vector<Sample> blockBuffers, window;

	// Unused input samples, which may be fractional
	int maxSurplusInputSamples = 0;
	double surplusInputSamples = 0;
	int prevInputIndex = 0;
};


// #define _USE_MATH_DEFINES

// #include "effects/pitchShifter.h"
// #include <cmath>
// #include <algorithm>
// #include <numeric>
// #include <functional>

// static float pitchShift = 1.0f;
// static std::vector<Sample> window;
// static std::vector<Complex> prevPhase;
// static std::vector<Sample> outputBuffer;
// static int outputBufferIndex = 0;

// void setPitchShift(float newPitchShift) {
//     pitchShift = newPitchShift;
// }

// void fft(const std::vector<Sample>& input, std::vector<Complex>& output) {
//     int n = input.size();
//     output.resize(n);
//     for (int k = 0; k < n; ++k) {
//         output[k] = {0, 0};
//         for (int t = 0; t < n; ++t) {
//             double angle = 2 * M_PI * t * k / n;
//             output[k] += input[t] * Complex(std::cos(angle), -std::sin(angle));
//         }
//     }
// }

// void ifft(const std::vector<Complex>& input, std::vector<Sample>& output) {
//     int n = input.size();
//     output.resize(n);
//     for (int t = 0; t < n; ++t) {
//         Complex sum = {0, 0};
//         for (int k = 0; k < n; ++k) {
//             double angle = 2 * M_PI * t * k / n;
//             sum += input[k] * Complex(std::cos(angle), std::sin(angle));
//         }
//         output[t] = sum.real() / n;
//     }
// }

// void applyWindow(std::vector<Sample>& buffer) {
//     std::transform(buffer.begin(), buffer.end(), window.begin(), buffer.begin(), std::multiplies<Sample>());
// }

// void overlapAdd(const std::vector<Sample>& buffer, std::vector<Sample>& output) {
//     std::transform(buffer.begin(), buffer.end(), output.begin() + outputBufferIndex, output.begin() + outputBufferIndex, std::plus<Sample>());
//     outputBufferIndex += buffer.size() / 2;
//     if (outputBufferIndex >= buffer.size()) {
//         outputBufferIndex = 0;
//     }
// }

// void processPitchShift(const std::vector<Sample>& input, std::vector<Sample>& output, int fftSize, int hopSize, float sampleRate) {
//     int numFrames = input.size() / hopSize;
//     output.resize(input.size());

//     window.resize(fftSize);
//     prevPhase.resize(fftSize / 2 + 1);
//     outputBuffer.resize(fftSize);

//     // Create a Hann window
//     std::generate(window.begin(), window.end(), [n = 0, fftSize]() mutable {
//         return 0.5f * (1.0f - std::cos(2.0f * M_PI * n++ / fftSize));
//     });

//     for (int frame = 0; frame < numFrames; ++frame) {
//         std::vector<Sample> inputFrame(input.begin() + frame * hopSize, input.begin() + frame * hopSize + fftSize);
//         applyWindow(inputFrame);

//         std::vector<Complex> fftBuffer;
//         fft(inputFrame, fftBuffer);

//         std::vector<Complex> shiftedFftBuffer(fftBuffer.size());
//         std::transform(fftBuffer.begin(), fftBuffer.end(), prevPhase.begin(), shiftedFftBuffer.begin(),
//             [hopSize, fftSize](Complex& bin, Complex& prev) {
//                 float magnitude = std::abs(bin);
//                 float phase = std::arg(bin);
//                 float phaseDiff = phase - std::arg(prev);
//                 float trueFreq = phaseDiff / (2.0f * M_PI * hopSize / fftSize);
//                 float newPhase = std::arg(prev) + 2.0f * M_PI * trueFreq * pitchShift;
//                 prev = bin;
//                 return Complex(magnitude * std::cos(newPhase), magnitude * std::sin(newPhase));
//             });

//         ifft(shiftedFftBuffer, inputFrame);
//         overlapAdd(inputFrame, output);
//     }
// }