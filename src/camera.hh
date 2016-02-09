/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "ray.hh"
#include "light.hh"
#include "shape.hh"
#include "mvector.hh"
#include "boost/shared_ptr.hpp"
#include <cassert>
#include <vector>
#include <iostream>
#include <ostream>
#include <iterator>
#include <cmath>

#ifndef CAMERA_HH
#define CAMERA_HH

/**
 * Represents a 3D camera. Note that there are some convenient typedefs in this
 * file.
 *
 * @tparam vec_T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam time_T The type of the time. Many arithmetic and
 *   relational operators must be supported. The type will likely be double or
 *   float, but int might work too.
 * @tparam dim The number of dimensions in which this camera will operate.
 *   This will almost certainly be three.
 */
template<typename vec_T, typename time_T, int dim>
class camera {
private:

	/**
	 * Camera's location in the scene.
	 */
	mvector<vec_T, dim> pos;

	/**
	 * Normalized direction vector.
	 */
	mvector<vec_T, dim> dir;

	/**
	 * Field of view in radians.
	 */
	vec_T fov;

	/*
	 * Normalized "camera up" vector.
	 */
	mvector<vec_T, dim> up;

	/**
	 * Normalized "camera right" vector.
	 */
	mvector<vec_T, dim> right;

	/**
	 * Used to compute the field of view.
	 */
	vec_T dist;


public:

	/**
	 * Constructs a camera object at the given @c position pointed
	 * at the @c lookAt point. The @c upDir argument
	 * helps pick one of the many possible orientations of the camera. The
	 * default field of view is 60 degrees but can be specified.
	 *
	 * @param position Center of the camera.
	 * @param lookAt Camera is aimed at this point.
	 * @param upDir The direction considered up for this camera.
	 * @param fieldOfView Default is @f$\frac{\pi}{3}@f$; must be in radians.
	 */
	camera(const mvector<vec_T, dim> &position,
			const mvector<vec_T, dim> &lookAt,
			const mvector<vec_T, dim> &upDir,
			vec_T fieldOfView = M_PI / 3) : pos(position), fov(fieldOfView) {
		dir = (lookAt - pos).norm();
		right = (dir % upDir).norm();
		up = (right % dir).norm();
		dist = 0.5 / tan((double)fov / 2);
	}

	/**
	 * Generates a ray that goes from the camera through the given pixel
	 * in the rectangular image grid determined by @c dist and @c fov.
	 *
	 * @param x x coordinate of pixel. Increases to right.
	 * @param y y coordinate of pixel. Increases to @e bottom.
	 * @param width Width of the image in pixels.
	 * @param height Height of the iamge in pixels.
	 *
	 * @return Ray from the camera's position through the given pixel.
	 */
	ray<vec_T, time_T, dim> getRayForPixel(int x, int y,
			int width, int height) const {
		assert(x >= 0);
		assert(y >= 0);
		assert(width > 0);
		assert(height > 0);
		vec_T centerx = ((vec_T) width) / height / 2;
	    mvector<vec_T, dim> pixelDir = dist * dir +
	    		(0.5 - (vec_T) y / (vec_T) (height - 1)) * up +
				((vec_T) x / (vec_T) (height - 1) - centerx) * right;

	    ray<vec_T, time_T, dim> pixelRay(pos, pixelDir);
	    return pixelRay;
	}

	/**
	 * Prints camera attributes to the given output stream.
	 *
	 * @param os Output stream to which to write a string representation of
	 *   this object.
	 */
	void printHelper(std::ostream &os) const {
		os << "[camera. pos: " << pos << ", dir: " << dir << ", up: " << up
				<< ", right: " << right << ", fov: " << fov << ", dist: " <<
				dist <<  "]";
	}
};

/**
 * Output operator overload for printing camera attributes to an output
 * stream.
 *
 * @param os Output stream to which to write a string representation of
 *   this object.
 * @param cam The camera to print.
 *
 *  @return Reference to the same output stream for operator chaining.
 */
template<typename vec_T, typename time_T, int dim>
std::ostream & operator<<(std::ostream &os,
		const camera<vec_T, time_T, dim> &cam) {
	cam.printHelper(os);
	return os;
}

typedef camera<double, double, 3> camerad;
typedef camera<double, float, 3> cameradf;
typedef camera<float, float, 3> cameraf;
typedef boost::shared_ptr<camera<double, double, 3> > sp_camerad;
typedef boost::shared_ptr<camera<double, float, 3> > sp_cameradf;
typedef boost::shared_ptr<camera<float, float, 3> > sp_cameraf;

#endif // CAMERA_HH
