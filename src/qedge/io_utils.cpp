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
#include <boost/unordered_set.hpp>
#include "Data.h"

using namespace Eigen;
using namespace std;

/** Reads a .node file specifying 2-dimensional points
 *  into a vector of Eigen::Vector2d points.*/
/** Reads a .node file specifying 2-dimension points
 *  into a vector of pointers to Eigen::Vector2d points.*/
void readNodeFile(const std::string &fname,
		std::vector<Vector2dPtr> &pts,
		boost::unordered_map<Vector2dPtr, int> &pt2index) {

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
				assert(("First line of .node should be : "
						"<# of vertices> <dimension (must be 2)> <# of attributes>"
						"<# of boundary markers (0 or 1)>", splitline.size()== 4));
				N   = atoi(splitline[0].c_str());
				dim = atoi(splitline[1].c_str());
				assert(("Dimension of vertices must be 2", dim==2));
				pts.clear();
				pt2index.clear();
				pts.resize(N);
				i = 0;
				readFirstLine = true;
			} else { // read the vertices
				assert(("Insufficient data while reading .node file. "
						"Vertices should be specified in the following format : "
						"<vertex #> <x> <y> [attributes] [boundary marker]", splitline.size() >= 3));
				int index =  boost::lexical_cast<int>(splitline[0]);
				double x  = boost::lexical_cast<double>(splitline[1]);
				double y  = boost::lexical_cast<double>(splitline[2]);

				if (i < N) {
					pts[i] = Vector2dPtr(new Vector2d(x,y));
					pt2index[pts[i]] = index;
					i += 1;
				} else {
					cout << ">>> Expecting "<< N << " points. Found more while reading "
							<< fname << ". Skipping."<< endl; break;}
			}
		}
	}
}


void reportTriangle(Edge::Ptr e, DelaunaySubdivision::Ptr subD,
		boost::unordered_set<Edge::Ptr> &marked,
		std::vector<std::vector<int> >  &tris) {
	if (marked.find(e) == marked.end()) {// not marked
		Edge::Ptr f = e;
		vector<int> tri(3);
		vector<Vector2dPtr> vertices(3);

		int i = 0;

		do {
			marked.insert(f);
			if (i < 3) {
				tri[i] = subD->pt2index[f->RotInv()->org()];
				vertices[i] = f->RotInv()->org();
			}
			i += 1;
			f = f->Onext();
		} while (f != e);

		if (i == 3 && ccw(vertices[0], vertices[1], vertices[2]))
			tris.push_back(tri);
	}
}


/** Writes an .ele file, corresponding to the .node file
 *  which was used to construct the triangulation.
 *  The name of the output file is fname.ele. */
void writeSubdivision(const std::string &fname, const DelaunaySubdivision::Ptr subD) {
	boost::unordered_set<Edge::Ptr> marked;
	vector<vector<int> > tris;

	for(boost::unordered_set<QuadEdge::Ptr>::iterator it = subD->qedges.begin();
			it != subD->qedges.end(); it++)
		reportTriangle((*it)->edges[0]->Rot(), subD, marked, tris);

	// print vertices
	cout << "Found : "<<tris.size()<< " triangles.\n";
	for (int t=0; t<tris.size(); t+=1)
		cout << tris[t][0]<<" "<< tris[t][1] << " "<< tris[t][2]<<endl;
}
