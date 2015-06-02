include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tool)
add_subdirectory(plugin)