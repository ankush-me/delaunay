/**
 *  Wrappers for common types of data.
 *  Defined so that an Edge can contain generic data. */


#ifndef __DATAA_H__
#define __DATAA_H__

#include <iostream>
#include <Eigen/Dense>
#include <boost/shared_ptr.hpp>


// Pointers to Eigen points.
namespace Vector2d {
	typedef boost::shared_ptr<Eigen::Vector2d> Ptr;
}
namespace Vector2f {
	typedef boost::shared_ptr<Eigen::Vector2f> Ptr;
}


class Data {
public:
	typedef boost::shared_ptr<Data> Ptr;
	virtual void print() =0;
};


class IntData : public Data{
public:
	static const std::string type;
	int i;
	IntData(int _i) : i(_i) {}
	void inline print () { std::cout << "data : "<< i <<std::endl;}
};


class Vector2Data : public Data{
public:
	static const std::string type;
	Eigen::Vector2f v;
	Vector2Data(Eigen::Vector2f _v) : v(_v) {}
	void inline print () { std::cout << "data : ["<< v.transpose() <<"]"<<std::endl;}
};

#endif // __DATAA_H__
