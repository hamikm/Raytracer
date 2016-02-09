/**
 * @file
 * @author Hamik Mukelyan
 */

#include <cassert>
#include <iostream>

#ifndef RGBCOLOR_HH
#define RGBCOLOR_HH

/**
 * Represents red green blue (RGB) colors. It has getters and setters for each
 * color  and supports addition, multiplication, and subtraction of colors as
 * well as scalar multiplication and division of colors. There is output stream
 * support.
 *
 * @tparam T The type of the components in this color object. Many
 *   arithmetic and relational operators must be supported, including real
 *   number ones. The type will likely be double or float.
 */
template<typename T>
class rgbcolor {
private:
	/** Red component. */
	T r;

	/** Green component */
	T g;

	/** Blue component. */
	T b;

public:

	/**
	 * Constructs a black color.
	 */
	rgbcolor() {
		r = 0;
		g = 0;
		b = 0;
	}

	/**
	 * Constructs a color with the given RGB values. Color values must be
	 * between 0 and 1, inclusive.
	 *
	 * @param r Red value.
	 * @param g Green value.
	 * @param b Blue value.
	 */
	rgbcolor(T r, T g, T b) {
		assert(r >= 0 && r <= 1);
		assert(g >= 0 && g <= 1);
		assert(b >= 0 && b <= 1);
		this->r = r;
		this->g = g;
		this->b = b;
	}

	/**
	 * Gets blue component.
	 *
	 * @return Blue value.
	 */
	T getB() const {
		return b;
	}

	/**
	 * Sets blue component.
	 *
	 * @param b New blue value.
	 */
	void setB(T b) {
		assert(b >= 0 && b <= 1);
		this->b = b;
	}

	/**
	 * Gets green component.
	 *
	 * @return Green value.
	 */
	T getG() const {
		return g;
	}

	/**
	 * Sets green component.
	 *
	 * @param g New green value.
	 */
	void setG(T g) {
		assert(g >= 0 && g <= 1);
		this->g = g;
	}

	/**
	 * Gets Red component.
	 *
	 * @return Red value.
	 */
	T getR() const {
		return r;
	}

	/**
	 * Sets red component.
	 *
	 * @param r New red value.
	 */
	void setR(T r) {
		assert(r >= 0 && r <= 1);
		this->r = r;
	}

	/**
	 * Adds this color to the argument component-wise then assigns the sum to
	 * this color.
	 *
	 * @param rhs Color to add to this one.
	 *
	 * @return Reference to this color for operator chaining.
	 */
	rgbcolor & operator+=(const rgbcolor &rhs) {
		r += rhs.getR();
		g += rhs.getG();
		b += rhs.getB();
		return *this;
	}

	/**
	 * Takes difference of this color and the given color then assigns the
	 * difference to this color.
	 *
	 * @param rhs
	 *
	 * @return Reference to this color for operator chaining.
	 */
	rgbcolor & operator-=(const rgbcolor &rhs) {
		r -= rhs.getR();
		g -= rhs.getG();
		b -= rhs.getB();
		return *this;
	}

	/**
	 * Takes product of this color and the given color then assigns the
	 * product to this color.
	 *
	 * @param rhs
	 *
	 * @return Reference to this color for operator chaining.
	 */
	rgbcolor & operator*=(const rgbcolor &rhs) {
		r *= rhs.getR();
		g *= rhs.getG();
		b *= rhs.getB();
		return *this;
	}

	/**
	 * Adds given color to this color.
	 *
	 * @param rhs
	 *
	 * @return Sum as a const value.
	 */
	const rgbcolor operator+(const rgbcolor &rhs) const {
		return rgbcolor(*this) += rhs;
	}

	/**
	 * Takes difference of this color and given color.
	 *
	 * @param rhs
	 *
	 * @return Difference as a const value.
	 */
	const rgbcolor operator-(const rgbcolor &rhs) const {
		return rgbcolor(*this) -= rhs;
	}

	/**
	 * Takes difference of this color and given color.
	 *
	 * @param rhs
	 *
	 * @return Difference as a const value.
	 */
	const rgbcolor operator*(const rgbcolor &rhs) const {
		return rgbcolor(*this) *= rhs;
	}

	/**
	 * Divides this color by the given scalar and assigns the result to
	 * this color.
	 *
	 * @param scalar
	 *
	 * @return Reference to this color for opeator chaining.
	 */
	rgbcolor & operator/=(T scalar) {
		assert(scalar != 0);
		r /= scalar;
		g /= scalar;
		b /= scalar;
		return *this;
	}

	/**
	 * Multiplies this color by the given scalar and assigns the result to
	 * this color.
	 *
	 * @param scalar
	 *
	 * @return Reference to this color after assignment for operator chaining.
	 */
	rgbcolor & operator*=(T scalar) {
		r *= scalar;
		g *= scalar;
		b *= scalar;
		return *this;
	}

	/**
	 * Takes quotient of this color and given scalar.
	 *
	 * @param scalar
	 *
	 * @return Quotient as a const value.
	 */
	const rgbcolor operator/(T scalar) const {
		assert(scalar != 0);
		return rgbcolor(*this) /= scalar;
	}

	/**
	 * Clamps RGB values to the given range.
	 * Sets RGB values to @c minv if they're less than @c minv and @c maxv
	 * if they're greater than @c maxv.
	 *
	 * @param minv Lower bound for color component values.
	 * @param maxv Upper bound for color component values.
	 */
	void clamp(T minv, T maxv) {
		assert(maxv > minv);
		if(r < minv) {
			r = minv;
		}
		if(g < minv) {
			g = minv;
		}
		if(b < minv) {
			b = minv;
		}
		if(r > maxv) {
			r = maxv;
		}
		if(g > maxv) {
			g = maxv;
		}
		if(b > maxv) {
			b = maxv;
		}
	}

	/**
	 * Print helper function. Prints this vector's color components in the
	 * format (r, g, b) to the given output stream.
	 *
	 * @param os Prints to this output stream.
	 */
    void print(std::ostream &os) const {
		os << "(" << r << ", " << g << ", " << b << ")";
	}
};

/**
 * Scalar multiplication of a color (left side) with a scalar (right side).
 *
 * @param lhs Vector on the left-hand side of the operator
 * @param scalar Scalar on the right-hand side of the operator
 *
 * @return Product as a const value.
 */
template<typename T>
const rgbcolor<T> operator*(const rgbcolor<T> &lhs, T scalar) {
	return rgbcolor<T>(lhs) *= scalar;
}

/**
 * Scalar multiplication of a scalar (left side) with a color (right side).
 *
 * @param scalar Scalar on the left-hand side of the operator.
 * @param rhs Vector on the righ-hand side of the operator.
 *
 * @return Product as a const value.
 */
template<typename T>
const rgbcolor<T> operator*(T scalar, const rgbcolor<T> &rhs) {
	return rgbcolor<T>(rhs) *= scalar;
}

/**
 * Prints the given color to the given output stream.
 *
 * @param os Output stream to which to print.
 * @param color The color to print to the given output stream.
 *
 * @return Reference to the given output stream for operator chaining.
 */
template<typename T>
std::ostream & operator<<(std::ostream &os, const rgbcolor<T> &color) {
	color.print(os);
	return os;
}

/**
 * Makes the input operator compatible with input formatted like
 * (r, g, b), where r, g, and b are colors and where
 * whitespace doesn't matter.
 *
 * @param is The input stream from which to read.
 * @param col The rgbcolor into which we'll read.
 *
 * @return The same input stream for operator chaining.
 *
 * @throw ios_base::failure If input doesn't conform to (r, g, b) format.
 */
template<typename T>
std::istream & operator>>(std::istream& is, rgbcolor<T> &col) {

	if(!is) { // If not ready...
		return is; // ...already failed, just return.
	}

	// Configure istream to throw an exception if the failbit gets set below.
	is.exceptions(std::ios_base::failbit);

	// Otherwise get the first char.
	char c;
	is >> c;
	if (c == '(') { // If it's the start of a color, then...

		// Set red component.
		T tmpcol;
		is >> tmpcol >> c;
		if (c != ',') { // If wasn't a comma after color, bad format.
			is.clear(std::ios_base::failbit);
		}
		col.setR(tmpcol);

		// Set green component.
		is >> tmpcol >> c;
		if (c != ',') { // If wasn't a comma after color, bad format.
			is.clear(std::ios_base::failbit);
		}
		col.setG(tmpcol);

		// Set red component.
		is >> tmpcol >> c;
		if (c != ')') { // If wasn't a comma after color, bad format.
			is.clear(std::ios_base::failbit);
		}
		col.setB(tmpcol);
	}
	else { // If first char wasn't (, put it back into the stream and return.
		is.putback(c);
		return is;
	}

	return is;
}

typedef rgbcolor<float> rgbcolorf;
typedef rgbcolor<double> rgbcolord;

#endif // RGBCOLOR_HH
