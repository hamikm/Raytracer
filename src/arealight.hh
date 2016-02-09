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
#include "spotlight.hh"
#include <math.h>
#include <vector>
#include <ostream>

#ifndef AREALIGHT_HH
#define AREALIGHT_HH

/**
 * Approximates a rectangular area light as a collection of point lights. The
 * lights are positioned on a finite plane with local uv coordinates; the u and
 * v axes are computed from the surface normal and an up vector. The width,
 * height, and spacing in each direction are used to compute the number of
 * point lights, then those point lights are automatically generated and placed
 * in a vector. Note that the color of this light will effectively be whatever
 * is passed to the constructor, but that the color of each individual light
 * will be that color divided by N, where N is the number of lights in this
 * area light. Recall that N is computed from the dimensions and spacing.
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
class arealight : public light<vec_T, color_T, time_T, dim> {
private:

	/**
	 * Surface normal vector.
	 */
	mvector<vec_T, dim> norm;

	/**
	 * Horizontal spacing of the point lights in this area light.
	 */
	vec_T horizontalSpacing;

	/**
	 * Vertical spacing of the point lights in this area light.
	 */
	vec_T verticalSpacing;

	/**
	 * Width of this area light.
	 */
	vec_T width;

	/**
	 * Height of this area light.
	 */
	vec_T height;


	/**
	 * The normalized vector pointing along the v axis of the area light plane.
	 */
	mvector<vec_T, dim> vhat;

	/**
	 * The normalized vector pointing along the u axis of the area light plane.
	 */
	mvector<vec_T, dim> uhat;

	/**
	 * Collection of all the point lights that constitute this area light.
	 */
	std::vector<boost::shared_ptr<light<vec_T, color_T, time_T, dim> > > lights;

	/**
	 * Helper function for the constructors that assumes that all the instance
	 * variables have been initialized except @c uhat , @c vhat , and
	 * @c lights, all of which are initialized by this function.
	 *
	 * @param upDirection Used to compute v-hat and u-hat.
	 */
	void constructorHelper(const mvector<vec_T, dim> &upDirection) {
		// Compute uhat by taking the cross product of the surface normal
		// and the up vector then normalizing.
		uhat = upDirection % norm;
		uhat = uhat.norm();

		// Compute vhat by taking the cross product of the surface normal
		// and uhat then normalizing.
		vhat = norm % uhat;
		vhat = vhat.norm();

		// Compute number of lights.
		int numlights = static_cast<int> (width / horizontalSpacing) *
				static_cast<int> (height / verticalSpacing);

		// Loop over the rows of the lights.
		for(vec_T u = -height / 2; u <= height / 2; u += verticalSpacing) {

			for(vec_T v = -width / 2; v <= width / 2; v += horizontalSpacing) {

				// Compute world coordinates from the local uv coordinates.
				mvector<vec_T, dim> worldpos = uhat * u + vhat * v +
						this->getPos();

				// Construct a point light at the world coordinates with a color
				// scaled so that the combination of all the lights in the area
				// light look like the color passed to the constructor.
				boost::shared_ptr<light<vec_T, color_T, time_T, dim> >
					currLight_sp(
							new light<vec_T, color_T, time_T, dim>(
									this->getColor() / numlights, worldpos));
				lights.push_back(currLight_sp);
			}
		}
	}

public:

	/**
	 * Constructs a white area light at <0, 1, 0> pointing down the y-axis with
	 * width 0.5, height 0.5, spacing 0.1. Assumes three dimensions.
	 *
	 * @warning Assertion is triggered if dimension isn't three.
	 */
	arealight() : light<vec_T, color_T, time_T, dim>() {
		// color is set to 0.7, 0.7, 0.7, but change position from origin
		this->setPos(mvector<vec_T, 3>(0.0, 1.0, 0.0));
		assert(dim == 3);
		norm[0] = 0;
		norm[1] = -1;
		norm[2] = 0;
		this->horizontalSpacing = 0.1;
		this->verticalSpacing = 0.1;
		this->width = 0.5;
		this->height = 0.5;
		constructorHelper(vector3d(1.0, 0.0, 0.0));
	}

	/**
	 * Constructs an area light with the given color, position, direction,
	 * dimensions, and spacing.
	 *
	 * @param color
	 * @param center
	 * @param surfaceNormal
	 * @param upDirection Used to compute v-hat and u-hat.
	 * @param horizontalSpacing Between point lights in the area light.
	 *        Used along with @c verticalSpacing to compute number of lights.
	 * @param verticalSpacing Between point lights in the area light.
	 * 		  Used along with @c horiztonalSpacing to compute number of lights.
	 * @param width
	 * @param height
	 */
	arealight(const rgbcolor<color_T> &color, const mvector<vec_T, dim> &center,
			const mvector<vec_T, dim> &surfaceNormal,
			const mvector<vec_T, dim> &upDirection,
			vec_T horizontalSpacing, vec_T verticalSpacing,
			vec_T width, vec_T height) :
				light<vec_T, color_T, time_T, dim>(color, center),
				norm(surfaceNormal.norm()),
				horizontalSpacing(horizontalSpacing),
				verticalSpacing(verticalSpacing),
				width(width), height(height) {

		// After the initializer list ends we still need to deal with
		// vhat, uhat, and the lights collection.
		assert(dim > 0);
		assert(horizontalSpacing > 0);
		assert(verticalSpacing > 0);
		assert(horizontalSpacing < width);
		assert(verticalSpacing < height);

		constructorHelper(upDirection);
	}

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 */
	arealight<vec_T, color_T, time_T, dim>& operator=(
			const arealight<vec_T, color_T, time_T, dim>& rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		this->setPos(rhs.getPos());
		this->setNorm(rhs.getNorm());
		this->setHorizontalSpacing(rhs.getHorizontalSpacing());
		this->setVerticalSpacing(rhs.getVerticalSpacing());
		this->setWidth(rhs.getWidth());
		this->setHeight(rhs.getHeight());
		this->setUhat(rhs.getUhat());
		this->setVhat(rhs.getVhat());

		// Deep copy lights from rhs to this object's lighs collection.
		lights.clear();
		typename std::vector<boost::shared_ptr<
			light<vec_T, color_T, time_T, dim> > >::const_iterator iter;
		for(iter = lights.begin(); iter < lights.end(); iter++) {
			boost::shared_ptr<light<vec_T, color_T, time_T, dim> >
								currLight_sp(
										new light<vec_T, color_T, time_T, dim>(
												**iter));
			lights.push_back(currLight_sp);
		}
		return *this;
	}

	/**
	 * Partially overrides the @c light @c printHelper
	 * function by first calling that function then printing more information
	 * about this object.
	 */
	void printHelper(std::ostream& os) const {
		light<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [area light. height: " << height <<
				", horizontal spacing: " << horizontalSpacing <<
				", surface normal: " << norm <<
				", uhat: " << uhat <<
				", vhat: " << vhat <<
				", width: " << width <<
				", lights:" << std::endl;
		os << "    {" << std::endl;
		typename std::vector<boost::shared_ptr<
			light<vec_T, color_T, time_T, dim> > >::const_iterator iter;
		for(iter = lights.begin(); iter < lights.end(); iter++) {
			os << "    " << **iter << std::endl;
		}
		os << "    }" << std::endl << "]";
	}

	/**
	 * Getter for height.
	 *
	 * @return Height.
	 */
	vec_T getHeight() const {
		return height;
	}

	/**
	 * Setter for height.
	 *
	 * @param height
	 */
	void setHeight(vec_T height) {
		this->height = height;
	}

	/**
	 * Getter for horizontal spacing.
	 *
	 * @return Horizontal spacing.
	 */
	vec_T getHorizontalSpacing() const {
		return horizontalSpacing;
	}

	/**
	 * Setter for horizontal spacing.
	 *
	 * @param horizontalSpacing
	 */
	void setHorizontalSpacing(vec_T horizontalSpacing) {
		this->horizontalSpacing = horizontalSpacing;
	}

	/**
	 * Getter for the collection of point lights in this area light.
	 *
	 * @return Vector of Boost pointers to lights.
	 */
	const std::vector<boost::shared_ptr<light<vec_T, color_T, time_T, dim> > >&
	getLights() const {
		return lights;
	}

	/**
	 * Getter for the surface Normal.
	 *
	 * @return Surface normal.
	 */
	const mvector<vec_T, dim>& getNorm() const {
		return norm;
	}

	/**
	 * Setter for the surface normal.
	 *
	 * @param norm
	 */
	void setNorm(const mvector<vec_T, dim>& norm) {
		this->norm = norm;
	}

	/**
	 * Getter for the normalized local coordinate system u axis vector.
	 *
	 * @return uhat
	 */
	const mvector<vec_T, dim>& getUhat() const {
		return uhat;
	}

	/**
	 * Setter for the normalized local coordinate system u axis vector.
	 *
	 * @param uhat
	 */
	void setUhat(const mvector<vec_T, dim>& uhat) {
		this->uhat = uhat;
	}

	/**
	 * Getter for the vertical spacing.
	 *
	 * @return Vertical spacing.
	 */
	vec_T getVerticalSpacing() const {
		return verticalSpacing;
	}

	/**
	 * Setter for the vertical spacing.
	 *
	 * @param verticalSpacing
	 */
	void setVerticalSpacing(vec_T verticalSpacing) {
		this->verticalSpacing = verticalSpacing;
	}

	/**
	 * Getter for the normalized local coordinate system v axis vector.
	 *
	 * @return vhat
	 */
	const mvector<vec_T, dim>& getVhat() const {
		return vhat;
	}

	/**
	 * Setter for the normalized local coordinate system v axis vector.
	 *
	 * @param vhat
	 */
	void setVhat(const mvector<vec_T, dim>& vhat) {
		this->vhat = vhat;
	}

	/**
	 * Getter for the width.
	 *
	 * @return Width.
	 */
	vec_T getWidth() const {
		return width;
	}

	/**
	 * Setter for the width.
	 *
	 * @param width
	 */
	void setWidth(vec_T width) {
		this->width = width;
	}
};

typedef arealight<double, double, double, 3> arealightd;
typedef arealight<double, double, float, 3> arealightddf;
typedef arealight<float, float, float, 3> arealightf;

typedef boost::shared_ptr<arealightd> sp_arealightd;
typedef boost::shared_ptr<arealightddf> sp_arealightddf;
typedef boost::shared_ptr<arealightf> sp_arealightf;

#endif // AREALIGHT_HH
