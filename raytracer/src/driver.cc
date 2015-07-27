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
#include "boost/shared_ptr.hpp"
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

using namespace std;

/**
 * A function pointer for @c sceneobj input handlers. Functions with this
 * signature will take input streams and construct @c sceneobj objects
 * from them. Note that lights and shapes, including spheres and planes, are
 * @c sceneobj derived classes but camera is not.
 */
typedef sp_shape3d (*sceneObjInputFunction)(istream&);

/**
 * Reads sphere data from the given input stream then outputs a Boost shared
 * pointer to a sphere constructed from that data. Format must be
 * @code color radius center @endcode
 * where @c color is in the format (r, g, b) and @c center is in the format
 * <x, y, z>.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the sphere.
 */
sp_shape3d readSphere(istream &is) {
	rgbcolord color;
	double rad;
	vector3d vec;
	cin >> color >> rad >> vec;
	sp_shape3d x(new sphere3d(color, rad, vec));
	return x;
}

/**
 * Reads plane data from the given input stream then outputs a Boost shared
 * pointer to a plane constructed from that data. Format must be
 * @code color distance_from_origin surface_normal @endcode
 * where @c color is in the format (r, g, b), @c distance_from_origin is
 * a double, and @c surface_normal is a vector in the format <x, y, z>.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the plane.
 */
sp_shape3d readPlane(istream &is) {
	rgbcolord color;
	double dist;
	vector3d vec;
	cin >> color >> dist >> vec;
	sp_shape3d x(new infplaned(color, dist, vec));
	return x;
}

/**
 * Reads light data from the given input stream then outputs a Boost shared
 * pointer to a light constructed from that data. Format must be
 * @code color position @endcode
 * where @c color is in the format (r, g, b) and @c position is
 * a vector in the format <x, y, z>.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the light.
 */
sp_lightd readLight(istream &is) {
	rgbcolord color;
	vector3d vec;
	cin >> color >> vec;
	sp_lightd x(new lightd(color, vec));
	return x;
}

/**
 * Reads camera data from the given input stream then outputs a @c camerad
 * object constructed from that data. Format must be
 * @code position look_at_position up_direction @endcode where
 * @c position , @c look_at_position , and @c up_direction are
 * vector in the format <x, y, z>.
 *
 * @param is The input stream from which to read.
 *
 * @returns A camera object.
 */
sp_camerad readCamera(istream &is) {
	vector3d vec1;
	vector3d vec2;
	vector3d vec3;
	cin >> vec1 >> vec2 >> vec3;
	sp_camerad x(new camerad(vec1, vec2, vec3));
	return x;
}

map<string, sceneObjInputFunction> readerFuncs;

/**
 * This program takes a scene description from @c cin (it's advised to redirect
 * the scene description from a file like @c example.dat) then renders a scene
 * of the given width and height to PPM formatted image. The output is written
 * to cout, so it's advised to pipe the output to @c pnmtopng , which converts
 * PPM images to PNG, then to redirect its output to a file like @c img.png.
 * For example, @code rt 640 480 < example.dat | pnmtopng > img.png @endcode
 */
int main(int argc, char **argv) {

	if (argc != 3) {
		cout << "---> Usage: " << argv[0]
				<< ": <width in pixels> <height in pixels> " << endl;
		cout << "---> It's intended for scene descriptions to be sent in with"
				<< " redirection like: " << endl
				<< "       " << argv[0] << " 640 480 < inputfile.dat" << endl;
		cout << "---> See example.dat for the scene description format."
				<< endl;
		return 1;
	}
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);

	/* Make the string-readerFunction mappings. */
	readerFuncs["sphere"] = readSphere;
	readerFuncs["plane"] = readPlane;

	scene3d scene;
	sp_camerad cam;
	string type;

	while (cin >> type) {

		// If the type is mapped to a shape reader function...
		if (readerFuncs.find(type) != readerFuncs.end()) {
			sp_shape3d shape;
			shape = readerFuncs[type](cin);
			scene.addShape(shape);
		}
		// Otherwise if it's a light...
		else if (type == "light") {
			sp_lightd light;
			light = readLight(cin);
			scene.addLight(light);
		}
		// Otherwise if the type is camera...
		else if (type == "camera") {
			cam = readCamera(cin);
		}
		// Otherwise if we see the scene description's end...
		else if (type == "end") {
			break;
		}
		// Otherwise if the "type" is a comment start character...
		// Note that comments must begin with a # AND a space, like "# ",
		// and that the "# " must be at the beginning of the line.
		else if (type == "#") {
			string throwaway;
			getline(cin, throwaway);
		}
		// Otherwise we have an unrecognized type, which is an error.
		else {
			cerr << "ERROR: \"" << type << "\" is not a recognized scene " <<
					"description type." << endl;
			return 1;
		}
	}

	/* Render width x height image of this scene. */
	scene.renderPPM(*cam, width, height, cout);

	return 0;
}
