#pragma once

#include <glm/glm.hpp>
#include <memory>

enum LightType {
    Point,
    Spot,
    Directional
};

class Light {
    private:
        LightType type;

        float intensity = 1.0f;

        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 colour = glm::vec3(1.0f);
        
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

    public:
        Light(  LightType type, 
                float intensity = 1.0f,
                const glm::vec3& pos = glm::vec3(0.0f),
                const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f),
                const glm::vec3& colour = glm::vec3(1.0f)
             );

        LightType getType() const { return type; }

        const glm::vec3& getPos() const { return position; }
        const glm::vec3& getDir() const { return direction; }
        const glm::vec3& getClr() const { return colour; }

        float getIntensity() const { return intensity; }
        float getConstant() const { return constant; }
        float getLinear() const { return linear; }
        float getQuadratic() const { return quadratic; }

        void setPos(const glm::vec3& pos) { position = pos; }
        void setDir(const glm::vec3& dir) { direction = dir; }
        void setClr(const glm::vec3& clr) { colour = clr; }
        
        void setIntensity(float i) { intensity = i; }

        void translate(const glm::vec3& offset) { position += offset; }
};

