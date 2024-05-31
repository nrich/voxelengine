#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include <cmath>
#include <random>
#include <array>
#include <iostream>

#include "Audio/Tone.h"
#include "Common/Shared.h"

#define PINK_MAX_RANDOM_ROWS   (30)
#define PINK_RANDOM_BITS       (24)
#define PINK_RANDOM_SHIFT      ((sizeof(int32_t)*8)-PINK_RANDOM_BITS)

static double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

class PinkNoise {
    private:
        std::array<int32_t, PINK_MAX_RANDOM_ROWS> rows;
        int32_t runningSum;     // Used to optimize summing of generators
        int32_t index;          // Incremented each sample
        int32_t indexMask;      // Index wrapped by ANDing with this mask
        float scalar;           // Used to scale within range of -1.0 to +1.0
        uint32_t seed;

        uint32_t generateRandomNumber() {
            static uint32_t randSeed = seed;
            randSeed = (randSeed * 196314165) + 907633515;
            return randSeed;
        }
    public:
        PinkNoise(int numRows) {
            numRows = std::min(PINK_MAX_RANDOM_ROWS, numRows);

            seed = std::chrono::system_clock::now().time_since_epoch().count();
            index = 0;
            indexMask = (1<<numRows) - 1;

            // Calculate maximum possible signed random value. Extra 1 for white noise always added.
            int32_t pmax = (numRows + 1) * (1<<(PINK_RANDOM_BITS-1));
            scalar = 1.0f / pmax;

            rows.fill(0);
            runningSum = 0;
        }

        float generate() {
            int32_t newRandom;
            int32_t sum;
            float output;

            // Increment and mask index
            index = (index + 1) & indexMask;

            // If index is zero, don't update any random values.
            if (index != 0) {
                // Determine how many trailing zeros in PinkIndex.
                // This algorithm will hang if n==0 so test first.
                int numZeros = 0;
                int n = index;
                while((n & 1) == 0) {
                    n = n >> 1;
                    numZeros++;
                }
                // Replace the indexed ROWS random value.
                // Subtract and add back to RunningSum instead of adding all the random
                // values together. Only one changes each time.
                runningSum -= rows[numZeros];
                newRandom = ((int32_t)generateRandomNumber()) >> PINK_RANDOM_SHIFT;
                runningSum += newRandom;
                rows[numZeros] = newRandom;
            }

            // Add extra white noise value.
            newRandom = ((int32_t)generateRandomNumber()) >> PINK_RANDOM_SHIFT;
            sum = runningSum + newRandom;
            // Scale to range of -1.0 to 0.9999.
            output = scalar * sum;

            return output;
        }
};

Audio::Tone::Tone() {
    v = 0.0;
}

void Audio::Tone::tone(float freq, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) {
    ToneObject to;
    to.freq = (double)freq;
    to.samplesLeft = duration * FREQUENCY / 1000;
    to.waveForm = waveForm;
    to.volume = (float)volume/(float)UINT8_MAX;

    const int Scale = to.samplesLeft / (10 * FREQUENCY / 1000);

    to.sustainLevel = (float)sustain/(float)UINT8_MAX;

    to.attackLeft = attack*Scale;
    to.attackStep = ((float)UINT8_MAX/(float)attack)/UINT8_MAX/Scale;

    if (sustain == 255) {
        to.decayLeft = 0;
    } else {
        to.decayLeft = decay*Scale;
        to.decayStep = (1.0-to.sustainLevel) * ((float)(UINT8_MAX)/(float)decay)/UINT8_MAX/Scale;
    }

    to.releaseLeft = release * Scale;
    to.releaseStep = to.sustainLevel * ((float)(UINT8_MAX)/(float)release)/UINT8_MAX/Scale;

    to.sustainLeft = to.samplesLeft - to.attackLeft - to.decayLeft - to.releaseLeft;

    tones.push(to);

#if 0
    std::cerr << (to.attackLeft * to.attackStep) << "," << (0 * to.attackStep) << std::endl;
    std::cerr << (to.decayLeft * to.decayStep) + to.sustainLevel << "," << (0 * to.decayStep) + to.sustainLevel << std::endl;
    std::cerr << to.sustainLevel << "," << to.sustainLeft << std::endl;
    std::cerr << (to.releaseLeft * to.releaseStep) << "," << (0 * to.releaseStep) << std::endl;
#endif
}

void Audio::Tone::generateSamples(float *stream, int length, float amplitude) {
    static PinkNoise pinknoise(16);
    int i = 0;

    std::array<float, 32> noise;

    for (size_t i = 0; i < noise.size(); i++) {
        noise[i] = fRand(-1.0, 1);
    }

    while (i < length) {
        if (tones.empty()) {
            while (i < length) {
                stream[i] += 0;
                i++;
            }
            return;
        }
        ToneObject& to = tones.front();

        int samplesToDo = std::min(i + to.samplesLeft, length);
        to.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) {
            float attack = 1.0f;
            float decay = 1.0f;
            float sustain = 1.0f;
            float release = 1.0f;

            if (to.attackLeft > 0) {
                attack -= (to.attackLeft * to.attackStep);
                to.attackLeft--;
            } else if (to.decayLeft > 0) {
                decay = (to.decayLeft * to.decayStep) + to.sustainLevel;
                to.decayLeft--;
            } else if (to.sustainLeft > 0) {
                sustain = to.sustainLevel;
                to.sustainLeft--;
            } else if (to.releaseLeft > 0) {
                release = (to.releaseLeft * to.releaseStep);
                to.releaseLeft--;
            }

            float pos = std::fmod(v/FREQUENCY,1.0);
            v += to.freq;

            switch (to.waveForm) {
                case Common::WaveForm::SAWTOOTH:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (pos*2 - 1);
                    break;
                case Common::WaveForm::TRIANGLE:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (1-std::fabs(pos-0.5)*4);
                    break;
                case Common::WaveForm::SQUARE:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (std::sin(pos*2*M_PI) >= 0 ? 1.0 : -1.0);
                    break;
                case Common::WaveForm::NOISE:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (noise[(int)(std::fmod(v/FREQUENCY,noise.size()))]);
                    break;
                case Common::WaveForm::PINKNOISE:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (pinknoise.generate());
                    break;
                case Common::WaveForm::SINE:
                default:
                    stream[i] += attack * decay * sustain * release * to.volume * amplitude * (std::sin(pos*2*M_PI));
            }
            i++;
        }

        if (to.samplesLeft == 0) {
            tones.pop();
        }
    }
}

void Audio::Tone::wait() {
    size_t size;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        size = tones.size();
    } while (size > 0);
}

