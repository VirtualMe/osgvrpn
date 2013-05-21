#include <osgVRPN/VRTracker.h>
#include <iostream>
#define VRPN_CLIENT_ONLY
#include "vrpn_Tracker.h"
using namespace osgVRPN;

VRTracker::VRTracker( const char* deviceName, int sensorID ) : Tracker(deviceName,sensorID)
{	// default pos, orientations to identity
	_tracker2roomOri=osg::Quat(0,0,0,1);
	_tracker2roomPos=osg::Vec3d(0,0,0);
	_unit2sensorOri=osg::Quat(0,0,0,1);
	_unit2sensorPos=osg::Vec3d(0,0,0);
	// base constructor sets up the change handler, we set up coordinate xform handler
	// T2R is for all sensors on a device, U2S could be per sensor
	_vrpnTracker->register_change_handler( this, s_T2RChangeHandler );
	_vrpnTracker->register_change_handler( this, s_U2SChangeHandler, sensorID );
	// now that handlers are set up, request transforms from the sensor
	_vrpnTracker->request_t2r_xform();
	_vrpnTracker->request_u2s_xform();
	for( int i = 0; i < 4; ++i ) _quatIndices[i] = i + 1; // these are 1-indexed values
	for ( int i = 0; i < 3; ++i ) _vecIndices[i] = i + 1;
}

VRTracker::~VRTracker(void)
{
	_vrpnTracker->unregister_change_handler( this, s_ChangeHandler );
	_vrpnTracker->unregister_change_handler( this, s_T2RChangeHandler );
	_vrpnTracker->unregister_change_handler( this, s_U2SChangeHandler );
	delete _vrpnTracker;
}

///////////////////////////////////////////////////////////////////////////

const osg::Matrixd VRTracker::getMatrix() const
{
	// Done in getMatrix instead of on the change handler - although
	// if both are being called by the same loop, it doesn't really matter
	osg::Vec3d pos;
	osg::Quat q;
    {	// offsets are combination of t2r and u2s
		osg::Vec3d tr = _unit2sensorPos + _rotation * _tracker2roomOri * _tracker2roomPos;
        pos[0] = _transScale[0] * (_position[0]+tr[0]);
        pos[1] = _transScale[1] * (_position[1]+tr[1]);
        pos[2] = _transScale[2] * (_position[2]+tr[2]);
		q = _unit2sensorOri * _rotation * _tracker2roomOri;
	}
	return osg::Matrixd::rotate( q ) *
            osg::Matrixd::translate( pos );
}

const osg::Matrixd VRTracker::getInverseMatrix() const
{
	// Done in getMatrix instead of on the change handler - although
	// if both are being called by the same loop, it doesn't really matter
	osg::Vec3d pos;
	osg::Quat q;
    {	// offsets are combination of t2r and u2s
		osg::Vec3d tr = _unit2sensorPos + _rotation * _tracker2roomOri * _tracker2roomPos;
        pos[0] = _transScale[0] * (_position[0]+tr[0]);
        pos[1] = _transScale[1] * (_position[1]+tr[1]);
        pos[2] = _transScale[2] * (_position[2]+tr[2]);
		q =  _unit2sensorOri * _rotation * _tracker2roomOri ;
	}
    return osg::Matrixd::translate( -pos ) *
            osg::Matrixd::rotate( q.inverse() );
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// Tracker::update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the s_ChangeHandler() callback.


// static methods are C++ class methods
/*static*/ void VRTracker::s_T2RChangeHandler( void* userdata, const vrpn_TRACKERTRACKER2ROOMCB info )
{
	static_cast<VRTracker*>(userdata)->t2rchangeHandler( info );
}
/*static*/ void VRTracker::s_U2SChangeHandler( void* userdata, const vrpn_TRACKERUNIT2SENSORCB info )
{
	static_cast<VRTracker*>(userdata)->u2schangeHandler( info );
}

void VRTracker::t2rchangeHandler( const vrpn_TRACKERTRACKER2ROOMCB& info )
{
	// I am reading these as if "Tracker To Room" is the pre-transform
	// and "Unit 2 Sensor" is the post transform - I haven't found any good
	// examples that use either of these, but it seems reasonable to do it
	// this way.
	for(int i=0;i<3;i++)
	{
		_tracker2roomPos[i] = info.tracker2room[i];
	}
	osg::Vec4d qvals;
	for(int i=0;i<4;i++)
	{
		qvals[i]=info.tracker2room_quat[i];
	}
	_tracker2roomOri.set(qvals);
}

void VRTracker::u2schangeHandler(const vrpn_TRACKERUNIT2SENSORCB &info)
{
	// I am reading these as if "Tracker To Room" is the pre-transform
	// and "Unit 2 Sensor" is the post transform - I haven't found any good
	// examples that use either of these, but it seems reasonable to do it
	// this way.
	for(int i=0;i<3;i++)
	{
		_unit2sensorPos[i] = info.unit2sensor[i];
	}
	_unit2sensorOri.set(info.unit2sensor_quat[0],
		info.unit2sensor_quat[1],
		info.unit2sensor_quat[2],
		info.unit2sensor_quat[3]);
}

void VRTracker::setQuatIndices(int indices[4])
{
	for(int i=0;i<4;i++) _quatIndices[i]=indices[i];
}

void VRTracker::setVecIndices( int indices[3] )
{
	for ( int i = 0; i < 3; ++i ) _vecIndices[i] = indices[i];
}

void VRTracker::changeHandler( const vrpn_TRACKERCB& info ) 
{
	Tracker::changeHandler(info);
	if(_sensorID==-1 || _sensorID==info.sensor)
	{
		// _position is set in parent class change handler
		osg::Vec3d vvals;
		for ( int i = 0; i < 3; ++i )
		{
			if ( _vecIndices[i] < 0 )
				vvals[i] = -_position[-(_vecIndices[i]) - 1];
			else
				vvals[i] = _position[(_vecIndices[i]) - 1];
		}
		_position.set( vvals );

		// _rotation is set in parent class change handler
		osg::Vec4d qvals;
		for(int i=0;i<4;i++)
		{
			if( _quatIndices[i] < 0 ) // use 1-indexed values so we can use negative
				qvals[i] = -_rotation[-(_quatIndices[i]) - 1];
			else
				qvals[i] = _rotation[(_quatIndices[i])-1];
		}
		_rotation.set(qvals);
	}
}
