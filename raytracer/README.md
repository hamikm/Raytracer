The easiest way to run is to type "make view". Make sure you have Eye of Gnome
and the Boost libraries installed. On Ubuntu you can install them like this: 
  
  sudo apt-get install eog
  sudo apt-get install libboost-all-dev
  
--------------------------------------------------------------------------------

This simple ray tracer was written for CS11 advanced C++ track in summer
2015. Donnie decided to give me credit for it as long as I did all the
work for labs one to seven and implemented the following: 

1)  Lights should also be scene objects, so the hierarchy is a bit better/
    richer.  SceneObject would have "Light" and "Shape" as children, and then 
    you could have different kinds of lights and different kinds of shapes.

2)  The scene description language needs lights.

3)  It would be cool to add spotlights as well as area lights to the raytracer.

4)  Shadows also aren't too hard to figure out.  When you have an intersection 
    with an object, just cast a ray to each light.  If the ray hits anything, 
    skip that light.
    
The lab specifications have been uploaded to this project as a tarball in the
root directory called labs.tar.gz.

