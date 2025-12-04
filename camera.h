#pragma once

#include "glm/glm.hpp"

class Camera {
    public:
        Camera() {};
        Camera(glm::vec3 position, glm::vec3 rotation, float sensorDist, glm::vec2 aspectRatio, float pixelWidth)
         : position(position), rotation(rotation), sensorDist(sensorDist), aspectRatio(aspectRatio), pixelWidth(pixelWidth) {};
        

        glm::vec3 position;
        glm::vec3 rotation;
        float sensorDist;
        float pixelWidth;
        glm::vec2 aspectRatio;
    private:
        
};