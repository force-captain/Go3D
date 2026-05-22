#include "graphics/camera.hpp"
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "graphics/ray.hpp"

Camera::Camera(glm::vec3 target, float distance, float yaw, float pitch, float fov, float nearClip, float farClip)
    : target(target), distance(distance), yaw(yaw), pitch(pitch), fov(fov), nearClip(nearClip), farClip(farClip) {}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(getPosition(), target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix(float width, float height) const {
    float aspect = static_cast<float>(width) / static_cast<float>(height);
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

Ray Camera::screenPoint(double mx, double my, float width, float height) const {
    glm::mat4 view = getViewMatrix();
    glm::mat4 proj = getProjectionMatrix(width, height);
    glm::vec4 viewport(0,0,width,height);

    glm::vec3 nearWorld = glm::unProject(glm::vec3(mx, my, 0.0f), view, proj, viewport);
    glm::vec3 farWorld  = glm::unProject(glm::vec3(mx, my, 1.0f), view, proj, viewport);

    glm::vec3 dir = glm::normalize(farWorld - getPosition());

    // Ray
    return Ray(getPosition(), dir);
}
