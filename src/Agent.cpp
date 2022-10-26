#include <Agent.hpp>

Agent::Agent() {
    position = glm::vec2(0.0f, 0.0f);
    direction = glm::vec2(glm::linearRand(-1.0, 1.0), glm::linearRand(-1.0, 1.0));
};