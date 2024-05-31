#ifndef __MATH_POINT2_H__
#define __MATH_POINT2_H__

#include "Common/Shared.h"

#include <string>
#include <sstream>

namespace Math {
    template <class T>struct Point3;

    template <class T>struct Point2 {
        private:
            union {
                T vec[2];

                struct {
                    T x;
                    T y;
                };
            };

        public:
            Point2<T>() {
                x = 0;
                y = 0;
            }

            Point2<T>(T _x, T _y) {
                x = _x;
                y = _y;
            }

            Point2<T>(const T v[2]) {
                x = v[0];
                y = v[1];
            }

            //Accessors
            T X() const {
                return x;
            }

            T Y() const {
                return y;
            }

            const T *data() const {
                return vec;
            }

            //maths functions

            //Add
            Point2<T> operator+(const Point2<T> &add) const {
                return Point2<T>((x+add.x), (y+add.y));
            }

            //Subtract
            Point2<T> operator-(const Point2<T> &sub) const {
                return Point2<T>((x-sub.x), (y-sub.y));
            }

            T length() const {
                return sqrt(x*x + y*y);
            }

            T distance(const Point2<T> &p) const {
                T xd = p.x - x;
                T yd = p.y - y;

                return xd * xd + yd * yd;
            }

            T angle() const {
                T angle = std::atan2(y, x);
                T degrees = 180.0*angle/M_PI; 

                return std::fmod(360.0f+std::round(degrees), 360);
            }

            Point2<T> operator*(T mult) const {
                return Point2<T>(x*mult, y*mult);
            }


            Point2<T> operator!() const {
                return Point2<T>(-x, -y);
            }

            Point2<T> operator/(T div) const {
                return Point2<T>(x/div, y/div);
            }

            // TODO non const operator
            // Potentially dangerous
            T &operator[](int index) {
                return vec[index];
            }

            const T operator[](int index) const {
                return vec[index];
            }

            T operator()(int index) const {
                return vec[index];
            }

            Point2<T> operator-() const {
                return Point2<T>(-x, -y);
            }

            //Comparison
            bool operator==(const Point2<T> &V) const {
                if (x != V.x)
                    return false;
                if (y != V.y)
                    return false;

                return true;
            }

            bool operator!=(const Point2<T> &V) const {
                if (x != V.x)
                    return true;
                if (y != V.y)
                    return true;

                return false;
            }

            bool operator>(const Point2<T> &V) const {
                if (x <= V.x)
                    return false;
                if (y <= V.y)
                    return false;

                return true;
            }

            bool operator<(const Point2<T> &V) const {
                if (x >= V.x)
                    return false;
                if (y >= V.y)
                    return false;

                return true;
            }

            bool operator>=(const Point2<T> &V) const {
                if (x < V.x)
                    return false;
                if (y < V.y)
                    return false;

                return true;
            }

            bool operator<=(const Point2<T> &V) const {
                if (x > V.x)
                    return false;
                if (y > V.y)
                    return false;

                return true;
            }

            //Dot Product
            const T operator%(const Point2<T> &v) const  {
                return (x*v.x + y*v.y);
            }

            Point2<T> operator~() const {
                T len = length();

                if (len) {
                    len = 1 / len;

                    T lx = x * len;
                    T ly = y * len;

                    return Point2<T>(lx, ly);
                } else {
                    return (*this);
                }
            }

            void copy(T out[2]) const{
                out[0] = x;
                out[1] = y;
            }

            static Point2<T> BuildMin(const Point2<T> &a, const Point2<T> &b) {
                T x = a.x < b.x ? a.x : b.x;
                T y = a.y < b.y ? a.y : b.y;

                return Point2<T>(x, y);
            }

            static Point2<T> BuildMax(const Point2<T> &a, const Point2<T> &b) {
                T x = a.x > b.x ? a.x : b.x;
                T y = a.y > b.y ? a.y : b.y;

                return Point2<T>(x, y);
            }

            Point2<T> withX(T X) const {
                return Point2<T>(X, y);
            }

            Point2<T> withY(T Y) const {
                return Point2<T>(x, Y);
            }

            Point3<T> XY(T Z) const {
                return Point3<T>(x, y, Z);
            }

            Point3<T> XZ(T Y) const {
                return Point3<T>(x, Y, y);
            }

            Point3<T> YZ(T X) const {
                return Point3<T>(X, x, y);
            }

            bool isEmpty() const {
                if (x != 0)
                    return false;
                if (y != 0)
                    return false;

                return true;
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << std::to_string(x) << "," << std::to_string(y) << ")";

                return s.str();
            }
    };
};

typedef Math::Point2<float> Vec2F;
typedef Math::Point2<double> Vec2D;
typedef Math::Point2<int32_t> Dot;

inline Dot DOT(const Vec2F &v) {
    return Dot(std::lroundf(v.X()), std::lroundf(v.Y()));
}

inline Dot DOT(const dot2 &d) {
    return Dot(d[0], d[1]);
}

inline Dot DOT(const Vec2D &v) {
    return Dot(std::lround(v.X()), std::lround(v.Y()));
}

inline Vec2F VEC2F(const Dot &d) {
    return Vec2F(d.X(), d.Y());
}

inline Vec2F VEC2F(const Vec2D &v) {
    return Vec2F(v.X(), v.Y());
}

inline Vec2D VEC2D(const Dot &d) {
    return Vec2D(d.X(), d.Y());
}

inline Vec2D VEC2D(const Vec2F &v) {
    return Vec2D(v.X(), v.Y());
}


#endif //__MATH_POINT2_H__
