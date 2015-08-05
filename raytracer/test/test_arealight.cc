/**
 * @file
 * @author Hamik Mukelyan
 */

#include "light.hh"
#include "arealight.hh"
#include "mvector.hh"
#include "rgbcolor.hh"
#include "gtest/gtest.h"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#include <iostream>

#ifndef TEST_AREALIGHT_CC
#define TEST_AREALIGHT_CC

/**
 * This test fixture class sets up an area light that we use
 * below for testing. Note that an object of this class is created before
 * each test case begins and is torn down when each test case ends.
 */
class arealightTest : public ::testing::Test {
protected:

	arealightd d; // should invoke default constructor
	sp_arealightd a;

	virtual void SetUp() {
		arealightd l(rgbcolord(0.8, 0.8, 0.8), vector3d(0.0, 4.0, 1.0),
				vector3d(0.0, -1.0, 0.0), vector3d(0.0, 0.0, 1.0),
				0.1, 0.1, 0.6, 0.6);
		a = boost::make_shared<arealightd>(l);
	}

	virtual void TearDown() { }
};

/*
 * I'm only really testing that the fields were initialized correctly, which
 * is must faster to do by looking at print output than by writing. Thus this
 * "test".
 */
TEST_F(arealightTest, Print) {
	std::cout << d << std::endl;
	std::cout << "----------------------------------------------------------"
			<< std::endl;
	std::cout << *a << std::endl;
}

#endif // TEST_AREALIGHT_CC
