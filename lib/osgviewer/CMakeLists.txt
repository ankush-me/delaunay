
find_package( Boost )
find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer )
find_package(osgGA)       
       
set(OSG_LIBRARIES osg osgViewer OpenThreads osgDB osgGA osgUtil)       

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${OSG_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/lib
)

                    
add_library(osgviewer   environment.cpp plotting.cpp simplescene.cpp util.cpp)
target_link_libraries(osgviewer ${OSG_LIBRARIES}  ${Boost_SYSTEM_LIBRARY}  osgShadow)
