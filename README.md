# Raytracer

# Usage
The program (raycast) should have this usage pattern:
 * raytrace width height input.json output.ppm  
 
The data file is derived from CSV (Comma Seperated Values). In this format, there is one object
per line. The first element declares the kind of object (e.g., camera, sphere, or plane). The
remaining fields are properties. Each property name ends with a colon and is followed either by
a scalar value or a value tuple.

# Example Scene
camera, width: 2.0, height: 2.0 <br>
sphere, radius: 2.0, reflectivity: 0.2, diffuse_color: [1, 0, 0], specular_color: [1, 1, 1], position: [0, 1, -5] <br>
plane, normal: [0, 1, 0], diffuse_color: [0, 1, 0], specular_color: [1, 1, 1], position: [0, -1, 0], reflectivity: 0.3 <br>
light, color: [2, 2, 2], theta: 0, radial-a2: 0.125, radial-a1: 0.125, radial-a0: 0.125, position: [1, 3, -1] <br>

# Known Issues
Shading is currently incorrectly being calculated
