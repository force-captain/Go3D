#include "graphics/light.hpp"

Light::Light(   LightType type,
                float intensity,
                const glm::vec3& pos,
                const glm::vec3& dir,
                const glm::vec3& clr)
    : type(type), intensity(intensity),
      position(pos), direction(dir), colour(clr) {}
