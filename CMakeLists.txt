cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- Project ----
project(libfunny VERSION 0.1 LANGUAGES CXX C)

include(cmake/opts.cmake)

# ---- Conan setup ----
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

# ---- Add source files ----
# Note: globbing sources is considered bad practice as CMake's generators may
# not detect new files automatically. Keep that in mind when changing files, or
# explicitly mention them here.
file(GLOB headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h*")
file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  include(GenerateExportHeader)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  add_library(${PROJECT_NAME} SHARED ${headers} ${sources})
  generate_export_header(${PROJECT_NAME})
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/exports)
else()
  add_library(${PROJECT_NAME} ${headers} ${sources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_compile_options(${PROJECT_NAME} PUBLIC ${compile_opts})

target_link_options(${PROJECT_NAME} PUBLIC ${link_opts})

target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS})

set (include_dirs
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})

add_subdirectory(console)
add_subdirectory(test)
