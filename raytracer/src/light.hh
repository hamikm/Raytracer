/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "ray.hh"
#include "mvector.hh"
#include "rgbcolor.hh"
#include <ostream>

#ifndef LIGHT_HH
#define LIGHT_HH

/**
 * Represents a light as a position and a color.
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
class light : public sceneobj<vec_T, color_T, time_T, dim> {
private:

	/**
	 * Position of this light.
	 */
	mvector<vec_T, dim> pos;

public:

	/**
	 * Constructs a white light at the origin.
	 */
	light() :
		sceneobj<vec_T, color_T, time_T, dim>(
				rgbcolor<color_T>((color_T)1, (color_T)1, (color_T)1)) {

		// set position to origin
		for(int i = 0; i < dim; i++) {
			pos[i] = 0;
		}
	}

	/**
	 * Constructor a light with the given position and color.
	 *
	 * @param color The color of this light.
	 * @param position The position of this light.
	 */
	light(rgbcolor<color_T> color, mvector<vec_T, dim> position) :
		sceneobj<vec_T, color_T, time_T, dim>(color), pos(position) { }

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 */
	light<vec_T, color_T, time_T, dim>& operator=(
			const light<vec_T, color_T, time_T, dim>& rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		pos = rhs.getPos();
		return *this;
	}

	/**
	 * Virtual, empty destructor. Is virtual because subclasses are
	 * anticipated.
	 */
	virtual ~light() { }

	/**
	 * Getter for position.
	 *
	 * @return The position of this light.
	 */
	const mvector<vec_T, dim>& getPos() const {
		return pos;
	}

	/**
	 * Partially overrides the @c sceneobj  @c printHelper
	 * function by first calling that function then printing more information
	 * about this object. Namely this light's position.
	 */
	virtual void printHelper(std::ostream& os) const {
		sceneobj<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [light. position: " << pos << "]";
	}
};

typedef light<double, double, double, 3> light3d;
typedef light<double, double, float, 3> light3ddf;
typedef light<float, float, float, 3> light3f;
typedef light<double, double, double, 2> light2d;
typedef light<double, double, float, 2> light2ddf;
typedef light<float, float, float, 2> light2f;

#endif // LIGHT_HH
