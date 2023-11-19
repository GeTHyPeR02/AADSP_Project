/**********************************************************/
/*    Company:        FTN-IRAM-RT Novi Sad                */
/*                                                        */
/* MAJOR_NUMBER 5     UPDATE_NUMBER 4     BUILD_NUMBER 0  */
/**********************************************************/

#ifndef __STDFIX_EMU_H
#define __STDFIX_EMU_H

#if defined(__CCC)

#include <stdfix.h>

#define FRACT_NUM(x) (x##r)
#define LONG_FRACT_NUM(x) (x##lr)
#define ACCUM_NUM(x) (x##lk)

#define FRACT_NUM_HEX(x) (x##r)

#define FRACT_TO_INT_BIT_CONV(x) (bitsr(x))
#define INT_TO_FRACT_BIT_CONV(x) (rbits(x))

#define long_accum long accum
#define long_fract long fract

#else /* __C_PLUS_PLUS */

#include "fixed_point_math.h"

#define __memX
#define __memY
#define __memXY
#define __memP

#define FRACT_NUM(x) ((fract)(x))
#define LONG_FRACT_NUM(x) ((long_fract)(x))
#define ACCUM_NUM(x) ((long_accum)(x))

#define FRACT_NUM_HEX(x) (fract(x,true))

#define FRACT_NUM_SCALE(dividend,divisor) (fract((dividend##f)/divisor))

//scale value from the q.r format to _Fract format 
//where r represent the number of bits after dot
#define FRACT_NUM_SCALE_Q(value,q) FRACT_NUM_SCALE(value,_Q_CONST_##q)
//scale value from the q.r format to _Fract format 
//where r represent the number of bits after dot
#define FRACT_NUM_SCALE_R(value,r) FRACT_NUM_SCALE(value,_R_CONST_##r)

#define FRACT_TO_INT_BIT_CONV(x) (x.data.toBinary())
#define INT_TO_FRACT_BIT_CONV(x) (fract(x,true))

#define _Q_CONST_1 0
#define _Q_CONST_2 2
#define _Q_CONST_3 4
#define _Q_CONST_4 8
#define _Q_CONST_5 16
#define _Q_CONST_6 32
#define _Q_CONST_7 64
#define _Q_CONST_8 128
#define _Q_CONST_9 256
#define _Q_CONST_10 512
#define _Q_CONST_11 1024
#define _Q_CONST_12 2048
#define _Q_CONST_13 4096
#define _Q_CONST_14 8192
#define _Q_CONST_15 16384
#define _Q_CONST_16 32768
#define _Q_CONST_17 65536
#define _Q_CONST_18 131072
#define _Q_CONST_19 262144
#define _Q_CONST_20 524288
#define _Q_CONST_21 1048576
#define _Q_CONST_22 2097152
#define _Q_CONST_23 4194304
#define _Q_CONST_24 8388608
#define _Q_CONST_25 16777216
#define _Q_CONST_26 33554432
#define _Q_CONST_27 67108864
#define _Q_CONST_28 134217728
#define _Q_CONST_29 268435456
#define _Q_CONST_30 536870912
#define _Q_CONST_31 1073741824

#define _R_CONST_31 0
#define _R_CONST_30 2
#define _R_CONST_29 4
#define _R_CONST_28 8
#define _R_CONST_27 16
#define _R_CONST_26 32
#define _R_CONST_25 64
#define _R_CONST_24 128
#define _R_CONST_23 256
#define _R_CONST_22 512
#define _R_CONST_21 1024
#define _R_CONST_20 2048
#define _R_CONST_19 4096
#define _R_CONST_18 8192
#define _R_CONST_17 16384
#define _R_CONST_16 32768
#define _R_CONST_15 65536
#define _R_CONST_14 131072
#define _R_CONST_13 262144
#define _R_CONST_12 524288
#define _R_CONST_11 1048576
#define _R_CONST_10 2097152
#define _R_CONST_9 4194304
#define _R_CONST_8 8388608
#define _R_CONST_7 16777216
#define _R_CONST_6 33554432
#define _R_CONST_5 67108864
#define _R_CONST_4 134217728
#define _R_CONST_3 268435456
#define _R_CONST_2 536870912
#define _R_CONST_1 1073741824

//sets value of sr register
void set_mr_sr(int);
//returns value of sr register
void get_mr_sr(int*);

class fract;
class long_fract;
class long_accum;

//returns value of the accumulator low part
fract rgetlo(long_accum);
//returns bit representation of the accumulator low part
int bitsgetlo(long_accum);

//returns value of the accumulator high part
fract rgethi(long_accum);
//returns bit representation of the accumulator high part
int bitsgethi(long_accum);

//returns value of the accumulator guard part
fract rgetg(long_accum);
//returns bit representation of the accumulator guard part
int bitsgetg(long_accum);

class long_accum
{
public:
	long_accum();
	long_accum(const long_accum &x);
	long_accum(const fract &x);
	long_accum(const long_fract &x);
	long_accum(double x);
	long_accum(int x);

	const long_accum &operator =(const long_accum &x);
	const long_accum &operator =(const fract &x);
	const long_accum &operator =(double x);
	const long_accum &operator =(long x);

	long_accum operator +(const long_accum &x);
	long_accum operator -(const long_accum &x);
	long_accum operator <<(const int &x);
	long_accum operator >>(const int &x);
	long_accum operator >>=(const int &x);
	long_accum operator <<=(const int &x);

	bool         operator<  ( const long_accum& rhs) const {return (data < rhs.data);}
	bool         operator>  ( const long_accum& rhs) const {return (data > rhs.data);}
	bool         operator== ( const long_accum& rhs) const {return (data == rhs.data);}
	bool         operator!= ( const long_accum& rhs) const {return (data != rhs.data);}
	bool         operator<= ( const long_accum& rhs) const {return (data <= rhs.data);}
	bool         operator>= ( const long_accum& rhs) const {return (data >= rhs.data);}

	const long_accum abs() const;
	const long_accum operator-  () const;
	const long_accum operator~  () const;
	const bool operator! ();
//	const long_accum operator& (const long_accum& rhs) const;
//	const long_accum operator| (const long_accum& rhs) const;
//	const long_accum operator^ (const long_accum& rhs) const;

	const long_accum operator * (const long_accum &x) const;
	const long_accum operator * (const int &x) const{return long_accum::operator *(long_accum(x));};

	const long_accum operator /(const long_accum &x);
	const long_accum operator / (const int &x) {return long_accum::operator /(long_accum(x));};

	const long_accum& long_accum::operator++ ();
    const long_accum long_accum::operator++ (int);
	const long_accum& long_accum::operator-- ();
    const long_accum long_accum::operator-- (int);

/*	operator long long() const {return data.getHi();}
	double toDouble() const { double d = (double)(((__int64)data.getHi() << 32) | (unsigned int)data.getLo());
                          d *= 1.0842021724855044340074528008699e-19; return d;}*/

	double toDouble() const;
	long long toLongLong() const;

	operator int() const;

	const long_accum &operator +=(const long_accum &x);
	const long_accum &operator -=(const long_accum &x);

	static void set_mr_sr(int _mr_sr) { mr_sr = _mr_sr; }
	static int get_mr_sr() { return mr_sr; }
	static void set_shift_saturate(bool _shift_saturate) { shift_saturate = _shift_saturate; }
	static bool get_shift_saturate() { return shift_saturate; }

	coyote_math::accumulator data;
protected:
	friend class fract;
	friend class long_fract;
	friend class LogExp;
private:
	static int mr_sr;
	static bool shift_saturate;
};

class fract
{
public:
	fract();
	fract(const long_accum &x);
	fract(const long_fract &x);
	fract(const fract &x);
	fract(double x);
	fract(int x);
	fract(int x, bool hex);

	const fract &operator =(const fract &x);
	const fract &operator =(const long_fract &x);
	const fract &operator =(const long_accum &x);
	const fract &operator =(double x);
	const fract &operator =(long x);

	fract operator +(const fract &x);
	fract operator -(const fract &x);
	fract operator >>(const int &x);
	fract operator <<(const int &x);
	fract operator >>=(const int &x);
	fract operator <<=(const int &x);

	bool operator<  ( const fract& rhs) const {return (long_accum(*this) < long_accum(rhs));}
	bool operator>  ( const fract& rhs) const {return (long_accum(*this) > long_accum(rhs));}
	bool operator== ( const fract& rhs) const {return (long_accum(*this) == long_accum(rhs));}
	bool operator!= ( const fract& rhs) const {return (long_accum(*this) != long_accum(rhs));}
	bool operator<= ( const fract& rhs) const {return (long_accum(*this) <= long_accum(rhs));}
	bool operator>= ( const fract& rhs) const {return (long_accum(*this) >= long_accum(rhs));}

	long_accum operator * (const fract &x) const;
	long_accum operator * (const long &x) const;
	long_accum operator * (const unsigned &x) const;

	const fract operator / (const fract &x);
	const fract operator / (const int &x);

	long toLong() {return data.toBinary();}
	double toDouble();

	const fract abs();
	const fract operator- ();
	const fract operator~ ();
	const bool operator! ();
	//const fract operator& (const fract& rhs) const;
	//const fract operator| (const fract& rhs) const;
	//const fract operator^ (const fract& rhs) const;

	const fract &operator +=(const fract &x);
	const fract &operator -=(const fract &x);

	const fract& operator++ ();
    const fract operator++ (int);
	const fract& operator-- ();
    const fract operator-- (int);

	operator int() const;

	static void set_mr_sr(int _mr_sr) { long_accum::set_mr_sr(_mr_sr); }
	static void set_shift_saturate(bool _shift_saturate) { shift_saturate = _shift_saturate; }

	coyote_math::fixed<1> data;
protected:
	static bool shift_saturate;

	friend class long_accum;
	friend class LogExp;
};

class long_fract
{
public:
	long_fract();
	long_fract(const long_accum &x);
	long_fract(const long_fract &x);
	long_fract(const fract &x);
	long_fract(double x);
	long_fract(int x);

	const long_fract &operator =(const long_accum &x);
	const long_fract &operator =(const long_fract &x);
	const long_fract &operator =(const fract &x);
	const long_fract &operator =(double x);
	const long_fract &operator =(long x);

	long_fract operator +(const long_fract &x);
	long_fract operator -(const long_fract &x);
	long_fract operator <<(const int &x);
	long_fract operator >>(const int &x);
	long_fract operator >>=(const int &x);
	long_fract operator <<=(const int &x);

	bool         operator<  ( const long_fract& rhs) const {return (long_accum(*this) < long_accum(rhs));}
	bool         operator>  ( const long_fract& rhs) const {return (long_accum(*this) > long_accum(rhs));}
	bool         operator== ( const long_fract& rhs) const {return (long_accum(*this) == long_accum(rhs));}
	bool         operator!= ( const long_fract& rhs) const {return (long_accum(*this) != long_accum(rhs));}
	bool         operator<= ( const long_fract& rhs) const {return (long_accum(*this) <= long_accum(rhs));}
	bool         operator>= ( const long_fract& rhs) const {return (long_accum(*this) >= long_accum(rhs));}

	const long_fract abs();
	const long_fract operator-  ();
	const long_fract operator~  ();
	const bool operator! ();
	//const long_fract operator& (const long_fract& rhs);
	//const long_fract operator| (const long_fract& rhs);
	//const long_fract operator^ (const long_fract& rhs);

	const long_fract operator * (const long_fract &x);
	const long_fract operator * (const int &x) {return long_fract::operator *(long_fract(x));};

	const long_fract operator / (const long_fract &x);
	const long_fract operator / (const int &x);

	const long_fract& long_fract::operator++ ();
    const long_fract long_fract::operator++ (int);
	const long_fract& long_fract::operator-- ();
    const long_fract long_fract::operator-- (int);

	double toDouble() const;
	long long toLongLong() const;

	operator int() const;

	const long_fract &operator +=(const long_fract &x);
	const long_fract &operator -=(const long_fract &x);

	static void set_mr_sr(int _mr_sr) { long_accum::set_mr_sr(_mr_sr); }
	static void set_shift_saturate(bool _shift_saturate) { shift_saturate = _shift_saturate; }

	coyote_math::lfixed data;
protected:
	static bool shift_saturate;

	friend class fract;
	friend class LogExp;
};

#endif /* __C_PLUS_PLUS */

#endif
