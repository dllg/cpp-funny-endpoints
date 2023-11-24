# add libraries here
set(dependencies
  argh
  Drogon::Drogon
  httplib::httplib
  nlohmann_json::nlohmann_json
  spdlog::spdlog
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
