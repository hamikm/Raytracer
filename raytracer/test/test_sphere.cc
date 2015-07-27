/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sphere.hh"
#include "mvector.hh"
#include "gtest/gtest.h"
#include <sstream>
#include <cmath>

#ifndef TEST_SPHERE_CC
#define TEST_SPHERE_CC

/**
 * This test fixture class sets up a couple of persistent planes that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class sphereTest : public ::testing::Test {
protected:

	sphere3d *a, *c;
	sphere2d *b;

	virtual void SetUp() {
		a = new sphere3d;

		double arr[] = {1, 2};
		vector2d pos(arr);
		b = new sphere2d(rgbcolord(0.4, 0.5, 0.6), 2, pos);

		// c should intersect the origin
		double arr1[] = {1, 1, 0};
		vector3d pos1(arr1);
		c = new sphere3d(rgbcolord(0.4, 0.5, 0.6), sqrt(2), pos1);
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
TEST_F(sphereTest, DefaultConstructor) {
	ASSERT_DOUBLE_EQ(1, a->getRadius());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, a->getColor().getB());
	ASSERT_DOUBLE_EQ(0, a->getCenter()[0]);
	ASSERT_DOUBLE_EQ(0, a->getCenter()[1]);
	ASSERT_DOUBLE_EQ(0, a->getCenter()[2]);
}

/*
 * Exercises the infplane (rgbcolor<color_T>, vec_T, mvector<vec_T, dim>)
 * constructor by making sure b from SetUp is correct.
 */
TEST_F(sphereTest, NondefaultConstructor) {
	ASSERT_DOUBLE_EQ(2, b->getRadius());
	ASSERT_DOUBLE_EQ(0.4, b->getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, b->getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, b->getColor().getB());
	ASSERT_DOUBLE_EQ(1, b->getCenter()[0]);
	ASSERT_DOUBLE_EQ(2, b->getCenter()[1]);

	ASSERT_DOUBLE_EQ(sqrt(2), c->getRadius());
	ASSERT_DOUBLE_EQ(0.4, c->getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, c->getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, c->getColor().getB());
	ASSERT_DOUBLE_EQ(1, c->getCenter()[0]);
	ASSERT_DOUBLE_EQ(1, c->getCenter()[1]);
	ASSERT_DOUBLE_EQ(0, c->getCenter()[2]);
}

/*
 * Exercises the copy constructor and assignment operator.
 */
TEST_F(sphereTest, CopyConstructorAndAssignmentOperator) {
	sphere3d p(*a);
	ASSERT_DOUBLE_EQ(1, p.getRadius());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getB());
	ASSERT_DOUBLE_EQ(0, p.getCenter()[0]);
	ASSERT_DOUBLE_EQ(0, p.getCenter()[1]);
	ASSERT_DOUBLE_EQ(0, p.getCenter()[2]);

	sphere3d *ptr_to_a = a;
	p = *ptr_to_a; // try to copy p into itself
	// make sure p is still ok
	ASSERT_DOUBLE_EQ(1, p.getRadius());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getG());
	ASSERT_DOUBLE_EQ(0.5, p.getColor().getB());
	ASSERT_DOUBLE_EQ(0, p.getCenter()[0]);
	ASSERT_DOUBLE_EQ(0, p.getCenter()[1]);
	ASSERT_DOUBLE_EQ(0, p.getCenter()[2]);

	sphere3d r; // use default constructor
	r = *c;
	// make sure everything got copied to r
	ASSERT_DOUBLE_EQ(sqrt(2), r.getRadius());
	ASSERT_DOUBLE_EQ(0.4, r.getColor().getR());
	ASSERT_DOUBLE_EQ(0.5, r.getColor().getG());
	ASSERT_DOUBLE_EQ(0.6, r.getColor().getB());
	ASSERT_DOUBLE_EQ(1, r.getCenter()[0]);
	ASSERT_DOUBLE_EQ(1, r.getCenter()[1]);
	ASSERT_DOUBLE_EQ(0, r.getCenter()[2]);
}

/*
 * The getter functions obviously work, so there are no unit tests for them.
 */

TEST_F(sphereTest, Intersection) {
	double varr[] = {0, 0, 5}, uarr[] = {0, 0, -1};
	vector3d v(varr), u(uarr);
	ray3d r(v, u); // point this at default constructor sphere.
	ASSERT_DOUBLE_EQ(4, a->intersection(r));

	// recall that c intersects the origin
	double xarr[] = {-5, 0, 0},	yarr[] = {0, -5, 0}, zarr[] = {-5, -5, 0};
	vector3d x(xarr), y(yarr), z(zarr);
	ray3d s1(x, -x), s2(y, -y), s3(z, -z); // all rays should point to origin
	ASSERT_DOUBLE_EQ(5, c->intersection(s1));
	ASSERT_DOUBLE_EQ(5, c->intersection(s2));
	ASSERT_DOUBLE_EQ(sqrt(50), c->intersection(s3));
}

/*
 * Exercises the surfNorm function.
 */
TEST_F(sphereTest, SurfNorm) {
	vector3d origin;
	ASSERT_DOUBLE_EQ(-1 / sqrt(2), c->surfaceNorm(origin)[0]);
	ASSERT_DOUBLE_EQ(-1 / sqrt(2), c->surfaceNorm(origin)[1]);
	ASSERT_DOUBLE_EQ(0, c->surfaceNorm(origin)[2]);

	double pos[] = {0, 1, 0};
	vector3d top(pos);
	ASSERT_DOUBLE_EQ(0, a->surfaceNorm(top)[0]);
	ASSERT_DOUBLE_EQ(1, a->surfaceNorm(top)[1]);
	ASSERT_DOUBLE_EQ(0, a->surfaceNorm(top)[2]); // TODO problem
}

/*
 * Exercises the << operator.
 */
TEST_F(sphereTest, Print) {
	std::stringstream s;
	s << *a;
	char cstring[200];
	s.getline(cstring, 200);
	ASSERT_STREQ("[scene object. color: (0.5, 0.5, 0.5)] ---> [shape] ---> "
			"[sphere. center: <0, 0, 0>, radius: 1]", cstring);

	std::stringstream t;
	t << *c;
	char cstring2[200];
	t.getline(cstring2, 200);
	ASSERT_STREQ("[scene object. color: (0.4, 0.5, 0.6)] ---> [shape] ---> "
			"[sphere. center: <1, 1, 0>, radius: 1.41421]", cstring2);
}

#endif // TEST_SPHERE_CC
