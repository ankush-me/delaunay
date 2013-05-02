add_executable(test_viewer test_viewer.cpp)
target_link_libraries(test_viewer  osgviewer utils)


add_executable(test_ptr test_ptr.cpp)
target_link_libraries(test_ptr  utils)

add_executable(test_read_node test_read_node.cpp 
                              ${CMAKE_SOURCE_DIR}/src/qedge/io_utils.cpp)


add_executable(test_delaunay_vertical test_delaunay_vertical.cpp)
target_link_libraries(test_delaunay_vertical delaunay_tri utils qedge osgviewer)
