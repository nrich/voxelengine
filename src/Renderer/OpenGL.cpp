#include "Renderer/OpenGL.h"

using namespace Renderer;

OpenGL::OpenGL(const Common::DisplayMode &displayMode, int interfaceScale, int resolutionScale, Common::AspectRatio ratio) : Base(displayMode, interfaceScale, resolutionScale, ratio) {
    if (!isPowerOfTwo(interfaceScale)) {
        std::cerr << "Invalid interface scale" << std::endl;
        exit(-1);
    }

    configureVirtualDisplay();
    setupWorldView();
}

void OpenGL::setupWorldView() {
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#if 1
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    /* shading model */
//    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);

    /* Culling. */
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

#endif
    float xratio = (float)displayMode.Width() / (float)(virtualWidth);
    float yratio = (float)displayMode.Height() / (float)(virtualHeight);

    /* viewport */
    glViewport(horizontalOffset * xratio, verticalOffset * yratio, (virtualWidth-2*horizontalOffset) * xratio, (virtualHeight-2*verticalOffset) * yratio);
}

void OpenGL::start() {

}

void OpenGL::finish() {

}

OpenGL::~OpenGL() {

}

