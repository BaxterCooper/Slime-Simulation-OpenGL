#ifndef CONFIG
#define CONFIG

/* WINDOW */
const unsigned short WINDOW_WIDTH = 800;
const unsigned short WINDOW_HEIGHT = 600;

/* AGENT */
const unsigned int AGENT_COUNT = 4096;
const float AGENT_MOVE_SPEED = 0.00002f; // As a proportion of the screen per frame
const float AGENT_LUMINANCE = 0.3f;
const float AGENT_TURN_SPEED = 1.0f; // unimplemented

/* AGENT - SENSOR */
const unsigned short SENSOR_SIZE = 0; // In pixels
const float SENSOR_OFFSET = 0.03f; // As a proportion of the screen
const float SENSOR_FOV = 1.0f; // IN RADIANS, keep small

/* SHADERS */
const float BLUR_SPEED = 0.8f;
const float FADE_SPEED = 0.0008f; // linear fade

#endif