#include "Common/Shared.h"

bool isPowerOfTwo(int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

std::string randomString(size_t len, const std::string &CHARACTERS) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < len; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

float degreesToRadians(const float degrees) {
    return degrees * DEG2RAD;
}

std::string get_uuid() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

    std::string res;
    for (int i = 0; i < 16; i++) {
        if (dash[i]) res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}
