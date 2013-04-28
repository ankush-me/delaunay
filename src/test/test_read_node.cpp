/**********************************
 *  Small test to test
 *     1. Reading .node files.
 *     2. lexico-graphic sorting.
 **********************************/

#include "qedge/io_utils.h"
#include "utils/sorting.h"

#include <Eigen/AlignedVector>

using namespace Eigen;
using namespace std;

int main (int argc, char* argv[]) {

	string files[] = {"spiral.node",
			"box.node",
			"grid.node",
			"flag.node"};

	string fname = string(EXPAND (PROJECT_DATA_DIR)) + "/" +  files[0];
	vector2d points;
	readNodeFile(fname, points);

	cout << "Number of points : " << points.size()<<endl;
	for (int i=0; i < points.size(); i++ ) {
		cout << "    ["<<points[i].transpose()<<"]"<<endl;
	}

	// test sorting.
	int min_idx = lexicoMin(points);
	int max_idx = lexicoMax(points);

	cout << "LexicoMin point : "<<points[min_idx].transpose()<<endl;
	cout << "LexicoMax point : "<<points[max_idx].transpose()<<endl;

	lexicoSort(points);
	cout << "Sorted points : "<<endl;
	for (int i=0; i < points.size(); i++ ) {
		cout << "    ["<<points[i].transpose()<<"]"<<endl;
	}

	return 0;
}
