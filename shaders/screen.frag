#version 330 core
out vec4 FragColor;
uniform sampler2D screen;
void main() {
   // vec2 pixelCoords = gl_FragCoord.xy;

   // vec4 pixel = vec4(0.0);
   // float offset = 0.0001;
   // for (float i = -offset; i < 2.0 * offset; i += offset) {
   //    for (float j = -offset; j < 2.0 * offset; j += offset) {
   //          float offset = 0.0001;
   //          vec2 offsetCoord = pixelCoords + vec2(i, j);
   //          vec4 neighbourPixel = texture(screen, offsetCoord);
   //          pixel += neighbourPixel;
   //    }
   // }

   // pixel /= 9.0;
   FragColor = texture(screen, 0.1, 0.1);
}