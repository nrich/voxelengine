#ifndef __CLIENT_BOUNDINGBOX_H__
#define __CLIENT_BOUNDINGBOX_H__

#include "Common/Shared.h"
#include "Math/Point3.h"

namespace Client {
    using Math::Point3;

    template <class T>struct BoundingBox {
        private:
            Point3<T> bounds[8];

            void set(const Point3<T> &min, const Point3<T> &max) {
                T minX = min.X() > max.X() ? max.X() : min.X();
                T minY = min.Y() > max.Y() ? max.Y() : min.Y();
                T minZ = min.Z() > max.Z() ? max.Z() : min.Z();

                T maxX = min.X() < max.X() ? max.X() : min.X();
                T maxY = min.Y() < max.Y() ? max.Y() : min.Y();
                T maxZ = min.Z() < max.Z() ? max.Z() : min.Z();

                bounds[0] = Point3<T>(minX, minY, minZ);
                bounds[1] = Point3<T>(maxX, minY, minZ);
                bounds[2] = Point3<T>(minX, minY, maxZ);
                bounds[3] = Point3<T>(maxX, minY, maxZ);

                bounds[4] = Point3<T>(minX, maxY, minZ);
                bounds[5] = Point3<T>(maxX, maxY, minZ);
                bounds[6] = Point3<T>(minX, maxY, maxZ);
                bounds[7] = Point3<T>(maxX, maxY, maxZ);
            }

        public:
            BoundingBox(const Point3<T> &min, const Point3<T> &max) {
                set(min, max);
            }

            Point3<T> getMin() const {
                return bounds[0];
            }

            Point3<T> getMax() const {
                return bounds[7];
            }

            T getLength() const {
                return (bounds[7] - bounds[0]).length();
            }

            Point3<T> operator[](int index) const {
                return bounds[index];
            }

            bool isInside(const Point3<T> &point) const {		// return true if Point is inside box
                if (point >= bounds[0] && point <= bounds[7])
                    return true;
                else
                    return false;                
            }

            bool overlap(const BoundingBox<T> &other) const {
                for (int i = 0; i < 8; i++) {
                    if (isInside(other.bounds[i]))
                        return true;
                }

                return false;
            }

            std::string toString() const {
                std::ostringstream s;

                s << "[" << bounds[0].toString() << bounds[7].toString() << "]";

                return s.str();
            }
    };
};

typedef Client::BoundingBox<float> BoundingBoxF;
typedef Client::BoundingBox<int32_t> BoundingBoxI;

#endif //__CLIENT_BOUNDINGBOX_H__
