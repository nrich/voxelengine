#ifndef __RENDERER_PROGRAM_H__
#define __RENDERER_PROGRAM_H__

#include <cstdint>
#include <string>
#include <vector>

#include "Common/Shared.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Point4.h"
#include "Math/Matrix.h"
#include "Common/Colour.h"

namespace Renderer {

class Program {
    GLuint programId;

    static GLuint CompileShader(const std::string &shader_source, GLenum shaderType);
    static GLuint LinkProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
public:
    Program(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename);
    Program(const std::string &shader_filename);

    Program(Program&& other);

    void use() const;

    const Program *operator()(const std::string &name, const float value) const;
    const Program *operator()(const std::string &name, const std::vector<float> &value) const;
    const Program *operator()(const std::string &name, const int32_t value) const;
    const Program *operator()(const std::string &name, const std::vector<int32_t> &value) const;
    const Program *operator()(const std::string &name, const uint32_t value) const;
    const Program *operator()(const std::string &name, const std::vector<uint32_t> &value) const;
    const Program *operator()(const std::string &name, const Vec2F &value) const;
    const Program *operator()(const std::string &name, const std::vector<Vec2F> &value) const;
    const Program *operator()(const std::string &name, const Vec3F &value) const;
    const Program *operator()(const std::string &name, const std::vector<Vec3F> &value) const;
    const Program *operator()(const std::string &name, const Vec4F &value) const;
    const Program *operator()(const std::string &name, const std::vector<Vec4F> &value) const;
    const Program *operator()(const std::string &name, const MatrixF &value) const;
    const Program *operator()(const std::string &name, const std::vector<MatrixF> &value) const;
    const Program *operator()(const std::string &name, const Common::Colour &value) const;
    const Program *operator()(const std::string &name, const std::vector<Common::Colour> &value) const;
    const Program *operator()(const std::string &name, const GLfloat *value) const;

    ~Program();
}; 

}; // namespace Renderer

#endif //__RENDERER_PROGRAM_H__
