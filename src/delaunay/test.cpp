/* TEST PROGRAM FOR DELAUNAY */

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <quadedge.h>

using namespace std;
#define VIEWING_DISTANCE_MIN  2.0


Point2d p1(-1,-1), p2(2,-1), p3(0.5,3);
Subdivision mesh(p1, p2, p3);

static bool g_LButtonUp        = false;
static GLfloat g_fInitViewDistance = VIEWING_DISTANCE_MIN;
static GLfloat g_fViewDistance = 1.2*VIEWING_DISTANCE_MIN;

static GLfloat g_fovy 			= 65.0;
static GLfloat g_nearPlane     = 1;
static GLfloat g_farPlane      = 1000;

static int g_Width            = 600;                          // Initial window width
static int g_Height           = 600;                         // Initial window height
static int g_yClick            = 0;
static float g_lightPos1[4] = { 10, 100, 0, 1 };  // Position of light
static float g_lightPos2[4] = { 10, 10, 100, 1 };  // Position of light

void getArguments(int, char**);
void InsertPoints(Subdivision&);

char *program;
int num = 20;



static void usage() {
	cout<< "usage: "<< program << " [ -n number_of_points ]\n";
}

void getArguments(int argc, char** argv) {
	program = argv[0];

	if(argc == 2 && strcmp(argv[1], "-h") == 0) {
		usage();
		exit(0);
	}

	int i=1;
	for (; i < argc && argv[i][0] == '-'; i++)
		if (strcmp(argv[i], "-n") == 0) {
			if(++i < argc)
				num = atoi(argv[i]);
			else
			cout << "option ``-n'': missing parameter"<<endl;
		} else
			cout << "unknown option"<<endl;

	if(argc - i > 0)
		cout << "too many parameters" <<endl;;
}


void InsertPoints(Subdivision& mesh)
{
	for (int i = 0; i < num; i++) {
		double u = drand48();
		double v = drand48();
		mesh.InsertSite(Point2d(u,v));
	}
}


long createWindow() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}


void drawMesh() {
	// Clear frame buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up viewing transformation, looking down -Z axis
	glLoadIdentity();
	gluLookAt(0, 0, -g_fViewDistance, 0, 0, -1, 0, 1, 0);


	// Set up the stationary light
	glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos1);
	glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos2);
	//	 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// Render the scene
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	mesh.Draw();

	glPopMatrix();
	glutSwapBuffers();
}


void printHelp() {
	cerr << "<left>	   insert a new point\n"
		 << "  <h>     print help message\n"
		 << "  <q>     quit\n";
}

void KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27:  // ESCAPE
		exit (0);
		break;
	}
	glutPostRedisplay();
}

void MouseButtonHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		g_LButtonUp  = (state == GLUT_UP) ? true : false;
		g_fInitViewDistance = g_fViewDistance;
		g_yClick = y;
	}
}

void reshape(GLint width, GLint height) {
	g_Width  = width;
	g_Height = height;
	glViewport(0, 0, g_Width, g_Height);
	glMatrixMode(GL_PROJECTION);    // edit the projection matrix
	glLoadIdentity();
	gluPerspective(g_fovy, (float)g_Width / g_Height, g_nearPlane, g_farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void InitGraphics(void) {
	int width, height;
	int nComponents;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void AnimateScene(void) {
	glutPostRedisplay();
}

int main(int argc, char** argv)  {
	//getArguments(argc, argv);

	// Initialize the mesh. Construct a triangle containing the unit square:
	InsertPoints(mesh);

	// GLUT Window Initialization:
	glutInit (&argc, argv);
	glutInitWindowSize (g_Width, g_Height);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Delaunay");

	// initialize OpenGL graphics state
	InitGraphics();

	// register callbacks:
	glutDisplayFunc(drawMesh);
	glutReshapeFunc  (reshape);

	// handle ui
	glutKeyboardFunc(KeyboardHandler);
	glutMouseFunc(MouseButtonHandler); // handle mouse clicks

	glutIdleFunc(AnimateScene);

	// Turn the flow of control over to GLUT
	glutMainLoop();

	exit(0);
}
