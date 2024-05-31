#ifndef __AUDIO_TONE_H__
#define __AUDIO_TONE_H__

#include <cstdint>
#include <queue>

#include "Common/WaveForm.h"

namespace Audio {
    const int FREQUENCY = 44100;

    struct ToneObject {
        double freq;
        int samplesLeft;
        uint8_t waveForm;
        float volume;

        int attackLeft;
        float attackStep;

        int decayLeft;
        float decayStep;

        int sustainLeft;
        float sustainLevel;

        int releaseLeft;
        float releaseStep;
    };

    class Tone {
        private:
            double v;
            std::queue<ToneObject> tones;
        public:
            Tone();
            ~Tone() {}
            void tone(float freq, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
            void generateSamples(float *stream, int length, float amplitude=1.0f);
            void wait();
    };
}; // Audio

#endif //__AUDIO_TONE_H__
