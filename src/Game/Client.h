#ifndef __GAME_CLIENT_H__
#define __GAME_CLIENT_H__

#include <memory>
#include <map>
#include <vector>
#include <utility>

#include "Math/Point3.h"

#include "Common/Shared.h"

namespace Game {
    class World;

    class Client {
        int64_t clientId;
        std::string username;
        std::string secret;

        Vec3F origin;
        Vec3F angles;

    public:
        Client(const uint64_t clientId, const std::string &username, const std::string &secret);

        int64_t Id() const {
            return clientId;
        }

        Vec3F Origin() const {
            return origin;
        }

        Client *Origin(Vec3F o) {
            origin = o;
            return this;
        }

        Vec3F Angles() const {
            return angles;
        }

        Client *Angles(Vec3F a) {
            angles = a;
            return this;
        }

        std::string Secret() const {
            return secret;
        }

        std::string Username() const {
            return username;
        }

        ~Client();
    };
};

#endif //__GAME_CLIENT_H__

