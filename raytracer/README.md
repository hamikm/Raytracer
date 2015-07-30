The easiest way to run is to type "make view". Make sure you have Eye of Gnome
and the Boost libraries installed. On Ubuntu you can install them like this: 
  
sudo apt-get install eog

sudo apt-get install libboost-all-dev
  
The Google test libraries needed for unit testing are included in this
repository and are automatically built by the makefile. There is fairly
good unit test coverage; look at some of the test suites to familiarize
yourself with the provided classes.

--------------------------------------------------------------------------------

This simple ray tracer was written for Caltech's CS11 advanced C++ track in 
summer 2015. The lab specifications have been uploaded to this project as a 
tarball in the root directory called labs.tar.gz. 

I implemented the following Donnie's suggestion (verbatim from email):

1)  Lights should also be scene objects, so the hierarchy is a bit better/
    richer.  SceneObject would have "Light" and "Shape" as children, and then 
    you could have different kinds of lights and different kinds of shapes.

2)  The scene description language needs lights.

TODO:

3)  It would be cool to add spotlights as well as area lights to the raytracer
    and to the scene description language.

4)  Shadows also aren't too hard to figure out.  When you have an intersection 
    with an object, just cast a ray to each light.  If the ray hits anything, 
    skip that light.

