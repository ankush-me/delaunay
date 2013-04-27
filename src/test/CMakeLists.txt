add_executable(test_viewer test_viewer.cpp)
target_link_libraries(test_viewer  osgviewer utils)


add_executable(test_ptr test_ptr.cpp)
target_link_libraries(test_ptr  utils)
