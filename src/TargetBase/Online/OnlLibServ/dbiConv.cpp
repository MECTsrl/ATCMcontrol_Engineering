/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/dbiConv.cpp 7     25.05.07 14:36 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: dbiConv.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/dbiConv.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 25.05.07 14:36 $
 *			 $Revision: 7 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#include "StdAfx.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#define __4CFILE__	"dbiConv.c"

/* ----  Includes:	 ---------------------------------------------------------- */

#include "vmSharedDef.h"
#include "vmShared.h"
#include "Connection.h"
#include "dbiConv.h"
#include "Config.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#define RETURN		return

/* ----  Global Variables:	 -------------------------------------------------- */

/* ----  Local Functions:	--------------------------------------------------- */

IEC_UINT Swap16(IEC_UINT u)
{
	return SwapShort(u);
}

IEC_UDINT Swap32(IEC_UDINT ul)
{
	return SwapLong(ul);
}

/* ----  Implementations:	--------------------------------------------------- */


/* CTokenizer
 * ----------------------------------------------------------------------------
 */
CTokenizer::CTokenizer()
{
	m_iNextToken	 = -1;
	m_szLastToken[0] = 0;
}

/* GetFirstToken
 * ----------------------------------------------------------------------------
 */
LPCTSTR	CTokenizer::GetFirstToken(CString const &s, LPCTSTR cszDelimiter)
{
	m_iNextToken	 = 0;
	m_szLastToken[0] = 0;

	return GetNextToken(s, cszDelimiter);
}

/* GetNextToken
 * ----------------------------------------------------------------------------
 */
LPCTSTR CTokenizer::GetNextToken(CString const &s, LPCTSTR cszDelimiter)
{
	INT iLen = s.GetLength();

	if (m_iNextToken != 0)
	{
		// Start search at the next position
		m_iNextToken += _tcslen(cszDelimiter);
	}

	if (m_iNextToken > iLen)
	{
		// EOL
		return NULL;
	}

	if (m_iNextToken == iLen)
	{
		int iIndex = s.Find(cszDelimiter, iLen - _tcslen(cszDelimiter));

		if (iIndex == -1)
		{
			// Last parameter is missing
			return NULL;
		}
		
		// String is terminated with a delimiter
		m_szLastToken[0] = 0;
		return m_szLastToken;
	}

	INT iStop = s.Find(cszDelimiter, m_iNextToken);

	if (iStop == -1)
	{
		// The last token. (No delimiter at the end of the string)
		iStop = iLen;
	}
	
	memcpy (m_szLastToken, (LPCTSTR)s + m_iNextToken, (iStop - m_iNextToken) * sizeof(TCHAR));
	m_szLastToken[iStop - m_iNextToken] = 0;

	m_iNextToken = iStop;
	return m_szLastToken;
}

/* IsLastToken
 * ----------------------------------------------------------------------------
 */
BOOL CTokenizer::IsLastToken(CString const &s, LPCTSTR cszDelimiter)
{
	int iPos = m_iNextToken + 1;

	while (IsOneOf(s, cszDelimiter, iPos))
	{
		// Skip leading delimiters
		iPos++;
	}

	if (iPos >= s.GetLength())
	{
		// no further tokens
		return TRUE;
	}

	// there are some more tokens
	return FALSE;
}

/* FindOneOf
 * ----------------------------------------------------------------------------
 * Finds the first occurance of any character in the string starting at the
 * given index.
 *
 * I cszDelimiter		Characters to be found.
 * I iStart				Index after which the search is started.
 * R					First index of occurance of one of the characters,
 *						-1 if not found.
 */
INT CTokenizer::FindOneOf(CString const &s, LPCTSTR cszDelimiter, INT iStart) const
{
	assert (cszDelimiter);

	INT iDelimiter	= _tcslen(cszDelimiter);
	INT iLen		= s.GetLength();
	INT j;

	for (int i = iStart; i < iLen; i++)
	{
		for (j = 0; j < iDelimiter; j++)
		{
			if (((LPCTSTR)s)[i] == cszDelimiter[j])
			{
				return i;
			}
		}
	}

	return -1;
}

/* IsOneOf
 * ----------------------------------------------------------------------------
 */
BOOL CTokenizer::IsOneOf(CString const &s, LPCTSTR cszDelimiter, INT iToTest) const
{
	if (iToTest >= s.GetLength())
	{
		return FALSE;
	}

	INT iDelimiter = _tcslen(cszDelimiter);

	for (int i = 0; i < iDelimiter; i++)
	{
		if (((LPCTSTR)s)[iToTest] == cszDelimiter[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}


// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CElem
 * ----------------------------------------------------------------------------
 */
CElem::CElem()
{
	m_pNext = NULL;
}

/* CElem
 * ----------------------------------------------------------------------------
 */
CElem::CElem(LPCTSTR szName)
{
	m_pNext = NULL;
	
	m_sName = szName;
	m_sName.MakeLower();
}

/* ~CElem
 * ----------------------------------------------------------------------------
 */
CElem::~CElem()
{

}


// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CElemArray
 * ----------------------------------------------------------------------------
 */
CElemArray::CElemArray()
{
	m_uCount = 0;

	m_pRoot		= NULL;
	m_pIterate	= NULL;
}

/* ~CElemArray
 * ----------------------------------------------------------------------------
 */
CElemArray::~CElemArray()
{
	DeleteAllElem();
}

/* GetCount
 * ----------------------------------------------------------------------------
 */
UINT CElemArray::GetCount()
{
	return m_uCount;
}

/* FindElem
 * ----------------------------------------------------------------------------
 */
CElem *CElemArray::FindElem(LPCSTR szElem)
{
	CElem *pElem = m_pRoot;

	while (pElem != NULL)
	{
		if (pElem->m_sName == szElem)
		{
			return pElem;
		}

		pElem = pElem->m_pNext;
	}

	return NULL;
}

/* GetElemAt
 * ----------------------------------------------------------------------------
 */
CElem *CElemArray::GetElemAt(int iIndex)
{
	CElem *pElem  = m_pRoot;
	int	   iLocal = 0;

	while (pElem != NULL)
	{
		if (iLocal == iIndex)
		{
			return pElem;
		}

		pElem = pElem->m_pNext;

		iLocal++;
	}

	return NULL;
}

/* GetFirst
 * ----------------------------------------------------------------------------
 */
CElem *CElemArray::GetFirst()
{
	m_pIterate = m_pRoot;

	return m_pIterate;
}

/* GetNext
 * ----------------------------------------------------------------------------
 */
CElem *CElemArray::GetNext()
{
	if (m_pIterate == NULL)
	{
		return NULL;
	}

	m_pIterate = m_pIterate->m_pNext;

	return m_pIterate;

}

/* AddElem
 * ----------------------------------------------------------------------------
 */
IEC_UINT CElemArray::AddElem(CElem *pAdd)
{
	m_pIterate = NULL;
	
	CElem *pElem = m_pRoot;
	CElem *pPrev = NULL;

	m_uCount++;

	while (pElem != NULL)
	{
		int iDiff = pElem->m_sName.CompareNoCase(pAdd->m_sName);

		if (iDiff >= 0)
		{
			if (pElem == m_pRoot)
			{
				// Add at head of the list
				pAdd->m_pNext = m_pRoot;
				m_pRoot		  = pAdd;
			}
			else
			{
				// Add in the middle of the list (before the current element)
				pPrev->m_pNext = pAdd;
				pAdd->m_pNext  = pElem;
			}

			RETURN(OK);
		}
		
		pPrev = pElem;
		pElem = pElem->m_pNext;
	}

	if (m_pRoot == NULL)
	{
		// Add first element to the list
		m_pRoot			 = pAdd;
		m_pRoot->m_pNext = NULL;
	}
	else
	{
		// Add at the tail of the list
		pAdd->m_pNext  = NULL;
		pPrev->m_pNext = pAdd;
	}

	RETURN(OK);
}

/* DeleteAllElem
 * ----------------------------------------------------------------------------
 */
void CElemArray::DeleteAllElem()
{
	m_pIterate = NULL;

	CElem *pElem = m_pRoot;
	CElem *pBack = NULL;

	while (pElem != NULL)
	{
		pBack = pElem->m_pNext;

		delete pElem;

		pElem = pBack;
	}

	m_pRoot  = NULL;
	m_uCount = 0;
}



// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CInstance
 * ----------------------------------------------------------------------------
 */
CInstance::CInstance()
{
	m_uType		= 0xffffu;
	m_uInst		= 0xffffu;
	
	m_uIndex	= 0xffffu;
}

/* CInstance
 * ----------------------------------------------------------------------------
 */
CInstance::CInstance(LPCTSTR szName) : CElem(szName)
{
	m_uType		= 0xffffu;
	m_uInst		= 0xffffu;
	
	m_uIndex	= 0xffffu;
}

/* ~CInstance
 * ----------------------------------------------------------------------------
 */
CInstance::~CInstance()
{
	m_aChildren.DeleteAllElem();
}

/* AddInstance
 * ----------------------------------------------------------------------------
 */
CInstance *CInstance::AddInstance(LPCTSTR szInst)
{
	CString sInst = szInst;
	
	int iPos = sInst.Find('.', 0);
	if (iPos == -1)
	{
		// A root object

		CInstance *pInst = (CInstance *)m_aChildren.FindElem(sInst);

		if (pInst == NULL)
		{
			pInst = new CInstance(sInst);

			m_aChildren.AddElem(pInst);
		}

		return pInst;
	}
	else
	{
		// An embedded object
		
		CString sDummy = sInst.Left(iPos);

		CInstance *pInst = (CInstance *)m_aChildren.FindElem(sDummy);

		if (pInst == NULL)
		{
			// Element does not exist, should not happen because __objects.hash
			// seems to be sorted hierarchically.

			return NULL;
		}

		sDummy = sInst.Right(sInst.GetLength() - iPos - 1);

		return pInst->AddInstance(sDummy);

	}
	
	return NULL;
}

/* Serialize
 * ----------------------------------------------------------------------------
 */
IEC_UINT CInstance::Serialize(HANDLE hFile, UINT *upSize, UINT *upFOAbsolut)
{
	XDBIInstance xInst;

	if (m_sName == "__globals")
	{
		m_sName = ".";
	}

	xInst.usType		= (IEC_USINT)m_uType;
	xInst.uInst			= Swap16(m_uInst);
	xInst.usNameSize	= (IEC_USINT)m_sName.GetLength();
	xInst.uIndex		= Swap16(m_uIndex);
	xInst.uChildren		= Swap16((IEC_UINT)m_aChildren.GetCount());
	xInst.ulFOAbsolut	= Swap32(*upFOAbsolut);

	DWORD dwWritten = 0;

	if (! WriteFile(hFile, &xInst, sizeof(XDBIInstance), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != sizeof(XDBIInstance))
	{
		RETURN(ERR_ERROR);
	}

	if (! WriteFile(hFile, (LPCTSTR)m_sName, m_sName.GetLength(), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != (DWORD)m_sName.GetLength())
	{
		RETURN(ERR_ERROR);
	}

	UINT uFOAbsolut = *upFOAbsolut;

	*upSize		  = sizeof(XDBIInstance) + m_sName.GetLength();
	*upFOAbsolut += *upSize;
	
	CInstance *pInst = (CInstance *)m_aChildren.GetFirst();
	while (pInst != NULL)
	{
		UINT uSize = 0;

		IEC_UINT uRes = pInst->Serialize(hFile, &uSize, upFOAbsolut);
		if (uRes != OK)
		{
			RETURN(uRes);
		}

		*upSize	+= uSize;

		pInst = (CInstance *)m_aChildren.GetNext();
	}

	xInst.ulFOBrother = Swap32(*upSize);

	if (SetFilePointer(hFile, uFOAbsolut, NULL, FILE_BEGIN) == INVALID_FILE_SIZE)
	{
		RETURN(ERR_ERROR);
	}

	if (! WriteFile(hFile, &xInst, sizeof(XDBIInstance), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != sizeof(XDBIInstance))
	{
		RETURN(ERR_ERROR);
	}

	if (SetFilePointer(hFile, 0, NULL, FILE_END) == INVALID_FILE_SIZE)
	{
		RETURN(ERR_ERROR);
	}

	RETURN(OK);
}


// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CType
 * ----------------------------------------------------------------------------
 */
CType::CType()
{

}

/* CType
 * ----------------------------------------------------------------------------
 */
CType::CType(LPCTSTR szName) : CElem(szName)
{

}

/* ~CType
 * ----------------------------------------------------------------------------
 */
CType::~CType()
{
	m_aMember.DeleteAllElem();
}

/* AddVariable
 * ----------------------------------------------------------------------------
 */
CVar *CType::AddVariable(LPCTSTR szName)
{
	CVar *pVar = new CVar(szName);

	m_aMember.AddElem(pVar);

	return pVar;
}

/* Serialize
 * ----------------------------------------------------------------------------
 */
IEC_UINT CType::Serialize(HANDLE hType, HANDLE hVar, UINT *upSize)
{
	XDBIType xType;
	IEC_UINT uType;

	int iPosL = m_sName.Find('[');
	int iPosR = m_sName.Find(']');


	xType.ulOffset = Swap32(*upSize);
	
	if (iPosR != -1 && iPosL != -1)
	{
		uType = DBI_DTM_ARRAY;
		
		CString sElems	= m_sName.Mid(iPosL + 1, iPosR - iPosL - 1);

		xType.uMember	= Swap16((IEC_UINT)strtoul(sElems, NULL, 10));
	}
	else
	{
		uType			= DBI_DTM_OBJECT;
		xType.uMember	= Swap16((IEC_UINT)m_aMember.GetCount());
	}

	/* Serialize member variables
	 */
	CVar *pVar  = (CVar *)m_aMember.GetFirst();
	BOOL bFirst = TRUE;

	while (pVar != NULL)
	{
		UINT uVar = 0;
		
		pVar->Serialize(hVar, &uVar);

		*upSize += uVar;

		if (bFirst == TRUE && uType == DBI_DTM_ARRAY)
		{
			/* Object is array ...
			 */
			if ((pVar->m_usType & DBI_DTM_SIMPLE) != 0)
			{
				/* ... of simple elements
				 */
				uType |= DBI_DTM_SIMPLE;
			}

			bFirst = FALSE;
		}

		pVar = (CVar *)m_aMember.GetNext();
	}

	xType.uType = Swap16(uType);
	
	/* Serialize type
	 */
	DWORD dwWritten = 0;
	if (! WriteFile(hType, &xType, sizeof(XDBIType), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != sizeof(XDBIType))
	{
		RETURN(ERR_ERROR);
	}	

	RETURN(OK);
}



// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CVar
 * ----------------------------------------------------------------------------
 */
CVar::CVar()
{
	m_usType	= 0;
	m_usDataType= 0;
	m_usBit		= 0xffu;
	m_uLen		= 0xffffu;
	m_uInst		= 0xffffu;
	m_ulOffset	= 0xfffffffful;
}

/* CVar
 * ----------------------------------------------------------------------------
 */
CVar::CVar(LPCTSTR szName) : CElem(szName)
{
	m_usType	= 0;
	m_usDataType= 0;
	m_usBit		= 0xffu;
	m_uLen		= 0xffffu;
	m_uInst		= 0xffffu;
	m_ulOffset	= 0xfffffffful;
}

/* ~CVar
 * ----------------------------------------------------------------------------
 */
CVar::~CVar()
{

}

/* Serialize
 * ----------------------------------------------------------------------------
 */
IEC_UINT CVar::Serialize(HANDLE hVar, UINT *upSize)
{
	XDBIVar xVar;

	xVar.usType	    = (IEC_USINT)(m_usType | m_usDataType);

	xVar.uLen		= Swap16(m_uLen);
	xVar.uInst		= Swap16(m_uInst);
	xVar.ulOffset	= Swap32(m_ulOffset);
	xVar.usBit		= m_usBit;

	xVar.usNameSize	= (IEC_USINT)m_sName.GetLength();

	DWORD dwWritten = 0;

	if (! WriteFile(hVar, &xVar, sizeof(XDBIVar), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != sizeof(XDBIVar))
	{
		RETURN(ERR_ERROR);
	}

	if (! WriteFile(hVar, (LPCTSTR)m_sName, m_sName.GetLength(), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != (DWORD)m_sName.GetLength())
	{
		RETURN(ERR_ERROR);
	}
	
	*upSize += sizeof(XDBIVar) + m_sName.GetLength();

	RETURN(OK);
}


// ============================================================================
// ============================================================================
// ============================================================================
// ============================================================================

/* CConvert
 * ----------------------------------------------------------------------------
 */
CConvert::CConvert()
{
	m_pInstRoot = new CInstance("__globals");

	m_pTypes	= new CObArray;

	m_pTypes->SetSize(0, 1000);
}

/* ~CConvert
 * ----------------------------------------------------------------------------
 */
CConvert::~CConvert()
{
	delete m_pInstRoot;
	m_pInstRoot = NULL;

	for (int i = 0; i < m_pTypes->GetSize(); i++)
	{
		delete m_pTypes->GetAt(i);
	}

	m_pTypes->RemoveAll();

	delete m_pTypes;
	m_pTypes = NULL;
}

/* ImportProject
 * ----------------------------------------------------------------------------
 */
IEC_UINT CConvert::ImportProject(LPCTSTR szPath, LPCTSTR szFile)
{
	/* Import globals first
	 */
	IEC_UINT uRes = ImportType(szPath, "__globals", &m_pInstRoot->m_uIndex);
	if (uRes != OK)
	{
		RETURN(uRes);
	}

	CString sFile = szPath;
	sFile += szFile;

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		RETURN(ERR_FILE_OPEN);
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		RETURN(ERR_ERROR);
	}

	CString sContent;

	CHAR *pBuff = sContent.GetBufferSetLength(dwSize + 1);
	DWORD dwRead = 0;

	if (! ReadFile(hFile, pBuff, dwSize, &dwRead, NULL))
	{
		sContent.ReleaseBuffer(0);
		CloseHandle(hFile);
		RETURN(ERR_ERROR);
	}

	CloseHandle(hFile);

	if (dwRead != dwSize)
	{
		sContent.ReleaseBuffer(0);
		RETURN(ERR_ERROR);
	}
	
	pBuff[dwSize] = 0;
	sContent.ReleaseBuffer(dwSize);

	CString sLine;

	CTokenizer tokMain;
	CTokenizer tokLine;

	LPCTSTR szLine = tokMain.GetFirstToken(sContent, "\r\n");

	while (szLine != NULL && szLine[0] != '\0')
	{
		sLine = szLine;

		
		/* Create the instance
		 * --------------------------------------------------------------------
		 */

		/* Instance name
		 */
		LPCTSTR szToken = tokLine.GetFirstToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in __objects.hash found.");
			RETURN(ERR_ERROR);
		}

		CInstance *pInst = m_pInstRoot->AddInstance(szToken);
		if (pInst == NULL)
		{
			RETURN(ERR_ERROR);
		}

		/* Task ID - skip!
		 */
		szToken = tokLine.GetNextToken(sLine, ";");

		/* Object ID
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in __objects.hash found.");
			RETURN(ERR_ERROR);
		}

		pInst->m_uInst = (IEC_UINT)strtoul(szToken, NULL, 10);

		/* Offset - skip!
		 */
		szToken = tokLine.GetNextToken(sLine, ";");

		/* Size - skip!
		 */
		szToken = tokLine.GetNextToken(sLine, ";");

		/* Skip next 3 members
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		szToken = tokLine.GetNextToken(sLine, ";");
		szToken = tokLine.GetNextToken(sLine, ";");

		/* Class ID - skip!
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		
		/* Class name
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in __objects.hash found.");
			RETURN(ERR_ERROR);
		}

		/* Check for array
		 */
		pInst->m_uType = (IEC_UINT)(szToken[strlen(szToken) - 1] == ']' ? DBI_DTM_ARRAY : DBI_DTM_OBJECT);


		/* Lookup the type
		 * --------------------------------------------------------------------
		 */
		uRes = ImportType(szPath, szToken, &pInst->m_uIndex);
		if (uRes != OK)
		{
			RETURN(ERR_ERROR);
		}

		szLine = tokMain.GetNextToken(sContent, "\r\n");

	} /* while (szLine != NULL && szLine[0] != '\0') */

	RETURN(OK);
}

/* ImportType
 * ----------------------------------------------------------------------------
 */
IEC_UINT CConvert::ImportType(LPCTSTR szPath, LPCTSTR szType, IEC_UINT *upIndex)
{	
	*upIndex = FindType(szType);
	if (*upIndex != 0xffffu)
	{
		/* Type already exists.
		 */

		RETURN(OK);
	}


	/* Create a new entry
	 * ------------------------------------------------------------------------
	 */
	*upIndex = AddType(szType);
	if (*upIndex == 0xffffu)
	{
		RETURN(ERR_ERROR);
	}


	/* Read the member variables
	 * ------------------------------------------------------------------------
	 */
	CType *pType = (CType *)m_pTypes->GetAt(*upIndex);
	if (pType == NULL)
	{
		RETURN(ERR_ERROR);
	}

	CString sFile = szPath;
	sFile += szType;
	sFile += DEF_FILE_EXT;

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		RETURN(ERR_FILE_OPEN);
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		RETURN(ERR_ERROR);
	}

	CString sContent;
	
	CHAR *pBuff = sContent.GetBufferSetLength(dwSize + 1);
	DWORD dwRead = 0;

	if (! ReadFile(hFile, pBuff, dwSize, &dwRead, NULL))
	{
		sContent.ReleaseBuffer(0);
		CloseHandle(hFile);
		RETURN(ERR_ERROR);
	}

	CloseHandle(hFile);

	if (dwRead != dwSize)
	{
		sContent.ReleaseBuffer(0);
		RETURN(ERR_ERROR);
	}
	
	pBuff[dwSize] = 0;
	sContent.ReleaseBuffer(dwSize);


	/* Create the member variables
	 * ------------------------------------------------------------------------
	 */
	unsigned uLine = 0;

	CString sLine;

	CTokenizer tokMain;
	CTokenizer tokLine;

	CVar Var("");

	LPCTSTR szLine = tokMain.GetFirstToken(sContent, "\r\n");

	while (szLine != NULL && szLine[0] != '\0')
	{
		sLine = szLine;

		/* CSV_IDX_NAME: Instance name
		 * --------------------------------------------------------------------
		 */
		LPCTSTR szToken = tokLine.GetFirstToken(sLine, ";");
		if (szToken == NULL || szToken[0] == '\0')
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		Var.m_sName = szToken;


		/* CSV_IDX_DECL_SPEC: Declaration Specification
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL || szToken[0] == '\0')
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		BOOL bGlobal = FALSE;

		if (szToken[0] == 'g' || szToken[0] == 'G')
		{
			Var.m_usType = DBI_DTM_SIMPLE;
			bGlobal = TRUE;
		}
		else if (szToken[0] == 'n' || szToken[0] == 'N')
		{
			Var.m_usType = DBI_DTM_SIMPLE;
		}
		else if (szToken[0] == 's' || szToken[0] == 'S')
		{
			Var.m_usType = DBI_DTM_OBJECT;
		}
		else
		{
			Var.m_usType = 0;
			
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		IEC_UINT uLen  = 0;
		IEC_UINT uInst = 0xffffu;

		BOOL bPointer = (szToken[1] == '*');
		
		if (bPointer == TRUE)
		{
			/* Don't add pointer variable because they can 
			 * not be watched via VisuComm the library.
			 */
			goto next_line;
		}

		
		/* CSV_IDX_SEGMENT: Segment Information
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}
			
		if (szToken[0] != '\0')
		{
			uInst = (IEC_UINT)strtoul(szToken, NULL, 10);
		}


		/* CSV_IDX_TYPE: Data Type
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL || szToken[0] == '\0')
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}
		
		if (Var.m_usType == DBI_DTM_SIMPLE)
		{
			// Only for simple variables

			if (stricmp(szToken, "bool") == 0)
			{
				Var.m_usDataType	= DBI_DT_BOOL;
				Var.m_uLen			= 1;
			}
			else if (stricmp(szToken, "byte") == 0)
			{
				Var.m_usDataType	= DBI_DT_BYTE;
				Var.m_uLen			= 1;
			}
			else if (stricmp(szToken, "word") == 0)
			{
				Var.m_usDataType	= DBI_DT_WORD;
				Var.m_uLen			= 2;
			}
			else if (stricmp(szToken, "dword") == 0)
			{
				Var.m_usDataType	= DBI_DT_DWORD;
				Var.m_uLen			= 4;
			}
			else if (stricmp(szToken, "lword") == 0)
			{
				Var.m_usDataType	= DBI_DT_LWORD;
				Var.m_uLen			= 8;
			}
			else if (stricmp(szToken, "usint") == 0)
			{
				Var.m_usDataType	= DBI_DT_USINT;
				Var.m_uLen			= 1;
			}
			else if (stricmp(szToken, "uint") == 0)
			{
				Var.m_usDataType	= DBI_DT_UINT;
				Var.m_uLen			= 2;
			}
			else if (stricmp(szToken, "udint") == 0)
			{
				Var.m_usDataType	= DBI_DT_UDINT;
				Var.m_uLen			= 4;
			}
			else if (stricmp(szToken, "ulint") == 0)
			{
				Var.m_usDataType	= DBI_DT_ULINT;
				Var.m_uLen			= 8;
			}
			else if (stricmp(szToken, "sint") == 0)
			{
				Var.m_usDataType	= DBI_DT_SINT;
				Var.m_uLen			= 1;
			}
			else if (stricmp(szToken, "int") == 0)
			{
				Var.m_usDataType	= DBI_DT_INT;
				Var.m_uLen			= 2;
			}
			else if (stricmp(szToken, "dint") == 0)
			{
				Var.m_usDataType	= DBI_DT_DINT;
				Var.m_uLen			= 4;
			}
			else if (stricmp(szToken, "lint") == 0)
			{
				Var.m_usDataType	= DBI_DT_LINT;
				Var.m_uLen			= 8;
			}
			else if (stricmp(szToken, "real") == 0)
			{
				Var.m_usDataType	= DBI_DT_REAL;
				Var.m_uLen			= 4;
			}
			else if (stricmp(szToken, "lreal") == 0)
			{
				Var.m_usDataType	= DBI_DT_LREAL;
				Var.m_uLen			= 8;
			}
			else if (stricmp(szToken, "string") == 0)
			{
				Var.m_usDataType	= DBI_DT_STRING;
				Var.m_uLen			= 0xffffu;
			}
			else if (stricmp(szToken, "time") == 0)
			{
				Var.m_usDataType	= DBI_DT_TIME;
				Var.m_uLen			= 4;
			}
			else
			{
				Var.m_usDataType	= DBI_DT_UNKNOWN;
				Var.m_uLen			= 4;

				assert(! "Invalid entry in *.def2 file found.");
				RETURN(ERR_ERROR);
			}
		
		} // if (Var.m_usType == DBI_DTM_SIMPLE)
			

		if (Var.m_usType == DBI_DTM_OBJECT)
		{
			// Check if object is an array

			CString sDummy(szToken);

			int iPosL = sDummy.Find('[');
			int iPosR = sDummy.Find(']');

			if (iPosR != -1 && iPosL != -1)
			{
				Var.m_usType	= DBI_DTM_ARRAY;

				CString sElems	= sDummy.Mid(iPosL + 1, iPosR - iPosL - 1);

				Var.m_uLen		= (IEC_UINT)strtoul(sElems, NULL, 10);
			}
		}
		
		/* Compute the segment
		 */
		if (Var.m_usType == DBI_DTM_SIMPLE && bGlobal == TRUE)
		{
			if (uInst == 0xffffu)
			{
				assert(! "Invalid entry in *.def2 file found.");
				RETURN(ERR_ERROR);
			}

			Var.m_uInst = uInst;
		}


		/* CSV_IDX_BYTE_OFFSET: Byte Offset
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		if (Var.m_usType == DBI_DTM_SIMPLE)
		{
			if (szToken[0] == '\0')
			{
				assert(! "Invalid entry in *.def2 file found.");
				RETURN(ERR_ERROR);
			}

			Var.m_ulOffset = strtoul(szToken, NULL, 10);
		}


		/* CSV_IDX_BIT_OFFSET: Bit Offset
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		Var.m_usBit = 0;

		if (Var.m_usDataType == DBI_DT_BOOL)
		{
			Var.m_usBit = (IEC_USINT)(strtoul(szToken, NULL, 10) + 1);
		}


		/* CSV_IDX_BIT_SIZE: Bit Size
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}
		
		if (Var.m_usType == DBI_DTM_SIMPLE)
		{
			if (szToken[0] == '\0')
			{
				assert(! "Invalid entry in *.def2 file found.");
				RETURN(ERR_ERROR);
			}

			uLen = (IEC_UINT)((strtoul(szToken, NULL, 10) + 7) / 8);
		}

		
		/* CSV_IDX_STRLEN: String Length
		 * --------------------------------------------------------------------
		 */
		szToken = tokLine.GetNextToken(sLine, ";");
		if (szToken == NULL)
		{
			assert(! "Invalid entry in *.def2 file found.");
			RETURN(ERR_ERROR);
		}

		if (Var.m_usDataType == DBI_DT_STRING)
		{
			if (szToken[0] == '\0')
			{
				assert(! "Invalid entry in *.def2 file found.");
				RETURN(ERR_ERROR);
			}

			Var.m_uLen = (IEC_UINT)strtoul(szToken, NULL, 10);
		}


		/* Check for array elements
		 */
		if (uLine > 0 && szType[strlen(szType) - 1] == ']')
		{
			/* Only store the first element of a simple array.
			 */
			szLine = NULL;		
			continue;
		}


		/* Add Variable to list
		 */
		if (bPointer == FALSE)
		{
			/* Don't add pointer variable because they can 
			 * not be watched via VisuComm the library.
			 */
			CVar *pVar = pType->AddVariable(Var.m_sName);
			if (pVar == NULL)
			{
				RETURN(ERR_ERROR);
			}

			pVar->m_uInst		= Var.m_uInst;
			pVar->m_uLen		= Var.m_uLen;
			pVar->m_ulOffset	= Var.m_ulOffset;
			pVar->m_usBit		= Var.m_usBit;
			pVar->m_usDataType	= Var.m_usDataType;
			pVar->m_usType		= Var.m_usType;
		}

next_line:

		/* Next Line
		 * --------------------------------------------------------------------
		 */
		szLine = tokMain.GetNextToken(sContent, "\r\n");

		Var.m_sName		= "";
		Var.m_usType	= 0;
		Var.m_usDataType= 0;
		Var.m_usBit		= 0xffu;
		Var.m_uLen		= 0xffffu;
		Var.m_uInst		= 0xffffu;
		Var.m_ulOffset	= 0xfffffffful;

		uLine++;

	} /* while (szLine != NULL && szLine[0] != '\0') */

	RETURN(OK);
}

/* FindType
 * ----------------------------------------------------------------------------
 */
IEC_UINT CConvert::FindType(LPCTSTR szType)
{
	for (int i = 0; i < m_pTypes->GetSize(); i++)
	{
		if (((CElem *)m_pTypes->GetAt(i))->m_sName == szType)
		{
			return (IEC_UINT)i;
		}
	}

	return 0xffff;
}

/* AddType
 * ----------------------------------------------------------------------------
 */
IEC_UINT CConvert::AddType(LPCTSTR szType)
{
	CType *pType = new CType (szType);

	m_pTypes->Add(pType);

	return (IEC_UINT)(m_pTypes->GetSize() - 1);
}

/* Serialize
 * ----------------------------------------------------------------------------
 */
IEC_UINT CConvert::Serialize(LPCTSTR szPath)
{
	CString sFile = szPath;
	sFile += DBI_FILE_INSTANCE;

	/* Serialize instance tree
	 */
	HANDLE hFile = CreateFile(sFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		RETURN(ERR_ERROR);
	}

	UINT uSize		= 0;
	UINT uOffset	= 0;

	IEC_UINT uRes = m_pInstRoot->Serialize(hFile, &uSize, &uOffset);

	CloseHandle(hFile);

	if (uRes != OK)
	{
		RETURN(uRes);
	}

	/* Serialize type tree
	 */
	sFile = szPath;
	sFile += DBI_FILE_TYPE;

	HANDLE hType = CreateFile(sFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hType == INVALID_HANDLE_VALUE)
	{
		RETURN(ERR_ERROR);
	}

	sFile = szPath;
	sFile += DBI_FILE_VAR;

	HANDLE hVar = CreateFile(sFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hType == INVALID_HANDLE_VALUE)
	{
		RETURN(ERR_ERROR);
	}

	/* Write number of types first
	 */
	DWORD dwWritten = 0;
	DWORD dwTypes	= Swap32(m_pTypes->GetSize());

	if (! WriteFile(hType, &dwTypes, sizeof(DWORD), &dwWritten, NULL))
	{
		RETURN(ERR_ERROR);
	}
	if (dwWritten != sizeof(DWORD))
	{
		RETURN(ERR_ERROR);
	}	

	uSize = 0;

	for (int i = 0; i < m_pTypes->GetSize(); i++)
	{
		CType *pType = (CType *)m_pTypes->GetAt(i);

		uRes = pType->Serialize(hType, hVar, &uSize);
		if (uRes != OK)
		{
			break;
		}
	}

	CloseHandle(hType);
	CloseHandle(hVar);

	RETURN(uRes);
}

/* ---------------------------------------------------------------------------- */
