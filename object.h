#pragma once

#include "glm/glm.hpp"

class Object {
    public:
        Object() {};
        Object(glm::vec3 position, float radius, glm::vec3 color, float lightEmission = 0.0f)
            : position(position), radius(radius), color(color), lightEmission(lightEmission) {};
        glm::vec3 position;
        float radius;
        glm::vec3 color;
        float lightEmission;
    private:

};