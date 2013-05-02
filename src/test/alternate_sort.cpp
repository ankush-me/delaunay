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

/** Compare points::Ptr based on i_th coordinate.
 *   - Ties broken using subsequent coordinates. */
struct PtrCoordinateComparator : std::binary_function
<boost::shared_ptr<Eigen::Vector2d>, boost::shared_ptr<Eigen::Vector2d>, bool> {
	// dimensions of the vector
	const int d;
	// the coordinate index based on which two points should be compared
	const int i;
	PtrCoordinateComparator(int _d, int _i) : d(_d), i(modL(_i,d)) {}

	bool operator() (const boost::shared_ptr<Eigen::Vector2d> &v1,
			const boost::shared_ptr<Eigen::Vector2d> &v2) const {
		int c = i;
		do {
			if ((*v1)[c] == (*v2)[c])
				c = modL(c+1,d);
			else
				return ((*v1)[c] < (*v2)[c]);
		} while (c != i);
		return false;
	}
};

/* Sorts the pts b/w [start, end] (inclusive) indices,
 * based on COMP_I coordinate of the points, breaking ties
 * by circularly subsequent coordinates. */
void lexicoSort(vector<boost::shared_ptr<Vector2d> > & pts,
		int start, int end,  int comp_coord=0) {
	PtrCoordinateComparator comp(2, comp_coord);
	int n = pts.size();
	if (n != 0) {
		cout <<"sorting"<<endl;
		sort(pts.begin()+start, pts.begin()+end+1, comp);
	}
}


/** Partially sorts an array of points in [start, end] (inclusive)
 *  such that [start,mid] < [mid,end].
 *   - E[O(end-start+1)] time.
 *   - Returns the index of the median element: mid \in [start, end].
 *   - comp_coord : index of the coordinate based on which the
 *                  comparisons should be done first.
 *   - Uses nth_element function of the standard library.
 *   - Mutates the vector b/w [start, end]. */
int median(vector<boost::shared_ptr<Vector2d> > & pts,
		int start, int end, int comp_coord=0) {
	if (start > end) return -1;
	PtrCoordinateComparator comp(2, comp_coord);
	const int mid = start + (end-start)/2;
	nth_element(pts.begin()+start, pts.begin()+mid, pts.begin()+end+1, comp);
	return mid;
}

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
