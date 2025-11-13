#include "graphics/camera.hpp"
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

Camera::Camera(glm::vec3 target, float distance, float yaw, float pitch, float fov, float aspect, float nearClip, float farClip)
    : target(target), distance(distance), yaw(yaw), pitch(pitch), fov(fov), aspect(aspect), nearClip(nearClip), farClip(farClip) {}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(getPosition(), target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix(float width, float height) const {
    return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}

glm::vec3 Camera::getPosition() const {
    glm::vec3 pos;
    pos.x = target.x + distance * cos(pitch) * sin(yaw);
    pos.y = target.y + distance * sin(pitch);
    pos.z = target.z + distance * cos(pitch) * cos(yaw);
    return pos;
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;

    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < minPitch) pitch = minPitch;
}

void Camera::lookAt(const glm::vec3& point) {
    target = point;
}
