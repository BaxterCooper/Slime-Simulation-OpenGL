#version 330 core
out vec4 FragColor;
uniform sampler2D screen;
void main() {
   // FragColor = texture(screen, 1.0, 1.0);
   FragColor = vec4(1, 0, 0, 1);
}