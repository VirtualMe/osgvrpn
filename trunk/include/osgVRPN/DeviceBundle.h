#ifndef OSGVRPN_DEVICEBUNDLE
#define OSGVRPN_DEVICEBUNDLE 1

#include <osg/ref_ptr>
#include <osgVRPN/Analog.h>
#include <osgVRPN/Button.h>
#include <osgVRPN/Tracker.h>
#include <osgVRPN/TrackerManipulator.h>

/* Singleton class to keep track of all devices. */
namespace osgVRPN
{
	class OSGVRPN_EXPORT DeviceBundle : public osg::Referenced
	{
		public:
			static DeviceBundle* Instance();
			~DeviceBundle();
			
			// Mutator functions; add one device at a time using std::vector's push_back().
			void addAnalog( const osg::ref_ptr<osgVRPN::Analog> analog ); 
			void addButton( const osg::ref_ptr<osgVRPN::Button> button );
			void addTracker( const osg::ref_ptr<osgVRPN::Tracker> tracker );
			void addManipulator( const osg::ref_ptr<osgVRPN::TrackerManipulator> manipulator);

			// Accessor functions; return entire std::vector of device types.
			std::vector< osg::ref_ptr<osgVRPN::Analog> >  getAnalogs() const;
			std::vector< osg::ref_ptr<osgVRPN::Button> >  getButtons() const;
			std::vector< osg::ref_ptr<osgVRPN::Tracker> > getTrackers() const;

			// Accessor functions; return one device in the std::vector.
			osg::ref_ptr<osgVRPN::Analog>  getAnalog( unsigned int deviceID ) const;
			osg::ref_ptr<osgVRPN::Button>  getButton( unsigned int deviceID ) const;
			osg::ref_ptr<osgVRPN::Tracker> getTracker( unsigned int deviceID ) const;
			osg::ref_ptr<osgVRPN::TrackerManipulator> getManipulator () const;

			// Easier access to osgVRPN::Button and osgVRPN::Analog interfaces.
			unsigned int getNumButtons( unsigned int deviceID ) const;
			bool getButtonState( unsigned int deviceID, unsigned int buttonID) const;
			unsigned int getNumChannels( unsigned int deviceID ) const;
			float getValue( unsigned int deviceID, unsigned int channelID ) const;

		protected:		// data
			std::vector< osg::ref_ptr<osgVRPN::Analog> >  _analogs;
			std::vector< osg::ref_ptr<osgVRPN::Button> >  _buttons;
			std::vector< osg::ref_ptr<osgVRPN::Tracker> > _trackers;
			osg::ref_ptr<osgVRPN::TrackerManipulator> _manipulator;
		protected:		// methods
			DeviceBundle() {}

		private:
			static DeviceBundle* _instance;
	};
} // end namespace

#endif // OSGVRPN_DEVICEBUNDLE