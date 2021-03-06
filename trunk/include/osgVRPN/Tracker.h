/* file:        include/osgVRPN/Tracker.h
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2011 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Tracker.h 605 2011-06-24 04:51:38Z mew $
*/

#ifndef OSGVRPN_TRACKER
#define OSGVRPN_TRACKER 1

#include <osg/Quat>
#include <osg/Vec3>
#include <osg/Matrixd>
#include <osgVRPN/TrackerBase.h>

#include <osgVRPN/Export.h>

#include <vrpn_Configure.h>
// fwd declarations to reduce need for VRPN headers
class vrpn_Tracker_Remote;
typedef struct _vrpn_TRACKERCB vrpn_TRACKERCB;

namespace osgVRPN {

/** an OSG wrapper for VRPN's vrpn_Tracker_Remote class. */

class OSGVRPN_EXPORT Tracker : public TrackerBase
{
public:
    Tracker( const char* deviceName, int sensorID = -1 );
    virtual ~Tracker();

    /** Update our state from the device. */
    virtual void update();

    /** Query the tracker's transform matrix */
    const osg::Matrixd getMatrix() const;
    const osg::Matrixd getInverseMatrix() const;

protected:      // methods
    static void VRPN_CALLBACK s_ChangeHandler( void* userdata, const vrpn_TRACKERCB info );
    virtual void changeHandler( const vrpn_TRACKERCB& info );

protected:      // data
    vrpn_Tracker_Remote* const  _vrpnTracker;
    osg::Vec3 _position;
    osg::Quat _rotation;
    const int _sensorID;

private:        // uncopyable
    Tracker(const Tracker&);
    Tracker& operator=(const Tracker&);
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
