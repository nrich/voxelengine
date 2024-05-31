#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

#include <optional>

#include "Math/Point3.h"

namespace Math {

    template <class T>struct Trace {
        public:
            Point3<T> point;
            Point3<T> dir;
            float length;

            Trace(const Point3<T> &p, const Point3<T> &d, float l) : point(p), dir(d), length(l) {
            }

            Trace(const Trace<T> &trace) : point(trace.point), dir(trace.dir), length(trace.length) {
            }

            std::string toString() const {
                std::ostringstream s;

                s << "[" << point.toString() << dir.toString() << "," << length << "]";

                return s.str();
            }
    };

    template <class T>struct Plane {
        private:
            union {
                T p[4];

                struct {
                    T a;
                    T b;
                    T c;
                    T d;   // Distance to origin along normal
                };
            };

        public:
            Plane() {
                a = 0;
                b = 0;
                c = 0;
                d = 0;
            };

            Plane(const Plane<T> &plane) {
                a = plane.a;
                b = plane.b;
                c = plane.c;
                d = plane.d;
            }

            Plane(const Point3<T> &vec, T D) {  // Normal and distance to origin
                a = vec.x;
                b = vec.y;
                c = vec.z;
                d = D; 
            }

            Plane(const Point3<T> &normal, const Point3<T> &point) {  // Normal and point on Plane
                a = normal.X();
                b = normal.Y();
                c = normal.Z();

                d = -point % normal;
            }

            Plane(const Point3<T> &v1, const Point3<T> &v2, const Point3<T> &v3) {  // Through three points
                Point3<T> normal = ~(((v2 - v1) ^ (v3 - v1)));

                a = normal.X();
                b = normal.Y();
                c = normal.Z();

                d = -(v1 % normal);
            }

            Plane(T _a, T _b, T _c, T _d) {  // Plane equation 
                a = _a;
                b = _b;
                c = _c;
                d = _d;
            }

            Point3<T> N() const {
                return Point3<T>(a, b, c);
            }

            Point3<T> P() const {
                return Point3<T>(a, b, c) * -d;
            }

            T operator^(const Plane &plane) const { // Angle between Planes
                return acosf(fabsf(N() % plane.N())/(N().length() * plane.N().length()));
            }

            T distance(const Point3<T> &point) const {
                return (N() % point) + d;
            }

            bool isInside(const Point3<T> &point) const {
                return distance(point) > 0.0f ? true : false;
            }

            bool isOutside(const Point3<T> &point) const {
                return distance(point) < 0.0f ? true : false;
            }

            bool isOn(const Point3<T> &point) const {
                return fabs(distance(point)) < DEF_EPSILON ? true : false;
            }

            const T operator[](int index) const {
                return p[index];
            }

            const T operator()(int index) const {
                return p[index];
            }

            Plane<T> operator~() const {  // Normalize the Plane equation
                Plane<T> out;

                T l = N().length();

                out.a = a / l;
                out.b = b / l;
                out.c = c / l;
                out.d = d / l;

                return out;
            }

            /*
            Trace<T> intersection(const Point3<T> &src, const Point3<T> &dst) const {
                auto dir = ~(dst-src);
                auto n = N();

                T nsrc = n % src;
                T ndir = n % dir;

                auto point = src + (dir * ((d - nsrc)/ndir));

                if ((point - src).length() > (dst - src).length()) {
                    return Trace<T>(
                        point, 
                        ~dir,
                        (T)1.0
                    );  
                } else {
                    return Trace<T>(
                        point, 
                        ~dir,
                        (point - src).length()/(dst - src).length()
                    );
                }
            }
            */

            std::optional<Trace<T>> intersection(const Point3<T> &src, const Point3<T> &dst) const {
                auto v = ~(dst-src);

                auto p = src;
                auto n = N();

                T denom = n % v;

                if (fabs(denom) < DEF_EPSILON)
                    return std::nullopt;

                T t = -((n % p) + d) / (n % v);

                if (t < DEF_EPSILON)
                    return std::nullopt;

                auto point = p + v * t;

                if ((point - src).length() > (dst - src).length()) {
                    return Trace<T>(
                        point,
                        v,
                        (T)1.0
                    );
                } else {
                    return Trace<T>(
                        point,
                        v,
                        (point - src).length()/(dst - src).length()
                    );
                }
            }

            std::optional<Point3<T>> intersection(const Plane<T> &a, const Plane<T> &b) const {
                T denom;

                denom = N() % (a.N() ^ b.N());

                if (std::abs(denom) < DEF_EPSILON) {
                    return std::nullopt;
                }

                return ( (a.N() ^ b.N()) * -d - ( b.N() ^  N() ) * a.d - (N() ^ a.N()) * b.d ) / denom;
            }

            bool intersection(const Plane<T> &a, const Plane<T> &b, Point3<T> &v) {
                double denom;

                denom = N() % (a.N() ^ b.N());

                if (fabs(denom) < DEF_EPSILON) {
                    return false;
                }

                v = ( (a.N() ^ b.N()) * -d - ( b.N() ^  N() ) * a.d - (N() ^ a.N()) * b.d ) / denom;

                return true;
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << a << "," << b << "," << c << "," << d << ")";

                return s.str();
            }

    };
};

typedef Math::Trace<float> TraceF;
typedef Math::Trace<double> TraceD;
typedef Math::Plane<float> PlaneF;
typedef Math::Plane<double> PlaneD;

#endif //__MATH_PLANE_H__
