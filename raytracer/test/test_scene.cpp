/**
 * @file
 * @author Hamik Mukelyan
 */

#include "scene.hh"
#include "light.hh"
#include "mvector.hh"
#include "rgbcolor.hh"
#include "sphere.hh"
#include "light.hh"
#include "shape.hh"
#include "infplane.hh"
#include "gtest/gtest.h"
#include <iostream>

/**
 * This test fixture class sets up a persistent scene that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class sceneTest : public ::testing::Test {
protected:

	scene3d *s;
	light3d *a, *b;
	shape3d *c, *d;

	/**
	 * Since we're passing pointers to the lights in the scene and pointers
	 * to the shapes in the scene we have to be careful to preserve the
	 * underlying objects... otherwise their destructors get called at the
	 * end of this function.
	 */
	virtual void SetUp() {
		rgbcolord c1(0.1, 0.3, 0.5);
		double v1arr[] = {1, 2, 3};
		vector3d v1(v1arr);
		a = new light3d(c1, v1);

		rgbcolord c2(0.2, 0.4, 0.6);
		double v2arr[] = {4, 5, 6};
		vector3d v2(v2arr);
		b = new light3d(c2, v2);

		double v3arr[] = {1, 3, 5};
		vector3d v3(v3arr);
		c = new sphere3d(c1, 2, v3);

		double v4arr[] = {2, 4, 6};
		vector3d v4(v4arr);
		d = new infplaned(c2, 2, v4);

		s = new scene3d();
		s->addShape(c);
		s->addShape(d);
		s->addLight(a);
		s->addLight(b);
	}

	virtual void TearDown() {
		delete s; // Note that we only need to delete s because a, b, c, d
		 	      // are automatically deleted by scene's destructor
	}
};

/*
 * The getter functions and default constructor obviously work, so there are
 * no unit tests for them.
 */

TEST_F(sceneTest, FindClosestShape) {
	ray3d r1(vector3d(1.0, 3.0, 10.0), vector3d(0.0, 0.0, -1.0));
	double intersectionTime; // this is out parameter
	s->findClosestShape(r1, intersectionTime);
	ASSERT_DOUBLE_EQ(3, intersectionTime);

	ray3d r2(vector3d(-2.0, 3.0, 5.0), vector3d(1.0, 0.0, 0.0));
	s->findClosestShape(r2, intersectionTime);
	ASSERT_DOUBLE_EQ(1, intersectionTime);
}

TEST_F(sceneTest, TraceRay) {
	ray3d r1(vector3d(1.0, 3.0, 10.0), vector3d(0.0, 0.0, 1.0));
	rgbcolord c1 = s->traceRay(r1);
	ASSERT_DOUBLE_EQ(0, c1.getR());
	ASSERT_DOUBLE_EQ(0, c1.getG());
	ASSERT_DOUBLE_EQ(0, c1.getB());

	ray3d r2(vector3d(1.0, 3.0, -10.0), vector3d(0.0, 0.0, 1.0));
	rgbcolord c2 = s->traceRay(r2);
	ASSERT_DOUBLE_EQ(0.051537615489350617, c2.getR());
	ASSERT_DOUBLE_EQ(0.2352538705067326, c2.getG());
	ASSERT_DOUBLE_EQ(0.55114876505214583, c2.getB());
}

TEST_F(sceneTest, Print) {
	std::stringstream str;
	str << *s;
	char cstring[200];
	str.getline(cstring, 200);
	ASSERT_STREQ("scene:", cstring);
	str.getline(cstring, 200);
	ASSERT_STREQ("  lights:", cstring);
	str.getline(cstring, 200);
	ASSERT_STREQ("    [scene object. color: (0.1, 0.3, 0.5)] ---> "
			"[light. position: (1, 2, 3)]", cstring);
	str.getline(cstring, 200);
		ASSERT_STREQ("    [scene object. color: (0.2, 0.4, 0.6)] ---> "
				"[light. position: (4, 5, 6)]", cstring);
	str.getline(cstring, 200);
	ASSERT_STREQ("  shapes:", cstring);
	str.getline(cstring, 200);
	ASSERT_STREQ("    [scene object. color: (0.1, 0.3, 0.5)] ---> [shape] "
			"---> [sphere. center: (1, 3, 5), radius: 2]", cstring);
	str.getline(cstring, 200);
	ASSERT_STREQ("    [scene object. color: (0.2, 0.4, 0.6)] ---> [shape] "
			"---> [infinite plane. dist from origin: 2, surface normal: "
			"(0.267261, 0.534522, 0.801784)]", cstring);
}
