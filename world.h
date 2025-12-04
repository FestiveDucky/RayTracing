#pragma once

#include "glm/glm.hpp"
#include "object.h"
#include "camera.h"
#include "ray.h"
#include <random>
#include <chrono>

class World {
    public:
        World();
        std::vector<Object> objects;
        void generateImage();
        void checkIntersection(Ray& ray);
        Object noHitObject; // Object to return when no intersection occurs
        int maxBounces;
        std::normal_distribution<double> dist; 

        unsigned seed;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
    private:
        Camera camera;
};