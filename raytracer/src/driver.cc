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
 * is @code ./<program name> | pnmtopng > <image filename>.png @endcode
 */
int main() {
	infplaned *plane1 = new infplaned(
			rgbcolord(0.5, 0.0, 0.5), 0, vector3d(0.0, 1.0, 0.0));
	sphere3d *sphere1 = new sphere3d(
			rgbcolord(1, 0, 0), 0.5, vector3d(-1.2, 0.5, 0.0));
	sphere3d *sphere2 = new sphere3d(
			rgbcolord(0, 1, 0), 0.5, vector3d(0.0, 0.5, 0.0));
	sphere3d *sphere3 = new sphere3d(
			rgbcolord(0, 0, 1), 0.5, vector3d(0.2, 0.5, 1.0));
	light3d *light1 = new light3d(
			rgbcolord(0.8, 0.8, 0.8), vector3d(-10.0, 10.0, 5.0));
	light3d *light2 = new light3d(
			rgbcolord(0.3, 0.3, 0.3), vector3d(5.0, 3.0, 5.0));
	camera3d *cam = new camera3d(vector3d(-1.5, 1.0, 3.0),
			vector3d(-0.3, 0.5, 0.0), vector3d(0.0, 1.0, 0.0));

	scene3d *scene = new scene3d;
	scene->addLight(light1);
	scene->addLight(light2);
	scene->addShape(sphere1);
	scene->addShape(sphere2);
	scene->addShape(sphere3);
	scene->addShape(plane1);


	scene->renderPPM(*cam, 1000, std::cout);

	delete cam;
	delete scene;

	return 0;
}
