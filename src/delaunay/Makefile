CFLAGS  = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
LDFLAGS = -lglut -lGLU

delaunay: test.o quadedge.o
	g++ -o delaunay test.o quadedge.o $(CLFAGS) $(LDFLAGS)

test.o: geom2d.h quadedge.h test.cpp
	g++ -I. -c test.cpp  $(CLFAGS) $(LDFLAGS)

quadedge.o: geom2d.h quadedge.h quadedge.C
	g++ -I. -c quadedge.C  $(CLFAGS) $(LDFLAGS)
