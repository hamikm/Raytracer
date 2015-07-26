/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "scene.hh"
#include "infplane.hh"
#include "sphere.hh"
#include "camera.hh"
#include "ray.hh"
#include "light.hh"
#include "shape.hh"
#include "mvector.hh"
#include <iostream>

/**
 * Creates a simple scene with three spheres and a plane then renders them
 * into an ASCII PPM file which is sent to standard out. The intended use
 * is @code ./<program name> | pnmtopng > <image filename> @endcode
 */
int main() {

	scene3d scene;
	scene.addLight(sp_light3d(new light3d(
			rgbcolord(0.8, 0.8, 0.8), vector3d(-10.0, 10.0, 5.0))));
	scene.addLight(sp_light3d(new light3d(
			rgbcolord(0.3, 0.3, 0.3), vector3d(5.0, 3.0, 5.0))));
	scene.addShape(sp_shape3d(new sphere3d(
			rgbcolord(1, 0, 0), 0.5, vector3d(-1.2, 0.5, 0.0))));
	scene.addShape(sp_shape3d(new sphere3d(
			rgbcolord(0, 1, 0), 0.5, vector3d(0.0, 0.5, 0.0))));
	scene.addShape(sp_shape3d(new sphere3d(
			rgbcolord(0, 0, 1), 0.5, vector3d(0.2, 0.5, 1.0))));
	scene.addShape(sp_shape3d(new infplaned(
			rgbcolord(0.5, 0.0, 0.5), 0, vector3d(0.0, 1.0, 0.0))));

	camera3d cam(vector3d(-1.5, 1.0, 3.0),
			vector3d(-0.3, 0.5, 0.0), vector3d(0.0, 1.0, 0.0));


	/* Render 1000 x 1000 image of this scene. */
	scene.renderPPM(cam, 100, std::cout);

	return 0;
}
