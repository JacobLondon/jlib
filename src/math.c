#include <stdlib.h>

#include <jlib/math.h>
#include <jlib/util.h>

float fast_sqrtf(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		int i; /* assume sizeof(float) == sizeof(int) */
	} conv = { number }; /* member 'f' set to value of 'number'. */
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= (threehalfs - (x2 * conv.f * conv.f ));
	return 1.0f / conv.f;
}

float fast_invsqrtf(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		int i; /* assume sizeof(float) == sizeof(int) */
	} conv = { number }; /* member 'f' set to value of 'number'. */
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= (threehalfs - (x2 * conv.f * conv.f ));
	return conv.f;
}

int rand_range(int min, int max)
{
	return rand() % (max - min) + min;
}

float rand_uniform(void)
{
	return (float)rand() / (float)RAND_MAX;
}

/**
 * From Linux kernel int_pow.c
 * 
 * int_pow - computes the exponentiation of the given base and exponent
 * @base: base which will be raised to the given power
 * @exp: power to be raised to
 *
 * Computes: pow(base, exp), i.e. @base raised to the @exp power
 */
unsigned long long int_pow(unsigned long long base, unsigned int exp)
{
	unsigned long long result = 1;

	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}

/* If normalization is done by loops, the even/odd algorithm is a win. */
unsigned long gcd(unsigned long a, unsigned long b)
{
	unsigned long r = a | b;

	if (!a || !b)
		return r;

	/* Isolate lsbit of r */
	r &= -r;

	while (!(b & r))
		b >>= 1;
	if (b == r)
		return r;

	for (;;) {
		while (!(a & r))
			a >>= 1;
		if (a == r)
			return r;
		if (a == b)
			return a;

		if (a < b)
			swap(&a, &b);
		a -= b;
		a >>= 1;
		if (a & r)
			a += b;
		a >>= 1;
	}
}

/* Lowest common multiple */
unsigned long lcm(unsigned long a, unsigned long b)
{
	if (a && b)
		return (a / gcd(a, b)) * b;
	else
		return 0;
}

double fast_sin(double x) {
	int k;
	double y;
	double z;
	union {
		double f;
		int i;
	} s;

	z = x;
	z *= 0.3183098861837907;
	z += 6755399441055744.0;
	s.f = z;
	k = s.i;
	z = k;
	z *= 3.1415926535897932;
	x -= z;
	y = x;
	y *= x;
	z = 0.0073524681968701;
	z *= y;
	z -= 0.1652891139701474;
	z *= y;
	z += 0.9996919862959676;
	x *= z;
	k &= 1;
	k += k;
	z = k;
	z *= x;
	x -= z;

	return x;
}

double fast_cos(double x)
{
	return fast_sin(x + 3.14159265358979323846 / 2.0);
}