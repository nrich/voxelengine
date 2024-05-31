#ifndef __MATH_POINT3_H__
#define __MATH_POINT3_H__

#include <string>
#include <sstream>

#include "Math/Point2.h"
#include "Common/Shared.h"

namespace Math {
    template <class T>struct Point3 {
        private:
            union {
                T vec[3];

                struct {
                    T x;
                    T y;
                    T z;
                };
            };

        public:
            Point3<T>() {
                x = 0;
                y = 0;
                z = 0;
            }

            Point3<T>(T _x, T _y, T _z) {
                x = _x;
                y = _y;
                z = _z;
            }

            Point3<T>(const T v[3]) {
                x = v[0];
                y = v[1];
                z = v[2];
            }

            Point3<T>(const Point3<T> &V) {
                x = V.x;
                y = V.y;
                z = V.z;
            }

            //Accessors
            T X() const {
                return x;
            }

            T Y() const {
                return y;
            }

            T Z() const {
                return z;
            }

            //maths functions

            //Add
            const Point3<T> operator+(const Point3<T> &add) const {
                return Point3<T>((x+add.x), (y+add.y), (z+add.z));
            }

            //Subtract
            const Point3<T> operator-(const Point3<T> &sub) const {
                return Point3<T>((x-sub.x), (y-sub.y), (z-sub.z));
            }

            //Dot Product
            const T operator%(const Point3<T> &v) const {
                return (x*v.x + y*v.y + z*v.z);
            }

            //Cross Product
            const Point3<T> operator^(const Point3<T> &v) const {
                return Point3<T>(	
                        y*v.z - z*v.y, 
                        z*v.x - x*v.z, 
                        x*v.y - y*v.x
                        );
            }

            const T *data() const {
                return vec;
            }

            const T length() const {
                return sqrt(x*x + y*y + z*z);
            }

            const Point3<T> operator*(T mult) const {
                return Point3<T>(x*mult, y*mult, z*mult);
            }


            const Point3<T> operator!() const {
                return Point3<T>(-x, -y, -z);
            }

            const Point3<T> operator/(T div) const {
                return Point3<T>(x/div, y/div, z/div);
            }

            // TODO non-const operator
            // postentially dangerous
            T &operator[](int index) {
                return vec[index];
            }

            T operator[](int index) const {
                return vec[index];
            }

            T operator()(int index) const {
                return vec[index];
            }

            const Point3<T> operator-() const {
                return Point3<T>(-x, -y, -z);
            }

            //Comparison
            const bool operator==(const Point3<T> &V) const {
                if (x != V.x)
                    return false;
                if (y != V.y)
                    return false;
                if (z != V.z)
                    return false;

                return true;
            }

            const bool operator!=(const Point3<T> &V) const {
                if (x != V.x)
                    return true;
                if (y != V.y)
                    return true;
                if (z != V.z)
                    return true;

                return false;
            }

            const bool operator>(const Point3<T> &V) const {
                if (x <= V.x)
                    return false;
                if (y <= V.y)
                    return false;
                if (z <= V.z)
                    return false;

                return true;
            }

            const bool operator<(const Point3<T> &V) const {
                if (x >= V.x)
                    return false;
                if (y >= V.y)
                    return false;
                if (z >= V.z)
                    return false;

                return true;
            }

            const bool operator>=(const Point3<T> &V) const {
                if (x < V.x)
                    return false;
                if (y < V.y)
                    return false;
                if (z < V.z)
                    return false;

                return true;
            }

            const bool operator<=(const Point3<T> &V) const {
                if (x > V.x)
                    return false;
                if (y > V.y)
                    return false;
                if (z > V.z)
                    return false;

                return true;
            }

            const Point3<T> operator~() const {
                return this->normal();
            }

            const Point3<T> operator|(const Point3<T> &V) const {
                return Point3<T>(
                    (vec[1]*V.vec[2])-(vec[2]*V.vec[1]),
                    (vec[2]*V.vec[0])-(vec[0]*V.vec[2]),
                    (vec[0]*V.vec[1])-(vec[1]*V.vec[0])
                );
            }

            const Point3<T> normal() const {
                T len = length();

                if (len) {
                    len = 1 / len;

                    T lx = x * len;
                    T ly = y * len;
                    T lz = z * len;

                    return Point3<T>(lx, ly, lz);
                } else {
                    return (*this);
                }
            }


            const Point3<T> clamp(T min = 0, T max = 360) const {
                T _x = x > max ? x - max : x;
                _x = _x < min ? _x + max : _x;

                T _y = y > max ? y - max : y;
                _y = _y < min ? _y + max : _y;

                T _z = z > max ? z - max : z;
                _z = _z < min ? _z + max : _z;

                return Point3<T>(_x, _y, _z);
            }

            const Point3<T> project(const Point3<T> &dir, T scale) const {
                return Point3<T>(x + dir.x*scale, y + dir.y*scale, z + dir.z*scale);
            }

            void copy(T out[3]) const {
                out[0] = x;
                out[1] = y;
                out[2] = z;
            }

            const T distanceFrom(const Point3<T> Vec) const {
                return sqrt(
                    (Vec.x - x) * (Vec.x - x) + (Vec.y - y) * (Vec.y - y) +(Vec.z - z) * (Vec.z - z)    
                );
            }

            const Point3<T> Forward() const {
                T angle = (T)(vec[YAW] * mathPiDiv180);
                T sy = (T)sin(angle);
                T cy = (T)cos(angle);
                angle = (T)(vec[PITCH] * mathPiDiv180);
                T sp = (T)sin(angle);
                T cp = (T)cos(angle);
                angle = (T)(vec[ROLL] * mathPiDiv180);
                T sr = (T)sin(angle);
                T cr = (T)cos(angle);

                return Point3<T>(-1*sr*sp*cy+-1*cr*-sy, -1*sr*cp, -1*sr*sp*sy+-1*cr*cy);
            }

            const Point3<T> Right() const {
                T angle = (T)(vec[YAW] * mathPiDiv180);
                T sy = (T)sin(angle);
                T cy = (T)cos(angle);
                angle = (T)(vec[PITCH] * mathPiDiv180);
                T sp = (T)sin(angle);
                T cp = (T)cos(angle);
                angle = (T)(vec[ROLL] * mathPiDiv180);

                return Point3<T>(cp*cy, -sp, cp*sy);
            }

            const Point3<T> Up() const {
                T angle = (T)(vec[YAW] * mathPiDiv180);
                T sy = (T)sin(angle);
                T cy = (T)cos(angle);
                angle = (T)(vec[PITCH] * mathPiDiv180);
                T sp = (T)sin(angle);
                T cp = (T)cos(angle);
                angle = (T)(vec[ROLL] * mathPiDiv180);
                T sr = (T)sin(angle);
                T cr = (T)cos(angle);

                return Point3<T>(cr*sp*cy+-sr*-sy, cr*cp, cr*sp*sy+-sr*cy);
            }


            const T toYaw() const {
                T yaw;

                if (/*vec[YAW] == 0 &&*/ vec[PITCH] == 0) {
                    yaw = 0;
                    if (vec[YAW] > 0)
                        yaw = 90;
                    else if (vec[YAW] < 0)
                        yaw = -90;
                } else {
                    yaw = (int) (atan2(vec[YAW], vec[PITCH]) * mathPiDiv180);
                    if (yaw < 0)
                        yaw += 360;
                }

                return yaw;
            }

            const Point3<T> toRadians() const {
                return ((*this) / 180) * mathDoublePi;
            }

#if 1
            const Point3<T> toAngle() const {
                T forward;
                T yaw, pitch;

                if (vec[1] == 0 && vec[0] == 0) {
                    yaw = 0;
                    if (vec[2] > 0)
                        pitch = 90;
                    else
                        pitch = 270;
                } else {
                    if (vec[0])
                        yaw = (int) (atan2(vec[1], vec[0]) * mathPiDiv180);
                    else if (vec[1] > 0)
                        yaw = 90;
                    else
                        yaw = -90;

                    if (yaw < 0)
                        yaw += 360;

                    forward = sqrt(vec[0]*vec[0] + vec[1]*vec[1]);
                    pitch = (int) (atan2(vec[2], forward) * mathPiDiv180);
                    if (pitch < 0)
                        pitch += 360;
                }

                return Point3<T>(-pitch, yaw, 0);
            }
#else
            const Point3<T> toAngle() const {
                T pitch, yaw, roll = (T)0;

#if 0
                pitch = asin(y / length());
                yaw = asin(x / (cos(pitch) * length()));
#else
                Point3<T> d = normal();

                pitch = asin(-d.Y());
                yaw = atan2(d.X(), d.Z());
#endif

                return Point3<T>(pitch, yaw, roll);
            }

#endif

            bool sameDir(const Point3<T> &other) const {
                if (x < 0 && other.x > 0)
                    return false;
                if (y < 0 && other.y > 0)
                    return false;
                if (z < 0 && other.z > 0)
                    return false;

                return true;
            }

            static Point3<T> BuildMin(const Point3<T> &a, const Point3<T> &b) {
                T x = a.x < b.x ? a.x : b.x;
                T y = a.y < b.y ? a.y : b.y;
                T z = a.z < b.z ? a.z : b.z;

                return Point3<T>(x, y, z);
            }

            static Point3<T> BuildMax(const Point3<T> &a, const Point3<T> &b) {
                T x = a.x > b.x ? a.x : b.x;
                T y = a.y > b.y ? a.y : b.y;
                T z = a.z > b.z ? a.z : b.z;

                return Point3<T>(x, y, z);
            }

            Point3<T> withX(T X) const {
                return Point3<T>(X, y, z);
            }

            Point3<T> withY(T Y) const {
                return Point3<T>(x, Y, z);
            }

            Point3<T> withZ(T Z) const {
                return Point3<T>(x, y, Z);
            }

            Point2<T> XY() const {
                return Point2<T>(x, y);
            }

            Point2<T> YZ() const {
                return Point2<T>(y, z);
            }

            Point2<T> XZ() const {
                return Point2<T>(x, z);
            }

            bool isEmpty() const {
                if (x != 0)
                    return false;
                if (y != 0)
                    return false;
                if (z != 0)
                    return false;

                return true;
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << std::to_string(x) << "," << std::to_string(y) << "," << std::to_string(z) << ")";

                return s.str();
            }
    };
};

typedef Math::Point3<float> Vec3F;
typedef Math::Point3<double> Vec3D;
typedef Math::Point3<int32_t> Dot3;
typedef Math::Point3<uint32_t> Dot3U;

inline Dot3 DOT3(const Vec3F &v) {
    return Dot3(std::lroundf(v.X()), std::lroundf(v.Y()), std::lroundf(v.Z()));
}

inline Dot3 DOT3(const Vec3D &v) {
    return Dot3(std::lround(v.X()), std::lround(v.Y()), std::lround(v.Z()));
}

inline Vec3F VEC3F(const Dot3 &d) {
    return Vec3F(d.X(), d.Y(), d.Z());
}

inline Vec3F VEC3F(const Vec3D &v) {
    return Vec3F(v.X(), v.Y(), v.Z());
}

inline Vec3D VEC3D(const Dot3 &d) {
    return Vec3D(d.X(), d.Y(), d.Z());
}

inline Vec3D VEC3D(const Vec3F &v) {
    return Vec3D(v.X(), v.Y(), v.Z());
}

#endif //__MATH_POINT3_H__
