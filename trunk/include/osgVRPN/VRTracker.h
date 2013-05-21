#pragma once
#include <osgVRPN/Tracker.h>
#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/Export.h>
#include <vrpn_Configure.h>

#include <string> // STL strings
// fwd declarations to reduce need for VRPN headers
class vrpn_Tracker_Remote;
typedef struct _vrpn_TRACKERTRACKER2ROOMCB vrpn_TRACKERTRACKER2ROOMCB;
typedef struct _vrpn_TRACKERUNIT2SENSORCB vrpn_TRACKERUNIT2SENSORCB;

namespace osgVRPN {

	class OSGVRPN_EXPORT VRTracker :	public Tracker
	{
	public:
		VRTracker( const char* deviceName, int sensorID = -1 );
		~VRTracker(void);	

		/** Get the tracker's transform matrix */
		const osg::Matrixd getMatrix() const;
		const osg::Matrixd getInverseMatrix() const;

	    static void VRPN_CALLBACK s_T2RChangeHandler( void* userdata, const vrpn_TRACKERTRACKER2ROOMCB info );
		static void VRPN_CALLBACK s_U2SChangeHandler( void* userdata, const vrpn_TRACKERUNIT2SENSORCB info);
		virtual void changeHandler( const vrpn_TRACKERCB& info );
		void t2rchangeHandler( const vrpn_TRACKERTRACKER2ROOMCB& info );
		void u2schangeHandler( const vrpn_TRACKERUNIT2SENSORCB& info );

		/** VRPN's coordinate system does not match all compatible devices.		 *
		  * These methods will set the order this class will look up orientation *
		  * information in the VRPN structs.									 */
		void setQuatIndices( int indices[4] );
		void setVecIndices( int indices[3] );
	protected:
		//virtual void changeHandler(const vrpn_TRACKERCB &info);
		// tracker to room and unit to sensor, treat as pre and post transforms
		osg::Vec3d _tracker2roomPos;
		osg::Quat  _tracker2roomOri;
		osg::Vec3d _unit2sensorPos;
		osg::Quat _unit2sensorOri;

		int _quatIndices[4];
		int _vecIndices[3];
	};

}