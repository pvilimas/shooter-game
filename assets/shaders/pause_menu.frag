#version 460

// Box blur shader for the pause screen

// Input texture coordinates from the vertex shader
in vec2 fragTexCoord;

// Output fragment color
out vec4 fragColor;

// Input texture
uniform sampler2D tex;

// Screen dimensions (you may need to set this in your application)
uniform vec2 screenSize;

// Blur radius (adjust as needed)
const int blurRadius = 5;

void main() {
    // Calculate the size of a texel (a pixel in normalized coordinates)
    vec2 texelSize = 1.0 / screenSize;

    // Initialize the final color
    vec3 finalColor = vec3(0.0);

    // Apply the box blur
    for (int i = -blurRadius; i <= blurRadius; i++) {
        for (int j = -blurRadius; j <= blurRadius; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize;
            finalColor += texture(tex, fragTexCoord + offset).rgb;
        }
    }

    // Normalize the final color
    finalColor /= float((2 * blurRadius + 1) * (2 * blurRadius + 1));

    // Output the final color
    fragColor = vec4(finalColor, 1.0);
}
