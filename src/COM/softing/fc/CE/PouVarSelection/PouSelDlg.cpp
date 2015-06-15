



#include "stdafx.h"
#include "resource.h"
#include "PouSelDlg.h"
#include "DragItem.h"


#include "PouVarSelCtrl.h"
#include "PouVarSelData.h"

#include "DragDropHelper\PouVarDefs.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPouSelDlg::CPouSelDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pPouVarSelCtrl = NULL;
    m_pPouVarSelData = NULL;

	m_pStaticLib    = new CStatic();
	m_pLBLib	    = new CListCtrl();
	m_pStaticTypes	= new CStatic();
	m_pLBTypes	    = new CListCtrl();
	m_pStaticLru	= new CStatic();
	m_pLBCLru       = new CListCtrl();
	m_pStaticPou	= new CStatic();
	m_pEditPou	    = new CEdit();
	m_pLBCPou	    = new CListCtrl();
	m_pStaticHeader	= new CStatic();
	m_pEditHeader	= new CEdit();
    m_pBtnApply     = new CButton();

    m_iLastSelected = LAST_SEL_NOTHING;

    m_uiClipFormatXML = RegisterClipboardFormat(CB_FORMAT_4CXML);

    m_bEditChanging = FALSE;

    m_strPVSSelAll.LoadString(IDS_PVS_SEL_ALL);
    m_strPVSSelProject.LoadString(IDS_PVS_SEL_PROJECT);
    m_strPVSSelHeader.LoadString(IDS_PVS_SEL_HEADER);
    m_strPVSSelGlobal.LoadString(IDS_PVS_SEL_GLOBAL);
    m_strPVSSelConsts.LoadString(IDS_PVS_SEL_CONSTS);
    m_strPVSSelComplex.LoadString(IDS_PVS_SEL_COMPLEX);
    m_strPVSSelSimple.LoadString(IDS_PVS_SEL_SIMPLE);
    m_strPVSSelNone.LoadString(IDS_PVS_SEL_NONE);
}

CPouSelDlg::~CPouSelDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pStaticLib->Detach();
    delete m_pStaticLib;

	m_pLBLib->Detach();
    delete m_pLBLib;

	m_pStaticTypes->Detach();	
    delete m_pStaticTypes;

	m_pLBTypes->Detach();
    delete m_pLBTypes;

    m_pStaticLru->Detach();
    delete m_pStaticLru;

	m_pLBCLru->Detach();
    delete m_pLBCLru;

	m_pStaticPou->Detach();
    delete m_pStaticPou;

	m_pEditPou->Detach();
    delete m_pEditPou;

	m_pLBCPou->Detach();
    delete m_pLBCPou;

	m_pStaticHeader->Detach();
    delete m_pStaticHeader;

	m_pEditHeader->Detach();
    delete m_pEditHeader;

    m_pBtnApply->Detach();
    delete m_pBtnApply;
}



LRESULT CPouSelDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_hWnd != NULL)
    {
        HWND hWndDlgItem;

        hWndDlgItem = GetDlgItem(IDC_POU_LIB_STATIC);
        m_pStaticLib->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_LIB_LIST);
        m_pLBLib->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_TYPES_STATIC);
        m_pStaticTypes->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_TYPES_LIST);
        m_pLBTypes->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_LRU_STATIC);
        m_pStaticLru->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_LRU_LISTCTRL);
        m_pLBCLru->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_POU_STATIC);
        m_pStaticPou->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_POU_EDIT);
        m_pEditPou->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_POU_LISTCTRL);
        m_pLBCPou->Attach(hWndDlgItem);
       
        hWndDlgItem = GetDlgItem(IDC_POU_HEADER_STATIC);
        m_pStaticHeader->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_HEADER_EDIT);
        m_pEditHeader->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_POU_APPLY_BTN);
        m_pBtnApply->Attach(hWndDlgItem);
    }

    int iSize = sizeof(m_sysMetrics);
    ZeroMemory(&m_sysMetrics, iSize);
    m_sysMetrics.cbSize = iSize;
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, iSize, &m_sysMetrics, 0);

	return 0;
}



LRESULT CPouSelDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    RECT r;
    r.top = r.left = 0;
    r.right  = LOWORD(lParam); // width of client area
    r.bottom = HIWORD(lParam); // height of client area

    LONG dbu = GetDialogBaseUnits();
    WORD dbuh = LOWORD(dbu);
    WORD dbuv = HIWORD(dbu);

    const int b = m_sysMetrics.iBorderWidth; // dialog border
    const int d1 = 2; // pad static fields -> control
    const int d2 = 4; // pad control -> control
    const int hStatic = dbuv; // height of a static field
    const int hEdit = m_sysMetrics.iCaptionHeight;  // height of an edit field
    const int hButton = m_sysMetrics.iCaptionHeight + 2*d1;  // height of a button
    const int minLB = dbuv*2; // minimum height of the list boxes
   
    const int iParts = 4; // how many parts the upper part of the dialog against the header
    const int iPartsU = 3; // upper part

    int hUpper = r.bottom - 2*b;  // height of the upper part of the dialog
    BOOL bShowLower = FALSE;
    
    if ( (3*(hStatic + d1 + minLB + d2)) < ((r.bottom * iPartsU) / iParts) )
    {
        hUpper = (hUpper*iPartsU) / iParts; 
        bShowLower = TRUE;
    }

    // left side of the dialog

    int x = b;
    int width = r.right/2 - b - d2;
    int y, height;

    int hSL;   // height of one Static LB in upper left side of the dlg
    hSL = hUpper / 3;
    int hList;   // height of one listbox
    hList = hSL - hStatic - d1 - d2;
    
    // libraries:
    y = b;
    height = hStatic;
    m_pStaticLib->MoveWindow(x,y,width,height);

    y += hStatic + d1;
    height = hList;
    m_pLBLib->MoveWindow(x,y,width,height);

    // types:
    y += hList + d2;
    height = hStatic;
    m_pStaticTypes->MoveWindow(x,y,width,height);

    y += hStatic + d1;
    height = hList;
    m_pLBTypes->MoveWindow(x,y,width,height);

    // LRUs
    y += hList + d2;
    height = hStatic;
    m_pStaticLru->MoveWindow(x,y,width,height);
    
    y += hStatic + d1;
    height = hList;
    m_pLBCLru->MoveWindow(x,y,width,height);

    m_pLBCLru->SetColumnWidth(0, width);

    // right side
    int hPouList = hUpper - hStatic - hEdit - hButton - 2*d1 - 2*d2;
    if (hPouList<0)
    {
        hPouList = 0;
    }
    
    x = r.right/2 + d2;
    
    y = b;
    height = hStatic;
    m_pStaticPou->MoveWindow(x,y,width,height);

    y += hStatic + d1;
    height = hEdit;
    m_pEditPou->MoveWindow(x,y,width,height);

    y += hEdit + d1;
    height = hPouList;
    m_pLBCPou->MoveWindow(x,y,width,height);

    m_pLBCPou->SetColumnWidth(0, width);

    y += hPouList + d2;
    height = hButton;
    CString strBtnText;
    m_pBtnApply->GetWindowText(strBtnText);
    int iBtnWidth = strBtnText.GetLength() * dbuh + 2*d2;
    if (iBtnWidth<width)
    {
        x += (width-iBtnWidth) / 2;
        width = iBtnWidth;
    }
    
    m_pBtnApply->MoveWindow(x,y,width,height);
   


    // Lower part of the dialog
    if (bShowLower)
    {
        x = b;
        y = hUpper + d2;
        width = r.right - 2*b;
        int hHeaderEdit = r.bottom - hUpper - 2*b -hStatic - d2 - d1;

        height = hStatic;
        m_pStaticHeader->MoveWindow(x,y,width,height);

        y += hStatic + d1;
        height = hHeaderEdit;
        m_pEditHeader->MoveWindow(x,y,width,height);

    }

    return 0;
}






void CPouSelDlg::InitDlg(CPouVarSelCtrl* pPouVarSelCtrl)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pPouVarSelCtrl = pPouVarSelCtrl;
    ASSERT(m_pPouVarSelCtrl);
    if (m_pPouVarSelCtrl)
    {
        m_pPouVarSelData = m_pPouVarSelCtrl->GetPouVarSelData();
        ASSERT(m_pPouVarSelData);
    }

    m_listPouSelectedLibraries.AddTail(m_strPVSSelAll);
    m_listPouSelectedTypes.AddTail(m_strPVSSelAll);
}


void CPouSelDlg::InvalidateDlgData()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ASSERT(m_pPouVarSelData);
    if (!m_pPouVarSelData)
    {
        return;
    }

    CMap<CString, LPCTSTR, CPouVarObject*, CPouVarObject*> mapAllPous;

    POSITION pos = NULL;
    BOOL bInPouList = FALSE;

    // update data
    m_mapPouPous.RemoveAll();
     // select objects from selected libraries where type = selected type
    pos = m_pPouVarSelData->m_listLibraries.GetHeadPosition();
    while(pos)
    {
        CLibraryContent* pLibCont = m_pPouVarSelData->m_listLibraries.GetNext(pos);
        ASSERT(pLibCont);
        CString strLibName = pLibCont->strLibName;
        
        if (m_listPouSelectedLibraries.Find(strLibName)
            || m_listPouSelectedLibraries.Find(m_strPVSSelAll))
        {
            bInPouList = TRUE;
        }
        else
        {
            bInPouList = FALSE;
        }

        // look at all pous: select from type
        POSITION pouPos;
        pouPos = (pLibCont->pouList).GetHeadPosition();
        while(pouPos)
        {
            CPouVarObject* pPouVarObj = NULL;
            pPouVarObj = (pLibCont->pouList).GetNext(pouPos);
            ASSERT(pPouVarObj);

            CString strPouName;
            strPouName = pPouVarObj->m_strName;

            mapAllPous.SetAt(strPouName, pPouVarObj);

            // test for type and library if it is in pou list
            if (bInPouList)
            {
                CString strType = pPouVarObj->m_strNativeType;
                if (m_listPouSelectedTypes.Find(strType)
                    || (m_listPouSelectedTypes.Find(m_strPVSSelAll)
                        && m_listPouTypes.Find(strType)))
                {
                    m_mapPouPous.SetAt(strPouName, pPouVarObj);
                }
            }
        }
        
    }
    // from project
    if (m_listPouSelectedLibraries.Find(m_strPVSSelAll)
        ||m_listPouSelectedLibraries.Find(m_strPVSSelProject))
    {
        bInPouList = TRUE;
    }
    else
    { 
        bInPouList = FALSE;
    }

    POSITION projPouPos;
    projPouPos = m_pPouVarSelData->m_listProjectPous.GetHeadPosition();
    while(projPouPos)
    {
        CPouVarObject* pPouVarObj = NULL;
        pPouVarObj = m_pPouVarSelData->m_listProjectPous.GetNext(projPouPos);
        ASSERT(pPouVarObj);
        
        CString strPouName;
        strPouName = pPouVarObj->m_strName;

        mapAllPous.SetAt(strPouName, pPouVarObj);

        if (bInPouList)
        {
            CString strType = pPouVarObj->m_strNativeType;
            if (m_listPouSelectedTypes.Find(strType)
                || (m_listPouSelectedTypes.Find(m_strPVSSelAll)
                    && m_listPouTypes.Find(strType)))
            {   
                m_mapPouPous.SetAt(strPouName, pPouVarObj);
            }
        }
    }

    
    {
        POSITION projConstPos;
        projConstPos = m_pPouVarSelData->m_listProjectConstants.GetHeadPosition();
        while(projConstPos)
        {
            CPouVarObject* pPouVarObj = NULL;
            pPouVarObj = m_pPouVarSelData->m_listProjectConstants.GetNext(projConstPos);
            ASSERT(pPouVarObj);
            CString strPouName;
            strPouName = pPouVarObj->m_strName;

            mapAllPous.SetAt(strPouName, pPouVarObj);

            if (bInPouList)
            {
                if (m_listPouSelectedTypes.Find((CString)PVS_XML_CONST)
                    || m_listPouSelectedTypes.Find(m_strPVSSelAll))
                {
                    m_mapPouPous.SetAt(strPouName, pPouVarObj);
                }
            }
        }
    }
   
    
    // update list box
    m_pLBCPou->DeleteAllItems();

    int iItem=0;
    pos = m_mapPouPous.GetStartPosition();
    while(pos)
    {
        CString strPouName;
        CPouVarObject* pPouVarObj;
        m_mapPouPous.GetNextAssoc(pos, strPouName, pPouVarObj);
        m_pLBCPou->InsertItem(iItem, strPouName);
        iItem++;
    }

    // update selection
    if (m_iLastSelected == LAST_SEL_POU)
    {
        LVFINDINFO findInfo;
        findInfo.flags = LVFI_STRING;
        findInfo.psz = m_strPouSelectedPou;

        int sel = m_pLBCPou->FindItem(&findInfo);
        if (sel>=0)
        {
            m_pLBCPou->SetItemState(sel,  LVIS_SELECTED, LVIS_SELECTED);
        }
    }
   
    // lru list 
    // TODO: optimize!!!
    m_mapPouLru.RemoveAll();

    pos = m_listPouLru.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CString strPouName;
        strPouName = m_listPouLru.GetNext(pos);

        CPouVarObject* pPouVarObj = NULL;
        if (mapAllPous.Lookup(strPouName, pPouVarObj))
        {
            ASSERT(pPouVarObj);
            m_mapPouLru.SetAt(strPouName, pPouVarObj);
        }
        else
        {
            m_listPouLru.RemoveAt(thisPos);
        }
    }

    // update list ctrl:
    m_pLBCLru->DeleteAllItems();


    iItem = 0;
    pos = m_listPouLru.GetHeadPosition();
    while(pos)
    {
        CString strPouName;
        strPouName = m_listPouLru.GetNext(pos);
        m_pLBCLru->InsertItem(iItem, strPouName);
        iItem++;
    }

    // update selection
    if (m_iLastSelected == LAST_SEL_LRU)
    {
        LVFINDINFO findInfo;
        findInfo.flags = LVFI_STRING;
        findInfo.psz = m_strPouSelectedPou;

        int sel = m_pLBCLru->FindItem(&findInfo);
        if (sel>=0)
        {
            m_pLBCLru->SetItemState(sel,  LVIS_SELECTED, LVIS_SELECTED);
        }
    }
}


void CPouSelDlg::InvalidateSelectionData()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ASSERT(m_pPouVarSelData);
    if (!m_pPouVarSelData)
    {
        return;
    }

    POSITION pos = NULL;

    ////////////////////////
    // Libraries list box

    // fill data list
    m_listPouLibraries.RemoveAll();
    m_listPouLibraries.AddTail(m_strPVSSelAll);
    m_listPouLibraries.AddTail(m_strPVSSelProject);
    pos = m_pPouVarSelData->m_listLibraries.GetHeadPosition();
    while(pos)
    {
        CLibraryContent* pLibCont = m_pPouVarSelData->m_listLibraries.GetNext(pos);
        ASSERT(pLibCont);
        if (!pLibCont)
        {
            continue;
        }
        m_listPouLibraries.AddTail(pLibCont->strLibName);
    }
    pos = m_listPouSelectedLibraries.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CString strLibName = m_listPouSelectedLibraries.GetNext(pos);
        if (!m_listPouLibraries.Find(strLibName))
        {
            m_listPouSelectedLibraries.RemoveAt(thisPos);
        }
    }
    
    // fill list box
    m_pLBLib->DeleteAllItems();
    pos = m_listPouLibraries.GetHeadPosition();
    while(pos)
    {
        CString strLibName;
        strLibName = m_listPouLibraries.GetNext(pos);
        m_pLBLib->InsertItem(m_pLBLib->GetItemCount(), strLibName);
    }

    // reselect entries
    pos = m_listPouSelectedLibraries.GetHeadPosition();
    while(pos)
    {
        CString strLibName;
        strLibName = m_listPouSelectedLibraries.GetNext(pos);
        LVFINDINFO  tFindInfo;
        memset(&tFindInfo, 0, sizeof(tFindInfo));
        tFindInfo.flags = LVFI_STRING;
        tFindInfo.psz = strLibName;
        int sel = m_pLBLib->FindItem(&tFindInfo);
        //int sel = m_pLBLib->FindStringExact(0, strLibName);
        if (sel>=0)
        {
            m_pLBLib->SetItemState(sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
            //m_pLBLib->SetSel(sel, TRUE);
        }
    }


    /////////////////////////
    // Types list box

    // fill data list
    m_listPouTypes.RemoveAll();
    m_listPouTypes.AddTail(m_strPVSSelAll);
    pos = m_pPouVarSelData->m_listLibClassInfos.GetHeadPosition();
    while(pos)
    {
        CClassInfo* pClassInfo = m_pPouVarSelData->m_listLibClassInfos.GetNext(pos);
        if (pClassInfo)
        {
            CString strClass = pClassInfo->strClassType;
            if (!pClassInfo->strClassText.IsEmpty())
            {
                m_listPouTypes.AddTail(strClass);
            }
        }
    }
    pos = m_listPouSelectedTypes.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CString strClass = m_listPouSelectedTypes.GetNext(pos);
        if (!m_listPouTypes.Find(strClass))
        {
            m_listPouSelectedTypes.RemoveAt(thisPos);
        }
    }
    
    // fill list box
    m_pLBTypes->DeleteAllItems();
    pos = m_listPouTypes.GetHeadPosition();
    while(pos)
    {
        CString strTypeName;
        strTypeName = m_listPouTypes.GetNext(pos);
        CString strText;
        strText = m_pPouVarSelData->GetTextForClassType(strTypeName);
        if (!strText.IsEmpty())
        {
            m_pLBTypes->InsertItem(m_pLBTypes->GetItemCount(), strText);
        }
    }
    m_pLBTypes->InsertItem(m_pLBTypes->GetItemCount(), m_strPVSSelAll);

    // reselect enties
    pos = m_listPouSelectedTypes.GetHeadPosition();
    while(pos)
    {
        CString strTypeName;
        strTypeName = m_listPouSelectedTypes.GetNext(pos);
        CString strText;
        strText = m_pPouVarSelData->GetTextForClassType(strTypeName);
        if (strText.IsEmpty())
        {
            strText = strTypeName;
        }
        LVFINDINFO  tFindInfo;
        memset(&tFindInfo, 0, sizeof(tFindInfo));
        tFindInfo.flags = LVFI_STRING;
        tFindInfo.psz = strText;
        int sel = m_pLBTypes->FindItem(&tFindInfo);
        //int sel = m_pLBTypes->FindStringExact(0, strText);
        if (sel>=0)
        {
            m_pLBTypes->SetItemState(sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
            //m_pLBTypes->SetSel(sel, TRUE);
        }
    }
    
}


void CPouSelDlg::InvalidateLRU()
{
    
    ASSERT(m_pPouVarSelData);
    if (!m_pPouVarSelData)
    {
        return;
    }

    POSITION pos = NULL;

    // Libraries list box
    m_pLBCLru->DeleteAllItems();

    int iItem = 0;
    pos = m_listPouLru.GetHeadPosition();
    while(pos)
    {
        CString strPouName;
        strPouName = m_listPouLru.GetNext(pos);
        m_pLBCLru->InsertItem(iItem, strPouName);
        iItem++;
    }
}




LRESULT CPouSelDlg::OnSelchangePouLibList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ASSERT(m_pPouVarSelData);
    if (!m_pPouVarSelData)
    {
        return 0;
    }

    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh; 
    if (pnmv->iItem<0)
    {
        return 0;
    }
    if (((pnmv->uChanged & LVIF_STATE)==0) || 
        ((pnmv->uNewState & LVIS_SELECTED)==0))
    {
        return 0;
    }

    // Libraries list box
    m_listPouSelectedLibraries.RemoveAll();

    POSITION pos = m_pLBLib->GetFirstSelectedItemPosition();
    int     nSel;
    CString strLibName;
    while(pos)
    {
        nSel = m_pLBLib->GetNextSelectedItem(pos);
        strLibName = m_pLBLib->GetItemText(nSel, 0);
        m_listPouSelectedLibraries.AddTail(strLibName);
    }

    this->InvalidateDlgData();

	return 0;
}


LRESULT CPouSelDlg::OnSelchangePouTypesList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ASSERT(m_pPouVarSelData);
    if (!m_pPouVarSelData)
    {
        return 0;
    }

    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh; 
    if (pnmv->iItem<0)
    {
        return 0;
    }
    if (((pnmv->uChanged & LVIF_STATE)==0) || 
        ((pnmv->uNewState & LVIS_SELECTED)==0))
    {
        return 0;
    }
    
    // Libraries list box
    m_listPouSelectedTypes.RemoveAll();

    POSITION pos = m_pLBTypes->GetFirstSelectedItemPosition();
    int     nSel;
    CString strClassType;
    CString strTypeName;
    while(pos)
    {
        nSel = m_pLBTypes->GetNextSelectedItem(pos);
        strTypeName = m_pLBTypes->GetItemText(nSel, 0);
        strClassType = m_pPouVarSelData->GetTypeForClassText(strTypeName);
        if (strClassType.IsEmpty())
        {
            m_listPouSelectedTypes.AddTail(strTypeName);
        }
        else
        {
            m_listPouSelectedTypes.AddTail(strClassType);
        }
    }

    this->InvalidateDlgData();

	return 0;
}



LRESULT CPouSelDlg::OnClickedPouApplyBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CListCtrl* pLBC;
    if (m_iLastSelected == LAST_SEL_LRU)
    {
        pLBC = m_pLBCLru;
    }
    else if (m_iLastSelected == LAST_SEL_POU)
    {
        pLBC = m_pLBCPou;
    }
    else
    {
        return 0;
    }

    int sel = pLBC->GetNextItem(-1, LVNI_SELECTED);
    if (sel<0)
    {
        return 0;
    }

    CString strPouName;
    CString strMangledName;
    CString strDaD;

    strPouName = pLBC->GetItemText(sel, 0);

    
    CPouVarObject* pPouVarObj;

    if (m_iLastSelected == LAST_SEL_LRU)
    {
        if (!m_mapPouLru.Lookup(strPouName, pPouVarObj))
        {
            return 0;
        }
    }
    else if (m_iLastSelected == LAST_SEL_POU)
    {
        if (!m_mapPouPous.Lookup(strPouName, pPouVarObj))
        {
            return 0;
        }
    }

    strDaD = pPouVarObj->GetDragAndDropString();
    
    CComBSTR sDaD(strDaD);

    m_pPouVarSelCtrl->Fire_InsertFBCall(sDaD);

    if (AddToLRU(strPouName))
    {
        InvalidateLRU();
    }


	return 0;
    
}


LRESULT CPouSelDlg::OnDblclkPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPNMITEMACTIVATE lpnmItem = (LPNMITEMACTIVATE) pnmh;

    int sel = lpnmItem->iItem;

    if (sel<0)
    {
        return 0;
    }

    CString strPouName;
    CString strMangledName;
    CString strDaD;

    strPouName = m_pLBCPou->GetItemText(sel, 0);
    
    CPouVarObject* pPouVarObj;

    if (!m_mapPouPous.Lookup(strPouName, pPouVarObj))
    {
        return 0;
    }

    ASSERT(pPouVarObj);
    if (!pPouVarObj)
    {
        return 0;
    }

    strDaD = pPouVarObj->GetDragAndDropString();

    CComBSTR sDaD(strDaD);

    m_pPouVarSelCtrl->Fire_InsertFBCall(sDaD);

    if (AddToLRU(strPouName))
    {
        InvalidateLRU();
    }

    m_iLastSelected = LAST_SEL_POU;
    sel = m_pLBCLru->GetNextItem(-1, LVNI_SELECTED);
    if (sel!=-1)
    {
        m_pLBCLru->SetItemState(sel,0,LVIS_SELECTED);
    }

	return 0;
}



LRESULT CPouSelDlg::OnChangeItemPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh; 
    if (pnmv->iItem<0)
    {
        return 0;
    }
    if (((pnmv->uChanged & LVIF_STATE)==0) || 
        ((pnmv->uNewState & LVIS_SELECTED)==0))
    {
        return 0;
    }
    DoSelchangePouPouListCtrl();
    return 0;
}



LRESULT CPouSelDlg::DoSelchangePouPouListCtrl()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strSel;
    int sel = m_pLBCPou->GetNextItem(-1, LVNI_SELECTED);
    if (sel==-1)
    {
        return 0;
    }

    strSel = m_pLBCPou->GetItemText(sel, 0);

    if (m_strPouSelectedPou.CompareNoCase(strSel)==0)
    {
        return 0;
    }

    m_strPouSelectedPou = strSel;
    if (!m_bEditChanging)
    {
        m_pEditPou->SetWindowText(strSel);
    }

    // get header to display
    CString strHeader = _T("");
    int iHeaderLen = 0;

    CPouVarObject* pPouVarObj;
    if (m_mapPouPous.Lookup(strSel, pPouVarObj))
    {
        strHeader = m_pPouVarSelData->GetHeader(pPouVarObj, iHeaderLen);    
    }
    
    if (iHeaderLen<=0)
    {
        iHeaderLen = strHeader.GetLength();
    }

    m_pEditHeader->SetWindowText(strHeader.Left(iHeaderLen));

    CString strTargets;

    strTargets.LoadString(IDS_HEADER_HEADER);

    CPouObject* pPouObj;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
    if (pPouObj)
    {
        POSITION tPos = pPouObj->m_listTargets.GetHeadPosition();
        CString strT;
        BOOL bFirst = TRUE;
        while(tPos)
        {
            if (!bFirst)
            {
                strT+=_T(";");
            }
            strT += pPouObj->m_listTargets.GetNext(tPos);

            bFirst = FALSE;            
        }
        if (!strT.IsEmpty())
        {
            CString strTargetsHeader;
            strTargetsHeader.LoadString(IDS_HEADER_TARGETS);
            strTargets += _T(" (") + strTargetsHeader + strT + _T(")");
        }
    }
    
    m_pStaticHeader->SetWindowText(strTargets);

    m_iLastSelected = LAST_SEL_POU;
    sel = m_pLBCLru->GetNextItem(-1, LVNI_SELECTED);
    if (sel!=-1)
    {
        m_pLBCLru->SetItemState(sel,0,LVIS_SELECTED);
    }

    return 0;
}


LRESULT CPouSelDlg::OnRclickPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pnmh;

    if (lpnmitem->iItem<0) // || (lpnmitem->uNewState & LVIS_SELECTED)==0)
    {
        bHandled = FALSE;
        return 0;
    }

    CString strPouName;
    CString strMangledName;
    CString strDaD;

    strPouName = m_pLBCPou->GetItemText(lpnmitem->iItem, 0);
    
    CPouVarObject* pPouVarObj;

    if (!m_mapPouPous.Lookup(strPouName, pPouVarObj))
    {
        return 0;
    }

    ASSERT(pPouVarObj);
    if (!pPouVarObj)
    {
        return 0;
    }

    CPouObject* pPouObj = NULL;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
    if (!pPouObj)
    {
        return 0;
    }

    m_strCMPouName = strPouName;

    CMenu menu;
    CMenu* pPopup = NULL;

    if (!menu.LoadMenu(IDR_POUSEL_MENU))
    {
        return 0;
    }

    pPopup = menu.GetSubMenu(0);
    if (!pPopup)
    {
        return 0;
    }

    if (pPouObj->m_strUserHelp.IsEmpty())
    {
        // TODO remove comments, menu only if an user help entry exists
        pPopup->EnableMenuItem(IDM_SHOW_USER_HELP, MF_BYCOMMAND | MF_GRAYED);
    }

    CPoint point = lpnmitem->ptAction;
    ::ClientToScreen(m_pLBCPou->m_hWnd, &point);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
                            point.x, point.y,
                            CWnd::FromHandle(this->m_hWnd));	

	return 1;
}






LRESULT CPouSelDlg::OnDblclkPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPNMITEMACTIVATE lpnmItem = (LPNMITEMACTIVATE) pnmh;

    int sel = lpnmItem->iItem;

    if (sel<0)
    {
        return 0;
    }

    CString strPouName;
    CString strMangledName;
    CString strDaD;

    strPouName = m_pLBCLru->GetItemText(sel, 0);
    
    CPouVarObject* pPouVarObj;

    if (!m_mapPouLru.Lookup(strPouName, pPouVarObj))
    {
        return 0;
    }

    strDaD = pPouVarObj->GetDragAndDropString();
    
    CComBSTR sDaD(strDaD);

    m_pPouVarSelCtrl->Fire_InsertFBCall(sDaD);

    if (AddToLRU(strPouName))
    {
        InvalidateLRU();
    }

    m_iLastSelected = LAST_SEL_LRU;
    sel = m_pLBCPou->GetNextItem(-1, LVNI_SELECTED);
    if (sel!=-1)
    {
        m_pLBCPou->SetItemState(sel,0,LVIS_SELECTED);
    }

	return 0;
}


LRESULT CPouSelDlg::OnChangeItemPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh; 
    if (pnmv->iItem<0)
    {
        return 0;
    }
    if (((pnmv->uChanged & LVIF_STATE)==0) || 
        ((pnmv->uNewState & LVIS_SELECTED)==0))
    {
        return 0;
    }
    DoSelchangePouLruListCtrl();
    return 0;
}


LRESULT CPouSelDlg::DoSelchangePouLruListCtrl()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strSel;
    int sel = m_pLBCLru->GetNextItem(-1, LVNI_SELECTED);

    if (sel==-1)
    {
        return 0;
    }

    strSel = m_pLBCLru->GetItemText(sel, 0);

    m_strPouSelectedLru = strSel;

    // get header to display
    CString strHeader = _T("");
    int iHeaderLen = -1;

    CPouVarObject* pPouVarObj;
    if (m_mapPouLru.Lookup(strSel, pPouVarObj))
    {
        strHeader = m_pPouVarSelData->GetHeader(pPouVarObj, iHeaderLen);        
    }

    if (iHeaderLen<=0)
    {
        iHeaderLen = strHeader.GetLength();
    }

    m_pEditHeader->SetWindowText(strHeader.Left(iHeaderLen));

    m_iLastSelected = LAST_SEL_LRU;
    sel = m_pLBCPou->GetNextItem(-1, LVNI_SELECTED);
    if (sel!=-1)
    {
        m_pLBCPou->SetItemState(sel,0,LVIS_SELECTED);
    }

    return 0;
}




LRESULT CPouSelDlg::OnChangePouPouEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strText;

    m_pEditPou->GetWindowText(strText);

    if (strText.IsEmpty())
    {
        return 0;
    }

    m_bEditChanging = TRUE;

    LVFINDINFO info;

    info.flags = LVFI_PARTIAL|LVFI_STRING;
    info.psz = strText;

    int sel = m_pLBCPou->FindItem(&info);
    if (sel>=0)
    {
        m_pLBCPou->SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
        m_pLBCPou->Update(sel);
        m_pLBCPou->EnsureVisible(sel, FALSE);

        // DoSelchangePouPouListCtrl();
    }

    m_bEditChanging = FALSE;
    
	return 0;
}


BOOL CPouSelDlg::AddToLRU(CString& strPouName)
{
    CPouVarObject* pPouObj = NULL;
    
    if (!m_mapPouPous.Lookup(strPouName, pPouObj))
    {
        return FALSE;
    }

    ASSERT(pPouObj);

    CPouVarObject* pTempPouObj = NULL;
    if (!m_mapPouLru.Lookup(strPouName, pTempPouObj))
    {
        m_mapPouLru.SetAt(strPouName, pPouObj);
    }
    else
    {
        // remove from list and insert at the beginning
        POSITION pos = m_listPouLru.Find(strPouName);
        if (pos)
        {
            m_listPouLru.RemoveAt(pos);
        }
    }

    m_listPouLru.AddHead(strPouName);

    if (m_listPouLru.GetCount()>PVS_MAX_LRU)
    {
        // remove last
        CString strRemPou = m_listPouLru.GetTail();
        m_mapPouLru.RemoveKey(strRemPou);
        m_listPouLru.RemoveTail();
    }

    return TRUE;
}


LRESULT CPouSelDlg::OnBegindragPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    LPNMLISTVIEW plvInfo = (LPNMLISTVIEW)pnmh;
    int iItem = plvInfo->iItem;

    if (iItem<0)
    {
        return 0;
    }

    CString strSel;
    strSel = m_pLBCPou->GetItemText(iItem, 0);

    CPouVarObject* pPouVarObj;
    if (!m_mapPouPous.Lookup(strSel, pPouVarObj))
    {
        return 0;
    }

    bHandled = TRUE;

    if (StartDrag(pPouVarObj))
    {
        if (AddToLRU(pPouVarObj->m_strName))
        {
            InvalidateLRU();
        }
    }

	return 0;
}


LRESULT CPouSelDlg::OnBegindragPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    LPNMLISTVIEW plvInfo = (LPNMLISTVIEW)pnmh;
    int iItem = plvInfo->iItem;

    if (iItem<0)
    {
        return 0;
    }

    CString strSel;
    strSel = m_pLBCLru->GetItemText(iItem, 0);

    CPouVarObject* pPouVarObj;
    if (!m_mapPouLru.Lookup(strSel, pPouVarObj))
    {
        return 0;
    }

    bHandled = TRUE;

    StartDrag(pPouVarObj);

	return 0;
}



BOOL CPouSelDlg::StartDrag(CPouVarObject* pPouVarObj)
{
	COleDataSource SrcItem;
    BOOL dropped = FALSE;
	try
	{
        CString strPath = pPouVarObj->m_strName;
        int itemType = pPouVarObj->GetDBObjectType();
        CString strXML = pPouVarObj->GetDragAndDropString();
		
		//
		//construct the drag item
		//
        CComBSTR sXML = strXML;
		CXMLDragItem Item(strPath,itemType,sXML);
    
		CSharedFile GlobFile;
     
		CArchive ar(&GlobFile, CArchive::store);
		Item.Serialize(ar);
		ar.Close();
		SrcItem.CacheGlobalData(m_uiClipFormatXML, GlobFile.Detach());
		DROPEFFECT def;
        def = SrcItem.DoDragDrop();
        if (def!=DROPEFFECT_NONE)
        {
            dropped = TRUE;
        }
	}
	catch(HRESULT hrRes)
	{
		//hr exception
		//TODO status output
		HRESULT hr = hrRes;
	}
	catch(CMemoryException *e)
	{
		//out of memory
		//TODO status output
		e->Delete();
	}
	catch(CFileException* e)
	{
		//file op failed
		//TODO status output
		e->Delete();
	}
	catch(CArchiveException* e)
	{
		//archive op failed
		//TODO status output
		e->Delete();
	}

    return dropped;
}


LRESULT CPouSelDlg::OnShowUserHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CString strUserHelp;
    int i;
    CPouVarObject* pPouVarObj;

    if (!m_mapPouPous.Lookup(m_strCMPouName, pPouVarObj))
    {
        return 0;
    }

    ASSERT(pPouVarObj);
    if (!pPouVarObj)
    {
        return 0;
    }

    m_pPouVarSelData->CheckOutUserHelp(pPouVarObj);
   
    CPouObject* pPouObj = NULL;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
    if (!pPouObj)
    {
        return 0;
    }

    if (pPouObj->m_strUserHelp.IsEmpty())
    {
        return 0;
    }
    
    strUserHelp = pPouObj->m_strUserHelp;

    // insert full path info between ATCMfile:<filename>
    CString strFullUrl = strUserHelp;
    CString strPath;
    strPath = pPouVarObj->m_strOrigin;
    if (!strPath.IsEmpty())
    {
        i = strPath.ReverseFind(_T('.'));
        if (i>0)
        {
            strPath = strPath.Left(i);
        }

        CString strUrlPrefix;
        CString strUrl;

        i = strUserHelp.Find(_T(":"));
        if (i>=0)
        {
            strUrlPrefix = strUserHelp.Left(i+1);
            strUrl = strUserHelp.Mid(i+1);
        }
        else
        {
            strUrlPrefix = _T("");
            strUrl = strUserHelp;
        }

        strFullUrl = strUrlPrefix + strPath + _T("\\") +strUrl;
    }

    CComBSTR sUrl = strFullUrl;

    m_pPouVarSelCtrl->Fire_ShowHelpFile(sUrl);

    return 0;
}



LRESULT CPouSelDlg::OnShowHeader(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CPouVarObject* pPouVarObj;

    if (!m_mapPouPous.Lookup(m_strCMPouName, pPouVarObj))
    {
        return 0;
    }

    ASSERT(pPouVarObj);
    if (!pPouVarObj)
    {
        return 0;
    }
    
    CString strHeaderFile;
    strHeaderFile = m_pPouVarSelData->CheckOutHeader(pPouVarObj);

    if (strHeaderFile.IsEmpty())
    {
        return 0;
    }
    
    CComBSTR sPou = pPouVarObj->m_strName;
    CComBSTR sHeaderFile = strHeaderFile;
    m_pPouVarSelCtrl->Fire_ShowHeaderFile(sHeaderFile);
    return 0;

}


CList<CString, CString&>* CPouSelDlg::GetLruList()
{
    return &m_listPouLru;
}


void CPouSelDlg::GetPouList(CList<CString, CString&>& listPous)
{
    int nr = m_pLBCPou->GetItemCount();
    for (int i=0; i<nr; i++)
    {
        CString str = m_pLBCPou->GetItemText(i,0);
        listPous.AddTail(str);
    }
    return;
}




LRESULT CPouSelDlg::OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ASSERT(m_pPouVarSelData!=NULL);
    if (m_pPouVarSelData==NULL)
    {
        return 0;
    }


    CComQIPtr<ICEProjInfo> pProjMan;
    pProjMan = m_pPouVarSelData->GetProjManInterface();
    ASSERT(pProjMan!=NULL);
    if (pProjMan==NULL)
    {
        return 0;
    }

    pProjMan->ShowContextHelp(IDD_DLG_POUSEL);
    return 0;
}

