#ifndef OSGVRPN_VRMANIPULATOR
#define OSGVRPN_VRMANIPULATOR 1

#include <osgVRPN/TrackerManipulator.h>
#include <osgVRPN/Export.h>

/* An osgVRPN::TrackerManipulator that abstracts away moving
 * through the navigated space.
 */

namespace osgVRPN 
{
	class OSGVRPN_EXPORT VRManipulator : public TrackerManipulator
	{
		public:
			VRManipulator ( osg::ref_ptr<osgVRPN::TrackerBase> tracker = 0 );
			const char* className() const { return "osgVRPN::VRManipulator"; }

			typedef enum 
			{
				HMD,
				FRONT,
				BACK,
				LEFT,
				RIGHT,
				TOP,
				BOTTOM
			} Mode;

			/** Set/get the "Walled Mode" setting. */
			void setMode( const Mode mode);
			Mode getMode() const;

			/** Query the position of this manipulator. */
			osg::Matrix getMatrix() const;
			osg::Matrix getInverseMatrix() const;

			/** Set/get the nav matrix. */
			void setNavMatrix(const osg::Matrixd& matrix); 
			osg::Matrix getNavMatrix();

			/** Methods for navigation. */
			void navTranslate( const osg::Vec3& vec );
			void navTranslate( float x, float y, float z );
			void navRotate( const osg::Quat& quat );
			void navRotate( float angle, const osg::Vec3& axis );

			/** Public access to identity functions for home and
			  * and nav matricies. */
			void navMakeIdentity();
			void homeMakeIdentity();

		protected:		// methods
			~VRManipulator() {}

		protected:		// data
			osg::Matrixd _navMatrix;
			Mode		 _mode;
	};
}

#endif // OSGVRPN_VRMANIPULATOR