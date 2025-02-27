#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 instancePos; // Instance position for each grass blade

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    float sway = sin(time + position.y * 5.0) * 0.1;
    vec3 animatedPosition = position + vec3(sway, 0.0, 0.0) + instancePos;

    gl_Position = projection * view * model * vec4(animatedPosition, 1.0);
}
