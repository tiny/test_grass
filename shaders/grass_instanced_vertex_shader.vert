#version 330 core

layout(location = 0) in vec3 position;    // Vertex position of the grass blade
layout(location = 1) in vec3 color;       // Base color of the vertex
layout(location = 2) in vec3 instancePos; // Position of the grass blade instance
layout(location = 3) in float swayAmp;    // Sway amplitude for animation
layout(location = 4) in float swaySpeed;  // Sway speed for animation
layout(location = 5) in float greenLevel; // Green color variation per blade

out vec3 vertexColor; // Pass color to the fragment shader

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate sway based on time and vertex height (position.y)
    float sway = sin(time * swaySpeed + position.y * 5.0) * swayAmp;

    // Apply sway animation to the blade's position
    vec3 animatedPosition = position + vec3(sway, 0.0, 0.0) + instancePos;

    // Set the vertex color with varying levels of green
    vertexColor = vec3(0.0, greenLevel, 0.0);

    // Calculate the final vertex position in screen space
    gl_Position = projection * view * model * vec4(animatedPosition, 1.0);
}
