#ifndef __MATH_POINT4_H__
#define __MATH_POINT4_H__

#include <string>
#include <sstream>

#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Common/Shared.h"

namespace Math {
    template <class T>struct Point4 {
        private:
            union {
                T vec[4];

                struct {
                    T x;
                    T y;
                    T z;
                    T w;
                };
            };

        public:
            Point4<T>() {
                w = 0;
                x = 0;
                y = 0;
                z = 0;
            }

            Point4<T>(T _w, T _x, T _y, T _z) {
                w = _w;
                x = _x;
                y = _y;
                z = _z;
            }

            Point4<T>(const T v[4]) {
                w = v[0];
                x = v[1];
                y = v[2];
                z = v[3];
            }

            Point4<T>(const Point4<T> &V) {
                w = V.w;
                x = V.x;
                y = V.y;
                z = V.z;
            }

            Point4<T>(const Point3<T> &V, T _w) {
                w = _w;
                x = V.X();
                y = V.Y();
                z = V.Z();
            }

            Point4<T>(T _w, const Point3<T> &V) {
                w = _w;
                x = V.X();
                y = V.Y();
                z = V.Z();
            }

            Point4<T>(T _w, const Point2<T> &V, T _z) {
                w = _w;
                x = V.X();
                y = V.Y();
                z = _z;
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

            T W() const {
                return w;
            }

            const T *data() const {
                return vec;
            }

            const T length() const {
                return sqrt(w*w + x*x + y*y + z*z);
            }


            const Point4<T> operator~() const {
                return this->normal();
            }

            const Point4<T> normal() const {
                T d = w*w + x*x + y*y + z*z;

                if (d < DEF_EPSILON)
                    return (*this);

                float len = 1 / d;

                T lw = w * len;
                T lx = x * len;
                T ly = y * len;
                T lz = z * len;

                return Point4<T>(lw, lx, ly, lz);
            }

            //maths functions

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

            const Point4<T> operator-() const {
                return Point4<T>(-w, -x, -y, -z);
            }

            //Comparison
            const bool operator==(const Point4<T> &V) const {
                if (x != V.x)
                    return false;
                if (y != V.y)
                    return false;
                if (z != V.z)
                    return false;
                if (w != V.w)
                    return false;

                return true;
            }

            const bool operator!=(const Point4<T> &V) const {
                if (x != V.x)
                    return true;
                if (y != V.y)
                    return true;
                if (z != V.z)
                    return true;
                if (w != V.w)
                    return true;

                return false;
            }

            void copy(T out[4]) const {
                out[0] = x;
                out[1] = y;
                out[2] = z;
                out[3] = w;
            }

            bool isEmpty() const {
                if (x != 0)
                    return false;
                if (y != 0)
                    return false;
                if (z != 0)
                    return false;
                if (w != 0)
                    return false;

                return true;
            }

            Point2<T> XY() const {
                return Point2<T>(x, y);
            }

            Point3<T> XYZ() const {
                return Point3<T>(x, y, z);
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << std::to_string(x) << "," << std::to_string(y) << "," << std::to_string(z) << "," << std::to_string(w) << ")";

                return s.str();
            }
    };
};

typedef Math::Point4<float> Vec4F;
typedef Math::Point4<double> Vec4D;
typedef Math::Point4<int32_t> Dot4;
typedef Math::Point4<uint32_t> Dot4U;

#endif //__MATH_POINT4_H__
