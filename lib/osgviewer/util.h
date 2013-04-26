#ifndef __UTIL_H__
#define __UTIL_H__

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/AlignedVector>
#include <osg/Vec3d>
#include <osg/Geometry>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "plotting.h"

using namespace std;

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

void setGlobalEnv(Environment::Ptr env);
Environment::Ptr getGlobalEnv();

void toggle(bool* b);
void add(int* n, int increment);

namespace util {


  ///////////////// CONVERSIONS ////////////////////////////
  inline osg::Vec3d toOSGVector(const Eigen::Vector3f &v) { return osg::Vec3d(v.x(), v.y(), v.z()); }
  inline Eigen::Vector3f toEigenVector(const osg::Vec3d &v) { return Eigen::Vector3f(v.x(), v.y(), v.z()); }
  osg::ref_ptr<osg::Vec3Array> toVec3Array(const vector3&);
  osg::ref_ptr<osg::Vec4Array> toVec4Array(const vector4&);
  osg::ref_ptr<osg::Vec3Array> toVec3Array(const Eigen::MatrixXf& in);
  osg::ref_ptr<osg::Vec4Array> toVec4Array(const Eigen::MatrixXf& in);

  // Nan/Inf checking
  inline bool isfinite(const Eigen::Vector3f &v) {
      return std::isfinite(v.x()) && std::isfinite(v.y()) && std::isfinite(v.z());
  }

  inline bool isfinite(const Eigen::Matrix3f &m) {
	  Eigen::Vector3f x = m.col(0);
	  Eigen::Vector3f y = m.col(1);
	  Eigen::Vector3f z = m.col(2);

      return isfinite(x) && isfinite(y) && isfinite(y);
  }

  inline bool isfinite(const Eigen::Affine3f &t) {
	  Eigen::Matrix3f rot   = t.linear();
	  Eigen::Vector3f trans = t.translation();
      return isfinite(rot) && isfinite(trans);
  }

  ///////////////// PLOTTING FOR DEBUGGING ////////////////////////////
  //These plot and remain in the environment

  void setGlobalEnv(Environment::Ptr env);
  Environment::Ptr getGlobalEnv();

  std::vector<float> toVec(const Eigen::VectorXf& in);
  Eigen::MatrixXf toEigenMatrix(const vector3& in);

  void drawSpheres(vector3 points, Eigen::Vector3f color, float alpha, float radius, Environment::Ptr env);
  void drawSpheres(Eigen::Vector3f point, Eigen::Vector3f color, float alpha, float radius, Environment::Ptr env);
  void drawLines(vector3 points0, vector3 points1, Eigen::Vector3f color, float alpha, Environment::Ptr env);
  void drawPoly(vector3 points, Eigen::Vector3f color, float alpha, Environment::Ptr env);
  void drawAxes(Eigen::Affine3f transform, float size, Environment::Ptr env);
}

#endif // __UTIL_H__
