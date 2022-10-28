#ifndef AGENT
#define AGENT

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

class Agent {
    public:
        glm::vec2 position;
        glm::vec2 direction;

        Agent();
};

#endif