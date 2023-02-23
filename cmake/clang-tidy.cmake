file(GLOB_RECURSE
  SOURCE_FILES
  src/*.cpp)

find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  execute_process(COMMAND ${CLANG_TIDY} ${SOURCE_FILES} -p=build/ -warnings-as-errors=* --quiet -header-filter=.*)
else()
  message(FATAL_ERROR "Unable to find clang-tidy")
endif()
