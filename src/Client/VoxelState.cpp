#include "Client/State.h"
#include "Client/VoxelState.h"
#include "Client/Camera.h"
#include "Client/ChangeState.h"

#include "Common/Keys.h"

#include <vector>

using namespace Client;

static uint64_t RequestId = 1;
static bool FrustumCull = true;
static bool BackfaceCull = true;
static bool OcclusionCull = true;

static const int Z_BUFFER_WIDTH = 160;
static const int Z_BUFFER_HEIGHT = 90;

static std::array<uint32_t, Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> zbuffer = {0};

VoxelState::VoxelState(const Common::World *world) : voxelprog("shaders/voxel.vert", "shaders/voxel.frag"), text("shaders/text.vert", "shaders/text.frag"), colourBuffer("shaders/colour_buffer.vert", "shaders/colour_buffer.frag"), world(world), inputState(0), clientId(0), secret(""), origin(2, 8, 8), tickTime(0), gameTime(0) {
}

static Dot world_to_screen(const Dot3 &world_pos, const MatrixF &view_matrix, const MatrixF &projection_matrix, int screen_width, int screen_height) {
    auto pos = Vec4F(VEC3F(world_pos), 1.0);
    auto v = projection_matrix * (view_matrix * pos);

    if (v.Z() < 0)
        return Dot(-1, -1);

    auto screen_pos = v.XYZ();
    if (v.W() != 0)
        screen_pos = screen_pos / v.W();

    auto screen_x = screen_pos.X()/2 * screen_width/2 + screen_width/2;
    auto screen_y = screen_height - (screen_pos.Y()/2 * screen_height/2 + screen_height/2);

    return Dot(screen_x, screen_y);
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
    auto frustum = camera.getFrustum(16.0f/9.0f, 45, 100);
    auto view = camera.look();

    auto model = MatrixF::Scaling(1.0);
    auto mvp = model * view * projection;

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
                        if (client_index != chunk_index && !frustum.isInside(VEC3F(chunk_index * BLOCKS_LEN), VEC3F(((chunk_index + Dot3(1, 1, 1)) * BLOCKS_LEN))))
                            continue;

                    frustum_culled_list.push_back(chunk_index); 
                }
            }
        }
    }

    std::sort(frustum_culled_list.begin(), frustum_culled_list.end(), [client_index](const Dot3 &a, const Dot3& b) {
        auto a_len = (a - client_index).length();
        auto b_len = (b - client_index).length();

        return a_len < b_len;
    });

    voxelprog.use();
    voxelprog("mvp", mvp);
    voxelprog("colours", colours);

    if (OcclusionCull) {
        std::fill(zbuffer.begin(), zbuffer.end(), 0);

        for (const auto &chunk_index : frustum_culled_list) {
            auto chunk = world->chunk(chunk_index);
            auto world_offset = VEC3F(chunk_index) * BLOCKS_LEN;

            for (int face_index = 0; face_index < Renderer::VOXEL_COUNT; face_index++) {
                std::array<uint32_t, Z_BUFFER_WIDTH * Z_BUFFER_HEIGHT> depth_buffer;
                std::fill(depth_buffer.begin(), depth_buffer.end(), 0);

                auto visible_faces = chunk->visibleFaces(face_index);
                std::vector<uint32_t> face_data;

                for (const uint32_t packed_voxel : visible_faces) {
                    std::vector<Dot3> points;
                    Dot3 first = Renderer::Voxel::Decode(packed_voxel);

                    points.push_back(first);

                    auto world_postion = world_offset + VEC3F(first);

                    if (face_index == Renderer::VOXEL_BOTTOM) {
                        auto normal = Vec3F(0, 1, 0);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(1, 0, 0));
                        points.push_back(first + Dot3(0, 0, 1));
                        points.push_back(first + Dot3(1, 0, 1));
                    } else if (face_index == Renderer::VOXEL_TOP) {
                        auto normal = Vec3F(0, -1, 0);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(1, 0, 0));
                        points.push_back(first + Dot3(0, 0, 1));
                        points.push_back(first + Dot3(1, 0, 1));
                    } else if (face_index == Renderer::VOXEL_LEFT) {
                        auto normal = Vec3F(1, 0, 0);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(0, 0, 1));
                        points.push_back(first + Dot3(0, 1, 0));
                        points.push_back(first + Dot3(0, 1, 1));
                    } else if (face_index == Renderer::VOXEL_RIGHT) {
                        auto normal = Vec3F(-1, 0, 0);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(0, 0, 1));
                        points.push_back(first + Dot3(0, 1, 0));
                        points.push_back(first + Dot3(0, 1, 1));
                    } else if (face_index == Renderer::VOXEL_BACK) {
                        auto normal = Vec3F(0, 0, -1);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(1, 0, 0));
                        points.push_back(first + Dot3(0, 1, 0));
                        points.push_back(first + Dot3(1, 1, 0));
                    } else if (face_index == Renderer::VOXEL_FRONT) {
                        auto normal = Vec3F(0, 0, 1);
                        if ((world_postion - potential) % normal < 0)
                            continue;

                        points.push_back(first + Dot3(1, 0, 0));
                        points.push_back(first + Dot3(0, 1, 0));
                        points.push_back(first + Dot3(1, 1, 0));
                    }

                    auto screen_pos_0 = world_to_screen(chunk_index * BLOCKS_LEN + points[0], view, projection, Z_BUFFER_WIDTH, Z_BUFFER_HEIGHT);
                    auto screen_pos_1 = world_to_screen(chunk_index * BLOCKS_LEN + points[1], view, projection, Z_BUFFER_WIDTH, Z_BUFFER_HEIGHT);
                    auto screen_pos_2 = world_to_screen(chunk_index * BLOCKS_LEN + points[2], view, projection, Z_BUFFER_WIDTH, Z_BUFFER_HEIGHT);
                    auto screen_pos_3 = world_to_screen(chunk_index * BLOCKS_LEN + points[3], view, projection, Z_BUFFER_WIDTH, Z_BUFFER_HEIGHT);

                    if ((screen_pos_0.X() < 0 || screen_pos_0.X() >= Z_BUFFER_WIDTH || screen_pos_0.Y() < 0 || screen_pos_0.Y() >= Z_BUFFER_HEIGHT) &&
                        (screen_pos_1.X() < 0 || screen_pos_1.X() >= Z_BUFFER_WIDTH || screen_pos_1.Y() < 0 || screen_pos_1.Y() >= Z_BUFFER_HEIGHT) &&
                        (screen_pos_2.X() < 0 || screen_pos_2.X() >= Z_BUFFER_WIDTH || screen_pos_2.Y() < 0 || screen_pos_2.Y() >= Z_BUFFER_HEIGHT) &&
                        (screen_pos_3.X() < 0 || screen_pos_3.X() >= Z_BUFFER_WIDTH || screen_pos_3.Y() < 0 || screen_pos_3.Y() >= Z_BUFFER_HEIGHT))
                        continue;

                    screen_pos_0 = Dot::BuildMax(screen_pos_0, Dot(0, 0));
                    screen_pos_0 = Dot::BuildMin(screen_pos_0, Dot(Z_BUFFER_WIDTH-1, Z_BUFFER_HEIGHT-1));

                    screen_pos_1 = Dot::BuildMax(screen_pos_1, Dot(0, 0));
                    screen_pos_1 = Dot::BuildMin(screen_pos_1, Dot(Z_BUFFER_WIDTH-1, Z_BUFFER_HEIGHT-1));

                    screen_pos_2 = Dot::BuildMax(screen_pos_2, Dot(0, 0));
                    screen_pos_2 = Dot::BuildMin(screen_pos_2, Dot(Z_BUFFER_WIDTH-1, Z_BUFFER_HEIGHT-1));

                    screen_pos_3 = Dot::BuildMax(screen_pos_3, Dot(0, 0));
                    screen_pos_3 = Dot::BuildMin(screen_pos_3, Dot(Z_BUFFER_WIDTH-1, Z_BUFFER_HEIGHT-1));

                    Common::Block block = chunk->block(first);

                    bool draw_any = false;

                    int min_x = screen_pos_0.X();
                    int max_x = screen_pos_0.X();
                    int min_y = screen_pos_0.Y();
                    int max_y = screen_pos_0.Y();

                    min_x = std::min(min_x, screen_pos_1.X());
                    min_x = std::min(min_x, screen_pos_2.X());
                    min_x = std::min(min_x, screen_pos_3.X());

                    max_x = std::max(max_x, screen_pos_1.X());
                    max_x = std::max(max_x, screen_pos_2.X());
                    max_x = std::max(max_x, screen_pos_3.X());

                    min_y = std::min(min_y, screen_pos_1.Y());
                    min_y = std::min(min_y, screen_pos_2.Y());
                    min_y = std::min(min_y, screen_pos_3.Y());

                    max_y = std::max(max_y, screen_pos_1.Y());
                    max_y = std::max(max_y, screen_pos_2.Y());
                    max_y = std::max(max_y, screen_pos_3.Y());

                    for (int y = min_y; y <= max_y; y++) {
                        for (int x = min_x; x <= max_x; x++) {
                            if (zbuffer[Z_BUFFER_WIDTH * y + x])
                                continue;
                            zbuffer[Z_BUFFER_WIDTH * y + x] = (uint32_t)block;
                            depth_buffer[Z_BUFFER_WIDTH * y + x] = (uint32_t)block;
                            draw_any = true;
                        }
                    }

                    draw_any = true;
                    if (draw_any) {
                        //std::cerr << "(" << min_x << "," << min_y << "),(" << max_x << "," << max_y << ") " << (int)block << "\n";
                        face_data.push_back(packed_voxel);
                    }
                }

                voxelprog("world_offset", world_offset);
                voxelprog("face_type", face_index);

                total += face_data.size();
                voxel.drawFaces(face_index, face_data);
            }
        }

        auto screen = renderer->screenTransformation(10, 30, 128, 72);
        std::vector<uint32_t> screen_buffer;
        std::copy(zbuffer.begin(), zbuffer.end(), std::back_inserter(screen_buffer));
        colourBuffer.draw(screen_buffer, Z_BUFFER_WIDTH, Z_BUFFER_HEIGHT, colours, screen);
    } else {
        for (const auto &chunk_index : frustum_culled_list) {
            auto chunk = world->chunk(chunk_index);
            auto world_offset = VEC3F(chunk_index) * BLOCKS_LEN;

            for (int face_index = 0; face_index < Renderer::VOXEL_COUNT; face_index++) {
                if (BackfaceCull) {
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

                auto visible_faces = chunk->visibleFaces(face_index);

                voxelprog("world_offset", world_offset);
                voxelprog("face_type", face_index);

                total += visible_faces.size();
                voxel.drawFaces(face_index, visible_faces);
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
        BackfaceCull = !BackfaceCull;
    } else if (event.keyCode == Common::Keys::K) {
        OcclusionCull = !OcclusionCull;

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
    } else if (data.type() == typeid(ChangeState)) {
        auto state = std::any_cast<ChangeState>(data);

        origin = state.origin;
        angles = state.angles;
        viewAngles = state.viewAngles;
        clientId = state.clientId;
        secret = state.secret;
    } else if (data.type() == typeid(ClientConnectResponse)) {
        auto response = std::any_cast<ClientConnectResponse>(data);

        clientId = response.id;
        secret = std::string(response.secret);
    }

}

