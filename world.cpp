#include "world.h"
#include <iostream>
#include <fstream>
#include "json/nlohmann/json.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "util.h"
using namespace std;
using json = nlohmann::json;


World::World() : seed(std::chrono::system_clock::now().time_since_epoch().count()), generator(seed), distribution(0.0, 1.0) {
    ifstream file("data.json");
    json j;
    file >> j;

    maxBounces = j["settings"]["maxBounces"].get<int>();

    for (const auto& item : j["objects"]) {
        glm::vec3 position = glm::vec3(
            item["position"][0].get<float>(),
            item["position"][1].get<float>(),
            item["position"][2].get<float>()
        );
        float radius = item["radius"].get<float>();
        glm::vec3 color = glm::vec3(
            item["color"][0].get<float>(),
            item["color"][1].get<float>(),
            item["color"][2].get<float>()
        );
        float lightEmission = 0.0f;
        if (item.contains("lightEmission")) {
            lightEmission = item["lightEmission"].get<float>();
        }
        objects.emplace_back(position, radius, color, lightEmission);
    }

    camera = Camera(glm::vec3(j["camera"]["position"][0].get<float>(),
                              j["camera"]["position"][1].get<float>(),
                              j["camera"]["position"][2].get<float>()),
                    glm::vec3(j["camera"]["rotation"][0].get<float>(),
                              j["camera"]["rotation"][1].get<float>(),
                              j["camera"]["rotation"][2].get<float>()),
                    j["camera"]["sensorDist"].get<float>(),
                    glm::vec2(j["camera"]["imageSize"][0].get<float>(),
                              j["camera"]["imageSize"][1].get<float>()),
                    j["camera"]["pixelSize"].get<float>());
    
    // camera = Camera(glm::vec3(0.0f, 0.0f, -40.0f), glm::vec3(0.0f), 1.0f, glm::vec2(2000, 1000), 0.01f);

    noHitObject = Object(glm::vec3(0.0f), 0.0f, glm::vec3(0.0f));
};

void World::generateImage() {
    cout << "Generating Image...\n";
    int width = static_cast<int>(camera.aspectRatio.x);
    int height = static_cast<int>(camera.aspectRatio.y);

    std::vector<unsigned char> img(width * height * 3);
    cout << "Camera Aspect Ratio: " << camera.aspectRatio.x << " x " << camera.aspectRatio.y << "\n";

    for (int i = 0; i < camera.aspectRatio.y; i++) {
        for (int j = 0; j < camera.aspectRatio.x; j++) {
            
            float x = (-width/2.0f + j) * camera.pixelWidth;
            float y = (height/2.0f - i) * camera.pixelWidth;

            glm::vec3 dir = glm::normalize(glm::vec3(x, y, camera.position.z + camera.sensorDist) - camera.position);



            // Use camera position as origin
            Ray r = Ray(camera.position, dir, &noHitObject, std::numeric_limits<float>::max());
            int bounces = 0;
            while (bounces < maxBounces) {
                checkIntersection(r);
                if (r.collisionObject == &noHitObject) {
                    // || *(&r.collisionObject.lightEmission) > 0
                    break; // No intersection or light source, stop bouncing
                }

                // Merge color contribution
                r.color *= r.collisionObject->color / 255.0f;
                r.lightIntensity += r.collisionObject->lightEmission;
                r.lightIntensity = std::min(r.lightIntensity, 255.0f);

                // Determine new ray direction
                glm::vec3 hitPoint = r.getPoint(r.collisionT);
                glm::vec3 normal = glm::normalize(hitPoint - r.collisionObject->position);
                
                glm::vec3 randDir = glm::normalize(glm::vec3(distribution(generator), distribution(generator), distribution(generator)));

                randDir *= sign(dot(normal, randDir));


                r.origin = hitPoint + normal * 0.001f; // Offset to avoid self-intersection
                r.direction = randDir;

                
                bounces++;
            }

            // Ray r = checkIntersection(camera.position, dir);
            glm::vec3 c = r.color;
            r.lightIntensity = r.lightIntensity / 255.0f;
            
            int idx = (i * width + j) * 3;
            img[idx + 0] = static_cast<unsigned char>(c.r * r.lightIntensity);
            img[idx + 1] = static_cast<unsigned char>(c.g * r.lightIntensity);
            img[idx + 2] = static_cast<unsigned char>(c.b * r.lightIntensity);
        }
    }
    
    cout << "Image Generated!\n";
    stbi_write_png("image.png", camera.aspectRatio.x, camera.aspectRatio.y, 3, img.data(), 3*camera.aspectRatio.x);
}



void World::checkIntersection(Ray& ray) {
    Object* first = &noHitObject;
    float closestT = std::numeric_limits<float>::max();

    for (auto& obj : objects) {
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(ray.direction, ray.origin - obj.position);
        float c = glm::dot(ray.origin - obj.position, ray.origin - obj.position) - obj.radius * obj.radius;
        
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            // No intersection
            continue;
        } else {
            // cout << discriminant << "\n";
            float t0, t1;
            t0 = (-b - sqrt(discriminant)) / (2.0f * a);
            t1 = (-b + sqrt(discriminant)) / (2.0f * a);
            if (t0 > 0 && t0 < closestT) {
                // cout << "Hit object at distance: " << t0 << "\n";
                closestT = t0;
                first = &obj;
            } else if (t1 > 0 && t1 < closestT) {
                // cout << "Hit object at distance: " << t1 << "\n";
                closestT = t1;
                first = &obj;
            }
        }
    }
    bool a = (first == &noHitObject);
    ray.collisionObject = first;
    ray.collisionT = closestT;
}