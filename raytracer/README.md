This is a simple raytracer which uses my own math vector class. It doesn't
use OpenGL or any other graphics libraries; everything is from scratch just 
like grandma's cookies! Spheres, planes, cylinders, point lights, and 
reflections, and shadows are currently supported. I originally wrote this as a 
project for Caltech's CS11 class in summer 2015. Check out the sample image, 
sample.png, in the root directory. 

Also see example.dat in the root directoy for a demonstration of the scene 
description language which is used to specify the scenes that the raytracer
then renders. If you modify this file you can then type "make view" for a quick 
rendering of the new scene. Make sure you have Eye of Gnome and the Boost 
libraries installed. On Ubuntu you can install them like this: 

sudo apt-get install eog
  
sudo apt-get install libboost-all-dev
  
If you want more control over image dimensions, the file format, and so forth
type "./rt" and read the output for instructions. All comments are written so
that doxygen can auto-generate documentation for the source; just type "make
docs" to build it into the doc folder. There are some LaTeX formulas in the
documentation for tracing functions to hopefully facilitate understanding of
the math.

The labs which guided this project have been uploaded to the root directory as 
labs.tar.gz. The Google test libraries needed for unit testing are included in 
this repository and are automatically built by the makefile. There is fairly
good unit test coverage; look at some of the test suites to familiarize
yourself with the provided classes.
