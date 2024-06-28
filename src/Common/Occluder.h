#ifndef __COMMON_OCCLUDER_H__
#define __COMMON_OCCLUDER_H__

#include <memory>
#include <vector>
#include <bitset>

#include "Common/Chunk.h"
#include "Renderer/Voxel.h"

namespace Common {
    class Occluder {
        Dot3 min;
        Dot3 max;

        int count;
        int size;
    public:
        Occluder(const Dot3 &min, const Dot3 &max, int count, int size) : min(min), max(max), count(count), size(size) {
        }

        Occluder() : count(0), size(0) {
        }

        Occluder(const Occluder &other) : min(other.min), max(other.max), count(other.count), size(other.size) {
        }

        Dot3 Min() const {
            return min;
        }

        Dot3 Max() const {
            return max;
        }

        int Count() const {
            return count;
        }

        int Size() const {
            return size;
        }

        float Ratio() const {
            if (count == 0)
                return 0;

            return (float)count/(float)size;
        }
    };
}; // namespace Common

#endif //__COMMON_OCCLUDER_H__
