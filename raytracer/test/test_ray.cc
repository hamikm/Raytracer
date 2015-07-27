/**
 * @file
 * @author Hamik Mukelyan
 */

#include "ray.hh"
#include "mvector.hh"
#include "gtest/gtest.h"
#include <sstream>

#ifndef TEST_RAY_CC
#define TEST_RAY_CC

/**
 * This test fixture class sets up a few persistent rays that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class RayTest : public ::testing::Test {
protected:

	ray3d *a;
	ray2f *b, *c;

	virtual void SetUp() {
		double v1arr[] = {0, 0, 5};
		vector3d v1(v1arr);
		double v2arr[] = {0, 0, -2};
		vector3d v2(v2arr);
		a = new ray3d(v1arr, v2arr); // should normalize by default

		float v3arr[] = {0, 5};
		vector2f v3(v3arr);
		float v4arr[] = {0, -2};
		vector2f v4(v4arr);
		b = new ray2f(v3arr, v4arr, false);

		float v5arr[] = {1, 2};
		vector2f v5(v5arr);
		float v6arr[] = {1, 1};
		vector2f v6(v6arr);
		c = new ray2f(v5arr, v6arr, false);
	}

	virtual void TearDown() {
		delete a;
		delete b;
		delete c;
	}
};

/*
 * Exercises the default constructor light().
 */
TEST_F(RayTest, DefaultConstructor) {
	ray3d r;
	ASSERT_DOUBLE_EQ(0, r.getOrig()[0]);
	ASSERT_DOUBLE_EQ(0, r.getOrig()[1]);
	ASSERT_DOUBLE_EQ(0, r.getOrig()[2]);
	ASSERT_DOUBLE_EQ(1, r.getDir()[0]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[1]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[2]);
}

/*
 * Exercises the light (rgbcolor<color_T>, mvector<vec_T, dim>) constructor by
 * making sure the objects initialized in SetUp are correct.
 */
TEST_F(RayTest, NondefaultConstructor) {
	ASSERT_DOUBLE_EQ(0, a->getOrig()[0]);
	ASSERT_DOUBLE_EQ(0, a->getOrig()[1]);
	ASSERT_DOUBLE_EQ(5, a->getOrig()[2]);
	ASSERT_DOUBLE_EQ(0, a->getDir()[0]);
	ASSERT_DOUBLE_EQ(0, a->getDir()[1]);
	ASSERT_DOUBLE_EQ(-1, a->getDir()[2]);

	ASSERT_FLOAT_EQ(0, b->getOrig()[0]);
	ASSERT_FLOAT_EQ(5, b->getOrig()[1]);
	ASSERT_FLOAT_EQ(0, b->getDir()[0]);
	ASSERT_FLOAT_EQ(-2, b->getDir()[1]);
}

/*
 * Exercises the copy constructor light(const light&) as well as the
 * assignment operator.
 */
TEST_F(RayTest, CopyConstructorAndAssignmentOperator) {
	ray3d r(*a);
	ASSERT_DOUBLE_EQ(0, r.getOrig()[0]);
	ASSERT_DOUBLE_EQ(0, r.getOrig()[1]);
	ASSERT_DOUBLE_EQ(5, r.getOrig()[2]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[0]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[1]);
	ASSERT_DOUBLE_EQ(-1, r.getDir()[2]);

	ray2f s(*b);
	ASSERT_FLOAT_EQ(0, s.getOrig()[0]);
	ASSERT_FLOAT_EQ(5, s.getOrig()[1]);
	ASSERT_FLOAT_EQ(0, s.getDir()[0]);
	ASSERT_FLOAT_EQ(-2, s.getDir()[1]);

	double v1arr[] = {1, 1, 5};
	vector3d v1(v1arr);
	double v2arr[] = {1, 1, -2};
	vector3d v2(v2arr);
	ray3d t(v1arr, v2arr);
	t = r; // copy stuff from r into t
	ASSERT_DOUBLE_EQ(0, r.getOrig()[0]);
	ASSERT_DOUBLE_EQ(0, r.getOrig()[1]);
	ASSERT_DOUBLE_EQ(5, r.getOrig()[2]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[0]);
	ASSERT_DOUBLE_EQ(0, r.getDir()[1]);
	ASSERT_DOUBLE_EQ(-1, r.getDir()[2]);
}

/*
 * Exercises the getPoinAtT(time_T) function.
 */
TEST_F(RayTest, GetPointAtTTest) {
	ASSERT_FLOAT_EQ(1, c->getPointAtT(0)[0]); // x coord at t = 0
	ASSERT_FLOAT_EQ(2, c->getPointAtT(0)[1]); // y coord at t = 0
	ASSERT_FLOAT_EQ(2, c->getPointAtT(1)[0]); // x coord at t = 1
	ASSERT_FLOAT_EQ(3, c->getPointAtT(1)[1]); // y coord at t = 1
	ASSERT_FLOAT_EQ(6.5, c->getPointAtT(5.5f)[0]); // x coord at t = 5.5
	ASSERT_FLOAT_EQ(7.5, c->getPointAtT(5.5f)[1]); // y coord at t = 5.5
}
/*
 * The getter functions obviously work, so there are no unit tests for them.
 */

/*
 * Exercises the << operator.
 */
TEST_F(RayTest, Print) {
	std::stringstream s;
	s << *a;
	char cstring[100];
	s.getline(cstring, 100);
	ASSERT_STREQ("start: <0, 0, 5>\tdirection: <0, 0, -1>", cstring);

	std::stringstream t;
	t << *b;
	char cstring2[100];
	t.getline(cstring2, 100);
	ASSERT_STREQ("start: <0, 5>\tdirection: <0, -2>", cstring2);
}

#endif // TEST_RAY_CC
