#ifndef CONFIG
#define CONFIG

/* WINDOW */
const unsigned short WINDOW_WIDTH = 600;
const unsigned short WINDOW_HEIGHT = 400;

/* AGENT */
const unsigned short AGENT_COUNT = 1024;
const float AGENT_MOVE_SPEED = 0.005f;
const float AGENT_TURN_SPEED = 0.01f;

/* AGENT - SENSOR */
const unsigned short SENSOR_SIZE = 1;
const float SENSOR_OFFSET = 0.3f;
const float SENSOR_FOV = 0.3f;

/* SHADERS */
const float BLUR_SPEED = 0.8f;
const float FADE_SPEED = 0.001f;

#endif