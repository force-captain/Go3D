#include <glm/glm.hpp>

enum LightType {
    Point,
    Spot,
    Directional
};

class Light {
    private:
        LightType type;
        float intensity;
        glm::vec3 position;
    public:
        Light(LightType type, float intensity, const glm::vec3& pos);
        ~Light();
        void translate(const glm::vec3& offset) { position += offset; }
        glm::vec3 getPos() const { return position; }
};

