#  Ray Tracer

A small ray tracer that i wrote while reading the ray tracer challenge.

## Building
run `cmake CMakeLists.txt` to generate the makefile
then `make`

## TODO
- clean the project file structure
- add support for building scenes from XML
- clean API
- optimizations:
    - cube-ray intersection
    - pre-compute the tranforms for children of groups
    - AABB optimization
    
- shapes to add
    - cones


## Progress timeline 

### end of chapter 6 
basic raytracing done,sphere shades correctly

<img src="/screenshots/sphere.png"/>

### end of chapter 7
added scenes,the possibility of having several children

<img src="screenshots/eoc_7.png" />

### end of chapter 8
added shadows

<img src="screenshots/eoc_8.png" />

### end of chapter 9
added planes

<img src="screenshots/eoc_9.png"/>

### end of chapter 10
added patterns and the option to nest them

<img src="screenshots/eoc_10.png" />

### reflections
added reflections 

<img src="screenshots/moc_11.png" /> 

### end of chapter 11
added reflections and refractions

<img src="screenshots/eoc11.png" />

### end of chapter 12
added support for cubes

<img src="screenshots/eoc12.png"/>

### end of chapter 13
added support for cylinders 

<img src="screenshots/eoc13_1.png"/>

### end of chapter 14
added groups

<img src ="screenshots/eoc_14.png"/>