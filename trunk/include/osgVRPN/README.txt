osgVRPN/README.txt - Mike Weiblen mew@mew.cx
$Id: README.txt 605 2011-06-24 04:51:38Z mew $
$URL: https://osgtoy.svn.sourceforge.net/svnroot/osgtoy/osgvrpn/trunk/include/osgVRPN/README.txt $

2011-06-22 : updated for VRPN 07_29

A library to integrate the Virtual Reality Peripheral Network with the
OpenSceneGraph 3D graphics system.  VRPN is a library (with sample client
and server applications) to provide network-transparent access to a wide
variety of VR devices.

osgVRPN is in an early stage of development, but seems functional enough
to share with others.  The one VRPN device currently supported is the
"tracker":
- osgVRPN::TrackerManipulator allows a camera to be positioned using
  a tracker.
- osgVRPN::TrackerTransform allows a scenegraph Node to be positioned
  using a tracker.

I developed osgVRPN using a Spaceball 2003 6-DOF controller mapped as a 
vrpn_Tracker_AnalogFly tracker emulator; see osgVRPNviewer/README.txt
for details.


CREDITS & THANKS

* For VRPN: The NIH National Research Resource in Molecular Graphics and
  Microscopy at the University of North Carolina at Chapel Hill

* For OSG: Robert Osfield and the OSG community


REFERENCES

http://openscenegraph.org/
http://vrpn.org/ or http://www.cs.unc.edu/Research/vrpn/
http://mew.cx/osg/
http://sourceforge.net/projects/osgtoy   <-- SVN tree is here

#EOF
