#include "grass_blade.h"

GrassBlade::GrassBlade(const glm::vec3& position, float swayAmplitude, float swaySpeed, float greenLevel)
    : position(position), swayAmplitude(swayAmplitude), swaySpeed(swaySpeed), greenLevel(greenLevel) {}

const glm::vec3& GrassBlade::getPosition() const {
    return position;
}

float GrassBlade::getSwayAmplitude() const {
    return swayAmplitude;
}

float GrassBlade::getSwaySpeed() const {
    return swaySpeed;
}

float GrassBlade::getGreenLevel() const {
    return greenLevel;
}
