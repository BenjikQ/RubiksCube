#include <array>

#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

using namespace gl;

int main() {
    spdlog::info("Running application");

    if (!glfwInit()) {
        spdlog::error("Couldn't initialize glfw");
        return -1;
    }

    GLFWwindow* window{ glfwCreateWindow(800, 600, "Hello, World!", nullptr, nullptr) };
    if (!window) {
        spdlog::error("Couldn't create a window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glbinding::initialize(glfwGetProcAddress);

    std::array vertices {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GLenum::GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GLenum::GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), std::data(vertices), GLenum::GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GLenum::GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(gl::GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}