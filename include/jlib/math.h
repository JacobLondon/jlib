#ifndef JLIB_MATH_H
#define JLIB_MATH_H

#define JLIB_CLAMP(Number, Max, Min) (((Number) > (Max)) ? (Max) : (((Number) < (Min)) ? (Min) : (Number)))
#define JLIB_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define JLIB_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define JLIB_ABS(x) ((x) < 0 ? -(x) : (x))

float jlib_fast_sqrtf(float number);
float jlib_fast_invsqrtf(float number);

#endif