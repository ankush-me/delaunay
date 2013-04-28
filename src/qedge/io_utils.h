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

/** Reads a .node file specifying 2-dimension points
 *  into a vector of Eigen::Vector2d points.*/
void readNodeFile(const std::string &fname, vector2d &pts);

/** Writes an .ele and its corresponding .node file,
 *  for a given delaunay triangulation.
 *  The name of the output file are {fname.ele, fname.node}. */
void writeSubdivision(const std::string &fname, const DelaunaySubdivision::Ptr sub);

#endif //__IO_UTILS_H__
