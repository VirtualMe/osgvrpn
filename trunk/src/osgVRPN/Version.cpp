// $Id: Version.cpp 613 2011-06-24 21:52:47Z mew $
// Subversion's keyword substitution must be enabled to automatically update these values:
//      svn propset svn:keywords "Id Date Author URL Revision" Version.cpp

#include <osgVRPN/Version.h>


const char* osgVRPNGetVersion()
{
    return "0.5.1";
}

const char* osgVRPNGetLibraryName()
{
    return "Open Scene Graph VRPN Library from http://mew.cx/";
}



const char* osgVRPNSvnKeywordId()
{
    return "$Id: Version.cpp 613 2011-06-24 21:52:47Z mew $";
}

const char* osgVRPNSvnKeywordDate()
{
    return "$Date: 2011-06-24 17:52:47 -0400 (Fri, 24 Jun 2011) $";
}

const char* osgVRPNSvnKeywordAuthor()
{
    return "$Author: mew $";
}

const char* osgVRPNSvnKeywordUrl()
{
    return "$URL: https://osgtoy.svn.sourceforge.net/svnroot/osgtoy/osgvrpn/trunk/src/osgVRPN/Version.cpp $";
}

const char* osgVRPNSvnKeywordRevision()
{
    return "$Revision: 613 $";
}


// vim: set sw=4 ts=8 et ic ai:
