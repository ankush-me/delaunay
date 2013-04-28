/**
 * Author: Ankush Gupta
 * Date  : 26 April 2013
 *
 * Defines the quad-edge structure as described in Guibas and Stolfi,
 * for representing subdivisions of manifolds.
 *
 * - SIMPLIFICATION : Assumes that the manifold is orientable.
 *   ---------------  As Delaunay traingulation/ Voronoi diagram need only 1 orientation,
 *                    only 1 orientation is represented (therefore, e.flip is not required/ represented).
 *
 *  A quad-edge is represented as a collection of 4 edges:
 *  			 {e, e.Rot, e.Rot.Rot, e.Rot.Rot.Rot},
 *  e.Rot gives the "dual" of e (not a complete dual as e.Rot.Rot == e.Sym != e).
 *
 *  For every edge, a "next" edge is defined to be the next counter-clockwise
 *  edge from the origin of e.
 */

#ifndef __QUADEDGE_H__
#define __QUADEDGE_H__

#include <boost/shared_ptr.hpp>
#include <vector>
#include "Data.h"

class Edge;

class QuadEdge {
public:
	typedef	 boost::shared_ptr<QuadEdge> Ptr;
	std::vector<boost::shared_ptr<Edge> > edges;

	QuadEdge();

	/** Makes a new quad-edge. LOOP is a flag which signifies
	 *  if we need an edge which forms a loop in the primal space.
	 *
	 *  It returns a pointer to the first primal edge. */
	static boost::shared_ptr<Edge> makeEdge(bool loop = false);
};

#endif //__QUADEDGE_H__
