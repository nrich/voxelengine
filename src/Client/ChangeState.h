#ifndef __CLIENT_CHANGESTATE_H__
#define __CLIENT_CHANGESTATE_H__

#include <cstdint>

#include <sstream>

namespace Client {
    struct ChangeState {
        Vec3F origin;
        Vec3F angles;
        Vec3F viewAngles;
        int64_t clientId;
        std::string secret;

        ChangeState(const Vec3F &origin, const Vec3F &angles, const Vec3F &viewAngles, int64_t clientId, const std::string &secret) : origin(origin), angles(angles), viewAngles(viewAngles), clientId(clientId), secret(secret) {
        }

        std::string toString() const {
            std::ostringstream s;

            s << "{" << origin.toString() << "," << angles.toString() << "," << viewAngles.toString() << "," << clientId << "," << secret << "}";

            return s.str();
        }
    };
}; // namespace Client

#endif //__CLIENT_CHANGESTATE_H__
