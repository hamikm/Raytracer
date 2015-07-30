/**
 * @file
 * @author Hamik Mukelyan
 */

#include <cmath>
#include "shape.hh"
#include "sceneobj.hh"
#include "mvector.hh"
#include "ray.hh"

#ifndef SPHERE_HH
#define SPHERE_HH

/**
 * Represents a sphere as a center and a radius.
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
template<typename vec_T, typename color_T,  typename time_T, int dim>
class sphere : public shape<vec_T, color_T, time_T, dim>  {
private:

	/**
	 * Radius of this sphere.
	 */
	vec_T rad;

	/**
	 * Center of this sphere.
	 */
	mvector<vec_T, dim> center;

public:

	/**
	 * Constructs a grey sphere with radius 1, center at the origin. Calls
	 * the @c shape default constructor to set the color and reflectivity.
	 */
	sphere() : shape<vec_T, color_T, time_T, dim>() {
		assert(dim > 0);
		mvector<vec_T, dim> v;
		center = v;
		rad = 1;
	}

	/**
	 * Constructors a sphere with given color, radius, center and reflectivity
	 * (default value of 0). Sets the color via a @c shape constructor.
	 *
	 * @param color Color.
	 * @param radius Radius.
	 * @param theCenter Center.
	 * @param reflectivity Default value is 0.
	 */
	sphere(const rgbcolor<color_T> &color,
		  vec_T radius,
		  const mvector<vec_T, dim> &theCenter,
		  float reflectivity = 0) :
			  shape<vec_T, color_T, time_T, dim>(color, reflectivity),
			  rad(radius),
			  center(theCenter) {
		assert(radius > 0);
	}

	/**
	 * Copy constructor. Uses the @c shape copy constructor for the color
	 * attribute.
	 *
	 * @param other The other @c sphere object to copy into this one.
	 */
	sphere(const sphere<vec_T, color_T, time_T, dim> &other) :
			shape<vec_T, color_T, time_T, dim>(
					other.getColor(), other.getReflectivity()) {
		rad = other.getRadius();
		center = other.getCenter();
	}

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 *
	 * @param rhs
	 */
	sphere<vec_T, color_T, time_T, dim>& operator=(
			const sphere<vec_T, color_T, time_T, dim> &rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		this->setReflectivity(rhs.getReflectivity());
		rad = rhs.getRadius();
		center = rhs.getCenter();
		return *this;
	}

	/**
	 * Gets the radius of this sphere.
	 *
	 * @return Radius.
	 */
	vec_T getRadius() const {
		return rad;
	}

	/**
	 * Gets a reference to the center of this sphere.
	 *
	 * @return Center.
	 */
	const mvector<vec_T, dim>& getCenter() const {
		return center;
	}

	/**
	 * Gets all intersection points of the given ray @c r with the this sphere.
	 * Note that a ray can intersect a sphere
	 * at 0, 1, or 2 points. The intersection times are returned in the out
	 * parameters @c t1 and @c t2. @c t2 is guaranteed to be larger than @c t1
	 * and @c t1 will be defined before @c t2. If something isn't defined then
	 * it gets a @c RAY_MISS sentinel value. Times less than zero are
	 * considered misses.
	 *
	 * The intersection points are computed as the solution(s) to
	 * @f$ a t^2 + b t + c = 0 @f$ where @f$ \mathbf{p} + \mathbf{d} t @f$
	 * defines the ray, @f$ |\mathbf{x} - \mathbf{c}| = r @f$ defines the
	 * sphere, @f$ a = \mathbf{d} \cdot \mathbf{d} @f$, @f$ b =
	 * 2 (\mathbf{p} \cdot \mathbf{d} - \mathbf{d} \cdot \mathbf{c}) @f$, and
	 * @f$ c = \mathbf{p} \cdot \mathbf{p} + \mathbf{c} \cdot \mathbf{c} -
	 * 2 \mathbf{p} \cdot \mathbf{c} - r^2 @f$.
     *
     * @param r The ray.
     * @param[out] t1 The first intersection time.
     * @param[out] t2 The second intersection time.
     *
     * @return Number of intersections of the given ray with this sphere.
	 */
	int getIntersections(const ray<vec_T, time_T, dim> &r,
			time_T &t1, time_T &t2) const {
		vec_T a = r.getDir() * r.getDir();
		vec_T b = 2 * (r.getOrig() * r.getDir() - r.getDir() * center);
		vec_T c = r.getOrig() * r.getOrig() + center * center -
				2 * (r.getOrig() * center) - rad * rad;
		vec_T tmpsqrt;
		tmpsqrt = b * b - 4 * a * c;
		if (tmpsqrt < 0) {
			t1 = RAY_MISS;
			t2 = RAY_MISS;
			return 0;
		}
		else if (tmpsqrt == 0) {
			t1 = (time_T)(-b / (2 * a));
			if (t1 < 0) {
				t1 = RAY_MISS;
				return 0;
			}
			return 1;
		}
		else {
			tmpsqrt = sqrt(tmpsqrt);
			t1 = (time_T)((-b + tmpsqrt) / (2 * a));
			t2 = (time_T)((-b - tmpsqrt) / (2 * a));
			// Put t1 and t2 into ascending order.
			if (t1 > t2) {
				time_T tmp = t1;
				t1 = t2;
				t2 = tmp;
			}
			// Now that they're in ascending order, check if they're negative.
			if (t1 < 0 && t2 < 0) {
				t1 = RAY_MISS;
				t2 = RAY_MISS;
				return 0;
			}
			if (t1 < 0) {
				t1 = RAY_MISS;
				return 1;
			}
			return 2;
		}
	}

	/**
	 * Gets the earliest time at which at intersection happens between
	 * the given ray and this sphere. Returns @c RAY_MISS if there's no hit.
	 *
	 * @param r The ray.
	 *
	 * @return The earliest time at which the ray @c r intersects this sphere.
	 */
	time_T intersection(const ray<vec_T, time_T, dim> &r) const {

		// t1 and t2 will receive the first and second intersection points
		// on the sphere
		time_T t1, t2;
		getIntersections(r, t1, t2);

		// return the first intersection point, since that's what this
		// function is supposed to do.
		return t1;
	}

	/**
	 * Returns the surface normal to this sphere at the point @c surfacePt.
	 *
	 * @param surfacePt The point at which to get a normal.
	 *
	 * @return The surface normal at @c surfacePt.
	 */
	mvector<vec_T, dim> surfaceNorm(
			const mvector<vec_T, dim> &surfacePt) const {
		mvector<vec_T, dim> tmp = surfacePt - center;
		return tmp.norm();
	}

	// colorAt doesn't have an override yet.

	/**
	 * Partially overrides the @c shape @c printHelper. Calls the base classe's
	 * @c printHelper then prints more info about this object, specifically
	 * this sphere's radius and center.
	 *
	 * @param os The output stream to which to write.
	 */
	void printHelper(std::ostream &os) const {
		shape<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [sphere. center: " << center << ", radius: " << rad
				<< "]";
	}
};

typedef sphere<double, double, double, 3> sphere3d;
typedef sphere<double, double, float, 3> sphere3ddf;
typedef sphere<float, float, float, 3> sphere3f;
typedef sphere<double, double, double, 2> sphere2d;
typedef sphere<double, double, float, 2> sphere2ddf;
typedef sphere<float, float, float, 2> sphere2f;

#endif // SPHERE_HH
