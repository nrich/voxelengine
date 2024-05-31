#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

#include "Common/Shared.h"
#include "Math/Point3.h"
#include "Math/Point4.h"

#include <cstring>

namespace Math {
    template <class T>struct Matrix {
        private:
            union {
                struct {
                    T m11;
                    T m12;
                    T m13; 
                    T m14;
                    T m21; 
                    T m22; 
                    T m23; 
                    T m24;
                    T m31; 
                    T m32; 
                    T m33; 
                    T m34;
                    T m41; 
                    T m42; 
                    T m43; 
                    T m44;
                };

                T m[16];
            };

            void loadIdentity() {
                const Matrix<T> Identity(
		    1, 0, 0, 0,
		    0, 1, 0, 0,
		    0, 0, 1, 0,
		    0, 0, 0, 1
		);   // Identity matrix

                std::memcpy(this->m, Identity.m, sizeof(Identity));
            }

        public:
            Matrix() {
                loadIdentity();
            }

            Matrix(
                    T _m11, T _m12, T _m13, T _m14,
                    T _m21, T _m22, T _m23, T _m24,
                    T _m31, T _m32, T _m33, T _m34,
                    T _m41, T _m42, T _m43, T _m44
                  ) {
                m11 = _m11;
                m12 = _m12;
                m13 = _m13;
                m14 = _m14;

                m21 = _m21;
                m22 = _m22;
                m23 = _m23;
                m24 = _m24;

                m31 = _m31;
                m32 = _m32;
                m33 = _m33;
                m34 = _m34;

                m41 = _m41;
                m42 = _m42;
                m43 = _m43;
                m44 = _m44;
            }

            Matrix(const T _m[16]) {
                //memcpy(this->m, _m, sizeof(T) * 16);
                for (int i = 0; i < 16; i++) {
                    m[i] = _m[i]; 
                }
            }

            Matrix(const Matrix<T> &_m) {
                //memcpy(this->m, _m, sizeof(T) * 16);
                for (int i = 0; i < 16; i++) {
                    m[i] = _m[i]; 
                }
            }

            static Matrix<T> Identity() {
                return Matrix<T>();
            }

            static Matrix<T> Projection(const T fovy, const T aspect, const T zNear, const T zFar) {
                Matrix<T> mat;

                T f = 1.0 / std::tan(fovy * M_PI / 360.0);

                T proj[] = {
                    (f/aspect), 0, 0, 0,
                    0, f, 0, 0,
                    0, 0, (zFar+zNear)/(zNear-zFar), -1,
                    0, 0, 2*zFar*zNear/(zNear-zFar), 0
                };

                for (int i = 0; i < 16; i++) {
                    mat.m[i] = proj[i];
                }

                return mat;
            }

            static Matrix<T> Ortho(const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) {
                Matrix<T> mat;

                mat.m[4*0+0] =  (T)2 / (right - left);
                mat.m[4*1+1] =  (T)2 / (top - bottom);
                mat.m[4*2+2] = -(T)2 / (zFar - zNear);
                mat.m[4*3+0] = -(right + left) / (right - left);
                mat.m[4*3+1] = -(top + bottom) / (top - bottom);
                mat.m[4*3+2] = -(zFar + zNear) / (zFar - zNear);

                return mat;
            }

            static Matrix<T> Scaling(const Point3<T> &factor) {
                Matrix<T> mat;

                mat.m[0*4+0] = factor.X();
                mat.m[1*4+0] = 0.0f;
                mat.m[2*4+0] = 0.0f;
                mat.m[3*4+0] = 0.0f;

                mat.m[0*4+1] = 0.0f;
                mat.m[1*4+1] = factor.Y();
                mat.m[2*4+1] = 0.0f;
                mat.m[3*4+1] = 0.0f;

                mat.m[0*4+2] = 0.0f;
                mat.m[1*4+2] = 0.0f;
                mat.m[2*4+2] = factor.Z();
                mat.m[3*4+2] = 0.0f;

                mat.m[0*4+3] = 0.0f;
                mat.m[1*4+3] = 0.0f;
                mat.m[2*4+3] = 0.0f;
                mat.m[3*4+3] = 1.0f;

                return mat;
            }

            static Matrix<T> Translation(const Point3<T> &pos) {
                Matrix<T> mat;

                mat.m[0*4+0] = 1.0f;
                mat.m[1*4+0] = 0.0f;
                mat.m[2*4+0] = 0.0f;
                mat.m[3*4+0] = pos.X();

                mat.m[0*4+1] = 0.0f;
                mat.m[1*4+1] = 1.0f;
                mat.m[2*4+1] = 0.0f;
                mat.m[3*4+1] = pos.Y();

                mat.m[0*4+2] = 0.0f;
                mat.m[1*4+2] = 0.0f;
                mat.m[2*4+2] = 1.0f;
                mat.m[3*4+2] = pos.Z();

                mat.m[0*4+3] = 0.0f;
                mat.m[1*4+3] = 0.0f;
                mat.m[2*4+3] = 0.0f;
                mat.m[3*4+3] = 1.0f;

                return mat;
            }

            static Matrix<T> RotateX(T angle) {
                Matrix<T> mat;

                T c = cos(angle);
                T s = sin(angle);

                mat.m[5] = c;
                mat.m[6] = s;
                mat.m[9] = -s;
                mat.m[10] = c;

                return mat;
            }

            static Matrix<T> RotateY(T angle) {
                Matrix<T> mat;

                T c = cos(angle);
                T s = sin(angle);

                mat.m[0] = c;
                mat.m[2] = -s;
                mat.m[8] = s;
                mat.m[10] = c;

                return mat;
            }

            static Matrix<T> RotateZ(T angle) {
                Matrix<T> mat;

                T c = cos(angle);
                T s = sin(angle);

                mat.m[0] = c;
                mat.m[1] = s;
                mat.m[4] = -s;
                mat.m[5] = c;

                return mat;
            }

            static Matrix<T> Scaling(const T s) {
                return Scaling(Point3<T>(s, s, s));
            }

            static Matrix<T> Scaling(const T x, const T y, const T z) {
                return Scaling(Point3<T>(x, y, z));
            }

            static Matrix<T> Translation(const T x, const T y, const T z) {
                return Translation(Point3<T>(x, y, z));
            }

            static Matrix<T> LookAt(const Point3<T> &eye, const Point3<T> &center, const Point3<T> &up) {
                Matrix<T> mat;
                Point3<T> f = ~(center - eye);
                Point3<T> s = ~(f ^ up);
                Point3<T> u = s ^ f;

                mat.m[0] = s.X();
                mat.m[4] = s.Y();
                mat.m[8] = s.Z();

                mat.m[1] = u.X();
                mat.m[5] = u.Y();
                mat.m[9] = u.Z();

                mat.m[2] = -f.X();
                mat.m[6] = -f.Y();
                mat.m[10] = -f.Z();

                mat.m[12] = -(s % eye);
                mat.m[13] = -(u % eye);
                mat.m[14] =  (f % eye);

                return mat;
            }

            const T *data() const {
                return m;
            }

            Matrix(const Point3<T> &forward, const Point3<T> &right, const Point3<T> &up) {
                loadIdentity();

                m[0] = right.X();
                m[1] = up.X();
                m[2] = -forward.X();

                m[4] = right.Y();
                m[5] = up.Y();
                m[6] = -forward.Y();

                m[8] = right.Z();
                m[9] = up.Z();
                m[10] = -forward.Z();
            }

            Matrix(const Point3<T> &Vec) {
                loadIdentity();

                m[12] = Vec.X();
                m[13] = Vec.Y();
                m[14] = Vec.Z();
            }

            T operator[](int index) const {
                return m[index];
            }

            T operator()(int i, int j) const {
                return m[(i-1)*4 + (j-1)];
            }

            Matrix<T> operator+(const Matrix<T> &M) const {
                return Matrix(
		    m11 + M.m11,
		    m12 + M.m12,
		    m13 + M.m13,
		    m14 + M.m14,

		    m21 + M.m21,
		    m22 + M.m22,
		    m23 + M.m23,
		    m24 + M.m24,

		    m31 + M.m31,
		    m32 + M.m32,
		    m33 + M.m33,
		    m34 + M.m34,

		    m41 + M.m41,
		    m42 + M.m42,
		    m43 + M.m43,
		    m44 + M.m44
                );
            }

            Matrix<T> operator-(const Matrix<T> &M) const {
                return Matrix<T>(
		    m11 - M.m11,
		    m12 - M.m12,
		    m13 - M.m13,
		    m14 - M.m14,

		    m21 - M.m21,
		    m22 - M.m22,
		    m23 - M.m23,
		    m24 - M.m24,

		    m31 - M.m31,
		    m32 - M.m32,
		    m33 - M.m33,
		    m34 - M.m34,

		    m41 - M.m41,
		    m42 - M.m42,
		    m43 - M.m43,
		    m44 - M.m44
                );
            }

            Matrix<T> operator*(T mul) const {
                return Matrix<T>(
		    m11 * mul,
		    m12 * mul,
		    m13 * mul,
		    m14 * mul,

		    m21 * mul,
		    m22 * mul,
		    m23 * mul,
		    m24 * mul,

		    m31 * mul,
		    m32 * mul,
		    m33 * mul,
		    m34 * mul,

		    m41 * mul,
		    m42 * mul,
		    m43 * mul,
		    m44 * mul
                );
            }

            Matrix<T> operator*(const Matrix<T> &B) const {
                return Matrix<T>(
#if 1
		    m11 * B.m11 + m12 * B.m21 + m13 * B.m31 + m14 * B.m41,
		    m11 * B.m12 + m12 * B.m22 + m13 * B.m32 + m14 * B.m42,
		    m11 * B.m13 + m12 * B.m23 + m13 * B.m33 + m14 * B.m43,
		    m11 * B.m14 + m12 * B.m24 + m13 * B.m34 + m14 * B.m44,

		    m21 * B.m11 + m22 * B.m21 + m23 * B.m31 + m24 * B.m41,
		    m21 * B.m12 + m22 * B.m22 + m23 * B.m32 + m24 * B.m42,
		    m21 * B.m13 + m22 * B.m23 + m23 * B.m33 + m24 * B.m43,
		    m21 * B.m14 + m22 * B.m24 + m23 * B.m34 + m24 * B.m44,

		    m31 * B.m11 + m32 * B.m21 + m33 * B.m31 + m34 * B.m41,
		    m31 * B.m12 + m32 * B.m22 + m33 * B.m32 + m34 * B.m42,
		    m31 * B.m13 + m32 * B.m23 + m33 * B.m33 + m34 * B.m43,
		    m31 * B.m14 + m32 * B.m24 + m33 * B.m34 + m34 * B.m44,

		    m41 * B.m11 + m42 * B.m21 + m43 * B.m31 + m44 * B.m41,
		    m41 * B.m12 + m42 * B.m22 + m43 * B.m32 + m44 * B.m42,
		    m41 * B.m13 + m42 * B.m23 + m43 * B.m33 + m44 * B.m43,
		    m41 * B.m14 + m42 * B.m24 + m43 * B.m34 + m44 * B.m44
#else
		    m11 * B.m11 + m21 * B.m12 + m31 * B.m13 + m41 * B.m14,
		    m11 * B.m21 + m21 * B.m22 + m31 * B.m23 + m41 * B.m24,
		    m11 * B.m31 + m21 * B.m32 + m31 * B.m33 + m41 * B.m34,
		    m11 * B.m41 + m21 * B.m42 + m31 * B.m43 + m41 * B.m44,

		    m12 * B.m11 + m22 * B.m12 + m32 * B.m13 + m42 * B.m14,
		    m12 * B.m21 + m22 * B.m22 + m32 * B.m23 + m42 * B.m24,
		    m12 * B.m31 + m22 * B.m32 + m32 * B.m33 + m42 * B.m34,
		    m12 * B.m41 + m22 * B.m42 + m32 * B.m43 + m42 * B.m44,

		    m13 * B.m11 + m23 * B.m12 + m33 * B.m13 + m43 * B.m14,
		    m13 * B.m21 + m23 * B.m22 + m33 * B.m23 + m43 * B.m24,
		    m13 * B.m31 + m23 * B.m32 + m33 * B.m33 + m43 * B.m34,
		    m13 * B.m41 + m23 * B.m42 + m33 * B.m43 + m43 * B.m44,

		    m14 * B.m11 + m24 * B.m12 + m34 * B.m13 + m44 * B.m14,
		    m14 * B.m21 + m24 * B.m22 + m34 * B.m23 + m44 * B.m24,
		    m14 * B.m31 + m24 * B.m32 + m34 * B.m33 + m44 * B.m34,
		    m14 * B.m41 + m24 * B.m42 + m34 * B.m43 + m44 * B.m44
#endif
		);
            }

            Matrix<T> operator/(T div) const {
                return Matrix<T>(
		    m11 / div,
		    m12 / div,
		    m13 / div,
		    m14 / div,

		    m21 / div,
		    m22 / div,
		    m23 / div,
		    m24 / div,

		    m31 / div,
		    m32 / div,
		    m33 / div,
		    m34 / div,

		    m41 / div,
		    m42 / div,
		    m43 / div,
		    m44 / div
		);
            }

            Point3<T> operator*(const Point3<T> &in) const{

/*
                Point3<T> out;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        T val = m[i * 4 + j];
                        T vecval = in(i);

                        out[i] += val * vecval;
                    }
                }

                return out;

*/

                T x = in[0] * m[0] + in[1] * m[4] + in[2] * m[8] + m[12]; 
                T y = in[0] * m[1] + in[1] * m[5] + in[2] * m[9] + m[13]; 
                T z = in[0] * m[2] + in[1] * m[6] + in[2] * m[10] + m[14];
                T w = in[0] * m[3] + in[1] * m[7] + in[2] * m[11] + m[15];

                if (w != 1 && w != 0) {
                    return Point3<T>(x, y, z)/w;
                }

                return Point3<T>(x, y, z);
            }

            Point4<T> operator*(const Point4<T> &in) const{
                T x = m[0]*in[0]+m[4]*in[1]+m[8]*in[2]+m[12]*in[3];
                T y = m[1]*in[0]+m[5]*in[1]+m[9]*in[2]+m[13]*in[3];
                T z = m[2]*in[0]+m[6]*in[1]+m[10]*in[2]+m[14]*in[3];
                T w = m[3]*in[0]+m[7]*in[1]+m[11]*in[2]+m[15]*in[3];

                return Point4<T>(w, x, y, z);
            }

            Matrix<T> operator!() const {   // Inverse
                Matrix<T> A;

                T M3344 = m33 * m44 - m43 * m34;
                T M2344 = m23 * m44 - m43 * m24;
                T M2334 = m23 * m34 - m33 * m24;
                T M3244 = m32 * m44 - m42 * m34;
                T M2244 = m22 * m44 - m42 * m24;
                T M2234 = m22 * m34 - m32 * m24;
                T M3243 = m32 * m43 - m42 * m33;
                T M2243 = m22 * m43 - m42 * m23;
                T M2233 = m22 * m33 - m32 * m23;
                T M1344 = m13 * m44 - m43 * m14;
                T M1334 = m13 * m34 - m33 * m14;
                T M1244 = m12 * m44 - m42 * m14;
                T M1234 = m12 * m34 - m32 * m14;
                T M1243 = m12 * m43 - m42 * m13;
                T M1233 = m12 * m33 - m32 * m13;
                T M1324 = m13 * m24 - m23 * m14;
                T M1224 = m12 * m24 - m22 * m14;
                T M1223 = m12 * m23 - m22 * m13;

                A.m11 = m22 * M3344 - m32 * M2344 + m42 * M2334;
                A.m21 = -m21 * M3344 + m31 * M2344 - m41 * M2334;
                A.m31 = m21 * M3244 - m31 * M2244 + m41 * M2234;
                A.m41 = -m21 * M3243 + m31 * M2243 - m41 * M2233;

                A.m12 = -m12 * M3344 + m32 * M1344 - m42 * M1334;
                A.m22 = m11 * M3344 - m31 * M1344 + m41 * M1334;
                A.m32 = -m11 * M3244 + m31 * M1244 - m41 * M1234;
                A.m42 = m11 * M3243 - m31 * M1243 + m41 * M1233;

                A.m13 = m12 * M2344 - m22 * M1344 + m42 * M1324;
                A.m23 = -m11 * M2344 + m21 * M1344 - m41 * M1324;
                A.m33 = m11 * M2244 - m21 * M1244 + m41 * M1224;
                A.m43 = -m11 * M2243 + m21 * M1243 - m41 * M1223;

                A.m14 = -m12 * M2334 + m22 * M1334 - m32 * M1324;
                A.m24 = m11 * M2334 - m21 * M1334 + m31 * M1324;
                A.m34 = -m11 * M2234 + m21 * M1234 - m31 * M1224;
                A.m44 = m11 * M2233 - m21 * M1233 + m31 * M1223;

                A = A / (m11 * A.m11 + m21 * A.m12 + m31 * A.m13 + m41 * A.m14);   // Determinant

                return A;
            }

            Matrix<T> operator~() const {   // Transpose
                return Matrix<T>(
		    m11, m21, m31, m41,
		    m12, m22, m32, m42,
		    m13, m23, m33, m43,
		    m14, m24, m34, m44
		);
            }

            T determinate() const {
                T M3344 = m33 * m44 - m43 * m34;
                T M2344 = m23 * m44 - m43 * m24;
                T M2334 = m23 * m34 - m33 * m24;
                T M1344 = m13 * m44 - m43 * m14;
                T M1334 = m13 * m34 - m33 * m14;
                T M1324 = m13 * m24 - m23 * m14;

                return
                    m11 * (m22 * M3344 - m32 * M2344 + m42 * M2334) -
                    m21 * (m12 * M3344 - m32 * M1344 + m42 * M1334) +
                    m31 * (m12 * M2344 - m22 * M1344 + m42 * M1324) -
                    m41 * (m12 * M2334 - m22 * M1334 + m32 * M1324);
            }

            Matrix<T> Rotate(T angle, Point3<T> Vec) const {
                Matrix<T> ret;
                Matrix<T> out;

                T c = cos(angle);
                T s = sin(angle);
                T mc = 1 - c;
                T x = Vec[0];
                T y = Vec[1];
                T z = Vec[2];

                ret.m[0] = (x * x * mc) + c;
                ret.m[1] = (y * x * mc) + (z * s);
                ret.m[2] = (x * z * mc) - (y * s);
                ret.m[4] = (x * y * mc) - (z * s);
                ret.m[5] = (y * y * mc) + c;
                ret.m[6] = (y * z * mc) + (x * s);
                ret.m[8] = (x * z * mc) + (y * s);
                ret.m[9] = (y * z * mc) - (x * s);
                ret.m[10] = (z * z * mc) + c;

                out = (*this) * ret;

                return out;
            }

            Matrix<T> Translate(const Point3<T> &Vec) const {
                Matrix<T> t;
                Matrix<T> out;

                t.m[12] = Vec.X();
                t.m[13] = Vec.Y();
                t.m[14] = Vec.Z();

                out = (*this) * t;

                return out;
            }

            Matrix<T> Translate(T x, T y, T z) const {
                Matrix<T> t;
                Matrix<T> out;

                t.m[12] = x;
                t.m[13] = y;
                t.m[14] = z;

                out = (*this) * t;

                return out;
            }

            Point3<T> Rotate(const Point3<T> &vec) const {
                T x = (m[0] * vec.X()) + (m[4] * vec.Y()) + (m[8]  * vec.Z());	// Rotate Around The X Axis (NEW)
                T y = (m[1] * vec.X()) + (m[5] * vec.Y()) + (m[9]  * vec.Z());	// Rotate Around The Y Axis (NEW)
                T z = (m[2] * vec.X()) + (m[6] * vec.Y()) + (m[10] * vec.Z());	// Rotate Around The Z Axis (NEW)

                return Point3<T>(x, y, z);
            }

            std::string toString() const {
                std::ostringstream s;

                s << "[" << m11 << "," << m12 << "," << m13 << "," << m14 << "]";
                s << "[" << m21 << "," << m22 << "," << m23 << "," << m24 << "]";
                s << "[" << m31 << "," << m32 << "," << m33 << "," << m34 << "]";
                s << "[" << m41 << "," << m42 << "," << m43 << "," << m44 << "]";

                return s.str();
            }
    };
};

typedef Math::Matrix<float> MatrixF;
typedef Math::Matrix<double> MatrixD;

#endif //__MATH_MATRIX_H__
