/*************************************************************************/
/*    fixed_point_math.cpp : a library of C/C++ callable routines that   */
/*                           emulates coyote core fixed point arithmetic */
/*                                                                       */
/*     MAJOR_NUMBER 5     UPDATE_NUMBER 4     BUILD_NUMBER 0             */
/*************************************************************************/

#include "fixed_point_math.h"
//#include "stdafx.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <assert.h>

using namespace std;

bool coyote_math::SRS::_satFlag = false;

namespace coyote_math {

long log2_table[128]={
		0x00000000,
		0x00010000,
		0x00030000,
		0x00040000,
		0x00060000,
		0x00070000,
		0x00080000,
		0x000a0000,
		0x000b0000,
		0x000d0000,
		0x000e0000,
		0x000f0000,
		0x00110000,
		0x00120000,
		0x00130000,
		0x00140000,
		0x00160000,
		0x00170000,
		0x00180000,
		0x001a0000,
		0x001b0000,
		0x001c0000,
		0x001d0000,
		0x001f0000,
		0x00200000,
		0x00210000,
		0x00220000,
		0x00230000,
		0x00250000,
		0x00260000,
		0x00270000,
		0x00280000,
		0x00290000,
		0x002a0000,
		0x002c0000,
		0x002d0000,
		0x002e0000,
		0x002f0000,
		0x00300000,
		0x00310000,
		0x00320000,
		0x00330000,
		0x00340000,
		0x00350000,
		0x00370000,
		0x00380000,
		0x00390000,
		0x003a0000,
		0x003b0000,
		0x003c0000,
		0x003d0000,
		0x003e0000,
		0x003f0000,
		0x00400000,
		0x00410000,
		0x00420000,
		0x00430000,
		0x00440000,
		0x00450000,
		0x00460000,
		0x00470000,
		0x00480000,
		0x00490000,
		0x004a0000,
		0x004b0000,
		0x004c0000,
		0x004d0000,
		0x004e0000,
		0x004f0000,
		0x00500000,
		0x00510000,
		0x00510000,
		0x00520000,
		0x00530000,
		0x00540000,
		0x00550000,
		0x00560000,
		0x00570000,
		0x00580000,
		0x00590000,
		0x005a0000,
		0x005b0000,
		0x005b0000,
		0x005c0000,
		0x005d0000,
		0x005e0000,
		0x005f0000,
		0x00600000,
		0x00610000,
		0x00610000,
		0x00620000,
		0x00630000,
		0x00640000,
		0x00650000,
		0x00660000,
		0x00670000,
		0x00670000,
		0x00680000,
		0x00690000,
		0x006a0000,
		0x006b0000,
		0x006b0000,
		0x006c0000,
		0x006d0000,
		0x006e0000,
		0x006f0000,
		0x006f0000,
		0x00700000,
		0x00710000,
		0x00720000,
		0x00730000,
		0x00730000,
		0x00740000,
		0x00750000,
		0x00760000,
		0x00760000,
		0x00770000,
		0x00780000,
		0x00790000,
		0x00790000,
		0x007a0000,
		0x007b0000,
		0x007c0000,
		0x007c0000,
		0x007d0000,
		0x007e0000,
		0x007f0000,
		0x007f0000
	};
long exp2_table[128]={
		0x40000000 ,
		0x40800000 ,
		0x40800000 ,
		0x41000000 ,
		0x41800000 ,
		0x42000000 ,
		0x42000000 ,
		0x42800000 ,
		0x43000000 ,
		0x43000000 ,
		0x43800000 ,
		0x44000000 ,
		0x44800000 ,
		0x44800000 ,
		0x45000000 ,
		0x45800000 ,
		0x46000000 ,
		0x46000000 ,
		0x46800000 ,
		0x47000000 ,
		0x47800000 ,
		0x47800000 ,
		0x48000000 ,
		0x48800000 ,
		0x49000000 ,
		0x49800000 ,
		0x49800000 ,
		0x4a000000 ,
		0x4a800000 ,
		0x4b000000 ,
		0x4b800000 ,
		0x4b800000 ,
		0x4c000000 ,
		0x4c800000 ,
		0x4d000000 ,
		0x4d800000 ,
		0x4e000000 ,
		0x4e000000 ,
		0x4e800000 ,
		0x4f000000 ,
		0x4f800000 ,
		0x50000000 ,
		0x50800000 ,
		0x51000000 ,
		0x51000000 ,
		0x51800000 ,
		0x52000000 ,
		0x52800000 ,
		0x53000000 ,
		0x53800000 ,
		0x54000000 ,
		0x54800000 ,
		0x55000000 ,
		0x55800000 ,
		0x55800000 ,
		0x56000000 ,
		0x56800000 ,
		0x57000000 ,
		0x57800000 ,
		0x58000000 ,
		0x58800000 ,
		0x59000000 ,
		0x59800000 ,
		0x5a000000 ,
		0x5a800000 ,
		0x5b000000 ,
		0x5b800000 ,
		0x5c000000 ,
		0x5c800000 ,
		0x5d000000 ,
		0x5d800000 ,
		0x5e000000 ,
		0x5e800000 ,
		0x5f000000 ,
		0x5f800000 ,
		0x60000000 ,
		0x60800000 ,
		0x61000000 ,
		0x61800000 ,
		0x62000000 ,
		0x62800000 ,
		0x63000000 ,
		0x64000000 ,
		0x64800000 ,
		0x65000000 ,
		0x65800000 ,
		0x66000000 ,
		0x66800000 ,
		0x67000000 ,
		0x67800000 ,
		0x68000000 ,
		0x69000000 ,
		0x69800000 ,
		0x6a000000 ,
		0x6a800000 ,
		0x6b000000 ,
		0x6b800000 ,
		0x6c000000 ,
		0x6d000000 ,
		0x6d800000 ,
		0x6e000000 ,
		0x6e800000 ,
		0x6f000000 ,
		0x70000000 ,
		0x70800000 ,
		0x71000000 ,
		0x71800000 ,
		0x72000000 ,
		0x73000000 ,
		0x73800000 ,
		0x74000000 ,
		0x74800000 ,
		0x75800000 ,
		0x76000000 ,
		0x76800000 ,
		0x77800000 ,
		0x78000000 ,
		0x78800000 ,
		0x79800000 ,
		0x7a000000 ,
		0x7a800000 ,
		0x7b000000 ,
		0x7c000000 ,
		0x7c800000 ,
		0x7d800000 ,
		0x7e000000 ,
		0x7e800000 ,
		0x7f800000 ,
};

typedef union {
	struct {
		unsigned long lsb;
		long msb;
	} int32;
	__int64 int64;
} longlong;

unsigned long doubleToUlong(double v, unsigned char integerBits)
{
	long limit = 1<<(integerBits);
	if ( (v<0.0) || ( v>=double(limit) ) )
		throw range_exception_t("initial value out of range for ufixed type");

	double c,roundValue;
	roundValue=0.5;
	c = (double)(0x80000000>>(integerBits-1));
	return (unsigned long) ((v * c)+roundValue);
}

long doubleToLong(double v, unsigned char integerBits)
{
 double c,roundValue;
 long limit = 1<<(integerBits-1);

 if (v<double(-limit))
		return (long) (0x80000000);
 else if( v>double(limit)) 
		return (long) (0x7fffffff);

 if (v>=0) roundValue=0.5; else roundValue=-0.5;
 c = (double)(0x80000000>>(integerBits-1));
 double tmp = (v * c)+roundValue;
 if (tmp >= 2147483648)
  return (long) (0x7fffffff);
 else
  return (long) tmp;
}


long long doubleToLongLong(double v, unsigned char integerBits)
{
 double c,roundValue;
 long limit = 1<<(integerBits-1);

  if (v<double(-limit))
		 return (long long) (0x8000000000000000);
  else if( v>double(limit)) 
		 return (long long) (0x7fffffffffffffff);

 if (v>=0) roundValue=0.5; else roundValue=-0.5;
 c = (double)(0x8000000000000000>>(integerBits-1));
 double tmp = (v * c)+roundValue;

 if (tmp >= 9223372036854775808)
  return (long long) (0x7fffffffffffffff);
 else
  return (long long) tmp;
}

void ufixed_t::assign(const coyote_math::accumulator& rhs)
{
	unsigned char integerBits = num_int_bits();
	if (integerBits!=rhs.num_int_bits())
		throw alignment_exception_t(integerBits,rhs.num_int_bits(),"=");
	value = rhs.getHi();
}

const std::string
ufixed_t::format(bool decimal) const
{
	std::ostringstream os;
	short integerBits = num_int_bits();
	if (decimal) {
		os << (value>>(32-integerBits)) << '.';
		int decimals = 0;
		longlong x; x.int64 = (unsigned)(value<<integerBits);
		while (x.int32.lsb!=0) {
			x.int64*=10;
//			os << (unsigned) x.int32.msb;
			x.int32.msb = 0;
			++decimals;
		}
		if (decimals==0) os << '0';
	} else {
		os << "0x" << hex << setfill('0') << setw(8) << value
			<< ":" << dec << setw(1) << integerBits;
	}
	return std::string(os.str());
}

const std::string
fixed_t::format(bool decimal) const
{
	std::ostringstream os;
	short integerBits = num_int_bits();
	if (decimal) {
		if (value&0x80000000){
			os << '-';

		}
		os << (abs(value)>>(32-integerBits)) << '.';
		int decimals = 0;
		longlong x; x.int64 = (unsigned)(abs(value)<<integerBits);
		while (x.int32.lsb!=0) {
			x.int64*=10;
//			if(x.int32.msb < 0)
//				x.int32.msb = -x.int32.msb + x.int32.lsb;
			os << x.int32.msb;
			x.int32.msb = 0;
			++decimals;
		}
		if (decimals==0) os << '0';
	} else {
		os << "0x" << hex << setfill('0') << setw(8) << value
			<< ":" << dec << setw(1) << integerBits;
	}
	return std::string(os.str());
}

coyote_math::SRS::SRS(const accumulator& a, shifting_t s, rounding_t r)
{
	_a = a;
	switch (s) {
		case sLEFT1  : _a<<=1; break;
		case sRIGHT1 : _a>>=1; break;
		case sRIGHT2 : _a>>=2; break;
	}
	switch (r) {
		case rHALF   : _a.addlo(0x80000000); break;
		case r2ZERO  : if (a._guard>=0) _a.lo=0; else _a.addhi(1); break;
		case rDITHER : throw exception_t("SRS rDITHER mode not yet implemented");
	}
}

long coyote_math::SRS::toLong(unsigned char numDecimals) const
{
	if (numDecimals!=_a.num_int_bits())
		return 0x0;//throw alignment_exception_t(numDecimals,_a.num_int_bits(),"=");
	// perform saturation
	if ( _a._guard>0 || (_a._guard==0 && _a.hi>0x7fffffff) ) {
		// saturated positive number
		_satFlag = true;
		return 0x7fffffff;
	} else if (_a._guard<-1 || (_a._guard==-1 && _a.hi<0x80000000) ) {
		// saturated negative number
		_satFlag = true;
		return 0x80000000;
	}
	return (long)_a.hi;
}

coyote_math::accumulator::accumulator(__int64 value, unsigned char numDecs, bool signExtend)
{
	decimalPlace = numDecs;
	lo = (unsigned long)(value&0x00000000ffffffff);
	hi = (unsigned long)(value>>32);
	if (signExtend && value<0) _guard=-1; else _guard=0;
}

coyote_math::accumulator& coyote_math::accumulator::operator = ( const ufixed_t& rhs)
{
	lo = 0;
	hi = rhs.value;
	_guard=0;
	decimalPlace=rhs.num_int_bits();
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator = ( const fixed_t& rhs)
{
	lo = 0;
	hi = (unsigned long)(rhs.value);
	if (rhs.value<0) _guard=0xff; else _guard=0;
	decimalPlace=rhs.num_int_bits();
	return *this;
}

// local masks used in shifting
static unsigned long msbits[9] = {0x00000000,
0x80000000,0xc0000000,0xe0000000,0xf0000000,
0xf8000000,0xfc000000,0xfe000000,0xff000000};
static unsigned long lsbits[9] = {0x00000000,
0x00000001,0x00000003,0x00000007,0x0000000f,
0x0000001f,0x0000003f,0x0000007f,0x000000ff};

coyote_math::accumulator& coyote_math::accumulator::operator>>=( unsigned short shift )
{
//	decimalPlace+=shift;
	// perform the shift a maximum of 8 bits at a time
	bool isNeg = _guard<0;
	for (;shift>8;shift-=8) {
		lo>>=8; lo|=(hi&lsbits[8])<<(32-8);
		hi>>=8; hi|=(unsigned long)(_guard)<<(32-8);
		if (isNeg) _guard=-1; else _guard=0;
	}
	// shift the last number of bits
	lo>>=shift; lo|=(hi&lsbits[shift])<<(32-shift);
	hi>>=shift; hi|=((unsigned long)(_guard&lsbits[shift]))<<(32-shift);
	_guard>>=shift;	if (isNeg) _guard|=(guard_t)(lsbits[shift])<<(8-shift);
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator<<=( unsigned short shift )
{
//	decimalPlace-=shift;  // NOTE: what if it goes negative?????
	// perform the shift a maximum of 8 bits at a time
	for (;shift>8;shift-=8) {
		_guard = (guard_t)(hi>>(32-8));
		hi<<=8; hi|=(lo>>(32-8));
		lo<<=8;
	}
	// shift the last number of bits
	_guard<<=shift; _guard|=(guard_t)(hi>>(32-shift));
	hi<<=shift; hi|=(lo>>(32-shift));
	lo<<=shift;
	return *this;
}

void coyote_math::accumulator::addlo ( unsigned long _lo)
{
	accumulator x(0,0,_lo,decimalPlace);
	operator+=(x);
}

void coyote_math::accumulator::addhi ( unsigned long _hi)
{
	accumulator x(0,_hi,0,decimalPlace);
	operator+=(x);
}

coyote_math::accumulator& coyote_math::accumulator::operator = (const coyote_math::SRS& rhs)					 
{ 
	lo=rhs._a.getLo(); 
	hi=rhs._a.getHi(); 
	_guard=rhs._a.getGuard(); 
	decimalPlace=rhs.num_int_bits(); 
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator = ( const coyote_math::lfixed& rhs)            
{ 
	lo=rhs.lo; 
	hi=rhs.hi; 
	_guard = 0x0;
	if (rhs.hi<0) _guard=0xff; else _guard=0;
	decimalPlace=1;
	return *this; 
}

coyote_math::accumulator& coyote_math::accumulator::operator+= ( const coyote_math::accumulator& rhs)
{
	if (decimalPlace != rhs.decimalPlace)
		throw alignment_exception_t(decimalPlace,rhs.decimalPlace,"+=");
	__int64 sum;
	__int64 clo;
	guard_t chi;
	sum = static_cast<__int64>(lo) + static_cast<__int64>(rhs.lo);
	lo = (unsigned long)(sum&0xffffffff);
	clo = sum>>32;
	sum = static_cast<__int64>(hi) + static_cast<__int64>(rhs.hi) + clo;
	hi = (unsigned long)(sum&0xffffffff);
	chi = (guard_t)(sum>>32);
	sum = _guard + rhs._guard + chi;
	_guard = (guard_t)(sum&0xff);
	if (sum>255)
	{
		hi = 0xffffffff;
		lo = 0xffffffff;
		_guard = 0x7f;
	}
	if (sum<-256)
	{
		hi = 0x00000000;
		lo = 0x00000000;
		_guard = 0x80;
	}
	
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator-= ( const coyote_math::accumulator& rhs)
{
	if (decimalPlace != rhs.decimalPlace)
		throw alignment_exception_t(decimalPlace,rhs.decimalPlace,"-=");
	operator+=(-rhs);
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator^= ( const coyote_math::accumulator& rhs)
{
	_guard ^= rhs._guard;
	hi ^= rhs.hi;
	lo ^= rhs.lo;
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator&= ( const coyote_math::accumulator& rhs)
{
	_guard &= rhs._guard;
	hi &= rhs.hi;
	lo &= rhs.lo;
	return *this;
}

coyote_math::accumulator& coyote_math::accumulator::operator|= ( const coyote_math::accumulator& rhs)
{
	_guard |= rhs._guard;
	hi |= rhs.hi;
	lo |= rhs.lo;
	return *this;
}

const coyote_math::accumulator  coyote_math::accumulator::operator~  () const
{
	return coyote_math::accumulator(~_guard,~hi,~lo,decimalPlace);
}

const coyote_math::accumulator  coyote_math::accumulator::operator-  () const
{
	accumulator result(~(*this));
	result.addlo(1);
	return result;
}

const coyote_math::accumulator coyote_math::accumulator::abs() const
{
	if (_guard>=0)
		return accumulator(*this);
	else
		return (-(*this));
}

bool coyote_math::accumulator::operator<  ( const coyote_math::accumulator& rhs) const
{
	if (decimalPlace != rhs.decimalPlace)
		throw alignment_exception_t(decimalPlace,rhs.decimalPlace,"<");
	return (_guard<rhs._guard) ||
		(( _guard==rhs._guard) && ( (hi<rhs.hi) ||
		                            (hi==rhs.hi) && (lo<rhs.lo)));
}

bool coyote_math::accumulator::operator>  ( const coyote_math::accumulator& rhs) const
{
	if (decimalPlace != rhs.decimalPlace)
		throw alignment_exception_t(decimalPlace,rhs.decimalPlace,">");
	return (_guard>rhs._guard) ||
		(( _guard==rhs._guard) && ( (hi>rhs.hi) ||
		                            (hi==rhs.hi) && (lo>rhs.lo)));
}

bool coyote_math::accumulator::operator== ( const coyote_math::accumulator& rhs) const
{
	if (decimalPlace != rhs.decimalPlace)
		throw alignment_exception_t(decimalPlace,rhs.decimalPlace,"==");
	return (_guard==rhs._guard) && (hi==rhs.hi) && (lo==rhs.lo);
}

const std::string coyote_math::accumulator::format(bool decimal) const
{
	std::ostringstream os;
	short integerBits = num_int_bits();
	if (decimal) {
		if (_guard<0) os << '-';
		assert(integerBits<=32);
		os << (static_cast<__int64>(hi>>(32-integerBits))| (static_cast<__int64>(_guard&0x7f)<<integerBits)) << '.';
		int decimals = 0;
		longlong x,v;
		x.int32.msb = v.int32.msb = 0;
		x.int32.lsb = (unsigned)(hi<<integerBits);
		v.int32.lsb = (unsigned)(lo<<integerBits);
		// x is msbits of value in lsbits field
		// v is lsbits of value in lsbits field
		while (x.int32.lsb!=0||v.int32.lsb!=0) {
			x.int64*=10; v.int64*=10;
			os << x.int32.msb;
			x.int32.lsb+=v.int32.msb;
			x.int32.msb = v.int32.msb = 0;
			++decimals;
		}
		if (decimals==0) os << '0';
	} else {
		os << "0x" << hex << setfill('0')
			<< setw(2) << ((unsigned short)_guard & 0xff)
			<< setw(8) << hi
			<< setw(8) << lo
			<< ":" << dec << integerBits;
	}
	return std::string(os.str());
}

const coyote_math::accumulator coyote_math::accumulator::multiply(const coyote_math::ufixed_t& lhs, const coyote_math::ufixed_t& rhs)
{
	__int64 product = static_cast<__int64>(lhs.value) * static_cast<__int64>(rhs.value);
	accumulator z(product,lhs.num_int_bits()+rhs.num_int_bits(),false);
	return z<<=1;
}

const coyote_math::accumulator coyote_math::accumulator::multiply(const coyote_math::ufixed_t& lhs, const coyote_math::fixed_t& rhs)
{
	__int64 product = (__int64(lhs.value)<<1) * rhs.value;
	return accumulator(product,
		lhs.num_int_bits()+rhs.num_int_bits()-1,
		(rhs.value<0)&&(lhs.value!=0));
}

const coyote_math::accumulator coyote_math::accumulator::multiply(const coyote_math::fixed_t& lhs, const coyote_math::fixed_t& rhs)
{
	__int64 product = (__int64(lhs.value)<<1) * static_cast<__int64>(rhs.value);
    return accumulator(product,
		lhs.num_int_bits()+rhs.num_int_bits()-1,
		((lhs.value&0x80000000)^(rhs.value&0x80000000))!=0);
}

const coyote_math::accumulator coyote_math::accumulator::multiplyNoShift(const coyote_math::fixed_t& lhs, const coyote_math::fixed_t& rhs)
{
	__int64 product = static_cast<__int64>(lhs.value) * static_cast<__int64>(rhs.value);
    return accumulator(product,
		lhs.num_int_bits()+rhs.num_int_bits(),
		((lhs.value&0x80000000)^(rhs.value&0x80000000))!=0);
}

const accumulator coyote_math::accumulator::multiply(const coyote_math::accumulator& lhs, unsigned char powerOfTwo)
{
	accumulator result(lhs);
	if (powerOfTwo>0) {
		unsigned char bitsToShift;
		for (bitsToShift=0; (powerOfTwo&1)==0;++bitsToShift,powerOfTwo>>=1) ;
		result <<= bitsToShift;
		result.decimalPlace=lhs.decimalPlace;
	}
	return result;
}


alignment_exception_t::	alignment_exception_t(unsigned char lhsDecs, unsigned char rhsDecs, const std::string& txt)
{
	ostringstream os;
	os << "lhs,rhs of " << txt << " have different numbers of integer digits ("
		<< static_cast<short>(lhsDecs) << " vs. " << static_cast<short>(rhsDecs) << ")";
	message = os.str();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class LogExp (JF 28.09.05)
/////////////////////////////////////////////////////////////////////////////////////////////////////

class LogExp
{
public:
	LogExp(){X=0;Y=0;}
	void LogExpIn(LogExp_function left_fun, LogExp_operator left_op, void *x, LogExp_function right_fun, LogExp_operator right_op, void *y);
	void LogExpOut(void* x, void* y);
private:
	long Norm32(long x);
	long Norm64(long x);
	long Log2(long x);
	long Exp(long x);
	long Sm(long x);
	long X,Y;
};

void LogExp::LogExpIn(LogExp_function left_fun, LogExp_operator left_op, void *x, LogExp_function right_fun, LogExp_operator right_op, void *y){
	long tempX, tempY;

	if (left_op > _X_SHR_1_)
		throw exception_t("Wrong left operator");
	if (right_op > norm64 && right_op < _Y_)
		throw exception_t("Wrong right operator");

	switch(left_op){
		case norm32:	tempX = Norm32(((fixed<1>*)(x))->toBinary());	break;
		case norm64:	tempX = Norm64(((fixed<1>*)(x))->toBinary());	break;
//		case _X_:		tempX = ((fixed<1>*)(x))->toBinary();			break;	//simulator
		case _X_:		tempX = ((fixed<1>*)(x))->toBinary() & 0xffff0000;	break;	//HW
		case _X_Y_:		tempX = X-Y;									break;
		case _X_SHR_1_:	tempX = X>>1;									break;
	}
	switch(right_op){
		case norm32:	tempY = Norm32(((fixed<1>*)(y))->toBinary());	break;
		case norm64:	tempY = Norm64(((fixed<1>*)(y))->toBinary());	break;
//		case _Y_:		tempY = ((fixed<1>*)(x))->toBinary();			break;  //simulator
		case _Y_:		tempY = ((fixed<1>*)(x))->toBinary() & 0xffff0000;	break;  //HW
//		case _Y_X_:		tempY = Y-X;									break;
		case _Y_X_:		tempY = X-Y;									break;
		case _Y_SHR_1_:	tempY = X>>1;									break;
	}
	X = tempX;
	Y = tempY;

	switch(left_fun){
		case log: X = Log2(X)	; break ;
		case exp: X = Exp(X)	; break ;
		case  sm: X = Sm(X)		; break ;
		case nop: X = X			; break ;
	}

	switch(right_fun){
		case log: Y = Log2(Y)	; break ;
		case exp: Y = Exp(Y)	; break ;
		case  sm: Y = Sm(Y)		; break ;
		case nop: Y = Y			; break ;
	}

	if (left_op == _X_ && right_op == _Y_)
		Y=X;
}

void LogExp::LogExpOut(void* x, void* y){
	((fixed<1>*)(x))->fromBinary(X);
	((fixed<1>*)(y))->fromBinary(Y);
}

long LogExp::Norm32(long x){
	long out=0;
	if (x<0){
		out |=0x80000000 ;
		x=~x;
	}
	long i = 0;
	if(x!=0)
		while(x>0){
			x<<=1;
			i++;
		}
	else i = 32 ;
	x&=0x7f000000;
	x>>=8;
	out|=((32 - i)<<23)+x;
	return out;
}
long LogExp::Norm64(long x){
	x=Norm32(x);
	if(x<0) x|=0x40000000;
	x|=0x10000000;
	return x;
}
long LogExp::Log2(long x){
	long out = (x&0x007f0000) >> 16 ;
	out = log2_table[out] ;
	out |= (x&0xff800000) ;
	return out;
}
long LogExp::Exp(long x){
	long out = (x&0x007f0000) >> 16 ;
	if((x&0x03f100000)>0x0ff80000)
		return 0x7fff0000 ;
	return exp2_table[out] ;
}
long LogExp::Sm(long x){
	int shift ; // = (31 - (x  >> 23)) ;
	long out = 0x80000000  ;
	bool sign_of_x = true;	//positive
	if(x < 0){
		out = 0x7fff0000 ;
		sign_of_x = false;	//negative
	}
	x &= 0x1fff0000 ;
	x >>= 23;
	shift = (31 - x) ;
	if(x < 16){
		if(sign_of_x)
			out = 0 ;
		else
			out = 0xffff0000 ;
		return out;
	}
	if(x > 31){
		if(sign_of_x)
			out = 0x80000000 ;
		else
			out = 0x7fff0000 ;
		return out;
	}
	out >>= shift ;
	out &= 0xffff0000;
	return out ;
}

LogExp logexp ;

void LogExpIn(LogExp_function left_fun, LogExp_operator left_op, void *x, LogExp_function right_fun, LogExp_operator right_op, void *y){
	logexp.LogExpIn(left_fun, left_op, x, right_fun, right_op, y);
}
void LogExpOut(void* x, void* y){
	logexp.LogExpOut(x,y);
}

/////////////////////////////////////////////////////////////////////////////////
// end of LogExp
/////////////////////////////////////////////////////////////////////////////////

coyote_math::lfixed& coyote_math::lfixed::operator = (const SRS& rhs)  					
{
	hi = rhs.toLong(1);

	if(hi == 0x7fffffff)
		lo = 0xffffffff;
	else
		if(hi == 0x80000000)
			lo = 0x00000000;
		else
			lo= 0x0; 

	return *this;
}
} // namespace coyote_math






//=========================== not used at this time ================================
#if 0
//	const accumulator  operator>> ( unsigned short shift ) const { return accumulator(*this)>>=shift; }
//	const accumulator  operator<< ( unsigned short shift ) const { return accumulator(*this)<<=shift; }

/*==========================================================================
 * The following template functions allow using the >> and << operators to
 * do "type" conversion for fixed point values, converting from a.b to x.y
 * representations, in a type-safe manner.
 *==========================================================================*/
namespace coyote_math {

/*
   vfixed is a signed, fixed-point class with a variable number (defaults to 1)
   of integer bits and 32-<numIntBits> fractional bits. The msbit of the
   integer bits is used as the sign bit
*/
class vfixed : public fixed_t
{
public:
	vfixed() : integerBits(1)                        { value = 0x7dcdcdcd; }
	vfixed(const fixed_t& other)                     { operator=(other); }
	vfixed(double _initialValue, unsigned char _intBits=1) : integerBits(_intBits) { value = doubleToLong(_initialValue,integerBits); }
	vfixed& operator = (const fixed_t& rhs)          { value = rhs.value; integerBits = rhs.num_int_bits(); return *this; }
	vfixed& operator = (const SRS& rhs);
	inline bool operator==( const std::string& rhs) const { return format(false) == rhs; }
	inline const vfixed operator -() const           { vfixed result(*this); result.value=-result.value; return result; }
	unsigned char num_int_bits() const               { return integerBits; }
	long toBinary() const                            { return value; }
	inline vfixed& fromBinary(long v)                { value = v; return *this; }
	vfixed(int v, unsigned char _intBits=32) : integerBits(_intBits) { value=v; if (value==0 && _intBits==32) integerBits=1; }
protected:
	unsigned char integerBits;
};

vfixed& vfixed::operator = (const SRS& rhs)
{
	integerBits=rhs.num_int_bits();
	value = rhs.toLong(integerBits);
	return *this;
}
template<unsigned char shiftAmount> class precision_shift{};
const precision_shift<1> by1;
const precision_shift<2> by2;
const precision_shift<3> by3;
const precision_shift<4> by4;
const precision_shift<5> by5;
const precision_shift<6> by6;
const precision_shift<7> by7;
const precision_shift<8> by8;
};
template<unsigned char integerBits,unsigned char shiftBits>
const coyote_math::ufixed<integerBits-shiftBits> operator << (const coyote_math::ufixed<integerBits>& f, coyote_math::precision_shift<shiftBits> s)
{
	return coyote_math::ufixed<integerBits-shiftBits>(f.value << shiftBits);
}
template<unsigned char integerBits,unsigned char shiftBits>
const coyote_math::ufixed<integerBits+shiftBits> operator >> (const coyote_math::ufixed<integerBits>& f, coyote_math::precision_shift<shiftBits> s)
{
	_ASSERTE(integerBits+shiftBits<=32);
	return coyote_math::ufixed<integerBits+shiftBits>(f.value >> shiftBits);
}
#endif
