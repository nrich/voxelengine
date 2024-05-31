#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <list>
#include <map>
#include <memory>
#include <queue>

#include "Common/Shared.h"
#include "Common/World.h"

#include "Math/Point3.h"

#include "Game/Client.h"

namespace Game {
    class State {
        RngT rng;

        std::map<int64_t, std::shared_ptr<Client>> clients;

        Common::World *world;
    public:
        State(Common::World *world);
        void tick(const uint64_t time);

        ClientConnectResponse clientConnect(const ClientConnectRequest &request);
        ClientGameStateResponse update(const ClientGameStateRequest &request);

        std::shared_ptr<Client> ClientById(int64_t client_id) const;

        void addClient(std::shared_ptr<Client> client) {
            clients[client->Id()] = client;
        }

        ~State();
    };

};

#endif //__GAME_STATE_H__

