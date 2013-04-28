/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#ifndef __DELAUNAY_SUBDIVISION_H__
#define __DELAUNAY_SUBDIVISION_H__


#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
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

	/** List of all the quad-edges in the subdivision. */
	boost::unordered_set<QuadEdge::Ptr> qedges;

	/** The type of division used in divide-conquer algo. */
	const CutsType location;

public:
	typedef boost::shared_ptr<DelaunaySubdivision> Ptr;
	DelaunaySubdivision(CutsType t);

	/** Adds a new edge connecting the destination of e1 to the origin of e2.
	 *  Returns the first primal edge of the newly added quad-edge.*/
	Edge::Ptr connect(Edge::Ptr e1, Edge::Ptr e2);

	/** Removes the edge E from the subdivision. From G&S [pg. 103]. */
	void deleteEdge(Edge::Ptr e);

	/** Flips the diagonal of the quadrilateral containing e. From G&S [pg. 104]. */
	void swap(Edge::Ptr e);

};

#endif
