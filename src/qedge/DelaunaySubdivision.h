/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#ifndef __DELAUNAY_SUBDIVISION_H__
#define __DELAUNAY_SUBDIVISION_H__


#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include "utils/geom_predicates.h"
#include <utility>
#include "QuadEdge.h"
#include "Edge.h"
#include "Data.h"
#include <Eigen/Dense>


/** The type of division used in divide-conquer algo. */
enum CutsType {
	VERTICAL_CUTS,
	ALTERNATE_CUTS
};


class DelaunaySubdivision {

	/** The type of division used in divide-conquer algo. */
	const CutsType location;

public:
	typedef boost::shared_ptr<DelaunaySubdivision> Ptr;

	/** List of all the quad-edges in the subdivision. */
	boost::unordered_set<QuadEdge::Ptr> qedges;

	DelaunaySubdivision(CutsType t);

	/** Adds a new edge connecting the destination of e1 to the origin of e2.
	 *  Returns the first primal edge of the newly added quad-edge.*/
	Edge::Ptr connect(Edge::Ptr e1, Edge::Ptr e2);

	/** Removes the edge E from the subdivision. From G&S [pg. 103]. */
	void deleteEdge(Edge::Ptr e);

	/** Flips the diagonal of the quadrilateral containing e. From G&S [pg. 104]. */
	void swap(Edge::Ptr e);


	/** Implements the G&S [pg. 114] divide-and-conquer algorithm
	 *  for delaunay triangulation using VERTICAL CUTS.
	 *
	 *  PTS   : vector of points. it is assumed that PTS.size() > 1
	 *          and that they are LEXICOGRAPHICALLY SORTED.
	 *                            -------------------------
	 *  start : the start index of PTS
	 *  end   : the end index   of PTS */
	std::pair<Edge::Ptr, Edge::Ptr>
	divideConquerVerticalCuts(std::vector<Vector2dPtr> pts, int start, int end);
};

// wrapper for CCW checks for pointer to points.
bool  ccw(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c);

// see page G&S page 113 for the following functions
/** is the point x to the right of the edge e.*/
bool rightOf(Vector2dPtr x, Edge::Ptr e);

/** is the point x to the left of the edge e.*/
bool leftOf(Vector2dPtr x, Edge::Ptr e);

/** An edge e is valid iff, its destination lies to right the edge basel.*/
bool valid(Edge::Ptr e, Edge::Ptr basel);

/** Wrapper for incircle (orient2d) function.*/
bool incircle(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c, Vector2dPtr d);

#endif
