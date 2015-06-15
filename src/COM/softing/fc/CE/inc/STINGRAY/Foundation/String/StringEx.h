/////////////////////////////////////////////////////////////////////////////
// StringEx.h
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Enhanced standard string support
//


#pragma once

#include <memory>
#include <cstring>
#include <string>
#include <iosfwd>
#include <cstdarg>
#include <cstdlib>
#include <crtdbg.h>

#pragma warning(disable:4786)

#if (_MFC_VER <= 0x0420)
#pragma warning (disable : 4097)
#endif // _MFC_VER <= 0x0420

namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////////
// Extended traits types
//
// Include support for additional functionality not included in the 
// standard string but available in MFC's strings
//
//

template <typename _CharType, typename _ConversionCharType>
struct char_traits_ex:
	public std::char_traits<_CharType>
{
	typedef std::char_traits<_CharType> base_traits;

	static int _format(_CharType* buffer, size_t count, const _CharType *format, va_list argptr )
	{return -1;}

	static size_t _convert(_CharType* dst, const _ConversionCharType* src, size_t count)
	{return -1}

	static size_t _conversion_length(const _ConversionCharType* )
	{return 0;}

	static _CharType* _empty_string()
	{return NULL;}
};


template <>
struct char_traits_ex<char, wchar_t>:
	public std::char_traits<char>
{
	typedef std::char_traits<char> base_traits;

	static int _format(_CharType* buffer, size_t count, const _CharType *format, va_list argptr )
	{return _vsnprintf(buffer, count, format, argptr);}

	static size_t _convert(_CharType* dst, const _ConversionCharType* src, size_t count)
	{return ::wcstombs(dst, src, count);}

	static size_t _conversion_length(const _ConversionCharType* _S)
	{return ::wcslen(_S);}

	static _CharType* _empty_string()
	{return "";}
};


template <>
struct char_traits_ex<wchar_t, char>:
	public std::char_traits<wchar_t>
{
	typedef std::char_traits<wchar_t> base_traits;

	static int _format(_CharType* buffer, size_t count, const _CharType *format, va_list argptr )
	{return _vsnwprintf(buffer, count, format, argptr);}

	static size_t _convert(_CharType* dst, const _ConversionCharType* src, size_t count)
	{return ::mbstowcs(dst, src, count);}

	static size_t _conversion_length(const _ConversionCharType* _S)
	{return ::strlen(_S);}

	static _CharType* _empty_string()
	{return L"";}
};




/////////////////////////////////////////////////////////////////////
// class basic_stringex
//
// Enhances the std::basic_string class with some useful functionality not 
// present there, like for example the conversion operator to LPCTSTR,
// the get/release buffer operations and a format function equivalent
// to printf.
//
//

template < typename _CharType, typename _ConversionCharType, typename _Traits = char_traits_ex<_CharType, _ConversionCharType>, typename _A = std::allocator<_Traits> >
class basic_string_ex:
	public std::basic_string<_CharType, _Traits, _A>
{
	typedef std::basic_string<_CharType, _Traits, _A>	_base_str;
	typedef basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> _this_str;
	typedef const_iterator _It;
public:

	explicit basic_string_ex(const allocator_type& _Al = allocator_type())
		: _base_str(_Al) {}
	basic_string_ex(const _base_str& _X)
		: _base_str(_X) {}
	basic_string_ex(const _CharType *_S, const allocator_type& _Al = allocator_type())
		: _base_str(_S, _Al) {}
	basic_string_ex(const _CharType *_S, size_type _N, const allocator_type& _Al = allocator_type())
		: _base_str(_S, _N, _Al) {}
	basic_string_ex(const _base_str& _X, size_type _P, size_type _M, const allocator_type& _Al = allocator_type())
		: _base_str(_X, _P, _M, _Al) {}
	basic_string_ex(size_type _N, _CharType _C, const allocator_type& _Al = allocator_type())
		: _base_str(_N, _C, _Al) {}
	basic_string_ex(_It _F, _It _L, const allocator_type& _Al = allocator_type())
		: _base_str(_F, _L, _Al) {}
	basic_string_ex(const _ConversionCharType *_S, const allocator_type& _Al = allocator_type())
		: _base_str(_Traits::_empty_string(), _Al) {assign(_S);}

	_this_str& operator=(const _this_str& _X)
	{_base_str::operator=(_X); return *this;}
	_this_str& operator=(const _CharType* _S)
	{_base_str::operator=(_S); return *this;}
	_this_str& operator=(_CharType _C)
	{_base_str::operator=(_C); return *this;}
	_this_str& operator= (const _ConversionCharType *_S)
	{return assign(_S);}

public:
	operator const _CharType*() const
	{return this->c_str();}

	_this_str& assign(const _ConversionCharType *_S);

	// Expandable buffer
	_CharType* get_buffer(unsigned int _N = 0)
	{if (_N > capacity()) resize(_N); return const_cast<_CharType*>(data());}
	_CharType* get_buffer_set_length(unsigned int _N = 0)
	{resize(_N); return const_cast<_CharType*>(data());}
	void release_buffer(unsigned int _N = 0)
	{if (_N == 0) _N = (UINT)_Traits::length(data()); resize(_N);}

	void format(const _CharType* lpFormat, ...);

private:
	void _format(const _CharType* lpFormat, va_list argList);
};

template <typename _CharType, typename _ConversionCharType, typename _Traits, typename _A>
inline 
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>::assign(const _ConversionCharType *_S)
{
	unsigned int nSrcLen = _S? _Traits::_conversion_length(_S) : 0;
	if (nSrcLen != 0) {
		_base_str::assign(nSrcLen, ' ');
		_Traits::_convert(const_cast<_CharType*>(c_str()), _S, nSrcLen+1);
	}
	return *this;
}


template <typename _CharType, typename _ConversionCharType, typename _Traits, typename _A>
inline 
void basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>::format(const _CharType* lpFormat, ...)
{
	va_list argList;
	va_start(argList, lpFormat);
	_format(lpFormat, argList);
	va_end(argList);
}


template <typename _CharType, typename _ConversionCharType, typename _Traits, typename _A>
void basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>::_format(const _CharType* lpFormat, va_list argList)
{
	// Empty this string
	resize(0);

	_ASSERTE(lpFormat);
	int nLength = _Traits::length(lpFormat);
	if(nLength == 0) {
		// We're done
		return;
	}

	// Default buffer size 256K, see below
	int nNewBufferSize = 128;

	_CharType* lpNewBuffer = NULL;
	int nFormat = -1; 

	// Loop until all of our text fits in the buffer
	while (nFormat == -1) {
		delete[] lpNewBuffer;
		nNewBufferSize *= 2;
		lpNewBuffer = new _CharType[nNewBufferSize];
		nFormat = _Traits::_format(lpNewBuffer, nNewBufferSize, lpFormat, argList);
	}

	if(nFormat > 0) {
		*this = lpNewBuffer;
	}

	delete[] lpNewBuffer;
}


// Template operations
template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
std::basic_ostream<_CharType, _Traits>& __cdecl operator<< (
		std::basic_ostream<_CharType, _Traits>& ostream,
		const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& str)
{
    return operator <<(ostream, static_cast<std::basic_string<_CharType, _Traits, _A> >(str)); 
}

template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> __cdecl operator+ (
	const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& lhs,
	const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& rhs
)
{
	return (basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>(lhs) += rhs); 
}

template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> __cdecl operator+ (
	const _CharType *lhs,
    const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& rhs
)
{
	return (basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>(lhs) += rhs); 
}

template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> __cdecl operator+ (
	const _CharType lhs,
	const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& rhs
)
{
	return (basic_string<_CharType, _ConversionCharType, _Traits, _A>(1, lhs) += rhs); 
}

template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> __cdecl operator+ (
	const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& lhs,
	const _CharType *rhs
)
{
	return (basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>(lhs) += rhs);
}

template<class _CharType, class _ConversionCharType, class _Traits, class _A> 
inline
basic_string_ex<_CharType, _ConversionCharType, _Traits, _A> __cdecl operator+ (
	const basic_string_ex<_CharType, _ConversionCharType, _Traits, _A>& lhs,
	const _CharType rhs
)
{
	return (basic_string_ex<_CharType, _Traits, _A>(lhs) += rhs);
}



typedef basic_string_ex< char, wchar_t, char_traits_ex<char, wchar_t>, std::allocator<char> > cstring;
typedef basic_string_ex< wchar_t, char, char_traits_ex<wchar_t, char>, std::allocator<wchar_t> > wstring;


#ifndef _UNICODE
typedef cstring string;
#else
typedef wstring string;
#endif

};
};


// Support for standard string streams
// with Unicode support
#include <iosfwd>
#include <streambuf>
#include <sstream>

namespace stingray {
namespace foundation {

typedef std::basic_stringstream<char, char_traits_ex<char, wchar_t>, std::allocator<char> > cstringstream;
typedef std::basic_stringstream<wchar_t, char_traits_ex<wchar_t, char>, std::allocator<wchar_t> > wstringstream;
#ifndef _UNICODE
typedef cstringstream stringstream;
#else
typedef wstringstream stringstream;
#endif


};	// namespace stingray::foundation
};  // namespace stingray