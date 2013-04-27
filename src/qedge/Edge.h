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

	// topological accessors
	Edge::Ptr Rot();

	Edge::Ptr Onext();
	Edge::Ptr Dnext();
	Edge::Ptr Lnext();
	Edge::Ptr Rnext();

	Edge::Ptr Oprev();
	Edge::Ptr Dprev();
	Edge::Ptr Lprev();
	Edge::Ptr Rprev();
};

#endif //__EDGE_H__
