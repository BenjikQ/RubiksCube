find_package(glfw3 REQUIRED)
find_package(glbinding REQUIRED)
find_package(spdlog REQUIRED)
find_package(tl-expected REQUIRED)

add_executable(rubikscube main.cpp)
target_compile_features(rubikscube PRIVATE cxx_std_20)
target_link_libraries(rubikscube
  PRIVATE
    glfw
    opengl::opengl
    glbinding::glbinding
    spdlog::spdlog
    tl::expected)

add_custom_command(
  TARGET rubikscube POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets/shaders $<TARGET_FILE_DIR:rubikscube>/assets/shaders)

