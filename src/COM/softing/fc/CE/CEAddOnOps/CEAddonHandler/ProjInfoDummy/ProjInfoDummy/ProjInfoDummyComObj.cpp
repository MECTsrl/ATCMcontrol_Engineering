// ProjInfoDummyComObj.cpp : Implementation of CProjInfoDummyComObj
#include "stdafx.h"
#include "ProjInfoDummy.h"
#include "ProjInfoDummyComObj.h"

#include "..\..\ContainerDummy\ContainerDummy\ContainerDummy_i.c"
#include "..\..\ContainerDummy\ContainerDummy\CEContainer_i.c"

/////////////////////////////////////////////////////////////////////////////
// CProjInfoDummyComObj

CProjInfoDummyComObj::CProjInfoDummyComObj()
{
    m_pIUnkCEContainer = NULL;
}

CProjInfoDummyComObj::~CProjInfoDummyComObj()
{
    if(m_pIUnkCEContainer)
    {
        disconnectCEContainer();
    }
}

STDMETHODIMP CProjInfoDummyComObj::fileChangedHint(BSTR sSourceFile)
{
    CString strMsg = _T("ICEProjInfo::fileChangedHint\n");
    strMsg += _T("\nsSourceFile:\t");
    strMsg += sSourceFile;
    AfxMessageBox(strMsg);

	return S_OK;
}

STDMETHODIMP CProjInfoDummyComObj::getContainerIF(BSTR sSourceFile,
                                                  BSTR sId,
                                                  IUnknown ** ppICEContainer,
                                                  BSTR * psParentSource,
                                                  BSTR * psParentId)
{
    CString strMsg = _T("ICEProjInfo::getContainerIF\n");
    strMsg += _T("\nsSourceFile:\t");
    strMsg += sSourceFile;
    strMsg += _T("\nsId:\t\t");
    strMsg += sId;
    strMsg += _T("\npsParentSource:\t");
    strMsg += *psParentSource;
    strMsg += _T("\npsParentId:\t");
    strMsg += *psParentId;
    AfxMessageBox(strMsg);

    *psParentSource = L"byVal";
    *psParentId = L"byVal";

    connectCEContainer();

    *ppICEContainer = m_pIUnkCEContainer;

	if (ppICEContainer == NULL)
		return E_POINTER;
		
	if (psParentSource == NULL)
		return E_POINTER;
		
	if (psParentId == NULL)
		return E_POINTER;
		
    (*ppICEContainer)->AddRef();

	return S_OK;
}

STDMETHODIMP CProjInfoDummyComObj::getContainerIFforFile(BSTR sContainerFile,
                                                         IUnknown ** ppICEContainer)
{
    CString strMsg = _T("ICEProjInfo::getContainerIFforFile\n");
    strMsg += _T("\nsContainerFile:\t");
    strMsg += sContainerFile;
    AfxMessageBox(strMsg);

    connectCEContainer();

    *ppICEContainer = m_pIUnkCEContainer;

	if (ppICEContainer == NULL)
		return E_POINTER;
		
    (*ppICEContainer)->AddRef();

	return S_OK;
}

BOOL CProjInfoDummyComObj::connectCEContainer()
{
    if(!m_pIUnkCEContainer)
    {
	    HRESULT hRes = CoCreateInstance(CLSID_ContainerDummyComObj,
							    NULL,
							    CLSCTX_INPROC_SERVER,
							    //IID_ICEContainer,
                                IID_IUnknown,
							    (LPVOID *) &m_pIUnkCEContainer);
        ASSERT(SUCCEEDED(hRes));
        if(SUCCEEDED(hRes))
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CProjInfoDummyComObj::disconnectCEContainer()
{
    if(m_pIUnkCEContainer)
    {
        m_pIUnkCEContainer->Release();
        m_pIUnkCEContainer = NULL;
        return TRUE;
    }
    return FALSE;
}
