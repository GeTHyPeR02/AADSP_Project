/*============================================================================
    MAJOR_NUMBER 5     UPDATE_NUMBER 4     BUILD_NUMBER 0             

coyote_math contains a set of C++ classes that implement fixed-point
arithmetic in a manner consistent with the coyote DSP. The classes are
designed to be helpful in detecting algorithmic problems as early as
possible. To this end, the classes have tight compile-time restrictions,
and throw exceptions at runtime if certain assumptions about fixed-point
math operations are violated.

The classes and operations are defined with two goals in mind: to allow
arithmetic operations to be expressed as naturally as possible in C (C++),
but at the same time to constrain the operations to be arithmetically
equivalent to the coyote DSP ALU. Furthermore, the assignment operators
enforce good decimal point bookkeeping.

Within the coyote_math namespace, there are several classes that are
defined:

coyote_math::fixed<numIntegerBits>
  This class represents a signed, 32-bit, fixed point number, with the
  specified numIntegerBits of msbits considered to be "left" of the
  decimal point.

coyote_math::ufixed<numIntegerBits>
  This class represents an unsigned, 32-bit, fixed point number, with the
  specified numIntegerBits of msbits considered to be "left" of the
  decimal point.

coyote_math::accumulator
  This class represents fixed-point values that are intermediate
  calculation results, typically the result of multiply or add operations.
  An accumulator object is analogous to the accumulators in the coyote
  ALU.

coyote_math::SRS
  This class represents the shift-round-saturate logic units of the coyote
  DSP. It is used as an adapter class to permit assignment of an accumulator
  to a fixed point variable.

The accumulator class supports the <<= and >>= shift operators, allowing the
decimal point to be moved. Fixed or ufixed variables have their decimal place
determined at compile time, and they may not be changed.

A fixed or ufixed variable declared with a given number of integer bits may NOT
be assigned a fixed-point value with a different number of integer bits --
such an assignment results in a coyote_math::alignment_exception_t exception.
The fixed and ufixed classes support all of the comparison operators. The
fixed type supports unary - as well. Variables of both types may be initialized
with floating point constants (e.g. 3.5). An exception will be thrown if the
initial constant value is outside the range that can be represented by the type.

Variables of type ufixed support assignment of an accumulator variable or another
ufixed variable on the right hand side of the assignment operator. In both cases,
attempted assignment of a value with a different numIntegerBits results in a
coyote_math::alignment_exception_t exception. When an accumulator is assigned
to a ufixed variable, no overflow, rounding, or saturation conditions are
checked.

Variables of type fixed support assignment of another fixed variable, or
assignment of an SRS variable. As with ufixed assignment, attempted assignment
of unequal numIntegerBits values results in a coyote_math::alignment_exception_t
exception.

The SRS adapter class is intended to be used when assigning an accumulator
value to a fixed variable. The SRS constructor takes an accumulator as a
parameter, and a shifting mode and rounding mode constant which specify how
the 72-bit accumulator value is to be converted to the 32-bit fixed value.
Supported shifting and rounding modes are the same as those supported by the
coyote DSP SRS logic.

Operators:
	Operators supported by the coyote_math package are described below. In
	these descriptions, acc is an accumulator, sf is a fixed variable,
	uf is a ufixed variable, f is either a fixed or a ufixed variable.

	f * f    // automatically shifts result left 1 bit, result type is acc
	f * f    // automatically shifts result left 1 bit, result type is acc
	f * f    // automatically shifts result left 1 bit, result type is acc
	acc = f;
	acc+= f;
	acc-= f;
	acc = acc
	acc+= acc
	acc-= acc
	acc + acc
	acc - acc
	acc > acc
	acc < acc
	acc == acc
	acc != acc
	acc >= acc
	acc <= acc
	acc ^= acc            // bitwise exclusive OR
	acc |= acc            // bitwise OR
	acc &= acc            // bitwise AND
	acc.abs()             // absolute value of acc
	acc.zero()            // set acc to zero in 1.63 format
	-acc                  // arithmetic negation of acc
	~acc                  // bitwise complement of acc
	acc >>= <int>
	acc <<= <int>

	sf = SRS(acc,<shift>,<round>);
	uf = acc;                        // no shifting, rounding, or saturation
	f  = <floating point constant>;
	sf = <long constant>;

Other methods:
    acc.format(bool decimal=true);   // returns acc formatted value as std::string
	sf.format (bool decimal=true);   // same for sfixed
	uf.format (bool decimal=true);   // same for ufixed
	acc.multiplyNoShift(f,f);        // same as acc = f * f, but does NOT
	                                 //      do the auto 1-bit shift left

Examples:
	using namespace coyote_math;

	fixed<1> half = 0.5;             // half is in 1.31 format
	fixed<1> quarter = 0.25;         // quarter is in 1.31 format
	fixed<1> eighth;                 // eighth will be in 1.31 format
	accumulator a0;
	a0 = half * quarter;             // a0 = .5 * .25 = .125
 									 //    multiply automatically shifts left 1 bit,
									 //    as in coyote core, so a0 is now in 1.63 format
	eighth = SRS(a0,sNONE,r2ZERO);   // eighth is now .125 in 1.31 format

/=========================================================================================
LogExp
	Usage:
	LogExp block have to be used in same way as in assembler.
	There are only to functions visible for LogExp block:

	void LogExpIn(LogExp_function left_fun, LogExp_operator left_op, void *x, LogExp_function right_fun, LogExp_operator right_op, void *y);

	This function is similar to ext(0xdHHH) instruction
	x and y operands are x and y register and can be any of fixed and ufixed classes
	left_fun and right_fun operands are functions(nop,log,exp,sm). They are declared in enum LogExp_function {nop, log, exp, sm}
	left_op and right_op operands are input multiplexers(norm32,norm64,X,Y,X-Y,Y-X,X>>1,Y>>1). They are declared in enum LogExp_operator {norm32, norm64, _X_, _X_Y_, _X_SHR_1_, _Y_, _Y_X_, _Y_SHR_1_}

	void LogExpOut(void* x, void* y);
	This function is similar to instruction Xn,Yn = logexp
	x and y operands are x and y register and can be any of fixed and ufixed classes

Examples:
	LogExpIn(log,norm32,&x,nop,norm64,&y)   =	logexp X=log(norm32(Xn)) Y=nop(Norm64(Yn))

	LogExpOut(&x,&y)						=	Xn,Yn = logexp

=======================================================================*/
#pragma once
#include <stdexcept>
#include <string>

#define CONST64( x) __int64(x)

namespace coyote_math {

extern unsigned long doubleToUlong(double v, unsigned char integerBits);
extern          long doubleToLong (double v, unsigned char integerBits);
extern long long doubleToLongLong(double v, unsigned char integerBits);


//class exception_t : public exception
class exception_t
{
public:
	exception_t(const std::string& what) : message(what) {}
	virtual const char *what( ) const { return message.c_str(); }
protected:
	exception_t() {}
	std::string message;
};

class alignment_exception_t : public exception_t
{
public:
	alignment_exception_t(unsigned char lhsDecs, unsigned char rhsDecs, const std::string& txt);
};

class range_exception_t : public exception_t
{
public:
	range_exception_t(const std::string& what) : exception_t(what) {}
};

enum rounding_t { rNONE, rHALF, r2ZERO, rDITHER };
enum shifting_t { sNONE, sRIGHT1, sRIGHT2, sLEFT1 };

class fixed_t; //forward
class ufixed_t; // forward
class SRS;         // forward
class lfixed;         // forward

class accumulator
{
	friend class SRS;
	friend class ufixed_t;
public:
	typedef char guard_t;
	accumulator() : lo(0xcdcdcdcd),hi(0xeeeeeeee),_guard(3),decimalPlace(15) {}
	accumulator(const accumulator& other)                        { operator =(other); }
	accumulator& operator = ( const accumulator& rhs)            { lo=rhs.lo; hi=rhs.hi; _guard=rhs._guard; decimalPlace=rhs.decimalPlace; return *this; }
	accumulator& operator = (const SRS& rhs);
	accumulator& operator = ( const lfixed& rhs);
	accumulator& operator+= ( const accumulator& rhs);
	accumulator& operator-= ( const accumulator& rhs);
	accumulator& operator^= ( const accumulator& rhs);
	accumulator& operator&= ( const accumulator& rhs);
	accumulator& operator|= ( const accumulator& rhs);
	const accumulator  operator~  () const;
	const accumulator  operator-  () const;
	const accumulator abs() const;
	accumulator& zero()                                          { lo=hi=_guard=0; decimalPlace=1; return *this;}
	bool         operator<  ( const accumulator& rhs) const;
	bool         operator>  ( const accumulator& rhs) const;
	bool         operator== ( const accumulator& rhs) const;
	bool         operator== ( const std::string& rhs) const      { return format(false) == rhs; }
	bool         operator!= ( const accumulator& rhs) const      { return !(operator==(rhs));}
	bool         operator<= ( const accumulator& rhs) const      { return !operator>(rhs); }
	bool         operator>= ( const accumulator& rhs) const      { return !operator<(rhs); }
	accumulator(const ufixed_t& uf)                              { *this = uf; }
	accumulator& operator = ( const ufixed_t& rhs);
	accumulator& operator+= ( const ufixed_t& rhs)               { return operator+=(accumulator(rhs)); }
	accumulator& operator-= ( const ufixed_t& rhs)               { return operator-=(accumulator(rhs)); }
	accumulator(const fixed_t& f)                                { *this = f; }
	accumulator& operator = ( const fixed_t& rhs);
	accumulator& operator+= ( const fixed_t& rhs)                { return operator+=(accumulator(rhs)); }
	accumulator& operator-= ( const fixed_t& rhs)                { return operator-=(accumulator(rhs)); }
	accumulator& operator>>=( unsigned short shift );
	accumulator& operator<<=( unsigned short shift );
	accumulator(__int64 value, unsigned char numDecs, bool signExtend);
	inline unsigned char num_int_bits() const                    { return decimalPlace; }
	const std::string format(bool decimal=true) const;
	static const accumulator multiply(const ufixed_t& lhs, const ufixed_t& rhs);
	static const accumulator multiply(const ufixed_t& lhs, const fixed_t& rhs);
	static const accumulator multiply(const fixed_t& lhs, const fixed_t& rhs);
	static const accumulator multiplyNoShift(const fixed_t& lhs, const fixed_t& rhs);
	static const accumulator multiply(const accumulator& lhs, unsigned char powerOfTwo);
	accumulator(guard_t _g, unsigned long _hi, unsigned long _lo, unsigned char _dec)
		: _guard(_g),hi(_hi),lo(_lo),decimalPlace(_dec) {}
	inline unsigned long getLo() const                           { return lo; }
	inline unsigned long getHi() const                           { return hi;}
	inline unsigned long getGuard() const                        { return _guard;}
	inline void setLo(const unsigned long Lo)                    { lo=Lo;}
	inline void setHi(const unsigned long Hi)                    { hi=Hi;}
	inline void setGuard(const guard_t Guard)                    { _guard=Guard;}
	inline void setDecimalPlace(const unsigned char place)       { decimalPlace=place;}
private:
	void addlo ( unsigned long _lo);
	void addhi ( unsigned long _hi);
	guard_t _guard;
	unsigned long hi,lo;
	unsigned char decimalPlace;
};

class ufixed_t {
	friend class accumulator;
public:
	virtual unsigned char num_int_bits() const = 0;
	virtual const std::string format(bool decimal=true) const;
protected:
	unsigned long value;
	void assign(const accumulator & rhs);
};

class fixed_t {
public:
	virtual unsigned char num_int_bits() const = 0;
	virtual const std::string format(bool decimal=true) const;
protected:
	long value;
	friend class accumulator;
};

/* The ufixed class represents coyote unsigned fractional quantities which have
 * a compile-time-specified number of integer bits in a total 32-bit value.
 */
template<unsigned char integerBits=1> class ufixed : public ufixed_t
{
public:
	typedef ufixed<integerBits> _ufixed;
	ufixed() { value = 0xcdcdcdcd;}
	ufixed(double _initialValue)                       { value = doubleToUlong(_initialValue,integerBits); }
	inline _ufixed& operator = (const _ufixed& rhs)    { value = rhs.value; return *this; }
	_ufixed& operator = (const accumulator& rhs)       { assign(rhs); return *this; }
	inline bool operator==( const std::string& rhs) const { return format(false) == rhs; }
	unsigned char num_int_bits() const                 { return integerBits; }
protected:
	ufixed(unsigned long v) { value = v; }
};
/*
   fixed<unsigned char> is a signed, fixed-point class with <unsigned char>
   integer bits and 32-<unsigned char> fractional bits. The msbit of the
   integer bits is used as the sign bit, so a fixed<1> variable is actually
   a signed, fractional quantity ranging from -1 to 0.99....
*/
template<unsigned char integerBits=1> class fixed : public fixed_t
{
public:
	typedef fixed<integerBits> _fixed;
	fixed() { value = 0x7dcdcdcd; }
	fixed(double _initialValue)                      { value = doubleToLong(_initialValue,integerBits); }
	inline _fixed& operator = (const _fixed& rhs)    { value = rhs.value; return *this; }
	inline _fixed& operator = (const SRS& rhs)       { value = rhs.toLong(integerBits); return *this; }
	inline bool operator==( const std::string& rhs) const { return format(false) == rhs; }
	inline const _fixed operator -() const           { return _fixed(-value); }
	unsigned char num_int_bits() const               { return integerBits; }
	long toBinary() const                            { return value; }
	inline _fixed& fromBinary(long v)                { value = v; return *this; }
	fixed(long v)                                    { value = v; }
	fixed(int v)                                    { value = v; }
//	operator long() const															{return  value; }

	friend class lfixed;
};

class fixed_integer : public fixed<32>
{
	friend class accumulator;
public:
	fixed_integer(long _i) { value = _i; }
};

class SRS
{
public:
	SRS(const accumulator& a, shifting_t s, rounding_t r);
	long toLong(unsigned char numDecimals) const;
	static bool isSat()                  { return _satFlag; }
	static void clearSat()               { _satFlag = false; }
	unsigned char num_int_bits() const   {return _a.num_int_bits(); }
private:
	SRS() {}
	accumulator _a;
	static bool _satFlag;

	friend class accumulator;
	friend class lfixed;
};

class lfixed
{
	friend class accumulator;
	friend class SRS;
public:
	lfixed() : lo(0xcdcdcdcd),hi(0xeeeeeeee) {}
	lfixed(const lfixed& other)                         { operator =(other); }
	lfixed& operator = (const lfixed& rhs)              { lo=rhs.lo; hi=rhs.hi; return *this; }
	lfixed& operator = (const fixed<1>& rhs)            { lo=0x0; hi=rhs.value; return *this; }
	lfixed& operator = (const SRS& rhs);
	lfixed& zero()                                          { lo=hi=0; return *this;}
	lfixed(unsigned long _hi, unsigned long _lo, unsigned char _dec)
		: hi(_hi),lo(_lo) {}
	inline unsigned long getLo() const                           { return lo; }
	inline unsigned long getHi() const                           { return hi;}
	inline void setLo(const unsigned long Lo)                    { lo=Lo;}
	inline void setHi(const unsigned long Hi)                    { hi=Hi;}
private:
	void addlo ( unsigned long _lo);
	void addhi ( unsigned long _hi);
	long hi,lo;
};


enum LogExp_function {nop, log, exp, sm} ;
enum LogExp_operator {norm32, norm64, _X_, _X_Y_, _X_SHR_1_, _Y_, _Y_X_, _Y_SHR_1_} ;

void LogExpIn(LogExp_function left_fun, LogExp_operator left_op, void *x, LogExp_function right_fun, LogExp_operator right_op, void *y);
void LogExpOut(void* x, void* y);

}; // namespace end

/*==========================================================================
 * some arithmetic operators
 *=========================================================================*/
inline const coyote_math::accumulator operator * (const coyote_math::ufixed_t& a,
										   const coyote_math::ufixed_t& b)
{ return coyote_math::accumulator::multiply(a,b); }
inline const coyote_math::accumulator operator * (const coyote_math::ufixed_t& a,
										   const coyote_math::fixed_t& b)
{ return coyote_math::accumulator::multiply(a,b); }
inline const coyote_math::accumulator operator * (const coyote_math::fixed_t& a,
										   const coyote_math::fixed_t& b)
{ return coyote_math::accumulator::multiply(a,b); }

inline const coyote_math::accumulator operator * (const coyote_math::ufixed_t& a,
										   const coyote_math::fixed_integer& b)
{ return coyote_math::accumulator::multiply(a,static_cast<const coyote_math::fixed_t&>(b)); }

inline const coyote_math::accumulator operator * (const coyote_math::fixed_t& a,
										   const coyote_math::ufixed_t& b)
{ return coyote_math::accumulator::multiply(b,a); }

inline const coyote_math::accumulator operator * (const coyote_math::fixed_integer& a,
										   const coyote_math::ufixed_t& b)
{ return coyote_math::accumulator::multiply(b, static_cast<const coyote_math::fixed_t&>(a)); }

inline const coyote_math::accumulator operator & (const coyote_math::accumulator& lhs,
												  const coyote_math::accumulator& rhs)
{ return coyote_math::accumulator(lhs)&=rhs; }

inline const coyote_math::accumulator operator | (const coyote_math::accumulator& lhs,
												  const coyote_math::accumulator& rhs)
{ return coyote_math::accumulator(lhs)|=rhs; }

inline const coyote_math::accumulator operator ^ (const coyote_math::accumulator& lhs,
												  const coyote_math::accumulator& rhs)
{ return coyote_math::accumulator(lhs)^=rhs; }

inline const coyote_math::accumulator operator + (const coyote_math::accumulator& lhs,
												  const coyote_math::accumulator& rhs)
{ return coyote_math::accumulator(lhs)+=rhs; }

inline const coyote_math::accumulator operator - (const coyote_math::accumulator& lhs,
												  const coyote_math::accumulator& rhs)
{ return coyote_math::accumulator(lhs)-=rhs; }

