#include "graphics/light.hpp"

Light::Light(LightType type, float intensity, const glm::vec3& pos) : type(type), intensity(intensity), position(pos) {

}

Light::~Light() {

}
