/**
 * @file
 * @author Hamik Mukelyan
 */

#include <cassert>
#include "mvector.hh"
#include <ostream>

#ifndef RAY_HH
#define RAY_HH

/**
 * Represents a ray with an origin point and a direction vector.
 *
 * @tparam vec_T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam time_T The type of the time. Many arithmetic and
 *   relational operators must be supported. The type will likely be double or
 *   float, but int might work too.
 * @tparam dim The number of dimensions, which is likely 2 or 3.
 */
template<typename vec_T, typename time_T, int dim>
class ray {
private:

	/**
	 * Point of origin of this ray. Represented as a vector even though it's
	 * actually a point.
	 */
	mvector<vec_T, dim> orig;

	/**
	 * Direction of this ray. Can be normalized or not.
	 */
	mvector<vec_T, dim> dir;

public:

	/**
	 * Constructs a normalized ray at the origin pointing along the positive
	 * x-axis.
	 */
	ray() {
		for (int i = 0; i < dim; i++) {
			orig[i] = 0;
			dir[i] = 0;
		}
		dir[0] = 1;
	}

	/**
	 * Constructs a ray with the specified origin and direction. The direction
	 * gets normalized by default but normalization can be turned off.
	 *
	 * @param start Origin point of this ray.
	 * @param direction Direction of this ray.
	 * @param normalizeDir @c true by default, can be set to @c false when the
	 *   direction vector shouldn't be normalized.
	 */
	ray(const mvector<vec_T, dim> &start, const mvector<vec_T, dim> &direction,
			bool normalizeDir = true) {
		this->orig = start; // copy origination point to orig
		this->dir = normalizeDir ? direction.norm() : direction;
	}

	/**
	 * Gets direction of this ray.
	 *
	 * @return Direction.
	 */
	const mvector<vec_T, dim>& getDir() const {
		return dir;
	}

	/**
	 * Assignment operator overload.
	 *
	 * @param rhs
	 *
	 * @return Reference to this ray for operator chaining.
	 */
	ray<vec_T, time_T, dim>& operator=(
			const ray<vec_T, time_T, dim>& rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		orig = rhs.getOrig();
		dir = rhs.getDir();
		return *this;
	}

	/**
	 * Gets origin point of this ray.
	 *
	 * @return Origin point.
	 */
	const mvector<vec_T, dim>& getOrig() const {
		return orig;
	}

	/**
	 * Computes the ray's position at time t, which must be at least 0. The
	 * position is computed as @f$ \mathbf{v} = \mathbf{p} + \mathbf{d} t @f$
	 *
	 * @param t Time at which to compute this ray's position.
	 *
	 * @return Position of the ray at the given time.
	 */
	mvector<vec_T, dim> getPointAtT(time_T t) const {
		assert(t >= 0);
		return orig + t * dir;
	}

	/**
	 * Prints a representation of this object to the given output stream.
	 *
	 * @param os The output stream to which to print.
	 */
	void printHelper(std::ostream& os) const {
		os << "start: " << orig << "\tdirection: " << dir;
	}
};

/**
 * Output operator overload for printing ray contents to an output stream.
 *
 * @param os The output stream to which to print.
 * @param theRay The ray to print to the given output stream.
 *
 * @return Reference to the given output stream for operator chaining.
 */
template<typename vec_T, typename time_T, int dim>
std::ostream & operator<<(std::ostream& os,
		const ray<vec_T, time_T, dim>& theRay) {
	theRay.printHelper(os);
	return os;
}

typedef ray<double, double, 3> ray3d;
typedef ray<double, float, 3> ray3df;
typedef ray<float, float, 3> ray3f;
typedef ray<double, double, 2> ray2d;
typedef ray<double, float, 2> ray2df;
typedef ray<float, float, 2> ray2f;

#endif // RAY_HH
