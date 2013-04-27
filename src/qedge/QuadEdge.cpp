#include "QuadEdge.h"

QuadEdge::QuadEdge() : edges(4) {}

/** Makes a new quad-edge.
 *  It returns a pointer to the first primal edge. */
static Edge::Ptr QuadEdge::MakeEdge(bool loop) {
	QuadEdge::Ptr qedge(new QuadEdge);
	for (int i=0; i < 4; i++) {
		Edge::Ptr e(new Edge(i));
		qedge->edges[i] = e;
	}
	/** Connect the internal four edges (G&S [pg. 96])
	 *
	 *   - In the primal space, a new edge is added (no loop)
	 *
	 *   - In the dual space (edges[1] and edges[3]),
	 *     the edges form a loop. As this represents that the
	 *     primal edge has the same-face around it.*/
	qedge->edges[0]->next = qedge->edges[0];
	qedge->edges[1]->next = qedge->edges[3];
	qedge->edges[2]->next = qedge->edges[2];
	qedge->edges[3]->next = qedge->edges[1];

	for (int i=0; i < 4; i++)
		qedge->edges[i]->qedge = qedge;

	return (loop? qedge->edges[1] : qedge->edges[0]);
}
