#ifndef AGENT_H
#define AGENT_H

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <config.hpp>

class Agent {
    public:
        glm::vec2 position;
        glm::vec2 direction;

        Agent();
};

#endif