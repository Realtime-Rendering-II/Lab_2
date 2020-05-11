#ifndef LAB_2_LIGHT_H
#define LAB_2_LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 Position;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    glm::mat4 model_matrix;
};
#endif //LAB_2_LIGHT_H