#include "v3math.h"

// Function Implementation
// A and B are 3D Vectors
//     [Vx]
// V = [Vy]
//     [Vz]
void v3_from_points(float *dst, float *a, float *b) {
    // Head - Tail
    // v3 = b - a
    v3_subtract(dst, b, a);
}

void v3_copy( float *dst, float *a){
    dst[0] = a[0];
    dst[1] = a[1];
    dst[2] = a[2];
}

void v3_add(float *dst, float *a, float *b) {
    //        [Ax + Bx]
    // A+B =  [Ay + By]
    //        [Az + Bz]
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}

void v3_subtract(float *dst, float *a, float *b) {
    //        [Ax - Bx]
    // A-B =  [Ay - By]
    //        [Az - Bz]
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

float v3_dot_product(float *a, float *b) {
    // A*B  =  AxBx    +     AyBy    +     AzBz
    return (a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]);
}

void v3_cross_product(float *dst, float *a, float *b) {
    //            [AyBz - AzBy]
    //  A*B   =   [AzBx - AxBz]
    //            [AxBy - AyBx]
    dst[0] = (a[1]*b[2] - a[2]*b[1]);
    dst[1] = (a[2]*b[0] - a[0]*b[2]);
    dst[2] = (a[0]*b[1] - a[1]*b[0]);
}

void v3_scale(float *dst, float s) {
    //          [Ax * s]
    //  s*A =   [Ay * s]
    //          [Az * s]
    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;
}

float v3_angle(float *a, float *b) {
    // angle = cos^-1(A * B / ||A|| * ||B||)
    return (float) acos(v3_angle_quick(a, b));
} 
float v3_angle_quick(float *a, float *b) {
    // A * B / ||A|| * ||B||
    return (v3_dot_product(a, b) / (v3_length(a) * v3_length(b)));
}

void v3_reflect(float *dst, float *v, float *n) {
    // Vr = V - 2(N * V)N
    float dotProd = v3_dot_product(n, v) * 2.0;

    v3_scale(n, dotProd);
    v3_subtract(dst, v, n);
}

float v3_length(float *a) {
    // ||A|| = sqrt(Ax^2 + Ay^2 + Az^2)
    return sqrt((a[0] * a[0]) + (a[1] * a[1]) + (a[2] * a[2]));
}

void v3_normalize(float *dst, float *a) {
    //       [Ax  / ||A||]
    //       [Ay  / ||A||]
    //       [Az  / ||A||]
    dst[0] = a[0] / v3_length(a);
    dst[1] = a[1] / v3_length(a);
    dst[2] = a[2] / v3_length(a);
    
}

bool v3_equals(float *a, float *b, float tolerance) {
    bool xEquiv, yEquiv, zEquiv;
    float difference[3];

    v3_subtract(difference, a, b);

    xEquiv = fabs(difference[0]) <= tolerance;
    yEquiv = fabs(difference[1]) <= tolerance;
    zEquiv = fabs(difference[2]) <= tolerance;

    return xEquiv && yEquiv && zEquiv;
}

void v3_setZeros(float *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
}

void v3_print(float *a) {
    printf("[%f, %f %f]\n", a[0], a[1], a[2]);
}