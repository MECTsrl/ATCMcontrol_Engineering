// ContainerDummyComObj.cpp : Implementation of CContainerDummyComObj
#include "stdafx.h"
#include "ContainerDummy.h"
#include "ContainerDummyComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CContainerDummyComObj

CContainerDummyComObj::CContainerDummyComObj()
{
}

STDMETHODIMP CContainerDummyComObj::createLink(BSTR sFileName, BSTR sLocation, BSTR sLinkName)
{
    CString strMsg = _T("ICEContainer::createLink\n");
    strMsg += _T("\nsFileName:\t");
    strMsg += sFileName;
    strMsg += _T("\nsLocation:\t");
    strMsg += sLocation;
    strMsg += _T("\nsLinkName:\t");
    strMsg += sLinkName;
    AfxMessageBox(strMsg);

    return S_OK;
}

STDMETHODIMP CContainerDummyComObj::renameLink(BSTR sFileName, BSTR sLocation, BSTR sOldLinkName, BSTR sNewLinkName)
{
    CString strMsg = _T("ICEContainer::renameLink\n");
    strMsg += _T("\nsFileName:\t");
    strMsg += sFileName;
    strMsg += _T("\nsLocation:\t");
    strMsg += sLocation;
    strMsg += _T("\nsOldLinkName:\t");
    strMsg += sOldLinkName;
    strMsg += _T("\nsNewLinkName:\t");
    strMsg += sNewLinkName;
    AfxMessageBox(strMsg);

    return S_OK;
}

STDMETHODIMP CContainerDummyComObj::deleteLink(BSTR sFileName, BSTR sLocation, BSTR sLinkName)
{
    CString strMsg = _T("ICEContainer::deleteLink\n");
    strMsg += _T("\nsFileName:\t");
    strMsg += sFileName;
    strMsg += _T("\nsLocation:\t");
    strMsg += sLocation;
    strMsg += _T("\nsLinkName:\t");
    strMsg += sLinkName;
    AfxMessageBox(strMsg);

    return S_OK;
}

