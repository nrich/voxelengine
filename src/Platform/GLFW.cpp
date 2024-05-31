#include "Platform/GLFW.h"

#include "Common/Keys.h"

#include <algorithm>

#include <iostream>
#include <cstring>

static bool RepeatKeys = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Client::State *clientState = (Client::State *)glfwGetWindowUserPointer(window);
    Client::KeyPress event;

    event.shiftMod = mods & GLFW_MOD_SHIFT;
    event.ctrlMod = mods & GLFW_MOD_CONTROL;
    event.altMod = mods & GLFW_MOD_ALT;
    event.guiMod = mods & GLFW_MOD_SUPER;

    if (key == GLFW_KEY_ESCAPE) {
        event.keyCode = Common::Keys::Escape;
    } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
        event.keyCode = Common::Keys::Enter;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        event.keyCode = Common::Keys::LShift;
    } else if (key == GLFW_KEY_RIGHT_SHIFT) {
        event.keyCode = Common::Keys::RShift;
    } else if (key == GLFW_KEY_LEFT_ALT) {
        event.keyCode = Common::Keys::LAlt;
    } else if (key == GLFW_KEY_RIGHT_ALT) {
        event.keyCode = Common::Keys::RAlt;
    } else if (key == GLFW_KEY_LEFT_CONTROL) {
        event.keyCode = Common::Keys::LControl;
    } else if (key == GLFW_KEY_RIGHT_CONTROL) {
        event.keyCode = Common::Keys::RControl;
    } else if (key == GLFW_KEY_LEFT_SUPER) {
        event.keyCode = Common::Keys::LSystem;
    } else if (key == GLFW_KEY_RIGHT_SUPER) {
        event.keyCode = Common::Keys::RSystem;
    } else if (key == GLFW_KEY_MENU) {
        event.keyCode = Common::Keys::Menu;
    } else if (key == GLFW_KEY_TAB) {
        event.keyCode = Common::Keys::Tab;
    } else if (key == GLFW_KEY_SPACE) {
        event.keyCode = Common::Keys::Space;
    } else if (key == GLFW_KEY_BACKSPACE) {
        event.keyCode = Common::Keys::Backspace;
    } else if (key == GLFW_KEY_APOSTROPHE) {
        event.keyCode = Common::Keys::Quote;
    } else if (key == GLFW_KEY_COMMA) {
        event.keyCode = Common::Keys::Comma;
    } else if (key == GLFW_KEY_MINUS) {
        event.keyCode = Common::Keys::Hyphen;
    } else if (key == GLFW_KEY_PERIOD) {
        event.keyCode = Common::Keys::Period;
    } else if (key == GLFW_KEY_SLASH) {
        event.keyCode = Common::Keys::Slash;
    } else if (key == GLFW_KEY_SEMICOLON) {
        event.keyCode = Common::Keys::Semicolon;
    } else if (key == GLFW_KEY_EQUAL) {
        event.keyCode = Common::Keys::Equal;
    } else if (key == GLFW_KEY_LEFT_BRACKET) {
        event.keyCode = Common::Keys::LBracket;
    } else if (key == GLFW_KEY_RIGHT_BRACKET) {
        event.keyCode = Common::Keys::RBracket;
    } else if (key == GLFW_KEY_GRAVE_ACCENT) {
        event.keyCode = Common::Keys::Backquote;
    } else if (key == GLFW_KEY_BACKSLASH) {
        event.keyCode = Common::Keys::Backslash;
    } else if (key == GLFW_KEY_A) {
        event.keyCode = Common::Keys::A;
    } else if (key == GLFW_KEY_B) {
        event.keyCode = Common::Keys::B;
    } else if (key == GLFW_KEY_C) {
        event.keyCode = Common::Keys::C;
    } else if (key == GLFW_KEY_D) {
        event.keyCode = Common::Keys::D;
    } else if (key == GLFW_KEY_E) {
        event.keyCode = Common::Keys::E;
    } else if (key == GLFW_KEY_F) {
        event.keyCode = Common::Keys::F;
    } else if (key == GLFW_KEY_G) {
        event.keyCode = Common::Keys::G;
    } else if (key == GLFW_KEY_H) {
        event.keyCode = Common::Keys::H;
    } else if (key == GLFW_KEY_I) {
        event.keyCode = Common::Keys::I;
    } else if (key == GLFW_KEY_J) {
        event.keyCode = Common::Keys::J;
    } else if (key == GLFW_KEY_K) {
        event.keyCode = Common::Keys::K;
    } else if (key == GLFW_KEY_L) {
        event.keyCode = Common::Keys::L;
    } else if (key == GLFW_KEY_M) {
        event.keyCode = Common::Keys::M;
    } else if (key == GLFW_KEY_N) {
        event.keyCode = Common::Keys::N;
    } else if (key == GLFW_KEY_O) {
        event.keyCode = Common::Keys::O;
    } else if (key == GLFW_KEY_P) {
        event.keyCode = Common::Keys::P;
    } else if (key == GLFW_KEY_Q) {
        event.keyCode = Common::Keys::Q;
    } else if (key == GLFW_KEY_R) {
        event.keyCode = Common::Keys::R;
    } else if (key == GLFW_KEY_S) {
        event.keyCode = Common::Keys::S;
    } else if (key == GLFW_KEY_T) {
        event.keyCode = Common::Keys::T;
    } else if (key == GLFW_KEY_U) {
        event.keyCode = Common::Keys::U;
    } else if (key == GLFW_KEY_V) {
        event.keyCode = Common::Keys::V;
    } else if (key == GLFW_KEY_W) {
        event.keyCode = Common::Keys::W;
    } else if (key == GLFW_KEY_X) {
        event.keyCode = Common::Keys::X;
    } else if (key == GLFW_KEY_Y) {
        event.keyCode = Common::Keys::Y;
    } else if (key == GLFW_KEY_Z) {
        event.keyCode = Common::Keys::Z;
    } else if (key == GLFW_KEY_1) {
        event.keyCode = Common::Keys::Num1;
    } else if (key == GLFW_KEY_2) {
        event.keyCode = Common::Keys::Num2;
    } else if (key == GLFW_KEY_3) {
        event.keyCode = Common::Keys::Num3;
    } else if (key == GLFW_KEY_4) {
        event.keyCode = Common::Keys::Num4;
    } else if (key == GLFW_KEY_5) {
        event.keyCode = Common::Keys::Num5;
    } else if (key == GLFW_KEY_6) {
        event.keyCode = Common::Keys::Num6;
    } else if (key == GLFW_KEY_7) {
        event.keyCode = Common::Keys::Num7;
    } else if (key == GLFW_KEY_8) {
        event.keyCode = Common::Keys::Num8;
    } else if (key == GLFW_KEY_9) {
        event.keyCode = Common::Keys::Num9;
    } else if (key == GLFW_KEY_0) {
        event.keyCode = Common::Keys::Num0;
    } else if (key == GLFW_KEY_KP_1) {
        event.keyCode = Common::Keys::Numpad1;
    } else if (key == GLFW_KEY_KP_2) {
        event.keyCode = Common::Keys::Numpad2;
    } else if (key == GLFW_KEY_KP_3) {
        event.keyCode = Common::Keys::Numpad3;
    } else if (key == GLFW_KEY_KP_4) {
        event.keyCode = Common::Keys::Numpad4;
    } else if (key == GLFW_KEY_KP_5) {
        event.keyCode = Common::Keys::Numpad5;
    } else if (key == GLFW_KEY_KP_6) {
        event.keyCode = Common::Keys::Numpad6;
    } else if (key == GLFW_KEY_KP_7) {
        event.keyCode = Common::Keys::Numpad7;
    } else if (key == GLFW_KEY_KP_8) {
        event.keyCode = Common::Keys::Numpad8;
    } else if (key == GLFW_KEY_KP_9) {
        event.keyCode = Common::Keys::Numpad9;
    } else if (key == GLFW_KEY_KP_0) {
        event.keyCode = Common::Keys::Numpad0;
    } else if (key == GLFW_KEY_F1) {
        event.keyCode = Common::Keys::F1;
    } else if (key == GLFW_KEY_F2) {
        event.keyCode = Common::Keys::F2;
    } else if (key == GLFW_KEY_F3) {
        event.keyCode = Common::Keys::F3;
    } else if (key == GLFW_KEY_F4) {
        event.keyCode = Common::Keys::F4;
    } else if (key == GLFW_KEY_F5) {
        event.keyCode = Common::Keys::F5;
    } else if (key == GLFW_KEY_F6) {
        event.keyCode = Common::Keys::F6;
    } else if (key == GLFW_KEY_F7) {
        event.keyCode = Common::Keys::F7;
    } else if (key == GLFW_KEY_F8) {
        event.keyCode = Common::Keys::F8;
    } else if (key == GLFW_KEY_F9) {
        event.keyCode = Common::Keys::F9;
    } else if (key == GLFW_KEY_F10) {
        event.keyCode = Common::Keys::F10;
    } else if (key == GLFW_KEY_F11) {
        event.keyCode = Common::Keys::F11;
    } else if (key == GLFW_KEY_F12) {
        event.keyCode = Common::Keys::F12;
    } else if (key == GLFW_KEY_F13) {
        event.keyCode = Common::Keys::F13;
    } else if (key == GLFW_KEY_F14) {
        event.keyCode = Common::Keys::F14;
    } else if (key == GLFW_KEY_F15) {
        event.keyCode = Common::Keys::F15;
    } else if (key == GLFW_KEY_LEFT) {
        event.keyCode = Common::Keys::Left;
    } else if (key == GLFW_KEY_RIGHT) {
        event.keyCode = Common::Keys::Right;
    } else if (key == GLFW_KEY_UP) {
        event.keyCode = Common::Keys::Up;
    } else if (key == GLFW_KEY_DOWN) {
        event.keyCode = Common::Keys::Down;
    } else if (key == GLFW_KEY_PAUSE) {
        event.keyCode = Common::Keys::Pause;
    } else if (key == GLFW_KEY_PAGE_UP) {
        event.keyCode = Common::Keys::PageUp;
    } else if (key == GLFW_KEY_PAGE_DOWN) {
        event.keyCode = Common::Keys::PageDown;
    } else if (key == GLFW_KEY_END) {
        event.keyCode = Common::Keys::End;
    } else if (key == GLFW_KEY_HOME) {
        event.keyCode = Common::Keys::Home;
    } else if (key == GLFW_KEY_INSERT) {
        event.keyCode = Common::Keys::Insert;
    } else if (key == GLFW_KEY_DELETE) {
        event.keyCode = Common::Keys::Delete;
    } else if (key == GLFW_KEY_KP_DIVIDE) {
        event.keyCode = Common::Keys::Divide;
    } else if (key == GLFW_KEY_KP_MULTIPLY) {
        event.keyCode = Common::Keys::Multiply;
    } else if (key == GLFW_KEY_KP_ADD) {
        event.keyCode = Common::Keys::Add;
    } else if (key == GLFW_KEY_KP_SUBTRACT) {
        event.keyCode = Common::Keys::Subtract;
    }

    if (action == GLFW_PRESS) {
        clientState->keyDown(event);
    } else if (action == GLFW_REPEAT && RepeatKeys) {
        clientState->keyDown(event);
    } else if (action == GLFW_RELEASE) {
        clientState->keyUp(event);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Client::State *clientState = (Client::State *)glfwGetWindowUserPointer(window);
    Client::MouseClick event;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    event.x = (int32_t)xpos;
    event.y = (int32_t)ypos;

    event.leftPressed = button == GLFW_MOUSE_BUTTON_LEFT;
    event.middlePressed = button == GLFW_MOUSE_BUTTON_MIDDLE;
    event.rightPressed = button == GLFW_MOUSE_BUTTON_RIGHT;
    event.x1Pressed = button == GLFW_MOUSE_BUTTON_4;
    event.x2Pressed = button == GLFW_MOUSE_BUTTON_5;

    if (action == GLFW_PRESS) {
        clientState->mouseButtonPress(event);
    } else if (action == GLFW_RELEASE) {
        clientState->mouseButtonRelease(event);
    }
}

static void mouse_scroll_callback(GLFWwindow* window, double xpos, double ypos) {
    Client::State *clientState = (Client::State *)glfwGetWindowUserPointer(window);
    Client::MouseScroll event;

    event.x = (int32_t)xpos;
    event.y = (int32_t)ypos;

    clientState->mouseScroll(event);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Client::State *clientState = (Client::State *)glfwGetWindowUserPointer(window);
    Client::MouseMove event;

    static double xrel = 0;
    static double yrel = 0;

    event.x = (int32_t)xpos;
    event.y = (int32_t)ypos;
    event.xrel = (int32_t)xpos - xrel;
    event.yrel = (int32_t)ypos - yrel;

    xrel = xpos;
    yrel = ypos;

    clientState->mouseMove(event);
}


Common::DisplayMode Platform::GLFW::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    auto current = currentDisplayMode();

    if (displayMode == current && fullscreen == isFullScreen())
        return displayMode;

    glfwSetWindowSize(window.get(), displayMode.Width(), displayMode.Height());

    if (fullscreen && !isFullScreen()) {
        GLFWmonitor *primary = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(window.get(), primary, 0, 0, displayMode.Width(), displayMode.Height(), 0);
    } else if (isFullScreen() && !fullscreen) {
        glfwSetWindowMonitor(window.get(), nullptr, 0, 0, displayMode.Width(), displayMode.Height(), 0);
    }

    return displayMode;
}

std::vector<Common::DisplayMode> Platform::GLFW::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;

    GLFWmonitor *primary = glfwGetPrimaryMonitor();

    int count;
    const GLFWvidmode *vidmodes = glfwGetVideoModes(primary, &count);
    const GLFWvidmode *current = glfwGetVideoMode(primary);

    for (int i = 0; i < count; i++) {
        auto mode = vidmodes[i];

        Common::DisplayMode displayMode(mode.width, mode.height, mode.refreshRate);

        if (displayMode.Refresh() != current->refreshRate)
            continue;

        if (displayMode.Width() < 1280)
            continue;

//        if (displayMode.Ratio() != Common::AspectRatio::_4x3)
//            continue;

        if (displayMode.Ratio() != Common::AspectRatio::Ignore) {
            modes.push_back(displayMode);
        }
    }

    if (modes.front().Width() > modes.back().Width()) {
        std::reverse(modes.begin(), modes.end());
    }
    return modes;
}

Common::DisplayMode Platform::GLFW::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();

    for (auto mode : modeList) {
        if (mode.Width() == width && mode.Height() == height) {
            return mode;
        }
    }

    return modeList[0];
}

Common::DisplayMode Platform::GLFW::currentDisplayMode() const {
    int width, height;
    glfwGetFramebufferSize(window.get(), &width, &height);
    return findDisplayMode(width, height);
}

std::pair<Common::DisplayMode, Common::DisplayMode> Platform::GLFW::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
    auto modeList = getDisplayModes();
    Common::DisplayMode previous = displayMode;
    Common::DisplayMode next = displayMode;

    size_t i = 0;
    for (auto mode : modeList) {
        if (mode == displayMode)
            break;

        i++;
    }

    if (i > 0)
        previous = modeList[i-1];
    if (i != modeList.size())
        next = modeList[i+1];

    return std::pair<Common::DisplayMode, Common::DisplayMode>(previous, next);
}

bool Platform::GLFW::isFullScreen() const {
    return glfwGetWindowMonitor(window.get()) != nullptr;
}

bool Platform::GLFW::handleEvents(std::shared_ptr<Client::State> clientState) {
    glfwSetWindowUserPointer(window.get(), clientState.get());
    glfwPollEvents();
    return !glfwWindowShouldClose(window.get());
}

void Platform::GLFW::keyRepeat(bool enable) {
    RepeatKeys = enable;
}

void Platform::GLFW::sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) {
    voices[voice].tone(frequency, duration, waveForm, volume, attack, decay, sustain, release);
}

void Platform::GLFW::setSwapInterval(int32_t interval) {
    glfwSwapInterval(interval);

    int code = glfwGetError(NULL);

    if (code == GLFW_NO_ERROR) {
        vsync = !!interval;
    }
}

static int tonecallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    Audio::Tone *voices = (Audio::Tone *)userData;

    float *out = (float *)outputBuffer;
    std::vector<float> buffer;
    buffer.resize(framesPerBuffer, 0.0f);

    voices[0].generateSamples(buffer.data(), framesPerBuffer, 0.25);
    voices[1].generateSamples(buffer.data(), framesPerBuffer, 0.25);
    voices[2].generateSamples(buffer.data(), framesPerBuffer, 0.25);
    voices[3].generateSamples(buffer.data(), framesPerBuffer, 0.25);

    std::memcpy(out, buffer.data(), sizeof(float) * framesPerBuffer);

    return 0;
}

Platform::GLFW::GLFW(const std::string &title, std::pair<uint8_t, uint8_t> glversion) : vsync(false) {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glversion.first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glversion.second);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto mode = getDisplayModes().front();

    for (int i = 0; i < VOICE_COUNT; i++)
        voices[i] = Audio::Tone();

    window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(mode.Width(), mode.Height(), (std::string("GLFW ") + title + std::string(" v") + std::string(VERSION)).c_str(), NULL, NULL),
        glfwDestroyWindow
    );

    glfwMakeContextCurrent(window.get());

    glfwSwapInterval(0);

    glfwSetKeyCallback(window.get(), key_callback);
    glfwSetMouseButtonCallback(window.get(), mouse_button_callback);
    glfwSetScrollCallback(window.get(), mouse_scroll_callback);
    glfwSetCursorPosCallback(window.get(), cursor_position_callback);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (Pa_Initialize() == paNoError && Pa_OpenDefaultStream(
        &stream,
        0,          // no input channels
        1,          // mono output
        paFloat32,
        Audio::FREQUENCY,
        256,        // frames per buffer
        tonecallback,
        voices.data()
    ) == paNoError && Pa_StartStream(stream) == paNoError);

    int major = glfwGetWindowAttrib(window.get(), GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(window.get(), GLFW_CONTEXT_VERSION_MINOR);

    std::cout << "version:" << major << "." << minor << std::endl;
}

Platform::GLFW::~GLFW() {
    Pa_CloseStream(stream);
    Pa_Terminate();
    glfwTerminate();
}
