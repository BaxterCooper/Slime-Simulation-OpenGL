#version 460 core

out vec4 FragColor;
uniform layout(rgba32f) image2D image;
uniform float agentLuminance;

void main() {
    ivec2 pixelCoord = ivec2(gl_FragCoord.xy);
    vec4 color = imageLoad(image, pixelCoord);
    color += vec4(agentLuminance);
    color = clamp(color, 0.0, 1.0);
    imageStore(image, pixelCoord, color);
}
