/* file:        include/osgVRPN/TrackerTransform.h
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2011 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerTransform.h 605 2011-06-24 04:51:38Z mew $
*/

#ifndef OSG_TRACKERTRANSFORM
#define OSG_TRACKERTRANSFORM 1

#include <osg/MatrixTransform>
#include <osgVRPN/TrackerBase.h>

#include <osgVRPN/Export.h>

namespace osgVRPN {

/** a Transform node controlled by a osgVRPN::TrackerBase.
It can be used to control the position of a node in the scene.
*/

class OSGVRPN_EXPORT TrackerTransform : public osg::MatrixTransform
{
public :
    TrackerTransform( osg::ref_ptr<TrackerBase> tracker = 0 );

    TrackerTransform( const TrackerTransform& xform,
            const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY ) :
                MatrixTransform( xform, copyop ),
                _tracker( xform._tracker )
    {}

    //FUTURE META_Node( osgVRPN, TrackerTransform );

    /** Set/get the tracker that controls this transform, 0==none */
    void setTracker( osg::ref_ptr<TrackerBase> tracker ) { _tracker = tracker; }
    osg::ref_ptr<TrackerBase> getTracker() const { return _tracker; }

protected:      // methods
    ~TrackerTransform() {}

protected:      // data
    osg::ref_ptr<TrackerBase> _tracker;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
