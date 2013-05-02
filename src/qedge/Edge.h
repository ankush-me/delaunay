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
public: typedef	 boost::shared_ptr<Edge> Ptr;

private:

	// let quad-edge access the private members
	friend class QuadEdge;

	// the index of this edge in QuadEdge's list of 4 edges; \in {0,1,2,3}
	const int idx;

	/** Origin of the edge.
	 * 	Destination is stored in the symmetric edge.*/
	Vector2dPtr origin;

	// generic data container
	Data::Ptr     data;

	// the quad-edge this edge is a part of
	QuadEdge::Ptr qedge;

	// ccw next edge about the origin
	Edge::Ptr     next;

public:
	// the index of this edge in QuadEdge's list of 4 edges; \in {0,1,2,3}
	Edge(int _idx) : idx(_idx) {}

	/** Return the quad-edge this edge is a part of.*/
	QuadEdge::Ptr qEdge();

	/** Operations on the origin/ destination of this edge.*/
	Vector2dPtr org();
	Vector2dPtr dest();
	void setOrg(Vector2dPtr pt);
	void setDest(Vector2dPtr pt);

	/** This is a topological operator which joins/ separates the
	 *  vertex/ face chains defined at edges e1 and e2.
	 *  Code based on G&S [pg. 98, pg. 102].
	 *
	 *  Note: Since there is no "flip", the code is especially simple.*/
	static void splice(Edge::Ptr e1, Edge::Ptr e2);


	/** These are some functions which help access the
	 * topological structure of the subdivision.
	 * The definitions are taken from pg. 84 of the Guibas & Stolfi Paper. */

	/** Return oppositely directed edge. Same orientation.*/
	Edge::Ptr Sym();

	/* Return the (pseudo) dual edge: e rotated 90 degrees CCW.*/
	Edge::Ptr Rot();

	/* Return the (pseudo) dual edge: e rotated 90 degrees CW.*/
	Edge::Ptr RotInv();

	/* Return the ccw next edge about the origin, pointing away from origin.*/
	Edge::Ptr Onext();

	/* Return the ccw next edge about the destination, pointing towards destination.*/
	Edge::Ptr Dnext();

	/* Return the ccw next edge about the left-face.*/
	Edge::Ptr Lnext();

	/* Return the ccw next edge about the right-face.*/
	Edge::Ptr Rnext();

	/* Return the cw next edge about the origin, pointing away from origin.*/
	Edge::Ptr Oprev();

	/* Return the cw next edge about the destination, pointing towards destination.*/
	Edge::Ptr Dprev();

	/* Return the cw next edge about the left-face.*/
	Edge::Ptr Lprev();

	/* Return the cw next edge about the right-face.*/
	Edge::Ptr Rprev();
};

#endif //__EDGE_H__
