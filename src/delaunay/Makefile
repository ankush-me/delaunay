delaunay: test.o quadedge.o
	g++ -o delaunay test.o quadedge.o -lgl_s -lX11 -lm

test.o: geom2d.h quadedge.h test.C
	g++ -I. -c test.C

quadedge.o: geom2d.h quadedge.h quadedge.C
	g++ -I. -c quadedge.C
