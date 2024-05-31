#ifndef __CLIENT_FRUSTUM_H__
#define __CLIENT_FRUSTUM_H__

#include <sstream>

#include "Common/Shared.h"
#include "Math/Point3.h"
#include "Math/Plane.h"
#include "Math/Matrix.h"

namespace Client {
        using Math::Point3;
        using Math::Plane;
        using Math::Matrix;

        template <class T>struct Frustum {
            private:
                enum {
                    F_LEFT		= 0,
                    F_RIGHT		= 1,
                    F_TOP		= 2,
                    F_BOTTOM	        = 3,
                    F_NEAR		= 4,
                    F_FAR		= 5,

                    F_COUNT
                };

                Plane<T> viewPlanes[6];

            public:
                Frustum(const Point3<T> &p, const Point3<T> &l, const Point3<T> &u, T nearview, T farview, float fov, float ratio) {
                    T tang = (T)tan(DEG2RAD * fov * 0.5);

                    T nh = nearview * tang;
                    T nw = nh * ratio;

                    Point3<T> dir,nc,fc,X,Y,Z; 

                    // compute the Z axis of camera
                    // this axis points in the opposite direction from
                    // the looking direction
                    Z = ~(p - l);

                    // X axis of camera with given "up" vector and Z axis
                    X = ~(u ^ Z);

                    // the real "up" vector is the cross product of Z and X
                    Y = Z ^ X;

                    // compute the centers of the near and far planes
                    nc = p - Z * nearview;
                    fc = p - Z * farview;

                    viewPlanes[F_NEAR] = Plane<T>(-Z, nc);
                    viewPlanes[F_FAR] = Plane<T>(Z, fc);

                    Point3<T> aux, normal;

                    aux = ~((nc + Y*nh) - p);
                    normal = aux ^ X;

                    viewPlanes[F_TOP] = Plane<T>(normal, nc+Y*nh);

                    aux = ~((nc - Y*nh) - p);
                    normal = X ^ aux;
                    viewPlanes[F_BOTTOM] = Plane<T>(normal, nc-Y*nh);
                   
                    aux = ~((nc - X*nw) - p);
                    normal = aux ^ Y; 
                    viewPlanes[F_LEFT] = Plane<T>(normal, nc-X*nw);

                    aux = ~((nc + X*nw) - p);
                    normal = Y ^ aux; 
                    viewPlanes[F_RIGHT] = Plane<T>(normal, nc+X*nw);
                }

                bool isInside(const Point3<T> &point) const  {
                    for (int i = F_LEFT; i < F_COUNT; i++)
                        if (viewPlanes[i].distance(point) <= 0)
                            return false;

                    return true;
                }

                bool isInside(const Point3<T> &min, const Point3<T> &max) const {
                    T min_x = std::min(min.X(), max.X());
                    T min_y = std::min(min.Y(), max.Y());
                    T min_z = std::min(min.Z(), max.Z());

                    T max_x = std::max(min.X(), max.X());
                    T max_y = std::max(min.Y(), max.Y());
                    T max_z = std::max(min.Z(), max.Z());

                    T mid_x = (T)(max_x - min_x)/2.0 + min_x;
                    T mid_y = (T)(max_y - min_y)/2.0 + min_y;
                    T mid_z = (T)(max_z - min_z)/2.0 + min_z;

                    if (isInside(Point3<T>(min_x, min_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, min_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, max_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, max_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, min_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, min_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, max_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, max_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, mid_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, min_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, min_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, min_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, min_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, mid_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, mid_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, mid_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, mid_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, max_y, min_z)))
                        return true;

                    if (isInside(Point3<T>(mid_x, max_y, max_z)))
                        return true;

                    if (isInside(Point3<T>(min_x, max_y, mid_z)))
                        return true;

                    if (isInside(Point3<T>(max_x, max_y, mid_z)))
                        return true;

                    return false;
                }

                std::string toString() const {
                    std::ostringstream s;

                    s << "[";
                    for (int i = 0; i < 6; i++) {
                        auto v = viewPlanes[i];

                        s << v.toString();
                    }
                    s << "]";

                    return s.str();
                }

        };

};

typedef Client::Frustum<float> Frustumf;

#endif //__CLIENT_FRUSTUM_H__
