cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(funny LANGUAGES CXX C)

include(../cmake/opts.cmake)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME}
  libfunny
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  LINKER_LANGUAGE CXX
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${defines})

target_compile_options(${PROJECT_NAME} PUBLIC ${compile_opts})

target_link_options(${PROJECT_NAME} PUBLIC ${link_opts})
