add_library(qedge  Edge.cpp
                   QuadEdge.cpp
                   Data.cpp
                   io_utils.cpp)
target_link_libraries(qedge utils)



add_library(delaunay_tri DelaunaySubdivision.cpp)
target_link_libraries(delaunay_tri   qedge
									 utils)
