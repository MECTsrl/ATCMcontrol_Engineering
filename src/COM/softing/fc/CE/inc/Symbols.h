#ifndef __SYMBOLS_H_
#define __SYMBOLS_H_

#include "baselib.h"


class CTypeDescriptor
{
public:
	CTypeDescriptor ();

	virtual CString AsString () = 0;
};


class CSimpleType :public CTypeDescriptor
{
public:
	enum BuiltinType
	{
		BuiltinUndefined = -1,

		BuiltinANY     = BL_ANY,
		BuiltinBOOL    = BL_BOOL,
		BuiltinBYTE    = BL_BYTE,
		BuiltinWORD    = BL_WORD,
		BuiltinDWORD   = BL_DWORD,
		BuiltinDINT    = BL_DINT,
		BuiltinLREAL   = BL_LREAL,
		BuiltinTIME    = BL_TIME,
		BuiltinDT      = BL_DT,
		BuiltinDATE    = BL_DATE,
		BuiltinTOD     = BL_TOD,
		BuiltinWSTRING = BL_WSTRING,
        BuiltinUINT    = BL_UINT,
        BuiltinUDINT   = BL_UDINT,

        BuiltinFirst = BuiltinANY,
		BuiltinLast = BuiltinUDINT,
	};

	CSimpleType (BuiltinType builtinType, bool bQVT);

	virtual CString AsString ();

	BuiltinType GetBuiltinType ();
	bool IsQVT ();

	static BuiltinType BuiltinTypeFromString (const CString &strName, bool &bQVT);

protected:
	BuiltinType m_builtinType;
	bool        m_bQVT;

	static LPCTSTR m_builtinTypeNames[];
	static LPCTSTR m_builtinQVTTypeNames[];
};


class CArrayType : public CTypeDescriptor
{
public:
	CArrayType (CSimpleType *pElementType, int iLowerBound, int iUpperBound);

	CSimpleType *GetElementType ();
	int GetLowerBound ();
	int GetUpperBound ();

	virtual CString AsString ();

protected:
	CSimpleType *m_pElementType;
	int         m_iLowerBound;
	int         m_iUpperBound;
};


class CTypeTable
{
public:
	CTypeTable ();
	~CTypeTable ();

	void Invalidate ();

	CSimpleType *GetSimpleType (CSimpleType::BuiltinType builtinType, bool bQVT);
	CArrayType *GetArrayType (CSimpleType *pElementType, int iLowerBound, int iUpperBound);

	CArray<CSimpleType *, CSimpleType *> m_simpleTypes;
	CArray<CSimpleType *, CSimpleType *> m_simpleQVTTypes;

protected:
	CSimpleType *m_pUnknownType;

	CList<CTypeDescriptor *, CTypeDescriptor *> m_UserTypes;

private:
	void DisposeUserTypes ();
};


class CSymbol
{
public:
	CSymbol ();
	CSymbol (CTypeDescriptor *type, const CString &strName);

	CTypeDescriptor *GetType ();
	CString GetName ();

	void *GetClientData ();
	void SetClientData (void *pClientData);

	// Must have at least one virtual method for
	// dynamic cast to work. Remove if some other
	// virtual method is added.
	virtual void Dummy () { }

protected:
	CTypeDescriptor *m_pType;
	CString         m_strName;
	void            *m_pClientData;
};


class CVariableSymbol : public CSymbol
{
public:
	CVariableSymbol (CTypeDescriptor *type, const CString &strName);
};


class CSymbolTable
{
public:
	CSymbolTable ();
	~CSymbolTable ();

	void SetSymbolFile (const CString &strSymbolFile, const CString &strPrefix="", bool bWantGlobals=true);
	void Invalidate ();
	
	unsigned int GetSymbolCount ();
	CSymbol *GetSymbolAt (unsigned int uiIndex);

	CSymbol *FindSymbol (const CString &strSymbolName);

protected:
	CString                      m_strSymbolFile;
	CString                      m_strPrefix;
	bool                         m_bWantGlobals;
	CArray<CSymbol *, CSymbol *> *m_pSymbols;
	CTypeTable                   *m_pTypeTable;

private:
	bool ReadSymbols ();
	bool HasPrefix (const CString &strName);
	bool IsGlobalName (const CString &strName);
	void DisposeSymbols ();
};

#endif