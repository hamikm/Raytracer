/**
 * @file
 * @author Hamik Mukelyan
 */

#include <iostream>
#include <cassert>
#include <cmath>

#ifndef MVECTOR_HH
#define MVECTOR_HH

/**
 * Represents math vectors of given size and type. Basic vector
 * operations such as vector addition, scalar multiplication, dot products,
 * component access, etc. are supported.
 *
 * @tparam T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam size The number of dimensions, which must be at least 1 but is
 *   likely 2 or 3.
 */
template<typename T, int size>
class mvector {
private:
	/**
	 *  Contains vector data. The i-th component of the vector corresponds
	 *  to v[i-1].
	 */
	T v[size];

public:

	/**
	 * Constructs the zero vector.
	 */
	mvector () {
		assert(size > 0);
		for (int i = 0; i < size; i++)
			v[i] = 0;
	}

	/**
	 * Constructs the vector specified by the given array.
	 *
	 * @param arr Array from which to construct this vector.
	 *
	 * @warning Length of the array must be the same as the dimension of this
	 *   vetor.
	 */
	mvector (const T arr[size]) {
		assert(size > 0);
		for (int i = 0; i < size; i++)
			v[i] = arr[i];
	}

	/**
	 * Convenience constructor so that 2D double mvectors don't have to be
	 * constructed with arrays.
	 *
	 * @param x
	 * @param y
	 */
	mvector (double x, double y) {
		assert(size == 2);
		v[0] = x;
		v[1] = y;
	}

	/**
	 * Convenience constructor so that 3D double mvectors don't have to be
	 * constructed with arrays.
	 *
	 * @param x
	 * @param y
	 * @param z
	 */
	mvector (double x, double y, double z) {
		assert(size == 3);
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	/**
	 * Convenience constructor so that 2D float mvectors don't have to be
	 * constructed with arrays.
	 *
	 * @param x
	 * @param y
	 */
	mvector (float x, float y) {
		assert(size == 2);
		v[0] = x;
		v[1] = y;
	}

	/**
	 * Convenience constructor so that 3D float mvectors don't have to be
	 * constructed with arrays.
	 *
	 * @param x
	 * @param y
	 * @param z
	 */
	mvector (float x, float y, float z) {
		assert(size == 3);
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	/**
	 * Copy constructor.
	 *
	 * @param other
	 */
	mvector (const mvector<T, size> &other) {
		assert (size > 0);
		assert (other.dim() == size);
		for (int i = 0; i < size; i++)
			v[i] = other[i];
	}

	/**
	 * Adds this vector to the argument then assigns the sum to this vector.
	 *
	 * @param rhs
	 *
	 * @return Reference to this vector for operator chaining.
	 */
	mvector & operator+=(const mvector &rhs) {
		for (int i = 0; i < size; i++)
			v[i] += rhs[i];
		return *this;
	}

	/**
	 * Takes difference of this vector and the given vector then assigns the
	 * difference to this vector.
	 *
	 * @param rhs
	 *
	 * @return Reference to this vector for operator chaining.
	 */
	mvector & operator-=(const mvector &rhs) {
		for (int i = 0; i < size; i++)
			v[i] -= rhs[i];
		return *this;
	}

	/**
	 * Takes product of this vector and the given scalar then assigns the
	 * product to this vector.
	 *
	 * @param scalar
	 *
	 * @return Reference to this vector for operator chaining.
	 */
	mvector & operator*=(T scalar) {
		for (int i = 0; i < size; i++)
			v[i] *= scalar;
		return *this;
	}

	/**
	 * Divides this vector by the given scalar and assigns the result to
	 * this vector.
	 *
	 * @param scalar
	 *
	 * @return Reference to this vector for operator chaining.
	 */
	mvector & operator/=(T scalar) {
		assert(scalar != 0);
		for (int i = 0; i < size; i++)
			v[i] /= scalar;
		return *this;
	}

	/**
	 * Adds the given vector to this vector.
	 *
	 * @param rhs
	 *
	 * @return The sum as a const value.
	 */
	const mvector operator+(const mvector &rhs) const {
		mvector tmp = *this;
		for (int i = 0; i < size; i++)
			tmp[i] += rhs[i];
		return tmp;
	}

	/**
	 * Takes difference of this vector and given vector.
	 *
	 * @param rhs
	 *
	 * @return The difference as a const value.
	 */
	const mvector operator-(const mvector &rhs) const {
		mvector tmp = *this;
		for (int i = 0; i < size; i++)
			tmp[i] -= rhs[i];
		return tmp;
	}

	/**
	 * Divides this vector by the given scalar.
	 *
	 * @param scalar
	 *
	 * @return The quotient as a const value.
	 */
	const mvector operator/(T scalar) const {
		assert(scalar != 0);
		mvector tmp = *this;
		for (int i = 0; i < size; i++)
			tmp[i] /= scalar;
		return tmp;
	}

	/**
	 * Negates this vector (unary minus).
	 *
	 * @return The negative of this vector as a const value.
	 *
	 * @warning This is not a mutator.
	 */
	const mvector operator-() const {
		mvector tmp = *this;
		for (int i = 0; i < size; i++)
			tmp[i] *= -1;
		return tmp;
	}

	/**
	 * Takes dot product of this vector and the given vector.
	 *
	 * @param rhs
	 *
	 * @return Dot product.
	 */
	T operator*(const mvector &rhs) const {
		T result = 0;
		for (int i = 0; i < size; i++)
			result += rhs[i] * v[i];
		return result;
	}

	/**
	 * Takes cross product of this vector and given vector. Both vectors must
	 * have three dimensions; if not an assertion is triggered.
	 *
	 * @param rhs
	 *
	 * @return Cross product as a const value.
	 */
	const mvector operator%(const mvector &rhs) const {
		assert(size == 3);
		assert(rhs.dim() == 3);
		mvector tmp;
		tmp[0] = v[1] * rhs[2] - v[2] * rhs[1];
		tmp[1] = -v[0] * rhs[2] + v[2] * rhs[0];
		tmp[2] = v[0] * rhs[1] - v[1] * rhs[0];
		return tmp;
	}

	/**
	 * Intended for vector component access on the left-hand side of
	 * assignment statements.
	 *
	 * @param index
	 *
	 * @return Reference to the requested component.
	 */
    T & operator[](int index) {
    	assert(index >= 0);
    	assert(index < size);
    	return v[index];
    }

    /**
	 * Intended for vector component access on the right-hand side of
	 * statements.
	 *
	 * @param index
	 *
	 * @return Requested component as a value.
	 */
    T operator[](int index) const {
    	assert(index >= 0);
    	assert(index < size);
    	return v[index];
    }

    /**
     * Gets squared magnitude of this vector.
     *
     * @return Squared magnitude
     */
    T magsq() const { return (*this) * (*this); }

    /**
     * Gets magnitude of this vector.
     *
     * @return Magnitude
     */
    T mag() const { return sqrt(magsq()); }

    /**
     * Gets a normalized version of this vector as a brand-new value.
     *
     * @return Normalized vector.
     */
    mvector norm() const {
    	mvector vec(*this);
    	T magnitude = mag();
    	for (int i = 0; i < size; i++)
    		vec[i] /= magnitude;
    	return vec;
    }

    /**
     * Helper function for output streams. Prints this vector in the format
     * @f$(v_1, v_2, \cdots , v_n)@f$.
     *
     * @param os The output stream to which to print.
     */
    void print(std::ostream &os) const {
    	int i = 0;
    	os << "<";
    	do {
    		os << v[i];
    		if (i + 1 != size)
    			os << ", ";
    	} while (++i < size);
    	os << ">";
    }

    /**
     * Gets number of dimensions in this vector.
     *
     * @return Number of dimensions.
     */
    int dim() const {
    	return size;
    }
};

/**
 * Scalar multiplication of a vector (left side) with a scalar (right side).
 *
 * @param lhs Vector on the left-hand side of the operator.
 * @param scalar Scalar on the right-hand side of the operator.
 *
 * @return Product as a const value.
 */
template<typename T, int size>
const mvector<T, size> operator*(const mvector<T, size> &lhs, T scalar) {
	mvector<T, size> tmp = lhs;
	for (int i = 0; i < lhs.dim(); i++)
		tmp[i] *= scalar;
	return tmp;
}

/**
 * Scalar multiplication of a scalar (left side) with a vector (right side).
 *
 * @param scalar Scalar on the left-hand side of the operator.
 * @param rhs Vector on the right-hand side of the operator.
 *
 * @return Product as a const value.
 */
template<typename T, int size>
const mvector<T, size> operator*(T scalar, const mvector<T, size> &rhs) {
	mvector<T, size> tmp = rhs;
	for (int i = 0; i < rhs.dim(); i++)
		tmp[i] *= scalar;
	return tmp;
}

/**
 * Prints the given vector to the given output stream via an output operator
 * overload.
 *
 * @param os The output stream to which to print.
 * @param vec The vector to print to the output stream.
 *
 * @return Reference to the given output stream for operator chaining.
 */
template<typename T, int size>
std::ostream & operator<<(std::ostream& os, const mvector<T, size> &vec) {
	vec.print(os);
	return os;
}

/**
 * Makes the input operator compatible with input formatted like
 * <x_1, ..., x_n>, where x_1, x_2, and so forth are coordinates and where
 * whitespace doesn't matter. Note that this function works with vectors
 * of any type or dimension as long as the input operator >> works on the
 * type.
 *
 * @param is The input stream from which to read.
 * @param v The vector into which we'll read.
 *
 * @return The same input stream for operator chaining.
 *
 * @throw ios_base::failure If input doesn't conform to <x_1, ..., x_n>
 */
template<typename T, int size>
std::istream & operator>>(std::istream& is, mvector<T, size> &v) {

	if(!is) { // If not ready...
		return is; // ...already failed, just return.
	}

	// Configure istream to throw an exception if the failbit gets set below.
	is.exceptions(std::ios_base::failbit);

	// Otherwise get the first char.
	char c;
	is >> c;
	if (c == '<') { // If it's the start of a vector, then...

		assert(size > 0);
		int i = 0;
		do {
			is >> v[i] >> c;
			if (c != ',') { // If wasn't a comma after coordinate, bad format.
				is.clear(std::ios_base::failbit);
			}
			i++;
		} while (i < size - 1);

		// When we reach here we have one coordinate left to read.
		assert(i == size - 1);
		is >> v[size - 1] >> c;
		if (c != '>') { // If no closing bracket, then bad format.
			is.clear(std::ios_base::failbit);
		}

	}
	else { // If first char wasn't <, put it back into the stream and return.
		is.putback(c);
		return is;
	}

	return is;
}

typedef mvector<double, 3> vector3d;
typedef mvector<float, 3> vector3f;
typedef mvector<double, 2> vector2d;
typedef mvector<float, 2> vector2f;

#endif // MVECTOR_HH
