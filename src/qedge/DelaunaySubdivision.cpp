/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#include "DelaunaySubdivision.h"


/** Constructor. */
DelaunaySubdivision::DelaunaySubdivision(PointLocationType t) : location(t), qedges() {}


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
	qedges.erase(e);
}


/** Flips the diagonal of the quadrilateral containing e. From G&S [pg. 104]. */
void DelaunaySubdivision::swap(Edge::Ptr e) {
	Edge::Ptr a = e->Oprev();
	Edge::Ptr b = e->Sym()->Oprev();
	Edge::splice(e, a);           Edge::splice(e->Sym(), b);
	Edge::splice(e, a->Lnext());  Edge::splice(e->Sym(), b->Lnext());
	e->setOrg(a->dest());
	e->setDest(b->dest());
}
