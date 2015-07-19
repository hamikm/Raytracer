/**
 * @file
 * @author Hamik Mukelyan
 */

#include "sceneobj.hh"
#include "ray.hh"
#include "light.hh"
#include "shape.hh"
#include "camera.hh"
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
 * Represents a 3D scene as a collection of shape object pointers and light
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
	 * An STL vector of pointers to all the lights in the scene.
	 */
	std::vector<light<vec_T, color_T, time_T, dim> * > lights;

	/**
	 * An STL vector of pointers to all the shapes in the scene.
	 */
	std::vector<shape<vec_T, color_T, time_T, dim> * > shapes;

public:

	/**
	 * Constructs an empty scene.
	 */
	scene() { }

	/**
	 * Steps through the vector of light pointers and vector of shape pointers,
	 * deleting each one.
	 *
	 * @warning Make sure that the lights and shapes added with @c addLight or
	 *   @c addShape are not deleted before or after this destructor runs.
	 */
	~scene() {
		typename std::vector<light<vec_T, color_T, time_T, dim> * >::iterator
			iter;
		for (iter = lights.begin(); iter < lights.end(); iter++) {
			delete *iter;
		}

		typename std::vector<shape<vec_T, color_T, time_T, dim> * >::iterator
			iter2;
		for (iter2 = shapes.begin(); iter2 < shapes.end(); iter2++) {
			delete *iter2;
		}
	}

	/**
	 * Adds an object to the scene. Assumes it was heap-allocated before
	 * invocation of this function.
	 *
	 * @param obj The shape to add to this scene.
	 */
	void addShape(shape<vec_T, color_T, time_T, dim> *obj) {
		assert(obj != 0);
		shapes.push_back(obj);
	}

	/**
	 * Adds a light to the scene. Assumes it was heap-allocated before
	 * function invocation.
	 *
	 * @param theLight The light to add to this scene.
	 */
	void addLight(light<vec_T, color_T, time_T, dim> *theLight) {
		assert(theLight != 0);
		lights.push_back(theLight);
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
	shape<vec_T, color_T, time_T, dim> * findClosestShape(
			const ray<vec_T, time_T, dim> &r, time_T &tIntersect) const {
		time_T t = RAY_MISS;
		tIntersect = RAY_MISS;
		shape<vec_T, color_T, time_T, dim> *intersectedObjPtr = 0, *tmpPtr;
		typename
			std::vector<shape<vec_T, color_T, time_T, dim> * >::const_iterator
			iter;
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
	 * @param r The ray whose color will be determined.
	 *
	 * @return The color of the given ray or @c DEFAULT_BKCOLOR if there is no
	 * intersection.
	 */
	rgbcolor<color_T> traceRay(const ray<vec_T, time_T, dim> &r) const {
		time_T tIntersect; // out parameter
		shape<vec_T, color_T, time_T, dim> *intersectedObjPtr =
				findClosestShape(r, tIntersect);
		if(intersectedObjPtr == 0) {
			return DEFAULT_BKCOLOR;
		}

		// Loop over all the lights, summing the color contributions from
		// each one
		rgbcolor<color_T> finalColor;
		typename
			std::vector<light<vec_T, color_T, time_T, dim> * >::const_iterator
			iter;
		for (iter = lights.begin(); iter < lights.end(); iter++) {
			mvector<vec_T, dim> intersectionPt = r.getPointAtT(tIntersect);
			light<vec_T, color_T, time_T, dim> *currLightPtr = *iter;
			mvector<vec_T, dim> L = currLightPtr->getPos() - intersectionPt;
			L = L.norm();
			mvector<vec_T, dim> N =
					intersectedObjPtr->surfaceNorm(intersectionPt);
			vec_T LdotN = L * N;
			if(LdotN > 0) {
				finalColor += (currLightPtr->getColor() *
						intersectedObjPtr->getColor() * LdotN);
			}
		}

		return finalColor;
	}

	/**
	 * Renders this scene for the given camera and image size as a PPM
	 * formatted image to the given output stream.
	 *
	 * @param cam The camera in this scene.
	 * @param imgSize The width and height of the image.
	 * @param os The output stream to which the PPM image will be written.
	 */
	void renderPPM(const camera<vec_T, time_T, dim> &cam,
			int imgSize,
			std::ostream &os) const {
		os << "P3 " << imgSize << " " << imgSize << " "
				<< COLORMAX << std::endl;
		for (int y = 0; y < imgSize; y++) {
			for (int x = 0; x < imgSize; x++) {
				ray<vec_T, time_T, dim> pixelRay = cam.getRayForPixel(
						x, y, imgSize);
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
		os << "scene:" << std::endl;
		os << "  lights:" << std::endl;
		typename
			std::vector<light<vec_T, color_T, time_T, dim> * >::const_iterator
			iter;
		for (iter = lights.begin(); iter < lights.end(); iter++) {
			os << "    " << **iter << std::endl;
		}

		os << "  shapes:" << std::endl;
		typename
			std::vector<shape<vec_T, color_T, time_T, dim> * >::const_iterator
			iter2;
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

#endif // SCENE_HH
