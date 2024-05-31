#include "Common/DisplayMode.h"

#include <iostream>

Common::DisplayMode::DisplayMode(uint16_t width, uint16_t height, uint16_t refresh) : width(width), height(height), refresh(refresh) {
    int r = (float)height/(float)width * 100;

    if (r == 75) {
        ratio = Common::AspectRatio::_4x3;
    } else if (r == 56) {
        ratio = Common::AspectRatio::_16x9;
    } else if (r == 62) {
        ratio = Common::AspectRatio::_16x10;
    } else {
        ratio = Common::AspectRatio::Ignore;
    }
}

