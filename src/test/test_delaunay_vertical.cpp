#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"

#include "utils/geom_predicates.h"
#include "utils/sorting.h"

#include "qedge/Data.h"
#include "qedge/DelaunaySubdivision.h"
#include "qedge/io_utils.h"

#include <Eigen/AlignedVector>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <limits>

using namespace Eigen;
using namespace std;

class CustomScene : public Scene{};

vector<Vector2dPtr> toPtrVector(const vector2d &pts) {
	vector<Vector2dPtr> ret(pts.size());
	for (int i=0; i<pts.size();i++) {
		Vector2dPtr pt(new Vector2d(pts[i]));
		ret[i] = pt;
	}
	return ret;
}

// compute delaunay triangulation and display it in the viewer.
void doDelaunay(vector2d &pts, bool verbose=false) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);

	lexicoSort(pts);

	if (verbose) {
		for (int i=0; i < pts.size(); i++)
			cout << pts[i].transpose()<<endl;
		cout << "sorted points."<<endl;
	}
	vector<Vector2dPtr> ptrPTS = toPtrVector(pts);

	DelaunaySubdivision subD(VERTICAL_CUTS);
	subD.divideConquerAlternatingCuts(ptrPTS, 0, pts.size()-1);

	vector3f pts0(subD.qedges.size()), pts1(subD.qedges.size());
	int i = 0;
	for(boost::unordered_set<QuadEdge::Ptr>::iterator it = subD.qedges.begin();
			it != subD.qedges.end(); it++, i++) {
		QuadEdge::Ptr q = *it;
		Vector2dPtr org = q->edges[0]->org();
		Vector2dPtr dst = q->edges[0]->dest();
		pts0[i] = Vector3f((float)org->x(), (float) org->y(), 0.f);
		pts1[i] = Vector3f((float)dst->x(), (float) dst->y(), 0.f);
	}
	util::drawLines(pts0, pts1, Vector3f(1.f,0.f,0.f), 1, s.env);
	s.run();
}


void testNODEFile(const int fidx = 0) {
	string files[] = {"4.node",
			"box.node",
			"flag.node",
			"ladder.node",
			"tri.node",
			"633.node",
			"dots.node",
			"grid.node",
			"spiral.node",
			"ttimeu1000000.node"};

	string fname = string(EXPAND (PROJECT_DATA_DIR)) + "/" +  files[fidx];
	vector2d points;
	readNodeFile(fname, points);
	doDelaunay(points, true);
}


/** Plots the delaunay triangulation of a random set of points. */
void testRand(const int N=10) {

	MatrixXd randm = MatrixXd::Random(N,2);
	vector2d pts(N);
	for (int i = 0 ; i < N; i+=1)
		pts[i] = randm.row(i);

	doDelaunay(pts);
}

/** Plots the delaunay triangulation of a random set of points. */
void testBox(const int N=10) {


	vector2d pts(13);
	pts[0] = Vector2d(0.,0.);
	pts[1] = Vector2d(0.1,0.);
	pts[2] = Vector2d(0.2,0.);
	pts[3] = Vector2d(0.3,0.);
	pts[4] = Vector2d(0.4,0.);

	pts[5] = Vector2d(0.,.1);
	pts[6] = Vector2d(.1,.1);
	pts[7] = Vector2d(.2,.1);
	pts[8] = Vector2d(.3,.1);
	pts[9] = Vector2d(.4,.1);

	pts[10] = Vector2d(0.,.2);
	pts[11] = Vector2d(.1,.2);
	pts[12] = Vector2d(.2,.2);

	double S = 1;
	for (int i=0; i < pts.size(); i++) {
		pts[i] *= S;
	}

	doDelaunay(pts,true);
}


int main (int argc, char* argv[]) {
	int N = 5;
	if (argc ==2) {
		N = atoi(argv[1]);
	}
	testRand(N);
	//testNODEFile(7);
	//testBox();
	return 0;
}
