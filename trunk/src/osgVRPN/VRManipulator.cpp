#include <osgVRPN/VRManipulator.h>
using namespace osgVRPN;

VRManipulator::VRManipulator ( osg::ref_ptr<TrackerBase> tracker )
{
	_tracker = tracker;
	_navMatrix.makeIdentity();
	_mode = HMD;
}

void VRManipulator::setMode( const VRManipulator::Mode mode )
{
	osg::Matrix reset;

	switch ( mode )
	{
		case FRONT: 
		case HMD:
			reset.makeLookAt( osg::Vec3( 0, 0, -1 ), osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 1, 0 ) );
			break;

		case BACK:
			reset.makeLookAt( osg::Vec3( 0, 0, 1 ), osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 1, 0 ) );
			break;

		case LEFT:
			reset.makeLookAt( osg::Vec3( 1, 0, 0 ), osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 1, 0 ) );
			break;

		case RIGHT:
			reset.makeLookAt( osg::Vec3( -1, 0, 0 ), osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 1, 0 ) );
			break;

		case TOP:
			reset.makeLookAt( osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 1, 0 ), osg::Vec3( 0, 0, -1 ) );
			break;

		case BOTTOM:
			reset.makeLookAt( osg::Vec3( 0, 1, 0 ), osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 0, 1 ) );
			break;

	default:
		return;
	}

	setHomeMatrix( reset );
	_mode = mode;
}

VRManipulator::Mode VRManipulator::getMode() const
{
	return _mode;
}

osg::Matrix VRManipulator::getMatrix() const
{
	if ( _tracker.valid() )
	{
		return _navMatrix * _tracker->getMatrix() * _homeMatrix;
	}
	else
	{
		return _homeMatrix;
	}
}

osg::Matrix VRManipulator::getInverseMatrix() const
{
	if ( _tracker.valid() )
	{
		if(_mode==HMD) 
		{
			return osg::Matrix::inverse( _homeMatrix ) * 
				_tracker->getInverseMatrix() * 
				osg::Matrix::inverse( _navMatrix );
		}
		else
		{
			osg::Matrixd trackermat = _tracker->getMatrix();
			osg::Vec3d trans, scale;
			osg::Quat  rot, so;

			trackermat.decompose(trans, rot, scale, so);
			trackermat.makeTranslate(trans);
			trackermat.scale(scale);

			return osg::Matrix::inverse( _homeMatrix ) * 
				osg::Matrixd::inverse( trackermat ) * 
				osg::Matrix::inverse( _navMatrix );
		}
	}
	else
	{
		return osg::Matrix::inverse( _homeMatrix );
	}
}

void VRManipulator::setNavMatrix( const osg::Matrix& navMatrix )
{
	_navMatrix = navMatrix;
}

osg::Matrix VRManipulator::getNavMatrix()
{
	return _navMatrix * _homeMatrix;
}

void VRManipulator::navTranslate( const osg::Vec3& vec )
{
	osg::Matrix transMatrix;
	transMatrix.makeTranslate( vec );
	setNavMatrix( transMatrix * _navMatrix );
}

void VRManipulator::navTranslate( float x, float y, float z )
{
	osg::Matrix transMatrix;
	transMatrix.makeTranslate( x, y, z );
	setNavMatrix( transMatrix * _navMatrix );
}

void VRManipulator::navRotate( const osg::Quat& quat )
{
	osg::Matrix rotMatrix;
	rotMatrix.makeRotate( quat );
	setNavMatrix( rotMatrix * _navMatrix );
}

void VRManipulator::navRotate( float angle, const osg::Vec3& axis )
{
	osg::Matrix rotMatrix;
	rotMatrix.makeRotate( angle, axis );
	setNavMatrix( rotMatrix * _navMatrix );
}

void VRManipulator::navMakeIdentity()
{
	_navMatrix.makeIdentity();
}

void VRManipulator::homeMakeIdentity()
{
	_homeMatrix.makeIdentity();
}