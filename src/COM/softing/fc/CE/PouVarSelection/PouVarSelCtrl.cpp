

#include "stdafx.h"
#include "PouVarSelection.h"
#include "PouVarSelCtrl.h"

#include "VarSelectionBox.h"

#include "CEMsg.h"
#include "CEMsg_i.c"

#include <afxcmn.h>




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @exception       -
 * @see             
*/
CPouVarSelCtrl::CPouVarSelCtrl() : 
    m_ctlSysTabControl32(WC_TABCONTROL, this, 1)  
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    InitCommonControls();
    m_bWindowOnly = TRUE;
    m_bDisabled = FALSE;
}


              

//------------------------------------------------------------------*
/**
 * destructor.
 *
 * @exception       -
 * @see             
*/
CPouVarSelCtrl::~CPouVarSelCtrl()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    TCHAR szBuff[128];
    wsprintf(szBuff, _T("ATL:%s"), m_ctlSysTabControl32.m_lpszClassName);
    UnregisterClass(szBuff, _Module.GetModuleInstance());
}



//------------------------------------------------------------------*
/**
 * OnDraw.
 *
 * @param           ATL_DRAWINFO& di:
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CPouVarSelCtrl::OnDraw(ATL_DRAWINFO& di)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pouSelDlg.m_hWnd != NULL && ::IsWindow(m_pouSelDlg.m_hWnd))
    {
        m_pouSelDlg.Invalidate();
    }
    if (m_varSelDlg.m_hWnd != NULL && ::IsWindow(m_varSelDlg.m_hWnd))
    {
        m_varSelDlg.Invalidate();
    }

    if (m_ctlSysTabControl32.m_hWnd != NULL)
    {
        m_ctlSysTabControl32.Invalidate();
    }
	return S_OK;
}



//------------------------------------------------------------------*
/**
 * OnDestroy
 *
 * @exception       -
 * @see             
*/
LRESULT CPouVarSelCtrl::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_ctlSysTabControl32.m_hWnd != NULL)
    {
        TabCtrl_DeleteAllItems(m_ctlSysTabControl32);
    }
    if (m_pouSelDlg.m_hWnd != NULL && ::IsWindow(m_pouSelDlg.m_hWnd))
    {
        m_pouSelDlg.DestroyWindow();
    }
    if (m_varSelDlg.m_hWnd != NULL && ::IsWindow(m_varSelDlg.m_hWnd))
    {
        m_varSelDlg.DestroyWindow();
    }
    return 0;
}



//------------------------------------------------------------------*
/**
 * OnCreate.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
LRESULT CPouVarSelCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nTabs = 2;
    UINT id = IDS_TITLE_POUSEL;
    RECT rc;
    GetWindowRect(&rc);
    rc.right -= rc.left;
    rc.bottom -= rc.top;
    m_ctlSysTabControl32.Create(m_hWnd, rc, _T(""), 
        WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE | TCS_OWNERDRAWFIXED );
 
    TC_ITEM tie;
    for (int i=0; i<nTabs; i++)
    {
        CString strTitle;
        strTitle.LoadString(id+i);
        tie.mask = TCIF_TEXT; // | TCIF_IMAGE;
        tie.iImage = -1;

        tie.pszText = strTitle.GetBuffer(strTitle.GetLength()+1); 
        tie.cchTextMax = strTitle.GetLength(); 
        if (TabCtrl_InsertItem(m_ctlSysTabControl32.m_hWnd, i, &tie) == -1)
        {
            AfxMessageBox(IDS_CREATE_ERROR, MB_OK | MB_ICONSTOP);
            strTitle.ReleaseBuffer();
            return E_FAIL;
        }
        strTitle.ReleaseBuffer();
    }

    rc.top += 30;
    rc.bottom -= 10;
    rc.left += 5;
    rc.right -= 10;
    m_pouSelDlg.Create(m_ctlSysTabControl32.m_hWnd, rc);
    ASSERT(::IsWindow(m_pouSelDlg.m_hWnd));
    m_varSelDlg.Create(m_ctlSysTabControl32.m_hWnd, rc);
    ASSERT(::IsWindow(m_varSelDlg));
    TabCtrl_SetCurSel(m_ctlSysTabControl32.m_hWnd, 0);
    DWORD dwStyle;
    dwStyle = m_varSelDlg.GetWindowLong(GWL_EXSTYLE);
    dwStyle |= WS_EX_CONTROLPARENT;
    m_varSelDlg.SetWindowLong(GWL_EXSTYLE, dwStyle);
    dwStyle = m_pouSelDlg.GetWindowLong(GWL_EXSTYLE);
    dwStyle |= WS_EX_CONTROLPARENT;
    m_pouSelDlg.SetWindowLong(GWL_EXSTYLE, dwStyle);

    dwStyle = m_ctlSysTabControl32.GetWindowLong(GWL_EXSTYLE);
    dwStyle |= WS_EX_CONTROLPARENT;
    m_ctlSysTabControl32.SetWindowLong(GWL_EXSTYLE, dwStyle);

    dwStyle = GetWindowLong(GWL_EXSTYLE);
    dwStyle |= WS_EX_CONTROLPARENT;
    SetWindowLong(GWL_EXSTYLE, dwStyle);

    m_pouSelDlg.ShowWindow(SW_SHOW);
    m_pouSelDlg.SetFocus();

	return 0;
}


//------------------------------------------------------------------*
/**
 * OnDrawItem: Owner drawing of the tab ctrls
 * this is necessary because the tab control uses a strange font
 * used as contained window in the ATL. Here the font can be changed.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
LRESULT CPouVarSelCtrl::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;

    DRAWITEMSTRUCT& ds = *lpDrawItemStruct;
   
    int iItem = ds.itemID;

    // Get tab item info
    char text[128];
    TCITEM tci;
    tci.mask = TCIF_TEXT;
    tci.pszText = text;
    tci.cchTextMax = sizeof(text);
    //GetItem(iItem, &tci);
    TabCtrl_GetItem(m_ctlSysTabControl32.m_hWnd, iItem , &tci);
    
    // use draw item DC
    CDC dc;
    dc.Attach(ds.hDC);

    LOGFONT logFont;
    CFont adaptedFont;

    NONCLIENTMETRICS sysMetrics;
    int iSize = sizeof(sysMetrics);
    ZeroMemory(&sysMetrics, iSize);
    sysMetrics.cbSize = iSize;
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, iSize, &sysMetrics, 0);

    logFont = sysMetrics.lfMenuFont; //lfCaptionFont;
    if (logFont.lfHeight<0)
        logFont.lfHeight += 2;
    adaptedFont.CreateFontIndirect(&logFont);

    dc.SelectObject(adaptedFont);

    // calculate text rectangle and color
    CRect rc = ds.rcItem;

    CRect fillrc = rc;
    fillrc.top += 4;
    CBrush brush;

    dc.FillSolidRect(&fillrc, dc.GetBkColor());

    rc += CPoint(1,5);                   // ?? by trial and error

    // draw the text
    dc.SetTextColor(GetSysColor(COLOR_BTNTEXT));
    dc.DrawText(text, &rc, DT_CENTER|DT_VCENTER);

    dc.Detach();

	return 0;
}



//------------------------------------------------------------------*
/**
 * OnSize: Calculate size of the tab control and the two
 * dlg pages and resize all.
 *
 * @exception       -
 * @see             
*/
LRESULT CPouVarSelCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (! ( (m_ctlSysTabControl32.m_hWnd != NULL)
         && (m_pouSelDlg.m_hWnd != NULL && ::IsWindow(m_pouSelDlg.m_hWnd))
         && (m_varSelDlg.m_hWnd != NULL && ::IsWindow(m_varSelDlg.m_hWnd)) ) )
    {
        return 0;
    }

    int cx, cy;
    cx = LOWORD(lParam);
    cy = HIWORD(lParam);
    ::SetWindowPos(m_ctlSysTabControl32.m_hWnd, HWND_TOP, 0, 0, cx, cy,
        SWP_SHOWWINDOW ); //SWP_NOZORDER | SWP_NOACTIVATE);

    int cxDlg = cx - 12;
    int cyDlg = cy - 40;
    ::SetWindowPos(m_pouSelDlg.m_hWnd, NULL, 5, 30, cxDlg, cyDlg, 
        SWP_NOZORDER | SWP_NOACTIVATE);
    ::SetWindowPos(m_varSelDlg.m_hWnd, NULL, 5, 30, cxDlg, cyDlg, 
        SWP_NOZORDER | SWP_NOACTIVATE);

    return 0;
}


LRESULT CPouVarSelCtrl::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_ctlSysTabControl32.SetFocus ();
    return 0;
}

//------------------------------------------------------------------*
/**
 * OnSelChanged handler is called when the user selects anothe tab.
 *
 * @exception       -
 * @see             
*/
LRESULT CPouVarSelCtrl::OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int curSel = TabCtrl_GetCurSel(m_ctlSysTabControl32.m_hWnd);
    ChangeSelectedTab(curSel);
    return 0;
}



//------------------------------------------------------------------*
/**
 * ChangeSelectedTab activates one of the two dialogs in the tab control..
 *
 * @param           sel: which dialog should be activated: 
 *                          PVS_DLG_POU_SEL, PVS_DLG_VAR_SEL
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelCtrl::ChangeSelectedTab(int sel)
{
    if (! ( (m_ctlSysTabControl32.m_hWnd != NULL)
         && (m_pouSelDlg.m_hWnd != NULL && ::IsWindow(m_pouSelDlg.m_hWnd))
         && (m_varSelDlg.m_hWnd != NULL && ::IsWindow(m_varSelDlg.m_hWnd)) ) )
    {
        return;
    }

    switch(sel)
    {
        case PVS_DLG_POU_SEL:
            m_varSelDlg.ShowWindow(SW_HIDE);
            m_pouSelDlg.ShowWindow(SW_SHOW);
            m_pouSelDlg.SetFocus();
            break;
        case PVS_DLG_VAR_SEL:
            m_pouSelDlg.ShowWindow(SW_HIDE);
			// 10.11.04 SIS >>
			// trigger loading new data
			m_varSelDlg.InvalidateDlgData();
			// 10.11.04 SIS <<
            m_varSelDlg.ShowWindow(SW_SHOW);
            m_varSelDlg.SetFocus();
            break;
    }

}



//------------------------------------------------------------------*
/**
 * IOleInPlaceObject_UIDeactivate.
 *
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CPouVarSelCtrl::IOleInPlaceObject_UIDeactivate()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    
    return CComControlBase::IOleInPlaceObject_UIDeactivate();
}


LRESULT CPouVarSelCtrl::OnSetFocusTabControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_pouSelDlg.IsWindowVisible ())
        m_pouSelDlg.SetFocus ();
    else if (m_varSelDlg.IsWindowVisible ())
        m_varSelDlg.SetFocus ();
    return 0;
}


//------------------------------------------------------------------*
/**
 * ShowPouSelection shows the pou selection dialog.
 *
 * @return          
 * @exception       -
 * @see             
*/
STDMETHODIMP CPouVarSelCtrl::ShowPouSelection()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TabCtrl_SetCurSel(m_ctlSysTabControl32.m_hWnd, PVS_DLG_POU_SEL);
    ChangeSelectedTab(PVS_DLG_POU_SEL);

	return S_OK;
}



//------------------------------------------------------------------*
/**
 * ShowVarSelection shows the variable selection dialog.
 *
 * @param           pouName: the name of the active pou.
 * @return          
 * @exception       -
 * @see             
*/
STDMETHODIMP CPouVarSelCtrl::ShowVarSelection(BSTR sPouName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TabCtrl_SetCurSel(m_ctlSysTabControl32.m_hWnd, PVS_DLG_VAR_SEL);
    ChangeSelectedTab(PVS_DLG_VAR_SEL);

    //sPouName may be "MyProgram.st|aVarName"
    //       or also  "MyProgram.st|aStruct.aArray[i]"   
    for(const wchar_t* psz = sPouName; *psz && *psz!='|'; psz++)
        ;

    CComBSTR sPouName2(psz - sPouName, sPouName);

    if(*psz=='|')
        psz++;
    
    //psz points now to aVarName or aStruct.aArray[i] or if not '|' to 
    //to the terminating 0-char.
    //Since we cannot handle any complex selection like "aStruct.aArray[i]"
    //we get the starting identifier part of any complex selection:
    for(const wchar_t* psz2 = psz; __iscsym(*psz2); psz2++)
        ;

    CComBSTR sVarNamePrefix(psz2-psz, psz);
    CComBSTR sDataType(_T(""));
    SetVarSelectionHint(sPouName2,  sVarNamePrefix, sDataType);
    
	return S_OK;
}



//------------------------------------------------------------------*
/**
 * InitPouVarSelection inits the dlg data and the two dlg pages.
 *
 * @return          
 * @exception       -
 * @see             
*/
STDMETHODIMP CPouVarSelCtrl::InitPouVarSelection()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    m_pouSelDlg.InitDlg(this);
    m_varSelDlg.InitDlg(this);
    m_pouVarData.Init(this);

    m_pouVarData.FinishUpdate();
    m_varSelDlg.FinishInit();

	return S_OK;
}


//------------------------------------------------------------------*
/**
 * SetVarSelectionHint propagates the hint for the variable selection
 * given by an editor the the dialog page.
 *
 * @param           sPouName: the name of the active pou
 * @param           sVarNamePrefix: the beginning of the variable name
 * @param           sDataType: the possible data type of the variable, 
 *                             if known
 * @return           
 * @exception        -
 * @see              
 */
STDMETHODIMP CPouVarSelCtrl::SetVarSelectionHint(/*[in, string]*/ BSTR sPouName,
                                                 /*[in, string]*/ BSTR sVarNamePrefix,
                                                 /*[in, string]*/ BSTR sDataType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

TAKE_TIME_INIT;
TAKE_TIME_START;

    if (m_pouVarData.GetActivePouFileName().CompareNoCase((CString)sPouName)!=0)
    {
        m_pouVarData.UpdateLocalInstances((CString)sPouName);
		// 10.11.04 SIS >>
		// call invalidate dialog data only if visible
		if(m_varSelDlg.IsWindowVisible())
		{
			m_varSelDlg.InvalidateDlgData();
		}
    }

    if (!((CString)sVarNamePrefix).IsEmpty())
    {
        m_varSelDlg.ChangeVarSelectionHint((CString)sPouName, 
                                           (CString)sVarNamePrefix, 
                                           (CString)sDataType);
    }

TAKE_TIME_END(_T("PouVarSelCtrl, SetVarSelectionHint: "));

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * LookupUserHelp finds the user help file for a given pou.
 *
 * @param            sPou: the class type of the fb, fun or struct
 *                         or the object identifier and the pou name 
 *                         where the object has been found separated with '|'
 * @param            psFileName: the user help file if found
 * @return           
 * @exception        -
 * @see              
*/
STDMETHODIMP CPouVarSelCtrl::LookupUserHelp(/*[in, string]*/ BSTR sPou,
                                            /*[out, string]*/ BSTR* psFileName)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strFileName;
    CString strPou = sPou;
    CString strParentName;
    CString strParentFileName;

    if(psFileName)
        *psFileName = NULL;
    else
        return E_INVALIDARG;

    int pos;
    pos = strPou.Find(_T("|"));
    if (pos!=-1)
    {
        strParentName = strPou.Mid(pos+1);
        strParentFileName = strParentName;
        int i = strParentName.Find(_T('.'));
        if (i>0)
        {
            strParentName = strParentName.Left(i);
        }
        strPou = strPou.Left(pos);
    }

    // parent file given ?
    // check if it is a local instance
    if (!strParentName.IsEmpty())
    {
        // update local data
        m_pouVarData.UpdateLocalInstances(strParentFileName);
        CPouVarObject* pPouVar = NULL;
        pPouVar = m_pouVarData.FindLocalInstance(strPou);
        if (pPouVar!=NULL)
        {
            // should be a var object
            CVarObject* pVarObj = dynamic_cast<CVarObject*>(pPouVar);
            if (pVarObj)
            {
                strPou = pVarObj->m_strDataType;
                strParentName = _T("");
            }
        }
    }

    // check if global instance
    if (!strParentName.IsEmpty())
    {
        CPouVarObject* pPouVar = NULL;
        pPouVar = m_pouVarData.FindGlobalInstance(strPou);
        if (pPouVar!=NULL)
        {
            // should be a var object
            CVarObject* pVarObj = dynamic_cast<CVarObject*>(pPouVar);
            if (pVarObj)
            {
                strPou = pVarObj->m_strDataType;
            }
        }
    }

    // search in local pous and libraries
    CPouVarObject* pPouVarObj = NULL;
    pPouVarObj = m_pouVarData.FindPou(strPou);

    if (pPouVarObj!=NULL)
    {
        CPouObject* pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
        if (pPouObj)
        {
            strFileName = pPouObj->m_strUserHelp;
            if (strFileName.IsEmpty())
            {
                if(pPouVarObj->m_strProjectFile.IsEmpty())
                    return S_FALSE;
                
                CString strPrjFile; 
                strPrjFile = _T("ATCM:") + pPouVarObj->m_strProjectFile;
                *psFileName = strPrjFile.AllocSysString();
                return S_OK;
            }
            else
            {
                if (!pPouObj->m_strOrigin.IsEmpty())
                {
                    m_pouVarData.CheckOutUserHelp(pPouVarObj);

                    // insert full path info between 4Cfile:<filename>
                    CString strPath;
                    strPath = pPouVarObj->m_strOrigin;
                    int i = strPath.ReverseFind(_T('.'));
                    if (i>0)
                    {
                        strPath = strPath.Left(i);
                    }

                    CString strUrlPrefix;
                    CString strUrl;

                    i = strFileName.Find(_T(":"));
                    if (i>=0)
                    {
                        strUrlPrefix = strFileName.Left(i+1);
                        strUrl = strFileName.Mid(i+1);
                    }
                    else
                    {
                        strUrlPrefix = _T("");
                        strUrl = strFileName;
                    }

                    strFileName = strUrlPrefix + strPath + _T("\\") +strUrl;
                }

                *psFileName = strFileName.AllocSysString();
                return S_OK;
            }
        }
    }

    return S_FALSE;
}



//------------------------------------------------------------------*
/**
 * LookupHeader returns the header of a given pou given in a library.
 *
 * @param            sPou: full qualified library access path:
 *                         <4cl-Path>::<Header-Path>
 *                         with <4cl-Path>:    full path of 4cl-File
 *                              <Header-Path>: path of header file within library
 * @param            psHeaderFile: The extracted header file path name of the pou,
 *                                 if found.
 * @return           
 * @exception        -
 * @see              
 */
STDMETHODIMP CPouVarSelCtrl::LookupHeader(/*[in, string]*/ BSTR sPou,
                                          /*[out, string]*/ BSTR* psHeaderFile)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strPou = sPou;
    CString strLibraryPath;
    CString strHeaderPath;
    CString strHeaderFilePath;

    // first syntax of input sPou:  <library name with or without path>::<path in the library>
    int pos;
    pos = strPou.Find(_T("::"));
    if (pos != -1)
    {
        strLibraryPath = strPou.Left(pos);
        strHeaderPath = strPou.Mid(pos + 2);

        if(m_pouVarData.CheckOutHeader(strLibraryPath, strHeaderPath, strHeaderFilePath))
        {
            *psHeaderFile = strHeaderFilePath.AllocSysString();
            return S_OK;
        }

        return E_FAIL;
    }

    // second way: <class_type> or <object_name or class_type>|<file name where identifier has been found>
    CString strFileName;
    CString strParentName;
    CString strParentFileName;
    
    // try to find data type, 
    // if identifier is no type, the type has been found in the local context
    pos = strPou.Find(_T("|"));
    if (pos!=-1)
    {
        strParentName = strPou.Mid(pos+1);
        strParentFileName = strParentName;
        int i = strParentName.Find(_T('.'));
        if (i>0)
        {
            strParentName = strParentName.Left(i);
        }
        strPou = strPou.Left(pos);
    }


    // parent file given ?
    // check if it is a local instance
    if (!strParentName.IsEmpty())
    {
        // update local data
        m_pouVarData.UpdateLocalInstances(strParentFileName);
        CPouVarObject* pPouVar = NULL;
        pPouVar = m_pouVarData.FindLocalInstance(strPou);
        if (pPouVar!=NULL)
        {
            // should be a var object
            CVarObject* pVarObj = dynamic_cast<CVarObject*>(pPouVar);
            if (pVarObj)
            {
                strPou = pVarObj->m_strDataType;
                strParentName = _T("");
            }
        }
    }

    // check if global instance
    if (!strParentName.IsEmpty())
    {
        CPouVarObject* pPouVar = NULL;
        pPouVar = m_pouVarData.FindGlobalInstance(strPou);
        if (pPouVar!=NULL)
        {
            // should be a var object
            CVarObject* pVarObj = dynamic_cast<CVarObject*>(pPouVar);
            if (pVarObj)
            {
                strPou = pVarObj->m_strDataType;
            }
        }
    }

    // strPou should contain a type now,
    // so search for information for this type

    // search in local pous and libraries
    CPouVarObject* pPouVarObj = NULL;
    pPouVarObj = m_pouVarData.FindPou(strPou);

    if (pPouVarObj!=NULL)
    {
        CPouObject* pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
        if (pPouObj)
        {
            strFileName = pPouObj->m_strHeaderFile;
            if (!strFileName.IsEmpty())
            {
                if (!pPouObj->m_strOrigin.IsEmpty())
                {
                    strFileName = m_pouVarData.CheckOutHeader(pPouVarObj);
                }

                *psHeaderFile = strFileName.AllocSysString();
                return S_OK;
            }
        }
    }

    strFileName = m_pouVarData.FindProgramFile(strPou);
    if (!strFileName.IsEmpty())
    {
        *psHeaderFile = strFileName.AllocSysString();
        return S_OK;
    }

    return S_FALSE;

}





STDMETHODIMP CPouVarSelCtrl::GetLRUPous(/*[out]*/ SAFEARRAY** parrPous)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CList<CString, CString&>* pLruList = NULL;

    pLruList = m_pouSelDlg.GetLruList();
    ASSERT(pLruList);
    if (!pLruList)
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = pLruList->GetCount();

    ASSERT(parrPous);

    *parrPous = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*parrPous)) 
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    POSITION pos = pLruList->GetHeadPosition();
    while(pos)
    {
        CString str = pLruList->GetNext(pos);
        CComBSTR sStr(str);
        hr = ::SafeArrayPutElement(*parrPous, &lIndex, 
                                    (void*)sStr);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*parrPous);
            return E_FAIL;
        }
        lIndex++;
    }

    return hr;
}

STDMETHODIMP CPouVarSelCtrl::GetPouXML(/*[in, string]*/  BSTR szPouName, 
                                       /*[out, string]*/ BSTR* pszPouDef)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str = szPouName;
    CString strXML;
 
    CPouVarObject* pPouVarObj = NULL;
    pPouVarObj = m_pouVarData.FindPou(str);

    if (!pPouVarObj)
    {
        return S_FALSE;
    }

    strXML = pPouVarObj->GetDragAndDropString();

    *pszPouDef = strXML.AllocSysString();

    return S_OK;       
}

STDMETHODIMP CPouVarSelCtrl::GetVarList(/*[out]*/ SAFEARRAY** parrVars)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CList<CString, CString&> listVars;

    m_varSelDlg.GetVarList(listVars, _T(""));

    if (listVars.IsEmpty())
    {
        return S_FALSE;
    }

    HRESULT hr = S_OK;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = listVars.GetCount();

    *parrVars = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*parrVars)) 
    {
        return E_FAIL;    
    }

    POSITION pos = listVars.GetHeadPosition();
    long lIndex = 0;
    while(pos)
    {
        CString str = listVars.GetNext(pos);
        CComBSTR sStr(str);
        hr = ::SafeArrayPutElement(*parrVars, &lIndex, 
                                    (void*)sStr);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*parrVars);
            return E_FAIL;
        }
        lIndex++;
    }

    return hr;
}

STDMETHODIMP CPouVarSelCtrl::GetPouList(/*[out]*/ SAFEARRAY** parrPous)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CList<CString, CString&> listPous;

    m_pouSelDlg.GetPouList(listPous);

    if (listPous.IsEmpty())
    {
        return S_FALSE;
    }

    HRESULT hr = S_OK;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = listPous.GetCount();

    *parrPous = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*parrPous)) 
    {
        return E_FAIL;    
    }

    POSITION pos = listPous.GetHeadPosition();
    long lIndex = 0;
    while(pos)
    {
        CString str = listPous.GetNext(pos);
        CComBSTR sStr(str);
        hr = ::SafeArrayPutElement(*parrPous, &lIndex, 
                                    (void*)sStr);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*parrPous);
            return E_FAIL;
        }
        lIndex++;
    }

    return hr;
}

STDMETHODIMP CPouVarSelCtrl::GetPouDefinition(/*[in, string]*/  BSTR szPouName, 
                                              /*[out, string]*/ BSTR* pszPouDef)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str = szPouName;
    CString strDef;
 
    CPouVarObject* pPouVarObj = NULL;
    pPouVarObj = m_pouVarData.FindPou(str);

    if (!pPouVarObj)
    {
        return S_FALSE;
    }

    CPouObject* pPouObj = NULL;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);

    if (!pPouObj)
    {
        return S_FALSE;
    }

    strDef = pPouObj->GetDefinition();
    if (strDef.IsEmpty())
    {
        return S_FALSE;
    }

    *pszPouDef = strDef.AllocSysString();

    return S_OK;       
}

STDMETHODIMP CPouVarSelCtrl::GetPouMember(/*[in, string]*/ BSTR szPouName, 
                                          /*[out]*/ SAFEARRAY** parrMembers)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str = szPouName;
    CList<CString, CString&> listMembers;

    // partitioning of the hint text (abc.def.xyz)
    // abc has to be a local or global variable
    // it has to be a complex data type, so get all members
    // if there are more .-separated parts in the hint text, take the
    // next part and search for that member, find the datatype of that member
    // this data type has to be a complex data type again, so get all members
    // of this data type
    // and so on
    CString strFirst = str;
    CString strRest;
    
    int ipos = strFirst.Find(_T("."), 0);
    if (ipos>=0)
    {
        if (strFirst.GetLength()>ipos+1)
        {
            strRest = strFirst.Mid(ipos+1);
        }
        strFirst = strFirst.Left(ipos);
    }

    // lookup local or global variable
    CPouVarObject* pVarObj = NULL;
    pVarObj = m_pouVarData.FindLocalInstance(strFirst);

    if (!pVarObj)
    {
        pVarObj = m_pouVarData.FindGlobalInstance(strFirst);
    }

    if (!pVarObj)
    {
        return S_FALSE;
    }

    CString strDataType;
    CVarObject* pLocVar = NULL;
    pLocVar = dynamic_cast<CVarObject*>(pVarObj);

    if (!pLocVar)
    {
        return S_FALSE;
    }

    if (!pLocVar->m_bIsFBorStructDataType)
    {
        return S_FALSE;
    }

    strDataType = pLocVar->m_strDataType;

    CPouVarObject* pPouVarObj = NULL;
    pPouVarObj = m_pouVarData.FindPou(strDataType);

    if (!pPouVarObj)
    {
        return S_FALSE;
    }

    CPouObject* pPouObj = NULL;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);

    if (!pPouObj)
    {
        return S_FALSE;
    }


    // is there more to do
    while(!strRest.IsEmpty())
    {
        // get next part
        ipos = strRest.Find(_T("."), 0);
        if(ipos>0)
        {
            strFirst = strRest.Left(ipos);
            if (strRest.GetLength()>ipos+1)
            {
                strRest = strRest.Mid(ipos+1);
            }
            else
            {
                strRest.Empty();
            }
        }
        else
        {
            strFirst = strRest;
            strRest.Empty();
        }

        // find member with name strFirst
        POSITION posMember = pPouObj->m_listVars.GetHeadPosition();
        BOOL bFound = FALSE;
        CVarObject* pVarMember = NULL;
        while (posMember)
        {
            pVarMember = pPouObj->m_listVars.GetNext(posMember);
            if (!pVarMember)
            {
                continue;
            }
            if (pVarMember->m_strName.CompareNoCase(strFirst)==0)
            {
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            return S_FALSE;
        }

        if (!pVarMember->m_bIsFBorStructDataType)
        {
            return S_FALSE;
        }

        strDataType = pVarMember->m_strDataType;

        pPouVarObj = NULL;
        pPouVarObj = m_pouVarData.FindPou(strDataType);

        if (!pPouVarObj)
        {
            return S_FALSE;
        }

        pPouObj = NULL;
        pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);

        if (!pPouObj)
        {
            return S_FALSE;
        }
    }


    if (!pPouObj->GetMembers(listMembers))
    {
        return S_FALSE;
    }

    if (listMembers.IsEmpty())
    {
        return S_FALSE;
    }

    HRESULT hr = S_OK;
    SAFEARRAYBOUND Bound;
    Bound.lLbound = 0;
    Bound.cElements = listMembers.GetCount();

    *parrMembers = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*parrMembers)) 
    {
        return E_FAIL;    
    }

    long lIndex = 0;
    POSITION pos = listMembers.GetHeadPosition();
    while(pos)
    {
        CString str = listMembers.GetNext(pos);
        CComBSTR sStr(str);
        hr = ::SafeArrayPutElement(*parrMembers, &lIndex, 
                                    (void*)sStr);
        if (FAILED(hr))
        {
            ::SafeArrayDestroy(*parrMembers);
            return E_FAIL;
        }
        lIndex++;
    }

    return hr;
}


STDMETHODIMP CPouVarSelCtrl::ShowVarSelectionBox(/*[in]*/LONG lXPos, /*[in]*/LONG lYPos,
                                                 /*[in, string]*/BSTR szLine, /*[in]*/LONG lColumn,
                                                 /*[in, string]*/BSTR szTypeHint,
                                                 /*[in, string]*/BSTR szPouName,
                                                 /*[out, string]*/BSTR* pszSelection,
                                                 /*[out]*/LONG* plStartColumn,
                                                 /*[out]*/LONG* plEndColumn)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CVarSelectionBox dlgVarSel;
    CString strSelection = _T("");

	m_varSelDlg.InvalidateDlgData();

    dlgVarSel.Init(this, lXPos, lYPos, (CString)szLine, lColumn, (CString)szTypeHint, (CString)szPouName);
    if (dlgVarSel.DoModal()==IDOK)
    {
        strSelection = dlgVarSel.GetSelection(*plStartColumn, *plEndColumn);
        *pszSelection = strSelection.AllocSysString();
        return S_OK;
    }

    return S_FALSE;
}


STDMETHODIMP CPouVarSelCtrl::DisablePouVarSelection(/*[in]*/BOOL bDisable)
{
   	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    m_bDisabled = bDisable;

    return S_OK;
}


