#ifndef __CLIENT_CHANGESTATE_H__
#define __CLIENT_CHANGESTATE_H__

#include <cstdint>

#include <sstream>

namespace Client {
    struct ChangeState {
        Vec3F origin;
        vecf angle;

        ChangeState(Vec3F origin, vecf angle) : origin(origin), angle(angle) {

        }

        std::string toString() const {
            std::ostringstream s;

            s << "{" << origin.toString() << "," << std::to_string(angle) << "}";

            return s.str();
        }
    };
}; // namespace Client

#endif //__CLIENT_CHANGESTATE_H__
