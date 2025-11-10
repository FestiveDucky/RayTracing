#include <iostream>
#include "glm/glm.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "json/nlohmann/json.hpp"
#include "tinyobj/tiny_obj_loader.h"

using namespace std;

int main() {
    glm::vec3 test(1.0f, 2.0f, 3.0f);
    cout << "Test working!" << endl;

    // Example JSON
    nlohmann::json j = { {"example", 123} };
    cout << j.dump(4) << endl;

    // Example STB write (tiny image)
    unsigned char img[3*3*3] = { 255,0,0, 0,255,0, 0,0,255, 255,255,0, 0,255,255, 255,0,255, 128,128,128, 64,64,64, 0,0,0 };
    stbi_write_png("out.png", 3, 3, 3, img, 3*3);
    

    return 0;
}
