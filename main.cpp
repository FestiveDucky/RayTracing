#include <iostream>
#include "glm/glm.hpp"
#include "tinyobj/tiny_obj_loader.h"
#include "world.h"

using namespace std;

int main() {
    World world = World();
    world.generateImage();
    // todo: shoot multiple rays from each point and merge those together, add camera rotation, add general objects (triangle meshes) with stuff like walls
    // Check fast inverse sqrt for normal calculations


    
    

    return 0;
}
