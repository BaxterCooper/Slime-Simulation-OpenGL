#ifndef CONFIG
#define CONFIG

/* WINDOW */
const unsigned short WINDOW_WIDTH = 800;
const unsigned short WINDOW_HEIGHT = 600;

/* AGENT */
const unsigned short AGENT_COUNT = 1024;
const float AGENT_MOVE_SPEED = 0.00002f; // As a proportion of the screen per frame
const float AGENT_TURN_SPEED = 0.01f;

/* AGENT - SENSOR */
const unsigned short SENSOR_SIZE = 1; // In pixels
const float SENSOR_OFFSET = 0.1f; // As a proportion of the screen
const float SENSOR_FOV = 0.3f; // IN RADIANS, keep small

/* SHADERS */
const float BLUR_SPEED = 0.8f;
const float FADE_SPEED = 0.002f; // linear fade

#endif