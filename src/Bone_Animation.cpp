#include "Bone_Animation.h"

Bone_Animation::Bone_Animation() { }

Bone_Animation::~Bone_Animation() { }

void Bone_Animation::init() {
    root_position = { 2.0f, 1.0f, 2.0f };

    scale_vector = {
        {1.0f, 1.0f, 1.0f},  // Red
        {0.5f, 4.0f, 0.5f},  // Yellow
        {0.5f, 3.0f, 0.5f},  // Magenta
        {0.5f, 2.0f, 0.5f}   // Cyan
    };

    // Initialize rotation angles for each bone
    rotation_degree_vector = {
        {0.0f, 0.0f, 0.0f}, // Red
        {0.0f, 0.0f, 0.0f}, // Yellow
        {0.0f, 0.0f, 0.0f}, // Magenta
        {0.0f, 0.0f, 0.0f}  // Cyan
    };

    colors = {
        {0.7f, 0.0f, 0.0f, 1.0f}, // Red
        {0.7f, 0.7f, 0.0f, 1.0f}, // Yellow
        {0.7f, 0.0f, 0.7f, 1.0f}, // Magenta
        {0.0f, 0.7f, 0.7f, 1.0f}  // Cyan
    };
}

void Bone_Animation::update(float delta_time) {
    // The update function would typically modify the bone's orientation based on rotation_degree_vector
    // Currently, we're just going to print the rotation angles
    for (const auto& rotation : rotation_degree_vector) {
        std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
    }
}

void Bone_Animation::reset() {
    // Reset rotation degrees to zero
    for (auto& rotation : rotation_degree_vector) {
        rotation = { 0.0f, 0.0f, 0.0f };
    }
}

std::vector<glm::vec3> Bone_Animation::getRotationAngles() const {
    return rotation_degree_vector; // Return the current rotation angles
}
