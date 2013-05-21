#include <osgVRPN/WalledModeHandler.h>
using namespace osgVRPN;

WalledModeHandler::WalledModeHandler( osg::ref_ptr<VRManipulator> manip )
{
	_manip = manip;
}

bool WalledModeHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& )
{
    if( ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN ) return false;

    switch( ea.getKey() )
    {
		case osgGA::GUIEventAdapter::KEY_A:
			_manip->setMode( VRManipulator::FRONT );
            return true;

		case osgGA::GUIEventAdapter::KEY_Z:
			_manip->setMode( VRManipulator::BACK );
            return true;

		case osgGA::GUIEventAdapter::KEY_J:
			_manip->setMode( VRManipulator::LEFT );
            return true;

		case osgGA::GUIEventAdapter::KEY_R:
			_manip->setMode( VRManipulator::RIGHT );
            return true;

		case osgGA::GUIEventAdapter::KEY_U:
			_manip->setMode( VRManipulator::TOP );
            return true;

		case osgGA::GUIEventAdapter::KEY_D:
			_manip->setMode( VRManipulator::BOTTOM );
            return true;

		case osgGA::GUIEventAdapter::KEY_K:
			_manip->setMode( VRManipulator::HMD );
			return true;

        default:  return false;
    }
}
