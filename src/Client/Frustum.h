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

/*
                bool isOutside(const BoundingBox<T> &box, const Plane<T> &plane) const {
                    return !(	
                            plane.isInside(box[0]) || 
                            plane.isInside(box[1]) ||
                            plane.isInside(box[2]) || 
                            plane.isInside(box[3]) ||
                            plane.isInside(box[4]) || 
                            plane.isInside(box[5]) ||
                            plane.isInside(box[6]) || 
                            plane.isInside(box[7])
                            );
                }
*/

            public:
                /*
                Frustum() {
                    extract();
                }
                */

                Frustum(const Point3<T> &p, const Point3<T> &l, const Point3<T> &u, T nearview, T farview, float fov, float ratio) {
                    T tang = (T)tan(DEG2RAD * fov * 0.5);

                    T nh = nearview * tang;
                    T nw = nh * ratio;

                    //T fh = far * tang;
                    //T fw = fh * ratio;

                    Point3<T> dir,nc,fc,X,Y,Z; 

                    // compute the Z axis of camera
                    // this axis points in the opposite direction from
                    // the looking direction
                    Z = ~(p - l);
                    //Z = ~(l - p);

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

/*
                bool isInside(const Common::Brush &brush) const {
                    if (isInside(brush.Min()) || isInside(brush.Max()) || isInside(brush.Centre()))
                        return true;

                    auto top = viewPlanes[F_TOP].distance(brush.Min()) <= 0 ? 1 : 0;
                    auto bottom = viewPlanes[F_BOTTOM].distance(brush.Min()) <= 0 ? 1 : 0;

                    auto left = viewPlanes[F_LEFT].distance(brush.Min()) <= 0 ? 1 : 0;
                    auto right = viewPlanes[F_RIGHT].distance(brush.Min()) <= 0 ? 1 : 0;

                    auto near = viewPlanes[F_NEAR].distance(brush.Min()) <= 0 ? 1 : 0;
                    auto far = viewPlanes[F_FAR].distance(brush.Min()) <= 0 ? 1 : 0;

                    if ((top != bottom || !top) && (left != right || !left) && (near != far || !near))
                        return true;

                    top = viewPlanes[F_TOP].distance(brush.Max()) <= 0 ? 1 : 0;
                    bottom = viewPlanes[F_BOTTOM].distance(brush.Max()) <= 0 ? 1 : 0;

                    left = viewPlanes[F_LEFT].distance(brush.Max()) <= 0 ? 1 : 0;
                    right = viewPlanes[F_RIGHT].distance(brush.Max()) <= 0 ? 1 : 0;

                    near = viewPlanes[F_NEAR].distance(brush.Max()) <= 0 ? 1 : 0;
                    far = viewPlanes[F_FAR].distance(brush.Max()) <= 0 ? 1 : 0;

                    if ((top != bottom || !top) && (left != right || !left) && (near != far || !near))
                        return true;


                    return false;
                }
*/

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
