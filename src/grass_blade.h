#pragma once

#include <glm/glm.hpp>

class GrassBlade {
public:
    GrassBlade(const glm::vec3& position, float swayAmplitude = 0.1f, float swaySpeed = 1.0f, float greenLevel = 1.0f);

    const glm::vec3& getPosition() const;
    float getSwayAmplitude() const;
    float getSwaySpeed() const;
    float getGreenLevel() const;

private:
    glm::vec3 position;
    float swayAmplitude;
    float swaySpeed;
    float greenLevel; // Varying green component
};
