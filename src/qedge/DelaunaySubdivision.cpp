/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#include "DelaunaySubdivision.h"

using namespace Eigen;
using namespace std;

/** is the point x to the right of the edge e.*/
bool rightOf (Vector2d::Ptr x, Edge::Ptr e) {
	return ccw(*x,*(e->dest()), *(e->org()));
}

/** is the point x to the left of the edge e.*/
bool leftOf(Vector2d::Ptr x, Edge::Ptr e) {
	return ccw(*x, *(e->org()), *(e->dest()));
}

/** An edge e is valid iff, its destination lies to right the edge basel.*/
bool valid (Edge::Ptr e, Edge::Ptr basel) {
	return rightOf(e->dest(), basel);
}


/** Constructor. */
DelaunaySubdivision::DelaunaySubdivision(CutsType t) : location(t), qedges() {}


/** Adds a new edge connecting the destination of e1 to the origin of e2.
 *  Returns the first primal edge of the newly added quad-edge.
 *  From G&S [pg. 103].*/
Edge::Ptr DelaunaySubdivision::connect(Edge::Ptr e1, Edge::Ptr e2) {
	Edge::Ptr e = QuadEdge::makeEdge();

	e->setOrg(e1->dest());
	e->setDest(e2->org());

	Edge::splice(e, e1->Lnext());
	Edge::splice(e->Sym(), e2);

	return e;
}


/** Removes the edge E from the subdivision. From G&S [pg. 103]. */
void DelaunaySubdivision::deleteEdge(Edge::Ptr e) {
	Edge::splice(e, e->Oprev());
	Edge::splice(e->Sym(), e->Sym()->Oprev());
	qedges.erase(e); // remove from the hash-set
}

/** Flips the diagonal of the quadrilateral containing e. From G&S [pg. 104]. */
void DelaunaySubdivision::swap(Edge::Ptr e) {
	Edge::Ptr a = e->Oprev();
	Edge::Ptr b = e->Sym()->Oprev();

	// disconnect the edge
	Edge::splice(e, a);           Edge::splice(e->Sym(), b);

	// reconnect the edge
	Edge::splice(e, a->Lnext());  Edge::splice(e->Sym(), b->Lnext());

	// update coordinates
	e->setOrg(a->dest());
	e->setDest(b->dest());
}


/** Implements the G&S [pg. 114] divide-and-conquer algorithm for delaunay triangulation
 *  using VERTICAL CUTS.
 *
 *  PTS : vector of points. it is assumed that PTS.size() > 1 and that they are lexicographically sorted.
 *  start : the start index of PTS
 *  end   : the end index   of PTS */
std::pair<Edge::Ptr, Edge::Ptr> DelaunaySubdivision::divideConquerVerticalCuts(vector2d pts, int start, int end) {
	if (pts.size() < 2) {
		cout << " Divide and conquer expecting at least 2 points. Found "<<pts.size()<<endl;
		throw(-1);
	}
}
