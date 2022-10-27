#version 460 core

out vec4 FragColor;
uniform layout(rgba32f) image2D image;

void main() {
    FragColor = imageLoad(image, ivec2(gl_FragCoord.xy));
}