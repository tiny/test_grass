cmake_minimum_required(VERSION 3.10)
project(grass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML package
# find_package(SFML 2.5 COMPONENTS window system REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED CONFIG)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME}  ${SOURCES})

# Add include directories for ImGui backends explicitly
#target_include_directories(${PROJECT_NAME} PUBLIC 
#    ${CMAKE_SOURCE_DIR}/third_party/SFML-2.5.1
#    ${CMAKE_SOURCE_DIR}/third_party/imgui
#    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
#)

#target_link_libraries(${PROJECT_NAME}  
#    ${SFML_LIBRARIES} 
#    ${GLEW_LIBRARIES} 
#    ${OPENGL_gl_LIBRARY}
#)

target_link_libraries(${PROJECT_NAME}  
    sfml-graphics 
    sfml-window 
    sfml-system 
    GLEW::GLEW 
    GL 
    glm::glm
)


# Copy shader files to the build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})


