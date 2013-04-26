#include "simplescene.h"
#include "util.h"

#include <iostream>

using namespace std;
using namespace Eigen;

Scene::Scene() {
    osg.reset(new OSGInstance());
    env.reset(new Environment(osg));

    // default callbacks
    addVoidKeyCallback('h', boost::bind(&Scene::help, this), "display help info");
    addVoidKeyCallback(osgGA::GUIEventAdapter::KEY_Escape, boost::bind(exit, 0), "(escape) exit");

    drawingOn = false;
}

void Scene::startViewer() {
    drawingOn = true;

    viewer.setUpViewInWindow(0, 0, windowWidth, windowHeight);
    manip = new EventHandler(*this);
    manip->setHomePosition(util::toOSGVector(cameraHomePosition), util::toOSGVector(cameraHomeCenter), util::toOSGVector(cameraHomeUp));
    manip->setWheelZoomFactor(zoomFactor);
    viewer.setCameraManipulator(manip);
    viewer.setSceneData(osg->root.get());

    //viewer.getCamera()->getView()->setLightingMode(osg::View::NO_LIGHT);

    osg::Light* light = new osg::Light();
    osg::LightSource * lightsource = new osg::LightSource();
    lightsource->setLight(light);
    osg->root->addChild(lightsource);

    osg::StateSet* stateset = osg->root->getOrCreateStateSet();
    lightsource->setStateSetModes(*stateset, osg::StateAttribute::ON);

    light->setAmbient(osg::Vec4d(0.0, 0.0, 0.0, 1.0));
    light->setDiffuse(osg::Vec4d(1.0, 1.0, 1.0, 1.0));
    light->setSpecular(osg::Vec4d(0.5, 0.2, 0.5, 1.0));
    light->setPosition(osg::Vec4d(2, 3, 3.0, 1.0));

    viewer.realize();
    step();
}


void Scene::help() {
  printf("key bindings:\n-------------\n");
  for (multimap<int,string>::iterator it = keyCallbackDescs.begin(); it != keyCallbackDescs.end(); ++it) {
    printf("\t%c : %s\n", (char)it->first, it->second.c_str());
  }
}

void Scene::step() {
    // run pre-step callbacks
    for (int i = 0; i < prestepCallbacks.size(); ++i)
        prestepCallbacks[i]();

    env->step();

    // run pre-draw callbacks
    for (int i = 0; i < predrawCallbacks.size(); ++i)
        predrawCallbacks[i]();
    draw();
}

void Scene::run() {
	startViewer();
	while (drawingOn && !viewer.done())
		step();
}

void Scene::draw() {
    if (!drawingOn)
        return;
    viewer.frame();
}

void Scene::addKeyCallback(int c, Callback cb, std::string desc) {
    if (keyCallbacks.count(c) != 0)
        cout << "warning: key " << c << " is bound to multiple callbacks" << endl;
    keyCallbacks.insert(make_pair(c, cb));
    keyCallbackDescs.insert(make_pair(c, desc));
}

void Scene::addVoidCallback(osgGA::GUIEventAdapter::EventType t, VoidCallback cb) {
    addCallback(t, boost::bind<bool>(VoidCallbackWrapper(cb)));
}
void Scene::addVoidKeyCallback(int c, VoidCallback cb, std::string desc) {
    addKeyCallback(c, boost::bind<bool>(VoidCallbackWrapper(cb)), desc);
}

void Scene::addPreStepCallback(VoidCallback cb) {
    prestepCallbacks.push_back(cb);
}

void Scene::addPreDrawCallback(VoidCallback cb) {
    predrawCallbacks.push_back(cb);
}

void EventHandler::getTransformation(osg::Vec3d &eye, osg::Vec3d &center, osg::Vec3d &up) const {
    center = _center;
    eye    = _center + _rotation * osg::Vec3d(0., 0., _distance);
    up     = _rotation * osg::Vec3d(0., 1., 0.);
}

bool EventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
    bool suppressDefault = false;
    osgGA::GUIEventAdapter::EventType t = ea.getEventType();

    // keypress handlers (for convenience)
    if (t == osgGA::GUIEventAdapter::KEYDOWN) {
        pair<Scene::KeyCallbackMap::const_iterator, Scene::KeyCallbackMap::const_iterator> range =
            scene.keyCallbacks.equal_range(ea.getKey());
        for (Scene::KeyCallbackMap::const_iterator i = range.first; i != range.second; ++i)
            suppressDefault |= i->second(ea);
    }

    // general handlers
    pair<Scene::CallbackMap::const_iterator, Scene::CallbackMap::const_iterator> range =
        scene.callbacks.equal_range(t);
    for (Scene::CallbackMap::const_iterator i = range.first; i != range.second; ++i)
        suppressDefault |= i->second(ea);

    if (!suppressDefault)
        return osgGA::TrackballManipulator::handle(ea, aa);

    return false;
}
