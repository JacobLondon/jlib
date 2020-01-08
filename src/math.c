#include <stdint.h>

#include <jlib/math.h>

float jlib_fast_sqrtf(float number)
{
    int i;
    float x, y;
    x = number * 0.5;
    y = number;
    i = *(int *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));
    return number * y;
}

float jlib_fast_invsqrtf(float number)
{
    const float x2 = number * 0.5F;
    const float threehalfs = 1.5F;

    union {
        float f;
        uint32_t i;
    } conv = { number }; /* member 'f' set to value of 'number'. */
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= (threehalfs - (x2 * conv.f * conv.f ));
    return conv.f;
}
