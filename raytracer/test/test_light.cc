/**
 * @file
 * @author Hamik Mukelyan
 */

#include "light.hh"
#include "mvector.hh"
#include "rgbcolor.hh"
#include "gtest/gtest.h"
#include <sstream>

#ifndef TEST_LIGHT_CC
#define TEST_LIGHT_CC

/**
 * This test fixture class sets up a couple of persistent lights that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class lightTest : public ::testing::Test {
protected:

	lightd *a;

	virtual void SetUp() {
		rgbcolord c1(0.1, 0.3, 0.5);
		double v1arr[] = {1, 2, 3};
		vector3d v1(v1arr);
		a = new lightd(c1, v1);
	}

	virtual void TearDown() {
		delete a;
	}
};

/*
 * Exercises the default constructor light().
 */
TEST_F(lightTest, DefaultConstructor) {
	lightddf c;
	ASSERT_DOUBLE_EQ(0.7, c.getColor().getR());
	ASSERT_DOUBLE_EQ(0.7, c.getColor().getG());
	ASSERT_DOUBLE_EQ(0.7, c.getColor().getB());
	ASSERT_DOUBLE_EQ(0, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[2]);
}

/*
 * Exercises the light (rgbcolor<color_t>, mvector<vec_t, dim>) constructor.
 */
TEST_F(lightTest, NondefaultConstructor) {
	rgbcolord c1(1, 0.5, 0);
	double v1arr[] = {1, 0, 0};
	vector3d v1(v1arr);
	lightd c(c1, v1);

	ASSERT_DOUBLE_EQ(1, c.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, c.getColor().getG());
	ASSERT_DOUBLE_EQ(0, c.getColor().getB());
	ASSERT_DOUBLE_EQ(1, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(0, c.getPos()[2]);
}

/*
 * Exercises the copy constructor light(const light&) as well as the
 * assignment operator.
 */
TEST_F(lightTest, CopyConstructorAndAssignmentOperator) {
	lightd c(*a);
	ASSERT_DOUBLE_EQ(0.1, c.getColor().getR());
	ASSERT_DOUBLE_EQ(0.3, c.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, c.getColor().getB());
	ASSERT_DOUBLE_EQ(1, c.getPos()[0]);
	ASSERT_DOUBLE_EQ(2, c.getPos()[1]);
	ASSERT_DOUBLE_EQ(3, c.getPos()[2]);

	lightd d;
	d = *a; // test assignment operator
	ASSERT_DOUBLE_EQ(0.1, d.getColor().getR());
	ASSERT_DOUBLE_EQ(0.3, d.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, d.getColor().getB());
	ASSERT_DOUBLE_EQ(1, d.getPos()[0]);
	ASSERT_DOUBLE_EQ(2, d.getPos()[1]);
	ASSERT_DOUBLE_EQ(3, d.getPos()[2]);

	lightd e = *a;
	lightd *ptr_to_e = &e;
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
			"[light. position: <1, 2, 3>]", cstring);
}

#endif // TEST_LIGHT_CC
