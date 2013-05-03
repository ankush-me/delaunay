#include <iostream>
#include "qedge/DelaunaySubdivision.h"


using namespace std;

string welcome =
			"*******************************************************\n\n"
			"                  DELAUNAY TRIANGULATOR        \n\n"
			"                      Ankush Gupta              \n\n"
			"*******************************************************\n\n"
			"This program computes the delaunay triangulation of"
			"\n2-dimensional points.\n"
			"The algorithm is based on Stolfi and Guibas [1985] paper.\n"
			"It uses Jonathan Shewchuck's robust geometric predicates.\n"
			"The file format for input/output is specified at : \n"
			"    http://www.cs.cmu.edu/~quake/triangle.node.html\n\n"
			"Please use Shewchuk's \'ShowMe\' program for visualization.\n"
			"It can be found at :\n"
			"    http://www.cs.cmu.edu/~quake/triangle.html\n\n"
			"This program was written by Ankush Gupta for CS274,\n"
			"Computational Geometry, Spring 2013, at \n"
			"University of California, Berkeley.\n\n"
			"Use \n    ./delaunay -h\nfor help on how to use this program.\n"
			"*******************************************************\n\n";

string usage =
		"./delaunay -i input_file_name [-o output_file_name] [-A or V]\n"
		"    -i input_file_name  : path to .node file\n"
		"    -o output_file_name : path to .ele file [optional].\n"
		"                          If not specified the .ele file is\n"
		"                          stored in the same directory as .node file.\n"
		"    -A or -V            : switch between alternating [default] or vertical cuts.\n";


void printUsageAndExit(char * v) {
	cout << usage;
	exit(0);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		cout << welcome;
		exit(0);
	}


	for ( int i = 1; i < argc; ++i ) {
		std::string arg( argv[i] );
		if ( arg == "--help" || arg == "-h" ) {
			printUsageAndExit( argv[0] );
		} else if ( arg == "--file" || arg == "-f" ) {
			if ( i == argc-1 ) {
				printUsageAndExit( argv[0] );
			}
			filename = argv[++i];
		} else if ( arg == "--no-kinect" ) {
			no_kinect = true;
		} else if ( arg == "--tf-suffix" ) {
			tf_suffix = argv[++i];
		} else {
			std::cerr << "Unknown option: '" << arg << "'\n";
			printUsageAndExit( argv[0] );
		}
	}
}
