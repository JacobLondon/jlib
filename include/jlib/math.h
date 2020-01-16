#ifndef JLIB_MATH_H
#define JLIB_MATH_H

#define CLAMP(x, xmax, xmin) (((x) > (xmax)) ? (xmax) : (((x) < (xmin)) ? (xmin) : (x)))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define ABS(x) ((x) < 0 ? -(x) : (x))

float fast_sqrtf(float number);
float fast_invsqrtf(float number);

/* Linux kernel functions */
unsigned long long int_pow(unsigned long long base, unsigned int exp);
unsigned long gcd(unsigned long a, unsigned long b);
unsigned long lcm(unsigned long a, unsigned long b);

#endif