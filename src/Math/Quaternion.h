#ifndef __MATH_QUATERNION_H__
#define __MATH_QUATERNION_H__

#include <string>
#include <sstream>

#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Point4.h"
#include "Math/Matrix.h"
#include "Common/Shared.h"

namespace Math {
    template <class T>struct Quaternion {
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
            Quaternion<T>() {
                w = 0;
                x = 0;
                y = 0;
                z = 0;
            }

            Quaternion<T>(T _w, T _x, T _y, T _z) {
                w = _w;
                x = _x;
                y = _y;
                z = _z;
            }

            Quaternion<T>(const T q[4]) {
                w = q[0];
                x = q[1];
                y = q[2];
                z = q[3];
            }

            Quaternion<T>(const Quaternion<T> &Q) {
                w = Q.W();
                x = Q.X();
                y = Q.Y();
                z = Q.Z();
            }

            Quaternion<T>(const Point4<T> &V) {
                w = V.W();
                x = V.X();
                y = V.Y();
                z = V.Z();
            }

            Quaternion<T>(const Point3<T> &axis, T angle) {
                Point3<T> V = ~axis;

                float sine = std::sin(angle);
                w = std::cos(angle);

                x = V.x * sine;
                y = V.y * sine;
                z = V.z * sine;
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


            const Quaternion<T> operator~() const {
                return this->normal();
            }

            const Quaternion<T> normal() const {
                T d = w*w + x*x + y*y + z*z;

                if (d < DEF_EPSILON)
                    return (*this);

                float len = 1 / d;

                T lw = w * len;
                T lx = x * len;
                T ly = y * len;
                T lz = z * len;

                return Quaternion<T>(lw, lx, ly, lz);
            }

            //maths functions

            // postentially dangerous
            T operator[](int index) const {
                return vec[index];
            }

            T operator()(int index) const {
                return vec[index];
            }

            const Quaternion<T> operator-() const {
                return Point3<T>(-x, -y, -z, -w);
            }

            //Comparison
            const bool operator==(const Quaternion<T> &Q) const {
                if (x != Q.x)
                    return false;
                if (y != Q.y)
                    return false;
                if (z != Q.z)
                    return false;
                if (w != Q.w)
                    return false;

                return true;
            }

            const bool operator!=(const Quaternion<T> &Q) const {
                if (x != Q.x)
                    return true;
                if (y != Q.y)
                    return true;
                if (z != Q.z)
                    return true;
                if (w != Q.w)
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

            const Matrix<T> getMatrix() const {
                const T xx = x * x;
                const T yy = y * y;
                const T zz = z * z;
                const T xy = x * y;
                const T xz = x * z;
                const T xw = x * w;
                const T yz = y * z;
                const T yw = y * w;
                const T zw = z * w;

                return Matrix<T>(
                    static_cast<T>(1.0) - static_cast<T>(2.0) * (yy + zz), static_cast<T>(2.0) * (xy + zw), static_cast<T>(2.0) * (xz - yw), static_cast<T>(0.0),
                    static_cast<T>(2.0) * (xy - zw), static_cast<T>(1.0) - static_cast<T>(2.0) * (xx + zz), static_cast<T>(2.0) * (yz + xw), static_cast<T>(0.0),
                    static_cast<T>(2.0) * (xz + yw), static_cast<T>(2.0) * (yz - xw), static_cast<T>(1.0) - static_cast<T>(2.0) * (xx + yy), static_cast<T>(0.0),
                    static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(1.0)

                );
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << x << "," << y << "," << z << "," << w << ")";

                return s.str();
            }
    };
};

typedef Math::Quaternion<float> QuatF;
typedef Math::Quaternion<double> QuatD;

#endif //__MATH_QUATERNION_H__
