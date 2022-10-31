#include <Agent.hpp>
#include "./config.hpp"

Agent::Agent() {
    float theta = glm::linearRand(0.0, 3.14 * 2.0);
    float radius = 0.99;
    float magnitude = glm::linearRand(0.0, 1.0);
    float aspectRatio = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    position = glm::vec2(sin(theta) * radius * magnitude, cos(theta) * radius * magnitude * aspectRatio);
    direction = -glm::normalize(position);
};