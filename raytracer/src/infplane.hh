/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "shape.hh"
#include "ray.hh"

#ifndef INFPLANE_HH
#define INFPLANE_HH

/**
 * Represents an infinite plane. The plane is determined completely by
 * a surface normal and a distance from the origin. Note that there are some
 * convenient typedefs in this file.
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
class infplane : public shape<vec_T, color_T, time_T, dim> {
private:

	/**
	 * Distance of this plane's center from the origin.
	 */
	vec_T dist;

	/**
	 * Surface normal for this vector.
	 */
	mvector<vec_T, dim> surfNorm;

public:

	/**
	 * Constructs a plane with distance 0 from the origin
	 * and a surface normal of @f$ (0, \cdots , 1) @f$. That is, it creates the
	 * @f$ xy @f$ plane in 3D and the @f$ x @f$ axis in 2D. Its color is grey
	 * via the @c shape default constructor.
	 */
	infplane() : shape<vec_T, color_T, time_T, dim>() {
		assert(dim > 0);
		dist = 0;
		mvector<vec_T, dim> v;
		v[dim - 1] = 1;
		surfNorm = v;
	}

	/**
	 * Constructs a plane with the given distance from origin and surface
	 * normal. The superclass constructor is called to deal
	 * with the color parameter.
	 *
	 * @param color Color of this plane.
	 * @param distFromOrig Distance from the origin.
	 * @param surfaceNormal A vector normal to this plane. This will get
	 *   automatically normalized.
	 *
	 * @warning @c surfaceNormal  will get automatically
	 *   normalized!
	 */
	infplane(const rgbcolor<color_T> &color,
			vec_T distFromOrig,
			const mvector<vec_T, dim> &surfaceNormal) :
				shape<vec_T, color_T, time_T, dim>(color),
				dist(distFromOrig), surfNorm(surfaceNormal.norm()) { }

	/**
	 * Copy constructor. Uses the @c shape copy constructor.
	 *
	 * @param other
	 */
	infplane(const infplane<vec_T, color_T, time_T, dim> &other) :
			shape<vec_T, color_T, time_T, dim>(other.getColor()) {
		dist = other.getDist();
		surfNorm = other.getSurfNorm();
	}

	/**
	 * Assignment operator overload. Guards against self-assignment.
	 *
	 * @param rhs
	 *
	 * @return Reference to this plane for operator chaining.
	 */
	infplane<vec_T, color_T, time_T, dim>& operator=(
			const infplane<vec_T, color_T, time_T, dim> &rhs) {
		if(this == &rhs) // check for self-assignment
			return *this;
		this->setColor(rhs.getColor());
		dist = rhs.getDist();
		surfNorm = rhs.getSurfNorm();
		return *this;
	}

	/**
	 * Getter for the distance of the center of this plane from the origin.
	 *
	 * @return Distance of center from origin.
	 */
	vec_T getDist() const {
		return dist;
	}

	/**
	 * Returns a reference to the surface normal of this plane.
	 */
	const mvector<vec_T, dim>& getSurfNorm() const {
		return surfNorm;
	}

	/**
	 * Computes the time at which the given ray intersects this plane.
	 * For a ray @f$ \mathbf{r} = \mathbf{p} + \mathbf{d} t @f$ the
	 * intersection time is given by @f[ t = -\frac{ \mathbf{p} \cdot \mathbf{n}
	 * + d }{ \mathbf{d} \cdot \mathbf{n}} @f]. If there is no intersection or
	 * if the intersection time is negative this function returns @c
	 * RAY_MISS .
     *
     * @param r The ray to intersect with this plane.
     *
     * @return Intersection time or @c RAY_MISS .
	 */
	time_T intersection(const ray<vec_T, time_T, dim> &r) const {
		vec_T denom = r.getDir() * surfNorm;
		if (denom == 0)
			return RAY_MISS;
		time_T t= (time_T) (-(r.getOrig() * surfNorm + dist) / denom);
		if (t < 0)
			return RAY_MISS;
		return t;
	}

	/**
	 * Returns the surface normal of this plane, which is the same at all
	 * points on the plane. This function doesn't check if the given point
	 * is on the plane.
	 *
	 * @param surfacePt Surface point at which to compute surface normal,
	 *   though it's not used.
	 *
	 * @return Surface normal at @c surfacePt .
	 */
	mvector<vec_T, dim> surfaceNorm(
			const mvector<vec_T, dim> &surfacePt) const {
		return surfNorm.norm();
	}

	// colorAt doesn't have an override yet.

	/**
	 * Partially overrides the @c shape  @c printHelper
	 * function by first calling that function then printing more information
	 * about this object. Namely this plane's distance from the origin and the
	 * surface normal.
	 *
	 * @param os Output stream to which to write a string representation of
	 *   this object.
	 */
	void printHelper(std::ostream &os) const{
		shape<vec_T, color_T, time_T, dim>::printHelper(os);
		os << " ---> [infinite plane. dist from origin: " << dist <<
				", surface normal: " << surfNorm << "]";
	}
};

typedef infplane<double, double, double, 3> infplaned;
typedef infplane<double, double, float, 3> infplaneddf;
typedef infplane<float, float, float, 3> infplanef;

#endif // INFPLANE_HH
