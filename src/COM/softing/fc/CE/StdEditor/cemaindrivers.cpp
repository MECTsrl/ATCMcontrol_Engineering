// Von Klassen-Assistent automatisch erstellte IDispatch-Kapselungsklasse(n).

#include "stdafx.h"
#include "cemaindrivers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// Eigenschaften CGenEdItemDriver 

/////////////////////////////////////////////////////////////////////////////
// Operationen CGenEdItemDriver 

SCODE CGenEdItemDriver::GetDocument(LPDISPATCH* pDocumentDispatch)
{
	SCODE result;
	static BYTE parms[] =
		VTS_PDISPATCH;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		pDocumentDispatch);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Eigenschaften CGenEdDocDriver 

/////////////////////////////////////////////////////////////////////////////
// Operationen CGenEdDocDriver 

SCODE CGenEdDocDriver::GetContainer(LPDISPATCH* pContainerDispatch)
{
	SCODE result;
	static BYTE parms[] =
		VTS_PDISPATCH;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		pContainerDispatch);
	return result;
}

SCODE CGenEdDocDriver::SetBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
	SCODE result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		lQualifier, lX, lY, lCX, lCY);
	return result;
}

SCODE CGenEdDocDriver::ClearBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
	SCODE result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		lQualifier, lX, lY, lCX, lCY);
	return result;
}

SCODE CGenEdDocDriver::ActivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
	SCODE result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4  VTS_I4  VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		lQualifier, lX, lY, lCX, lCY);
	return result;
}

SCODE CGenEdDocDriver::DeactivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
	SCODE result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		lQualifier, lX, lY, lCX, lCY);
	return result;
}


SCODE CGenEdDocDriver::ContinueTask()
{
	SCODE result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_ERROR, (void*)&result, NULL);
	return result;
}

//mode=0: single step, mode=1: step over, mode=2: step out 
SCODE CGenEdDocDriver::StepTask(long mode)
{
	SCODE result;
	static BYTE parms[] =VTS_I4;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms, mode);
	return result;
}


SCODE CGenEdDocDriver::ClearInstanceBreakpoints()
{
	SCODE result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_ERROR, (void*)&result, NULL);
	return result;
}


SCODE CGenEdDocDriver::ClearAllBreakpoints()
{
	SCODE result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_ERROR, (void*)&result, NULL);
	return result;
}

SCODE CGenEdDocDriver::OnEdit()
{
	SCODE result;
	InvokeHelper(0xA, DISPATCH_METHOD, VT_ERROR, (void*)&result, NULL);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// Eigenschaften CGenEdContDriver 

/////////////////////////////////////////////////////////////////////////////
// Operationen CGenEdContDriver 

SCODE CGenEdContDriver::ShowURL(LPCTSTR szURL)
{
	SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szURL);
	return result;
}

SCODE CGenEdContDriver::ShowFile(LPCTSTR szFile)
{
	SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szFile);
	return result;
}


SCODE CGenEdContDriver::SelectFBForCall()
{
	SCODE result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_ERROR, (void*)&result, NULL);
	return result;
}

SCODE CGenEdContDriver::ShowVarSelection(LPCTSTR szPouName)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPouName);
	return result;
}

SCODE CGenEdContDriver::SetVarSelectionHint(LPCTSTR szPouName,
                                            LPCTSTR szVarNamePrefix,
                                            LPCTSTR szDataType)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPouName, szVarNamePrefix, szDataType);
	return result;
}

SCODE CGenEdContDriver::ShowHelp(LPCTSTR szPOU)
{
	SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPOU);
	return result;
}

SCODE CGenEdContDriver::ShowHeader(LPCTSTR szPOU)
{
	SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPOU);
	return result;
}

SCODE CGenEdContDriver::ShowHeaderFile(LPCTSTR szFileName)
{
	SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szFileName);
	return result;
}

SCODE CGenEdContDriver::GetLRUPous(VARIANT* parrPous)
{
    SCODE result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		parrPous);
	return result;
}

SCODE CGenEdContDriver::GetPouXML(LPCTSTR szPouName, BSTR* pszPouDef)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR VTS_PBSTR;
	InvokeHelper(0xA, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPouName, pszPouDef);
	return result;
}

SCODE CGenEdContDriver::GetVarList(VARIANT* parrVars)
{
    SCODE result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xB, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		parrVars);
	return result;
}

SCODE CGenEdContDriver::GetPouList(VARIANT* parrPous)
{
    SCODE result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xC, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		parrPous);
	return result;
}

SCODE CGenEdContDriver::GetPouDefinition(LPCTSTR szPouName, BSTR* pszPouDef)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR VTS_PBSTR;
	InvokeHelper(0xD, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPouName, pszPouDef);
	return result;
}

SCODE CGenEdContDriver::GetPouMember(LPCTSTR szPouName, VARIANT* parrMembers)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0xE, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		szPouName, parrMembers);
	return result;
}


SCODE CGenEdContDriver::XRefChangeRefPosition(BOOL bForward)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xF, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		bForward);
	return result;
}


SCODE CGenEdContDriver::XRefSetActiveQuery(LPCTSTR sVariableName)
{
    SCODE result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		sVariableName);
	return result;

}


SCODE CGenEdContDriver::ShowVarSelectionBox(LONG lXPos, LONG lYPos,
                                            LPCTSTR szLine, LONG lColumn,
                                            LPCTSTR szTypeHint,
                                            LPCTSTR szPouName,
                                            BSTR FAR* pszSelection,
                                            LONG* plStartColumn,
                                            LONG* plEndColumn)
{
    SCODE result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_PBSTR VTS_PI4 VTS_PI4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		lXPos, lYPos, szLine, lColumn, szTypeHint, szPouName, pszSelection, plStartColumn, plEndColumn);
	return result;

}

SCODE CGenEdContDriver::GetProjectManager(IUnknown** pProjManUnk)
{
    SCODE result;
	static BYTE parms[] =
		VTS_PUNKNOWN;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_ERROR, (void*)&result, parms,
		pProjManUnk);
	return result;
}

