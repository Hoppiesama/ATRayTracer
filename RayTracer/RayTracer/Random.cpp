#include "Random.h"


void Random::lcong48(unsigned short p[7])
{
	_rand48_seed[0] = p[0];
	_rand48_seed[1] = p[1];
	_rand48_seed[2] = p[2];
	_rand48_mult[0] = p[3];
	_rand48_mult[1] = p[4];
	_rand48_mult[2] = p[5];
	_rand48_add = p[6];
}

unsigned short * Random::seed48(unsigned short xseed[3])
{
	static unsigned short sseed[3];

	sseed[0] = _rand48_seed[0];
	sseed[1] = _rand48_seed[1];
	sseed[2] = _rand48_seed[2];
	_rand48_seed[0] = xseed[0];
	_rand48_seed[1] = xseed[1];
	_rand48_seed[2] = xseed[2];
	_rand48_mult[0] = RAND48_MULT_0;
	_rand48_mult[1] = RAND48_MULT_1;
	_rand48_mult[2] = RAND48_MULT_2;
	_rand48_add = RAND48_ADD;
	return sseed;
}

void Random::srand48(long seed)
{
	_rand48_seed[0] = RAND48_SEED_0;
	_rand48_seed[1] = (unsigned short)seed;
	_rand48_seed[2] = (unsigned short)(seed >> 16);
	_rand48_mult[0] = RAND48_MULT_0;
	_rand48_mult[1] = RAND48_MULT_1;
	_rand48_mult[2] = RAND48_MULT_2;
	_rand48_add = RAND48_ADD;
}

long Random::jrand48(unsigned short xseed[3])
{
	_dorand48(xseed);
	return ((long)xseed[2] << 16) + (long)xseed[1];
}

long Random::mrand48(void)
{
	_dorand48(_rand48_seed);
	return ((long)_rand48_seed[2] << 16) + (long)_rand48_seed[1];
}

long Random::nrand48(unsigned short xseed[3])
{
	_dorand48(xseed);
	return ((long)xseed[2] << 15) + ((long)xseed[1] >> 1);
}

long Random::lrand48(void)
{
	_dorand48(_rand48_seed);
	return ((long)_rand48_seed[2] << 15) + ((long)_rand48_seed[1] >> 1);
}

double Random::drand48(void)
{
	return erand48(_rand48_seed);
}

double Random::erand48(unsigned short xseed[3])
{
	_dorand48(xseed);
	return ldexp((double)xseed[0], -48) +
		ldexp((double)xseed[1], -32) +
		ldexp((double)xseed[2], -16);
}

void Random::_dorand48(unsigned short xseed[3])
{
	unsigned long accu;
	unsigned short temp[2];

	accu = (unsigned long)_rand48_mult[0] * (unsigned long)xseed[0] +
		(unsigned long)_rand48_add;
	temp[0] = (unsigned short)accu;        /* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long)_rand48_mult[0] * (unsigned long)xseed[1] +
		(unsigned long)_rand48_mult[1] * (unsigned long)xseed[0];
	temp[1] = (unsigned short)accu;        /* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += _rand48_mult[0] * xseed[2] + _rand48_mult[1] * xseed[1] + _rand48_mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short)accu;
}
