/**********************************************************/
/*    Company:        FTN-IRAM-RT Novi Sad                */
/*                                                        */
/* MAJOR_NUMBER 5     UPDATE_NUMBER 4     BUILD_NUMBER 0  */
/**********************************************************/

#include "stdfix_emu.h"
#include <math.h>


int long_accum::mr_sr=4;
bool long_accum::shift_saturate=false;
bool fract::shift_saturate=false;
bool long_fract::shift_saturate=false;

void get_mr_sr(int* x)
{
	(*x) = long_accum::get_mr_sr();
}

void set_mr_sr(int x)
{
	long_accum::set_mr_sr(x);
}

long_accum::long_accum()
{
}

long_accum::long_accum(const long_accum &x)
{
	data = x.data;
}

long_accum::long_accum(const fract &x)
{
	data = x.data;
}

long_accum::long_accum(const long_fract &x)
{
	data = x.data;
}

long_accum::long_accum(double x)
{
	double integral;
	double value = fabs(x);
	double fractional = modf(value, &integral);
	int mr_srBackUp;

	if(integral >= 256 && x < 0)
	{
		data.setLo(0x0);
		data.setHi(0x0);
		data.setGuard(0x80);
		data.setDecimalPlace(1);
		return;
	}

	mr_srBackUp=mr_sr;
	mr_sr = 0x4;

	long long tmp = coyote_math::doubleToLongLong(integral,9);
	coyote_math::accumulator acc1(tmp,1,true);
	acc1 <<= 8;

	tmp = coyote_math::doubleToLongLong(fractional,1);
	coyote_math::accumulator acc2(tmp,1,true);

	acc1.setHi(acc1.getHi() & 0x80000000);
	acc1.setLo(0);

	acc2.setGuard(0);

	acc1 |= acc2;

	acc2.zero();

	if(x < 0)
		acc1 = acc2 - acc1;

	data = acc1;

	mr_sr = mr_srBackUp;
}

long_accum::long_accum(int x)
{
   long long tmp = coyote_math::doubleToLongLong((double)x,9);
   coyote_math::accumulator acc(tmp,1,true);
   data = acc;
   data <<= 8;
}

const long_accum & long_accum::operator =(const long_accum &x)
{
	data = x.data;
	return *this;
}

const long_accum & long_accum::operator =(const fract &x)
{
	data = x.data;
	return *this;
}

const long_accum & long_accum::operator =(double x)
{
	long_accum acc1(x);
	*this = acc1;
	return *this;
}

const long_accum & long_accum::operator =(long x)
{
   long long tmp = coyote_math::doubleToLongLong((double)x,9);
   coyote_math::accumulator acc(tmp,1,true);
   data = acc;
   data <<= 8;
   return *this;
}

const long_accum long_accum::abs() const
{
	long_accum acc;
	acc.data = data.abs();
	return acc;
}
const long_accum long_accum::operator-  () const
{
	long_accum acc;
	acc -= (*this);
	return acc;
}
const long_accum long_accum::operator~  () const
{
	long_accum acc;
	acc.data = ~data;
	return acc;
}

const bool long_accum::operator! ()
{
	long_accum t(*this);
	return !t.toLongLong();
}
/*
const long_accum long_accum::operator& (const long_accum& rhs) const
{
	long_accum acc;
	acc.data = data&rhs.data;
	return acc;
}
const long_accum long_accum::operator| (const long_accum& rhs) const
{
	long_accum acc;
	acc.data = data | rhs.data;
	return acc;
}

const long_accum long_accum::operator^ (const long_accum& rhs) const
{
	long_accum acc;
	acc.data = data ^ rhs.data;
	return acc;
}*/

long_accum long_accum::operator +(const long_accum &x)
{
	long_accum t;
	t.data = (data + x.data);
	return t;
}

long_accum long_accum::operator -(const long_accum &x)
{
	long_accum t;
	t.data = (data - x.data);
	return t;
}

long_accum long_accum::operator <<(const int &x)
{
	long_accum a0;
	a0.data = data;

	if(!shift_saturate)
		a0.data <<= x;
	else
	{
		if(a0 > long_accum(0))
		{
			for(int i = 0; i < x; i++)
			{
				a0.data <<= 1;
				if(a0 < long_accum(0))
				{
					a0 = long_accum(256);
					break;
				}
			}
		}
		else
		{
			for(int i = 0; i < x; i++)
			{
				a0.data <<= 1;
				if(a0 > long_accum(0))
				{
					a0 = long_accum(-256);
					break;
				}
			}
		}
	}

	return a0;
}

long_accum long_accum::operator >>(const int &x)
{
	long_accum t;
	t.data = data;
	t.data >>= x;
	return t;
}

long_accum long_accum::operator <<=(const int &x)
{
	long_accum t;
	t.data = data;
	t = t  << x;
	data = t.data;
	return *this;
}

long_accum long_accum::operator >>=(const int &x)
{
	data >>= x;
	return *this;
}

const long_accum & long_accum::operator +=(const long_accum &x)
{
	data = data + x.data;
	return *this;
}

const long_accum & long_accum::operator -=(const long_accum &x)
{
	data = data - x.data;
	return *this;
}

const long_accum& long_accum::operator++ ()
{
   long_accum acc1(*this);
   long_accum acc2(1);

   acc1 += acc2;
   data = acc1.data;
   return *this;
}
 
const long_accum long_accum::operator++ (int)
{
  long_accum acc = *this;
  ++(*this);
  return acc;
}

const long_accum& long_accum::operator-- ()
{
   long_accum acc1(*this);
   long_accum acc2(1);

   acc1 -= acc2;
   data = acc1.data;
   return *this;
}
 
const long_accum long_accum::operator-- (int)
{
  long_accum acc = *this;
  --(*this);
  return acc;
}

const long_accum long_accum::operator * (const long_accum &x) const
{
	long_accum b3(256);
	long_accum b2(0);
	long_accum b0(0);
	long_accum a0 = (*this);
	long_accum a2;
	long_accum a1 = x;

	a0.data = a0.data.abs();
	a1.data = a1.data.abs();

	a2.data = a0.data;

    long_accum b1(0);
	b1.data.setLo(1);

	b1.data = b1.data & a2.data;

	if(b1.data != long_accum(0).data)
		b2.data.setGuard(0x80);

	a0.data = long_accum(0).data;

	a2.data >>= 1;
	a2.data = a2.data & b3.data;

	for(int i = 0; i < 72; i+=1)
	{
		if(a1.data < long_accum(0).data)
		{
			if(b0.data >= long_accum(0).data)
			{
				b0.data = b0.data + b2.data;

				if(b2.data < long_accum(0).data)
				{
					if(b0.data >= long_accum(0).data)
					{
						b1.data = long_accum(0).data;
						b1.data.setLo(1);
						a0.data = a0.data + b1.data;
					}
				}
			}
			else
			{
				b0.data = b0.data + b2.data;

				if(b2.data >= long_accum(0).data)
				{
					if(b0.data >= long_accum(0).data)
					{
						b1.data = long_accum(0).data;
						b1.data.setLo(1);
						a0.data = a0.data + b1.data;
					}
				}
				else
				{
					b1.data = long_accum(0).data;
					b1.data.setLo(1);
					a0.data = a0.data + b1.data;
				}
			}
			a0.data = a0.data + a2.data;
		}

		b1.data = long_accum(0).data;
		b1.data.setLo(1);

		b2.data >>= 1;
		b2.data = b2.data & b3.data;
		b1.data = b1.data & a2.data;
		
		if(b1.data != long_accum(0).data)
		{
			b1.data =~ b3.data;
			b2.data = b2.data | b1.data;
		}

		a2.data >>= 1;
		a2.data = a2.data & b3.data;
		a1.data <<= 1;

	}

	for(int i = 0; i < 9; i+=1)
	{
		if(a0.data >= long_accum(0).data)
		{
			b1.data = long_accum(0).data;

			if(b0.data < long_accum(0).data)
				b1.data.setLo(1);

			a0.data <<= 1;
			a0.data = a0.data | b1.data;
			b0.data <<= 1;
		}
		else
		{
			a0.data = long_accum(0).data;
			b0.data = long_accum(0).data;
			a0.data =~ a0.data;
			b0.data =~ b0.data;
		}
	}

	if(a0.data != long_accum(0).data)
	{
		a2.data = (*this).data^x.data;

		if(a2.data < long_accum(0).data)
			a0.data = long_accum(0).data - a0.data;

		a1.data = a0.data ^ a2.data;

		if(a1.data < long_accum(0).data)
		{
			if(a2.data < long_accum(0).data)
				a0.data = long_accum(-256).data;
			else
				a0.data = long_accum(256).data;
		}
	}

	return a0;
}

const long_accum long_accum::operator /(const long_accum &x)
{
	long_accum a0 = (*this);
	long_accum a2;
	long_accum a1 = x;

	a0.data = a0.data.abs();
	a1.data = a1.data.abs();

	if(a1.data != long_accum(0).data)
	{
		int j = 63;
		long_accum a3 = long_accum(0);

		a3.data.setLo(1);

		while(a1.data >= long_accum(0).data)
		{
			a1.data <<= 1;
			j++;
		}

		a2.data = long_accum(0).data;
		
		if(a0.data < long_accum(0).data)
		{
			if(a0.data >= a1.data)
			{
				a0.data = a0.data - a1.data;
				a2.data.setLo(1);
			}
		}

		for(int i = 0; i < j; i++)
		{
			if(a2.data >= long_accum(0).data)
				a2.data <<= 1;
			else
			{
				//saturate check
				a2.data = long_accum(0).data;
				a2.data =~ a2.data;
			}

			if(a0.data >= long_accum(0).data)
			{
				a0.data <<= 1;

				if(a0.data < long_accum(0).data && a0.data >= a1.data )
				{
					a0.data = a0.data - a1.data;
					a2.data = a2.data | a3.data;
				}
			}
			else
			{
				a0.data <<= 1;
				a0.data = a0.data - a1.data;
				a2.data = a2.data | a3.data;
			}	
		}
		a0.data = a2.data;
	}
	
	if(a0.data != long_accum(0).data)
	{
		a2.data = (*this).data^x.data;

		if(a2.data < long_accum(0).data)
			a0.data = long_accum(0).data - a0.data;

		a1.data = a0.data ^ a2.data;

		if(a1.data < long_accum(0).data)
		{
			if(a2.data < long_accum(0).data)
				a0.data = long_accum(-256).data;
			else
				a0.data = long_accum(256).data;

		}
	}

	return a0;
}


double long_accum::toDouble() const 
{
	double d;
	double c;
	char unsignGuard;
	unsigned long long bitValue = 0;
	long_accum a = (*this);

	if(a < long_accum(0))
		a = long_accum(0) - a;
		
	unsignGuard =  a.data.getGuard();

	bitValue |= ((unsigned long long)a.data.getLo());
	bitValue |= ((unsigned long long)a.data.getHi()) << 32;

	d = bitValue/9223372036854775807.;

	bitValue = ((long long)unsignGuard) << 56;

	c = bitValue/36028797018963968.;

	d += bitValue/36028797018963968.;

	if((*this) < long_accum(0))
		d = -d;

	return d;
}

long long long_accum:: toLongLong() const 
{
	long long bitValue = 0;

	bitValue |= ((long long)data.getLo()) >> 8;
	bitValue |= ((long long)data.getHi()) << 24;
	bitValue |= ((long long)data.getGuard()) << 56;

	return bitValue;
}

long_accum::operator int() const
{
	long_accum acc1(*this);
	long_accum acc2(0);
	
	acc2.data.setHi(0x01FF);

	acc1 = acc1.abs();

	acc1.data.setLo(acc1.data.getHi());
	acc1.data.setHi(acc1.data.getGuard());
	acc1 = acc1 << 1;
	acc1.data = acc1.data & acc2.data;
	acc1.data.setGuard(0);	
	acc1.data.setLo(0);

	if((*this) < long_accum(0) )
		acc1 = long_accum(0) - acc1;

	return acc1.data.getHi();
}


fract::fract()
{
}

fract::fract(const long_accum &x)
{
	data = coyote_math::SRS(x.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
}

fract::fract(const long_fract &x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
}

fract::fract(const fract &x)
{
	data = x.data;
}

fract::fract(double x)
{
	int mr_srBackUp=long_accum::mr_sr;
	long_accum::mr_sr = 0x4;
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	long_accum::mr_sr = mr_srBackUp;
}

fract::fract(int x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
}

fract::fract(int x, bool hex)
{
	if(hex = true)
		data = x;
	else
		fract(x);
}

const fract & fract::operator =(const fract &x)
{
	data = x.data;
	return *this;
}

const fract & fract::operator =(const long_fract &x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

const fract & fract::operator =(const long_accum &x)
{
	data = coyote_math::SRS(x.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

const fract & fract::operator =(double x)
{
	int mr_srBackUp=long_accum::mr_sr;
	long_accum::mr_sr = 0x4;
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	long_accum::mr_sr = mr_srBackUp;
	return *this;
}

const fract & fract::operator =(long x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

fract fract::operator +(const fract &x)
{
	fract t;
	coyote_math::accumulator acc1 = data, acc2 = x.data;

	acc1 = acc1 + acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

fract fract::operator -(const fract &x)
{
	fract t;
	coyote_math::accumulator acc1 = data, acc2 = x.data;

	acc1 = acc1 - acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

fract fract::operator >>(const int &x)
{
	fract t;
	coyote_math::accumulator acc1 = data;

	acc1 >>= x;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

fract fract::operator <<(const int &x)
{
	fract t;
	long_accum acc1;
	bool accum_option = long_accum::get_shift_saturate();

	acc1.data = data;
	long_accum::set_shift_saturate(shift_saturate);
	acc1 <<= x;
	long_accum::set_shift_saturate(accum_option);
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

fract fract::operator <<=(const int &x)
{
	long_accum acc1;
	bool accum_option = long_accum::get_shift_saturate();

	acc1.data = data;
	long_accum::set_shift_saturate(shift_saturate);
	acc1 <<= x;
	long_accum::set_shift_saturate(accum_option);
	data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

fract fract::operator >>=(const int &x)
{
	coyote_math::accumulator acc1 = data;
	acc1 >>= x;
	data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

const fract fract::abs()
{
	long_accum acc;
	fract reg;

	acc.data = data;
	acc.data = acc.data.abs();
	reg.data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return reg;
}


const fract fract::operator- ()
{
	fract t(*this);
	t.data = -data;
	return t;
}

const bool fract::operator! ()
{
	fract t(*this);
	return !t.toLong();
}


const fract fract::operator~  ()
{
	fract t;
	coyote_math::accumulator acc = data;

	acc = ~acc;
	t.data = coyote_math::SRS(acc, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}
/*
const fract fract::operator& (const fract& rhs) const
{
	fract t;
	coyote_math::accumulator acc1 = data;
	coyote_math::accumulator acc2 = rhs.data;

	acc1 = acc1&acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}
const fract fract::operator| (const fract& rhs) const
{
	fract t;
	coyote_math::accumulator acc1 = data;
	coyote_math::accumulator acc2 = rhs.data;

	acc1 = acc1|acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const fract fract::operator^ (const fract& rhs) const
{
	fract t;
	coyote_math::accumulator acc1 = data;
	coyote_math::accumulator acc2 = rhs.data;

	acc1 = acc1^acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}*/

const fract& fract::operator +=(const fract &x)
{   
	fract t(*this);
	t = t + x;
	data = t.data;
	return *this;
}

const fract& fract::operator -=(const fract &x)
{
	fract t(*this);
	t = t - x;
	data = t.data;
	return *this;
}

const fract& fract::operator++ ()
{
   fract t1(*this);
   fract t2(1);

   t1 += t2;
   data = t1.data;
   return *this;
}
 
const fract fract::operator++ (int)
{
  fract t = *this;
  ++(*this);
  return t;
}

const fract& fract::operator-- ()
{
   fract t1(*this);
   fract t2(1);

   t1 -= t2;
   data = t1.data;
   return *this;
}
 
const fract fract::operator-- (int)
{
  fract t = *this;
  --(*this);
  return t;
}

long_accum fract::operator *(const fract &x) const
{
	long_accum t;
	t.data = data * x.data;
	return t;
}

long_accum fract::operator *(const unsigned &y) const
{
	long_accum acc;
	coyote_math::ufixed<1> uY = y/2147483648.;		// acc = x * (unsigned)y

	acc.data = data * uY;

	return acc;
}

const fract fract::operator / (const fract &x)
{
	fract t;
	long_accum acc1, acc2;

	acc1.data = data;
	acc2.data = x.data;

	acc1 = acc1 / acc2;
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const fract fract::operator / (const int &x)
{
	fract t;
	long_accum acc1, acc2(x);

	acc1.data = data;

	acc1 = acc1 / acc2;
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}


long_accum fract::operator *(const long &x) const
{
	long_accum acc;

	__int64 _y = ((__int64)data.toBinary())<<32;

	coyote_math::accumulator acc_data(x * _y,1,true);

	acc.data = acc_data;

	return acc;
}

fract::operator int() const
{
	long_accum acc1(*this);
	long_accum acc2(0);
	
	acc2.data.setHi(0x01FF);

	acc1 = acc1.abs();

	acc1.data.setLo(acc1.data.getHi());
	acc1.data.setHi(acc1.data.getGuard());
	acc1 = acc1 << 1;
	acc1.data = acc1.data & acc2.data;
	acc1.data.setGuard(0);	
	acc1.data.setLo(0);

	if((*this) < fract(0) )
		acc1 = long_accum(0) - acc1;

	return acc1.data.getHi();
}

double fract::toDouble() 
{
	return data.toBinary()/2147483648.;
}

long_fract::long_fract()
{
}

long_fract::long_fract(const long_accum &x)
{
	data = coyote_math::SRS(x.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
}

long_fract::long_fract(const long_fract &x)
{
	data = x.data;
}

long_fract::long_fract(const fract &x)
{
	data = x.data;
}

long_fract::long_fract(double x)
{
	int mr_srBackUp = long_accum::mr_sr;
	long_accum::mr_sr = 0x4;
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	long_accum::mr_sr = mr_srBackUp;
}

long_fract::long_fract(int x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
}

const long_fract & long_fract::operator =(const fract &x)
{
	data = x.data;
	return *this;
}

const long_fract & long_fract::operator =(const long_fract &x)
{
	data = x.data;
	return *this;
}


const long_fract & long_fract::operator =(const long_accum &x)
{
	data = coyote_math::SRS(x.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

const long_fract & long_fract::operator =(double x)
{
	int mr_srBackUp = long_accum::mr_sr;
	long_accum::mr_sr = 0x4;
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	long_accum::mr_sr = mr_srBackUp;
	return *this;
}

const long_fract & long_fract::operator =(long x)
{
	long_accum acc(x);
	data = coyote_math::SRS(acc.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

long_fract long_fract::operator +(const long_fract &x)
{
	long_fract t;
	coyote_math::accumulator acc1, acc2; 
	
	acc1= data; 
	acc2 = x.data;

	acc1 = acc1 + acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

long_fract long_fract::operator -(const long_fract &x)
{
	long_fract t;
	coyote_math::accumulator acc1, acc2; 
	
	acc1= data; 
	acc2 = x.data;

	acc1 = acc1 - acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

long_fract long_fract::operator >>(const int &x)
{
	long_fract t;
	coyote_math::accumulator acc1; 
	
	acc1 = data;

	acc1 >>= x;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

long_fract long_fract::operator <<(const int &x)
{
	long_fract t;
	long_accum acc1;
	bool accum_option = long_accum::get_shift_saturate();

	acc1.data = data;
	long_accum::set_shift_saturate(shift_saturate);
	acc1 <<= x;
	long_accum::set_shift_saturate(accum_option);
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

long_fract long_fract::operator <<=(const int &x)
{
	long_accum acc1;
	bool accum_option = long_accum::get_shift_saturate();
		
	acc1.data = data;
	long_accum::set_shift_saturate(shift_saturate);
	acc1 <<= x;
	long_accum::set_shift_saturate(accum_option);
	data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

long_fract long_fract::operator >>=(const int &x)
{
	coyote_math::accumulator acc1;
		
	acc1 = data;

	acc1 >>= x;
	data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return *this;
}

const long_fract long_fract::abs()
{
	long_fract t;
	coyote_math::accumulator acc;

	acc = data;

	acc = acc.abs();
	t.data = coyote_math::SRS(acc, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract long_fract::operator- ()
{
	long_fract t;
	coyote_math::accumulator acc;

	acc = data;

	acc =- acc;
	t.data = coyote_math::SRS(acc, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract long_fract::operator~  ()
{
	long_fract t;
	coyote_math::accumulator acc;
		
	acc = data;

	acc = ~acc;
	t.data = coyote_math::SRS(acc, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const bool long_fract::operator! ()
{
	long_fract t(*this);
	return !t.toLongLong();
}

/*
const long_fract long_fract::operator& (const long_fract& rhs)
{
	long_fract t;
	coyote_math::accumulator acc1, acc2; 

	acc1 = data;
	acc2 = rhs.data;

	acc1 = acc1 & acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}
const long_fract long_fract::operator| (const long_fract& rhs)
{
	long_fract t;
	coyote_math::accumulator acc1, acc2;

	acc1 = data;
	acc2 = rhs.data;

	acc1 = acc1 | acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract long_fract::operator^ (const long_fract& rhs)
{
	long_fract t;
	coyote_math::accumulator acc1, acc2; 

	acc1 = data;
	acc2 = rhs.data;

	acc1 = acc1 ^ acc2;
	t.data = coyote_math::SRS(acc1, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}*/

const long_fract long_fract::operator* (const long_fract &x)
{
	long_fract t;
	long_accum acc1, acc2;

	acc1.data = data;
	acc2.data = x.data;

	acc1 = acc1 * acc2;
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract long_fract::operator/ (const long_fract &x)
{
	long_fract t;
	long_accum acc1, acc2;

	acc1.data = data;
	acc2.data = x.data;

	acc1 = acc1 / acc2;
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract long_fract::operator / (const int &x)
{
	long_fract t;
	long_accum acc1, acc2(x);

	acc1.data = data;

	acc1 = acc1 / acc2;
	t.data = coyote_math::SRS(acc1.data, (coyote_math::shifting_t)(long_accum::mr_sr & 0x3), (coyote_math::rounding_t)((long_accum::mr_sr >> 2) & 0x3));
	return t;
}

const long_fract& long_fract::operator +=(const long_fract &x)
{   
	long_fract t(*this);
	t = t + x;
	data = t.data;
	return *this;
}

const long_fract& long_fract::operator -=(const long_fract &x)
{
	long_fract t(*this);
	t = t - x;
	data = t.data;
	return *this;
}

const long_fract& long_fract::operator++ ()
{
   long_fract t1(*this);
   long_fract t2(1);

   t1 += t2;
   data = t1.data;
   return *this;
}
 
const long_fract long_fract::operator++ (int)
{
  long_fract t = *this;
  ++(*this);
  return t;
}

const long_fract& long_fract::operator-- ()
{
   long_fract t1(*this);
   long_fract t2(1);

   t1 -= t2;
   data = t1.data;
   return *this;
}
 
const long_fract long_fract::operator-- (int)
{
  long_fract t = *this;
  --(*this);
  return t;
}

double long_fract::toDouble() const 
{
	double d;

	long long bitValue = 0;

	bitValue |= ((long long)data.getLo());
	bitValue |= ((long long)data.getHi()) << 32;

	d = bitValue/9223372036854775807.;

	return d;
}

long long long_fract::toLongLong() const 
{
	long long bitValue = 0;

	bitValue |= ((long long)data.getLo());
	bitValue |= ((long long)data.getHi()) << 32;

	return bitValue;
}

long_fract::operator int() const
{
	long_accum acc1(*this);
	long_accum acc2(0);
	
	acc2.data.setHi(0x01FF);

	acc1 = acc1.abs();

	acc1.data.setLo(acc1.data.getHi());
	acc1.data.setHi(acc1.data.getGuard());
	acc1 = acc1 << 1;
	acc1.data = acc1.data & acc2.data;
	acc1.data.setGuard(0);	
	acc1.data.setLo(0);

	if((*this) < fract(0) )
		acc1 = long_accum(0) - acc1;

	return acc1.data.getHi();
}
