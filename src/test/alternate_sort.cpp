/** Small test to sort points based on alternating coordinates.*/
#include "utils/geom_predicates.h"
#include "utils/sorting.h"

#include <Eigen/AlignedVector>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <limits>

#include <algorithm>

using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {

	const int N = 6;

	//	MatrixXd randm = MatrixXd::Random(N,2);
	//	vector2d pts(N);
	//	for (int i = 0 ; i < N; i+=1) {
	//		pts[i] = randm.row(i);
	//		cout <<pts[i].transpose()<<endl;
	//	}
	//
	//

	vector<boost::shared_ptr<Vector2d> > pts(N);
	MatrixXd randm = MatrixXd::Random(N,2);
	for (int i = 0 ; i < N; i+=1)
		pts[i].reset(new Vector2d(randm.row(i)));

	int s = 1;
	int e = 4;

	for (int i = 0 ; i < N; i+=1)
		cout <<pts[i]->transpose()<<endl;

	//lexicoSort(pts, 1, s, e);
	int mid = median(pts, s, e, 0);

	cout <<"-------------\n";
	cout << "median int : "<<mid<<endl;
	for (int i = 0 ; i < N; i+=1) {
		cout <<pts[i]->transpose()<<endl;
	}

	//median = a[size/2];
	return 0;
}
