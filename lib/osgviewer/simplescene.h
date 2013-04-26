#pragma once
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include "environment.h"
#include "plotting.h"
#include <Eigen/Dense>
#include <boost/bind.hpp>
#include <boost/function.hpp>

const Eigen::Vector3f cameraHomePosition(5, 0, 5);
const Eigen::Vector3f cameraHomeCenter(0, 0, 0);
const Eigen::Vector3f cameraHomeUp(0, 0, 1);
const int windowHeight = 800;
const int windowWidth = 800;
const float zoomFactor = 0.1;

class Scene;

class EventHandler : public osgGA::TrackballManipulator {
private:
	Scene &scene;
public:
	EventHandler(Scene &scene_) : scene(scene_) {}
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	// for osg 2.8 - 3.x compatibility issues
	void getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const;
};


struct Scene {
	typedef boost::shared_ptr<Scene> Ptr;

	OSGInstance::Ptr osg;
	Environment::Ptr env;

	osgViewer::Viewer viewer;
	osg::ref_ptr<EventHandler> manip;


	//BoxObject::Ptr ground;

	// callbacks should return true if the default TrackballManipulator::handle behavior
	// should be suppressed. if all callbacks return false, then it won't be suppressed
	typedef boost::function<bool(const osgGA::GUIEventAdapter &)> Callback;
	typedef multimap<osgGA::GUIEventAdapter::EventType, Callback> CallbackMap;
	CallbackMap callbacks;

	void addCallback(osgGA::GUIEventAdapter::EventType t, Callback cb) { callbacks.insert(make_pair(t, cb)); }
	typedef multimap<int, Callback> KeyCallbackMap;
	KeyCallbackMap keyCallbacks;
	multimap<int, std::string> keyCallbackDescs;
	void addKeyCallback(int c, Callback cb, std::string desc="");

	typedef boost::function<void(void)> VoidCallback;
	void addVoidCallback(osgGA::GUIEventAdapter::EventType t, VoidCallback cb);
	void addVoidKeyCallback(int c, VoidCallback cb, std::string desc="");
	struct VoidCallbackWrapper {
		VoidCallback fn;
		VoidCallbackWrapper(VoidCallback fn_) : fn(fn_) { }
		bool operator()() { fn(); return false; }
	};

	vector<VoidCallback> prestepCallbacks;
	void addPreStepCallback(VoidCallback cb);

	vector<VoidCallback> predrawCallbacks;
	void addPreDrawCallback(VoidCallback cb);


	Scene();

	void showWindow(bool showWindow, bool realtime);

	bool drawingOn;
	void setDrawing(bool b) { drawingOn = b; }

	// Starts the viewer. Must be called before any step/draw/viewerLoop call
	// and after adding objects to the environment
	void startViewer();
	void help();

	// updates the display (if displayOn is true)
	// If syncTime is true, then these will block until the time interval passes on the system clock
	virtual void step();
	virtual void draw();

	virtual void run();
};
