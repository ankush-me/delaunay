/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#ifndef __DELAUNAY_SUBDIVISION_H__
#define __DELAUNAY_SUBDIVISION_H__


#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

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

	// prefix of the .node used.
	std::string out_prefix;

	// holds the vertices
	std::vector<Vector2dPtr> points;


	/** Does mundane checks on the range of the indices. */
	void checkRange(const int start, const int end) const;



	/** Handles base-cases of delaunay triangulation; i.e. when |S| is 2 or 3.*/
	std::pair<Edge::Ptr, Edge::Ptr> doBaseCases(const int start, const int end);



	/** Rotate the handles.
	 *  First handle  (this is the LEFT handle) goes DOWN,
	 *  Second handle (this is the RIGHT handle) goes UP. */
	std::pair<Edge::Ptr, Edge::Ptr> rotate_handles(std::pair<Edge::Ptr, Edge::Ptr> handles);



	/** Rotate the handles.
	 *  First handle  (this is the TOP handle)    goes RIGHT,
	 *  Second handle (this is the BOTTOM handle) goes LEFT. */
	std::pair<Edge::Ptr, Edge::Ptr> unrotate_handles(std::pair<Edge::Ptr, Edge::Ptr> handles);



	/** Merges triangulations, given the appropriate handles of their convex hulls.
	 *  if the triangulations are LEFT, RIGHT, then:
	 *			handles correspond to lexicomin and max vertices.
	 *	if the triangulations are TOP, BOTTOM, then:
	 *			handles should correspond to topmost, bottom-most points.
	 *          (i.e. points in lexico-order but in (y,x) comparison order).
	 *
	 *  Returns the outer handles.*/
	std::pair<Edge::Ptr, Edge::Ptr>
	mergeTriangulations(std::pair<Edge::Ptr, Edge::Ptr> first_hs,
			std::pair<Edge::Ptr, Edge::Ptr> second_hs);


	/** Adds a new edge connecting the destination of e1 to the origin of e2.
	 *  Returns the first primal edge of the newly added quad-edge.*/
	Edge::Ptr connect(Edge::Ptr e1, Edge::Ptr e2);


	/** Removes the edge E from the subdivision. From G&S [pg. 103]. */
	void deleteEdge(Edge::Ptr e);


	/** Flips the diagonal of the quadrilateral containing e. From G&S [pg. 104]. */
	void swap(Edge::Ptr e); // NOT REQUIRED FOR DIVIDE-AND-CONQUER



	/** Implements the G&S [pg. 114] divide-and-conquer algorithm
	 *  for delaunay triangulation using VERTICAL CUTS.
	 *
	 *  PTS   : vector of points. it is assumed that PTS.size() > 1
	 *          and that they are LEXICOGRAPHICALLY SORTED.
	 *                            -------------------------
	 *  start : the start index of PTS
	 *  end   : the end index   of PTS */
	std::pair<Edge::Ptr, Edge::Ptr>
	divideConquerVerticalCuts(int start, int end);



	/** Adapted from the G&S [pg. 114] divide-and-conquer algorithm
	 *  for delaunay triangulation using ALTERNATING CUTS.
	 *
	 *  ********** THIS FUNCTION DOES NOT ASSUME THAT THE POINTS ARE SORTED**********
	 *
	 *  PTS   : vector of points. it is assumed that PTS.size() > 1
	 *  start : the start index of PTS
	 *  end   : the end index   of PTS
	 *  axis  : the axis along which the point-set needs to be cut. */
	std::pair<Edge::Ptr, Edge::Ptr>
	divideConquerAlternatingCuts(int start, int end, int axis=1);



public:
	typedef boost::shared_ptr<DelaunaySubdivision> Ptr;

	void drawEdges();


	DelaunaySubdivision(std::string fname, std::string outname="xdefaultx");

	/** List of all the quad-edges in the subdivision. */
	boost::unordered_set<QuadEdge::Ptr> qedges;

	boost::unordered_map<Vector2dPtr, int> pt2index;

	/** Main interface function.
	 *  T specifies which algorithm to use.*/
	void computeDelaunay(CutsType t=ALTERNATE_CUTS);

	/** Writes this subdivision to file.*/
	void writeToFile();
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
