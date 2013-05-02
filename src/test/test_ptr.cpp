#include <Eigen/AlignedVector>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <limits>

#include "qedge/Data.h"

using namespace Eigen;
using namespace std;



namespace Test{
class Edge;

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
	void print () { cout << "data : "<< i <<endl;}
};
const string IntData::type = "int";


class Vector2Data : public Data{
public:
	static const std::string type;
	Vector2f v;
	Vector2Data(Vector2f _v) : v(_v) {}
	void print () { cout << "data : ["<< v.transpose() <<"]"<<endl;}

};
const string Vector2Data::type = "vector2";


class QuadEdge {
public:
	typedef	 boost::shared_ptr<QuadEdge>  Ptr;
	std::vector<boost::shared_ptr<Edge> > edges;
};


class Edge {
	public:
	typedef	 boost::shared_ptr<Edge> Ptr;
	boost::shared_ptr<Data> data;
	QuadEdge::Ptr qedge;

	Edge(boost::shared_ptr<Data> _data) : data(_data) {}

	/** Print the data stored.*/
	void print() {
		data->print();
	}
};

class S {
private:
	int val;
public:
	S(int v) : val(v) {}
	static int ret(S o) {return o.val;}
};

};
int main (int argc, char* argv[]) {


	Test::Data::Ptr       i(new Test::IntData(1));
	Test::Data::Ptr       v(new Test::Vector2Data(Vector2f(1,2))  );

	Test::Edge::Ptr e1(new Test::Edge(i));
	Test::Edge::Ptr e2(new Test::Edge(v));

	Test::QuadEdge::Ptr qe(new Test::QuadEdge);
	qe->edges.push_back(e1);
	qe->edges.push_back(e2);

	for(int i=0; i < qe->edges.size(); i++)
		qe->edges[i]->print();

	Test::S temp(1);
	cout << "Return : "<<Test::S::ret(temp)<<endl;

	float inf = numeric_limits<float>::infinity();
	float Ninf = numeric_limits<float>::infinity();

	cout << "inf : "<<inf<<endl;
	cout << "n inf : "<<Ninf<<endl;
	cout << "inf/Ninf : "<<(inf/Ninf)<<endl;


	Vector2dPtr ptr(new Vector2d(1,2));
	cout << "Ptr check : "<<ptr->transpose()<<endl;

	return 0;

}
