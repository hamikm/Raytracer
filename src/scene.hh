/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "ray.hh"
#include "light.hh"
#include "spotlight.hh"
#include "arealight.hh"
#include "shape.hh"
#include "camera.hh"
#include "boost/shared_ptr.hpp"
#include <cassert>
#include <vector>
#include <iostream>
#include <ostream>
#include <iterator>

#ifndef SCENE_HH
#define SCENE_HH

/**
 * The color that will be returned when a ray misses all the objects in the
 * scene. The default is black.
 */
#define DEFAULT_BKCOLOR rgbcolor<color_T>()

/**
 * Colors are 0 through 255, inclusive, as ints.
 */
#define COLORMAX 255

/**
 * Maximum number of reflections to track.
 */
#define MAX_REFLECT 10

/**
 * Used to make sure a ray emitted from an object doesn't intersect itself.
 */
#define DELTA 0.00001

/**
 * Represents a 3D scene as a collection of shape pointers and light
 * pointers.
 *
 * @tparam vec_T The type of the vector components. Many arithmetic and
 *   relational operators must be supported. The type will likely be double,
 *   float, or int.
 * @tparam time_T The type of the time. Many arithmetic and
 *   relational operators must be supported. The type will likely be double or
 *   float, but int would likely work.
 * @tparam color_T The type of the @c rgbcolor. Many arithmetic
 *   and relational operators must be supported, including real number ones.
 *   The type will likely be double or float.
 * @tparam dim The number of dimensions. Note that 2 would make sense, since
 *   it would specify a line.
 */
template<typename vec_T, typename color_T,  typename time_T, int dim>
class scene {
private:

	/**
	 * Boost shared pointer typedef for pointlights. This is generic unlike the
	 * typedefs in the light header file.
	 */
	typedef boost::shared_ptr<light<vec_T, color_T, time_T, dim> > sp_light;

	/**
	 * Boost shared pointer typedef for spotlights. This is generic unlike the
	 * typedefs in the spotlight header file.
	 */
	typedef boost::shared_ptr<spotlight<vec_T, color_T, time_T, dim> >
		sp_spotlight;

	/**
	 * Boost shared pointer typedef for shapes. This is generic unlike the
	 * typedefs in the light header file.
	 */
	typedef boost::shared_ptr<shape<vec_T, color_T, time_T, dim> > sp_shape;

	/**
	 * Boost shared pointer typedef for area lights. This is generic unlike the
	 * typedefs in the light header file.
	 */
	typedef boost::shared_ptr<arealight<vec_T, color_T, time_T, dim> >
		sp_arealight;

	/**
	 * An STL vector of Boost shared pointers to the point lights in the scene.
	 */
	std::vector<sp_light> pointlights;

	/**
	 * An STL vector of Boost shared pointers to the spotlights in this scene.
	 */
	std::vector<sp_spotlight> spotlights;

	/**
	 * An STL vector of Boost shared pointers to all the shapes in the scene.
	 */
	std::vector<sp_shape> shapes;

	/**
	 * Controls if shadows are used in the raytracer or not.
	 */
	bool useShadows;

public:

	/**
	 * Constructs an empty scene with or without shadows.
	 *
	 * @param useShadows If true, renders if shadows, if false, not
	 */
	scene(bool useShadows) : useShadows(useShadows) { }

	/**
	 * Adds a shape to the scene.
	 *
	 * @param obj Boost shared pointer to a shape to add to this scene.
	 */
	void addShape(sp_shape obj) {
		assert(obj != 0);
		shapes.push_back(obj);
	}

	/**
	 * Adds a point light to the scene.
	 *
	 * @param theLight Boost shared pointer to the point light to add to this
	 *        scene.
	 */
	void addPointLight(sp_light theLight) {
		assert(theLight != 0);
		pointlights.push_back(theLight);
	}

	/**
	 * Adds a spot light to the scene.
	 *
	 * @param theLight Boost shared pointer to the point light to add to this
	 *        scene.
	 */
	void addSpotLight(sp_spotlight theLight) {
		assert(theLight != 0);
		spotlights.push_back(theLight);
	}

	/**
	 * Adds an area light to the scene by dumping all the point lights in
	 * area light approximation into this scene's point light collection.
	 *
	 * @param theLight Boost shared pointer to the point light to add to this
	 *        scene.
	 */
	void addAreaLight(sp_arealight theLight) {
		assert(theLight != 0);
		typename std::vector<boost::shared_ptr<
			light<vec_T, color_T, time_T, dim> > >::const_iterator iter;
		for(iter = theLight->getLights().begin();
				iter < theLight->getLights().end(); iter++) {
			addPointLight(*iter);
		}
	}

	/**
	 * Finds the closest object that intersects the given ray and the time
	 * at which that intersection occurs, which is returned through the
	 * @c tIntersect out-parameter. Returns 0 and sets the out-parameter
	 * to RAY_MISS if there is no such object.
	 *
	 * @param r The ray.
	 * @param[out] tIntersect The time at which the intersection with @c r
	 *   occurs or @c RAY_MISS if there's no intersection.
	 *
	 * @return The nearest object that intersects the ray @c r.
	 */
	sp_shape findClosestShape(
			const ray<vec_T, time_T, dim> &r, time_T &tIntersect) const {
		time_T t = RAY_MISS;
		tIntersect = RAY_MISS;
		sp_shape intersectedObjPtr, tmpPtr;
		typename std::vector<sp_shape>::const_iterator iter;
		for (iter = shapes.begin(); iter < shapes.end(); iter++) {
			tmpPtr = *iter;
			t = tmpPtr->intersection(r);
			if (t != RAY_MISS && t > 0) {
				if (tIntersect == RAY_MISS) { // tIntersect starts at RAY_MISS
					tIntersect = t;
					intersectedObjPtr = tmpPtr;
				}
				else if (t < tIntersect) {
					intersectedObjPtr = tmpPtr;
					tIntersect = t;
				}
			}
		}
		return intersectedObjPtr;
	}

	/**
	 * Determines the color of the given ray. Determines its color by finding
	 * the nearest intersecting object and by combining its color with the
	 * colors of lights in the scene.
	 *
	 * TODO: there is probably a better way to process the non-point lights.
	 * One way would be to write a virtual function called "bool useLight()"
	 * in the light class so that we can have just one light vector in this
	 * class, not one vector for each type of light and one loop for each
	 * vector. That would be a great improvement!
	 *
	 * @param r The ray whose color will be determined.
	 * @param depth The current reflection depth. The recursion
	 *
	 * @return The color of the given ray or @c DEFAULT_BKCOLOR if there is no
	 * intersection.
	 */
	rgbcolor<color_T> traceRay(
			const ray<vec_T, time_T, dim> &r, int depth = 0) const {
		time_T tIntersect; // out parameter
		sp_shape intersectedObjPtr =
				findClosestShape(r, tIntersect);
		if(intersectedObjPtr == 0) {
			return DEFAULT_BKCOLOR;
		}

		mvector<vec_T, dim> intersectionPt = r.getPointAtT(tIntersect);
		mvector<vec_T, dim> N = intersectedObjPtr->surfaceNorm(intersectionPt);

		// Loop over all the point lights, summing the color contributions from
		// each one
		rgbcolor<color_T> finalColor;
		typename std::vector<sp_light>::const_iterator iter;
		for (iter = pointlights.begin(); iter < pointlights.end(); iter++) {
			sp_light currLightPtr = *iter;
			mvector<vec_T, dim> L = currLightPtr->getPos() - intersectionPt;
			L = L.norm();
			vec_T LdotN = L * N;

			// A hack to make sure an object doesn't intersect itself...
			// make the "to light" ray start a little outside an object itself
			// by adding a tiny scalar multiple of a normal line to the
			// starting point
			mvector<vec_T, dim> intersectionPtWithDelta =
					intersectionPt + N * DELTA;

			// shoot a ray to this light. if it hits anything on the way,
			// skip the light (if shadows are on)
			ray<vec_T, time_T, dim> rayToLight(
					intersectionPtWithDelta, L);
			time_T t;
			if (useShadows && findClosestShape(rayToLight, t) != 0) {
				continue;
			}

			// add in the color contribution of the light
			if(LdotN > 0) {
				finalColor += (currLightPtr->getColor() *
						intersectedObjPtr->getColor() * LdotN);
			}
		}

		// Loop over all the spotlights, summing the color contributions from
		// each one. Skip the spotlight if the angle between the ray and the
		// spotlight's direction vector is > the cone angle.
		typename std::vector<sp_spotlight>::const_iterator iter2;
		for (iter2 = spotlights.begin(); iter2 < spotlights.end(); iter2++) {
			sp_spotlight currLightPtr = *iter2;
			mvector<vec_T, dim> L = currLightPtr->getPos() - intersectionPt;
			L = L.norm();

			// skip this light if it's outside the spotlight cone
			float ray_spotlight_angle =
					acos(currLightPtr->getDir().norm() * -L);
			if (ray_spotlight_angle > currLightPtr->getAngle()) {
				continue;
			}

			vec_T LdotN = L * N;

			// A hack to make sure an object doesn't intersect itself...
			// make the "to light" ray start a little outside an object itself
			// by adding a tiny scalar multiple of a normal line to the
			// starting point
			mvector<vec_T, dim> intersectionPtWithDelta =
					intersectionPt + N * DELTA;

			// shoot a ray to this light. if it hits anything on the way,
			// skip the light (if shadows are on)
			ray<vec_T, time_T, dim> rayToLight(
					intersectionPtWithDelta, L);
			time_T t;
			if (useShadows && findClosestShape(rayToLight, t) != 0) {
				continue;
			}

			// add in the color contribution of the light
			if(LdotN > 0) {
				finalColor += (currLightPtr->getColor() *
						intersectedObjPtr->getColor() * LdotN);
			}
		}

		// handle reflections
		if (intersectedObjPtr->getReflectivity() > 0 && depth < MAX_REFLECT) {
			// R_r is the direction of the reflected vector
			ray<vec_T, time_T, dim> R_r = r.reflect(intersectionPt, N);
			// col_r is the reflection color
			rgbcolor<color_T> col_r = traceRay(R_r, depth + 1);
			finalColor +=
					((color_T) intersectedObjPtr->getReflectivity()) * col_r;
		}

		return finalColor;
	}

	/**
	 * Renders this scene for the given camera and image size as a PPM
	 * formatted image to the given output stream.
	 *
	 * @param cam The camera in this scene.
	 * @param width The width of the image in pixels.
	 * @param height The height of the image in pixels.
	 * @param os The output stream to which the PPM image will be written.
	 */
	void renderPPM(const camera<vec_T, time_T, dim> &cam,
			int width, int height,
			std::ostream &os) const {
		os << "P3 " << width << " " << height << " "
				<< COLORMAX << std::endl;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				ray<vec_T, time_T, dim> pixelRay = cam.getRayForPixel(
						x, y, width, height);
				rgbcolor<color_T> c = traceRay(pixelRay);
				c *= COLORMAX;
				c.clamp(0, COLORMAX);
				os << (int)c.getR() << " " << (int)c.getG() << " " <<
						(int)c.getB() << std::endl;
			}
		}
	}

	/**
	 * Prints scene contents to the given output stream.
	 *
	 * @param os The output stream to which the scene attributes will be
	 *   written.
	 */
	void printHelper(std::ostream &os) const {
		os << "scene: shadows " << (useShadows ? "ON" : "OFF") << std::endl;
		os << "  point lights:" << std::endl;
		typename std::vector<sp_light>::const_iterator iter;
		for (iter = pointlights.begin(); iter < pointlights.end(); iter++) {
			os << "    " << **iter << std::endl;
		}

		os << "  spotlights:" << std::endl;
		typename std::vector<sp_spotlight>::const_iterator iter3;
		for (iter3 = spotlights.begin(); iter3 < spotlights.end(); iter3++) {
			os << "    " << **iter3 << std::endl;
		}

		os << "  shapes:" << std::endl;
		typename std::vector<sp_shape>::const_iterator iter2;
		for (iter2 = shapes.begin(); iter2 < shapes.end(); iter2++) {
			os << "    " << **iter2 << std::endl;
		}
	}
};

/**
 * Output operator overload for printing the scene contents to the screen.
 *
 * @param os The output stream.
 * @param sc The scene to write to @c os.
 *
 * @return The same output stream for operator chaining.
 */
template<typename vec_T, typename color_T,  typename time_T, int dim>
std::ostream & operator<<(std::ostream &os,
		const scene<vec_T, color_T, time_T, dim> &sc) {
	sc.printHelper(os);
	return os;
}

typedef scene<double, double, double, 3> scene3d;
typedef scene<double, double, float, 3> scene3ddf;
typedef scene<float, float, float, 3> scene3f;
typedef scene<double, double, double, 2> scene2d;
typedef scene<double, double, float, 2> scene2ddf;
typedef scene<float, float, float, 2> scene2f;

typedef boost::shared_ptr<scene3d> sp_scene3d;
typedef boost::shared_ptr<scene3ddf> sp_scene3ddf;
typedef boost::shared_ptr<scene3f> sp_scene3f;
typedef boost::shared_ptr<scene3d> sp_scene2d;
typedef boost::shared_ptr<scene3ddf> sp_scene2ddf;
typedef boost::shared_ptr<scene3f> sp_scene2f;

#endif // SCENE_HH
