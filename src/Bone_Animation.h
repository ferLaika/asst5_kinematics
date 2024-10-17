#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

class Bone_Animation
{
public:
    Bone_Animation();
    ~Bone_Animation();

    void init();
    void update(float delta_time);
    void reset();

    // Additional function to get rotation vectors
    std::vector<glm::vec3> getRotationAngles() const;

public:
    // Here the head of each vector is the root bone
    std::vector<glm::vec3> scale_vector;          // Scale for each bone
    std::vector<glm::vec3> rotation_degree_vector; // Rotations for each bone
    std::vector<glm::vec4> colors;                // Colors for visualization

    glm::vec3 root_position;                       // Root position of the entire bone structure
};
