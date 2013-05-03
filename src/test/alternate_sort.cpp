/** Small test to sort points based on alternating coordinates.*/
#include "utils/geom_predicates.h"
#include "utils/sorting.h"

#include <Eigen/AlignedVector>
#include <boost/shared_ptr.hpp>

#include "qedge/Data.h"

#include <vector>
#include <limits>

#include <algorithm>

using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {

	const int N = 4;

	//	MatrixXd randm = MatrixXd::Random(N,2);
	//	vector2d pts(N);
	//	for (int i = 0 ; i < N; i+=1) {
	//		pts[i] = randm.row(i);
	//		cout <<pts[i].transpose()<<endl;
	//	}
	//
	//

	vector<boost::shared_ptr<Vector2d> > pts(N);
	pts[0].reset(new Vector2d(0,0));
	pts[1].reset(new Vector2d(1,0));
	pts[2].reset(new Vector2d(1,1));
	pts[3].reset(new Vector2d(0,1));

//	MatrixXd randm = MatrixXd::Random(N,2);
//	for (int i = 0 ; i < N; i+=1)
//		pts[i].reset(new Vector2d(randm.row(i)));

	int s = 0;
	int e = 3;

	for (int i = 0 ; i < N; i+=1)
		cout <<pts[i]->transpose()<<endl;

	//lexicoSort(pts, 1, s, e);
	int mid = median(pts, s, e, 1);

	cout <<"-------------\n";
	cout << "median int : "<<mid<<endl;
	for (int i = 0 ; i < N; i+=1) {
		cout <<pts[i]->transpose()<<endl;
	}

	return 0;
}
