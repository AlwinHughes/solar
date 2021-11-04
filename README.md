# solar
Software based ray tracing enging built in C

![Test Image](/demo/image_63.png)

## Compiling

Compiles on Linux using 
```make solar```
compilation requires xlib and libpng

## Using

Supplying the `-w` option l write the image generated to file

Supplying the `-h` option will hide the preview that is shown by default

## How it works

Rays are emitted out of a point in a grid. Their intersection is calculated with each object in the scene and the closest one is used to shade the pixel. 
More specifically the rays are grouped into a number of boxes and each box is processed one at a time. 
Each object can determine how it is shaded and has access to all the data in the scene. Currently written shaders include phong and phong with shadows but other could easily be added

### Types

The project revolves around a few structs

* `ray_t`
Contains vectors for the direction and start of the ray.

* `renderable_t`
Contains pointers to functions for getting the closest intersection in a given scene, getting the colour at an intersection. Also contains pointers to additional data needed by the previous functions.

* `light_t`
Contains the colour and position of a point light



* `intersection_t`
Contains the position and normal of the intersection, a pointer to the object and a flag to check if the intersection is empty

## What next
Using this foundation it should be possible to render any object as long as we can define a function for calculating the intersection with a ray. 

Additional features I would like to implement:

* Multithreading
* Use of vector registers using SIMD
* A way to save/load scenes


