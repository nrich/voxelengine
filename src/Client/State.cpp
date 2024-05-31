#include "Client/State.h"

using namespace Client;

State::State(std::shared_ptr<Renderer::Base> renderer, std::shared_ptr<Platform::Base> sys, std::shared_ptr<Game::State> gameState, std::pair<uint32_t, std::shared_ptr<BaseState>> state) : renderer(renderer), sys(sys), gameState(gameState) {
    states[state.first] = state.second;
    currentState = state.second;

    std::any empty;
    currentState->onEnterState(*this, empty);
}

void State::changeState(const uint32_t newState, std::any enter, std::any leave) {
    currentState->onLeaveState(*this, leave);
    currentState = states[newState];
    currentState->onEnterState(*this, enter);
}

void State::render(const uint64_t time) {
    //renderer->start();
    currentState->onRender(*this, time);
    //renderer->finish();
}

void State::tick(const uint64_t time) {
    currentState->onTick(*this, time);
}

void State::mouseMove(const MouseMove &event) {
    currentState->onMouseMove(*this, event);
}

void State::mouseButtonPress(const MouseClick &event) {
    currentState->onMouseButtonPress(*this, event);
}

void State::mouseButtonRelease(const MouseClick &event) {
    currentState->onMouseButtonRelease(*this, event);
}

void State::mouseScroll(const MouseScroll &event) {
    currentState->onMouseScroll(*this, event);
}

void State::keyDown(const KeyPress &event) {
    currentState->onKeyDown(*this, event);
}

void State::keyUp(const KeyPress &event) {
    currentState->onKeyUp(*this, event);
}

void State::touchPress(const TouchEvent &event) {
    currentState->onTouchPress(*this, event);
}

void State::touchRelease(const TouchEvent &event) {
    currentState->onTouchRelease(*this, event);
}

void State::touchMotion(const TouchEvent &event) {
    currentState->onTouchMotion(*this, event);
}


State::~State() {

}
