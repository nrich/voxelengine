#ifndef __CLIENT_BASESTATE_H__
#define __CLIENT_BASESTATE_H__

#include <cstdint>
#include <memory>
#include <any>

#include "Client/Events.h"

#include "Math/Point3.h"
#include "Renderer/Base.h"

#include "Common/DisplayMode.h"

namespace Client {
    class State;

    class BaseState {
        public:
            virtual void onRender(State &state, const uint64_t time);
            virtual void onTick(State &state, const uint64_t time);
            virtual void onMouseMove(State &state, const MouseMove &event);
            virtual void onMouseButtonPress(State &state, const MouseClick &event);
            virtual void onMouseButtonRelease(State &state, const MouseClick &event);
            virtual void onMouseScroll(State &state, const MouseScroll &event);
            virtual void onKeyDown(State &state, const KeyPress &event);
            virtual void onKeyUp(State &state, const KeyPress &event);
            virtual void onTouchPress(State &state, const TouchEvent &event);
            virtual void onTouchRelease(State &state, const TouchEvent &event);
            virtual void onTouchMotion(State &state, const TouchEvent &event);
            virtual void onEnterState(State &state, std::any data);
            virtual void onLeaveState(State &state, std::any data);

            virtual void changeDisplayMode(const Common::DisplayMode &displayMode);

            virtual ~BaseState();
    };
};

#endif //__CLIENT_BASESTATE_H__
