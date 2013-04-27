/***************************
 * Author: Ankush Gupta
 * Date  : 26 April 2013
 *
 * See the doc in QuadEdge.h
 * *************************/

#include "QuadEdge.h"
#include "Edge.h"
#include "utils/misc.h"



/** This is a topological operator which joins/ separates the
 *  vertex/ face chains defined at edges e1 and e2.
 *  Code based on G&S [pg. 102]. */
static void Edge::Splice(Edge::Ptr e1, Edge::Ptr e2) {
  Edge::Ptr alpha = e1->Onext()->Rot();
  Edge::Ptr beta  = e2->Onext()->Rot();

  e1->next.swap(e2->next);
  alpha->next.swap(beta->next);
}


/** These are some functions which help access the
 * topological structure of the subdivision.
 * The definitions are taken from pg. 84 of the Guibas & Stolfi Paper. */

/** Return oppositely directed edge. Same orientation.*/
Edge::Ptr Edge::Sym() {
	return Rot()->Rot();
}

/** Return the (pseudo) dual edge: e rotated 90 degrees CCW.*/
Edge::Ptr Edge::Rot() {
	return qedge->edges[mod(idx+1, 4)];
}

/** Return the (pseudo) dual edge: e rotated 90 degrees CW.*/
Edge::Ptr Edge::RotInv() {
	return qedge->edges[mod(idx-1, 4)];
}

/** Return the ccw next edge about the origin, pointing away from origin. */
Edge::Ptr Edge::Onext() {
	return next;
}

/** Return the ccw next edge about the destination, pointing towards destination. */
Edge::Ptr Edge::Dnext() {
	return Sym()->Onext()->Sym();
}

/** Return the ccw next edge about the left-face. */
Edge::Ptr Edge::Lnext() {
	return RotInv()->Onext()->Rot();
}

/** Return the ccw next edge about the right-face. */
Edge::Ptr Edge::Rnext() {
	return Rot()->Onext()->RotInv();
}

/** Return the cw next edge about the origin, pointing away from origin. */
Edge::Ptr Edge::Oprev() {
	return Rot()->Onext()->Rot();
}

/** Return the cw next edge about the destination, pointing towards destination. */
Edge::Ptr Edge::Dprev() {
 return RotInv()->Onext()->RotInv();
}

/** Return the cw next edge about the left-face. */
Edge::Ptr Edge::Lprev() {
	return Onext()->Sym();
}

/** Return the cw next edge about the right-face. */
Edge::Ptr Edge::Rprev() {
	return Sym()->Onext();
}
