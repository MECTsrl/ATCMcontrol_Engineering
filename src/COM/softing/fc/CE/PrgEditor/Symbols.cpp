
#include "stdafx.h"
#include "Symbols.h"


CTypeTable::CTypeTable ()
{
	CSimpleType::BuiltinType builtinType;

	m_pUnknownType = new CSimpleType (CSimpleType::BuiltinUndefined, false);

	for (builtinType = CSimpleType::BuiltinFirst;
	     builtinType <= CSimpleType::BuiltinLast;
		 builtinType = (CSimpleType::BuiltinType)((int)builtinType + 1))
	{
		m_simpleTypes.SetAtGrow (builtinType, new CSimpleType (builtinType, false));
		m_simpleQVTTypes.SetAtGrow (builtinType, new CSimpleType (builtinType, true));
	};
}


CTypeTable::~CTypeTable ()
{
	CSimpleType::BuiltinType builtinType;

	DisposeUserTypes ();

	delete m_pUnknownType;

	for (builtinType = CSimpleType::BuiltinANY;
	     builtinType <= CSimpleType::BuiltinLast;
		 builtinType = (CSimpleType::BuiltinType) ((int)builtinType + 1))
	{
		delete m_simpleTypes[builtinType];
		delete m_simpleQVTTypes[builtinType];
	};
}


void CTypeTable::Invalidate ()
{
	DisposeUserTypes ();
}


CSimpleType *CTypeTable::GetSimpleType (CSimpleType::BuiltinType builtinType, bool bQVT)
{
	if (builtinType == CSimpleType::BuiltinUndefined)
	{
		return (m_pUnknownType);
	};

	if (!bQVT)
	{
		return (m_simpleTypes[builtinType]);
	};

	return (m_simpleQVTTypes[builtinType]);
}


CArrayType *CTypeTable::GetArrayType (CSimpleType *pElementType, int iLowerBound, int iUpperBound)
{
	POSITION pPosition;
	CArrayType *pArrayType;

	pPosition = m_UserTypes.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CTypeDescriptor *pType;

		pType = m_UserTypes.GetNext (pPosition);

		if ((pArrayType = dynamic_cast<CArrayType *> (pType)) != NULL)
		{
			if (pArrayType->GetElementType () == pElementType &&
			    pArrayType->GetLowerBound () == iLowerBound &&
			    pArrayType->GetUpperBound () == iUpperBound)
			{
				return (pArrayType);
			};
		};
	};

	pArrayType = new CArrayType (pElementType, iLowerBound, iUpperBound);
	m_UserTypes.AddTail (pArrayType);

	return (pArrayType);
}


void CTypeTable::DisposeUserTypes ()
{
	POSITION pPosition;

	pPosition = m_UserTypes.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CTypeDescriptor *pType;

		pType = m_UserTypes.GetNext (pPosition);
		delete pType;
	};

	m_UserTypes.RemoveAll ();
}


CTypeDescriptor::CTypeDescriptor ()
{
}


LPCTSTR CSimpleType::m_builtinTypeNames[] =
{
	_T ("ANY"),
	_T ("BOOL"),
	_T ("BYTE"),
	_T ("WORD"),
	_T ("DWORD"),
	_T ("DINT"),
	_T ("LREAL"),
	_T ("TIME"),
	_T ("DT"),
	_T ("DATE"),
	_T ("TOD"),
	_T ("WSTRING")
};


LPCTSTR CSimpleType::m_builtinQVTTypeNames[] =
{
	_T ("QVT_ANY"),
	_T ("QVT_BOOL"),
	_T ("QVT_BYTE"),
	_T ("QVT_WORD"),
	_T ("QVT_DWORD"),
	_T ("QVT_DINT"),
	_T ("QVT_LREAL"),
	_T ("QVT_TIME"),
	_T ("QVT_DT"),
	_T ("QVT_DATE"),
	_T ("QVT_TOD"),
	_T ("QVT_WSTRING")
};


CSimpleType::CSimpleType (BuiltinType builtinType, bool bQVT)
: m_builtinType (builtinType),
  m_bQVT (bQVT)
{
}


CString CSimpleType::AsString ()
{
	CString strType;

	if (m_builtinType == BuiltinUndefined)
	{
		return (_T ("undefined"));
	};

	if (m_builtinType > BuiltinLast)
	{
		return (_T ("unknown"));
	};

	if (!m_bQVT)
	{
		return (m_builtinTypeNames[m_builtinType]);
	};

	return (m_builtinQVTTypeNames[m_builtinType]);
}


CSimpleType::BuiltinType CSimpleType::GetBuiltinType ()
{
	return (m_builtinType);
}


bool CSimpleType::IsQVT ()
{
	return (m_bQVT);
}


CSimpleType::BuiltinType CSimpleType::BuiltinTypeFromString (const CString &strName, bool &bQVT)
{
	HRESULT hr;
	BL_IEC_TYP iecType;
	DWORD modifiers;
	USES_CONVERSION;

	hr = BL_TypenameToDatatype (T2COLE (strName), &iecType, &modifiers);
	if (FAILED (hr))
	{
		return (BuiltinUndefined);
	};

	bQVT = (modifiers & BL_DTYPE_IS_QVT) != 0;
	return ((BuiltinType)iecType);
}


CArrayType::CArrayType (CSimpleType *pElementType, int iLowerBound, int iUpperBound)
: m_pElementType (pElementType),
  m_iLowerBound (iLowerBound),
  m_iUpperBound (iUpperBound)
{
}


CSimpleType *CArrayType::GetElementType ()
{
	return (m_pElementType);
}


int CArrayType::GetLowerBound ()
{
	return (m_iLowerBound);
}


int CArrayType::GetUpperBound ()
{
	return (m_iUpperBound);
}


CString CArrayType::AsString ()
{
	CString strType;

	strType.Format (_T ("ARRAY [%d..%d] OF %s"), m_iLowerBound, m_iUpperBound, (LPCTSTR)m_pElementType->AsString ());
	return (strType);
}


////////////////////////////////////////////////////////////////////////////////
// class CSymbolTable

static bool IsIdentifier (char c);
static bool ScanBound (const CString &strLine, int &iLineIndex, int &iBound);


CSymbolTable::CSymbolTable ()
: m_pSymbols (NULL)
{
	m_pTypeTable = new CTypeTable;
	m_bWantGlobals = true;
}


CSymbolTable::~CSymbolTable ()
{
	DisposeSymbols ();
	delete m_pTypeTable;
}


void CSymbolTable::SetSymbolFile (const CString &strSymbolFile, const CString& strPrefix, bool bWantGlobals)
{
	Invalidate ();
	m_strSymbolFile = strSymbolFile;
	m_strPrefix = strPrefix;
	m_bWantGlobals = bWantGlobals;
}


void CSymbolTable::Invalidate ()
{
	DisposeSymbols ();
	m_pTypeTable->Invalidate ();
}


unsigned int CSymbolTable::GetSymbolCount ()
{
	if (m_pSymbols == NULL)
	{
		if (!ReadSymbols ())
		{
			return (0);
		};
	};

	ASSERT (m_pSymbols != NULL);
	if (m_pSymbols == NULL)
	{
		return (0);
	};

	return (m_pSymbols->GetSize ());
}


CSymbol *CSymbolTable::GetSymbolAt (unsigned int uiIndex)
{
	CSymbol *pSymbol;

	if (m_pSymbols == NULL)
	{
		if (!ReadSymbols ())
		{
			return (NULL);
		};
	};

	ASSERT (m_pSymbols != NULL);
	if (m_pSymbols == NULL)
	{
		return (NULL);
	};

	if (uiIndex >= (unsigned int)m_pSymbols->GetSize ())
	{
		return (NULL);
	};

	pSymbol = (*m_pSymbols)[uiIndex];
	return (pSymbol);
}


CSymbol *CSymbolTable::FindSymbol (const CString &strSymbolName)
{
	int iIndex;

	if (m_pSymbols == NULL)
	{
		if (!ReadSymbols ())
		{
			return (NULL);
		};
	};

	ASSERT (m_pSymbols != NULL);
	if (m_pSymbols == NULL)
	{
		return (NULL);
	};

	for (iIndex = 0; iIndex < m_pSymbols->GetSize (); ++iIndex)
	{
		CString strEntry;

		strEntry = (*m_pSymbols)[iIndex]->GetName ();

		if (strEntry.CompareNoCase (strSymbolName) == 0)
		{
			return (*m_pSymbols)[iIndex];
		};
	};

	return (NULL);
}


bool CSymbolTable::ReadSymbols ()
{
	CString strLine;

	if (m_pSymbols != NULL)
	{
		return (true);
	};

	m_pSymbols = new CArray<CSymbol *, CSymbol *>;

	if (m_strSymbolFile.IsEmpty ())
	{
		return (true);
	};


	TRY
	{
		CStdioFile file (m_strSymbolFile, CFile::modeRead);
		CString strLine;


		while (file.ReadString (strLine))
		{
			int iLineIndex=0;
			CString strBuiltinType;
			CSimpleType::BuiltinType builtinType;
			bool bQVT;
			bool bIsArray=false;
			int iLowerBound;
			int iUpperBound;
			CString strName;
			CTypeDescriptor *pType;
			CVariableSymbol *pVariable;

			while (iLineIndex < strLine.GetLength () &&
			       IsIdentifier (strLine[iLineIndex]))
			{
				++iLineIndex;
			};

			if (iLineIndex == 0)
			{
				continue;
			};

			strBuiltinType = strLine.Mid (0, iLineIndex);

			builtinType = CSimpleType::BuiltinTypeFromString (strBuiltinType, bQVT);

			if (iLineIndex < strLine.GetLength () &&
			    strLine[iLineIndex] == '[')
			{
				bIsArray = true;

				++iLineIndex;

				if (!ScanBound (strLine, iLineIndex, iLowerBound))
				{
					continue;
				};

				if (iLineIndex >= strLine.GetLength () - 1 ||
				    strLine[iLineIndex] != '.' ||
				    strLine[iLineIndex + 1] != '.')
				{
					continue;
				};

				iLineIndex += 2;

				if (!ScanBound (strLine, iLineIndex, iUpperBound))
				{
					continue;
				};

				if (iLineIndex >= strLine.GetLength () ||
				    strLine[iLineIndex] != ']')
				{
					continue;
				};

				++iLineIndex;
			};


			while (iLineIndex <= strLine.GetLength () &&
			       (strLine[iLineIndex] == ' ' || strLine[iLineIndex] == '\t'))
			{
				++iLineIndex;
			};

			if (iLineIndex == strLine.GetLength ())
			{
				continue;
			};

			strName = strLine.Right (strLine.GetLength () - iLineIndex);

			if (!HasPrefix (strName) && (!m_bWantGlobals || !IsGlobalName (strName)))
			{
				continue;
			};

			pType = m_pTypeTable->GetSimpleType (builtinType, bQVT);
			if (bIsArray)
			{
				pType = m_pTypeTable->GetArrayType ((CSimpleType *)pType, iLowerBound, iUpperBound);
			};

			pVariable = new CVariableSymbol (pType, strName);
			m_pSymbols->Add (pVariable);
		};

		file.Close ();
	}
	CATCH (CFileException, e)
	{
		e->Delete ();
		return (false);
	}
	END_CATCH

	return (true);
}


static bool IsIdentifier (char c)
{
	return (IsCharAlphaNumeric (c) || c == '_');
}


static bool ScanBound (const CString &strLine, int &iLineIndex, int &iBound)
{
	bool bGoodBound=false;
	bool bNegative=false;

	iBound = 0;

	if (strLine[iLineIndex] == '-')
	{
		bNegative = true;
		++iLineIndex;
	};

	while (iLineIndex <= strLine.GetLength () &&
	       strLine[iLineIndex] >= '0' && strLine[iLineIndex] <= '9')
	{
		bGoodBound = true;
		iBound = iBound * 10 + strLine[iLineIndex] - '0';
		++iLineIndex;
	};

	if (bNegative)
	{
		iBound = -iBound;
	};

	return (bGoodBound);
}


bool CSymbolTable::HasPrefix (const CString &strName)
{
	if (m_strPrefix.IsEmpty ())
	{
		return (true);
	};

	if (strName.GetLength () <= m_strPrefix.GetLength ())
	{
		return (false);
	};

	return (strName.Left (m_strPrefix.GetLength ()).CompareNoCase (m_strPrefix) == 0);
}


bool CSymbolTable::IsGlobalName (const CString &strName)
{
	int iPos;
	CString strTemp;

	if ((iPos = strName.Find (_T ('.'))) < 0)
	{
		return (true);
	};

	strTemp = strName.Mid (iPos + 1);
	if ((iPos = strTemp.Find (_T ('.'))) < 0)
	{
		return (true);
	};

	strTemp = strTemp.Mid (iPos + 1);
	return (strTemp.Find (_T ('.')) <0);
}


void CSymbolTable::DisposeSymbols ()
{
	if (m_pSymbols == NULL)
	{
		return;
	};

	for (int iIndex = 0; iIndex < m_pSymbols->GetSize (); ++iIndex)
	{
		delete (*m_pSymbols)[iIndex];
	};

	delete m_pSymbols;
	m_pSymbols = NULL;
}



////////////////////////////////////////////////////////////////////////////////
// class CSymbol

CSymbol::CSymbol (CTypeDescriptor *pType, const CString &strName)
: m_pType (pType),
  m_strName (strName),
  m_pClientData (NULL)
{
}


CTypeDescriptor *CSymbol::GetType ()
{
	return (m_pType);
}


CString CSymbol::GetName ()
{
	return (m_strName);
}

void *CSymbol::GetClientData ()
{
	return (m_pClientData);
}


void CSymbol::SetClientData (void *pClientData)
{
	m_pClientData = pClientData;
}



////////////////////////////////////////////////////////////////////////////////
// class CVariableSymbol

CVariableSymbol::CVariableSymbol (CTypeDescriptor *pType, const CString &strName)
: CSymbol (pType, strName)
{
}
