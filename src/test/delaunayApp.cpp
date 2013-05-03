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
		"./delaunay -i input_file_name [-o output_file_name] [-A or V] [-T]\n"
		"    -i input_file_name  : path to .node file\n"
		"    -o output_file_name : path to .ele file [optional].\n"
		"                          If not specified the .ele file is\n"
		"                          stored in the same directory as .node file.\n"
		"    -A or -V            : switch between alternating [default] or vertical cuts.\n"
		"    -T                  : print the time taken for the algorithm to run.\n"
		"                          File I/O time is excluded.\n\n";

void printUsageAndExit() {
	cout << usage;
	exit(0);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		cout << welcome;
		exit(0);
	}
	string input, output;

	bool vertical       = false;
	bool time_algorithm = false;
	for ( int i = 1; i < argc; ++i ) {
		std::string arg( argv[i] );
		if ( arg == "--help" || arg == "-h" ) {
			printUsageAndExit();
		} else if ( arg == "-i") {
			if ( i == argc-1 ) {
				printUsageAndExit();
			}
			input = argv[++i];
		} else if ( arg == "-o" ) {
			output = argv[++i];
		} else if ( arg == "-V" ) {
			vertical = true;
		} else if (arg == "-T") {
			time_algorithm = true;
		} else if (arg == "-A") {
			continue;
		} else {
			cout << "Unknown option: '" << arg << "'\n";
			printUsageAndExit();
		}
	}

	if(!input.length())
		printUsageAndExit();

	if (!output.length()) {
		DelaunaySubdivision subD(input);
		subD.computeDelaunay(((vertical)? VERTICAL_CUTS : ALTERNATE_CUTS), time_algorithm);
		subD.writeToFile();
	} else {
		DelaunaySubdivision subD(input, output);
		subD.computeDelaunay(((vertical)? VERTICAL_CUTS : ALTERNATE_CUTS), time_algorithm);
		subD.writeToFile();
	}
}
