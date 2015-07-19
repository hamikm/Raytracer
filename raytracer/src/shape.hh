/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "rgbcolor.hh"
#include "mvector.hh"
#include "ray.hh"
#include <ostream>

#ifndef SHAPE_HH
#define SHAPE_HH

/**
 * Abstract base class for shapes. Subclasses of this class will include
 * spheres, infinite planes, cones, etc. In addition to the color attribute
 * and getter/setter inherited from @c sceneobj this class has abstract
 * functions related to physical dimensions, like @c intersection and
 * @c surfaceNorm.
 *
 * @tparam vec_T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam time_T The type of the time. Many arithmetic and
 *   relational operators must be supported. The type will likely be double or
 *   float, but int would likely work.
 * @tparam color_T The type of the @c rgbcolor. Many arithmetic
 *   and relational operators must be supported, including real number ones.
 *   The type will likely be double or float.
 * @tparam dim The number of dimensions. Note that 2 would make sense, since
 *   it would specify a line.
 */
template<typename vec_T, typename color_T, typename time_T, int dim>
class shape : public sceneobj<vec_T, color_T, time_T, dim> {
public:

	/**
	 * Initializes the color of this object to gray via the @c sceneobj
	 * default constructor.
	 */
	shape() : sceneobj<vec_T, color_T, time_T, dim>() { }

	/**
	 * Initializes the color of this object to the specified value via a
	 * @c sceneobj constructor.
	 *
	 * @param color Color.
	 */
	shape(rgbcolor<color_T> color) :
		sceneobj<vec_T, color_T, time_T, dim>(color) { }

	/**
	 * Copy constructor. Uses the @c sceneobj copy constructor.
	 *
	 * @param other The other @c shape object to copy into this one.
	 */
	shape(const shape& other) :
		sceneobj<vec_T, color_T, time_T, dim>(other) { }

	/**
	 * Does nothing here but might need to be defined in subclasses.
	 */
	virtual ~shape() { }

	/**
	 * Gets the earliest time at which at intersection happens between
	 * the given ray and this scene object. This function is abstract and
	 * should be defined in subclasses. It should return the @c RAY_MISS
	 * sentinel if no intersection occurs.
	 *
	 * @param r The ray.
	 *
	 * @return The time of intersection of the given ray @c r with this shape.
	 */
	virtual time_T intersection(const ray<vec_T, time_T, dim> &r) const = 0;

	/**
	 * Gets the surface normal to this scene object at the specified point.
	 * The argument is assumed to be on the surface of this object. This
	 * function is abstract.
	 *
	 * @param surfacePt The point at which to get a surface normal.
	 *
	 * @return The surface normal.
	 */
	virtual mvector<vec_T, dim> surfaceNorm(
			const mvector<vec_T, dim> &surfacePt) const = 0;

	/**
	 * Print helper function which partially overrides the one in @c sceneobj.
	 * Derived classes should partially override this.
	 *
	 * @param os The output stream to which to write.
	 */
	virtual void printHelper(std::ostream &os) const {
		sceneobj<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [shape]";
	}
};

typedef shape<double, double, double, 3> shape3d;
typedef shape<double, double, float, 3> shape3ddf;
typedef shape<float, float, float, 3> shape3f;
typedef shape<double, double, double, 2> shape2d;
typedef shape<double, double, float, 2> shape2ddf;
typedef shape<float, float, float, 2> shape2f;

#endif // SHAPE_HH
