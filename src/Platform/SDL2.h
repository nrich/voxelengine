#ifndef __PLATFORM_SDL2_H__
#define __PLATFORM_SDL2_H__

#include <SDL.h>

#include <memory>
#include <functional>
#include <array>

#include "Platform/Base.h"
#include "Client/State.h"
#include "Audio/Tone.h"

namespace Platform {
    class SDL2 : public Base {
        std::shared_ptr<SDL_Window> window;
        SDL_AudioDeviceID dev;
        SDL_GLContext context;

        void setWindow(std::shared_ptr<SDL_Window> window) {
            this->window = window;
        }

        bool repeatKeys;

        std::array<Audio::Tone, VOICE_COUNT> voices;

        bool audioStarted;
        bool vsync;

        void initAudio();
    public:
        SDL2(const std::string &title, std::pair<uint8_t, uint8_t> glversion = std::make_pair(3, 3));
        ~SDL2();
        Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
        std::vector<Common::DisplayMode> getDisplayModes() const;
        Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
        Common::DisplayMode currentDisplayMode() const;
        std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
        bool isFullScreen() const;
        bool handleEvents(std::shared_ptr<Client::State> clientState);
    
        void swapBuffers() {
            SDL_GL_SwapWindow(window.get());
        }

        uint64_t getTicks() const {
            return SDL_GetTicks() * 1000;
        }

        void clearScreen() const {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void keyRepeat(bool enable) {
            repeatKeys = enable;
        }

        bool vsyncEnabled() const {
            return vsync;
        }

        void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);

        void setSwapInterval(int32_t interval);

        void grabInput(bool grab) {
            SDL_SetWindowGrab(window.get(), grab ? SDL_TRUE : SDL_FALSE);
        }
    };
}; // Platform

#endif //__PLATFORM_SDL2_H__
