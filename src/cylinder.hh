/**
 * @file
 * @author Hamik Mukelyan
 */

#include <cmath>
#include <cassert>
#include "shape.hh"
#include "sphere.hh"
#include "sceneobj.hh"
#include "mvector.hh"
#include "ray.hh"

#ifndef CYLINDER_HH
#define CYLINDER_HH

/**
 * The dimension of a cylinder.
 */
#define CDIM 3

/**
 * Represents a cylinder with a vector for the center, a unit vector
 * for the orientation of the long axis, a number for the
 * radius, and a number for the height. There is no dimension template
 * argument because a 2D cylinder doesn't make sense.
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
 */
template<typename vec_T, typename color_T,  typename time_T>
class cylinder : public shape<vec_T, color_T, time_T, CDIM>  {
private:

	/**
	 * The center.
	 */
	mvector<vec_T, CDIM> center;

	/**
	 * The radius.
	 */
	vec_T radius;

	/**
	 * Unit vector along the long axis.
	 */
	mvector<vec_T, CDIM> axis;

	/**
	 * The height.
	 */
	vec_T height;

public:

	/**
	 * Constructs a grey cylinder with radius 1, center at the origin,
	 * long axis equal to @f$ \hat{j} @f$, and height 1. Calls the @c shape
	 * default constructor to set the color and reflectivity.
	 */
	cylinder() : shape<vec_T, color_T, time_T, CDIM>() {
		mvector<vec_T, CDIM> v;
		assert(CDIM == 3);
		mvector<vec_T, CDIM> u(0, 1, 0);
		center = v;
		radius = 1;
		axis = u;
		height = 1;
	}

	/**
	 * Constructors a cylinder with the given color, radius, center, height,
	 * long axis orientation, and reflectivity (default value of 0). Sets the
	 * color via a @c shape constructor.
	 *
	 * @param color Color.
	 * @param radius Radius.
	 * @param theCenter Center.
	 * @param height The height.
	 * @param orientation Unit vector for long axis orientation.
	 * @param reflectivity Default value is 0. Should be between 0 and 1,
	 *        inclusive.
	 *
	 * @warning The argument for long axis orientation is normalized.
	 */
	cylinder(const rgbcolor<color_T> &color,
		  vec_T radius,
		  const mvector<vec_T, CDIM> &theCenter,
		  vec_T height,
		  const mvector<vec_T, CDIM> &orientation,
		  float reflectivity = 0) :
			  shape<vec_T, color_T, time_T, CDIM>(color, reflectivity),
			  center(theCenter), radius(radius),
			  axis(orientation.norm()), height(height) {
		assert(CDIM == 3);
		assert(height > 0);
		assert(radius > 0);
	}

	/**
	 * Copy constructor. Uses the @c shape copy constructor for the color
	 * attribute.
	 *
	 * @param other The other @c cylinder object to copy into this one.
	 */
	cylinder(const cylinder<vec_T, color_T, time_T> &other) :
			shape<vec_T, color_T, time_T, CDIM>(
					other.getColor(), other.getReflectivity()) {
		this->radius = other.getRadius();
		this->center = other.getCenter();
		this->axis = other.getAxis();
		this->height = other.getHeight();
	}

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 *
	 * @param rhs
	 */
	cylinder<vec_T, color_T, time_T>& operator=(
			const cylinder<vec_T, color_T, time_T> &rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		this->setReflectivity(rhs.getReflectivity());
		this->radius = rhs.getRadius();
		this->center = rhs.getCenter();
		this->axis = rhs.getAxis();
		this->height = rhs.getHeight();
		return *this;
	}

	/**
	 * Getter for axis.
	 *
	 * @return Axis.
	 */
	const mvector<vec_T, CDIM>& getAxis() const {
		return axis;
	}

	/**
	 * Setter for axis.
	 *
	 * @param axis The new axis.
	 *
	 * @warning The axis vector is automatically normalized.
	 */
	void setAxis(const mvector<vec_T, CDIM>& axis)
	{
		this->axis = axis.nom();
	}

	/**
	 * Getter for the center.
	 *
	 * @return The center.
	 */
	const mvector<vec_T, CDIM>& getCenter() const {
		return center;
	}

	/**
	 * Setter for the center.
	 *
	 * @param center The center of this cylinder.
	 */
	void setCenter(const mvector<vec_T, CDIM>& center)
	{
		this->center = center;
	}

	/**
	 * Getter for the height of this cylinder.
	 *
	 * @return The height.
	 */
	vec_T getHeight() const {
		return height;
	}

	/**
	 * Setter for the height.
	 *
	 * @param height The new height of this cylinder.
	 */
	void setHeight(vec_T height) {
		assert(height > 0);
		this->height = height;
	}

	/**
	 * Getter for the radius.
	 *
	 * @return The radius.
	 */
	vec_T getRadius() const {
		return radius;
	}

	/**
	 * Setter for the radius.
	 *
	 * @param radius The new radius for this cylinder.
	 */
	void setRadius(vec_T radius) {
		assert(radius > 0);
		this->radius = radius;
	}

	/**
	 * Gets the earliest time at which at intersection happens between
	 * the given ray and this cylinder. Returns @c RAY_MISS if there's no hit.
	 *
	 * @param r The ray.
	 *
	 * @return The earliest time at which the ray @c r intersects this
	 * cylinder.
	 */
	time_T intersection(const ray<vec_T, time_T, CDIM> &r) const {

		mvector<vec_T, CDIM> cpar = center.proj(axis);
		mvector<vec_T, CDIM> cperp = center - cpar;

		mvector<vec_T, CDIM> P = r.getOrig();
		mvector<vec_T, CDIM> D = r.getDir();

		mvector<vec_T, CDIM> ppar = P.proj(axis);
		mvector<vec_T, CDIM> pperp = P - ppar;

		mvector<vec_T, CDIM> dpar = D.proj(axis);
		mvector<vec_T, CDIM> dperp = D - dpar;

		sphere<vec_T, color_T, time_T, CDIM> fakesphere(
				this->getColor(),
				radius,
				cperp,
				this->getReflectivity());

		ray<vec_T, time_T, CDIM> ray(pperp, dperp, false);
		time_T t1, t2;

		// t1 and t2 will receive the first and second intersection points
		// on the sphere
		fakesphere.getIntersections(ray, t1, t2);

		mvector<vec_T, CDIM> tmp, val1 = ppar + dpar * t1 - cpar;
		mvector<vec_T, CDIM> val2 = ppar + dpar * t2 - cpar;
		if(val1.magsq() > val2.magsq()) {
			tmp = val1;
			val1 = val2;
			val2 = tmp;
		}
		// now val1 is the smallest
		if(val1.magsq() >= height * height / 4 &&
				val2.magsq() >= height * height / 4) {
			return RAY_MISS;
		}
		else {
			return t1;
		}

		return RAY_MISS;
	}

	/**
	 * Returns the surface normal to this cyulinder at the point @c surfacePt.
	 *
	 * @param surfacePt The point at which to get a normal.
	 *
	 * @return The surface normal at @c surfacePt.
	 */
	mvector<vec_T, CDIM> surfaceNorm(
			const mvector<vec_T, CDIM> &surfacePt) const {
		mvector<vec_T, CDIM> vec = surfacePt - center;
		mvector<vec_T, CDIM> vperp = vec - vec.proj(axis);
		return vperp.norm();
	}

	/**
	 * Partially overrides the @c shape @c printHelper. Calls the base classe's
	 * @c printHelper then prints more info about this object, specifically
	 * this cylinder's radius, center, axis, and height.
	 *
	 * @param os The output stream to which to write.
	 */
	void printHelper(std::ostream &os) const {
		shape<vec_T, color_T, time_T, CDIM>::printHelper(os);
		os << " ---> [cylinder. center: " << center << ", radius: " << radius
				<< ", axis: " << axis << ", height: " << height << "]";
	}
};

typedef cylinder<double, double, double> cylinderd;
typedef cylinder<double, double, float> cylinderddf;
typedef cylinder<float, float, float> cylinderf;

#endif // CYLINDER_HH
