#ifndef __COMMON_SHARED_H__
#define __COMMON_SHARED_H__

#include <cassert>
#include <cstdint>
#include <string>
#include <cmath>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#define GLEW_STATIC
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include "SDL_opengl.h"
#else
#include <GL/glew.h>
#endif

#ifdef _WIN32
#include "mingw.future.h"
#include "mingw.thread.h"
#include "mingw.mutex.h"
#else
#include <future>
#include <thread>
#include <mutex>
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#ifndef DEG2RAD
#define DEG2RAD (M_PI/180.0)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f/M_PI)
#endif

#define mathPiDiv180 (M_PI/180)
#define mathDoublePi (M_PI*2)

#define DEF_EPSILON 0.00001

#define PITCH                           0
#define YAW                             1
#define ROLL                            2

typedef float vecf;
typedef vecf vec2f[2];
typedef vecf vec3f[3];
typedef vecf vec4f[4];

typedef int32_t dot;
typedef dot dot2[2];
typedef dot dot3[3];
typedef dot dot4[4];

#include <random>
typedef std::mt19937 RngT;

#define VOICE_COUNT 4

bool isPowerOfTwo(int x);
float degreesToRadians(const float degrees);
std::string randomString(size_t len, const std::string &charset="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_-+=,.?><{}[]|;:`~");

enum Actions {
    StepForward     = 1 << 0,
    StepBackward    = 1 << 1,
    StepLeft        = 1 << 2,
    StepRight       = 1 << 3,
    TurnLeft        = 1 << 4,
    TurnRight       = 1 << 5
};

enum GameClientStatus {
    OK = 0,
    UNKNOWN_CLIENT
};

struct ClientGameStateRequest {
    uint64_t id;
    int64_t client_id;
    uint32_t state;
    uint32_t timeDelta;
    vec3f origin;
    vec3f angles;
    vec2f mouse;
};

struct ClientGameStateResponse {
    uint64_t id;
    int32_t status;
    uint32_t state;
    vec3f new_origin;
    vec3f new_angles;
    vec3f velocity;
    uint64_t time;
};

struct ClientConnectRequest {
    uint64_t id;
    char username[64+1];
};

struct ClientConnectResponse {
    int32_t status;
    int64_t id;
    char secret[16+1];
};

struct ClientSessionRequest {
    uint64_t id;
};

struct ClientSessionResponse {
    char sessionUUID[16+1];
};

#define WORLD_ENTITY 0

#define MS_PER_UPDATE 16667
#define GAME_MS_PER_UPDATE (3*MS_PER_UPDATE)
#define GAME_TICKS GAME_MS_PER_UPDATE

#define DISPLAY_SCALE 1

#define WORLD_TARGET 1

#endif //__COMMON_SHARED_H__
