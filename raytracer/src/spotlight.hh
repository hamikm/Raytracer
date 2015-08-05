/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "ray.hh"
#include "mvector.hh"
#include "boost/shared_ptr.hpp"
#include "rgbcolor.hh"
#include "light.hh"
#include <math.h>
#include <ostream>

#ifndef SPOTLIGHT_HH
#define SPOTLIGHT_HH

/**
 * Represents a spotlight as a color, position, direction vector, and cone
 * angle.
 *
 * @tparam vec_T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam time_T The type of the time. Many arithmetic and
 *   relational operators must be supported. The type will likely be double or
 *   float, but int might work too.
 * @tparam color_T The type of the @c rgbcolor. Many arithmetic
 *   and relational operators must be supported, including real number ones.
 *   The type will likely be double or float.
 * @tparam dim The number of dimensions, which is likely 3.
 */
template<typename vec_T, typename color_T,  typename time_T, int dim>
class spotlight : public light<vec_T, color_T, time_T, dim> {
private:

	/**
	 * Direction vector, should be normalized.
	 */
	mvector<vec_T, dim> dir;

	/**
	 * Cone angle, should be in radians and between 0 and @f$ 2 \pi @f$.
	 */
	float angle;

public:

	/**
	 * Constructs a white spotlight at the origin pointing up the x-axis with
	 * a cone angle of @f$ \frac{\pi}{6} @f$.
	 */
	spotlight() : light<vec_T, color_T, time_T, dim>() {

		// set direction to +x axis
		dir[0] = 1;
		for(int i = 0; i < dim; i++) {
			dir[i] = 0;
		}

		// set angle to 30 degrees
		angle = M_PI / 6;
	}

	/**
	 * Constructs a spotlight with the given color, position, direction, and
	 * cone angle.
	 *
	 * @param color The rgb color of this spotlight.
	 * @param position The position.
	 * @param direction The direction (will be automatically normalized).
	 * @param coneAngle The cone angle in radians. Must be in @f$(0, \pi]@f$.
	 */
	spotlight(const rgbcolor<color_T> &color, const mvector<vec_T, dim> &pos,
			const mvector<vec_T, dim> &direction, float coneAngle) :
				light<vec_T, color_T, time_T, dim>(color, pos),
				dir(direction), angle(coneAngle) {
		assert(dim > 0);
		assert(coneAngle > 0 && coneAngle <= M_PI);
	}

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 */
	spotlight<vec_T, color_T, time_T, dim>& operator=(
			const spotlight<vec_T, color_T, time_T, dim>& rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		this->setPos(rhs.getPos());
		this->dir = rhs.getDir();
		this->angle = rhs.getAngle();
		return *this;
	}

	/**
	 * Getter for the cone angle.
	 *
	 * @return Cone angle.
	 */
	float getAngle() const {
		return angle;
	}

	/**
	 * Setter for the cone angle.
	 *
	 * @param angle The new cone angle.
	 */
	void setAngle(float angle) {
		assert(angle > 0 && angle <= M_PI);
		this->angle = angle;
	}

	/**
	 * Getter for the direction.
	 *
	 * @return The direction of this spotlight.
	 */
	const mvector<vec_T, dim>& getDir() const {
		return dir;
	}

	/**
	 * Setter for the direction of this spotlight.
	 *
	 * @param New direction.
	 */
	void setDir(const mvector<vec_T, dim>& dir) {
		this->dir = dir;
	}

	/**
	 * Partially overrides the @c light @c printHelper
	 * function by first calling that function then printing more information
	 * about this object. Namely this spotlight's direction and cone angle.
	 */
	void printHelper(std::ostream& os) const {
		light<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [spotlight. direction: " << dir << ", cone angle: " <<
				angle << "]";
	}
};

typedef spotlight<double, double, double, 3> spotlightd;
typedef spotlight<double, double, float, 3> spotlightddf;
typedef spotlight<float, float, float, 3> spotlightf;

typedef boost::shared_ptr<spotlightd> sp_spotlightd;
typedef boost::shared_ptr<spotlightddf> sp_spotlightddf;
typedef boost::shared_ptr<spotlightf> sp_spotlightf;

#endif // SPOTLIGHT_HH
