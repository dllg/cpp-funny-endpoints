cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(funnytests LANGUAGES CXX C)

include(../cmake/opts.cmake)

find_package(GTest CONFIG REQUIRED)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME}
  libfunny
  GTest::gmock
  GTest::gtest
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  LINKER_LANGUAGE CXX
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(compile_opts ${compile_opts} -Wno-gnu-zero-variadic-macro-arguments)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC ${compile_opts})

target_link_options(${PROJECT_NAME} PUBLIC ${link_opts})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>)

ENABLE_TESTING()

# Run tests after the build
if (NOT DEFINED ENV{SKIP_RUNNING_TESTS})
  add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMENT "Run tests"
    POST_BUILD
    COMMAND ${PROJECT_NAME}
  )
endif()
