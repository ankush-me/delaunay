/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#include "DelaunaySubdivision.h"


/** Adds a new edge connecting the destination of e1 to the origin of e2.
 *  Returns the newly added edge. From G&S [pg. 103]*/
Edge::Ptr DelaunaySubdivision::Connect(Edge::Ptr e1, Edge::Ptr e2) {
	Edge::Ptr e = QuadEdge::MakeEdge();

	e->setOrg(e1->Dest());
	e->setDest(e2->Org());

	Edge::Splice(e, e1->Lnext());
	Edge::Splice(e->Sym(), e2);

	return e;
}


/** Removes the edge E from the subdivision. */
void DeleteEdge(Edge::Ptr e) {

}
