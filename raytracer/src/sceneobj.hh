/**
 * @file
 * @author Hamik Mukelyan
 */

#include "rgbcolor.hh"
#include "mvector.hh"
#include "ray.hh"
#include <ostream>

#ifndef SCENEOBJ_HH
#define SCENEOBJ_HH

/**
 * The value reported by the intersection function if a ray misses.
 */
#define RAY_MISS -1

/**
 * Abstract base class for scene objects. Subclasses of this class will
 * include shapes like spheres, infinite planes, lights, etc. This base class
 * just has a color attribute and support methods.
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
class sceneobj {
private:

	/**
	 * Color of this scene object.
	 */
	rgbcolor<color_T> color;

public:

	/**
	 * Initializes the color of this object to gray.
	 */
	sceneobj() {
		this->color = rgbcolor<color_T>(0.5, 0.5, 0.5);
	}

	/**
	 * Initializes the color of this object to the specified value.
	 *
	 * @param color Color.
	 */
	sceneobj(rgbcolor<color_T> color) {
		this->color = color; // copy the given color into the state variable
	}

	/**
	 * Copy constructor.
	 *
	 * @param other Other @c sceneobj object to copy into this one.
	 */
	sceneobj(const sceneobj& other) {
		this->color = other.getColor();
	}

	/**
	 * Does nothing here but might need to be defined in subclasses.
	 */
	virtual ~sceneobj() { }

	/**
	 * Gets the color of this scene object.
	 *
	 * @return Color.
	 */
	const rgbcolor<color_T> & getColor() const {
		return color;
	}

	/**
	 * Sets the color of this scene object to the given value.
	 *
	 * @param color Color.
	 */
	void setColor(const rgbcolor<color_T> &color) {
		this->color = color;
	}

	/**
	 * Gets the color at the given point on this scene object. This version
	 * simply returns the color of this object, but subclasses can
	 * override this function to take into account surface normals, light,
	 * textures, etc.
	 *
	 * @param surfacePt The point at which to compute the color.
	 *
	 * @return The color at @c surfacePt.
	 */
	virtual rgbcolor<color_T> colorAt(
			const mvector<vec_T, dim> &surfacePt) const {
		return color;
	}

	/**
	 * Print helper function. Derived classes should (partially) override this.
	 *
	 * @param os The output stream to which to write scene object information.
	 */
	virtual void printHelper(std::ostream &os) const {
		os << "[scene object. color: " << color << "]";
	}
};

/**
 * Output operator override for printing contents of the given object
 * to an output stream. Uses the printHelper function, which is virtual
 * because derived classes will want to modify or enhance printing behavior.
 *
 * @param os The output stream to which to write.
 * @param so The @c sceneobj object to write.
 *
 * @return The same output stream for operator chaining.
 */
template<typename vec_T, typename color_T,  typename time_T, int dim>
std::ostream & operator<<(std::ostream &os,
		const sceneobj<vec_T, color_T, time_T, dim> &so) {
	so.printHelper(os);
	return os;
}

typedef sceneobj<double, double, double, 3> sceneobj3d;
typedef sceneobj<double, double, float, 3> sceneobj3ddf;
typedef sceneobj<float, float, float, 3> sceneobj3f;
typedef sceneobj<double, double, double, 2> sceneobj2d;
typedef sceneobj<double, double, float, 2> sceneobj2ddf;
typedef sceneobj<float, float, float, 2> sceneobj2f;

#endif // SCENEOBJ_HH
