#version 330 core

in vec3 vertexColor; // Input from the vertex shader
out vec4 FragColor;  // Output color

void main() {
    // Output the final color with full opacity
    FragColor = vec4(vertexColor, 1.0);
}
