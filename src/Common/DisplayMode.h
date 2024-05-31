#ifndef __COMMON_DISPLAYMODE_H__
#define __COMMON_DISPLAYMODE_H__

#include "Common/Shared.h"

#include <cstdint>
#include <vector>
#include <sstream>

namespace Common {
    enum class AspectRatio {
        Ignore,
        _4x3,
        _16x9,
        _16x10
    };

    class DisplayMode {
        uint16_t width; 
        uint16_t height; 
        uint16_t refresh;
        AspectRatio ratio;

    public:
        DisplayMode(uint16_t width, uint16_t height, uint16_t refresh);
        DisplayMode(const DisplayMode &mode) : width(mode.width), height(mode.height), refresh(mode.refresh), ratio(mode.ratio) {}

        uint16_t Width() const { return width; }
        uint16_t Height() const { return height; }
        uint16_t Refresh() const { return refresh; }
        AspectRatio Ratio() const { return ratio; }
        static std::vector<Common::DisplayMode> Modes;

        static const DisplayMode Find(uint16_t width, uint16_t height);
        static const DisplayMode Find(const AspectRatio &ratio);

        bool operator==(const DisplayMode &other) const {
            if (other.width != width)
                return false;
            if (other.height != height)
                return false;
            if (other.refresh != refresh)
                return false;

            return true;
        }

        std::string toString() const {
            std::ostringstream s;

            s << width << "x" << height << "@" << refresh;

            return s.str();
        }
    };
}; // namespace Common


#endif //__COMMON_DISPLAYMODE_H__
