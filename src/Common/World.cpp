#include "Common/World.h"

#include <algorithm>

using namespace Common;

World::World(uint64_t seed) : seed(seed) {
    root = std::make_shared<Chunk>();

#if 0
    root->set(4, 1, 16, Block::Rock);
    root->set(4, 2, 16, Block::Rock);
    root->set(4, 3, 16, Block::Rock);
    root->set(3, 1, 16, Block::Rock);
    root->set(3, 2, 16, Block::Rock);
    root->set(3, 3, 16, Block::Rock);
    root->set(5, 1, 16, Block::Rock);
    root->set(5, 2, 16, Block::Rock);
    root->set(5, 3, 16, Block::Rock);

    root->set(4, 1, 20, Block::Sand);
    root->set(4, 1, 10, Block::Grass);

    root->buildMesh();
return;
#endif

    root->fill(seed, 0, 0, 0);
    root->buildMesh();
//return;
    expand(root, Dot3(0, 0, 0));

    expand(root->neighbour(Neighbour::Left), Dot3(-BLOCKS_LEN, 0, 0));
    expand(root->neighbour(Neighbour::Right), Dot3(BLOCKS_LEN, 0, 0));
    expand(root->neighbour(Neighbour::Front), Dot3(0, 0, -BLOCKS_LEN));
    expand(root->neighbour(Neighbour::Back), Dot3(0, 0, BLOCKS_LEN));
}

void World::expand(std::shared_ptr<Chunk> chunk, const Dot3 &centre) {
    auto left = chunk->neighbour(Neighbour::Left);
    if (!left) {
        left = std::make_shared<Chunk>();
        left->fill(seed, centre.X() - BLOCKS_LEN, centre.Y(), centre.Z());
        left->buildMesh();
        Chunk::Join(chunk, Neighbour::Left, left);
    }

    auto right = chunk->neighbour(Neighbour::Right);
    if (!right) {
        right = std::make_shared<Chunk>();
        right->fill(seed, centre.X() + BLOCKS_LEN, centre.Y(), centre.Z());
        right->buildMesh();
        Chunk::Join(chunk, Neighbour::Right, right);
    }

    auto front = chunk->neighbour(Neighbour::Front);
    if (!front) {
        front = std::make_shared<Chunk>();
        front->fill(seed, centre.X(), centre.Y(), centre.Z() - BLOCKS_LEN);
        front->buildMesh();
        Chunk::Join(chunk, Neighbour::Front, front);
    }

    auto back = chunk->neighbour(Neighbour::Back);
    if (!back) {
        back = std::make_shared<Chunk>();
        back->fill(seed, centre.X(), centre.Y(), centre.Z() + BLOCKS_LEN);
        back->buildMesh();
        Chunk::Join(chunk, Neighbour::Back, back);
    }

    auto front_left = left->neighbour(Neighbour::Front);
    if (!front_left) {
        front_left = front->neighbour(Neighbour::Left);

        if (!front_left) {
            front_left = std::make_shared<Chunk>();
            front_left->fill(seed, centre.X() - BLOCKS_LEN, centre.Y(), centre.Z() - BLOCKS_LEN);
            front_left->buildMesh();
        }
    }

    Chunk::Join(left, Neighbour::Front, front_left);
    Chunk::Join(front, Neighbour::Left, front_left);

    auto back_left = left->neighbour(Neighbour::Back);
    if (!back_left) {
        back_left = back->neighbour(Neighbour::Left);

        if (!back_left) {
            back_left = std::make_shared<Chunk>();
            back_left->fill(seed, centre.X() - BLOCKS_LEN, centre.Y(), centre.Z() + BLOCKS_LEN);
            back_left->buildMesh();
        }
    }

    Chunk::Join(left, Neighbour::Back, back_left);
    Chunk::Join(back, Neighbour::Left, back_left);

    //

    auto front_right = right->neighbour(Neighbour::Front);
    if (!front_right) {
        front_right = front->neighbour(Neighbour::Right);

        if (!front_right) {
            front_right = std::make_shared<Chunk>();
            front_right->fill(seed, centre.X() + BLOCKS_LEN, centre.Y(), centre.Z() - BLOCKS_LEN);
            front_right->buildMesh();
        }
    }

    Chunk::Join(right, Neighbour::Front, front_right);
    Chunk::Join(front, Neighbour::Right, front_right);

    auto back_right = right->neighbour(Neighbour::Back);
    if (!back_right) {
        back_right = back->neighbour(Neighbour::Right);

        if (!back_right) {
            back_right = std::make_shared<Chunk>();
            back_right->fill(seed, centre.X() + BLOCKS_LEN, centre.Y(), centre.Z() + BLOCKS_LEN);
            back_right->buildMesh();
        }
    }

    Chunk::Join(right, Neighbour::Back, back_right);
    Chunk::Join(back, Neighbour::Right, back_right);
}

std::shared_ptr<Chunk> World::chunk(int32_t x, int32_t y, int32_t z) {
    std::shared_ptr<Chunk> current = root;

    while (x) {
        if (x > 0) {
            current = current->neighbour(Neighbour::Right);
            if (!current)
                return nullptr;
            x--;
        } else if (x < 0) {
            current = current->neighbour(Neighbour::Left);
            if (!current)
                return nullptr;
            x++;
        }
    }

    while (y) {
        if (y > 0) {
            current = current->neighbour(Neighbour::Top);
            if (!current)
                return nullptr;
            y--;
        } else if (y < 0) {
            current = current->neighbour(Neighbour::Bottom);
            if (!current)
                return nullptr;
            y++;
        }
    }

    while (z) {
        if (z > 0) {
            current = current->neighbour(Neighbour::Back);
            if (!current)
                return nullptr;
            z--;
        } else if (z < 0) {
            current = current->neighbour(Neighbour::Front);
            if (!current)
                return nullptr;
            z++;
        }
    }

    return current;
}

std::shared_ptr<Chunk> World::chunk(const Dot3 &index) {
    return chunk(index.X(), index.Y(), index.Z());
}

std::shared_ptr<Chunk> World::chunk(const Vec3F &world_pos) {
    auto index = calculateChunkIndex(world_pos); 
    return chunk(index.X(), index.Y(), index.Z());
}

const std::shared_ptr<Chunk> World::chunk(int32_t x, int32_t y, int32_t z) const {
    std::shared_ptr<Chunk> current = root;

    while (x) {
        if (x > 0) {
            current = current->neighbour(Neighbour::Right);
            if (!current)
                return nullptr;
            x--;
        } else if (x < 0) {
            current = current->neighbour(Neighbour::Left);
            if (!current)
                return nullptr;
            x++;
        }
    }

    while (y) {
        if (y > 0) {
            current = current->neighbour(Neighbour::Top);
            if (!current)
                return nullptr;
            y--;
        } else if (y < 0) {
            current = current->neighbour(Neighbour::Bottom);
            if (!current)
                return nullptr;
            y++;
        }
    }

    while (z) {
        if (z > 0) {
            current = current->neighbour(Neighbour::Back);
            if (!current)
                return nullptr;
            z--;
        } else if (z < 0) {
            current = current->neighbour(Neighbour::Front);
            if (!current)
                return nullptr;
            z++;
        }
    }

    return current;

}

Dot3 World::calculateChunkIndex(const Vec3F &world_point) const {
    auto x = std::trunc(world_point.X() / BLOCKS_LEN);
    auto y = std::trunc(world_point.Y() / BLOCKS_LEN);
    auto z = std::trunc(world_point.Z() / BLOCKS_LEN);

    if (std::signbit(x))
        x -= 1;
    else
        x = std::floor(world_point.X()) / BLOCKS_LEN;

    if (std::signbit(y))
        y -= 1;
    else
        y = std::floor(world_point.Y()) / BLOCKS_LEN;

    if (std::signbit(z))
        z -= 1;
    else
        z = std::floor(world_point.Z()) / BLOCKS_LEN;

    return Dot3(x, y, z);
}

const std::shared_ptr<Chunk> World::chunk(const Dot3 &index) const {
    return chunk(index.X(), index.Y(), index.Z());
}

const std::shared_ptr<Chunk> World::chunk(const Vec3F &world_pos) const {
    auto index = calculateChunkIndex(world_pos);
    return chunk(index.X(), index.Y(), index.Z());
}

World::~World() {

}
