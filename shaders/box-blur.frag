#version 460 core

out vec4 FragColor;
uniform layout(rgba32f) image2D image;

void main() {
    ivec2 dims = imageSize(image);
    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);

    vec4 color = imageLoad(image, pixelCoords);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            ivec2 offsetCoords = pixelCoords + ivec2(i, j);
            vec4 neighbourPixel = imageLoad(image, offsetCoords);
            color += neighbourPixel;
        }
    }
    color /= 9.0;

    color *= vec4(0.8);
    color.a = 1.0;

    imageStore(image, pixelCoords, color);
}