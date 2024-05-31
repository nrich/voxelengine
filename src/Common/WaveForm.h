#ifndef __COMMON_WAVEFORM_H__
#define __COMMON_WAVEFORM_H__

#include "Common/Shared.h"

namespace Common {
    namespace WaveForm {
        enum {
            SINE = 0,
            SAWTOOTH,
            TRIANGLE,
            SQUARE,
            NOISE,
            PINKNOISE,
            COUNT
        };
    }; // namespace WaveForm
}; // namespace Common

#endif //__COMMON_WAVEFORM_H__
