/**
 * @file
 * @author Hamik Mukelyan
 */

#include "mvector.hh"
#include "gtest/gtest.h"
#include <cmath>
#include <sstream>
#include <string>

#ifndef TEST_MVECTOR_CC
#define TEST_MVECTOR_CC

/**
 * This test fixture class sets up a few persistent vectors that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class mvectorTest : public ::testing::Test {
protected:
	vector3d a, b, c, d;
	vector2f e, f;

	/*
	 * This setup function assumes that vector component access with reference
	 * works.
	 */
	virtual void SetUp() {
		a[0] = 2; b[0] = 0; c[0] = 0; d[0] = 2;
		a[1] = 0; b[1] = 3; c[1] = 0; d[1] = 0;
		a[2] = 0; b[2] = 0; c[2] = 4; d[2] = -3;

		e[0] = 3; f[0] = -2;
		e[1] = 5; f[1] = 7;
	}
};

/*
 * Exercises the bracket operator that doesn't return a reference--that is,
 * the one that is used on the right hand side (RHS) of assignment statements.
 * This is the first test because the RHS bracket operator is used in most
 * of the tests below.
 */
TEST_F(mvectorTest, ElementAccessWithoutReference) {
	ASSERT_DOUBLE_EQ(2, a[0]);
	ASSERT_DOUBLE_EQ(0, a[1]);
	ASSERT_DOUBLE_EQ(0, a[2]);
}

/*
 * Exercises the default constructor mvector() by making a 4D vector and a
 * 2D vector then checking to see that the components of both are all zero.
 * Assumes that vector component access without reference works.
 */
TEST_F(mvectorTest, DefaultConstructor) {
	mvector<double, 4> v;
	ASSERT_DOUBLE_EQ(0, v[0]);
	ASSERT_DOUBLE_EQ(0, v[1]);
	ASSERT_DOUBLE_EQ(0, v[2]);
	ASSERT_DOUBLE_EQ(0, v[3]);

	mvector<int, 4> u;
	ASSERT_EQ(0, u[0]);
	ASSERT_EQ(0, u[1]);
}

/*
 * Exercises the mvector (const T arr[size]) constructor by making a 4D vector
 * and a 2D vector then checking to see that their components correctly
 * initialized. Assumes that vector component access without reference works.
 */
TEST_F(mvectorTest, ArrayConstructor) {
	double init_arr1[] = {1, 2, 3, 4};
	mvector<double, 4> v(init_arr1);
	ASSERT_DOUBLE_EQ(1, v[0]);
	ASSERT_DOUBLE_EQ(2, v[1]);
	ASSERT_DOUBLE_EQ(3, v[2]);
	ASSERT_DOUBLE_EQ(4, v[3]);

	int init_arr2[] = {1, 2};
	mvector<int, 4> u(init_arr2);
	ASSERT_EQ(1, u[0]);
	ASSERT_EQ(2, u[1]);
}

/*
 * Exercises the copy constructor mvector(const mvector&) by constructing a new
 * 3D vector from an old one. Does the same thing with a 2D vector.
 * Also tests that assignment operator works and handles self-assignment.
 * Assumes that vector component access without reference works.
 */
TEST_F(mvectorTest, CopyConstructorAndAssignmentOperator) {
	vector3d v(d);
	ASSERT_DOUBLE_EQ(2, v[0]);
	ASSERT_DOUBLE_EQ(0, v[1]);
	ASSERT_DOUBLE_EQ(-3, v[2]);

	v = a;
	ASSERT_DOUBLE_EQ(2, v[0]);
	ASSERT_DOUBLE_EQ(0, v[1]);
	ASSERT_DOUBLE_EQ(0, v[2]);

	vector2f u(e);
	ASSERT_FLOAT_EQ(3, u[0]);
	ASSERT_FLOAT_EQ(5, u[1]);

	u = f;
	ASSERT_FLOAT_EQ(-2, u[0]);
	ASSERT_FLOAT_EQ(7, u[1]);

	vector3d *ptr_to_v = &v;
	v = *ptr_to_v; // this is self-assignment
	ASSERT_DOUBLE_EQ(2, v[0]);
	ASSERT_DOUBLE_EQ(0, v[1]);
	ASSERT_DOUBLE_EQ(0, v[2]);
}

/*
 * Exercises the dimension getter int dim().
 */
TEST_F(mvectorTest, Dim) {
	mvector<double,7> v;
	ASSERT_EQ(7, v.dim());
	ASSERT_EQ(3, a.dim());
	ASSERT_EQ(2, e.dim());
}

/*
 * Exercises the magnitude getter T mag().
 */
TEST_F(mvectorTest, Mag) {
	EXPECT_DOUBLE_EQ(2, a.mag());
	EXPECT_DOUBLE_EQ(sqrt(13), d.mag());
	EXPECT_FLOAT_EQ(sqrt(34), e.mag());
}

/*
 * Exercises the T magsq() getter.
 */
TEST_F(mvectorTest, Magsq) {
	ASSERT_DOUBLE_EQ(4, a.magsq());
	ASSERT_DOUBLE_EQ(13, d.magsq());
	ASSERT_FLOAT_EQ(34, e.magsq());
}

/*
 * Exercises the mvector norm() function by calling it on some vectors and
 * checking that properly normalized vectors are returned. Assumes that vector
 * component access without reference works.
 */
TEST_F(mvectorTest, Norm) {
	EXPECT_DOUBLE_EQ(1, a.norm()[0]);
	EXPECT_DOUBLE_EQ(0, a.norm()[1]);
	EXPECT_DOUBLE_EQ(0, a.norm()[2]);

	EXPECT_DOUBLE_EQ(2 / sqrt(13), d.norm()[0]);
	EXPECT_DOUBLE_EQ(0, d.norm()[1]);
	EXPECT_DOUBLE_EQ(-3 / sqrt(13), d.norm()[2]);

	EXPECT_FLOAT_EQ(3 / sqrt(34), e.norm()[0]);
	EXPECT_FLOAT_EQ(5 / sqrt(34), e.norm()[1]);
}

/*
 * Exercises binary minus function. Assumes that vector component access
 * without reference works.
 */
TEST_F(mvectorTest, BinaryMinus) {
	EXPECT_DOUBLE_EQ(-2, (b - d)[0]);
	EXPECT_DOUBLE_EQ(3, (b - d)[1]);
	EXPECT_DOUBLE_EQ(3, (b - d)[2]);

	EXPECT_FLOAT_EQ(5, (e - f)[0]);
	EXPECT_FLOAT_EQ(-2, (e - f)[1]);
}

/*
 * Exercises unary minus function. Assumes that vector component access
 * without reference works.
 */
TEST_F(mvectorTest, UnaryMinus) {
	EXPECT_DOUBLE_EQ(-2, (-d)[0]);
	EXPECT_DOUBLE_EQ(0, (-d)[1]);
	EXPECT_DOUBLE_EQ(3, (-d)[2]);

	EXPECT_FLOAT_EQ(-3, (-e)[0]);
	EXPECT_FLOAT_EQ(-5, (-e)[1]);
}

/*
 * Exercises divide by scalar function. Assumes copy constructor works. Assumes
 * that vector component access without reference works.
 */
TEST_F(mvectorTest, DivideByScalar) {
	vector3d v = d / 2;
	EXPECT_DOUBLE_EQ(1, v[0]);
	EXPECT_DOUBLE_EQ(0, v[1]);
	EXPECT_DOUBLE_EQ(-1.5, v[2]);

	vector2f u = e / 0.5;
	EXPECT_FLOAT_EQ(6, u[0]);
	EXPECT_FLOAT_EQ(10, u[1]);
}

/*
 * Exercises /= operator.Assumes that vector component access without reference
 * works.
 */
TEST_F(mvectorTest, DivideEqual) {
	d /= 2;
	ASSERT_DOUBLE_EQ(1, d[0]);
	ASSERT_DOUBLE_EQ(0, d[1]);
	ASSERT_DOUBLE_EQ(-1.5, d[2]);

	e /= 0.5;
	ASSERT_FLOAT_EQ(6, e[0]);
	ASSERT_FLOAT_EQ(10, e[1]);
}

/*
 * Exercises the [] operator used on the left hand side of assignment
 * statements. Assumes that vector component access without reference works.
 */
TEST_F(mvectorTest, ElementAccessWithReference) {
	a[0] = 7;
	a[2] = 13;
	ASSERT_DOUBLE_EQ(7, a[0]);
	ASSERT_DOUBLE_EQ(0, a[1]);
	ASSERT_DOUBLE_EQ(13, a[2]);
}

/*
 * Exercises the dot product operator, which is * with an mvector on each
 * side.
 */
TEST_F(mvectorTest, DotProductTest) {
	EXPECT_DOUBLE_EQ(4, a * d);
	EXPECT_DOUBLE_EQ(13, d * d);
	EXPECT_DOUBLE_EQ(-12, d * c);

	EXPECT_FLOAT_EQ(29, f * e);
	EXPECT_FLOAT_EQ(34, e * e);
}

/*
 * Exercises the *= operator (with a scalar on the right hand side).
 * Assumes that vector component access without reference works.
 */
TEST_F(mvectorTest, TimesEqual) {
	d *= 2;
	ASSERT_DOUBLE_EQ(4, d[0]);
	ASSERT_DOUBLE_EQ(0, d[1]);
	ASSERT_DOUBLE_EQ(-6, d[2]);

	e *= 0.5;
	ASSERT_FLOAT_EQ(1.5, e[0]);
	ASSERT_FLOAT_EQ(2.5, e[1]);
}

/*
 * Exercises the cross product operator. Note that it's illegal to take the
 * cross product of two 2D vectors.
 */
TEST_F(mvectorTest, CrossProduct) {
	EXPECT_DOUBLE_EQ(0, (a % d)[0]);
	EXPECT_DOUBLE_EQ(6, (a % d)[1]);
	EXPECT_DOUBLE_EQ(0, (a % d)[2]);
}

/*
 * Exercises + operator. Assumes that vector component access without
 * reference works.
 */
TEST_F(mvectorTest, Plus) {
	EXPECT_DOUBLE_EQ(4, (a + d)[0]);
	EXPECT_DOUBLE_EQ(0, (a + d)[1]);
	EXPECT_DOUBLE_EQ(-3, (a + d)[2]);

	EXPECT_FLOAT_EQ(1, (e + f)[0]);
	EXPECT_FLOAT_EQ(12, (e + f)[1]);
}

/*
 * Exercises += operator. Assumes that vector component access without
 * reference works.
 */
TEST_F(mvectorTest, PlusEqual) {
	a += d;
	ASSERT_DOUBLE_EQ(4, a[0]);
	ASSERT_DOUBLE_EQ(0, a[1]);
	ASSERT_DOUBLE_EQ(-3, a[2]);

	e += f;
	ASSERT_FLOAT_EQ(1, e[0]);
	ASSERT_FLOAT_EQ(12, e[1]);
}

/*
 * Exercise -= operator.
 */
TEST_F(mvectorTest, MinusEqual) {
	b -= d;
	ASSERT_DOUBLE_EQ(-2, b[0]);
	ASSERT_DOUBLE_EQ(3, b[1]);
	ASSERT_DOUBLE_EQ(3, b[2]);

	e -= f;
	ASSERT_FLOAT_EQ(5, e[0]);
	ASSERT_FLOAT_EQ(-2, e[1]);
}

/*
 * Exercises the scalar multiplication operation * (scalar on right side).
 */
TEST_F(mvectorTest, ScalarOnRightTimes) {
	ASSERT_DOUBLE_EQ(10, (a * 5.0)[0]);
	ASSERT_DOUBLE_EQ(0, (a * 5.0)[1]);
	ASSERT_DOUBLE_EQ(0, (a * 5.0)[2]);

	ASSERT_FLOAT_EQ(-15, (e * -5.0f)[0]);
	ASSERT_FLOAT_EQ(-25, (e * -5.0f)[1]);
}

/*
 * Exercises the scalar multiplication operation * (scalar on left side).
 */
TEST_F(mvectorTest, ScalarOnLeftTimes) {
	ASSERT_DOUBLE_EQ(10, (5.0 * a)[0]);
	ASSERT_DOUBLE_EQ(0, (5.0 * a)[1]);
	ASSERT_DOUBLE_EQ(0, (5.0 * a)[2]);

	ASSERT_FLOAT_EQ(-15, (-5.0f * e)[0]);
	ASSERT_FLOAT_EQ(-25, (-5.0f * e)[1]);
}

/*
 * Exercises the << operator.
 */
TEST_F(mvectorTest, Print) {
	std::stringstream s;
	s << a;
	char cstring[100];
	s.getline(cstring, 100);
	ASSERT_STREQ("<2, 0, 0>", cstring);
}

/*
 * Exercises the input operator.
 */
TEST_F(mvectorTest, Input) {

	// Test input of 3-vector with ints.
	std::string strvec = "<1, 2, 3>";
	std::stringstream iss(strvec);
	std::stringstream oss;
	mvector<int, 3> itestvec;
	iss >> itestvec;
	oss << itestvec;
	ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
	ASSERT_EQ(strvec.length(), iss.tellg()); // assert input was consumed

	// Test input of 3-vector with doubles.
	strvec = "<1.5, 2.5, 3.5>";
	iss.str(strvec); // overwrite the stringstream with new contents
	oss.str(""); // overwrite the stringstream with empty string
	vector3d dtestvec1;
	iss >> dtestvec1;
	oss << dtestvec1;
	ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
	ASSERT_EQ(strvec.length(), iss.tellg()); // assert input was consumed

	// Test input of 5-vector with doubles.
	strvec = "<1.5, 2.5, 3.5, 4.5, 5.5>";
	iss.str(strvec);
	oss.str("");
	mvector<double, 5> dtestvec2;
	iss >> dtestvec2;
	oss << dtestvec2;
	ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
	ASSERT_EQ(strvec.length(), iss.tellg()); // assert input was consumed

	// Test that starting without < doesn't consume input and no exception.
	strvec = "(1, 2, 3)";
	iss.str(strvec);
	oss.str("");
	iss >> itestvec; // should do nothing. iss should be unmodified.
	ASSERT_STREQ("", oss.str().c_str());
	ASSERT_EQ(0, iss.tellg()); // assert NO INPUT was consumed

	// Test that bad internal formatting causes exception.
	strvec = "<2 , 2 2>"; // bad formatting... should throw exception on input
	iss.str(strvec);
	oss.str("");
	ASSERT_THROW(iss >> itestvec, std::ios_base::failure);
	iss.clear(); // reset error flags

	// Test that not ending with a > causes exception.
	strvec = "<2 , 2 2)"; // bad formatting... should throw exception on input
	iss.str(strvec);
	oss.str("");
	ASSERT_THROW(iss >> itestvec, std::ios_base::failure);
	iss.clear(); // reset error flags

	// Test that whitespace is handled correctly.
	strvec = "  <    1.5  , 2.5,3.5    >    f";
	std::string real = "<1.5, 2.5, 3.5>";
	iss.str(strvec); // overwrite the stringstream with new contents
	oss.str(""); // overwrite the stringstream with empty string
	iss >> dtestvec1;
	oss << dtestvec1;
	ASSERT_STREQ(real.c_str(), oss.str().c_str());
	// Check that vector was consumed by checking if f is the next thing read.
	char c;
	iss >> c;
	ASSERT_EQ('f', c);
}

#endif // TEST_MVECTOR_CC
