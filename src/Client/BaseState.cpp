#include "Client/BaseState.h"

#include "Client/State.h" 

using namespace Client;

void BaseState::onRender(State &state, const uint64_t time) {

}

void BaseState::onTick(State &state, const uint64_t time) {

}

void BaseState::onMouseMove(State &state, const MouseMove &event) {

}

void BaseState::onMouseButtonPress(State &state, const MouseClick &event) {

}

void BaseState::onMouseButtonRelease(State &state, const MouseClick &event) {

}

void BaseState::onMouseScroll(State &state, const MouseScroll &event) {

}

void BaseState::onKeyDown(State &state, const KeyPress &event) {

}

void BaseState::onKeyUp(State &state, const KeyPress &event) {

}

void BaseState::onTouchPress(State &state, const TouchEvent &event) {

}

void BaseState::onTouchRelease(State &state, const TouchEvent &event) {

}

void BaseState::onTouchMotion(State &state, const TouchEvent &event) {

}

void BaseState::onEnterState(State &state, std::any data) {

}

void BaseState::onLeaveState(State &state, std::any data) {

}

void BaseState::changeDisplayMode(const Common::DisplayMode &displayMode) {

}

BaseState::~BaseState() {
}

