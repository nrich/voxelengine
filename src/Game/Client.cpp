#include "Game/Client.h"

#include <iostream>

using namespace Game;

Client::Client(const uint64_t clientId, const std::string &username, const std::string &secret) : clientId(clientId), username(username), secret(secret) {
}

Client::~Client() {

}

