/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/TerrainManipulator>
#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/VRTracker.h>
#include <osgVRPN/AnalogTracker.h>
#include <osgVRPN/TrackerTransform.h>
#include <osgVRPN/VRManipulator.h>
#include <osgVRPN/DeviceBundle.h>
#include <osgVRPN/DevicesCallback.h>
#include <osgVRPN/WalledModeHandler.h>

int main( int argc, char* argv[] )
{
	// ARGUMENT PARSING //
    osg::ArgumentParser args( &argc, argv );
    args.getApplicationUsage()->setApplicationName( args.getApplicationName() );
    args.getApplicationUsage()->setDescription(args.getApplicationName()+" demonstrates osgVRPN");
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()+" [options]");
    args.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
    args.getApplicationUsage()->addKeyboardMouseBinding( "1", "Trackball manipulator" );
    args.getApplicationUsage()->addKeyboardMouseBinding( "2", "Tracker manipulator" );

	// INITIALIZE TRACKER //
	osgVRPN::Analog* analog( new osgVRPN::Analog( "Wanda@localhost" ) );
	osgVRPN::Button* button( new osgVRPN::Button( "Wanda@localhost" ) );
	osgVRPN::VRTracker* tracker( new osgVRPN::VRTracker( "Tracker0@localhost", 1 ) );	// ID for Flock HMD
    osgVRPN::VRManipulator* manip( new osgVRPN::VRManipulator( tracker ) );
	manip->setAutoComputeHomePosition( false );

	osgVRPN::DeviceBundle::Instance()->addAnalog( analog );
	osgVRPN::DeviceBundle::Instance()->addButton( button );
	osgVRPN::DeviceBundle::Instance()->addTracker( tracker );
	osgVRPN::DeviceBundle::Instance()->addManipulator( manip );
	osgVRPN::DevicesCallback* devices( new osgVRPN::DevicesCallback( osgVRPN::DeviceBundle::Instance() ) );

	// INITIALIZE CAMERA //
    osgViewer::Viewer viewer;
    viewer.getUsage( *args.getApplicationUsage() );
    if (args.read("-h") || args.read("--help"))
    {
        args.getApplicationUsage()->write( std::cout );
        return 1;
    }

    osg::StateSet* ss( viewer.getCamera()->getOrCreateStateSet() );
    viewer.addEventHandler( new osgGA::StateSetManipulator( ss ) );
    viewer.addEventHandler( new osgViewer::HelpHandler( args.getApplicationUsage() ) );
    viewer.addEventHandler( new osgViewer::ThreadingHandler );
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    viewer.addEventHandler( new osgViewer::StatsHandler );
    viewer.addEventHandler( new osgViewer::LODScaleHandler );
    viewer.addEventHandler( new osgViewer::ScreenCaptureHandler );
	viewer.addEventHandler( new osgVRPN::WalledModeHandler( manip ) );

    osgGA::KeySwitchMatrixManipulator* ksm( new osgGA::KeySwitchMatrixManipulator );
    viewer.setCameraManipulator( ksm );

	// CREATE SCENE //
	osg::Group* scene( new osg::Group );
	osg::Node*  world( new osg::Node );

    if( args.argc() > 1 )
    {
        world = osgDB::readNodeFiles(args);
    }
	else
	{
		world = osgDB::readNodeFile( "Images/stadium.ive.[-90,180,0].rot.[-1500,-250,4200].trans" );
	}
	scene->addChild( world );
	scene->addUpdateCallback( devices );

	// SET HOME MATRIX AND ADD MANIPULATORS //
    ksm->addMatrixManipulator( '1', "Tracker", manip );
    ksm->addMatrixManipulator( '2', "Trackball", new osgGA::TrackballManipulator() );

	// Increase the field of view so we can see a good portion of the stadium.
	double fovy, aspectRatio, zNear, zFar;
	osg::ref_ptr<osg::Camera> camera = viewer.getCamera();

	camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
	camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	fovy *= 2.5;  /*aspectRatio *= 2;*/  zNear *= 5;  zFar *= 5;
	camera->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);

	// Set the model.  Lighting is terrible, but osg::View::NO_LIGHT 
	// seems to work.
    osgUtil::Optimizer optimizer;
    optimizer.optimize( scene );
    viewer.setSceneData( scene );
	viewer.setLightingMode( osg::View::NO_LIGHT );

	// VRManipulator navigation functions have Y up.
	viewer.frame(); 
	osg::Vec3 eye( 0, 0, -1 );
	osg::Vec3 center( 0, 0, 0 );
	osg::Vec3 up( 0, 1, 0 );
	osg::Matrix matrix;
	
	matrix.makeLookAt( eye, center, up );
	manip->setHomeMatrix( matrix );

	// MAIN LOOP //
	unsigned int numAnalogs = 0;
	float analogVal = 0.0f;

	while(!viewer.done())
	{
		if ( numAnalogs >= 1 )
		{	
			// Allow for a certain tolerance in moving the joystick.
			analogVal = osgVRPN::DeviceBundle::Instance()->getValue( 0, 0 );

			if ( analogVal > 0.2f || analogVal < -0.2f )
			{
				manip->navRotate( osg::DegreesToRadians( -analogVal ), osg::Vec3( 0, 1, 0 ) );
			}
		}
		
		numAnalogs = osgVRPN::DeviceBundle::Instance()->getNumChannels( 0 );

		viewer.frame();
	}

	return 0;	

} 