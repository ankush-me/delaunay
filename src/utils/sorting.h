#ifndef __SORTING_H__
#define __SORTING_H__

#include <Eigen/Dense>
#include <Eigen/AlignedVector>
#include <vector>
#include <iostream>
#include <algorithm>
#include "misc.h"
#include <boost/shared_ptr.hpp>

/** Compare points::Ptr based on i_th coordinate.
 *   - Ties broken using subsequent coordinates. */
struct PtrCoordinateComparator : std::binary_function
<boost::shared_ptr<Eigen::Vector2d>, boost::shared_ptr<Eigen::Vector2d>, bool> {
	// dimensions of the vector
	const int d;
	// the coordinate index based on which two points should be compared
	const int i;
	PtrCoordinateComparator(int _d, int _i);

	bool operator() (const boost::shared_ptr<Eigen::Vector2d> &v1,
			const boost::shared_ptr<Eigen::Vector2d> &v2) const ;
};


// compare points based on i_th coordinate. Ties broken using subsequent coordinates.
struct CoordinateComparator : std::binary_function <Eigen::VectorXd, Eigen::VectorXd, bool> {
	// dimensions of the vector
	const int d;

	// the coordinate index based on which two points should be compared
	const int i;
	CoordinateComparator(int _d, int _i);

	bool operator() (const Eigen::VectorXd &v1, const Eigen::VectorXd &v2) const;
};


// compare points lexico-graphically
struct Comparator : std::binary_function <Eigen::VectorXd, Eigen::VectorXd, bool> {
	int d; // dimensions of the vector
	Comparator(int _d);

	bool operator() (const Eigen::VectorXd &v1, const Eigen::VectorXd &v2) const ;
};

struct EqComparator : std::binary_function <Eigen::VectorXd, Eigen::VectorXd, bool> {
	int d; // dimensions of the vector
	EqComparator(int _d);

	bool operator() (const Eigen::VectorXd &v1, const Eigen::VectorXd &v2) const;
};



/* Sorts the pts b/w [start, end] (inclusive) indices,
 * based on COMP_I coordinate of the points, breaking ties
 * by circularly subsequent coordinates. */
void lexicoSort(std::vector<boost::shared_ptr<Eigen::Vector2d> > & pts,
		int start, int end,  int comp_coord=0);

/** Partially sorts an array of points in [start, end] (inclusive)
 *  such that [start,mid] < [mid,end].
 *   - E[O(end-start+1)] (linear) time.
 *   - Returns the index of the median element: mid \in [start, end].
 *   - comp_coord : index of the coordinate based on which the
 *                  comparisons should be done first.
 *   - Uses nth_element function of the standard library.
 *   - Mutates the vector b/w [start, end]. */
int median(std::vector<boost::shared_ptr<Eigen::Vector2d> > & pts,
		int start, int end, int comp_coord=0);


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
