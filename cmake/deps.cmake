# add libraries here
set(dependencies
  tinyobjloader::tinyobjloader
  argh
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  httplib::httplib
  Drogon::Drogon
)

foreach (dependency ${dependencies})
  string(REGEX REPLACE "::.*" "" dependency ${dependency})
  message(STATUS "Searching for ${dependency}")
  find_package(${dependency} CONFIG REQUIRED)
  if (NOT ${dependency}_FOUND)
    message(FATAL_ERROR "${dependency} not found")
  endif()
endforeach(dependency)

# Add include directories here
set (include_dirs
)

# Add library directories here
set (library_dirs
  ""
)
