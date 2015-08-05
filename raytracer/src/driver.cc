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
#include "cylinder.hh"
#include "light.hh"
#include "shape.hh"
#include "mvector.hh"
#include "spotlight.hh"
#include "boost/shared_ptr.hpp"
#include <iostream>
#include <string>
#include <sstream>
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
 * @code color radius center reflectivity @endcode
 * where @c color is in the format (r, g, b), @c radius is just a double,
 * @c center is in the format <x, y, z>, and @c reflectivity is just a float
 * between 0 and 1, inclusive. All fields are required.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the sphere.
 */
sp_shape3d readSphere(istream &is) {
	rgbcolord color;
	float reflectivity;
	double rad;
	vector3d vec;
	cin >> color >> rad >> vec >> reflectivity;
	sp_shape3d x(new sphere3d(color, rad, vec, reflectivity));
	return x;
}

/**
 * Reads cylinder data from the given input stream then outputs a Boost shared
 * pointer to a cylinder constructed from that data. Format must be
 * @code color radius center axis height reflectivity @endcode
 * where @c color is in the format (r, g, b), @c radius is just a double,
 * @c center is in the format <x, y, z>, @c axis is the orientation of the long
 * axis of the cylinder, @c height is the height of the cylinder, and
 * @c reflectivity is just a float between 0 and 1, inclusive. All fields are
 * required.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the sphere.
 */
sp_shape3d readCylinder(istream &is) {
	rgbcolord color;
	float reflectivity;
	double radius, height;
	vector3d vec1, vec2;
	cin >> color >> radius >> vec1 >> vec2 >> height >> reflectivity;
	sp_shape3d x(
			new cylinderd(color, radius, vec1, height, vec2, reflectivity));
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
	float reflectivity;
	double dist;
	vector3d vec;
	cin >> color >> dist >> vec >> reflectivity;
	sp_shape3d x(new infplaned(color, dist, vec, reflectivity));
	return x;
}

/**
 * Reads point light data from the given input stream then outputs a Boost
 * shared pointer to a light constructed from that data. Format must be
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
 * Reads spotlight data from the given input stream then outputs a Boost shared
 * pointer to a spot light constructed from that data. Format must be
 * @code color position look_at_position angle @endcode
 * where @c color is in the format (r, g, b), @c position is
 * a vector in the format <x, y, z>, @c look_at_position is also a vector, and
 * @c angle is a float describing the cone angle.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the light.
 */
sp_spotlightd readSpotLight(istream &is) {
	rgbcolord color;
	vector3d pos, lookat;
	float angle;
	cin >> color >> pos >> lookat >> angle;
	assert(!(pos[0] == lookat[0] &&
			lookat[1] == lookat[1] && pos[2] == lookat[2]));
	sp_spotlightd x(new spotlightd(color, pos, (lookat - pos).norm(), angle));
	return x;
}

/**
 * Reads area light data from the given input stream then outputs a Boost
 * shared pointer to an area light constructed from that data. Format must be
 * @code color center surface_normal up_direction horizontal_spacing
 * vertical_spacing width height @endcode
 * where @c color is in the format (r, g, b), @c center is
 * a vector in the format <x, y, z>, @c surface_normal is also a vector,
 * @c up_direction is a vector pointing along the v axis, @c horizontal_spacing
 * is a double, @c vertical_spacing is a double, @c width is a double, and
 * @c height is a double.
 *
 * @param is The input stream from which to read.
 *
 * @returns A Boost shared pointer to the area light.
 */
sp_arealightd readAreaLight(istream &is) {
	rgbcolord color;
	vector3d center, normal, upDir;
	double hspace, vspace, width, height;
	cin >> color >> center >> normal >> upDir >> hspace >> vspace >> width
		>> height;
	sp_arealightd x(new arealightd(
			color, center, normal, upDir, hspace, vspace, width, height));
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
 * Prints usage message to stdout.
 *
 * @param progname Name of this program.
 */
void usage(char *progname) {
	cout << "---> Usage: " << progname
			<< ": <width in pixels> <height in pixels> -s" << endl;
	cout << "---> -s is for shadows and must be the last argument." << endl;
	cout << "---> It's intended for scene descriptions to be sent in with"
			<< " redirection like: " << endl
			<< "       " << progname << " 640 480 < inputfile.dat" << endl;
	cout << "---> See example.dat for the scene description format."
			<< endl;
}

/**
 * This program takes a scene description from @c cin (it's advised to redirect
 * the scene description from a file like @c example.dat) then renders a scene
 * of the given width and height to PPM formatted image. Shadows can be turned
 * on with the flag -s, but it must be the last argument to the program. The
 * output is written to cout so it's advised to pipe the output to @c pnmtopng ,
 * which converts PPM images to PNG, then to redirect its output to a file like
 * @c img.png. For example, @code rt 640 480 -s < example.dat | pnmtopng >
 * img.png @endcode
 */
int main(int argc, char **argv) {

	if (argc != 4 && argc != 3) {
		usage(argv[0]);
		return 1;
	}
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	bool shadowsOn = false;
	if(argc == 4) {
		stringstream ss;
		ss << argv[3];
		if(ss.str() == "-s") {
			shadowsOn = true;
		}
		else {
			usage(argv[0]);
			return 1;
		}
	}

	/* Make the string-readerFunction mappings. */
	readerFuncs["sphere"] = readSphere;
	readerFuncs["plane"] = readPlane;
	readerFuncs["cylinder"] = readCylinder;

	scene3d scene(shadowsOn);
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
			scene.addPointLight(light);
		}
		// Otherwise if it's a spot light...
		else if (type == "spotlight") {
			sp_spotlightd slight;
			slight = readSpotLight(cin);
			scene.addSpotLight(slight);
		}
		// Otherwise if it's an area light...
		else if (type == "arealight") {
			sp_arealightd alight;
			alight = readAreaLight(cin);
			scene.addAreaLight(alight);
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

	// TODO
	//cerr << scene << endl;
	//return 1;

	/* Render width x height image of this scene. */
	scene.renderPPM(*cam, width, height, cout);

	return 0;
}
