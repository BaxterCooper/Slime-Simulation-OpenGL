#include <Agent.hpp>

Agent::Agent() {
    position = glm::vec2(0.0f, 0.0f);
    angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 * M_PI;
};

void Agent::update() {
    glm::vec2 direction = glm::vec2(cos(angle), sin(angle));
    glm::vec2 newPosition = position + direction * static_cast <float> (AGENT_SPEED) * static_cast <float> (DELTA_TIME);

    if (newPosition.x < 0 || newPosition.x >= WINDOW_WIDTH || newPosition.y < 0 || newPosition.y >= WINDOW_HEIGHT) {
        newPosition.x = std::min(static_cast <float> (WINDOW_WIDTH), std::max(0.0f, newPosition.x));
        newPosition.y = std::min(static_cast <float> (WINDOW_HEIGHT), std::max(0.0f, newPosition.y));
        angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 * M_PI;
    }

    position = newPosition;
}