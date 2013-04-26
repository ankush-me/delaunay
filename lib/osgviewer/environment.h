#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_



#include <vector>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/StandardShadowMap>


#define RECEIVES_SHADOW_MASK 0x1
#define CASTS_SHADOW_MASK    0x2


using namespace std;

struct OSGInstance {
    typedef boost::shared_ptr<OSGInstance> Ptr;
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osgShadow::MinimalCullBoundsShadowMap> sm;

    OSGInstance();
};


struct Environment;

class EnvironmentObject {
private:
    Environment *env;

public:
    typedef boost::shared_ptr<EnvironmentObject> Ptr;

    bool receiveShadow;

    EnvironmentObject() { }
    EnvironmentObject(Environment *env_) : env(env_) , receiveShadow(false) { }
    virtual ~EnvironmentObject() { }

    Environment *getEnvironment() { return env; }

    // methods only to be called by the Environment
    void setEnvironment(Environment *env_) { env = env_; }
    virtual void init() { }
    virtual void preDraw() { }
    virtual void destroy() { }

    virtual osg::Node *getOSGNode() const { return NULL; }

    virtual void setColor(float r, float g, float b, float a) {};
    virtual void adjustTransparency(float increment) {};
};


struct Environment {
    typedef boost::shared_ptr<Environment> Ptr;

    OSGInstance::Ptr osg;

    typedef std::vector<EnvironmentObject::Ptr> ObjectList;
    ObjectList objects;

    typedef std::vector<EnvironmentObject::Ptr> ConstraintList;
    ConstraintList constraints;

    Environment(OSGInstance::Ptr osg_) : osg(osg_) { }
    ~Environment();

    void add(EnvironmentObject::Ptr obj);
    void remove(EnvironmentObject::Ptr obj);

    void step();
};


// Not a real object; just wraps a bunch of child objects.
template<class ChildType>
class CompoundObject : public EnvironmentObject {
protected:
    typedef std::vector<typename ChildType::Ptr> ChildVector;

public:
    typedef boost::shared_ptr<CompoundObject<ChildType> > Ptr;
    ChildVector children;
    ChildVector &getChildren() { return children; }

    CompoundObject() { }


    void init() {
        typename ChildVector::iterator i;
        for (i = children.begin(); i != children.end(); ++i) {
            if (*i) {
                (*i)->setEnvironment(getEnvironment());
                (*i)->init();
            }
        }
    }

    void preDraw() {
        typename ChildVector::iterator i;
        for (i = children.begin(); i != children.end(); ++i)
            if (*i)
                (*i)->preDraw();
    }

    void destroy() {
        typename ChildVector::iterator i;
        for (i = children.begin(); i != children.end(); ++i)
            if (*i)
                (*i)->destroy();
    }

    void setColor(float r, float g, float b, float a) {
			typename ChildVector::iterator i;
			for (i = children.begin(); i != children.end(); ++i)
				if (*i)
					(*i)->setColor(r,g,b,a);
    }

    void adjustTransparency(float increment) {
    	typename ChildVector::iterator i;
    	for (i = children.begin(); i != children.end(); ++i)
    		if (*i)
    			(*i)->adjustTransparency(increment);
    }
};


#endif // _ENVIRONMENT_H_
