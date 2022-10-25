#ifndef AGENT_H
#define AGENT_H

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <config.hpp>

class Agent {
    glm::vec2 position;
    glm::float32 angle;

    void update() {};
};

#endif