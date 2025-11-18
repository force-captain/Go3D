#pragma once

#include <glm/glm.hpp>

class Ray;

class Camera {
    private:
        glm::vec3 target;
        glm::vec3 rotation;

        float distance;
        float yaw;
        float pitch;

        float maxPitch = glm::radians(85.0f);
        float minPitch = glm::radians(-85.0f);

        float fov;
        float aspect;
        float nearClip;
        float farClip;
    public:
        Camera(glm::vec3 target, float distance, float yaw, float pitch, float fov, float aspect, float nearClip = 0.1f, float farClip = 100.0f);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float width=0, float height=0) const;

        void rotate(float deltaYaw, float deltaPitch);
        void lookAt(const glm::vec3& point);

        glm::vec3 getPosition() const;
        glm::vec3 getTarget() const { return target; }

        Ray screenPoint(double mx, double my, float width, float height) const;
};
