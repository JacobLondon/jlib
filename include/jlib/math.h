#ifndef JLIB_MATH_H
#define JLIB_MATH_H

#include <stdint.h>

#define JLIB_CLAMP(Number, Max, Min) (((Number) > (Max)) ? (Max) : (((Number) < (Min)) ? (Min) : (Number)))
#define JLIB_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define JLIB_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define JLIB_ABS(x) ((x) < 0 ? -(x) : (x))

float jlib_fast_sqrtf(float number);
float jlib_fast_invsqrtf(float number);

/* Linux kernel functions */
uint64_t int_pow(uint64_t base, unsigned int exp);
unsigned long gcd(unsigned long a, unsigned long b);
unsigned long lcm(unsigned long a, unsigned long b);

#endif