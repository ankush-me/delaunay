/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and delaunay triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  G&S, pages 103 and 104.*/

#ifndef __DELAUNAY_SUBDIVISION_H__
#define __DELAUNAY_SUBDIVISION_H__


#include <boost/shared_ptr.hpp>
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
	std::vector<QuadEdge::Ptr> qedges;

	/** Maps an QuadEdge pointer to its index in the QEDGES vector. */
	std::map<QuadEdge::Ptr, int> edgeToIndex;

	/** Point location method to be used. */
	const PointLocationType location;

public:

	DelaunaySubdivision(PointLocationType t);



};


#endif
