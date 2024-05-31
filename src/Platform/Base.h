#ifndef __PLATFORM_BASE_H__
#define __PLATFORM_BASE_H__

#include <cstdint>
#include <vector>
#include <utility>
#include <memory>

#include "Common/Shared.h"
#include "Common/DisplayMode.h"

namespace Client {
    class State;
};

namespace Platform {
    class Base {
    protected:
        bool fullscreen;
    public:
        virtual Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) = 0;
        virtual std::vector<Common::DisplayMode> getDisplayModes() const = 0;
        virtual Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const = 0;
        virtual Common::DisplayMode currentDisplayMode() const = 0;
        virtual std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const = 0;
        virtual bool isFullScreen() const = 0;
        virtual uint64_t getTicks() const = 0;
        virtual void clearScreen() const = 0;
        virtual void swapBuffers() = 0;
        virtual bool vsyncEnabled() const = 0;
        virtual bool handleEvents(std::shared_ptr<Client::State> clientState) = 0;
        virtual void keyRepeat(bool enable) = 0;
        virtual void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) = 0;
        virtual void setSwapInterval(int32_t interval) = 0;
        virtual void grabInput(bool grab) = 0;
        std::string DataPath(const std::string &path);
        virtual ~Base() {}
    };
}; // Platform

#endif //__PLATFORM_BASE_H__
