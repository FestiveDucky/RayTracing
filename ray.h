#pragma once

#include "glm/glm.hpp"
#include "object.h"

class Ray {
    public:
        Ray(const glm::vec3& origin, const glm::vec3& direction, Object* collisionObject, float collisionT)
            : origin(origin), direction(glm::normalize(direction)), collisionObject(collisionObject), collisionT(collisionT) {};
        
        glm::vec3 getPoint(float t) const {
            return origin + t * direction;
        };

        glm::vec3 origin;
        glm::vec3 direction;
        Object* collisionObject;
        float collisionT;
        float lightIntensity;
        glm::vec3 color = glm::vec3(255.0f);

};