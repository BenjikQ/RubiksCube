#include <array>
#include <string>
#include <string_view>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <tl/expected.hpp>

using namespace gl;
using namespace std::string_view_literals;

static constexpr auto vertex_shader_source = R"glsl(
#version 330 core
layout (location = 0) in vec3 pos;

void main() {
    gl_Position = vec4(pos, 1.0);
}
)glsl"sv;

static constexpr auto fragment_shader_source = R"glsl(
#version 330 core
out vec4 color;

void main() {
    color = vec4(0.3, 0.5, 0.2, 1.0);
}
)glsl"sv;

[[nodiscard]] tl::expected<GLuint, std::string> create_shader(GLenum shader_type, std::string_view source) {
    const GLuint shader{ glCreateShader(shader_type) };
    std::array sources{ source.data() };
    glShaderSource(shader, 1, sources.data(), nullptr);
    glCompileShader(shader);

    GLint shader_compiled{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
    if (shader_compiled != GL_TRUE) {
        GLsizei log_length{};
        constexpr auto max_length = 1024;
        std::string message(max_length, '0');
        glGetShaderInfoLog(shader, max_length, &log_length, message.data());
        message.resize(log_length);
        return tl::unexpected{ message };
    }

    return shader;
}

template<typename... Shaders>
[[nodiscard]] tl::expected<GLuint, std::string> create_shader_program(Shaders&&... shaders) {
    const GLuint shader_program{ glCreateProgram() };
    (glAttachShader(shader_program, shaders), ...);
    glLinkProgram(shader_program);

    GLint program_linked{};
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
        GLsizei log_length{};
        constexpr auto max_length = 1024;
        std::string message(max_length, '0');
        glGetProgramInfoLog(shader_program, max_length, &log_length, message.data());
        message.resize(log_length);
        return tl::unexpected{ message };
    }

    return shader_program;
}

int main() {
    spdlog::info("Running application");

    if (!glfwInit()) {
        spdlog::error("Couldn't initialize glfw");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr auto width = 800;
    constexpr auto height = 600;
    GLFWwindow* window{ glfwCreateWindow(width, height, "Hello, World!", nullptr, nullptr) };
    if (!window) {
        spdlog::error("Couldn't create a window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glbinding::initialize(glfwGetProcAddress);

    const auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
    if (!vertex_shader) {
        spdlog::error("Vertex shader compilation error: {}", vertex_shader.error());
        return -1;
    }

    const auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if (!fragment_shader) {
        spdlog::error("Fragment shader compilation error: {}", fragment_shader.error());
        return -1;
    }

    const auto shader_program = create_shader_program(vertex_shader.value(), fragment_shader.value());
    if (!shader_program) {
        spdlog::error("Shader linking error: {}", shader_program.error());
    }

    glDeleteShader(vertex_shader.value());
    glDeleteShader(fragment_shader.value());

    // clang-format off
    constexpr std::array vertices{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    // clang-format on

    constexpr std::array indices{ 0, 1, 2 };

    GLuint vertex_array{};
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer{};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), std::data(vertices), GL_STATIC_DRAW);

    GLuint index_buffer{};
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, std::size(indices) * sizeof(int), std::data(indices), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(gl::GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program.value());
        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
