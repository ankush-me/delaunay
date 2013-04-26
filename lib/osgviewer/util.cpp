#include <math.h>
#include "util.h"
#include <boost/foreach.hpp>
#include <vector>

using namespace Eigen;

void toggle(bool* b){
	*b = !(*b);
}

void add(int* n, int increment) {
	*n += increment;
}

namespace util {


std::vector<float> toVec(const Eigen::VectorXf& in) {
  std::vector<float> out(in.rows());
  for (int i=0; i < out.size(); i++) out[i] = in[i];
  return out;
}

Eigen::MatrixXf toEigenMatrix(const vector3& in) {
  Eigen::MatrixXf out(in.size(), 3);
  for (int i=0; i<in.size(); i++) out.row(i) = in[i].transpose();
  return out;
}


osg::ref_ptr<osg::Vec3Array> toVec3Array(const vector3& in) {
    osg::ref_ptr<osg::Vec3Array> out = new osg::Vec3Array();
    out->reserve(in.size());
    for (int i=0; i < in.size(); i+=1) {
    	Vector3f pt = in[i];
    	out->push_back(osg::Vec3(pt.x(),pt.y(),pt.z()));
    }
    return out;
}

osg::ref_ptr<osg::Vec4Array> toVec4Array(const vector4& in) {
    osg::ref_ptr<osg::Vec4Array> out = new osg::Vec4Array();
    out->reserve(in.size());
    for (int i=0; i < in.size(); i+=1) {
    	Vector4f pt = in[i];
        out->push_back(osg::Vec4(pt.x(),pt.y(),pt.z(),pt.w()));
    }
    return out;
}

osg::ref_ptr<osg::Vec3Array> toVec3Array(const Eigen::MatrixXf& in) {
	assert(in.cols() == 3);
    osg::ref_ptr<osg::Vec3Array> out = new osg::Vec3Array();
    out->reserve(in.rows());
    for (int row=0; row < in.rows(); row++)
    	out->push_back(osg::Vec3(in(row, 0), in(row, 1), in(row, 2)));
    return out;
}

osg::ref_ptr<osg::Vec4Array> toVec4Array(const Eigen::MatrixXf& in) {
		assert(in.cols() == 4);
    osg::ref_ptr<osg::Vec4Array> out = new osg::Vec4Array();
    out->reserve(in.rows());
    for (int row=0; row < in.rows(); row++)
    	out->push_back(osg::Vec4(in(row, 0), in(row, 1), in(row, 2), in(row, 3)));
    return out;
}

void drawSpheres(vector3 points, Eigen::Vector3f color, float alpha, float radius, Environment::Ptr env) {
	PlotSpheres::Ptr plot_spheres(new PlotSpheres());
	env->add(plot_spheres);

	MatrixXf centers = toEigenMatrix(points);
	MatrixXf rgba(points.size(), 4);
	rgba << color.transpose().replicate(points.size(),1) , VectorXf::Constant(points.size(), alpha);
	VectorXf sizes = VectorXf::Constant(points.size(), radius);
	plot_spheres->plot(util::toVec3Array(centers), util::toVec4Array(rgba), toVec(sizes));
}

void drawSpheres(Eigen::Vector3f point, Vector3f color, float alpha, float radius, Environment::Ptr env) {
	drawSpheres(vector3(1,point), color, alpha, radius, env);
}

void drawLines(vector3 points0, vector3 points1, Vector3f color, float alpha, Environment::Ptr env) {
	assert(points0.size() == points1.size());
	PlotLines::Ptr plot_lines(new PlotLines());
	env->add(plot_lines);

	vector3 linePoints;
	vector4 lineColors;

	for (int i=0; i<points0.size(); i++) {
		linePoints.push_back(points0[i]);
		linePoints.push_back(points1[i]);
		lineColors.push_back(Vector4f(color(0), color(1), color(2), alpha));
	}
	plot_lines->setPoints(linePoints, lineColors);
}

void drawPoly(vector3 points, Vector3f color, float alpha, Environment::Ptr env) {
	vector3 pts0;
	vector3 pts1;
	for (int i=0; i<(points.size()-1); i++) {
		pts0.push_back(points[i]);
		pts1.push_back(points[i+1]);
	}
	pts0.push_back(points[points.size()-1]);
	pts1.push_back(points[0]);

	drawLines(pts0, pts1, color, alpha, env);
}

void drawAxes(Eigen::Affine3f transform, float size, Environment::Ptr env) {
	PlotAxes::Ptr plot_axes(new PlotAxes());
	env->add(plot_axes);

	plot_axes->setup(transform, size);
}

static Environment::Ptr gEnv;
void setGlobalEnv(Environment::Ptr env) { gEnv = env; }
Environment::Ptr getGlobalEnv() { return gEnv; }
}
