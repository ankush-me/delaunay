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

/** Specify the data-structure used for point-location. */
enum PointLocationType {
	WALKING,
	HISTORY_DAG
};


class DelaunaySubdivision {

	/** List of all the quad-edges in the subdivision. */
	boost::unordered_set<QuadEdge::Ptr> qedges;

	/** Point location method to be used. */
	const PointLocationType location;

public:

	DelaunaySubdivision(PointLocationType t);

	/** Adds a new edge connecting the destination of e1 to the origin of e2.
	 *  Returns the first primal edge of the newly added quad-edge.*/
	Edge::Ptr Connect(Edge::Ptr e1, Edge::Ptr e2);

	/** Removes the edge E from the subdivision. */
	void DeleteEdge(Edge::Ptr e);

};


#endif
