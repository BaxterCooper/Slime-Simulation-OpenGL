#include <Agent.hpp>

Agent::Agent() {
    position = glm::vec2(0.0, 0.0);
    // position = glm::vec2(glm::linearRand(-1.0, 1.0), glm::linearRand(-1.0, 1.0));
    direction = glm::normalize(glm::vec2(glm::linearRand(-1.0, 1.0), glm::linearRand(-1.0, 1.0)));
};