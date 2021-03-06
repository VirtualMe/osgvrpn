/* file:        src/osgVRPN/Tracker.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2007 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Tracker.cpp 607 2011-06-24 04:54:07Z mew $
*/

#include <osgVRPN/Tracker.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Tracker.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Tracker::Tracker( const char* deviceName, int sensorID ) :
        _vrpnTracker( new vrpn_Tracker_Remote(deviceName) ),
        _sensorID( sensorID )
{
    _vrpnTracker->register_change_handler( this, s_ChangeHandler );
}

Tracker::~Tracker()
{
    _vrpnTracker->unregister_change_handler( this, s_ChangeHandler );
    delete _vrpnTracker;
}

///////////////////////////////////////////////////////////////////////////

const osg::Matrixd Tracker::getMatrix() const
{
    return osg::Matrixd::rotate( _rotation ) *
            osg::Matrixd::translate( _position );
}

const osg::Matrixd Tracker::getInverseMatrix() const
{
    return osg::Matrixd::translate( -_position ) *
            osg::Matrixd::rotate( _rotation.inverse() );
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the s_ChangeHandler() callback.

void Tracker::update()
{
    if( _enabled ) _vrpnTracker->mainloop();
}

/*static*/ void Tracker::s_ChangeHandler( void* userdata, const vrpn_TRACKERCB info )
{
    static_cast<Tracker*>(userdata)->changeHandler( info );
}

void Tracker::changeHandler( const vrpn_TRACKERCB& info )
{
    if( (_sensorID == -1) || (_sensorID == info.sensor) )
    {
        _position[0] = _transScale[0] * info.pos[0];
        _position[1] = _transScale[1] * info.pos[1];
        _position[2] = _transScale[2] * info.pos[2];
        _rotation.set( info.quat[0], info.quat[1], info.quat[2], info.quat[3] );
    }

    ++_eventCounter;
}

// vim: set sw=4 ts=8 et ic ai:
