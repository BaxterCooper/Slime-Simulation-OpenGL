#version 460 core

out vec4 FragColor;
uniform layout(rgba32f) image2D image;

void main() {
    ivec2 pixelCoord = ivec2(gl_FragCoord.xy);
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    imageStore(image, pixelCoord, color);
}