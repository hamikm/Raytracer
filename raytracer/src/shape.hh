/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "rgbcolor.hh"
#include "mvector.hh"
#include "ray.hh"
#include "boost/shared_ptr.hpp"
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
private:

	/**
	 * Reflectivity of this object, which is a number between 0 and 1. 1 means
	 * totally reflective and 0 means not at all reflective.
	 */
	float reflectivity;

public:

	/**
	 * Initializes the color of this object to gray via the @c sceneobj
	 * default constructor and the reflectivity of this object to 0.
	 */
	shape() : sceneobj<vec_T, color_T, time_T, dim>(), reflectivity(0) { }

	/**
	 * Initializes the color of this object to the specified value via a
	 * @c sceneobj constructor and initializes reflectivity to the given
	 * value or zero by default. Relflectivity must be between 0 and 1.
	 *
	 * @param color
	 * @param reflectivity Optional parameter between 0 and 1 that defaults to
	 * 0.
	 */
	shape(rgbcolor<color_T> color, float reflectivity = 0) :
			sceneobj<vec_T, color_T, time_T, dim>(color) {
		assert(reflectivity >= 0 && reflectivity <= 1);
		this->reflectivity = reflectivity;
	}

	/**
	 * Copy constructor. Uses the @c sceneobj copy constructor.
	 *
	 * @param other The other @c shape object to copy into this one.
	 */
	shape(const shape& other) :
		sceneobj<vec_T, color_T, time_T, dim>(other) {
		this->reflectivity = other.reflectivity;
	}

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
	 * Getter for reflectivity.
	 *
	 * @returns Reflectivity.
	 */
	float getReflectivity() const {
		return reflectivity;
	}

	/**
	 * Setter for reflectivity.
	 *
	 * @param reflectivity The new reflectivity.
	 */
	void setReflectivity(float reflectivity) {
		assert(reflectivity >= 0 && reflectivity <= 1);
		this->reflectivity = reflectivity;
	}

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
typedef boost::shared_ptr<shape<double, double, double, 3> > sp_shape3d;
typedef boost::shared_ptr<shape<double, double, float, 3> > sp_shape3ddf;
typedef boost::shared_ptr<shape<float, float, float, 3> > sp_shape3f;
typedef boost::shared_ptr<shape<double, double, double, 2> > sp_shape2d;
typedef boost::shared_ptr<shape<double, double, float, 2> > sp_shape2ddf;
typedef boost::shared_ptr<shape<float, float, float, 2> > sp_shape2f;

#endif // SHAPE_HH
