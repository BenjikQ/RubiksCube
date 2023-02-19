file(GLOB_RECURSE
  SOURCE_FILES
  src/*.cpp src/*.hpp)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  execute_process(COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES})
else()
  message(FATAL_ERROR "Unable to find clang-format")
endif()
