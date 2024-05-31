#ifndef __CLIENT_CAMERA_H__
#define __CLIENT_CAMERA_H__

#include <sstream>

#include "Common/Shared.h"
#include "Math/Point3.h"
#include "Math/Plane.h"
#include "Math/Matrix.h"
#include "Client/Frustum.h"

namespace Client {
    class Camera {
        const Vec3F origin;
        const Vec3F angles;

    public:
        Camera(const Vec3F &origin, const Vec3F &angles) : origin(origin), angles(angles) {
        }

        MatrixF look() const;
        const Frustumf getFrustum(float ratio, float fov, float distance=1000.0) const;

        Vec3F Origin() const {
            return origin;
        }

        Vec3F Angles() const {
            return angles;
        }

        std::string toString() const;
    };
};

#endif //__CLIENT_CAMERA_H__
