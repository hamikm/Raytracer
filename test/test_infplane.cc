/**
 * @file
 * @author Hamik Mukelyan
 */

#include "infplane.hh"
#include "mvector.hh"
#include "gtest/gtest.h"
#include <sstream>
#include <cmath>

#ifndef TEST_INFPLANE_CC
#define TEST_INFPLANE_CC

/**
 * This test fixture class sets up a couple of persistent planes that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class infplaneTest : public ::testing::Test {
protected:

	infplaned *a, *b, *c;

	virtual void SetUp() {
		a = new infplaned;

		double arr[] = {1, 2, 3};
		vector3d pos(arr);
		b = new infplaned(rgbcolord(0.4, 0.5, 0.6), 2, pos);

		double arr1[] = {1, 1, 0};
		vector3d pos1(arr1);
		c = new infplaned(rgbcolord(0.4, 0.5, 0.6), sqrt(2), pos1);

	}

	virtual void TearDown() {
		delete a;
		delete b;
		delete c;
	}
};

/*
 * Exercises the default constructor infplane() by using a from SetUp.
 */
TEST_F(infplaneTest, DefaultConstructor) {
	ASSERT_DOUBLE_EQ(0, a->getDist());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getB());
	ASSERT_DOUBLE_EQ(0, a->getSurfNorm()[0]);
	ASSERT_DOUBLE_EQ(0, a->getSurfNorm()[1]);
	ASSERT_DOUBLE_EQ(1, a->getSurfNorm()[2]);
}

/*
 * Exercises the infplane (rgbcolor<color_T>, vec_T, mvector<vec_T, dim>)
 * constructor by making sure b from SetUp is correct.
 */
TEST_F(infplaneTest, NondefaultConstructor) {
	ASSERT_DOUBLE_EQ(2, b->getDist());
	ASSERT_DOUBLE_EQ(0.4, b->getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, b->getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, b->getColor().getB());
	ASSERT_DOUBLE_EQ(1/sqrt(14), b->getSurfNorm()[0]);
	ASSERT_DOUBLE_EQ(2/sqrt(14), b->getSurfNorm()[1]);
	ASSERT_DOUBLE_EQ(3/sqrt(14), b->getSurfNorm()[2]);
}

/*
 * Exercises the copy constructor and assignment operator.
 */
TEST_F(infplaneTest, CopyConstructorAndAssignmentOperator) {
	infplaned p(*b);
	ASSERT_DOUBLE_EQ(2, p.getDist());
	ASSERT_DOUBLE_EQ(0.4, p.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, p.getColor().getB());
	ASSERT_DOUBLE_EQ(1/sqrt(14), p.getSurfNorm()[0]);
	ASSERT_DOUBLE_EQ(2/sqrt(14), p.getSurfNorm()[1]);
	ASSERT_DOUBLE_EQ(3/sqrt(14), p.getSurfNorm()[2]);

	infplaned *ptr_to_b = b;
	p = *ptr_to_b; // try to copy p into itself
	// make sure p is still ok
	ASSERT_DOUBLE_EQ(2, p.getDist());
	ASSERT_DOUBLE_EQ(0.4, p.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, p.getColor().getB());
	ASSERT_DOUBLE_EQ(1/sqrt(14), p.getSurfNorm()[0]);
	ASSERT_DOUBLE_EQ(2/sqrt(14), p.getSurfNorm()[1]);
	ASSERT_DOUBLE_EQ(3/sqrt(14), p.getSurfNorm()[2]);

	infplaned r; // use default constructor
	r = p; // copy stuff from p to r using assignment operator
	// make sure everything got copied to r
	ASSERT_DOUBLE_EQ(2, r.getDist());
	ASSERT_DOUBLE_EQ(0.4, r.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, r.getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, r.getColor().getB());
	ASSERT_DOUBLE_EQ(1/sqrt(14), r.getSurfNorm()[0]);
	ASSERT_DOUBLE_EQ(2/sqrt(14), r.getSurfNorm()[1]);
	ASSERT_DOUBLE_EQ(3/sqrt(14), r.getSurfNorm()[2]);
}

/*
 * The getter functions obviously work, so there are no unit tests for them.
 * So does surfNorm since it just returns the only surface normal.
 */

TEST_F(infplaneTest, Intersection) {
	double varr[] = {0, 0, 5}, uarr[] = {0, 0, -1};
	vector3d v(varr), u(uarr);
	ray3d r(v, u); // point this at default constructor plane. I.e. xy plane
	ASSERT_DOUBLE_EQ(5, a->intersection(r));

	double warr[] = {-1, -1, 0};
	vector3d w(warr), x;
	ray3d s(x, w);
	ASSERT_DOUBLE_EQ(sqrt(2), c->intersection(s));
}

/*
 * Exercises the << operator.
 */
TEST_F(infplaneTest, Print) {
	std::cout << std::endl << *a << std::endl << std::endl;
}

#endif // TEST_INFPLANE_CC
