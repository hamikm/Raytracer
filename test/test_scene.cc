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
#include "arealight.hh"
#include "infplane.hh"
#include "gtest/gtest.h"
#include <iostream>
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"

#ifndef TEST_SCENE_CC
#define TEST_SCENE_CC

/**
 * This test fixture class sets up a persistent scene that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends->
 */
class sceneTest : public ::testing::Test {
protected:

	sp_scene3d s;
	sp_lightd a, b;
	sp_shape3d c, d;
	arealightd defarealight;

	/**
	 * Since we're passing pointers to the lights in the scene and pointers
	 * to the shapes in the scene we have to be careful to preserve the
	 * underlying objects otherwise their destructors get called at the
	 * end of this function.
	 */
	virtual void SetUp() {
		s = boost::make_shared<scene3d>(scene3d(false));

		rgbcolord c1(0.1, 0.3, 0.5);
		vector3d v1(1.0, 2.0, 3.0);
		lightd l1(c1, v1);
		a = boost::make_shared<lightd>(l1);

		rgbcolord c2(0.2, 0.4, 0.6);
		vector3d v2(4.0, 5.0, 6.0);
		lightd l2(c2, v2);
		b = boost::make_shared<lightd>(l2);

		vector3d v3(1.0, 3.0, 5.0);
		sphere3d sp(c1, 2, v3);
		c = boost::dynamic_pointer_cast<shape3d>(
				boost::make_shared<sphere3d>(sp));

		vector3d v4(2.0, 4.0, 6.0);
		infplaned p(c2, 2, v4);
		d = boost::dynamic_pointer_cast<shape3d>(
				boost::make_shared<infplaned>(p));

		s->addShape(c);
		s->addShape(d);
		s->addPointLight(a);
		s->addPointLight(b);
	}

	virtual void TearDown() { }
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
	s->addAreaLight(boost::make_shared<arealightd>(defarealight));
	std::cout << std::endl << *s << std::endl << std::endl;
}

#endif // TEST_SCENE_CC
