#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"

#include "utils/geom_predicates.h"
#include "utils/sorting.h"

#include "qedge/Data.h"
#include "qedge/DelaunaySubdivision.h"


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

int main (int argc, char* argv[]) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);

	const int N = 10;
	MatrixXd randm = MatrixXd::Random(N,2);
	vector2d pts(N);
	for (int i = 0 ; i < N; i+=1)
		pts[i] = randm.row(i);

	lexicoSort(pts);
	cout << "sorted points."<<endl;
	vector<Vector2dPtr> ptrPTS = toPtrVector(pts);
	DelaunaySubdivision subD(VERTICAL_CUTS);
	subD.divideConquerVerticalCuts(ptrPTS, 0, N-1);

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
