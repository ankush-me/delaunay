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
#include <boost/unordered_map.hpp>
#include <limits>

#include <fstream>

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
void doDelaunay(vector<Vector2dPtr> &pts, boost::unordered_map<Vector2dPtr, int> &umap,
		bool verbose=false) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);

	lexicoSort(pts, 0, pts.size()-1);

	if (verbose) {
		for (int i=0; i < pts.size(); i++)
			cout << pts[i]->transpose()<<endl;
		cout << "sorted points."<<endl;
	}

	////	DelaunaySubdivision::Ptr subD(new DelaunaySubdivision);
	//	subD->pt2index = umap;
	//	subD->divideConquerAlternatingCuts(pts, 0, pts.size()-1);
	//
	//	writeSubdivision("blah", subD);
	//
	//
	//	// DRAW SUBDIVISION
	//	vector3f pts0(subD->qedges.size()), pts1(subD->qedges.size());
	//	int i = 0;
	//	for(boost::unordered_set<QuadEdge::Ptr>::iterator it = subD->qedges.begin();
	//			it != subD->qedges.end(); it++, i++) {
	//		QuadEdge::Ptr q = *it;
	//		Vector2dPtr org = q->edges[0]->org();
	//		Vector2dPtr dst = q->edges[0]->dest();
	//		pts0[i] = Vector3f((float)org->x(), (float) org->y(), 0.f);
	//		pts1[i] = Vector3f((float)dst->x(), (float) dst->y(), 0.f);
	//	}
	//	util::drawLines(pts0, pts1, Vector3f(1.f,0.f,0.f), 1, s.env);
	//	s.run();
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
	vector<Vector2dPtr> points;
	boost::unordered_map<Vector2dPtr, int> umap;
	readNodeFile(fname, points, umap);

	cout << "printing vertex indices : "<<endl;
	for (int i=0; i < points.size(); i++)
		cout << umap[points[i]]<<" : "<<points[i]->transpose()<<endl;

	doDelaunay(points, umap, true);

}


/** Plots the delaunay triangulation of a random set of points. */
void testRand(const int N=10) {
	string fname = string(EXPAND (PROJECT_DATA_DIR)) + "/" +  "temp.node";
	fstream tmp;
	tmp.open(fname.c_str(),ios::out);

	tmp << N << "\t"<<2<<"\t"<<0<<"\t"<<0<<endl;

	MatrixXd randm = MatrixXd::Random(N,2);
	for (int i = 0 ; i < N; i+=1)
		tmp << i<<"\t"<<randm.row(i)(0) <<"\t"<<randm.row(i)(1) <<endl;
	tmp.close();


	DelaunaySubdivision subD(fname);
	subD.computeDelaunay(ALTERNATE_CUTS);

	//	// DRAW SUBDIVISION
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);
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

/** Plots the delaunay triangulation of a random set of points. */
void testBox(const int N=10) {

	vector<Vector2dPtr> pts(13);
	pts[0].reset(new Vector2d(0.,0.));
	pts[1].reset(new Vector2d(0.1,0.));
	pts[2].reset(new Vector2d(0.2,0.));
	pts[3].reset(new Vector2d(0.3,0.));
	pts[4].reset(new Vector2d(0.4,0.));

	pts[5].reset(new Vector2d(0.,.1));
	pts[6].reset(new Vector2d(.1,.1));
	pts[7].reset(new Vector2d(.2,.1));
	pts[8].reset(new Vector2d(.3,.1));
	pts[9].reset(new Vector2d(.4,.1));

	pts[10].reset(new Vector2d(0.,.2));
	pts[11].reset(new Vector2d(.1,.2));
	pts[12].reset(new Vector2d(.2,.2));

	double S = 1;
	for (int i=0; i < pts.size(); i++) {
		*pts[i] *= S;
	}

	//doDelaunay(pts,true);
}

void newTest(int fidx=0) {
	string files[] = {
			"dots2.node",
			"4.node",
			"box.node",
			"flag.node",
			"ladder.node",
			"tri.node",
			"633.node",
			"333.node",
			"dots.node",
			"grid.node",
			"spiral.node",
			"ttimeu1000000.node"};


	string fname = string(EXPAND (PROJECT_DATA_DIR)) + "/" +  files[fidx];
	DelaunaySubdivision subD(fname);
	subD.computeDelaunay();
	subD.writeToFile();


	//	// DRAW SUBDIVISION
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);
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


int main (int argc, char* argv[]) {
	int N = 5;
	if (argc ==2) {
		N = atoi(argv[1]);
	}
	testRand(N);
	//testNODEFile(N);
	//testBox();
	//newTest(N);
	return 0;
}
