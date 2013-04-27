/**
 * Author: Ankush Gupta
 * Date  : 26 April 2013
 *
 * See the doc in QuadEdge.h */

#include "QuadEdge.h"
#include "Edge.h"
#include "utils/misc.h"

/** Return the (pseudo) dual edge: e rotated 90 degrees CCW.*/
Edge::Ptr Rot() {
	return qedge->edges[mod(idx+1,4)];
}

Edge::Ptr Onext() {


}
Edge::Ptr Dnext();
Edge::Ptr Lnext();
Edge::Ptr Rnext();

Edge::Ptr Oprev();
Edge::Ptr Dprev();
Edge::Ptr Lprev();
Edge::Ptr Rprev();
