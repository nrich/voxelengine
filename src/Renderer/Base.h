#ifndef __RENDERER_BASE_H__
#define __RENDERER_BASE_H__

#include <cstdint>
#include <string>
#include <map>
#include <array>
#include <vector>

#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Matrix.h"
#include "Common/Colour.h"
#include "Common/DisplayMode.h"

namespace Renderer {

class Base {
    std::map<std::string, GLuint> textures;
    GLuint notexture;
    GLuint loadNoTexture();

    Vec3F cameraPostion;
    Vec3F cameraAngle;
protected:
    Common::DisplayMode displayMode;

    int virtualHeight;
    int verticalOffset;

    int virtualWidth;
    int horizontalOffset;

    float horizontalRatio;
    float verticalRatio;

    int interfaceScale;
    float interfaceRatio;

    int resolutionScale;

    const Common::AspectRatio ratio;

    void configureVirtualDisplay();

public:
    Base(const Common::DisplayMode &displayMode, int interfaceScale, int resolutionScale, Common::AspectRatio ratio) : displayMode(displayMode), interfaceScale(interfaceScale), resolutionScale(resolutionScale), ratio(ratio) {}

    virtual void start() {}
    virtual void finish() {}

    GLuint loadTexture(const std::string &filename);
    GLuint loadTexture(const std::string &filename, const Common::Colour &transparent);

    void changeDisplayMode(const Common::DisplayMode &displayMode);
    bool translatePoint(Dot &screen, const Dot &real);

    void CameraPosition(const Vec3F &position) {
        cameraPostion = position;
    }

    const Vec3F &CameraPosition() const {
        return cameraPostion;
    }

    void CameraAngle(const Vec3F &angle) {
        cameraAngle = angle;
    }

    const Vec3F &CameraAngle() const {
        return cameraAngle;
    }

    MatrixF screenTransformation(const float x, const float y, const float w=0.0f, const float h = 0.0f) const;

    virtual ~Base() {}
};

}; // namespace Renderer

#endif //__RENDERER_BASE_H__
