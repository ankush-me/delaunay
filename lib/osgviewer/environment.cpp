#include "environment.h"

OSGInstance::OSGInstance(bool shadows) {
	if (!shadows) {
    root = new osg::Group;               // does not do shadow magic
	} else {
	osg::ref_ptr<osgShadow::ShadowedScene> shadow_root = new osgShadow::ShadowedScene;   // does do shadow magic
    shadow_root->setReceivesShadowTraversalMask(RECEIVES_SHADOW_MASK);
    shadow_root->setCastsShadowTraversalMask(CASTS_SHADOW_MASK);

    sm = new osgShadow::MinimalCullBoundsShadowMap;// ParallelSplitShadowMap;// DebugShadowMap;// ShadowMap;
    shadow_root->setShadowTechnique(sm.get());
    int mapres = 4*1024;
    sm->setTextureSize(osg::Vec2s(mapres,mapres));

    root = shadow_root;
	}
    osg::setNotifyLevel(osg::FATAL);
}


Environment::~Environment() {
    for (ConstraintList::iterator i = constraints.begin(); i != constraints.end(); ++i)
        (*i)->destroy();
    for (ObjectList::iterator i = objects.begin(); i != objects.end(); ++i)
        (*i)->destroy();
}

void Environment::add(EnvironmentObject::Ptr obj) {

    obj->setEnvironment(this);
    obj->init();
    objects.push_back(obj);

    if (obj->getOSGNode())  {//shadow magic : osg does not respect you or all the flags you set
    	obj->getOSGNode()->setNodeMask(CASTS_SHADOW_MASK);
    	if (obj->receiveShadow)
    		obj->getOSGNode()->setNodeMask(RECEIVES_SHADOW_MASK);
    	else
    		obj->getOSGNode()->setNodeMask(obj->getOSGNode()->getNodeMask() & ~RECEIVES_SHADOW_MASK);
    }

// objects are reponsible for adding themselves
// to the dynamics world and the osg root
}

void Environment::remove(EnvironmentObject::Ptr obj) {
    for (ObjectList::iterator i = objects.begin(); i != objects.end(); ++i) {
        if (obj == *i) {
            (*i)->destroy();
            objects.erase(i);
            return;
        }
    }
}


void Environment::step() {
    ObjectList::iterator i;
    for (i = objects.begin(); i != objects.end(); ++i)
        (*i)->preDraw();
}
