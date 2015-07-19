/**
 * @file
 * @author Hamik Mukelyan
 */

#include "gtest/gtest.h"
#include "test_light.cc"
#include "test_ray.cc"
#include "test_mvector.cc"
#include "test_rgbcolor.cc"
#include "test_infplane.cc"
#include "test_sphere.cc"

using namespace testing;

/**
 * Runs all the test suites in the @c test directory.
 *
 * @param argc
 * @param argv
 *
 * @return Usually zero :-)
 */
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
