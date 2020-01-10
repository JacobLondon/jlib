#ifndef JLIB_MATH_H
#define JLIB_MATH_H

#include <stdint.h>

#define CLAMP(v, max, min) (((v) > (max)) ? (max) : (((v) < (min)) ? (min) : (v)))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define ABS(x) ((x) < 0 ? -(x) : (x))

float fast_sqrtf(float number);
float fast_invsqrtf(float number);

/* Linux kernel functions */
uint64_t int_pow(uint64_t base, unsigned int exp);
unsigned long gcd(unsigned long a, unsigned long b);
unsigned long lcm(unsigned long a, unsigned long b);

#endif