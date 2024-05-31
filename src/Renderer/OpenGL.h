#ifndef __RENDERER_OPENGL_H__
#define __RENDERER_OPENGL_H__

#include <cstdint>
#include <string>
#include <map>
#include <array>
#include <vector>

#include "Renderer/Base.h"

namespace Renderer {

class OpenGL : public Base {
    void setupWorldView();
public:
    OpenGL(const Common::DisplayMode &displayMode, int interfaceScale=2, int resolutionScale=1, Common::AspectRatio ratio=Common::AspectRatio::_16x9);

    void start();
    void finish();

    ~OpenGL();
};

}; // namespace Renderer

#endif //__RENDERER_OPENGL_H__

