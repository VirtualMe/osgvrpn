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

#include <iostream>
#include <map>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Vec3>
#include <osg/PositionAttitudeTransform>
#include <osg/io_utils>
#include <osg/Texture2D>
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

typedef std::map<int, bool> ButtonStateMap;

static osg::Node* buildScene( void )
{
    osg::Group* scene( new osg::Group );
    osg::Geode* geode(0);

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,-2),100,100,0.1)));
    osg::Texture2D* tex0( new osg::Texture2D );
    tex0->setImage( osgDB::readImageFile( "Images/lz.rgb" ) );
    geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex0, osg::StateAttribute::ON );
    scene->addChild( geode );

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0),3,3,0.1)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(40,0,0),5)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0,40,0),2,9)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-40,0,0),3,7)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,-40,0),7,7,7)));
    scene->addChild( geode );

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0,0,0),1,2)));
    scene->addChild( geode );
 
    return scene;
}

static osgVRPN::TrackerTransform* buildWand( osg::ref_ptr<osgVRPN::VRTracker> tracker )
{
	osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
	osg::Cylinder *cylinder = new osg::Cylinder( osg::Vec3( 0, 15, 0 ), 2, 30 ) ;
	osg::ref_ptr<osg::ShapeDrawable> drawable( new osg::ShapeDrawable( cylinder ) );
	
	drawable->setColor( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
	cylinder->setRotation( osg::Quat( osg::DegreesToRadians( -90.0f ), osg::Vec3( 1, 0, 0 ) ) );
	geode->addDrawable( drawable );	

	osgVRPN::TrackerTransform* transform( new osgVRPN::TrackerTransform( tracker ) );
	transform->addChild( geode );

	return transform;
}

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

    osgViewer::Viewer viewer;
    viewer.getUsage( *args.getApplicationUsage() );
    if (args.read("-h") || args.read("--help"))
    {
        args.getApplicationUsage()->write( std::cout );
        return 1;
    }

	// INITIALIZE CAMERA //
    osg::StateSet* ss( viewer.getCamera()->getOrCreateStateSet() );
    viewer.addEventHandler( new osgGA::StateSetManipulator( ss ) );
    viewer.addEventHandler( new osgViewer::HelpHandler( args.getApplicationUsage() ) );
    viewer.addEventHandler( new osgViewer::ThreadingHandler );
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    viewer.addEventHandler( new osgViewer::StatsHandler );
    viewer.addEventHandler( new osgViewer::LODScaleHandler );
    viewer.addEventHandler( new osgViewer::ScreenCaptureHandler );

    osgGA::KeySwitchMatrixManipulator* ksm( new osgGA::KeySwitchMatrixManipulator );
    viewer.setCameraManipulator( ksm );

	// INITIALIZE DEVICES //
	osgVRPN::Analog* analog( new osgVRPN::Analog( "Wanda@localhost" ) );
	osgVRPN::Button* button( new osgVRPN::Button( "Wanda@localhost" ) );
	osgVRPN::VRTracker* tracker( new osgVRPN::VRTracker( "Tracker0@localhost", 1 ) );		// FoB Head Tracker ID
	osgVRPN::VRTracker* wanda( new osgVRPN::VRTracker( "Tracker0@localhost", 2 ) );			// FoB Wanda ID
    osgVRPN::VRManipulator* manip( new osgVRPN::VRManipulator( tracker ) );

	// Ascension Flock of Birds coordinate system is +Z Down, +Y Right, and +X Out of the screen.
	// The tracker in Fakespace V-Wand is positioned awkwardly, and requires different indices.
	int flock_rotations[] = { -2, 3, 1, 4 };
	int flock_positions[] = { 2, -3, -1 };
	int wanda_rotations[] = { -2, -1, 3, 4 };

	tracker->setQuatIndices( flock_rotations );
	tracker->setVecIndices( flock_positions );
	wanda->setQuatIndices( wanda_rotations );

	osgVRPN::DeviceBundle::Instance()->addAnalog( analog );
	osgVRPN::DeviceBundle::Instance()->addButton( button );
	osgVRPN::DeviceBundle::Instance()->addTracker( tracker );
	osgVRPN::DeviceBundle::Instance()->addTracker( wanda );
	osgVRPN::DeviceBundle::Instance()->addManipulator( manip );
	osgVRPN::DevicesCallback* devices( new osgVRPN::DevicesCallback( osgVRPN::DeviceBundle::Instance() ) );

	// CREATE SCENE //
	osg::Group* scene( new osg::Group );
	osg::Node*  world( new osg::Node );

    if( args.argc() > 1 )
    {
        world = osgDB::readNodeFiles(args);
    }
	else
	{
		world = buildScene();
	}
	scene->addChild( world );

	osgVRPN::TrackerTransform* wand( buildWand( wanda ) );
	scene->addChild( wand );
	scene->addUpdateCallback( devices );

	// SET HOME MATRIX AND ADD MANIPULATORS //
	const osg::BoundingSphere& bs( world->getBound() );
    manip->setHomeMatrix( osg::Matrix::translate(0,0,1.5*bs._radius) * osg::Matrixd::rotate(1.2,1,0,0) );
	manip->setAutoComputeHomePosition( false );
    ksm->addMatrixManipulator( '1', "Tracker", manip );
    ksm->addMatrixManipulator( '2', "Trackball", new osgGA::TrackballManipulator() );

    osgUtil::Optimizer optimizer;
    optimizer.optimize( scene );
    viewer.setSceneData( scene );
	viewer.setLightingMode( osg::View::SKY_LIGHT );

	// MAIN LOOP //
	int numAnalogs = 0;
	float analogVal = 0.0f;
	int numButtons = 0;
	ButtonStateMap prevStates;

	osg::Quat rotation( 0, 0, 0, 1 );

    while( !viewer.done() )
    {
		// For some reason, VRPN does not report button or analog devices until 
		// AFTER they have been used, with the number being the highest ID of 
		// those used thus far.  The program goes bonkers trying to do anything 
		// with analogs before these devices have actually reported, hence the 
		// checks.

		if ( numButtons >= 3 )
		{
			if ( osgVRPN::DeviceBundle::Instance()->getButtonState( 0, 0 ) && !prevStates[0] )
			{
				manip->navRotate( osg::Quat( osg::DegreesToRadians( -5.0f ), osg::Vec3( 0, 0, 1 ) ) );
				prevStates[0] = true;
			}
			else if ( ! osgVRPN::DeviceBundle::Instance()->getButtonState( 0, 0 ) )
			{
				prevStates[0] = false;
			}

			if ( osgVRPN::DeviceBundle::Instance()->getButtonState( 0, 2 ) && !prevStates[2] )
			{
				manip->navRotate( osg::Quat( osg::DegreesToRadians( 5.0f ), osg::Vec3( 0, 0, 1 ) ) );
				prevStates[2] = true;
			}
			else if ( ! osgVRPN::DeviceBundle::Instance()->getButtonState( 0, 2 ) )
			{
				prevStates[2] = false;
			}
		}

		if ( numAnalogs >= 2 )
		{	
			// Allow for a certain tolerance in moving the joystick.
			analogVal = osgVRPN::DeviceBundle::Instance()->getValue( 0, 1 );

			if ( analogVal > 0.2f || analogVal < -0.2f )
			{
				// Move in the direction of the wand based on the value of the up/down
				// channel.  The forward vector in osgViewer::Viewer is negative z.
				// Multiplying the rotation of the tracker in the navigated space
				// by this vector gives the vector of the wand.

				osg::Vec3 forward( 0, 1, 0 );

				// The Position Attitude Transform does not tell the tracker rotation has occured.
				rotation = wanda->getMatrix().getRotate() * osg::Quat( osg::DegreesToRadians( -90.0f ), osg::Vec3( 1, 0, 0 ) );
				forward = rotation * forward;
				forward *= analogVal;

				manip->navTranslate( forward );
			}
		}

		// Continue to poll VRPN for the number of devices; this number 
		// changes as more report in.
		numAnalogs = osgVRPN::DeviceBundle::Instance()->getNumChannels( 0 );
		numButtons = osgVRPN::DeviceBundle::Instance()->getNumButtons( 0 );

        viewer.frame();
	}

    return 0;
}