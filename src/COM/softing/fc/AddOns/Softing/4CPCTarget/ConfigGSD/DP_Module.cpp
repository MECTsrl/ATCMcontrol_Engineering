// DP_Module.cpp: implementation of the CDP_Module class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DP_Module.h"
#include "ExtParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//---------------------------- CONSTRUCTOR------------------------------------*
CDP_Module::CDP_Module(int id)
{
    m_ModuleId = id;
    m_dwUserPrmLen = 0;
	RemoveAllConfBytes();
	RemoveAllUserPrmBytes();
}

//------------------------- COPY - CONSTRUCTOR -------------------------------*
CDP_Module::CDP_Module(CDP_Module const & other)
{
	*this = other;		// just call assignment
}

//---------------------------- DESTRUCTOR ------------------------------------*
CDP_Module::~CDP_Module()
{
	POSITION pos = m_ExtParams.GetHeadPosition();
	while (pos)	{
		ExtParam* pParam = (ExtParam*)m_ExtParams.GetNext(pos);
		delete pParam;
	}
}

//---------------------------- ASSIGNMENT ------------------------------------*
const CDP_Module&
	CDP_Module::operator = (const CDP_Module& other)
{
	if (this == &other)
		return *this;	// copying myself, I'm done

    m_ModuleId = other.m_ModuleId;
	m_dwUserPrmLen = other.m_dwUserPrmLen;
	m_szModule = other.m_szModule;
    m_ConfigBytes.RemoveAll();
	m_ConfigBytes.InsertAt(0, (CByteArray*)&(other.m_ConfigBytes));
    m_UserPrmBytes.RemoveAll();
	m_UserPrmBytes.InsertAt(0, (CByteArray*)&(other.m_UserPrmBytes));

	m_ExtParams.RemoveAll();
	POSITION pos = other.m_ExtParams.GetHeadPosition();
	while (pos)	 {
		ExtParam* pElement = new ExtParam(*((ExtParam*)other.m_ExtParams.GetNext(pos)));
		m_ExtParams.AddTail(pElement);
	}

	return *this;
}

//----------------------------- FUNCTION -------------------------------------*
void CDP_Module::SetModuleName(CString szModule)
{
	m_szModule = szModule;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::AddConfigByte(unsigned char ucByte)
{
	if (m_ConfigBytes.Add(ucByte) >= 0)
		return TRUE;
	else
		return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::AddUserPrmByte(unsigned char ucByte)
{
	if (m_UserPrmBytes.Add(ucByte) >= 0)
		return TRUE;
	else
		return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::SetConfigBytes(unsigned char* pucArray, DWORD dwLength)
{
	unsigned char* pucTemp = pucArray;
	RemoveAllConfBytes();

	if (dwLength == 0)
		return FALSE;
	
	for (DWORD i = 0; i < dwLength; i++)	{
		AddConfigByte(*pucTemp);
		pucTemp++;
	}

	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::SetUserPrmBytes(unsigned char* pucArray, DWORD dwLength)
{
	unsigned char* pucTemp = pucArray;
	RemoveAllUserPrmBytes();

	if (dwLength == 0)
		return FALSE;

	for (DWORD i = 0; i < dwLength; i++)	{
		AddUserPrmByte(*pucTemp);
		pucTemp++;
	}

	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::RemoveAllConfBytes()
{
	m_ConfigBytes.RemoveAll();
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::RemoveAllUserPrmBytes()
{
	m_UserPrmBytes.RemoveAll();
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
CString CDP_Module::GetModuleName()
{
	return m_szModule;
}

//----------------------------- FUNCTION -------------------------------------*
void CDP_Module::AddExtParamDef(ExtParam* pParam)	{
	m_ExtParams.AddTail(pParam);
}

//----------------------------- FUNCTION -------------------------------------*
CString CDP_Module::GetState()
{
    CString retval;
    ExtParam*pParam;
    int iParam = 0;
    int nParams = 0;
    POSITION pos = m_ExtParams.GetHeadPosition();
    while (pos)	{
        pParam = (ExtParam*)m_ExtParams.GetNext(pos);
        if (pParam->IsUsed()) {
            CString val;
            val.Format(_T("%d;"), iParam);
            val += pParam->GetState();
            retval += val;
            nParams++;
        }
        iParam++;
    }
    CString hdr;
    hdr.Format(_T("%d;"), nParams);
    retval = hdr + retval;
    return retval;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CDP_Module::SetState(LPCTSTR &buffer)
{
    int nParams = GetNextValue(buffer);
    for (int i=0; i<nParams; i++) {
        int iParam = GetNextValue(buffer);
        POSITION pos = m_ExtParams.GetHeadPosition();
        ExtParam*pParam = NULL;

        for (int j=0; j<=iParam && pos; j++)	{
            ExtParam*pNext = (ExtParam*)m_ExtParams.GetNext(pos);
            if (j == iParam)
                pParam = pNext;
            
        }

        if (pParam == NULL || pParam->IsConstantParam() || pParam->IsUsed()) {
            return FALSE;
        }

        pParam->SetUsed(TRUE);
        if (!pParam->SetState(buffer)) {
            return FALSE;
        }
    }
    return TRUE;
}

