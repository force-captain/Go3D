#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
    private:
        GLuint programID;

        GLuint compileShader(GLenum type, const std::string& source);
        
    public:
        Shader() : programID(0) {}
        ~Shader() { if (programID) glDeleteProgram(programID); }

        bool load(const std::string& vertexSource, const std::string& fragmentSource);

        void use() const { glUseProgram(programID); }

        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        GLuint getProgramID() const { return programID; }
};
