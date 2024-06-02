#include "Client/State.h"
#include "Client/VoxelState.h"
#include "Client/Camera.h"

#include "Common/Keys.h"

#include <vector>

using namespace Client;

static uint64_t RequestId = 1;
static bool FrustumCull = true;
static bool CullFaces = true;

VoxelState::VoxelState(const Common::World *world) : voxelprog("shaders/voxel.vert", "shaders/voxel.frag"), text("shaders/text.vert", "shaders/text.frag"), world(world), inputState(0), clientId(0), secret(""), origin(2, 8, 8), tickTime(0), gameTime(0) {
}

void VoxelState::onRender(State &state, const uint64_t time) {
    auto renderer = state.getRenderer();

    static std::vector<Common::Colour> colours = {
        Common::Colour::DimGrey,
        Common::Colour::SandyBrown,
        Common::Colour::SaddleBrown,
        Common::Colour::ForestGreen,
        Common::Colour::DodgerBlue,
        Common::Colour::LightCyan,
    };

    int left = std::min(time, tickTime);
    tickTime -= left;

    float step = (float)(MS_PER_UPDATE - tickTime)/(float)MS_PER_UPDATE;
    auto potential = oldOrigin + (velocity * step);

    auto projection = MatrixF::Projection(45.0f, 640.0f/360.0f, 0.1f, 100.0f);
    auto camera = Camera(potential, viewAngles);
    auto frustum = camera.getFrustum(16.0f/9.0f, 65, 100);
    auto view = camera.look();

    auto model = MatrixF::Scaling(1.0);
    auto mvp = model * view * projection;

    voxelprog.use();
    voxelprog("mvp", mvp);
    voxelprog("colours", colours);

    int total = 0;

    Dot3 client_index = world->calculateChunkIndex(potential);

    std::vector<Dot3> frustum_culled_list;

    for (int wy = client_index.Y()-5; wy < client_index.Y()+6; wy++) {
        for (int wz = client_index.Z()-5; wz < client_index.Z()+6; wz++) {
            for (int wx = client_index.X()-5; wx < client_index.X()+6; wx++) {
                auto chunk_index = Dot3(wx, wy, wz);
                auto chunk = world->chunk(chunk_index);

                if (chunk) {
                    if (FrustumCull)
                        if (client_index != chunk_index && !frustum.isInside(VEC3F(chunk_index * BLOCKS_LEN), VEC3F(((chunk_index + Dot3(1, 1, 1) * BLOCKS_LEN)))))
                            continue;

                    frustum_culled_list.push_back(chunk_index); 

                    auto world_offset = VEC3F(chunk_index * BLOCKS_LEN);

                    for (int face_index = 0; face_index < Renderer::VOXEL_COUNT; face_index++) {
                        if (CullFaces) {
                            if (face_index == Renderer::VOXEL_FRONT) {
                                auto normal = Vec3F(0, 0, -1);

                                auto plane = PlaneF(normal, world_offset + Vec3F(0, 0, BLOCKS_LEN));

                                if (plane.isOutside(potential))
                                    continue;
                            } else if (face_index == Renderer::VOXEL_BACK) {
                                auto normal = Vec3F(0, 0, 1);

                                auto plane = PlaneF(normal, world_offset);
                                if (plane.isOutside(potential))
                                    continue;
                            } else if (face_index == Renderer::VOXEL_LEFT) {
                                auto normal = Vec3F(-1, 0, 0);

                                auto plane = PlaneF(normal, world_offset + Vec3F(BLOCKS_LEN, 0, 0));

                                if (plane.isOutside(potential))
                                    continue;
                            } else if (face_index == Renderer::VOXEL_RIGHT) {
                                auto normal = Vec3F(1, 0, 0);

                                auto plane = PlaneF(normal, world_offset);
                                if (plane.isOutside(potential))
                                    continue;
                            } else if (face_index == Renderer::VOXEL_BOTTOM) {
                                auto normal = Vec3F(0, -1, 0);

                                auto plane = PlaneF(normal, world_offset + Vec3F(0, BLOCKS_LEN, 0));

                                if (plane.isOutside(potential))
                                    continue;
                            } else if (face_index == Renderer::VOXEL_TOP) {
                                auto normal = Vec3F(0, 1, 0);

                                auto plane = PlaneF(normal, world_offset);
                                if (plane.isOutside(potential))
                                    continue;
                            }
                        }

                        auto face_data = chunk->visible_faces(face_index);

                        voxelprog("world_offset", world_offset);
                        voxelprog("face_type", face_index);

                        total += face_data.size();
                        voxel.drawFaces(face_index, face_data);
                    }
                }
            }
        }
    }

    uint32_t fps = 1000000/(time|1);

    auto screen = renderer->screenTransformation(10, 10, 16, 16);
    text.draw("FPS " + std::to_string(fps) + " " + std::to_string(total), Common::Colour::White, screen);
}

void VoxelState::onTick(State &state, const uint64_t time) {
    tickTime = time;

    velocity = Vec3F();
    oldOrigin = origin;

    if (inputState & Actions::TurnLeft) {
        viewAngles = viewAngles - Vec3F(0, 1.5, 0);
    }

    if (inputState & Actions::TurnRight) {
        viewAngles = viewAngles + Vec3F(0, 1.5, 0);
    }

    angles = viewAngles.withZ(0);

    ClientGameStateRequest request;
    request.id = RequestId++;
    request.client_id = clientId;
    request.state = inputState;
    request.timeDelta = time;
    origin.copy(request.origin);
    angles.copy(request.angles);

    auto response = state.getGameState()->update(request);
    origin = Vec3F(response.new_origin);
    angles = Vec3F(response.new_angles);
    velocity = Vec3F(response.velocity);
    inputState = response.state;

    gameTime = response.time;
}

void VoxelState::onMouseMove(State &state, const MouseMove &event) {

}

void VoxelState::onMouseButtonPress(State &state, const MouseClick &event) {

}

void VoxelState::onMouseButtonRelease(State &state, const MouseClick &event) {

}

void VoxelState::onKeyDown(State &state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::W) {
        inputState |= Actions::StepForward;
    } else if (event.keyCode == Common::Keys::S) {
        inputState |= Actions::StepBackward;
    } else if (event.keyCode == Common::Keys::A) {
        inputState |= Actions::StepLeft;
    } else if (event.keyCode == Common::Keys::D) {
        inputState |= Actions::StepRight;
    } else if (event.keyCode == Common::Keys::Left) {
        inputState |= Actions::TurnLeft;
    } else if (event.keyCode == Common::Keys::Right) {
        inputState |= Actions::TurnRight;
    } else if (event.keyCode == Common::Keys::Up) {
        inputState |= Actions::StepForward;
    } else if (event.keyCode == Common::Keys::Down) {
        inputState |= Actions::StepBackward;
    } else if (event.keyCode == Common::Keys::P) {
        static bool line_mode = false;
        line_mode = !line_mode;

        if (line_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (event.keyCode == Common::Keys::O) {
        FrustumCull = !FrustumCull;
    } else if (event.keyCode == Common::Keys::L) {
        CullFaces = !CullFaces;
    }
}

void VoxelState::onKeyUp(State &state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::W) {
        inputState &= ~Actions::StepForward;
    } else if (event.keyCode == Common::Keys::S) {
        inputState &= ~Actions::StepBackward;
    } else if (event.keyCode == Common::Keys::A) {
        inputState &= ~Actions::StepLeft;
    } else if (event.keyCode == Common::Keys::D) {
        inputState &= ~Actions::StepRight;
    } else if (event.keyCode == Common::Keys::Left) {
        inputState &= ~Actions::TurnLeft;
    } else if (event.keyCode == Common::Keys::Right) {
        inputState &= ~Actions::TurnRight;
    } else if (event.keyCode == Common::Keys::Up) {
        inputState &= ~Actions::StepForward;
    } else if (event.keyCode == Common::Keys::Down) {
        inputState &= ~Actions::StepBackward;
    }
}

void VoxelState::onTouchPress(State &state, const TouchEvent &event) {
}

void VoxelState::onTouchRelease(State &state, const TouchEvent &event) {

}

void VoxelState::onTouchMotion(State &state, const TouchEvent &event) {

}

void VoxelState::onEnterState(State &state, std::any data) {
    if (!data.has_value()) {
        ClientConnectRequest request;
        std::string username = randomString(16);

        std::memset(request.username, 0, sizeof(request.username));
        std::strncpy(request.username, username.c_str(), std::min(username.size(), sizeof(request.username)-1));

        auto response = state.getGameState()->clientConnect(request);

        clientId = response.id;
        secret = std::string(response.secret);
    } else if (data.type() == typeid(ClientConnectResponse)) {
        auto response = std::any_cast<ClientConnectResponse>(data);

        clientId = response.id;
        secret = std::string(response.secret);
    }

}

