#ifndef __PLATFORM_GLFW_H__
#define __PLATFORM_GLFW_H__

#include "Common/Shared.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <functional>
#include <queue>
#include <vector>
#include <utility>

#include <portaudio.h>

#include "Platform/Base.h"
#include "Client/State.h"

#include "Audio/Tone.h"

namespace Platform {
    class GLFW : public Base {
        std::shared_ptr<GLFWwindow> window;
        PaStream *stream;
        std::array<Audio::Tone, VOICE_COUNT> voices;
        bool vsync;
    public:
        GLFW(const std::string &title, std::pair<uint8_t, uint8_t> glversion = std::make_pair(3, 3));
        ~GLFW();
        Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
        std::vector<Common::DisplayMode> getDisplayModes() const;
        Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
        Common::DisplayMode currentDisplayMode() const;
        std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
        bool isFullScreen() const;
        bool handleEvents(std::shared_ptr<Client::State> clientState);
    
        void swapBuffers() {
            glfwSwapBuffers(window.get());
        }

        uint64_t getTicks() const {
            return (uint64_t)(glfwGetTime()*1000000);
        }

        void clearScreen() const {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        bool vsyncEnabled() const {
            return vsync;
        }

        void keyRepeat(bool enable);

        void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);

        void setSwapInterval(int32_t interval);

        void grabInput(bool grab) {
            glfwSetInputMode(window.get(), GLFW_CURSOR, grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    };
}; // Platform

#endif //__PLATFORM_GLFW_H__
