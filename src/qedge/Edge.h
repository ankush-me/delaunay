/**
 * Author: Ankush Gupta
 * Date  : 26 April 2013
 *
 * See the doc in QuadEdge.h */

#ifndef __EDGE_H__
#define __EDGE_H__

#include <boost/shared_ptr.hpp>
#include <vector>
#include "Data.h"
#include "QuadEdge.h"


class Edge {
private:

	// let quad-edge access the private members
	friend class QuadEdge;

	typedef	 boost::shared_ptr<Edge> Ptr;


	// the index of this edge in QuadEdge's list of 4 edges; \in {0,1,2,3}
	const int idx;

	// origin and destination of the edge
	Vector2d::Ptr org, dst;

	// generic data container
	Data::Ptr     data;

	// the quad-edge this edge is a part of
	QuadEdge::Ptr qedge;

	// ccw next edge about the origin
	Edge::Ptr     next;

public:


	Edge(int _idx) : idx(_idx) {}

	/** Splice e1 and e2.*/
	static void Splice(Edge::Ptr e1, Edge::Ptr e2);

	/** These are some functions which help access the
	 * topological structure of the subdivision.
	 * The definitions are taken from pg. 84 of the Guibas & Stolfi Paper. */

	/** Return oppositely directed edge. Same orientation.*/
	Edge::Ptr Sym();

	/** Return the (pseudo) dual edge: e rotated 90 degrees CCW.*/
	Edge::Ptr Rot();

	/** Return the (pseudo) dual edge: e rotated 90 degrees CW.*/
	Edge::Ptr RotInv();

	/** Return the ccw next edge about the origin, pointing away from origin. */
	Edge::Ptr Onext();

	/** Return the ccw next edge about the destination, pointing towards destination. */
	Edge::Ptr Dnext();

	/** Return the ccw next edge about the left-face. */
	Edge::Ptr Lnext();

	/** Return the ccw next edge about the right-face. */
	Edge::Ptr Rnext();

	/** Return the cw next edge about the origin, pointing away from origin. */
	Edge::Ptr Oprev();

	/** Return the cw next edge about the destination, pointing towards destination. */
	Edge::Ptr Dprev();

	/** Return the cw next edge about the left-face. */
	Edge::Ptr Lprev();

	/** Return the cw next edge about the right-face. */
	Edge::Ptr Rprev();
};

#endif //__EDGE_H__
