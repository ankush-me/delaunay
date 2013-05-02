#include "geom_predicates.h"

// Shewchuk's robust predicates
extern "C" {
	#include "predicates.c"
}

using namespace Eigen;


double orient2d (Vector2d pa, Vector2d pb, Vector2d pc) {
	return (double) orient2d(pa.data(), pb.data(), pc.data());
}

double orient2d (Vector2f pa, Vector2f pb, Vector2f pc) {
	Vector2d v1((double) pa[0], (double) pa[1]);
	Vector2d v2((double) pb[0], (double) pb[1]);
	Vector2d v3((double) pc[0], (double) pc[1]);
	return (double) orient2d(v1.data(), v2.data(), v3.data());
}

double incircle (Eigen::Vector2d pa, Eigen::Vector2d pb, Eigen::Vector2d pc) {
	return incircle(pa.data(), pb.data(), pc.data());
}

double incircle (Eigen::Vector2f pa, Eigen::Vector2f pb, Eigen::Vector2f pc) {
	Vector2d v1((double) pa[0], (double) pa[1]);
	Vector2d v2((double) pb[0], (double) pb[1]);
	Vector2d v3((double) pc[0], (double) pc[1]);
	return incircle(v1.data(), v2.data(), v3.data());
}

bool ccw(Vector2d pa, Vector2d pb, Vector2d pc) {
	return (bool) (orient2d(pa, pb, pc) > 0.0);
}
bool ccw(Vector2f pa, Vector2f pb, Vector2f pc) {
	return (bool)(orient2d(pa, pb, pc) > 0.0);
}


bool cw(Vector2d pa, Vector2d pb, Vector2d pc) {
	return (bool) (orient2d(pa, pb, pc) < 0.0);
}
bool cw(Vector2f pa, Vector2f pb, Vector2f pc) {
	return (bool) (orient2d(pa, pb, pc) < 0.0);
}
