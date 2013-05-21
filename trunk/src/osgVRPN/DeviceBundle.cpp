#include <osgVRPN/DeviceBundle.h>
using namespace osgVRPN;

DeviceBundle* DeviceBundle::_instance = NULL;
DeviceBundle* DeviceBundle::Instance()
{
	if ( _instance == NULL ) _instance = new DeviceBundle;
	return _instance;
}
DeviceBundle::~DeviceBundle()
{
	delete _instance;
	_instance = NULL;
}

void DeviceBundle::addAnalog( const osg::ref_ptr<Analog> analog )
{
	_analogs.push_back( analog );
}

void DeviceBundle::addButton( const osg::ref_ptr<Button> button )
{
	_buttons.push_back( button );
}

void DeviceBundle::addTracker( const osg::ref_ptr<Tracker> tracker )
{
	_trackers.push_back( tracker );
}

void DeviceBundle::addManipulator( const osg::ref_ptr<TrackerManipulator> manipulator )
{
	_manipulator = manipulator;
}

std::vector< osg::ref_ptr<Analog> > DeviceBundle::getAnalogs() const
{
	return _analogs;
}

std::vector< osg::ref_ptr<Button> > DeviceBundle::getButtons() const
{
	return _buttons;
}

std::vector< osg::ref_ptr<Tracker> > DeviceBundle::getTrackers() const
{
	return _trackers;
}

osg::ref_ptr<Analog> DeviceBundle::getAnalog( unsigned int deviceID ) const
{
	if ( deviceID > _analogs.size() )  return NULL;
	return _analogs[deviceID];
}

osg::ref_ptr<Button> DeviceBundle::getButton( unsigned int deviceID ) const
{
	if ( deviceID > _buttons.size() )  return NULL;
	return _buttons[deviceID];
}

osg::ref_ptr<Tracker> DeviceBundle::getTracker( unsigned int deviceID ) const
{
	if ( deviceID > _trackers.size() )  return NULL;
	return _trackers[deviceID];
}

osg::ref_ptr<TrackerManipulator> DeviceBundle::getManipulator() const
{
	return _manipulator;
}

unsigned int DeviceBundle::getNumButtons( unsigned int deviceID ) const
{
	if ( deviceID > _buttons.size() )  return 0;
	return _buttons[deviceID]->getNumButtons();
}

bool DeviceBundle::getButtonState( unsigned int deviceID, unsigned int buttonID ) const
{
	if ( deviceID > _buttons.size() )  return false;
	return _buttons[deviceID]->getButtonState( buttonID );
}

unsigned int DeviceBundle::getNumChannels( unsigned int deviceID ) const
{
	if ( deviceID > _analogs.size() )  return 0;
	return _analogs[deviceID]->getNumChannels();
}

float DeviceBundle::getValue( unsigned int deviceID, unsigned int channelID ) const
{
	if ( deviceID > _analogs.size() )  return 0.0f;
	return _analogs[deviceID]->getValue( channelID );
}