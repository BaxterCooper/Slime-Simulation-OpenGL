#version 460 core

out vec4 FragColor;
uniform float blurSpeed;
uniform float fadeSpeed;

uniform layout(rgba32f) image2D image;

void main() {
    ivec2 dims = imageSize(image);
    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);
    vec4 originalColor = imageLoad(image, pixelCoords);

    vec4 color = originalColor;

    // BLUR
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            ivec2 offsetCoords = pixelCoords + ivec2(i, j);
            vec4 neighbourPixel = imageLoad(image, offsetCoords);
            color += neighbourPixel;
        }
    }
    color /= 10.0; // >9 in order to contribute to dimming
    color = mix(originalColor, color, blurSpeed);

    // FADE
    color -= fadeSpeed;
    color = clamp(color, 0.0, 1.0);

    // KEEP COLOR ALPHA CONSTANT
    color.a = 1.0;

    imageStore(image, pixelCoords, color);
}