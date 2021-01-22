if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if (SANITIZER)
    set(compile_opts -O1 -g -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Werror -Wno-unused-parameter -Wno-missing-field-initializers)
    set(link_opts -g -fsanitize=address)
  else()
    set(compile_opts -Wall -Wextra -Werror -Wno-unused-parameter -Wno-missing-field-initializers)
  endif()
endif()

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(compile_opts ${compile_opts} -pedantic)
endif()

set(defines
    NOMINMAX
)
