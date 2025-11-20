#pragma once

#include <glm/fwd.hpp>
#include <memory>
#include "graphics/objects/shader.hpp"
#include "graphics/objects/mesh.hpp"


class Object {
    protected:
        std::shared_ptr<Material> mat;
        BoundingBox bounds;
        std::shared_ptr<Mesh> mesh;
        std::function<void()> onClick;
        std::function<void()> onHover;
        std::function<void()> onUnhover;
        std::function<void(float)> onUpdate;
        void initBounds();
    public:
        virtual ~Object() = default;

        bool prevClick;
        bool prevHover;

        virtual glm::mat4 getModelMatrix() const = 0;
        virtual void update(float deltaTime) = 0;

        BoundingBox getWorldBoundingBox() const;
        
        void setMaterial(const std::shared_ptr<Material>& newMat) {mat = newMat; }
        const std::shared_ptr<Material>& getMaterial() const { return mat; }

        void triggerClick() { if (onClick) onClick(); }
        void triggerHover() { if (onHover) onHover(); }
        void triggerUnhover() { if (onUnhover) onUnhover(); }

        void setClick(std::function<void()> f) { onClick = f; }
        void setHover(std::function<void()> f) { onHover = f; }
        void setUnhover(std::function<void()> f) { onUnhover = f; }
        void setUpdate(std::function<void(float)> f) { onUpdate = f; }
};
