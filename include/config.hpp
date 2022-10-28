#ifndef CONFIG
#define CONFIG

/* WINDOW */
const unsigned short WINDOW_WIDTH = 800;
const unsigned short WINDOW_HEIGHT = 600;

/* AGENT */
const unsigned int AGENT_COUNT = 1024;
const float AGENT_MOVE_SPEED = 0.00001f;
const float AGENT_TURN_SPEED = 1.0f;

/* AGENT - SENSOR */
const unsigned short SENSOR_SIZE = 3;
const float SENSOR_OFFSET = 0.001f;
const float SENSOR_FOV = 70.0f;

/* SHADERS */
const float BLUR_SPEED = 0.8f;
const float FADE_SPEED = 0.001f;

#endif