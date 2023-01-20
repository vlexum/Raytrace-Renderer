# Raytracer

# Usage
The program (raycast) should have this usage pattern:
 * raytrace width height input.json output.ppm  
 
The data file is derived from CSV (Comma Seperated Values). In this format, there is one object
per line. The first element declares the kind of object (e.g., camera, sphere, or plane). The
remaining fields are properties. Each property name ends with a colon and is followed either by
a scalar value or a value tuple.

* camera, width: 0.5, height: 0.5  
* sphere, color: [1.0, 0, 0], position: [0, 2, 5], radius: 2  
* plane, color: [0, 0, 1.0], position: [0, 0, 0], normal: [0, 1, 0]  


# Known Issues
Shading is currently incorrectly being calculated
