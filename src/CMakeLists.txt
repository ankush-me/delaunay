include_directories(
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/src
)

link_directories(
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(utils)
add_subdirectory(qedge)
add_subdirectory(test)
