#include "sorting.h"
#include <algorithm>

#include <algorithm>

using namespace Eigen;
using namespace std;


/** Comparator used in sorting vectors. Implements v1<v2 (LESS THAN).*/
bool Comparator::operator() (const VectorXf &v1, const VectorXf &v2) const {
	int i = 0;
	while(i < d && v1[i] == v2[i]) {i++;}
	return (i==d)? false :  v1[i] < v2[i];
}


/** Comparator used in sorting vectors. */
bool EqComparator::operator() (const VectorXf &v1, const VectorXf &v2) const {
	int i = 0;
	while(i < d && v1[i] == v2[i]) {i++;}
	return (i==d);
}


/** Returns the lexicographically minimum points in a list of points.*/
int lexicoMin(const MatrixXf &mat_nd) {
	const int n = mat_nd.rows();
	if (!n) return -1;

	const int d = mat_nd.cols();
	Comparator isLess(d);

	VectorXf min_pt = mat_nd.row(0);
	int min_idx = 0;
	for (int i= 1; i < n; i += 1) {
		if (isLess(mat_nd.row(i), min_pt) ) {
			min_pt = mat_nd.row(i);
			min_idx = i;
		}
	}
	return min_idx;
}


template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMin(const vector<Matrix<T, R,C,  _Options, _MaxRows, _MaxCols> > &mat_nd) {

	const int n = mat_nd.size();
	if (!n) return -1;

	const int d = mat_nd[0].size();
	Comparator isLess(d);

	int min_idx = 0;

	for (int i= 1; i < n; i += 1) {
		if (isLess(mat_nd[i], mat_nd[min_idx]) )
			min_idx = i;
	}
	return min_idx;
}


template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMin(const vector<Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		          aligned_allocator<Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
	const int n = mat_nd.size();
	if (!n) return -1;

	const int d = mat_nd[0].size();
	Comparator isLess(d);

	int min_idx = 0;

	for (int i= 1; i < n; i += 1) {
		if (isLess(mat_nd[i], mat_nd[min_idx]) )
			min_idx = i;
	}
	return min_idx;
}
template int lexicoMin  (const vector2 & mat);
template int lexicoMin  (const vector3 & mat);
template int lexicoMin  (const vector4 & mat);
template int lexicoMin  (const std::vector<Eigen::VectorXf> & mat);



/** Returns the lexicographically maximum points in a list of points.*/
int lexicoMax(const MatrixXf &mat_nd) {
	const int n = mat_nd.rows();
	if (!n) return -1;

	const int d = mat_nd.cols();
	Comparator isLess(d);

	VectorXf min_pt = mat_nd.row(0);
	int min_idx = 0;

	for (int i= 1; i < n; i += 1) {
		if (!isLess(mat_nd.row(i), min_pt) ) {
			min_pt = mat_nd.row(i);
			min_idx = i;
		}
	}
	return min_idx;
}

template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMax(const vector<Matrix<T, R,C,  _Options, _MaxRows, _MaxCols> > &mat_nd) {

	const int n = mat_nd.size();
	if (!n) return -1;

	const int d = mat_nd[0].size();
	Comparator isLess(d);

	int min_idx = 0;

	for (int i= 1; i < n; i += 1) {
		if (!isLess(mat_nd[i], mat_nd[min_idx]) )
			min_idx = i;
	}
	return min_idx;
}

template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMax(const vector<Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		          aligned_allocator<Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
	const int n = mat_nd.size();
	if (!n) return -1;

	const int d = mat_nd[0].size();
	Comparator isLess(d);

	int min_idx = 0;

	for (int i= 1; i < n; i += 1) {
		if (!isLess(mat_nd[i], mat_nd[min_idx]) )
			min_idx = i;
	}
	return min_idx;
}
template int lexicoMax  (const vector2 & mat);
template int lexicoMax  (const vector3 & mat);
template int lexicoMax  (const vector4 & mat);
template int lexicoMax  (const std::vector<Eigen::VectorXf> & mat);



/** Sort a matrix NxD (N points in D dimensions) lexicographically. In-place. */
void lexicoSort(Eigen::MatrixXf &mat_nd) {
	int n = mat_nd.rows();
	int d = mat_nd.cols();

	vector<VectorXf> dat(n);
	for (int i=0; i<n; i++)
		dat[i] = mat_nd.row(i);

	sort(dat.begin(), dat.end(), Comparator(d));

	//copy back
	for (int i=0; i<n; i++)
		mat_nd.row(i) = dat[i];
}


/** Sort a vector of points lexicographically. In-place.*/
template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
void lexicoSort(vector<Matrix<T, R, C, _Options, _MaxRows, _MaxCols> >& mat_nd) {
	int n = mat_nd.size();
	if (n != 0) {
		int d = mat_nd[0].size();
		sort(mat_nd.begin(), mat_nd.end(), Comparator(d));
	}
}

/** Sort a vector of points lexicographically. In-place.*/
template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
void lexicoSort(vector<Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		Eigen::aligned_allocator<Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
	int n = mat_nd.size();
	if (n != 0) {
		int d = mat_nd[0].size();
		sort(mat_nd.begin(), mat_nd.end(), Comparator(d));
	}
}
/** Force the compiler to generate the following instantiations of lexicoSort. */
template void lexicoSort  (vector2 & mat);
template void lexicoSort  (vector3 & mat);
template void lexicoSort  (vector4 & mat);
template void lexicoSort  (std::vector<Eigen::VectorXf> & mat);
