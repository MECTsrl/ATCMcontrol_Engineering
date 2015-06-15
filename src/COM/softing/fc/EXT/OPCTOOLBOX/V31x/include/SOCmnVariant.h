//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnVariant.h                                              |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : VARIANT object class                                        |
//                - SOCmnVariant: VARIANT object class                        |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNVARIANT_H_
#define _SOCMNVARIANT_H_

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOCmnVariant                                                               |
//-----------------------------------------------------------------------------

#ifndef LPCVARIANT
typedef const VARIANT* LPCVARIANT;
#endif

class SOCMN_EXPORT SOCmnVariant : public tagVARIANT
{
public:
	SOCmnVariant();
	SOCmnVariant(IN const SOCmnVariant& cmnVar);
	SOCmnVariant(IN const VARIANT& var);
	SOCmnVariant(IN LPCVARIANT pvar);
	SOCmnVariant(IN const LPCTSTR sz);
	SOCmnVariant(IN char c);
	SOCmnVariant(IN BYTE b);
	SOCmnVariant(IN short s);
	SOCmnVariant(IN unsigned short us);
	SOCmnVariant(IN long l);
	SOCmnVariant(IN unsigned long ul);
	SOCmnVariant(IN float flt);
	SOCmnVariant(IN double dbl);
	SOCmnVariant(IN BOOL bo);
	SOCmnVariant(IN DECIMAL dec);
	SOCmnVariant(IN __int64 cy);
	~SOCmnVariant();

	const SOCmnVariant& operator=(IN SOCmnVariant& cmnVar);
	const SOCmnVariant& operator=(IN const VARIANT& var);
	const SOCmnVariant& operator=(IN LPCVARIANT pvar);
	const SOCmnVariant& operator=(IN const LPCTSTR sz);
	const SOCmnVariant& operator=(IN char c);
	const SOCmnVariant& operator=(IN BYTE b);
	const SOCmnVariant& operator=(IN short s);
	const SOCmnVariant& operator=(IN unsigned short us);
	const SOCmnVariant& operator=(IN long l);
	const SOCmnVariant& operator=(IN unsigned long ul);
	const SOCmnVariant& operator=(IN float flt);
	const SOCmnVariant& operator=(IN double dbl);
	const SOCmnVariant& operator=(IN BOOL bo);
	const SOCmnVariant& operator=(IN DECIMAL dec);
	const SOCmnVariant& operator=(IN __int64 cy);

	SOCmnVariant& set(IN void *src, IN VARTYPE type, IN WORD elSize);

	operator LPVARIANT();
	operator LPCVARIANT() const;

	BOOL operator==(IN const VARIANT& var) const;
	BOOL operator==(IN LPCVARIANT pvar) const;
	BOOL operator==(IN const SOCmnVariant& cmnVar) const;

	// clear variant
	void Clear(void);
	void clear(void);

	BOOL copyTo(OUT LPVARIANT destination);

	LPCTSTR toString(OUT SOCmnString *string) const;

	void init(IN VARTYPE datatype);

	// datatype conversion
	BOOL changeDatatype(IN VARTYPE type);
	static BOOL changeDatatype(IN VARIANT *value, IN VARTYPE type, OUT VARIANT *changedValue);
	static BOOL changeDatatype(IN VARIANT *value, IN VARTYPE type, IN LCID localeID,
				OUT VARIANT *changedValue);

	// compare variants
	static BOOL areEqual(IN LPCVARIANT pvar1, IN LPCVARIANT pvar2);
	static BOOL areEqualArrays(IN LPCVARIANT pvar1, IN LPCVARIANT pvar2);
}; // SOCmnVariant

inline SOCmnVariant::operator LPVARIANT()
{ return this; }

inline SOCmnVariant::operator LPCVARIANT() const
{ return this; }

inline void SOCmnVariant::Clear(void)
{ clear(); }

inline BOOL SOCmnVariant::changeDatatype(IN VARTYPE type)
{ return changeDatatype(this, type, this); }

inline BOOL SOCmnVariant::operator==(IN const VARIANT& var) const
{ return areEqual(this, (LPCVARIANT)&var); }

inline BOOL SOCmnVariant::operator==(IN LPCVARIANT pvar) const
{ return areEqual(this, pvar); }

inline BOOL SOCmnVariant::operator==(IN const SOCmnVariant& cmnVar) const
{ return areEqual(this, (LPCVARIANT)&cmnVar); }

#pragma pack(pop)
#endif
