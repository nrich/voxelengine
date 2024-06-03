#ifndef __CLIENT_VOXELSTATE_H__
#define __CLIENT_VOXELSTATE_H__

#include <array>

#include "Client/BaseState.h"
#include "Renderer/Program.h"
#include "Renderer/Text.h"
#include "Renderer/ColourBuffer.h"
#include "Renderer/Voxel.h"
#include "Common/Chunk.h"
#include "Common/World.h"

namespace Client {
    class State;

    class VoxelState : public BaseState {
        Renderer::Program voxelprog;
        Renderer::Voxel voxel;
        Renderer::Text text;
        Renderer::ColourBuffer colourBuffer;
        const Common::World *world;

        uint32_t inputState;

        int64_t clientId;
        std::string secret;

        Vec3F origin;
        Vec3F oldOrigin;
        Vec3F viewAngles;
        Vec3F angles;
        Vec3F velocity;

        uint64_t tickTime;
        uint64_t gameTime;
    public:
        VoxelState(const Common::World *world);

        void onRender(State &state, const uint64_t time);
        void onTick(State &state, const uint64_t time);
        void onMouseMove(State &state, const MouseMove &event);
        void onMouseButtonPress(State &state, const MouseClick &event);
        void onMouseButtonRelease(State &state, const MouseClick &event);
        void onKeyDown(State &state, const KeyPress &event);
        void onTouchPress(State &state, const TouchEvent &event);
        void onTouchRelease(State &state, const TouchEvent &event);
        void onTouchMotion(State &state, const TouchEvent &event);
        void onKeyUp(State &state, const KeyPress &event);
        void onEnterState(State &state, std::any data);
    };
};

#endif //__CLIENT_VOXELSTATE_H__
