/*******************************
 * Author: Ankush Gupta
 * SID   : 22388145
 *
 * Utility functions for file I/O.
 **********************************/
#ifndef __IO_UTILS_H__
#define __IO_UTILS_H__

#include <vector>
#include <string>
#include <Eigen/AlignedVector>
#include "DelaunaySubdivision.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)


/** Reads a .node file specifying 2-dimension points
 *  into a vector of Eigen::Vector2d points
 *
 *  Also stores against the pointer of each point, its
 *  index in the node file. This is useful in writing the .ele
 *  file later. */
void readNodeFile(const std::string &fname,
		            std::vector<Vector2dPtr> &pts,
		            boost::unordered_map<Vector2dPtr, int> &pt2index);


/** Writes an .ele and its corresponding .node file,
 *  for a given delaunay triangulation.
 *  The name of the output file are {fname.ele, fname.node}. */
void writeSubdivision(const std::string &fname, DelaunaySubdivision* sub);

#endif //__IO_UTILS_H__
