#include "qedge/io_utils.h"
#include <Eigen/AlignedVector>

using namespace Eigen;
using namespace std;

int main (int argc, char* argv[]) {

	string files[] = {"spiral.node",
			          "box.node",
			          "grid.node",
			          "flag.node"};

	string fname = string(EXPAND (PROJECT_DATA_DIR)) + "/" +  files[2];
	vector2d points;
	readNodeFile(fname, points);

	cout << "Number of points : " << points.size()<<endl;
	for (int i=0; i < points.size(); i++ ) {
		cout << "    ["<<points[i].transpose()<<"]"<<endl;
	}

	return 0;
}
