/////////////////////////////////////////////////////////////////////////////
// AutoTypes.h: Wrappers for COM Automation standard types
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  C++ Wrappers for COM Automation standard types
//

#pragma once

#include <oaidl.h>
#include <wtypes.h>
#include <utility>
#include <ctime>
#include <cmath>
#include <stdexcept>

#include <foundation\string\StringEx.h>
#include <comutil.h>


namespace stingray {
namespace foundation {



/////////////////////////////////////////////
// macros for dealing with variant types


#define NOTREF_TYPE(var) \
	((var).vt & ~VT_BYREF)

#define NOTREF_VAL2(var, memb1, memb2) \
	((var).vt & VT_BYREF? *(var).p##memb2 : (var).memb1)

#define NOTREF_VAL(var, member) \
	(NOTREF_VAL2((var), member, member))

#define NUMERIC_DATA_S(var) \
	((NOTREF_TYPE(var) == VT_I4)? (NOTREF_VAL(var, lVal)) : \
	((NOTREF_TYPE(var) == VT_I2)? (NOTREF_VAL(var, iVal)) : \
	((NOTREF_TYPE(var) == VT_UI1)? (NOTREF_VAL(var, bVal)) : \
	((NOTREF_TYPE(var) == VT_BOOL)? (NOTREF_VAL(var, boolVal)) : \
	((0))))))

#define NUMERIC_DATA(var) \
	((NOTREF_TYPE(var) != VT_VARIANT)? NUMERIC_DATA_S(var) : \
	(NUMERIC_DATA_S(*(var).pvarVal)))

#define NUMERIC_VAL(var) NUMERIC_DATA(var)

#define IS_NUMERIC_S(var) \
	((NOTREF_TYPE(var) == VT_I4) || \
	 (NOTREF_TYPE(var) == VT_I2) || \
	 (NOTREF_TYPE(var) == VT_UI1))

#define IS_NUMERIC(var) \
	((NOTREF_TYPE(var) != VT_VARIANT)? IS_NUMERIC_S(var) : \
	(IS_NUMERIC_S(*(var).pvarVal)))


#define IS_MISSING(var) \
	((var.vt == VT_ERROR) || (var.vt == VT_EMPTY))

#define BSTR_VAL_S(var) \
	(NOTREF_VAL((var), bstrVal))

#define BSTR_VAL(var) \
	((NOTREF_TYPE(var) == VT_BSTR)? \
		(BSTR_VAL_S(var)) : \
		((NOTREF_TYPE(var) == VT_VARIANT)? \
			(BSTR_VAL_S(*(var).pvarVal)) : \
			(NULL)))



/////////////////////////////////
// forward definitions


class _safearray_t;
class _currency_t;
class _datetime_t;
class _timespan_t;


/////////////////////////////////
// class _currency_t

class _currency_t
{
private:
	CURRENCY _cur;

// Constructors
public:
	_currency_t() {_cur.Hi = 0; _cur.Lo = 0;}
	_currency_t(const _currency_t& cySrc) {_cur.Hi = cySrc._cur.Hi; _cur.Lo = cySrc._cur.Lo;}
	_currency_t(const CURRENCY& cySrc) {_cur.Hi = cySrc.Hi; _cur.Lo = cySrc.Lo;}
	_currency_t(const VARIANT& varSrc) {operator =(varSrc);}
	_currency_t(long intpart, long fractpart) {set(intpart, fractpart);}

	const _currency_t& operator=(const _currency_t& cySrc) {_cur.Hi = cySrc._cur.Hi; _cur.Lo = cySrc._cur.Lo; return *this;}
	const _currency_t& operator=(const CURRENCY& cySrc) {_cur.Hi = cySrc.Hi; _cur.Lo = cySrc.Lo; return *this;}
	const _currency_t& operator=(const VARIANT& varSrc);

// Operations
public:
	operator CURRENCY() const {return _cur;}
	operator CURRENCY*() {return &_cur;}

	// Currency logic
	bool operator==(const _currency_t& cur) const 	{return (_cur.Hi == cur._cur.Hi && _cur.Lo == cur._cur.Lo);}
	bool operator<(const _currency_t& cur) const {return((_cur.Hi == cur._cur.Hi)? (_cur.Lo < cur._cur.Lo) : (_cur.Hi < cur._cur.Hi));}

	// Currency math
	_currency_t operator+(const _currency_t& cur) const;
	_currency_t operator-(const _currency_t& cur) const;
	_currency_t operator-() const;
	const _currency_t& operator+=(const _currency_t& cur) {*this = *this + cur; return *this;}
	const _currency_t& operator-=(const _currency_t& cur) {*this = *this - cur; return *this;}

	_currency_t operator*(long nOp) const;
	_currency_t operator/(long nOp) const;
	const _currency_t& operator*=(long nOp) {*this = *this * nOp; return *this;}
	const _currency_t& operator/=(long nOp) {*this = *this / nOp; return *this;}

	bool parse(const char* lpszCurrency, LCID = LANG_USER_DEFAULT, unsigned long dwFlags = 0);
	string format(LCID lcid = LANG_USER_DEFAULT, unsigned long dwFlags = 0) const;
	void set(long intpart, long fractpart);
};




/////////////////////////////////
// class _timespan_t


class _timespan_t
{
private:
	double _span;

public:
	_timespan_t(): _span(0) {}
	_timespan_t(const _timespan_t& tsSrc): _span(tsSrc._span) {}
	_timespan_t(const double dblSrc): _span(dblSrc) {}
	_timespan_t(long lDays, int nHours, int nMins, int nSecs): _span(0) {set(lDays, nHours, nMins, nSecs);}

	const _timespan_t& operator=(const _timespan_t& tsSrc)
	{_span = tsSrc._span; return *this;}
	const _timespan_t& operator=(const double dblSrc)
	{_span = dblSrc; return *this;}

public:
	operator double() const {return _span;}

	double total_days() const {return _span;}
	double total_hours() const {return _span * 24;}
	double total_minutes() const {return _span * 24 * 60;}
	double total_seconds() const {return _span * 24 * 60 * 60;}

	long days() const
	{return (long)_span;}
	int hours() const
	{double dblTemp; modf(_span, &dblTemp); return (long)(dblTemp*24);}
	int minutes() const
	{double dblTemp; modf(_span*24, &dblTemp); return (long)(dblTemp*60);}
	int seconds() const
	{double dblTemp; modf(_span*24*60, &dblTemp); return (long)(dblTemp*60);}

public:
	// _timespan_t logic
	bool operator==(const _timespan_t& tsp) const
	{return _span == tsp._span;}
	bool operator<(const _timespan_t& tsp) const
	{return _span < tsp._span;}

	// _timespan_t math
	_timespan_t operator+(const _timespan_t& tsp) const
	{return _timespan_t(_span + tsp._span);}
	_timespan_t operator-(const _timespan_t& tsp) const
	{return _timespan_t(_span - tsp._span);}
	const _timespan_t& operator+=(const _timespan_t tsp)
	{_span += tsp._span; return *this;}
	const _timespan_t& operator-=(const _timespan_t tsp)
	{_span -= tsp._span; return *this;}
	_timespan_t operator-() const
	{return _timespan_t(-_span);}

	void set(long lDays, int nHours, int nMins, int nSecs)
	{_span = lDays + ((double)nHours)/24 + ((double)nMins)/(24*60) + ((double)nSecs)/(24*60*60);}

	friend class _datetime_t;
};



/////////////////////////////////
// class _datetime_t


class _datetime_t
{
private:
	DATE _date;

public:
	_datetime_t() {_date = now()._date;}
	_datetime_t(const _datetime_t& dtSrc): _date(dtSrc._date) {}
	_datetime_t(const DATE dtSrc): _date(dtSrc) {}
	_datetime_t(time_t t) {operator=(t);}
	_datetime_t(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec) {set(nYear, nMonth, nDay, nHour, nMin, nSec);}
	_datetime_t(const char* lpszDate, LCID lcid = LANG_USER_DEFAULT, unsigned long dwFlags = 0) {parse(lpszDate, lcid, dwFlags);}

	const _datetime_t& operator=(const _datetime_t& dtSrc)
	{_date = dtSrc._date; return *this;}
	const _datetime_t& operator=(const DATE& dtSrc)
	{_date = dtSrc; return *this;}
	const _datetime_t& operator=(time_t t)
	{tm* ptm = localtime(&t); if (!ptm) throw std::runtime_error("Invalid _datetime_t"); _date = DATEfromtm(*ptm); return *this;}
	const _datetime_t& operator=(const char* lpszDate)
	{parse(lpszDate); return *this;}

// Operations
public:
	operator DATE() const {return _date;}
	operator DATE*() {return &_date;}

	int year() const;
	int month() const;
	int day() const;
	int hour() const;
	int minute() const;
	int second() const;
	int day_of_week() const;
	int day_of_year() const;

public:
	bool operator==(const _datetime_t& date) const
	{return _date == date._date;}
	bool operator<(const _datetime_t& date) const
	{return doublefromDATE(_date) < doublefromDATE(date._date);}

	// DateTime math
	_datetime_t operator+(const _timespan_t& tsp) const
	{return _datetime_t(DATEfromdouble(doublefromDATE(_date)+(double)tsp));}
	_datetime_t operator-(const _timespan_t& tsp) const
	{return _datetime_t(DATEfromdouble(doublefromDATE(_date)-(double)tsp));}
	_timespan_t operator-(const _datetime_t& date) const
	{return _timespan_t(doublefromDATE(_date) - doublefromDATE(date._date));}
	const _datetime_t& operator+=(const _timespan_t tsp)
	{_date = (operator+(tsp))._date; return *this;}
	const _datetime_t& operator-=(const _timespan_t tsp)
	{_date = (operator-(tsp))._date; return *this;}

	void set(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
	void set_date(int nYear, int nMonth, int nDay)
	{set(nYear, nMonth, nDay, 0, 0, 0);}
	void set_time(int nHour, int nMin, int nSec)
	{set(1899, 12, 30, nHour, nMin, nSec);}

	void parse(const char* lpszDate, LCID lcid = LANG_USER_DEFAULT, unsigned long dwFlags = 0);
	string format(LCID lcid = LANG_USER_DEFAULT, unsigned long dwFlags = 0) const;

public:
	static _datetime_t now()
	{return _datetime_t(::time(NULL));}

private:
	static void tmfromDATE(DATE dtSrc, struct tm& tmDest);
	static DATE DATEfromtm(tm& tmTm);
	static DATE DATEfromdouble(double dbl);
	static double doublefromDATE(DATE dt);
	friend class _timespan_t;
};




/////////////////////////////////
// class _safearray_t


class _safearray_t
{
private:
	SAFEARRAY* _psa;
	bool _bOwn;

public:
	_safearray_t(): _psa(NULL), _bOwn(true) {}
	_safearray_t(SAFEARRAY* x, bool bOwn = true): _psa(NULL), _bOwn(bOwn) {if (bOwn) copy(x); else attach(x);}
	_safearray_t(VARTYPE vt, long nelems, unsigned int dims = 1): _psa(NULL), _bOwn(true) {init(vt, nelems, dims);}
	_safearray_t(const _safearray_t& x, bool bOwn = true): _psa(NULL), _bOwn(true) {if (bOwn) copy(x); else attach(x);}
	_safearray_t(VARTYPE vt, unsigned int cDims, SAFEARRAYBOUND* rgsabound): _psa(NULL), _bOwn(true) {init(vt, cDims, rgsabound);}
	~_safearray_t() {if (_bOwn) free();}

	const _safearray_t& operator=(const _safearray_t& x)
	{copy(x); return *this;}
	const _safearray_t& operator=(SAFEARRAY* x)
	{copy(x); return *this;}

	const _safearray_t& attach(const _safearray_t& x, bool bOwn = false)
	{return attach(x._psa, bOwn);}
	const _safearray_t& attach(SAFEARRAY* x, bool bOwn = false)
	{free(); _psa = x; _bOwn = bOwn; return *this;}
	SAFEARRAY* detach()
	{SAFEARRAY* psaret = _psa; _psa = 0; _bOwn = true; return psaret;}

public:
	operator SAFEARRAY*() const	{return _psa;}

	// unlock() must be called after the raw data has been used
	void* access_data()
	{void* p = 0; HRESULT hr = ::SafeArrayAccessData(_psa, &p); if (FAILED(hr)) _com_issue_error(hr); return p;}
	void lock()
	{HRESULT hr; hr = ::SafeArrayLock(_psa); if (FAILED(hr)) _com_issue_error(hr);}
	void unlock()
	{HRESULT hr; hr = ::SafeArrayUnlock(_psa); if (FAILED(hr)) _com_issue_error(hr);}

	unsigned int dims() const
	{return SafeArrayGetDim(_psa);}
	long lbound(unsigned int dim) const
	{long l; HRESULT hr; hr = ::SafeArrayGetLBound(_psa, dim, &l); if (FAILED(hr)) _com_issue_error(hr); return l;}
	long ubound(unsigned int dim) const
	{long l; HRESULT hr; hr = ::SafeArrayGetUBound(_psa, dim, &l); if (FAILED(hr)) _com_issue_error(hr); return l;}

	void* get_raw_data()
	{return _psa->pvData;}
	void* get_element(long index, void* elem);
	void* get_element(long* indexes, void* elem);
	void put_element(long index, void* elem);
	void put_element(long* indexes, void* elem);
	unsigned int element_size() const
	{return ::SafeArrayGetElemsize(_psa);}

	void redim(long nelems, long lbnd = -1);
	void init(VARTYPE vt, long nelems, unsigned int dims = 1);
	void init(VARTYPE vt, unsigned int cDims, SAFEARRAYBOUND* rgsabound)
	{if (_psa && _bOwn) free(); _psa = ::SafeArrayCreate(vt, cDims, rgsabound);}

protected:
	void copy(const _safearray_t& x)
	{copy(x._psa);}
	void copy(SAFEARRAY* x)
	{HRESULT hr; free(); hr = ::SafeArrayCopy(x, &_psa); if (FAILED(hr)) _com_issue_error(hr); _bOwn = true;}
	void free()
	{if (_psa && _bOwn) {HRESULT hr; hr = ::SafeArrayDestroy(_psa); if (FAILED(hr)) _com_issue_error(hr); _psa = 0;}}
};


inline
void* _safearray_t::get_element (
	long index, 
	void* elem
)
{
	if (dims() > 1) throw std::range_error("Not enough dimensions");
	return get_element(&index, elem);
}


inline
void* _safearray_t::get_element (
	long* indexes, 
	void* elem
)
{
	HRESULT hr = ::SafeArrayGetElement(_psa, indexes, elem);
	if (FAILED(hr)) _com_issue_error(hr);
	return elem;
}


inline
void _safearray_t::put_element (
	long index, 
	void* elem
)
{
	if (dims() > 1) throw std::range_error("Not enough dimensions");
	put_element(&index, elem);
}


inline
void _safearray_t::put_element (
	long* indexes, 
	void* elem
)
{
	HRESULT hr = ::SafeArrayPutElement(_psa, indexes, elem);
	if (FAILED(hr)) _com_issue_error(hr);
}


inline
void _safearray_t::init (
	VARTYPE vt, 
	long nelems, 
	unsigned int dims
)
{
	SAFEARRAYBOUND* sabs = new SAFEARRAYBOUND[dims];
	try {
		for (unsigned int i = 0; i < dims; i++) {
			sabs[i].lLbound = 0; 
			sabs[i].cElements = nelems; 
		}
		init(vt, dims, sabs);
	}
	catch(...) {
		delete[] sabs;
		throw;
	}
}


inline
void _safearray_t::redim (
	long nelems, 
	long lbnd
)
{
	if (lbnd == -1) lbnd = lbound(0);
	SAFEARRAYBOUND sab = {nelems, lbnd};
	HRESULT hr = ::SafeArrayRedim(_psa, &sab);
	if (FAILED(hr)) _com_issue_error(hr);
}




};	// namespace stingray::foundation
};	// namespace stingray