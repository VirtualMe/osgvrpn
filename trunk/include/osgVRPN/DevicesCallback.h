#ifndef OSGVRPN_DEVICESCALLBACK
#define OSGVRPN_DEVICESCALLBACK 1

#pragma warning ( disable : 4018 )

#include <osg/ref_ptr>
#include <osgVRPN/Analog.h>
#include <osgVRPN/Button.h>
#include <osgVRPN/DeviceBundle.h>
#include <osgVRPN/Tracker.h>

namespace osgVRPN
{
	class DevicesCallback : public osg::NodeCallback
	{
		public:
			DevicesCallback( osgVRPN::DeviceBundle* bundle ) { _bundle = bundle; }
			~DevicesCallback() { delete _bundle; }

			inline virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
			{
				for ( int i = 0; i < _bundle->getAnalogs().size(); ++i )
				{
					if ( _bundle->getAnalog( i ).valid() )
						_bundle->getAnalog( i )->update();
				}

				for ( int i = 0; i < _bundle->getButtons().size(); ++i )
				{
					if ( _bundle->getButton( i ).valid() )
						_bundle->getButton( i )->update();
				}

				for ( int i = 0; i < _bundle->getTrackers().size(); ++i )
				{
					if ( _bundle->getTracker( i ).valid() )
						_bundle->getTracker( i )->update();
				}

				traverse(node, nv);
			}

		protected:
			osg::ref_ptr<osgVRPN::DeviceBundle> _bundle;
	};
}		// end namespace osgVRPN

#endif //OSGVRPN_DEVICESCALLBACK