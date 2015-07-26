/**
 * @file
 * @author Hamik Mukelyan
 */

#include "rgbcolor.hh"
#include "gtest/gtest.h"
#include <sstream>

#ifndef TEST_RGBCOLOR_CC
#define TEST_RGBCOLOR_CC

/**
 * This test fixture class sets up a few persistent colors that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class rgbcolorTest : public ::testing::Test {
protected:

	rgbcolord a, b, e;
	rgbcolorf c, d, f;

	virtual void SetUp() {
		b.setR(0.1);
		b.setG(0.2);
		b.setB(0.3);

		d.setR(0.1f);
		d.setG(0.3f);
		d.setB(0.5f);

		e.setR(0.3);
		e.setG(0.7);
		e.setB(0.5);

		f.setR(0.3f);
		f.setG(0.7f);
		f.setB(0.5f);
	}
};

/*
 * Exercises the default constructor rgbcolor().
 */
TEST_F(rgbcolorTest, DefaultConstructor) {
	ASSERT_DOUBLE_EQ(0, a.getR());
	ASSERT_DOUBLE_EQ(0, a.getG());
	ASSERT_DOUBLE_EQ(0, a.getB());

	ASSERT_FLOAT_EQ(0, c.getR());
	ASSERT_FLOAT_EQ(0, c.getG());
	ASSERT_FLOAT_EQ(0, c.getB());
}

/*
 * Exercises the rgbcolor (T, T, T) constructor.
 */
TEST_F(rgbcolorTest, NondefaultConstructor) {
	ASSERT_DOUBLE_EQ(0.1, b.getR());
	ASSERT_DOUBLE_EQ(0.2, b.getG());
	ASSERT_DOUBLE_EQ(0.3, b.getB());

	ASSERT_FLOAT_EQ(0.1, d.getR());
	ASSERT_FLOAT_EQ(0.3, d.getG());
	ASSERT_FLOAT_EQ(0.5, d.getB());
}

/*
 * Exercises the copy constructor rgbcolor(const rgbcolor&) as well as the
 * assignment operator.
 */
TEST_F(rgbcolorTest, CopyConstructorAndAssignmentOperator) {
	rgbcolor<double> v(a);
	ASSERT_DOUBLE_EQ(0, v.getR());
	ASSERT_DOUBLE_EQ(0, v.getG());
	ASSERT_DOUBLE_EQ(0, v.getB());

	rgbcolor<float> u(d);
	ASSERT_FLOAT_EQ(0.1, u.getR());
	ASSERT_FLOAT_EQ(0.3, u.getG());
	ASSERT_FLOAT_EQ(0.5, u.getB());

	v = b;
	ASSERT_DOUBLE_EQ(0.1, v.getR());
	ASSERT_DOUBLE_EQ(0.2, v.getG());
	ASSERT_DOUBLE_EQ(0.3, v.getB());

	u = c;
	ASSERT_FLOAT_EQ(0, u.getR());
	ASSERT_FLOAT_EQ(0, u.getG());
	ASSERT_FLOAT_EQ(0, u.getB());
}

/*
 * The getter and setter functions for the red, green, and blue color
 * components obviously work, so there are no rests for them.
 */

/*
 * Exercises binary minus function (unary minus, that is, negation, doesn't
 * make sense for colors).
 */
TEST_F(rgbcolorTest, BinaryMinus) {
	ASSERT_DOUBLE_EQ(0.2, (e - b).getR());
	ASSERT_DOUBLE_EQ(0.5, (e - b).getG());
	ASSERT_DOUBLE_EQ(0.2, (e - b).getB());

	ASSERT_DOUBLE_EQ(-0.2, (b - e).getR());
	ASSERT_DOUBLE_EQ(-0.5, (b - e).getG());
	ASSERT_DOUBLE_EQ(-0.2, (b - e).getB());

	ASSERT_FLOAT_EQ(-0.2, (d - f).getR());
	ASSERT_FLOAT_EQ(-0.4, (d - f).getG());
	ASSERT_FLOAT_EQ(0, (d - f).getB());

	ASSERT_FLOAT_EQ(0.2, (f - d).getR());
	ASSERT_FLOAT_EQ(0.4, (f - d).getG());
	ASSERT_FLOAT_EQ(0, (f - d).getB());
}

/*
 * Exercises divide by scalar function.
 */
TEST_F(rgbcolorTest, DivideByScalar) {
	ASSERT_DOUBLE_EQ(0.2, (b / 0.5).getR());
	ASSERT_DOUBLE_EQ(0.4, (b / 0.5).getG());
	ASSERT_DOUBLE_EQ(0.6, (b / 0.5).getB());

	ASSERT_FLOAT_EQ(0.15, (f / 2).getR());
	ASSERT_FLOAT_EQ(0.35, (f / 2).getG());
	ASSERT_FLOAT_EQ(0.25, (f / 2).getB());
}

/*
 * Exercises /= operator.
 */
TEST_F(rgbcolorTest, DivideEqual) {
	b /= 0.5;
	ASSERT_DOUBLE_EQ(0.2, b.getR());
	ASSERT_DOUBLE_EQ(0.4, b.getG());
	ASSERT_DOUBLE_EQ(0.6, b.getB());

	f /= 2;
	ASSERT_FLOAT_EQ(0.15, f.getR());
	ASSERT_FLOAT_EQ(0.35, f.getG());
	ASSERT_FLOAT_EQ(0.25, f.getB());
}

/*
 * Exercises the *= operator (with a scalar on the right hand side).
 */
TEST_F(rgbcolorTest, TimesEqual) {
	b *= 2;
	ASSERT_DOUBLE_EQ(0.2, b.getR());
	ASSERT_DOUBLE_EQ(0.4, b.getG());
	ASSERT_DOUBLE_EQ(0.6, b.getB());

	f *= 0.5;
	ASSERT_FLOAT_EQ(0.15, f.getR());
	ASSERT_FLOAT_EQ(0.35, f.getG());
	ASSERT_FLOAT_EQ(0.25, f.getB());
}

/*
 * Exercises + operator. Assumes that vector component access without
 * reference works.
 */
TEST_F(rgbcolorTest, Plus) {
	ASSERT_DOUBLE_EQ(0.4, (e + b).getR());
	ASSERT_DOUBLE_EQ(0.9, (e + b).getG());
	ASSERT_DOUBLE_EQ(0.8, (e + b).getB());

	ASSERT_DOUBLE_EQ(0.4, (b + e).getR());
	ASSERT_DOUBLE_EQ(0.9, (b + e).getG());
	ASSERT_DOUBLE_EQ(0.8, (b + e).getB());

	ASSERT_FLOAT_EQ(0.4, (d + f).getR());
	ASSERT_FLOAT_EQ(1, (d + f).getG());
	ASSERT_FLOAT_EQ(1, (d + f).getB());

	ASSERT_FLOAT_EQ(0.4, (f + d).getR());
	ASSERT_FLOAT_EQ(1, (f + d).getG());
	ASSERT_FLOAT_EQ(1, (f + d).getB());
}

/*
 * Exercises += operator. Assumes that vector component access without
 * reference works.
 */
TEST_F(rgbcolorTest, PlusEqual) {
	e += b;
	ASSERT_DOUBLE_EQ(0.4, e.getR());
	ASSERT_DOUBLE_EQ(0.9, e.getG());
	ASSERT_DOUBLE_EQ(0.8, e.getB());

	d += f;
	ASSERT_FLOAT_EQ(0.4, d.getR());
	ASSERT_FLOAT_EQ(1, d.getG());
	ASSERT_FLOAT_EQ(1, d.getB());
}

/*
 * Exercise -= operator.
 */
TEST_F(rgbcolorTest, MinusEqual) {
	e -= b;
	ASSERT_DOUBLE_EQ(0.2, e.getR());
	ASSERT_DOUBLE_EQ(0.5, e.getG());
	ASSERT_DOUBLE_EQ(0.2, e.getB());

	e += b; // assuming += works, set e back to what it was before e -= b
	b -= e;
	ASSERT_DOUBLE_EQ(-0.2, b.getR());
	ASSERT_DOUBLE_EQ(-0.5, b.getG());
	ASSERT_DOUBLE_EQ(-0.2, b.getB());

	d -= f;
	ASSERT_FLOAT_EQ(-0.2, d.getR());
	ASSERT_FLOAT_EQ(-0.4, d.getG());
	ASSERT_FLOAT_EQ(0, d.getB());

	d += f; // assuming += works, set d back to what it was before d -= f
	f -= d;
	ASSERT_FLOAT_EQ(0.2, f.getR());
	ASSERT_FLOAT_EQ(0.4, f.getG());
	ASSERT_FLOAT_EQ(0, f.getB());
}


/*
 * Exercises the scalar multiplication operation * (scalar on right side).
 */
TEST_F(rgbcolorTest, ScalarOnRightTimes) {
	ASSERT_DOUBLE_EQ(0.2, (b * 2.0).getR());
	ASSERT_DOUBLE_EQ(0.4, (b * 2.0).getG());
	ASSERT_DOUBLE_EQ(0.6, (b * 2.0).getB());

	ASSERT_FLOAT_EQ(0.15, (f * 0.5f).getR());
	ASSERT_FLOAT_EQ(0.35, (f * 0.5f).getG());
	ASSERT_FLOAT_EQ(0.25, (f * 0.5f).getB());
}

/*
 * Exercises the scalar multiplication operation * (scalar on left side).
 */
TEST_F(rgbcolorTest, ScalarOnLeftTimes) {
	ASSERT_DOUBLE_EQ(0.2, (2.0 * b).getR());
	ASSERT_DOUBLE_EQ(0.4, (2.0 * b).getG());
	ASSERT_DOUBLE_EQ(0.6, (2.0 * b).getB());

	ASSERT_FLOAT_EQ(0.15, (0.5f * f).getR());
	ASSERT_FLOAT_EQ(0.35, (0.5f * f).getG());
	ASSERT_FLOAT_EQ(0.25, (0.5f * f).getB());
}

/*
 * Exercises the << operator.
 */
TEST_F(rgbcolorTest, Print) {
	std::stringstream s;
	s << e;
	char cstring[100];
	s.getline(cstring, 100);
	ASSERT_STREQ("(0.3, 0.7, 0.5)", cstring);
}

#endif // TEST_RGBCOLOR_CC
