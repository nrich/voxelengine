#include "Client/Camera.h"

using namespace Client;

MatrixF Camera::look() const {
    auto F = angles.Forward();
    auto R = angles.Right();
    auto U = angles.Up();

    MatrixF r(
        F, R, U
    );

    auto t = MatrixF(-origin);
    auto m = t * r;

    return m;
}

const FrustumF Camera::getFrustum(float ratio, float fov, float distance) const {
    auto F = angles.Forward();
    auto U = angles.Up();

    auto dest = origin + (F * 100);

    auto frustum = Frustum<float>(origin, dest, U, (float)1.0, distance, fov, ratio);

    return frustum;
}

std::string Camera::toString() const {
    std::ostringstream s;

    s << "[" << origin.toString() << angles.toString() << "]";

    return s.str();
}

