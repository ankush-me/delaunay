/** Class to represent the delaunay subdivision structure.
 *  It holds the quad-edges and Delaunay Triangulation specific
 *  topological operators like Connect/ Swap/ Delete as described in
 *  Guibas & Stolfi, pages 103 and 104.*/

#include "DelaunaySubdivision.h"
#include "io_utils.h"
#include "utils/sorting.h"

using namespace Eigen;
using namespace std;

// wrapper for CCW checks for pointer to points.
bool  ccw(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c) {
	bool ret =  ccw(*a, *b, *c);
	return ret;
}

/** Wrapper for incircle (orient2d) function.*/
bool incircle(Vector2dPtr a, Vector2dPtr b, Vector2dPtr c, Vector2dPtr d) {
	double val =  incircle(*a, *b, *c, *d);
	//cout << "incircle :"<<val<<endl;
	//if (abs(val) < 1e-18) val = 0;
	bool ret =  val > 0.0;
	return ret;
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
DelaunaySubdivision::DelaunaySubdivision(string fname, string outname) : points(), pt2index(), qedges() {
	if (fname.substr(fname.length()-5,5)!= ".node") {
		cout << "Expecting input file with .node extension. Instead, found "
				<<fname.substr(fname.length()-5,5)<<". Exiting.\n";
		exit(-1);
	}

	out_prefix  = (outname =="xdefaultx")? fname.substr(0, fname.length()-5) : outname;
	readNodeFile(fname, points, pt2index);
}

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
void DelaunaySubdivision::checkRange(const int start, const int end) const {
	// check the range of the indices.
	if (!(0<=start && start < points.size() && start <= end && 0<=end && end<points.size())) {
		cout <<"Delaunay Div-&-Conquer : Indices out of range. Exiting\n";
		exit(-1);
	}

	if (end-start+1 < 2) {
		cout << " Divide and conquer expecting at least 2 points. Found "
				<< points.size() << " ." << endl;
		exit(-1);
	}
}


/** Handles base-cases of delaunay triangulation; i.e. when |S| is 2 or 3.*/
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::doBaseCases(const int start, const int end) {
	const int SIZE = end-start+1;
	if (SIZE == 2) {
		// make a single edge
		Edge::Ptr a =  QuadEdge::makeEdge(); qedges.insert(a->qEdge());
		a->setOrg (points[start + 0]);
		a->setDest(points[start + 1]);

		return make_pair(a, a->Sym());
	}

	else if (SIZE == 3) {
		Vector2dPtr p1  = points[start + 0];
		Vector2dPtr p2  = points[start + 1];
		Vector2dPtr p3  = points[start + 2];

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
DelaunaySubdivision::divideConquerVerticalCuts(int start, int end) {
	const int SIZE = end-start+1;
	checkRange(start, end);

	if (SIZE==2 || SIZE==3)
		return doBaseCases(start, end);
	else {
		// make recursive calls. Split the points into left and right
		const int mid = start + (end-start)/2;
		pair<Edge::Ptr, Edge::Ptr> lhandles = divideConquerVerticalCuts(start, mid);
		pair<Edge::Ptr, Edge::Ptr> rhandles = divideConquerVerticalCuts(mid+1, end);

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
DelaunaySubdivision::divideConquerAlternatingCuts(int start, int end, int axis) {
	const int SIZE = end-start+1;
	checkRange(start, end);

	if (SIZE == 2 || SIZE ==3) {
		// sort lexico-graphically for further processing.
		// this takes constant time, as the size is constant.
		lexicoSort(points, start, end);
		return doBaseCases(start, end);
	} else {
		// make recursive calls. Split the points into left and right
		const int mid = median(points, start, end, axis);


		pair<Edge::Ptr, Edge::Ptr> first_handles  = divideConquerAlternatingCuts(start, mid, mod(axis+1,2));
		pair<Edge::Ptr, Edge::Ptr> second_handles = divideConquerAlternatingCuts(mid+1, end, mod(axis+1,2));

		if (axis==1) { //horizontal cut : rotate handles
			first_handles  = rotate_handles(first_handles);
			second_handles = rotate_handles(second_handles);
		}

		pair<Edge::Ptr, Edge::Ptr> outer_handles  = mergeTriangulations (first_handles, second_handles);
		return ((axis==1)? unrotate_handles(outer_handles) : outer_handles);
	}
}


/** Rotate the handles.
 *  First handle  (this is the LEFT handle) goes DOWN,
 *  Second handle (this is the RIGHT handle) goes UP.*/
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::rotate_handles(std::pair<Edge::Ptr, Edge::Ptr> handles) {
	Edge::Ptr lh = handles.first;
	Edge::Ptr rh = handles.second;

	while(lh->Rprev()->org()->y() < lh->org()->y())
		lh = lh->Rprev();

	while(rh->org()->y() < rh->Lprev()->org()->y())
		rh = rh->Lprev();

	return make_pair(lh, rh);
}

/** Rotate the handles.
 *  First handle (this is the BOTTOM handle) goes LEFT,
 *  Second handle  (this is the TOP handle) goes RIGHT. */
std::pair<Edge::Ptr, Edge::Ptr>
DelaunaySubdivision::unrotate_handles(std::pair<Edge::Ptr, Edge::Ptr> handles) {
	Edge::Ptr bh = handles.first;
	Edge::Ptr th = handles.second;

	while(bh->Rnext()->org()->x() < bh->org()->x())
		bh = bh->Rnext();

	while(th->Lnext()->org()->x() > th->org()->x())
		th = th->Lnext();

	return make_pair(bh, th);
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


/** Main interface function.*/
void DelaunaySubdivision::computeDelaunay(CutsType t) {
	if (t==VERTICAL_CUTS) {
		lexicoSort(points, 0, points.size()-1);
		divideConquerVerticalCuts(0, points.size()-1);
	} else {
		divideConquerAlternatingCuts(0, points.size()-1);
	}
}

/** Writes this subdivision to file.*/
void DelaunaySubdivision::writeToFile() {
	writeSubdivision(out_prefix+".ele", this);
}




void DelaunaySubdivision::drawEdges() {

}

