#ifndef WALLEDMODE_HANDLER
#define WALLEDMODE_HANDLER 1

#include <osgGA/GUIEventHandler>
#include <osgVRPN/VRManipulator.h> 

namespace osgVRPN
{
	class OSGVRPN_EXPORT WalledModeHandler : public osgGA::GUIEventHandler
	{
		public:
			WalledModeHandler( osg::ref_ptr<VRManipulator> manip );			
			bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& );

		private:
			osg::ref_ptr<VRManipulator> _manip;
			WalledModeHandler();		// disallowed
	};
} // end namespace osgVRPN

#endif //WALLEDMODE_HANDLER