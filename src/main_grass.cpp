#include <iostream>

#include "grass_blade.h"
#include "shader_loader.h"

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

// OpenGL buffers
GLuint vao, vbo, ebo;
GLuint instancePosVBO, swayAmpVBO, swaySpeedVBO, greenLevelVBO;
GLuint shaderProgram;

// Vertex structure to store position and base color
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

// Vertices for a basic grass blade
std::vector<Vertex> grassVertices = {
    {{-0.01f, 0.0f, 0.0f}, {0.0f, 0.8f, 0.0f}},
    {{0.01f, 0.0f, 0.0f}, {0.0f, 0.8f, 0.0f}},
    {{-0.02f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.02f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.0f, 1.0f, 0.0f}, {0.5f, 1.0f, 0.0f}}
};

// Indices to draw the blade as a triangle strip
std::vector<GLuint> grassIndices = {
    0, 1, 2, 
    1, 2, 3, 
    2, 3, 4  
};

int32_t generateGrassField(int count, float areaSize, std::vector<GrassBlade>& grassBlades) {
    std::default_random_engine generator;
    std::uniform_real_distribution<float> posDist(-areaSize / 2.0f, areaSize / 2.0f);
    std::uniform_real_distribution<float> swayAmpDist(0.05f, 0.15f);
    std::uniform_real_distribution<float> swaySpeedDist(0.5f, 2.0f);
    std::uniform_real_distribution<float> greenDist(0.5f, 1.0f);

    for (int i = 0; i < count; ++i) {
        glm::vec3 position(posDist(generator), 0.0f, posDist(generator));
        float swayAmplitude = swayAmpDist(generator);
        float swaySpeed = swaySpeedDist(generator);
        float greenLevel = greenDist(generator);

        // Create a GrassBlade with position, sway properties, and a varying green level
        grassBlades.emplace_back(position, swayAmplitude, swaySpeed, greenLevel);
    }

    return grassBlades.size();
}


void initializeGrassBlades(const std::vector<GrassBlade>& grassBlades) {
    std::vector<glm::vec3> instancePositions;
    std::vector<float> swayAmplitudes;
    std::vector<float> swaySpeeds;
    std::vector<float> greenLevels;

    for (const auto& blade : grassBlades) {
        instancePositions.push_back(blade.getPosition());
        swayAmplitudes.push_back(blade.getSwayAmplitude());
        swaySpeeds.push_back(blade.getSwaySpeed());
        greenLevels.push_back(blade.getGreenLevel());
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer (positions and colors)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(Vertex), grassVertices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    // Element Buffer (EBO)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grassIndices.size() * sizeof(GLuint), grassIndices.data(), GL_STATIC_DRAW);

    // Instance Position VBO (location = 2)
    glGenBuffers(1, &instancePosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancePosVBO);
    glBufferData(GL_ARRAY_BUFFER, instancePositions.size() * sizeof(glm::vec3), instancePositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // Sway Amplitude VBO (location = 3)
    glGenBuffers(1, &swayAmpVBO);
    glBindBuffer(GL_ARRAY_BUFFER, swayAmpVBO);
    glBufferData(GL_ARRAY_BUFFER, swayAmplitudes.size() * sizeof(float), swayAmplitudes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // Sway Speed VBO (location = 4)
    glGenBuffers(1, &swaySpeedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, swaySpeedVBO);
    glBufferData(GL_ARRAY_BUFFER, swaySpeeds.size() * sizeof(float), swaySpeeds.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    // Green Level VBO (location = 5)
    glGenBuffers(1, &greenLevelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, greenLevelVBO);
    glBufferData(GL_ARRAY_BUFFER, greenLevels.size() * sizeof(float), greenLevels.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

    glBindVertexArray(0);
}


int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(
        sf::VideoMode(1000, 600),
        "Swaying Grass",
        sf::Style::Default,
        settings
    );

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }    

    shaderProgram = createShaderProgram(
        "shaders/grass_instanced_vertex_shader.vert", 
        "shaders/grass_fragment_shader.frag"
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<GrassBlade> grassBlades;
    generateGrassField(10000, 20.0f, grassBlades);
    initializeGrassBlades(grassBlades); 

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        float time = clock.getElapsedTime().asSeconds();
        GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
        if (timeLoc == -1) {
            std::cerr << "Uniform 'time' not found in shader program!" << std::endl;
        } else {
            glUniform1f(timeLoc, time);
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(5.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1000.0f / 600.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElementsInstanced(GL_TRIANGLES, grassIndices.size(), GL_UNSIGNED_INT, 0, grassBlades.size());
//        glDrawArrays(GL_POINTS, 0, grassVertices.size());

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << err << std::endl;
        }

        window.display();    
    }

    return 0;
}
