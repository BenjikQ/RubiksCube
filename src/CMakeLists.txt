find_package(glfw3 REQUIRED)
find_package(glbinding REQUIRED)
find_package(spdlog REQUIRED)

add_executable(rubikscube main.cpp)
target_compile_features(rubikscube PRIVATE cxx_std_23)
target_link_libraries(rubikscube
  PRIVATE
    glfw
    opengl::opengl
    glbinding::glbinding
    spdlog::spdlog)