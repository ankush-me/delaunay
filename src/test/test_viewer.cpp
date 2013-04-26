#include "osgviewer/simplescene.h"
#include "osgviewer/plotting.h"
#include "osgviewer/util.h"

#include "utils/geom_predicates.h"
#include "utils/sorting.h"

#include <Eigen/AlignedVector>

using namespace Eigen;
using namespace std;

class CustomScene : public Scene{};


vector3 to3d(const vector2 &pts) {
	vector3 pts3d(pts.size());
	for(int i =0 ; i < pts.size(); i++ ) {
		Vector2f pt = pts[i];
		pts3d[i] = Vector3f(pt.x(), pt.y(), 0);
	}

	return pts3d;
}



int main (int argc, char* argv[]) {
	CustomScene s;
	Eigen::Affine3f tf = Eigen::Affine3f::Identity();
	util::drawAxes(tf, 0.5, s.env);


	PlotPolygons::Ptr polys(new PlotPolygons);
	s.env->add(polys);

	vector3 verts;
	verts.push_back(Vector3f(1,0,0));
	verts.push_back(Vector3f(0,1,0));
	verts.push_back(Vector3f(0,0,1));
	polys->setDefaultColor(0.6,0.3,0,0.7);
	polys->addPolygon(verts);

	//draws a sequence of lines connecting a pt to its successor
	util::drawPoly(verts, Vector3f(1,0,0), 1, s.env);

	s.run();
}
