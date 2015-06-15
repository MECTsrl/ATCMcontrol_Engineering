




#include "stdafx.h"
#include "resource.h"
#include "VarSelDlg.h"


#include "PouVarSelCtrl.h"
#include "PouVarSelData.h"

#include "CeVarDecl.h"

#include "DragDropHelper\PouVarDefs.h"
#include "comhelper.h"

#include "baselib.h"
#include "BaseLibCom.h"
#include "BaseLibCom_i.c"
#include "fc_tools\fc_string.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVarSelDlg::CVarSelDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pPouVarSelCtrl = NULL;
    m_pPouVarSelData = NULL;

    m_bChangeEditLock = FALSE;
    m_bDoNotChangeInfo = FALSE;

    m_pStaticFilter     = new CStatic();
    m_pStaticTypes      = new CStatic();
    m_pLBTypes          = new CListBox();

    m_pStaticVar        = new CStatic();
    m_pEditVar          = new CEdit();
    m_pLBCtrlVar        = new CListCtrl();

    m_strPVSSelAll.LoadString(IDS_PVS_SEL_ALL);
    m_strPVSSelHeader.LoadString(IDS_PVS_SEL_HEADER);
    m_strPVSSelGlobal.LoadString(IDS_PVS_SEL_GLOBAL);
    m_strPVSSelComplex.LoadString(IDS_PVS_SEL_COMPLEX);
    m_strPVSSelSimple.LoadString(IDS_PVS_SEL_SIMPLE);
    m_strPVSDefine= "Define"; //TODO use resource strings
    m_strPVSUpdate= "Update";
    m_strPVSSource= "Source: ";
    m_strPVSInsert= "Insert to: ";
}

CVarSelDlg::~CVarSelDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pStaticFilter->Detach();
    delete m_pStaticFilter;
    
    m_ScopeFilterList.Detach();

    m_pStaticTypes->Detach();
    delete m_pStaticTypes;
    m_pLBTypes->Detach();
    delete m_pLBTypes;

    m_pStaticVar->Detach();
    delete m_pStaticVar;
    m_pEditVar->Detach();
    delete m_pEditVar;
    m_pLBCtrlVar->Detach();
    delete m_pLBCtrlVar;

    m_btnApply.Detach();
    m_btnExternal.Detach();
    m_btnDefineOrUpdate.Detach();
    m_btnDelete.Detach();

    m_staticVNName.Detach();
    m_editVNName.Detach();
    m_staticVNAddr.Detach();
    m_editVNAddr.Detach();
    m_staticVNType.Detach();
    m_CBVNType.Detach();
    m_staticVNInitVal.Detach();
    m_editVNInitVal.Detach();
    m_staticVNClass.Detach();
    m_CBVNClass.Detach();
    m_staticVNComment.Detach();
    m_editVNComment.Detach();
    
    m_staticSourceFile.Detach();
    m_editIECText.Detach();

}


LRESULT CVarSelDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_hWnd != NULL)
    {
        HWND hWndDlgItem;

        hWndDlgItem = GetDlgItem(IDC_VAR_FILTER_STATIC);
        m_pStaticFilter->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_SCOPE_FILTER_LIST);
        m_ScopeFilterList.Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_VAR_TYPES_STATIC);
        m_pStaticTypes->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_VAR_DATA_TYPES_LIST);
        m_pLBTypes->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_VAR_VAR_STATIC);
        m_pStaticVar->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_VAR_VAR_EDIT);
        m_pEditVar->Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_VAR_VAR_LISTCTRL);
        m_pLBCtrlVar->Attach(hWndDlgItem);

        m_btnApply.Attach(          GetDlgItem(IDC_VAR_APPLY_BTN));
        m_btnDelete.Attach(         GetDlgItem(IDC_VAR_DELETE_BTN));
        m_btnDefineOrUpdate.Attach( GetDlgItem(IDC_VAR_DEFINEORUPDATE_BTN));
        m_btnExternal.Attach(       GetDlgItem(IDC_VAR_EXTERNAL_BTN));

        m_staticVNName.Attach(      GetDlgItem(IDC_VAR_VN_NAME_STATIC));
        m_editVNName.Attach(        GetDlgItem(IDC_VAR_VN_NAME_EDIT));
        
        m_staticVNAddr.Attach(        GetDlgItem(IDC_VAR_VN_ADDR_STATIC));
        m_editVNAddr.Attach(        GetDlgItem(IDC_VAR_VN_ADDR_EDIT));
        
        m_staticVNType.Attach(      GetDlgItem(IDC_VAR_VN_TYPE_STATIC));
        m_CBVNType.Attach(          GetDlgItem(IDC_VAR_VN_TYPE_CB));
        
        m_staticVNInitVal.Attach(   GetDlgItem(IDC_VAR_VN_INITVAL_STATIC));
        m_editVNInitVal.Attach(     GetDlgItem(IDC_VAR_VN_INITVAL_EDIT));
        
        m_staticVNClass.Attach(     GetDlgItem(IDC_VAR_VN_CLASS_STATIC));
        m_CBVNClass.Attach(         GetDlgItem(IDC_VAR_VN_CLASS_CB));
        
        m_staticVNComment.Attach(   GetDlgItem(IDC_VAR_VN_COMMENT_STATIC));
        m_editVNComment.Attach(     GetDlgItem(IDC_VAR_VN_COMMENT_EDIT));        

        m_staticSourceFile.Attach(  GetDlgItem(IDC_SOURCEFILE_STATIC));
        m_editIECText.Attach(       GetDlgItem(IDC_IECTEXT_EDIT));
    }

    int iSize = sizeof(m_sysMetrics);
    ZeroMemory(&m_sysMetrics, iSize);
    m_sysMetrics.cbSize = iSize;
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, iSize, &m_sysMetrics, 0);
	 
    return 0;
}


LRESULT CVarSelDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    const int bGes      = LOWORD(lParam); // width of client area
    const int hGes      = HIWORD(lParam); // height of client area
    const LONG dbu      = GetDialogBaseUnits();
    const WORD dbuh     = LOWORD(dbu);
    const WORD dbuv     = HIWORD(dbu);

    const int a         = 4; // pad static fields -> control
    const int b         = 8; // pad control -> control
    const int hStatic   = dbuv; // height of a static field
    const int hEdit     = m_sysMetrics.iCaptionHeight;  // height of an edit field
    const int hCombo    = hEdit + 3;    // height of combo
    const int hButton   = MulDiv(m_sysMetrics.iCaptionHeight, 3, 2);  // height of a button

    
    int hFix = 2*hCombo + 4*hEdit + 7*a + hStatic + 2*hEdit + 3*b + hButton;

    //hf + hFix = hGes;
    //hf = 2*hfc + 2*b + 2*a + (2*hStatic)/3;<== should be "2*hfc + 2*b + 3*a + (2*hStatic)/3"
    //                                           don't know why 2 works better...                         
    //hfcMin = 2*hEdit;
    const int hfc = max(( (hGes - hFix) - 2*b - 2*a - MulDiv(hStatic, 3, 2))/2, 2*hEdit);
    const int hf  = 2*hfc + 2*b + 2*a + MulDiv(hStatic, 3, 2);

    //bGes = 2*b + a + bf +bv
    const int bfMax = 8*hEdit;
    const int bf    = min((bGes-2*b-a)/2, bfMax);
    const int bv    = bGes - 2*b - a - bf;
    int iyCur;      // relative y coordinate in client coordinates
    int ixCur;      // relative x coordinate in client coordinates

                   //MoveWindow(x, y, w, h);
    iyCur = a;
    ixCur = b;
    m_pStaticFilter->MoveWindow(ixCur, iyCur, bf, hf);
    m_pStaticVar->MoveWindow(ixCur+bf+a, iyCur, bv, hf);
    
    iyCur += hStatic/2 + b;
    ixCur += a;
    m_ScopeFilterList.MoveWindow(ixCur, iyCur, bf-2*a, hfc);
    m_pEditVar->MoveWindow(ixCur+bf+a, iyCur, bv-2*a, hEdit);

    m_pLBCtrlVar->MoveWindow(ixCur+bf+a, iyCur+hEdit+a, bv-2*a, hf-2*a-b-hEdit-hStatic/2);
    m_pLBCtrlVar->SetColumnWidth(0, bv-2*a);

    iyCur += hfc+b;
    m_pStaticTypes->MoveWindow(ixCur, iyCur, bf-2*a, hStatic);

    iyCur += hStatic+a;
    m_pLBTypes->MoveWindow(ixCur, iyCur, bf-2*a, hfc);




    CString str = _T("Identifier:");
    int bId   = str.GetLength() * dbuh;

    iyCur  = hf + b;
    ixCur  = b;
    m_staticVNName.MoveWindow(ixCur, iyCur, bId, hEdit);

    iyCur += hEdit+a;
    m_staticVNClass.MoveWindow(ixCur, iyCur, bId, hEdit);
    
    iyCur += hCombo+a;
    m_staticVNAddr.MoveWindow(ixCur, iyCur, bId, hEdit);

    iyCur += hEdit+a;
    m_staticVNType.MoveWindow(ixCur, iyCur, bId, hEdit);
    
    iyCur += hCombo+a;
    m_staticVNInitVal.MoveWindow(ixCur, iyCur, bId, hEdit);
    
    iyCur += hEdit+a;
    m_staticVNComment.MoveWindow(ixCur, iyCur, bId, hEdit);

    iyCur += hEdit+a;
    m_staticSourceFile.MoveWindow(ixCur, iyCur ,bGes - 2*b, hEdit);
    
    iyCur += hEdit+a;
    m_editIECText.MoveWindow(ixCur, iyCur ,bGes - 2*b, 2*hEdit);

    int bButton = (bGes - 3*b -3*a) / 4;
    iyCur += 2*hEdit + b;
    m_btnApply.MoveWindow(ixCur, iyCur, bButton, hButton);

    ixCur += bButton + a;
    m_btnDelete.MoveWindow(ixCur, iyCur, bButton, hButton);
    
    ixCur += bButton + a;
    m_btnDefineOrUpdate.MoveWindow(ixCur, iyCur, bButton, hButton);
    
    ixCur += bButton + a;
    m_btnExternal.MoveWindow(ixCur, iyCur, bButton, hButton);



    int bEdit = bGes - 3*b - bId;
    iyCur  = hf + b;
    ixCur  = b + bId + b;
    m_editVNName.MoveWindow(ixCur, iyCur ,bEdit, hEdit);

    iyCur += hEdit+a;
    m_CBVNClass.MoveWindow(ixCur, iyCur ,bEdit, 10*hEdit);

    iyCur += hCombo+a;
    m_editVNAddr.MoveWindow(ixCur, iyCur ,bEdit, hEdit);

    iyCur += hEdit+a;
    m_CBVNType.MoveWindow(ixCur, iyCur ,bEdit, 10*hEdit);

    iyCur += hCombo+a;
    m_editVNInitVal.MoveWindow(ixCur, iyCur ,bEdit, hEdit);

    iyCur += hEdit+a;
    m_editVNComment.MoveWindow(ixCur, iyCur ,bEdit, hEdit);

    return 0;
}

void CVarSelDlg::InitDlg(CPouVarSelCtrl* pPouVarSelCtrl)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pPouVarSelCtrl = pPouVarSelCtrl;
    m_pPouVarSelData = pPouVarSelCtrl->GetPouVarSelData();

    m_strVarSelectedVar = _T("");
}

void CVarSelDlg::FinishInit()
{
    FillTypesList();
    FillDataTypeFilter();
    FillDataTypeSelection();
    FillVarClassSelection();

    m_pLBTypes->SetSel(0);

    // if there are a lot of global variables -> 
    // disable Global Vars Check Box by default, to avoid long update times
    // (the update of the list box and var map has to be optimized!!)
    //##EF TODO if (m_pPouVarSelData->m_listGlobalInstances.GetCount()>1000)
    //##EF{
    //##EF    m_bSelectGlobalVars = FALSE;
    //##EF    m_pChBGlobal->SetCheck(0);
    //##EF}
}


void CVarSelDlg::FillScopeFilterList()
{
    CString lastScopeFilter; //only for optimization...

    // first add special scopes:
    m_ScopeFilterList.ResetContent();
    m_ScopeFilterList.InsertString(-1, m_strPVSSelAll);
    m_ScopeFilterList.InsertString(-1, m_strPVSSelHeader);
    if(m_pPouVarSelData->m_mapGVLGroups.GetCount()>1)
        m_ScopeFilterList.InsertString(-1, m_strPVSSelGlobal);

    if(m_CurrentScopeFilter.IsEmpty()) //default is <All>
        m_CurrentScopeFilter = m_strPVSSelAll;

    lastScopeFilter = m_CurrentScopeFilter;

    // then all gvls of the project:
    POSITION pos;
    pos = m_pPouVarSelData->m_mapGVLGroups.GetStartPosition();

    CString          strGvl;
    CGroupContainer* pGrpCont;
    CSize            sz;
    int              dx=0;
    CDC*             pDC = m_ScopeFilterList.GetDC();

    while(pos)
    {   //TODO: order of CMap is not original order as in 4cp !
        m_pPouVarSelData->m_mapGVLGroups.GetNextAssoc(pos, strGvl, pGrpCont);
        m_ScopeFilterList.InsertString(-1, pGrpCont->strFileName);
        sz = pDC->GetTextExtent(pGrpCont->strFileName);
        if (sz.cx > dx)
            dx = sz.cx;

    }
    m_ScopeFilterList.ReleaseDC(pDC);
    m_ScopeFilterList.SetHorizontalExtent(dx);//make the H-Scroll work if long gvl file name...

    if(LB_ERR == m_ScopeFilterList.SelectString(-1, m_CurrentScopeFilter))
    {
        //maybe a gvl was removed that was prevoiusly selected, in this case
        //simpley select <All> as was done initially:
        m_CurrentScopeFilter = m_strPVSSelAll;
        m_ScopeFilterList.SelectString(-1, m_CurrentScopeFilter);
    }

    if(lastScopeFilter != m_CurrentScopeFilter)
        InvalidateDlgData();
    //else: optimize update
}



void CVarSelDlg::FillDataTypeFilter()
{
    // first add special types
    m_pLBTypes->AddString(m_strPVSSelAll);
    m_pLBTypes->AddString(m_strPVSSelComplex);
    m_pLBTypes->AddString(m_strPVSSelSimple);

    // then all avaliable types (supported by installed targets)
    POSITION pos;
    pos = m_listVarTypes.GetHeadPosition();

    while(pos)
    {
        CString str = m_listVarTypes.GetNext(pos);
        m_pLBTypes->AddString(str);
    }
}

void CVarSelDlg::FillDataTypeSelection()
{
    POSITION pos;
    pos = m_listVarTypes.GetHeadPosition();

    while(pos)
    {
        CString str = m_listVarTypes.GetNext(pos);
        m_CBVNType.AddString(str);
    }
}


void CVarSelDlg::FillVarClassSelection()
{
    HRESULT hr;
    BOOL bSuppRetain = FALSE;

    CComQIPtr<ICEProjInfo> pPI = m_pPouVarSelData->GetProjManInterface();
    if (pPI!=NULL)
        hr = pPI->getSupportRetain(&bSuppRetain);

    m_CBVNClass.ResetContent();

    bool bOnlyGlobal = !_tcsicmp(FC_StringGetFileExt(m_pPouVarSelData->GetActivePouFileName()), _T("gvl"));

    if(bOnlyGlobal)
    {
        m_CBVNClass.AddString(PVS_XML_VAR_GLOBAL);
        if(bSuppRetain)
            m_CBVNClass.AddString(PVS_XML_VAR_GLOBAL _T(" ") PVS_XML_RETAIN);
    }
    else
    {
        m_CBVNClass.AddString(PVS_XML_VAR);
        m_CBVNClass.AddString(PVS_XML_VAR_GLOBAL);

        if (m_pPouVarSelData->GetActivePouSupportInput())
            m_CBVNClass.AddString(PVS_XML_VAR_INPUT); 

        if (m_pPouVarSelData->GetActivePouSupportOutput())
            m_CBVNClass.AddString(PVS_XML_VAR_OUTPUT);
    
        if (m_pPouVarSelData->GetActivePouSupportInOut())
            m_CBVNClass.AddString(PVS_XML_VAR_IN_OUT);

        if (bSuppRetain && m_pPouVarSelData->GetActivePouSupportRetain())
        {
            m_CBVNClass.AddString(PVS_XML_VAR _T(" ") PVS_XML_RETAIN);
        
            if(bSuppRetain)
                m_CBVNClass.AddString(PVS_XML_VAR_GLOBAL _T(" ") PVS_XML_RETAIN);

            if (m_pPouVarSelData->GetActivePouSupportInput())
                m_CBVNClass.AddString(PVS_XML_VAR_INPUT _T(" ") PVS_XML_RETAIN); 
        
            if (m_pPouVarSelData->GetActivePouSupportOutput())
                m_CBVNClass.AddString(PVS_XML_VAR_OUTPUT _T(" ") PVS_XML_RETAIN);
        
            m_CBVNClass.AddString(PVS_XML_VAR _T(" ") PVS_XML_NONRETAIN);
        
            if (m_pPouVarSelData->GetActivePouSupportInput())
                m_CBVNClass.AddString(PVS_XML_VAR_INPUT _T(" ") PVS_XML_NONRETAIN); 

            if (m_pPouVarSelData->GetActivePouSupportOutput())
                m_CBVNClass.AddString(PVS_XML_VAR_OUTPUT _T(" ") PVS_XML_NONRETAIN);
        }
        m_CBVNClass.AddString(PVS_XML_VAR _T(" ") PVS_XML_CONST);//##EF TODO PVS_XML_CONSTANT is better!
    }

    //seclet again what ever was in there before, if now is allowed scope:
    m_CBVNClass.SelectString(-1, m_strSelectedVarClass);
    m_CBVNClass.GetWindowText(m_strSelectedVarClass);//may now also be ""
}


void CVarSelDlg::FillTypesList()
{
    HRESULT hr;
    CComQIPtr<ICEProjInfo> pProjInfo;

    pProjInfo = m_pPouVarSelData->GetProjManInterface();
    if (!pProjInfo)
    {
        return;
    }

    SAFEARRAY* parrDataTypes;

    hr = pProjInfo->getSupportedIECDataTypes(&parrDataTypes);

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrDataTypes, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrDataTypes)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = (CString)bstr;
        m_listVarTypes.AddTail(str);
    }

    ::SafeArrayUnaccessData(parrDataTypes);
    ::SafeArrayDestroy(parrDataTypes);
}



void CVarSelDlg::InvalidateDlgData()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

TAKE_TIME_INIT;
TAKE_TIME_START;

    FillVarClassSelection();

    POSITION pos;
    m_mapVarVars.RemoveAll();

#if 0 //##EF TODO remove ?
    if (!m_pPouVarSelData->ActivePouSupportVarDecl())
    {
        m_pBtnInsert->EnableWindow(FALSE);
        m_pBtnNew->EnableWindow(FALSE);
        m_pBtnModify->EnableWindow(FALSE);
        m_pBtnDelete->EnableWindow(FALSE);
        m_pBtnExternal->EnableWindow(FALSE);    
        m_editVNName.EnableWindow(FALSE);
    }
    else
    {
        m_pBtnInsert->EnableWindow(TRUE); 
        m_editVNName.EnableWindow(TRUE);
    }
#endif

    // get types filter
    BOOL bAll = FALSE;
    BOOL bComplex = FALSE;
    PVS_BITSET bsType = 0;

    int anzSel = m_pLBTypes->GetSelCount();
    int* pSelInd;
    pSelInd = new int[anzSel];
    m_pLBTypes->GetSelItems(anzSel, pSelInd);
    for (int i=0; i<anzSel; i++)
    {
        CString strTypeName;
        m_pLBTypes->GetText(pSelInd[i], strTypeName);
        if (strTypeName.Compare(m_strPVSSelAll)==0)
        {
            bAll = TRUE;
        }
        else if (strTypeName.Compare(m_strPVSSelComplex)==0)
        {
            bComplex = TRUE;
        }
        else if (strTypeName.Compare(m_strPVSSelSimple)==0)
        {
            bsType |= PVS_TBS_ANY_SIMPLE;
        }
        else
        {
            bsType |= CVarObject::GetDataTypeCodeFromText(strTypeName);
        }
    }

    delete [] pSelInd;


    // fill list ctrl
    m_pLBCtrlVar->DeleteAllItems();

    int iItem = 0;


TAKE_TIME_END(_T("VarSelDlg::InvalidateDlgData - Init  "));
TAKE_TIME_START;

    // get local vars
    if (!m_CurrentScopeFilter.Compare(m_strPVSSelAll) || !m_CurrentScopeFilter.Compare(m_strPVSSelHeader))
    {
        pos = m_pPouVarSelData->m_listLocalInstances.GetHeadPosition();
        while(pos)
        {
            CVarObject* pVarObj = m_pPouVarSelData->m_listLocalInstances.GetNext(pos);
            m_mapVarVars.SetAt(pVarObj->m_strName, pVarObj);
            // select from data type ....
            if (bAll || (bComplex && pVarObj->m_bIsFBorStructDataType)
                     || ((bsType & pVarObj->m_bsetDataType) > 0) )
            {
                m_pLBCtrlVar->InsertItem(iItem, pVarObj->m_strName);
                iItem++;
            }
        }
    }

TAKE_TIME_END(_T("VarSelDlg::InvalidateDlgData - Fill loc vars  "));
TAKE_TIME_START;

    // get global vars
    POSITION lastPos = NULL;
    //<all> or <global>, insert all variables from all gvls:
    pos = m_pPouVarSelData->m_listGlobalInstances.GetHeadPosition();

    CGroupContainer* pGVLCont = NULL;
    if (m_CurrentScopeFilter.Compare(m_strPVSSelHeader))//else: <header> is selected as filter
                                                     //then no globals at all.
    {
        if(!_tcsicmp(FC_StringGetFileExt(m_CurrentScopeFilter), _T("gvl")))
        {
            CString gvlFileLower = m_CurrentScopeFilter;
            gvlFileLower.MakeLower();
            m_pPouVarSelData->m_mapGVLGroups.Lookup(gvlFileLower, pGVLCont);
            if(pGVLCont)
            {
                //only globals within that range belong to the gvl selected:
                pos = pGVLCont->firstMemberPos;
                lastPos = pGVLCont->lastMemberPos;
            }//else should not happen...
        }//else <all> or <global>, insert all variables from all gvls:
            
            
        bool bDoBreak = false;
        while(pos)
        {
            if(lastPos == pos)
                bDoBreak = true;
            
            CVarObject* pVarObj = m_pPouVarSelData->m_listGlobalInstances.GetNext(pos);

            CVarObject* tempVarObj;
            if (pGVLCont!=NULL || !m_mapVarVars.Lookup(pVarObj->m_strName, tempVarObj))
            {
                m_mapVarVars.SetAt(pVarObj->m_strName, pVarObj);
            
                // select from data type ....
                if (bAll || (bComplex && pVarObj->m_bIsFBorStructDataType)
                         || ((bsType & pVarObj->m_bsetDataType) > 0) )
                {
                    m_pLBCtrlVar->InsertItem(iItem, pVarObj->m_strName);
                    iItem++;
                }
            }//else: variable already exists via locals (as VAR_EXTERNAL)
             //      This test is not needed if we are building the list for a single gvl
             //      scope, therefore do not Lookup in m_mapVarVars if pGVLCont!=NULL.
            
            if(bDoBreak)
                break;
        }
    }
 
TAKE_TIME_END(_T("VarSelDlg::InvalidateDlgData - Fill glob vars  "));
TAKE_TIME_START;
   
    // update selection
    CString     strCurVar;
    LVFINDINFO  findInfo;
    
    m_editVNName.GetWindowText(strCurVar);
    findInfo.flags = LVFI_STRING;
    findInfo.psz = strCurVar;
    int sel = m_pLBCtrlVar->FindItem(&findInfo);
    if(strCurVar.IsEmpty())
        m_pLBCtrlVar->SetItemState(0,  LVIS_SELECTED, LVIS_SELECTED);
    else if (sel>=0)
        m_pLBCtrlVar->SetItemState(sel,  LVIS_SELECTED, LVIS_SELECTED);
    
    //call m_editVNName.SetWindowText once to update all var info fields:
    m_pEditVar->GetWindowText(strCurVar);
    m_editVNName.SetWindowText(strCurVar);
    CVarObject* pVarObj = NULL;
    if (m_mapVarVars.Lookup(strCurVar, pVarObj))
       UpdateInfoFieldsFromVarObj(pVarObj);


TAKE_TIME_END(_T("VarSelDlg::InvalidateDlgData - UpdateListView  "));

}




void CVarSelDlg::ChangeVarSelectionHint(const CString& strPouName, 
                                        const CString& strVarNamePrefix, 
                                        const CString& strDataType)
{
    if (!strDataType.IsEmpty())
    {
        int sel = m_CBVNType.FindString(-1,strDataType);
        if (sel<0)
        {
            m_CBVNType.SetWindowText(strDataType);
        }
        else
        {
            if (m_CBVNType.GetCount()>sel)
                m_CBVNType.SetCurSel(sel);
        }
    }

    if (!strVarNamePrefix.IsEmpty())
    {
        //good idea?: if the hint has a variable name always select <All> as scope:
        if(m_CurrentScopeFilter != m_strPVSSelAll)
        {
            m_CurrentScopeFilter = m_strPVSSelAll;
            m_ScopeFilterList.SelectString(-1, m_strPVSSelAll); //SelectString does not fire 
            InvalidateDlgData();                                //OnSelchangeScopeFilterList, so do it manually
        }

        //check if it is an existing var or a new one (Osai special feature):
        CString strTypeHint;
        if(m_pPouVarSelData->IsPredefinedVar(strVarNamePrefix, strTypeHint))
        {
            CVarObject* pVarObj = m_pPouVarSelData->FindGlobVarByAddress(strVarNamePrefix);
            if(pVarObj)
            {   //enter here if the predef var is used as <symbolic name> AT <predef var>.
                //now the special thing: osai wants to show the correspondig global decl
                //not any var_external of the sympolic name <symbolic name>, we change 
                //the scope to the gvl before we set m_editVNName.SetWindowText:
                if(m_CurrentScopeFilter != pVarObj->m_strUserData)
                {
                    m_CurrentScopeFilter = pVarObj->m_strUserData;
                    m_ScopeFilterList.SelectString(-1, pVarObj->m_strUserData); //SelectString does not fire 
                    InvalidateDlgData();                                        //OnSelchangeScopeFilterList, so do it manually
                }
                m_editVNName.SetWindowText(pVarObj->m_strName);
                UpdateInfoFieldsFromVarObj(pVarObj);
            }
            else
            {   
                m_strSelectedVarClass = PVS_XML_VAR_GLOBAL;
                m_CBVNClass.SetWindowText(PVS_XML_VAR_GLOBAL);  
                m_editVNName.SetWindowText(_T(""));  
                m_CBVNType.SelectString(-1, strTypeHint);
                m_editVNAddr.SetWindowText(strVarNamePrefix);  
                GotoDlgCtrl(m_editVNName.GetSafeHwnd());//goto identifier edit
            }
        }
        else
        {
            CVarObject* pVarObj = m_pPouVarSelData->FindGlobalInstance(strVarNamePrefix);
            if(pVarObj && m_CurrentScopeFilter != pVarObj->m_strUserData)
            {
                m_CurrentScopeFilter = pVarObj->m_strUserData;
                m_ScopeFilterList.SelectString(-1, pVarObj->m_strUserData); //SelectString does not fire 
                InvalidateDlgData();                                        //OnSelchangeScopeFilterList, so do it manually
            }
            
            m_editVNName.SetWindowText(strVarNamePrefix);  
            pVarObj = NULL;
            if (m_mapVarVars.Lookup(strVarNamePrefix, pVarObj))
               UpdateInfoFieldsFromVarObj(pVarObj);
            
            if(pVarObj)
            {
                if(pVarObj->m_varClass==VARCLASS_GLOBAL)
                    GotoDlgCtrl(m_editVNAddr.GetSafeHwnd());
                else
                    GotoDlgCtrl(m_CBVNType.GetSafeHwnd());
            }
            else
            {
                GotoDlgCtrl(m_CBVNClass.GetSafeHwnd());
            }

        }
    }//if (!strVarNamePrefix.IsEmpty())
}


LRESULT CVarSelDlg::OnSelchangeScopeFilterList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    int idx = m_ScopeFilterList.GetCurSel();
    assert(idx != LB_ERR);
    m_ScopeFilterList.GetText(idx, m_CurrentScopeFilter);
    InvalidateDlgData();
    return 0;
}


LRESULT CVarSelDlg::OnSelchangeVarDataTypesList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    InvalidateDlgData();
	return 0;
}


//Called if edit box above the var list box has changed:
LRESULT CVarSelDlg::OnChangeVarVarEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strText;
    m_pEditVar->GetWindowText(strText);

    //make the other var name edit field display 100% the same:
    CString strCurrText;
    m_editVNName.GetWindowText(strCurrText);
    if(strCurrText != strText)
        m_editVNName.SetWindowText(strText);

    if (m_bChangeEditLock)
        return 0;

    if (strText.IsEmpty())
        return 0;

    LVFINDINFO info;
    info.flags = LVFI_PARTIAL|LVFI_STRING;
    info.psz = strText;

    //try to find an item in the list that starts with strText:
    int sel = m_pLBCtrlVar->FindItem(&info);
    if (sel>=0)
    {
        CString  strFound = m_pLBCtrlVar->GetItemText(sel, 0);
        int nStart = FC_StringStartsWithI(strFound, strText);
        if(nStart>=0)
        {
            m_pLBCtrlVar->SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
            m_pLBCtrlVar->Update(sel);
            m_pLBCtrlVar->EnsureVisible(sel, FALSE);
            m_pEditVar->SetSel(nStart, _tcslen(strFound), FALSE);
        }//else for some reason LVFI_PARTIAL finds "g_bool" if info.psz = "g_arrayBool"
         //therefore test explicitely if starts with
    }
    
	return 0;
}



//called if anyhow the item selection in the var list control changed:
LRESULT CVarSelDlg::OnItemchangedVarVarListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int sel = m_pLBCtrlVar->GetNextItem(-1, LVNI_SELECTED);
    CString strSelected = sel>=0 ? m_pLBCtrlVar->GetItemText(sel, 0) : _T("");
    if (!strSelected.IsEmpty())
    {
        BOOL bOldLock = m_bChangeEditLock;
        m_bChangeEditLock = TRUE;
            m_pEditVar->SetWindowText(strSelected);
        m_bChangeEditLock = bOldLock;

        CVarObject* pVarObj = NULL;
        if (m_mapVarVars.Lookup(strSelected, pVarObj))
            UpdateInfoFieldsFromVarObj(pVarObj);
    }
    return 0;
}



LRESULT CVarSelDlg::OnChangeVarVNNameEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strVarName;
    m_editVNName.GetWindowText(strVarName);

    //make the other var name edit field display 100% the same:
    CString strVarName2;
    m_pEditVar->GetWindowText(strVarName2);
    if(strVarName2 != strVarName)
    {
        BOOL bOldLock = m_bChangeEditLock;
        m_bChangeEditLock = TRUE;
            m_pEditVar->SetWindowText(strVarName);
        m_bChangeEditLock = bOldLock;
    }

    BOOL bEnableEditFields;
    CVarObject* pVarObj = NULL;
    if (m_mapVarVars.Lookup(strVarName, pVarObj))
    {
        m_btnDefineOrUpdate.SetWindowText(m_strPVSUpdate);
        if(pVarObj->m_varClass == VARCLASS_EXTERNAL)
        {
            m_btnDefineOrUpdate.EnableWindow(FALSE);
            bEnableEditFields = FALSE;
        }
        else
        {
            m_btnDefineOrUpdate.EnableWindow(TRUE);
            bEnableEditFields = TRUE;
        }
    }
    else
    {
        m_btnDefineOrUpdate.SetWindowText(m_strPVSDefine);
        m_btnDefineOrUpdate.EnableWindow(m_pPouVarSelData->ActivePouSupportVarDecl());
        bEnableEditFields = TRUE;
    }

    //always editabe excepet if var_external:
    m_CBVNClass.EnableWindow(bEnableEditFields);
    m_editVNAddr.EnableWindow(bEnableEditFields);
    m_CBVNType.EnableWindow(bEnableEditFields);
    m_editVNInitVal.EnableWindow(bEnableEditFields);
    m_editVNComment.EnableWindow(bEnableEditFields);


    //enable/disable the external button:
    if(pVarObj && pVarObj->m_varClass == VARCLASS_GLOBAL && 
        m_pPouVarSelData->GetActivePouSupportExternal() &&
        m_pPouVarSelData->FindLocalInstance(strVarName)==NULL
      )
      m_btnExternal.EnableWindow(TRUE && m_pPouVarSelData->ActivePouSupportVarDecl());
    else
      m_btnExternal.EnableWindow(FALSE);

    //enable/disable the delete button:
    m_btnDelete.EnableWindow(pVarObj != NULL);
    

    UpdateIECText(NULL, NULL);
    UpdateSourceFileInfo(NULL);

    return 0;
}

void CVarSelDlg::UpdateSourceFileInfo(const TCHAR* pszClass)
{
    CVarObject* pVarObj = NULL;
    CString strVarName;
    m_editVNName.GetWindowText(strVarName);
    m_mapVarVars.Lookup(strVarName, pVarObj);

    if(pVarObj)
    {
        m_staticSourceFile.SetWindowText(m_strPVSSource + pVarObj->m_strUserData + _T(":"));
    }
    else
    {
        CString strClass;
        if(pszClass)
            strClass = pszClass;
        else
            m_CBVNClass.GetWindowText(strClass);
        
        if(FC_StringStartsWithI(strClass, PVS_XML_VAR_GLOBAL)>=0)
        {
            if(m_pPouVarSelData->m_mapGVLGroups.GetCount()==1)
            {
                POSITION pos;
                CString  strSourceFile;
                pos = m_pPouVarSelData->m_mapGVLGroups.GetStartPosition();
                CGroupContainer* pGrpCont;
                m_pPouVarSelData->m_mapGVLGroups.GetNextAssoc(pos, strSourceFile, pGrpCont);
                m_staticSourceFile.SetWindowText(m_strPVSInsert + pGrpCont->strFileName + _T(":"));
            }
            else
            {
                m_staticSourceFile.SetWindowText(m_strPVSInsert +_T("*.gvl:"));
            }
        }
        else
        {
            m_staticSourceFile.SetWindowText(m_strPVSInsert + m_pPouVarSelData->GetActivePouFileName() + _T(":"));
        }
    }

}
LRESULT CVarSelDlg::OnClickedVarApplyBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strVarName;

    m_editVNName.GetWindowText(strVarName);
    if (!strVarName.IsEmpty())
    {
        CVarObject* pVarObj = NULL;
        if (m_mapVarVars.Lookup(strVarName, pVarObj))
        {
            CString strDaD = pVarObj->GetDragAndDropString();
            CComBSTR sDaD(strDaD);
            m_pPouVarSelCtrl->Fire_InsertFBCall(sDaD);
        }
        else
        {
            CComBSTR sVarName(strVarName);
            m_pPouVarSelCtrl->Fire_InsertVariable(sVarName);
        }
    }

	return 0;    
}

static TCHAR g_szSelectedGVL[MAX_PATH];
BOOL CALLBACK GVLSelectDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char szb[256];
    wsprintf(szb, "uMsg=0x%3x  wParam=%d, lParam=%d\n", uMsg, wParam, lParam);
    OutputDebugString(szb);
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        CString         strGVLfile;
        POSITION        pos;
        CGroupContainer*pGrpCont;
        CPouVarSelData* pVSD = (CPouVarSelData*)lParam;
        HWND            hwndCombo = GetDlgItem(hwndDlg, IDC_GVLSELECT_COMBO); 
        
        g_szSelectedGVL[0] = 0;//invalidate 

        SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0); 

        pos = pVSD->m_mapGVLGroups.GetStartPosition();
        while(pos)
        { 
            pVSD->m_mapGVLGroups.GetNextAssoc(pos, strGVLfile, pGrpCont);
            int nRet = SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)pGrpCont->strFileName); 
        }

        pos = pVSD->m_mapGVLGroups.GetStartPosition();
        if(pos)
        {
            pVSD->m_mapGVLGroups.GetNextAssoc(pos, strGVLfile, pGrpCont);
            SendMessage(hwndCombo, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)pGrpCont->strFileName); 
        }
        SendMessage(hwndCombo, CB_SHOWDROPDOWN, TRUE, 0);
        SetFocus(hwndCombo); 
    }
    return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        { 
        case IDOK: 
        {
            HWND hwndCombo = GetDlgItem(hwndDlg, IDC_GVLSELECT_COMBO); 
            SendMessage(hwndCombo, WM_GETTEXT, MAX_PATH, (LPARAM)g_szSelectedGVL); 
        }//fall th.
        case IDCANCEL: 
            EndDialog(hwndDlg, LOWORD(wParam)); 
        return TRUE; 
        case IDC_GVLSELECT_COMBO:
            if(HIWORD(wParam)==CBN_DROPDOWN)
                return TRUE;
        }
    }
    return FALSE;
}

#include <afxdlgs.h>


LRESULT CVarSelDlg::OnClickedVarDefineOrUpdateBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT hr;

    CString strName;
    CString strVarClass;
    CString strDataType;
    CString strAddress;
    CString strInitVal;
    CString strComment;

    m_editVNName.GetWindowText(strName);
    m_CBVNClass.GetWindowText(strVarClass);
    m_CBVNType.GetWindowText(strDataType);
    m_editVNAddr.GetWindowText(strAddress);
    m_editVNInitVal.GetWindowText(strInitVal);
    m_editVNComment.GetWindowText(strComment);

    //check needed fields:
    if (!CheckIECIdentifier(strName))
    {
        return 0;
    }
    if (strVarClass.IsEmpty() || strDataType.IsEmpty())
    {
        CString str;
        str.LoadString(IDS_ERROR_INCOMPLETE_VAR_INFOS);
        AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
        return 0;
    }
    if (!CheckInitValue(strInitVal, strDataType))
    {
        return 0;
    }

    CComQIPtr<ICEProjInfo> pProjInfo;
    if((pProjInfo = m_pPouVarSelData->GetProjManInterface())==NULL)
        return 0;

    //get the desination source file:
    CString strSourceFile;
    CVarObject* pVarObj = NULL;
    m_mapVarVars.Lookup(strName, pVarObj);
    if(pVarObj)
    {
        strSourceFile = pVarObj->m_strUserData;
    }
    else if(FC_StringStartsWith(strVarClass, PVS_XML_VAR_GLOBAL)>=0)
    {
        if(m_pPouVarSelData->m_mapGVLGroups.GetCount()==0)
        {
            CString str;
            str.LoadString(IDS_ERROR_NOGVL);
            AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
            return 0;
        }
        
        if(m_pPouVarSelData->m_mapGVLGroups.GetCount()==1)
        {
            POSITION pos;
            pos = m_pPouVarSelData->m_mapGVLGroups.GetStartPosition();

            CGroupContainer* pGrpCont;
            m_pPouVarSelData->m_mapGVLGroups.GetNextAssoc(pos, strSourceFile, pGrpCont);
        }
        else if(!_tcsicmp(FC_StringGetFileExt(m_CurrentScopeFilter), _T("gvl")))
        {
            strSourceFile = m_CurrentScopeFilter;
        }
        else
        {
            int nRet= DialogBoxParam(_Module.GetResourceInstance(), 
                                     MAKEINTRESOURCE(IDD_SELECTGVL), this->m_hWnd, 
                                     GVLSelectDialogProc, (LPARAM)this->m_pPouVarSelData
                                    ); 
            if(nRet != IDOK)
                return 0;
            strSourceFile = g_szSelectedGVL;
        }
    }
    else
    {
        strSourceFile = m_pPouVarSelData->GetActivePouFileName();
    }

    ICEVarDecl* pVarDecl;
    if ((pVarDecl = GetVarDeclHandler(strSourceFile))==NULL)
        return 0 ;

    CComBSTR sSourceFile(strSourceFile);
    m_pPouVarSelCtrl->Fire_SaveEditorFile(sSourceFile);

    bool bAnyGlobal = FC_StringStartsWithI(strVarClass, PVS_XML_VAR_GLOBAL)>=0;
    
    CComBSTR sName = strName;
    CComBSTR sVarClass = strVarClass;
    CComBSTR sDataType = strDataType;
    CComBSTR sAddress = bAnyGlobal ? strAddress : "";
    CComBSTR sInitVal = strInitVal;
    CComBSTR sAlias = ""; //removed old fckn in version 2.10
    CComBSTR sComment = strComment;


    if(pVarObj)
    {
        if(pVarObj->m_varClass==VARCLASS_GLOBAL && !bAnyGlobal 
          || bAnyGlobal && pVarObj->m_varClass!=VARCLASS_GLOBAL)
        {
            CString str;
            str.FormatMessage(IDS_BAD_DECL_SCOPE, pVarObj->GetVarClassText(), strVarClass);
            AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
            pVarDecl->Release();
            return 0;
        }

        hr = pVarDecl->ModifyVariable(sSourceFile,
                                      pProjInfo,
                                      sName,
                                      sName,
                                      sVarClass,
                                      sDataType,
                                      sAddress,
                                      sAlias,
                                      sComment,
                                      sInitVal);
    }
    else
    {
        hr = pVarDecl->InsertVariable(sSourceFile,
                                      pProjInfo,
                                      sName,
                                      sVarClass,
                                      sDataType,
                                      sAddress,
                                      sAlias,
                                      sComment,
                                      sInitVal);
    }

    if (hr==S_OK)
        m_pPouVarSelCtrl->Fire_ReloadEditorFile(sSourceFile);
    //##ef TODO msg box insert failed
    pVarDecl->Release();

	return 0;
}

LRESULT CVarSelDlg::OnClickedVarDeleteBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT                 hr;
    CString                 strName;
    CVarObject*             pVarObj = NULL;
    ICEVarDecl*             pVarDecl = NULL;
    CComQIPtr<ICEProjInfo>  pProjInfo;

    m_editVNName.GetWindowText(strName);

    if(!m_mapVarVars.Lookup(strName, pVarObj) || !pVarObj)
        return 0;
    if ((pVarDecl = GetVarDeclHandler(pVarObj->m_strUserData))==NULL)
        return 0 ;
    if ((pProjInfo = m_pPouVarSelData->GetProjManInterface())==NULL)
        return 0 ;

    CComBSTR sName       = strName;
    CComBSTR sSourceFile = pVarObj->m_strUserData;
    
    m_pPouVarSelCtrl->Fire_SaveEditorFile(sSourceFile);
    
    hr = pVarDecl->DeleteVariable(sSourceFile, pProjInfo, sName);
    if (hr==S_OK)
        m_pPouVarSelCtrl->Fire_ReloadEditorFile(sSourceFile);

    pVarDecl->Release();

	return 0;
}


LRESULT CVarSelDlg::OnClickedVarExternalBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT                 hr;
    CString                 strSourceFile;
    CString                 strName;
    CString                 strDataType;
    ICEVarDecl*             pVarDecl = NULL;
    CComQIPtr<ICEProjInfo>  pProjInfo;

    m_editVNName.GetWindowText(strName);
    m_CBVNType.GetWindowText(strDataType);
    
    strSourceFile = m_pPouVarSelData->GetActivePouFileName();

    if ((pVarDecl = GetVarDeclHandler(strSourceFile))==NULL)
        return 0 ;
    if((pProjInfo = m_pPouVarSelData->GetProjManInterface())==NULL)
        return 0;
    
    CComBSTR sSourceFile(strSourceFile);
    m_pPouVarSelCtrl->Fire_SaveEditorFile(sSourceFile);

    CComBSTR sName = strName;
    CComBSTR sVarClass = PVS_XML_VAR_EXTERNAL;
    CComBSTR sDataType = strDataType;
    CComBSTR sBlank = _T("");

    hr = pVarDecl->InsertVariable(sSourceFile,
                                  pProjInfo,
                                  sName,
                                  sVarClass,
                                  sDataType,
                                  sBlank, //sAddress
                                  sBlank, //sAlias,
                                  sBlank, //sComment,
                                  sBlank  //sInitVal
                                  );
    if (hr==S_OK)
        m_pPouVarSelCtrl->Fire_ReloadEditorFile(sSourceFile);

    pVarDecl->Release();
    
    return 0;
}


LRESULT CVarSelDlg::OnDblclkVarVarListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (!m_pPouVarSelData->ActivePouSupportVarDecl())
    {
        return 0;
    }

    CString strVarName;

    int sel = m_pLBCtrlVar->GetNextItem(-1, LVNI_SELECTED);
    if (sel>=0)
    {
        strVarName = m_pLBCtrlVar->GetItemText(sel, 0);
         
        CVarObject* pVarObj = NULL;
        if (m_mapVarVars.Lookup(strVarName, pVarObj))
        {
            CString strDaD = pVarObj->GetDragAndDropString();
            CComBSTR sDaD(strDaD);
            m_pPouVarSelCtrl->Fire_InsertFBCall(sDaD);
        }
        else
        {
            CComBSTR sVarName(strVarName);
            m_pPouVarSelCtrl->Fire_InsertVariable(sVarName);
        }
    }

	return 0; 	
}





ICEVarDecl* CVarSelDlg::GetVarDeclHandler(const CString& strFileName)
{
    // TODO get handler from KAD
    // HACK for testing only: built in fixed addon handler
    //CGuid guid(_T("{C2EED755-E717-11D4-B8D2-002018641833}"));

    ICEVarDecl* pVarDecl = NULL;
    
    IUnknown* pUnk = NULL;

    HRESULT hr;
    GUID g = m_pPouVarSelData->GetActiveVarHandler();
    hr = ::CoCreateInstance(g, NULL, CLSCTX_ALL,
                            __uuidof(ICEVarDecl),
                            (LPVOID*)&pVarDecl);
		                 
    if (hr != S_OK)
    {
        return NULL;
    }
    
    return pVarDecl;
}





LRESULT CVarSelDlg::OnEditupdateVarVNClassCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // do some special handling with the var storage class combo box
    // this box is a drop down box, so a unavailable storage class
    // can be shown, but this field sould not be editable and the user should
    // only be able to select an availabe storage class (available storage 
    // classes are determined via KAD)
    // So show combo box with edit field but 
    // disallow the user to edit the field here.
    m_CBVNClass.SetWindowText(m_strSelectedVarClass);
	return 0;
}
LRESULT CVarSelDlg::OnSelchangeVarVNClassCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    //HACK do to the fact that m_CBVNClass.GetWindowText does do work if selection changed:
    int sel = m_CBVNClass.GetCurSel();
    if(sel>=0) 
        m_CBVNClass.GetLBText(sel, m_strSelectedVarClass);

    UpdateIECText(NULL, m_strSelectedVarClass);
    UpdateSourceFileInfo(m_strSelectedVarClass);
	return 0;
}
LRESULT CVarSelDlg::OnEditchangeVarVNTypeCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    UpdateIECText(NULL, NULL);
    return 0;
}
LRESULT CVarSelDlg::OnSelchangeVarVNTypeCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    //HACK do to the fact that m_CBVNType.GetWindowText does do work if selection changed:
    CString str;
    int sel = m_CBVNType.GetCurSel();
    if(sel>=0) 
        m_CBVNType.GetLBText(sel, str);

    UpdateIECText(str, NULL);
    return 0;
}


LRESULT CVarSelDlg::OnChangeVarVNAddrEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    UpdateIECText(NULL, NULL);
    return 0;
}
LRESULT CVarSelDlg::OnChangeVarVNCommentEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    UpdateIECText(NULL, NULL);
    return 0;
}
LRESULT CVarSelDlg::OnChangeVarVNInitvalEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_btnDefineOrUpdate.EnableWindow(TRUE & m_pPouVarSelData->ActivePouSupportVarDecl());
    UpdateIECText(NULL, NULL);
    return 0;
}


LRESULT CVarSelDlg::OnClickedSourcefile_static(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
#if 0 //TODO nice to have: show (gvl)file if clicked the name:
    CString strSource;
    m_staticSourceFile.GetWindowText(strSource);

    strSource = strSource.Mid(m_strPVSSource.GetLength(), strSource.GetLength()-m_strPVSSource.GetLength()-1);
    CComBSTR sSource = strSource;
    m_pPouVarSelCtrl->Fire_ShowFile(sSource);
#endif
    return 0;
}




//build something like: g_int AT %MW0.0 : dint; (*[ comment2 ]*)
void CVarSelDlg::UpdateIECText(const TCHAR* pszDataType, const TCHAR* pszVarClass)
{
    CString strIEC;
    CString str;

    if(pszVarClass)
        str = pszVarClass; //HACK do to the fact that m_CBVNClass.GetWindowText does do work if selection changed
    else
        m_CBVNClass.GetWindowText(str);

    bool bAnyGlobal = FC_StringStartsWithI(str, PVS_XML_VAR_GLOBAL)>=0;
    m_editVNAddr.EnableWindow(bAnyGlobal);


    strIEC += str.IsEmpty() ? _T("VAR_????") : str;
    strIEC += _T(" ");

    m_editVNName.GetWindowText(str);
    strIEC += str.IsEmpty() ? _T("????") : str;
    strIEC += _T(" ");

    m_editVNAddr.GetWindowText(str);
    if(!str.IsEmpty() && bAnyGlobal)
    {
        strIEC += _T("AT ");
        strIEC += str;
        strIEC += _T(" ");
    }

    strIEC += _T(": ");
    if(pszDataType)
        str = pszDataType; //HACK do to the fact that m_CBVNType.GetWindowText does do work if selection changed
    else
        m_CBVNType.GetWindowText(str);
    
    strIEC += str.IsEmpty() ? _T("????") : str;


    m_editVNInitVal.GetWindowText(str);
    if(!str.IsEmpty())
    {
        strIEC += _T(" := ");
        strIEC += str;
    }
    strIEC += _T(";");
        
    m_editVNComment.GetWindowText(str);
    if(!str.IsEmpty())
    {
        strIEC += _T(" (*");
        strIEC += str;
        strIEC += _T(" *)");
    }

    m_editIECText.SetWindowText(strIEC);
}


void CVarSelDlg::UpdateInfoFieldsFromVarObj(CVarObject* pVarObj)
{
    m_strSelectedVarClass = pVarObj->GetVarClassWithModifierText();
    m_CBVNClass.SetWindowText(m_strSelectedVarClass);
    m_CBVNType.SetWindowText(pVarObj->m_strDataType);
    m_editVNAddr.SetWindowText(pVarObj->m_strAddress);
    m_editVNInitVal.SetWindowText(pVarObj->m_strInitValue);
    m_editVNComment.SetWindowText(pVarObj->m_strComment);
    m_btnDefineOrUpdate.EnableWindow(FALSE);
}



BOOL CVarSelDlg::CheckInitValue(const CString& strValue, const CString& strDataType)
{
    long lResult;
    HRESULT hr = S_OK;
    CString strErrorMsg;
    IBL_Conversion *pIBL_Conversion = NULL;

    CString strV = strValue;
    CString strD = strDataType;

    strV.TrimLeft();
    strV.TrimRight();
    strD.TrimLeft();
    strD.TrimRight();

    if (strV.IsEmpty())
    {
        return TRUE;
    }

    if (strD.IsEmpty())
    {
        CString str;
        str.LoadString(IDS_ERROR_EMPTY_DATATYPE);
        AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!((strD.CompareNoCase(_T("BOOL")) == 0) ||
          (strD.CompareNoCase(_T("BYTE")) == 0) ||
          (strD.CompareNoCase(_T("WORD")) == 0) ||
          (strD.CompareNoCase(_T("DWORD")) == 0) ||
          (strD.CompareNoCase(_T("LWORD")) == 0) ||
          (strD.CompareNoCase(_T("INT")) == 0) ||
          (strD.CompareNoCase(_T("DINT")) == 0) ||
          (strD.CompareNoCase(_T("LINT")) == 0) ||
          (strD.CompareNoCase(_T("REAL")) == 0) ||
          (strD.CompareNoCase(_T("LREAL")) == 0) ||
          (strD.CompareNoCase(_T("TIME")) == 0) ||
          (strD.CompareNoCase(_T("TIME_OF_DAY")) == 0) ||
          (strD.CompareNoCase(_T("DATE_AND_TIME")) == 0) ||
          (strD.CompareNoCase(_T("DT")) == 0) ||
          (strD.CompareNoCase(_T("DATE")) == 0) ||
          (strD.CompareNoCase(_T("TOD")) == 0) ||
          (strD.CompareNoCase(_T("STRING")) == 0) ||
          (strD.CompareNoCase(_T("WSTRING")) == 0)) )  
    {
        return TRUE;
    }

    hr = CoCreateInstance(CLSID_FCBaseLib,
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IBL_Conversion,
                          ((void**)&pIBL_Conversion));

    if(SUCCEEDED(hr)) 
    {
        CComBSTR sValue = strV;
        CComBSTR sDatatype = strD;
        CComBSTR sFormat = _T("");
        long options = BL_USE_IEC_FORMAT;

        hr = pIBL_Conversion->CheckString(sValue, sDatatype, sFormat, options, &lResult);

        if (pIBL_Conversion != NULL)
        {
            pIBL_Conversion->Release();
        }
    }
    else
    {
        return TRUE;
    }


    if (hr == S_OK)
    {
        return true;
    }
    else if (hr == S_FALSE)
    {
       if (lResult == E_BL_ERR_SYNTAX)
        {
            strErrorMsg.LoadString(IDS_ERROR_SYNTAX);
        }
        else if (lResult == E_BL_ERR_RANGE)
        {
            strErrorMsg.LoadString(IDS_ERROR_RANGE);
        }
        else if (lResult == E_BL_ERR_FORMATERROR)
        {
            strErrorMsg.LoadString(IDS_ERROR_FORMAT);
        }
        else if (lResult == E_BL_ERR_INVALIDARG)
        {
            strErrorMsg.LoadString(IDS_ERROR_ARGUMENT);
        }
        else if (lResult == E_BL_ERR_FUN_NOT_IMPL)
        {
            strErrorMsg.LoadString(IDS_ERROR_FUNCTION);
        }
        else if (lResult == E_BL_ERR_BUFFER_TO_SMALL)
        {
            strErrorMsg.LoadString(IDS_ERROR_BUFFER);
        }
        else if (lResult == E_BL_ERR_INTERNAL)
        {
            strErrorMsg.LoadString(IDS_ERROR_INTERNAL);
        }
        else if (lResult == E_BL_ERR_LOSS_OF_PRECIS)
        {
            strErrorMsg.LoadString(IDS_ERROR_PRECISITION);
        }
        else if (lResult == E_BL_ERR_RADIX)
        {
            strErrorMsg.LoadString(IDS_ERROR_RADIX);
        }
        
        CString strMsg;
        strMsg.LoadString(IDS_ERROR_INITVAL);
        strMsg += strErrorMsg;
        AfxMessageBox(strMsg, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    else
    {
        // some other error, value not checked, do it anyway
        return TRUE;
    }
    
}



BOOL CVarSelDlg::CheckIECIdentifier(const CString& strName)
{
    // check name: it should be a valid IEC identifier
    CComBSTR sName(strName);
    HRESULT hr;
    CComQIPtr<ICEProjInfo> pProjInfo;

    if(strName.IsEmpty())
    {
        CString str;
        str.LoadString(IDS_ERROR_EMPTY_VARNAME);
        AfxMessageBox(str, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    pProjInfo = m_pPouVarSelData->GetProjManInterface();
    if (!pProjInfo)
    {
        return TRUE;
    }

    CString strErrorMsg;

    enum E_IEC_IDENTIFIER_ERROR error;
    hr = pProjInfo->CheckIECIdentifier(sName, &error);
    if (hr!=S_OK)
    {
        if (error==IDENTIFIER_SYNTAX_ERROR)
        {
            strErrorMsg.LoadString(IDS_ERROR_IDENTIFIER_SYNTAX_ERROR);
        }
        else if (error==IDENTIFIER_IS_KEYWORD)
        {
            strErrorMsg.LoadString(IDS_ERROR_IDENTIFIER_IS_KEYWORD);
        }
        else if (error==IDENTIFIER_TOO_LONG)
        {
            strErrorMsg.LoadString(IDS_ERROR_IDENTIFIER_TOO_LONG);
        }
        else
        {
            strErrorMsg.LoadString(IDS_ERROR_IDENTIFIER);
        }

        CString strMsg;
        strMsg.LoadString(IDS_ERROR_IECVAL);
        strMsg += strErrorMsg;
        AfxMessageBox(strMsg, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;
}




void CVarSelDlg::GetVarList(CList<CString, CString&>& listVars, const CString& strTypeHint)
{
    int nr = m_pLBCtrlVar->GetItemCount();
    for (int i=0; i<nr; i++)
    {
        BOOL bAdd = FALSE;
        CString str = m_pLBCtrlVar->GetItemText(i,0);
        if (!strTypeHint.IsEmpty())
        {
            CVarObject* pVarObj = NULL;
            if (m_mapVarVars.Lookup(str, pVarObj))
            {
                if (strTypeHint.CompareNoCase(pVarObj->m_strDataType)==0)
                {
                    bAdd = TRUE;
                }
            }
        }
        else
        {
            bAdd = TRUE;
        }

        if (bAdd)
        {
            listVars.AddTail(str);
        }
    }
    return;
}





LRESULT CVarSelDlg::OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

    pProjMan->ShowContextHelp(IDD_DLG_VARSEL);
    return 0;
}
