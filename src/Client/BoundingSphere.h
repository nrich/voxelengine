#ifndef __CLIENT_BOUNDINGSPHERE_H__
#define __CLIENT_BOUNDINGSPHERE_H__

#include "Common/Shared.h"
#include "Math/Point3.h"

namespace Client {
    using Math::Point3;

    template <class T>struct BoundingSphere {
        private:
            Point3<T> center;
            T radius;

        public:
            BoundingSphere(const Math::Point3<T> &center, const T radius) : center(center), radius(radius) {
            }

            const T Radius() const {
                return radius;
            }

            const Point3<T> Center() const {
                return center;
            }
    };
};

typedef Client::BoundingSphere<float> BoundingSphereF;

#endif //__CLIENT_BOUNDINGSPHERE_H__

