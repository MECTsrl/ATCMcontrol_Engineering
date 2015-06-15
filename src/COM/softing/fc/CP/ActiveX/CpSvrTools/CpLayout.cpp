// CpLayout.cpp : Implementation von CCpLayout
#include "stdafx.h"
#include "CpSvrTools.h"
#include "CpLayout.h"

/////////////////////////////////////////////////////////////////////////////
// CCpLayout

STDMETHODIMP CCpLayout::OnStartPage (IUnknown* pUnk)  
{
	if(!pUnk)
		return E_POINTER;

	CComPtr<IScriptingContext> spContext;
	HRESULT hr;

	// Anfordern des IScriptingContext Interfaces
	hr = pUnk->QueryInterface(IID_IScriptingContext, (void **)&spContext);
	if(FAILED(hr))
		return hr;
	
	
	m_bOnStartPageCalled = TRUE;
	return S_OK;
}

STDMETHODIMP CCpLayout::OnEndPage ()  
{
	m_bOnStartPageCalled = FALSE;
	// Freigabe aller Interfaces

	return S_OK;
}

//*****************************************************************************
STDMETHODIMP CCpLayout::Init(
	BSTR bstrPath, 
	BSTR bstrUser)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(bstrPath != NULL);
	if (bstrPath == NULL)
		return S_FALSE;
	m_Path = bstrPath;
	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	ASSERT(bstrUser != NULL);
	if (bstrUser == NULL)
		return S_FALSE;
	CString strUser(bstrUser);
	ASSERT(strUser.GetLength() > 0);
	if (strUser.GetLength() == 0)
		return S_FALSE;

	m_Path += "\\users";
	CreateDirectory(m_Path, NULL);

	m_Path += "\\" + strUser;
	CreateDirectory(m_Path, NULL);

	m_Path += "\\Layout";
	CreateDirectory(m_Path, NULL);

	return S_OK;
}

//*****************************************************************************
STDMETHODIMP CCpLayout::Save(
	BSTR bstrName, 
	BSTR bstrData)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	ASSERT(bstrName != NULL);
	if (bstrName == NULL)
		return S_FALSE;
	CString strName(bstrName);
	ASSERT(strName.GetLength() > 0);
	if (strName.GetLength() == 0)
		return S_FALSE;
	strName += ".4la";

	CStdioFile File;
	if (File.Open(m_Path + "\\" + strName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		File.WriteString(CString(bstrData));
		File.Close();
		return S_OK;
	}

	return S_FALSE;
}

//*****************************************************************************
STDMETHODIMP CCpLayout::Read(
	BSTR bstrName, 
	BSTR* pbstrData)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	ASSERT(bstrName != NULL);
	if (bstrName == NULL)
		return S_FALSE;
	CString strName(bstrName);
	ASSERT(strName.GetLength() > 0);
	if (strName.GetLength() == 0)
		return S_FALSE;
	strName += ".4la";

	CStdioFile File;
	if (File.Open(m_Path + "\\" + strName, CFile::modeRead | CFile::typeText))
	{
		CString data;
		File.ReadString(data);
		*pbstrData = data.AllocSysString();
		File.Close();
		return S_OK;
	}

	return S_FALSE;
}

//*****************************************************************************
STDMETHODIMP CCpLayout::GetDefaultName(
	BSTR* pName)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	CString strName;
	CStdioFile File;
	if (File.Open(m_Path + "\\name.ini", CFile::modeRead | CFile::typeText))
	{
		File.ReadString(strName);
		File.Close();
	}

	*pName= strName.AllocSysString();
	return S_OK;
}

//*****************************************************************************
STDMETHODIMP CCpLayout::SetDefaultName(
	BSTR bstrName)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	ASSERT(bstrName != NULL);
	if (bstrName == NULL)
		return S_FALSE;
	CString strName(bstrName);
	ASSERT(strName.GetLength() > 0);
	if (strName.GetLength() == 0)
		return S_FALSE;

	CStdioFile File;
	if (File.Open(m_Path + "\\name.ini", CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		File.WriteString(strName);
		File.Close();
		return S_OK;
	}

	return S_FALSE;
}
//*****************************************************************************
STDMETHODIMP CCpLayout::GetNameList(
	BSTR* pbstrList)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	CString strList;

	WIN32_FIND_DATA FileData;
	HANDLE hff = FindFirstFile(m_Path + "\\*.4la", &FileData);
	if (hff != INVALID_HANDLE_VALUE)
	{
		do
		{
			FileData.cFileName[strlen(FileData.cFileName) - 4] = '\0';
			strList += CString(FileData.cFileName) + ';';
		}
		while (FindNextFile(hff, &FileData));
		FindClose(hff);
	}
	
	*pbstrList = strList.AllocSysString();

	return S_OK;
}
//*****************************************************************************
STDMETHODIMP CCpLayout::Delete(
	BSTR bstrName)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_Path.GetLength() > 0);
	if (m_Path.GetLength() == 0)
		return S_FALSE;

	ASSERT(bstrName != NULL);
	if (bstrName == NULL)
		return S_FALSE;
	CString strName(bstrName);
	ASSERT(strName.GetLength() > 0);
	if (strName.GetLength() == 0)
		return S_FALSE;
	strName += ".4la";

	return remove(m_Path + "\\" + strName) == -1 ? S_FALSE : S_OK;
}
