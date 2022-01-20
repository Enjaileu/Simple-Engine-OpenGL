#include "Shader.h"
#include "Log.h"
#include <SDL.h>

#include <sstream>
#include <string>

void Shader::Unload()
{
    glDeleteProgram(id);
}

Shader& Shader::Use()
{
    glUseProgram(id);
    return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource,
    const GLchar* tessControlSource, const GLchar* tessEvalSource, const GLchar* geometrySource)
{
    CompileVertexShader(vertexSource);
    bool tessExists = CompileTessControlShader(tessControlSource);
    tessExists &= CompileTessEvalShader(tessEvalSource);
    bool gsExists = CompileGeometryShader(geometrySource);
    CompileFragmentShader(fragmentSource);
    CreateShaderProgram(tessExists, gsExists);
    PrintAllParams(id);
}

void Shader::CompileVertexShader(const GLchar* vertex_source)
{
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_source, NULL);
    glCompileShader(vs);
    CheckShaderErrors(vs, "vertex");
}

void Shader::CompileFragmentShader(const GLchar* fragment_source)
{
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_source, NULL);
    glCompileShader(fs);
    CheckShaderErrors(fs, "fragment");
}

bool Shader::CompileTessControlShader(const GLchar* tessControlSource) {
    if (tessControlSource == nullptr) {
        return false;
    }

    tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tcs, 1, &tessControlSource, NULL);
    glCompileShader(tcs);
    CheckShaderErrors(tcs, "tessellation control");
    return true;
}

bool Shader::CompileTessEvalShader(const GLchar* tessEvalSource) {
    if (tessEvalSource == nullptr) {
        return false;
    }

    tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tes, 1, &tessEvalSource, NULL);
    glCompileShader(tes);
    CheckShaderErrors(tes, "tessellation evaluation");
    return true;
}

bool Shader::CompileGeometryShader(const GLchar* geometry_source)
{
    if (geometry_source == nullptr)
    {
        return false;
    }

    gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, &geometry_source, NULL);
    glCompileShader(gs);
    CheckShaderErrors(gs, "geometry");

    return true;
}

void Shader::CreateShaderProgram(bool tessShadersExist, bool geometryShaderExists)
{
    // Create program
    id = glCreateProgram();
    glAttachShader(id, fs);
    if (tessShadersExist)
    {
        glAttachShader(id, tcs);
        glAttachShader(id, tes);
    }
    if (geometryShaderExists)
    {
        glAttachShader(id, gs);
    }
    glAttachShader(id, vs);
    glLinkProgram(id);

    // Check for linking error
    int params = -1;
    glGetProgramiv(id, GL_LINK_STATUS, &params);
    if (params != GL_TRUE)
    {
        std::ostringstream s;
        s << "Could not link shader programme GL index " << id;
        Log::error(LogCategory::Render, s.str());
        PrintProgrammeInfoLog(id);
    }
    if (!IsValid(id))
    {
        std::ostringstream s;
        s << "Could not validate shader " << id;
        Log::error(LogCategory::Render, s.str());
    }

    // Delete shaders for they are no longer used
    glDeleteShader(vs);
    if (tessShadersExist)
    {
        glDeleteShader(tcs);
        glDeleteShader(tes);
    }
    if (geometryShaderExists)
    {
        glDeleteShader(gs);
    }
    glDeleteShader(fs);
}

void Shader::SetFloat(const GLchar* name, GLfloat value)
{
    glUniform1f(glGetUniformLocation(id, name), value);
}
void Shader::SetInteger(const GLchar* name, GLint value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}
void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y)
{
    glUniform2f(glGetUniformLocation(id, name), x, y);
}
void Shader::SetVector2f(const GLchar* name, const Vector2& value)
{
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

void Shader::PrintShaderInfoLog(GLuint shaderIndex)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog(shaderIndex, max_length, &actual_length, log);
    std::ostringstream s;
    s << "Shader info log for GL index " << shaderIndex;
    Log::info(s.str());
}

void Shader::PrintProgrammeInfoLog(GLuint id)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetProgramInfoLog(id, max_length, &actual_length, log);
    std::ostringstream s;
    s << "Program info log for GL index " << id;
    Log::info(s.str());
}

void Shader::CheckShaderErrors(GLuint shader, std::string shaderType)
{
    int params = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE)
    {
        std::ostringstream s;
        s << "GL " << shaderType << " shader index " << shader << " did not compile.";
        Log::error(LogCategory::Render, s.str());
        PrintShaderInfoLog(shader);
    }
}

const char* Shader::GLTypeToString(GLenum type)
{
    switch (type)
    {
    case GL_BOOL:
        return "bool";
    case GL_INT:
        return "int";
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    case GL_SAMPLER_2D:
        return "sampler2D";
    case GL_SAMPLER_3D:
        return "sampler3D";
    case GL_SAMPLER_CUBE:
        return "samplerCube";
    case GL_SAMPLER_2D_SHADOW:
        return "sampler2DShadow";
    default:
        break;
    }
    return "other";
}

void Shader::PrintAllParams(GLuint id)
{
    Log::info("-----------------------------");
    std::ostringstream s;
    s << "Shader programme " << id << " info: ";
    Log::info(s.str());
    int params = -1;
    glGetProgramiv(id, GL_LINK_STATUS, &params);
    s.str("");
    s << "GL_LINK_STATUS = " << params;
    Log::info(s.str());

    glGetProgramiv(id, GL_ATTACHED_SHADERS, &params);
    s.str("");
    s << "GL_ATTACHED_SHADERS = " << params;
    Log::info(s.str());

    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &params);
    s.str("");
    s << "GL_ACTIVE_ATTRIBUTES = " << params;
    Log::info(s.str());
    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(id, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[77];
                sprintf_s(long_name, "%s[%i]", name, j);
                int location = glGetAttribLocation(id, long_name);
                std::ostringstream s;
                s << "  " << i << ") type:" << GLTypeToString(type) << " name:" << long_name << " location:" << location;
                Log::info(s.str());
            }
        }
        else
        {
            int location = glGetAttribLocation(id, name);
            std::ostringstream s;
            s << "  " << i << ") type:" << GLTypeToString(type) << " name:" << name << " location:" << location;
            Log::info(s.str());
        }
    }

    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &params);
    s.str("");
    s << "GL_ACTIVE_UNIFORMS = " << params;
    Log::info(s.str());
    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(id, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[77];
                sprintf_s(long_name, "%s[%i]", name, j);
                int location = glGetUniformLocation(id, long_name);
                std::ostringstream s;
                s << "  " << i << ") type:" << GLTypeToString(type) << " name:" << long_name << " location:" << location;
                Log::info(s.str());
            }
        }
        else
        {
            int location = glGetUniformLocation(id, name);
            std::ostringstream s;
            s << "  " << i << ") type:" << GLTypeToString(type) << " name:" << name << " location:" << location;
            Log::info(s.str());
        }
    }
    PrintProgrammeInfoLog(id);
}

bool Shader::IsValid(GLuint id)
{
    glValidateProgram(id);
    int params = -1;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &params);
    Log::info("");
    std::ostringstream s;
    s << "program " << id << " GL_VALIDATE_STATUS = " << params;
    Log::info(s.str());
    if (params != GL_TRUE)
    {
        PrintProgrammeInfoLog(id);
        return false;
    }
    return true;
}