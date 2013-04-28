#ifndef __SORTING_H__
#define __SORTING_H__

#include <Eigen/Dense>
#include <Eigen/AlignedVector>
#include <vector>
#include <iostream>
#include <algorithm>


// compare points lexico-graphically
struct Comparator : std::binary_function <Eigen::VectorXd, Eigen::VectorXd, bool> {
	int d; // dimensions of the vector
	Comparator(int _d) : d(_d) {}

	bool operator() (const Eigen::VectorXd &v1, const Eigen::VectorXd &v2) const {
		int i = 0;
		while(i < d && v1[i] == v2[i]) {i++;}
		return (i==d)? false :  v1[i] < v2[i];
	}
};

struct EqComparator : std::binary_function <Eigen::VectorXd, Eigen::VectorXd, bool> {
	int d; // dimensions of the vector
	EqComparator(int _d) : d(_d) {}

	bool operator() (const Eigen::VectorXd &v1, const Eigen::VectorXd &v2) const {
		int i = 0;
		while(i < d && v1[i] == v2[i]) {i++;}
		return (i==d);
	}
};

/** Templatized comparator for lexicographic less-than test.
 *  The comparisons are made of the indices based on the points stores in PTS.*/
template <typename T, typename T_allocator>
struct IndexedComparator : std::binary_function <int, int, bool> {
private:

public:
	int d; // dimensions of the vector
	std::vector<T, T_allocator> * pts;
	int N;

	IndexedComparator(int _d, std::vector<T, T_allocator> *_pts) : d(_d), pts(_pts), N(_pts->size()) {}

	bool operator() (const int &i1, const int &i2) const {
		int i = 0;
		while(i < d && pts->at(i1)[i] == pts->at(i2)[i]) {i++;}
		bool res = (i==d)? false : pts->at(i1)[i] < pts->at(i2)[i];
		return res;
	}
};

/** Sort a vector of points lexicographically. In-place. */
/** Sort a vector of points lexicographically. In-place. Redefined for aligned-vectors.*/
template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
void lexicoSort(std::vector<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		Eigen::aligned_allocator<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
	int n = mat_nd.size();
	if (n != 0) {
		int d = mat_nd[0].size();
		sort(mat_nd.begin(), mat_nd.end(), Comparator(d));
	}
}

/** Returns the index of the lexicographically MINIMUM point in a list of points.*/
template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMin(const std::vector<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		Eigen::aligned_allocator<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
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

/** Returns the index of the lexicographically MAXIMUM point in a list of points.*/
template <typename T, int R, int C, int _Options, int _MaxRows, int _MaxCols>
int lexicoMax(const std::vector<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols>,
		Eigen::aligned_allocator<Eigen::Matrix<T, R, C, _Options, _MaxRows, _MaxCols> > >& mat_nd) {
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

#endif
