An application demonstrating additions to the osgVRPN plugin.

Navigation through the scene is abstracted out in the osgVRPN::VRTracker 
and osgVRPN::VRManipulator classes.  A wand is created and attached to an
osgVRPN::TrackerTransform.  Wand buttons are used to rotate around the 
scene and the joystick will navigate through the scene in the direction the
wand is pointing.  A regular tracker is used to control the camera.

This app does not have multiple configurations as the original did; a wand 
is required for all of the new functionality as shown.

Original plugin created by Mike Weiblen.
http://mew.cx/osg/