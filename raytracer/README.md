This is a simple raytracer which uses my own math vector class. It doesn't
use OpenGL or any other graphics libraries; everything is from scratch just 
like grandma's cookies! I know that reinventing the wheel is bad; I wrote 
this to practice C++ :-).  Spheres, planes, cylinders, point lights, 
spotlights, area lights, reflections, and shadows are currently supported.
Check out this sample render:

<img src="sample.png" width="512px" height="320">

Also see example.dat in the root directoy for a demonstration of the scene 
description language which is used to specify the scenes. If you modify this 
file you can then type "make view" to render the new scene. Make sure you have 
Eye of Gnome and the Boost libraries installed. On Ubuntu you can install them 
like this by apt-getting eog and libboost-all-dev.
  
If you want more control over image dimensions, the file format, and so forth
type "./rt" and read the output for instructions. All comments are written so
that doxygen can auto-generate documentation for the source; just type "make
docs" to build it into the doc folder. There are some LaTeX formulas in the
docs to help with comprehension of the math.

The Google test libraries needed for unit testing are included in 
this repository and are automatically built by the makefile. There is fairly
good unit test coverage; look at some of the test suites to familiarize
yourself with the provided classes.
