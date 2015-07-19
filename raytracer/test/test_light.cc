/**
 * @file
 * @author Hamik Mukelyan
 */

#include "light.hh"
#include "mvector.hh"
#include "rgbcolor.hh"
#include "gtest/gtest.h"
#include <sstream>

/**
 * This test fixture class sets up a couple of persistent lights that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class lightTest : public ::testing::Test {
protected:

	light3d *a;
	light2ddf *b;

	virtual void SetUp() {
		rgbcolord c1(0.1, 0.3, 0.5);
		double v1arr[] = {1, 2, 3};
		vector3d v1(v1arr);
		a = new light3d(c1, v1);

		rgbcolord c2(0.2, 0.4, 0.6);
		double v2arr[] = {2, 3};
		vector2d v2(v2arr);
		b = new light2ddf(c2, v2);
	}

	virtual void TearDown() {
		delete a;
		delete b;
	}
};

/*
 * Exercises the default constructor light().
 */
TEST_F(lightTest, DefaultConstructor) {
	light3ddf c;
	ASSERT_DOUBLE_EQ(1.0, c.getColor().getR());
	ASSERT_DOUBLE_EQ(1.0, c.getColor().getG());
	ASSERT_DOUBLE_EQ(1.0, c.getColor().getB());
	ASSERT_DOUBLE_EQ(0, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[2]);

	light2f d;
	ASSERT_FLOAT_EQ(1.0f, c.getColor().getR());
	ASSERT_FLOAT_EQ(1.0f, c.getColor().getG());
	ASSERT_FLOAT_EQ(1.0f, c.getColor().getB());
	ASSERT_FLOAT_EQ(0, c.getPos()[0]);
	ASSERT_FLOAT_EQ(0, c.getPos()[1]);
}

/*
 * Exercises the light (rgbcolor<color_t>, mvector<vec_t, dim>) constructor.
 */
TEST_F(lightTest, NondefaultConstructor) {
	rgbcolord c1(1, 0.5, 0);
	double v1arr[] = {1, 0, 0};
	vector3d v1(v1arr);
	light3d c(c1, v1);
	ASSERT_DOUBLE_EQ(1, c.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, c.getColor().getG());
	ASSERT_DOUBLE_EQ(0, c.getColor().getB());
	ASSERT_DOUBLE_EQ(1, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[2]);

	rgbcolorf c2(0.1, 0.2, 0.3);
	float v2arr[] = {0, 1};
	vector2f v2(v2arr);
	light2f d(c2, v2);
	ASSERT_FLOAT_EQ(0.1, d.getColor().getR());
	ASSERT_FLOAT_EQ(0.2, d.getColor().getG());
	ASSERT_FLOAT_EQ(0.3, d.getColor().getB());
	ASSERT_FLOAT_EQ(0, d.getPos()[0]);
	ASSERT_FLOAT_EQ(1, d.getPos()[1]);
}

/*
 * Exercises the copy constructor light(const light&) as well as the
 * assignment operator.
 */
TEST_F(lightTest, CopyConstructorAndAssignmentOperator) {
	light3d c(*a);
	ASSERT_DOUBLE_EQ(0.1, c.getColor().getR());
	ASSERT_DOUBLE_EQ(0.3, c.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, c.getColor().getB());
	ASSERT_DOUBLE_EQ(1, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(2, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(3, c.getPos()[2]);

	light3d d;
	d = *a; // test assignment operator
	ASSERT_DOUBLE_EQ(0.1, d.getColor().getR());
	ASSERT_DOUBLE_EQ(0.3, d.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, d.getColor().getB());
	ASSERT_DOUBLE_EQ(1, d.getPos()[0]);
	ASSERT_DOUBLE_EQ(2, d.getPos()[1]);
	ASSERT_DOUBLE_EQ(3, d.getPos()[2]);

	light3d e = *a;
	light3d *ptr_to_e = &e;
	e = *ptr_to_e; // assign e to itself
	ASSERT_DOUBLE_EQ(0.1, e.getColor().getR());
	ASSERT_DOUBLE_EQ(0.3, e.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, e.getColor().getB());
	ASSERT_DOUBLE_EQ(1, e.getPos()[0]);
	ASSERT_DOUBLE_EQ(2, e.getPos()[1]);
	ASSERT_DOUBLE_EQ(3, e.getPos()[2]);
}

/*
 * The getter functions obviously work, so there are no unit tests for them.
 */

/*
 * Exercises the << operator.
 */
TEST_F(lightTest, Print) {
	std::stringstream s;
	s << *a;
	char cstring[100];
	s.getline(cstring, 100);
	ASSERT_STREQ("[scene object. color: (0.1, 0.3, 0.5)] ---> "
			"[light. position: (1, 2, 3)]", cstring);
}
