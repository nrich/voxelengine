#include <cstdlib>
#include <memory>
#include <functional>
#include <map>
#include <tuple>

#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <gl4esinit.h>
#endif

#ifndef __EMSCRIPTEN__
#include "ezOptionParser.hpp"
#endif

#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include "Client/State.h"

#ifdef __EMSCRIPTEN__
#include <gl4esinit.h>
#include "Renderer/OpenGL.h"
#else
#include "Renderer/OpenGL.h"
#endif

#include "Common/DisplayMode.h"

#if MINBUILD
#else
#include "Platform/SDL2.h"
#endif

#ifndef __EMSCRIPTEN__
#include "Platform/GLFW.h"
#endif

#include "Client/VoxelState.h"

#include "Game/State.h"

#define APPNAME "Voxel Engine"

#define GAME_MS_PER_UPDATE (3*MS_PER_UPDATE)

void game_loop(std::shared_ptr<Platform::Base> platform, std::shared_ptr<Client::State> clientState, std::shared_ptr<Game::State> gameState) {
    uint64_t previous = platform->getTicks();
    uint64_t lag = 0;
    uint64_t game_lag = 0;

    while (1) {
        uint64_t current = platform->getTicks();
        uint64_t elapsed = current - previous;

        previous = current;

        lag += elapsed;
        game_lag += elapsed;

        if (!platform->handleEvents(clientState))
            break;

        while (lag >= MS_PER_UPDATE) {
            clientState->tick(MS_PER_UPDATE);
            lag -= MS_PER_UPDATE;
        }

        while (game_lag >= GAME_MS_PER_UPDATE) {
            gameState->tick(GAME_MS_PER_UPDATE);
            game_lag -= GAME_MS_PER_UPDATE;
        }

        platform->clearScreen();
        clientState->render(elapsed);
        platform->swapBuffers();
    }
}

#if __EMSCRIPTEN__
void emscripten_loop(void *userdata) {
    auto *args = static_cast<std::tuple<std::shared_ptr<Platform::Base>, std::shared_ptr<Client::State>, std::shared_ptr<Game::State>> *>(userdata);

    auto platform = std::get<0>(*args);
    auto clientState = std::get<1>(*args);
    auto gameState = std::get<2>(*args);

    game_loop(platform, clientState, gameState);
}
#endif

int main(int argc, char **argv) {
    int vsync = 0;
    bool debug = false;

    auto glVersion = std::make_pair(3, 3);

#ifndef __EMSCRIPTEN__
    ez::ezOptionParser opt;

    opt.overview = "engine";
    opt.syntax = std::string(argv[0]) + " [OPTIONS] [runfile]\n";
    opt.footer = std::string(argv[0]) + " v" + std::string(VERSION) + "\n";

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Display usage instructions.", // Help description.
        "-h",     // Flag token.
        "-help",  // Flag token.
        "--help", // Flag token.
        "--usage" // Flag token.
    );

    opt.add(
#ifdef _WIN32
#if MINBUILD
        "glfw", // Default.
#else
        "sdl2", // Default.
#endif
#else
        "sdl2", // Default.
#endif
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Display usage instructions.", // Help description.
        "-s",     // Flag token.
        "-sys",  // Flag token.
        "--sys" // Flag token.
    );

    opt.add(
        "0", // Default.
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "VSync mode", // Help description.
        "-v",     // Flag token.
        "-vsync",   // Flag token.
        "--vsync"  // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Legacy renderer", // Help description.
        "-l",     // Flag token.
        "-legacy",   // Flag token.
        "--legacy"  // Flag token.
    );

    opt.parse(argc, (const char**)argv);
    if (opt.isSet("-h")) {
        std::string usage;
        opt.getUsage(usage);
        std::cout << usage << std::endl;
        exit(1);
    }
#endif

#ifdef __EMSCRIPTEN__
    initialize_gl4es();
#endif

    std::shared_ptr<Renderer::Base> renderer;
    std::shared_ptr<Platform::Base> platform;

#ifndef __EMSCRIPTEN__
    std::string platname;

    opt.get("-s")->getString(platname);

    if (opt.isSet("-l")) {
        glVersion = std::make_pair(2, 0);
    }

    if (platname == "glfw") {
        platform = std::make_shared<Platform::GLFW>(APPNAME, glVersion);
#if MINBUILD
#else
    } else if (platname == "sdl2") {
        platform = std::make_shared<Platform::SDL2>(APPNAME, glVersion);
#endif
    } else {
        std::cerr << "Unknown platform " << platname << std::endl;
        exit(0);
    }

    debug = opt.isSet("-d");
    opt.get("-v")->getInt(vsync);

    std::cerr << "GL version: " << glGetString(GL_VERSION) << std::endl <<  std::endl;

    glewExperimental = GL_TRUE;
    GLenum res = glewInit();

    if (res != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        exit(-1);
    }

    renderer = std::make_shared<Renderer::OpenGL>(platform->currentDisplayMode(), 1, 2);
#else
    platform = std::make_shared<Platform::SDL2>(APPNAME, glVersion);
    renderer = std::make_shared<Renderer::OpenGL>(platform->currentDisplayMode(), 1, 2);
#endif

    platform->keyRepeat(false);

    platform->setSwapInterval(vsync);

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    RngT rng(seed);

    uint64_t world_seed = std::uniform_int_distribution<uint64_t>()(rng);
    Common::World world(world_seed);

    auto gameState = std::make_shared<Game::State>(
        &world
    );

    auto voxelState = std::make_shared<Client::VoxelState>(&world);

    auto clientState = std::make_shared<Client::State>(
        std::dynamic_pointer_cast<Renderer::Base>(renderer),
        std::dynamic_pointer_cast<Platform::Base>(platform),
        gameState,
        std::pair<uint32_t, std::shared_ptr<Client::BaseState>>(0, std::dynamic_pointer_cast<Client::BaseState>(voxelState))
    );

#if __EMSCRIPTEN__
    auto args = std::make_tuple(platform, clientState, gameState);
    emscripten_set_main_loop_arg(emscripten_loop, (void *)&args, -1, 1);
#else
    game_loop(platform, clientState, gameState);
#endif

    (void)debug;
    exit(0);
}

