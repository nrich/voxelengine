#include "Game/State.h"
#include "Common/Shared.h"

#include <iostream>

#include <cstring>
#include <cmath>

using namespace Game;

State::State(Common::World *world) : world(world) {
}

void State::tick(const uint64_t time) {
}

ClientConnectResponse State::clientConnect(const ClientConnectRequest &request) {
    int64_t id = std::uniform_int_distribution<int64_t>()(rng);
    auto secret = randomString(16);

    auto client = std::make_shared<Client>(id, std::string(request.username), secret);

    addClient(client); 

    ClientConnectResponse response;
    response.id = id;
    std::memset(response.secret, 0, sizeof(response.secret));
    std::strncpy(response.secret, secret.c_str(), std::min(secret.size(), sizeof(response.secret)-1));
    response.status = GameClientStatus::OK;

    return response;
}

ClientGameStateResponse State::update(const ClientGameStateRequest &request) {
    ClientGameStateResponse response;
    response.id = request.id;

    auto client = ClientById(request.client_id);
    if (!client) {
        std::cerr << "Unknown client " << request.client_id << std::endl;
        response.status = GameClientStatus::UNKNOWN_CLIENT;
        return response;
    }

    float stepSize = 0.1;

    auto inputState = request.state;
    auto origin = Vec3F(request.origin);
    auto oldOrigin = Vec3F(request.origin);
    auto angles = Vec3F(request.angles);

    if (inputState & Actions::StepForward) {
        auto dir = angles.Forward().withY(0);

        origin = origin.project(dir, stepSize);
    }

    if (inputState & Actions::StepBackward) {
        auto dir = angles.Forward().withY(0);

        origin = origin.project(dir, -stepSize);
    }

    if (inputState & Actions::StepLeft) {
        auto dir = angles.Right().withY(0);

        origin = origin.project(dir, -stepSize);
    }

    if (inputState & Actions::StepRight) {
        auto dir = angles.Right().withY(0);

        origin = origin.project(dir, stepSize);
    }

    auto chunk_index = world->calculateChunkIndex(origin);
    auto chunk = world->chunk(chunk_index);
    if (chunk) {
        float chunk_x = std::trunc(origin.X());
        float chunk_z = std::trunc(origin.Z());

        if (std::signbit(chunk_x)) {
            chunk_x = 31 + (int(chunk_x) % 32);
        } else {
            chunk_x = (int)std::floor(origin.X()) % 32;
        }

        if (std::signbit(chunk_z)) {
            chunk_z = 31 + (int(chunk_z) % 32);
        } else {
            chunk_z = (int)std::floor(origin.Z()) % 32;
        }

        //std::cerr << origin.toString() << " " << chunk_index.toString() << " (" << chunk_x << "," << chunk_z << ")\n";

        for (int y = 0; y < BLOCKS_LEN; y++) {
            auto block = chunk->block(chunk_x, y, chunk_z);
            if (block == Common::Block::Empty) {
                origin = origin.withY(y+3);
                break;
            }
        }

    }

    auto velocity = ~(origin - oldOrigin) * stepSize;

    response.state = inputState;
    origin.copy(response.new_origin);
    velocity.copy(response.velocity);
    angles.copy(response.new_angles);

    return response;
}

std::shared_ptr<Client> State::ClientById(int64_t client_id) const {
    auto existing = clients.find(client_id);
    if (existing == clients.end()) {
        return nullptr;
    }

    return existing->second;
}

State::~State() {

}

