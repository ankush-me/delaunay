/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#include "DelaunaySubdivision.h"

using namespace Eigen;
using namespace std;

// wrapper for CCW checks for pointer to points.
bool  ccw(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c) {
	return ccw(*a, *b, *c);
}

/** Wrapper for incircle (orient2d) function.*/
bool incircle(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c, Vector2dPtr d) {
	return incircle(*a, *b, *c, *d) > 0.0;
}


/** is the point x to the right of the edge e.*/
bool rightOf (Vector2dPtr x, Edge::Ptr e) {
	return ccw(x, e->dest(), e->org());
}

/** is the point x to the left of the edge e.*/
bool leftOf(Vector2dPtr x, Edge::Ptr e) {
	return ccw(x, e->org(), e->dest());
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
	qedges.insert(e->qEdge());

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
	qedges.erase(e->qEdge()); // remove from the hash-set
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


/** Does mundane checks on the range of the indices. */
void DelaunaySubdivision::checkRange(std::vector<Vector2dPtr> &pts,
		const int start, const int end) const {
	// check the range of the indices.
	assert (("Delaunay Div-&-Conquer : Indices out of range.",
			0<=start && start < pts.size()));
	assert (("Delaunay Div-&-Conquer : Indices out of range.",
			start <= end && 0<=end && end<pts.size()));

	if (end-start+1 < 2) {
		cout << " Divide and conquer expecting at least 2 points. Found "
				<< pts.size() << " ." << endl;
		throw(-1);
	}
}


/** Handles base-cases of delaunay triangulation; i.e. when |S| is 2 or 3.*/
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::doBaseCases(std::vector<Vector2dPtr> &pts, const int start, const int end) {
	const int SIZE = end-start+1;
	if (SIZE == 2) {
		// make a single edge
		Edge::Ptr a =  QuadEdge::makeEdge(); qedges.insert(a->qEdge());
		a->setOrg (pts[start + 0]);
		a->setDest(pts[start + 1]);
		return make_pair(a, a->Sym());
	}

	else if (SIZE == 3) {
		Vector2dPtr p1  = pts[start + 0];
		Vector2dPtr p2  = pts[start + 1];
		Vector2dPtr p3  = pts[start + 2];

		// make two edges
		Edge::Ptr a = QuadEdge::makeEdge(); qedges.insert(a->qEdge());
		Edge::Ptr b = QuadEdge::makeEdge(); qedges.insert(b->qEdge());
		Edge::splice(a->Sym(), b);
		a->setOrg(p1); a->setDest(p2);
		b->setOrg(p2); b->setDest(p3);

		// close the triangle
		if (ccw(p1, p2, p3)) {
			Edge::Ptr c = connect(b, a);
			return make_pair(a, b->Sym());
		} else if (ccw(p1, p3, p2)) {
			Edge::Ptr c = connect(b, a);
			return make_pair(c->Sym(), c);
		} else {// collinear
			return make_pair(a, b->Sym());
		}
	}
}

/** Implements the G&S [pg. 114] divide-and-conquer algorithm
 *  for delaunay triangulation using VERTICAL CUTS.
 *
 *  PTS   : vector of points. it is assumed that PTS.size() > 1
 *          and that they are LEXICOGRAPHICALLY SORTED.
 *                            -------------------------
 *  start : the start index of PTS [INCLUSIVE].
 *  end   : the end   index of PTS [INCLUSIVE]. */
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::divideConquerVerticalCuts(vector<Vector2dPtr> &pts, int start, int end) {
	const int SIZE = end-start+1;
	checkRange(pts, start, end);

	if (SIZE==2 || SIZE==3)
		return doBaseCases(pts, start, end);
	else {
		// make recursive calls. Split the points into left and right
		const int mid = start + (end-start)/2;
		pair<Edge::Ptr, Edge::Ptr> lhandles = divideConquerVerticalCuts(pts, start, mid);
		pair<Edge::Ptr, Edge::Ptr> rhandles = divideConquerVerticalCuts(pts, mid+1, end);

		return mergeTriangulations (lhandles, rhandles);
	}
}


/** Adapted from the G&S [pg. 114] divide-and-conquer algorithm
 *  for delaunay triangulation using ALTERNATING CUTS.
 *
 *  ********** THIS FUNCTION DOES NOT ASSUME THAT THE POINTS ARE SORTED**********
 *
 *  PTS   : vector of points. it is assumed that PTS.size() > 1
 *  start : the start index of PTS
 *  end   : the end index   of PTS
 *  axis  : the axis along which the point-set needs to be cut. */
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::divideConquerAlternatingCuts(std::vector<Vector2dPtr> &pts,
		int start, int end, int axis) {

	const int SIZE = end-start+1;
	checkRange(pts, start, end);

	if (SIZE == 2 || SIZE ==3) {
		// sort lexico-graphically for further processing.
		lexicoSort(pts, start, end);
		return doBaseCases(pts, start, end);
	} else {
		// make recursive calls. Split the points into left and right
		const int mid = median(pts, start, end, axis);
		pair<Edge::Ptr, Edge::Ptr> first_handles  = divideConquerAlternatingCuts(pts, start, mid, mod(axis+1,2));
		pair<Edge::Ptr, Edge::Ptr> second_handles = divideConquerAlternatingCuts(pts, mid+1, end, mod(axis+1,2));

		if (axis==0) { //horizontal cut : rotate handles
			first_handles  = rotate_handles(first_handles);
			second_handles = rotate_handles(second_handles);
		}
		pair<Edge::Ptr, Edge::Ptr> outer_handles  = mergeTriangulations (first_handles, second_handles);
		return ((axis==0)? unrotate_handles(outer_handles) : outer_handles);
	}
}

/** Merges triangulations, given the appropriate handles of their convex hulls.
 *  if the triangulations are LEFT, RIGHT, then:
 *			handles correspond to lexicomin and max vertices.
 *
 *	if the triangulations are TOP, BOTTOM, then:
 *			handles should correspond to topmost, bottom-most points.
 *          (i.e. points in lexico-order but in (y,x) comparison order).
 *
 *  Returns the outer handles.*/
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::mergeTriangulations (std::pair<Edge::Ptr, Edge::Ptr> first_handles,
		std::pair<Edge::Ptr, Edge::Ptr> second_handles) {

	Edge::Ptr ldo = first_handles.first; Edge::Ptr ldi = first_handles.second;
	Edge::Ptr rdi = second_handles.first; Edge::Ptr rdo = second_handles.second;

	// compute the lower common tangent of L and R.
	while (true) {
		if       (leftOf(rdi->org(), ldi))     ldi = ldi->Lnext();
		else if (rightOf(ldi->org(), rdi))  	rdi = rdi->Rprev();
		else break;
	}

	Edge::Ptr basel = connect(rdi->Sym(), ldi);
	if (*(ldi->org()) == *(ldo->org())) 	ldo = basel->Sym();
	if (*(rdi->org()) == *(rdo->org())) 	rdo = basel;

	// merge the two triangulations
	while (true)  {
		Edge::Ptr lcand = basel->Sym()->Onext();
		if (valid(lcand, basel)) {
			while (incircle(basel->dest(), basel->org(),
					lcand->dest(), lcand->Onext()->dest())) {
				lcand = lcand->Onext();
				deleteEdge(lcand->Oprev());
			}
		}

		Edge::Ptr rcand = basel->Oprev();
		if (valid(rcand, basel)) {
			while (incircle(basel->dest(), basel->org(),
					rcand->dest(), rcand->Oprev()->dest())) {
				rcand = rcand->Oprev();
				deleteEdge(rcand->Onext());
			}
		}

		const bool lvalid = valid(lcand, basel);
		const bool rvalid = valid(rcand, basel);

		// we have reached the upper common tangent. This exits out of the merge loop.
		if (!lvalid && !rvalid) break;

		// check which side to connect to.
		const bool check =  (!lvalid || (rvalid && incircle(lcand->dest(), lcand->org(), rcand->org(), rcand->dest())));
		basel = (check)? connect(rcand, basel->Sym()) : connect(basel->Sym(), lcand->Sym());
	}
	return make_pair(ldo, rdo);
}
