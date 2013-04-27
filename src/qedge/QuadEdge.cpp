#include "QuadEdge.h"

QuadEdge::QuadEdge() : edges(4) {
	for (int i=0; i < 4; i++) {
		Edge::Ptr e(new Edge(i));
		edges[i] = e;
	}

	/** Connect the internal four edges (G&S [pg. 96])
	 *
	 *   - In the primal space, a new edge is added (no loop)
	 *
	 *   - In the dual space (edges[1] and edges[3]),
	 *     the edges form a loop. As this represents that the
	 *     primal edge has the same-face around it.*/
	edges[0]->next = edges[0];
	edges[1]->next = edges[3];
	edges[2]->next = edges[2];
	edges[3]->next = edges[1];
}

/** This is just an alias for the constructor of QuadEdge.
 *  Added due to its use in the G&S paper. */
static QuadEdge::Ptr QuadEdge::MakeEdge() {
	return QuadEdge::Ptr(new QuadEdge);
}
