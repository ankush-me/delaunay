/*******************************
 * Author: Ankush Gupta
 * SID   : 22388145
 *
 * Utility functions for file I/O.
 **********************************/

#include "io_utils.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <Eigen/Dense>
#include <boost/lexical_cast.hpp>

using namespace Eigen;
using namespace std;

/** Reads a .node file specifying 2-dimensional points
 *  into a vector of Eigen::Vector2d points.*/
void readNodeFile(const std::string &fname, vector2d &pts) {

	bool readFirstLine        = false;
	unsigned int N           = -1;
	unsigned int dim         = -1;
	unsigned int i           =  0;

	ifstream inpfile(fname.c_str());
	if(!inpfile.is_open()) {
		cout << "Unable to open file : " << fname << endl;
	} else {
		while(!inpfile.eof()) {
			string line;
			getline(inpfile,line);
			vector<string> splitline;
			string buf;
			stringstream ss(line);

			while (ss >> buf)  // extract white-space separated strings on a line
				splitline.push_back(buf);

			if (splitline.size()==0)   // skip the blank lines
				continue;
			if(splitline[0][0] == '#') // comments
				continue;

			if (!readFirstLine) {
				assert(("\nFirst line of .node should be\n\t"
						"<# of vertices> <dimension (must be 2)> <# of attributes>"
						"<# of boundary markers (0 or 1)>\n", splitline.size()== 4));
				N   = atoi(splitline[0].c_str());
				dim = atoi(splitline[1].c_str());
				assert(("\nDimension of vertices must be 2\n", dim==2));
				pts.clear();
				pts.resize(N);
				i = 0;
				readFirstLine = true;
			} else { // read the vertices
				assert(("\nInsufficient data while reading .node file. "
						"Vertices should be specified in the following format :\n\t"
						"<vertex #> <x> <y> [attributes] [boundary marker]\n", splitline.size() >= 3));
				double x = boost::lexical_cast<double>(splitline[1]);
				double y = boost::lexical_cast<double>(splitline[1]);
				Eigen::Vector2d pt(x,y);

				if (i < N) {
					pts[i] = pt;
					i += 1;
				} else {
					cout << "Expecting "<< N << " points. Found more while reading "
						 << fname << ". Skipping."<< endl; break;}
			}
		}
	}
}
