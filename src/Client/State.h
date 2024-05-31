#ifndef __CLIENT_STATE_H__
#define __CLIENT_STATE_H__

#include <map>
#include <memory>
#include <any>

#include "Client/BaseState.h"
#include "Game/State.h"

#include "Renderer/Base.h"
#include "Common/DisplayMode.h"

#include "Platform/Base.h"

namespace Client {
    class State {
        std::map<uint32_t, std::shared_ptr<BaseState>> states;
        std::shared_ptr<Client::BaseState> currentState;
        std::shared_ptr<Renderer::Base> renderer;
        std::shared_ptr<Platform::Base> sys;
        std::shared_ptr<Game::State> gameState;
    public:
        State(std::shared_ptr<Renderer::Base> renderer, std::shared_ptr<Platform::Base> sys, std::shared_ptr<Game::State> gameState, std::pair<uint32_t, std::shared_ptr<BaseState>> state);
        void changeState(const uint32_t newState, std::any enter=0, std::any leave=0);

        // Event dispatch
        void render(const uint64_t time);
        void tick(const uint64_t time);
        void mouseMove(const MouseMove &event);
        void mouseButtonPress(const MouseClick &event);
        void mouseButtonRelease(const MouseClick &event);
        void mouseScroll(const MouseScroll &event);
        void keyDown(const KeyPress &event);
        void keyUp(const KeyPress &event);
        void touchPress(const TouchEvent &event);
        void touchRelease(const TouchEvent &event);
        void touchMotion(const TouchEvent &event);

        std::shared_ptr<Renderer::Base> getRenderer() const {
            return renderer;
        }

        std::shared_ptr<Platform::Base> getPlatform() const {
            return sys;
        }

        std::shared_ptr<Game::State> getGameState() const {
            return gameState;
        }

        const Common::DisplayMode getDisplayMode() const {
            return sys->currentDisplayMode();
        }


        void addState(const uint32_t newState, std::shared_ptr<BaseState> state) {
            states[newState] = state;
        }

        void changeDisplayMode(const Common::DisplayMode &mode, bool fullscreen) {
            //displayMode = sys->changeDisplayMode(mode, fullscreen);
            sys->changeDisplayMode(mode, fullscreen);
            renderer->changeDisplayMode(mode);
        }

        ~State();
    };
};

#endif //__CLIENT_STATE_H__
