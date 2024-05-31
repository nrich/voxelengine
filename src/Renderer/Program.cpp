#include "Renderer/Program.h"

#include <fstream>
#include <sstream>
#include <exception>

using namespace Renderer;

Program::Program(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename) : programId(-1) {
    std::string vertex_shader_source;
    std::ifstream vertex_shader_stream(vertex_shader_filename, std::ios::in);
    if (vertex_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << vertex_shader_stream.rdbuf();
        vertex_shader_source = sstr.str();
        vertex_shader_stream.close();
    } else {
        throw std::runtime_error(std::string("Could not open vertext shader program ") + vertex_shader_filename);
    }

    std::string fragment_shader_source;
    std::ifstream fragment_shader_stream(fragment_shader_filename, std::ios::in);
    if (fragment_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << fragment_shader_stream.rdbuf();
        fragment_shader_source = sstr.str();
        fragment_shader_stream.close();
    } else {
        throw std::runtime_error(std::string("Could not open fragment shader program ") + fragment_shader_filename);
    }

    GLuint vertex_shader_id = CompileShader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = CompileShader(fragment_shader_source, GL_FRAGMENT_SHADER);

    programId = LinkProgram(vertex_shader_id, fragment_shader_id);
}

Program::Program(const std::string &shader_filename) : programId(-1) {
    std::string shader_source;
    std::ifstream shader_stream(shader_filename, std::ios::in);
    if (shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << shader_stream.rdbuf();
        shader_source = sstr.str();
        shader_stream.close();
    } else {
        throw std::runtime_error(std::string("Could not open shader program ") + shader_filename);
    }

    GLuint vertex_shader_id = CompileShader(shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = CompileShader(shader_source, GL_FRAGMENT_SHADER);

    programId = LinkProgram(vertex_shader_id, fragment_shader_id);
}

Program::Program(Program&& other) {
    programId = other.programId;
    other.programId = -1;
}

GLuint Program::CompileShader(const std::string &shader_source, GLenum shaderType) {
    GLuint shader_id = glCreateShader(shaderType);

    char const *shader_source_pointer = shader_source.c_str();

    glShaderSource(shader_id, 1, &shader_source_pointer, NULL);
    glCompileShader(shader_id);

    GLint compile_status = GL_FALSE;
    int info_log_length;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        std::vector<char> shader_error_message(info_log_length+1);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &shader_error_message[0]);
        throw std::runtime_error(std::string("Could not compile shader program: ") + std::string(&shader_error_message[0]));
    }

    return shader_id;
}

GLuint Program::LinkProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    GLuint program_id = glCreateProgram(); 

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    GLint link_status = GL_FALSE;
    int info_log_length;

    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> program_error_message(info_log_length+1);
        glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
        throw std::runtime_error(std::string("Could not link shader program: ") + std::string(&program_error_message[0]));
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);
    
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

void Program::use() const {
    glUseProgram(programId);
}

const Program *Program::operator()(const std::string &name, const float value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1f(loc, value);

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<float> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1fv(loc, value.size(), (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const int32_t value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1i(loc, value);

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<int32_t> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1iv(loc, value.size(), (const GLint *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const uint32_t value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1ui(loc, value);

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<uint32_t> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1uiv(loc, value.size(), (const GLuint *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const Vec2F &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform2f(loc, value.X(), value.Y());

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<Vec2F> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform2fv(loc, value.size(), (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const Vec3F &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform3f(loc, value.X(), value.Y(), value.Z());

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<Vec3F> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform3fv(loc, value.size(), (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const Vec4F &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform4f(loc, value.X(), value.Y(), value.Z(), value.W());

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<Vec4F> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniform4fv(loc, value.size(), (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const MatrixF &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<MatrixF> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, value.size(), GL_FALSE, (const GLfloat *)value.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const Common::Colour &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());

    glUniform4f(loc, (GLfloat)value.R()/255.0, (GLfloat)value.G()/255.0, (GLfloat)value.B()/255.0, (GLfloat)value.A()/255.0);

    return this;
}

const Program *Program::operator()(const std::string &name, const std::vector<Common::Colour> &value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());

    std::vector<std::array<GLfloat, 4>> gl_colours;
    gl_colours.resize(value.size());
    for (size_t i = 0; i < value.size(); i++) {
        gl_colours[i] = {(GLfloat)value[i].R()/(GLfloat)255.0, (GLfloat)value[i].G()/(GLfloat)255.0, (GLfloat)value[i].B()/(GLfloat)255.0, (GLfloat)value[i].A()/(GLfloat)255.0};
    }

    glUniform4fv(loc, gl_colours.size()*4, (const GLfloat *)gl_colours.data());

    return this;
}

const Program *Program::operator()(const std::string &name, const GLfloat *value) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);

    return this;
}

Program::~Program() {
    if (programId > 0)
        glDeleteProgram(programId);
}
