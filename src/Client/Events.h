#ifndef __CLIENT_EVENTS_H__
#define __CLIENT_EVENTS_H__

#include <cstdint>

namespace Client {
    struct KeyPress {
        uint32_t keyCode; 
        bool shiftMod = false;
        bool ctrlMod = false;
        bool altMod = false;
        bool guiMod = false;
    };

    struct MouseMove {
        int32_t x;
        int32_t y;
        int32_t xrel;
        int32_t yrel;

/*
        bool leftPressed;
        bool middlePressed;
        bool rightPressed;
        bool x1Pressed;
        bool x2Pressed;
*/
    };

    struct MouseClick {
        int32_t x;
        int32_t y;

        bool leftPressed;
        bool middlePressed;
        bool rightPressed;
        bool x1Pressed;
        bool x2Pressed;
    };

    struct MouseScroll {
        int32_t x;
        int32_t y;
    };

    struct TouchEvent {
        int32_t x;
        int32_t y;
        int64_t finger;
        float pressure;
    };
}; // namespace Client

#endif //__CLIENT_EVENTS_H__
