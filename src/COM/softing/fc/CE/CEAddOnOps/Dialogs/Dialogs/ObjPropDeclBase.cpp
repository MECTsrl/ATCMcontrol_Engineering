
#include "stdafx.h"
#include "dialogs.h"
#include "ObjPropDeclBase.h"
#include "ModifyFbMember.h"

#ifndef ARRAY_LEN
#define ARRAY_LEN(var) (sizeof(var)/sizeof((var)[0]))
#endif //ARRAY_LEN

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::CObjPropDeclBase
 * Description      constructor for class CObjPropDeclBase
 * @param           DIALOGTYPE DialogType
 * @param           UINT nIDTemplate
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropDeclBase::CObjPropDeclBase(DIALOGTYPE DialogType, 
                                   UINT nIDTemplate, 
                                   CPropertySheetBase *PropertySheet,
                                   int iColumnStrId) :
    CObjProp(DialogType, PropertySheet, nIDTemplate)
{
    m_bInit = FALSE;
    m_pMembers = NULL;
    m_iColumnStrId = iColumnStrId;
    InitColumnHeaders();
}

void CObjPropDeclBase::InitColumnHeaders()
{
    if(m_iColumnStrId == IDS_PROPDECL_COLHEAD_2)
    {
        m_aiCol[COL_NAME]           = 0;
        m_aiCol[COL_ALIAS]          = 1;
        m_aiCol[COL_TYPE]           = 2;
        m_aiCol[COL_INITVAL]        = 3;
        m_aiCol[COL_COMMENT]        = 4;
        m_aiCol[COL_STORAGECLASS]   = -1;    // not used
    }
    else
    {
        m_aiCol[COL_STORAGECLASS]   = 0;
        m_aiCol[COL_NAME]           = 1;
        m_aiCol[COL_ALIAS]          = 2;
        m_aiCol[COL_TYPE]           = 3;
        m_aiCol[COL_INITVAL]        = 4;
        m_aiCol[COL_COMMENT]        = 5;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::~CObjPropDeclBase
 * Description      destructor for class CObjPropDeclBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropDeclBase::~CObjPropDeclBase()
{
    int iSize, iIndex;
    iSize = m_DataArray.GetSize();
    for(iIndex=0; iIndex<iSize; iIndex++)
    {
        pDataArrayType pDataArray = m_DataArray.GetAt(iIndex);
        delete pDataArray;
    }

    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }

    if(m_FbMembers)
    {
        m_FbMembers.DeleteAllItems();
    }
}

BEGIN_MESSAGE_MAP(CObjPropDeclBase, CObjProp)
	ON_BN_CLICKED(IDC_ADDMEMBER, OnAddMember)
	ON_BN_CLICKED(IDC_MODIFYMEMBER, OnModifyMember)
	ON_BN_CLICKED(IDC_DELETEMEMBER, OnDeleteMember)
	ON_NOTIFY(NM_CLICK, IDC_MEMBERS, OnClickFbmembers)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MEMBERS, OnClickFbmembers)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropDeclBase message handlers

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::DoDataExchange(CDataExchange* pDX)
{
	CObjProp::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEMBERS, m_FbMembers);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnClickFbmembers
 * Description      -
 * @param           NMHDR* pNMHDR
 * @param           LRESULT* pResult
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::OnClickFbmembers(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(m_DialogType != IVIEWPROP)
    {
        int iSelCount = m_FbMembers.GetSelectedCount();

	    // Enable'Modify' and 'Delete' button
	    GetDlgItem(IDC_MODIFYMEMBER)->EnableWindow(iSelCount == 1);
	    GetDlgItem(IDC_DELETEMEMBER)->EnableWindow(iSelCount > 0);
    }

	*pResult = 0;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnAddMember
 * Description      handler for the add member button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::OnAddMember() 
{
	CString	strName = _T("");
	CString	strAlias = _T("");
	CString	strComment = _T("");
	CString	strType = _T("");
	CString	strInitVal = _T("");
	CString strSclass = _T("");
	int		iCount = 0;

    //OH
    CString sIndex = _T("-1"); 
    CString sRange = _T("-1"); 
    CString sHint  = _T("No");  
    CArray<CString, CString>* pDataArray;
    pDataArray = new CArray<CString, CString>;
    pDataArray->Add(sIndex);
    pDataArray->Add(sRange);
    pDataArray->Add(sHint);
    //
    
    CModifyFbMember aFbMember(m_pszSClasses);

    aFbMember.SetParent(this);

	switch(aFbMember.DoModal())
	{
	case IDOK:
		aFbMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal, strSclass);

		if(strName.IsEmpty() || strType.IsEmpty())
		{
			break;
		}

        CheckStringValue(strInitVal, strType, TRUE);

		iCount = m_FbMembers.GetItemCount();
		m_FbMembers.InsertItem(iCount, strSclass);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_NAME], strName);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_ALIAS], strAlias);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_COMMENT], strComment);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_TYPE], strType);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_INITVAL], strInitVal);
        sHint = _T("Add");
        pDataArray->SetAt(2, sHint);
        m_FbMembers.SetItemData(iCount, (DWORD)pDataArray);
		break;
	case IDCANCEL:
		break;
	default:
		;
	}
    //select first Item
    if(m_FbMembers.GetItemCount() > 0)
    {
        BOOL bSelected = m_FbMembers.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
	return;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnModifyMember
 * Description      handler for the modify member button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::OnModifyMember() 
{
	CString	strName = _T("");
	CString	strAlias = _T("");
	CString	strComment = _T("");
	CString	strType = _T("");
	CString	strInitVal = _T("");
	CString strSclassOld = _T("");
    CString strSclassNew = _T("");
    LVITEM * pItem = NULL;

    //OH
    CString sHint = _T("No");
    pDataArrayType pDataArray; 

	// Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	int iItem = m_FbMembers.GetNextSelectedItem(pos);

    if(iItem == -1)
    {
        return;
    }
    else
    {
        strSclassOld = m_FbMembers.GetItemText(iItem, 0);
        strName = m_FbMembers.GetItemText(iItem, m_aiCol[COL_NAME]);
        strAlias = m_FbMembers.GetItemText(iItem, m_aiCol[COL_ALIAS]);
        strComment = m_FbMembers.GetItemText(iItem, m_aiCol[COL_COMMENT]);
        strType = m_FbMembers.GetItemText(iItem, m_aiCol[COL_TYPE]);
        strInitVal = m_FbMembers.GetItemText(iItem, m_aiCol[COL_INITVAL]);
        pDataArray = (pDataArrayType)m_FbMembers.GetItemData(iItem);

        // SIS: strip quotation marks
        CheckStringValue(strInitVal, strType, FALSE);
        
        CModifyFbMember aFbMember(m_pszSClasses, strName, strAlias, strComment, strType, strInitVal, strSclassOld);
        m_FbMembers.SetItemText(iItem, 1, "");
        aFbMember.SetParent(this);
        switch(aFbMember.DoModal())
        {
        case IDOK:
            aFbMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal, strSclassNew);
            // SIS: add quotation marks
            CheckStringValue(strInitVal, strType, TRUE);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_STORAGECLASS], strSclassNew);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], strName);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_ALIAS], strAlias);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_COMMENT], strComment);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_TYPE], strType);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_INITVAL], strInitVal);
            //OH: Set the hint to Mod or ModStrClass
            sHint = pDataArray->GetAt(2);
            if(sHint != _T("Add"))
            {
                if(strSclassNew != strSclassOld)
                {
                    sHint  = _T("ModStrClass");
                }
                else
                {
                    sHint  = _T("Mod");
                }
                pDataArray->SetAt(2, sHint);
                m_FbMembers.SetItemData(iItem, (DWORD)pDataArray);
            }
            break;
        case IDCANCEL:
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], strName);
            break;
        default:
            ;
        }
        
        return;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnDeleteMember
 * Description      handler for the delete member button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::OnDeleteMember() 
{
	// Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	int iItem = m_FbMembers.GetNextSelectedItem(pos);

	//OH
    CString	strName = _T("");
	CString	strAlias = _T("");
	CString	strComment = _T("");
	CString	strType = _T("");
	CString	strInitVal = _T("");
	CString strSclass = _T("");
    CString sHint = _T("Del");
    pDataArrayType pDataArray;

    strSclass = m_FbMembers.GetItemText(iItem, m_aiCol[COL_STORAGECLASS]);
	strName = m_FbMembers.GetItemText(iItem, m_aiCol[COL_NAME]);
	strAlias = m_FbMembers.GetItemText(iItem, m_aiCol[COL_ALIAS]);
	strComment = m_FbMembers.GetItemText(iItem, m_aiCol[COL_COMMENT]);
	strType = m_FbMembers.GetItemText(iItem, m_aiCol[COL_TYPE]);
	strInitVal = m_FbMembers.GetItemText(iItem, m_aiCol[COL_INITVAL]);
    pDataArray = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem);

    pDataArray->SetAt(2, sHint);

    CObjPropData PropData;

    PropData.m_sStringArray.Add(strSclass);
    PropData.m_sStringArray.Add(strName);
    PropData.m_sStringArray.Add(strAlias);
    PropData.m_sStringArray.Add(strComment);
    PropData.m_sStringArray.Add(strType);
    PropData.m_sStringArray.Add(strInitVal);
    PropData.m_pDataArray = pDataArray;
    m_tObjDataList.Add(PropData);
    
    //m_FbMembers.SetSelectionMark(iItem-1);
    m_FbMembers.DeleteItem(iItem);

	return;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropDeclBase::OnInitDialog() 
{
	CObjProp::OnInitDialog();

    if(m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_ADDMEMBER)->EnableWindow(FALSE);
        GetDlgItem(IDC_DELETEMEMBER)->EnableWindow(FALSE);
        GetDlgItem(IDC_MODIFYMEMBER)->EnableWindow(FALSE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnSetActive
 * Description      is called when this property page becomed the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropDeclBase::OnSetActive() 
{
	if(!m_bInit)
	{
		CRect Rect;
		CString strHeadings;
		int iPos;
		int iCount = 0;

		m_FbMembers.GetWindowRect(&Rect);

		// Set column headers (each column header is separated with ',' 
		// in loaded string
		strHeadings.LoadString(m_iColumnStrId);
        // Count number of columns
		while((iPos = strHeadings.Find(_T(","))) != -1) 
		{
            strHeadings = strHeadings.Mid(iPos + 1);
			iCount++;
		}
        m_iColumns = iCount;
        iCount = 0;
        // Insert column header
		strHeadings.LoadString(m_iColumnStrId);
		while((iPos = strHeadings.Find(_T(","))) != -1) 
		{
			CString strItem;
			strItem = strHeadings.Left(iPos);
			m_FbMembers.InsertColumn(iCount, strItem, LVCFMT_LEFT, Rect.Width() / m_iColumns);
			strItem = strHeadings.Mid(iPos + 1);
			strHeadings = strItem;
			iCount++;
		}
		m_bInit = TRUE; //initialized

        // Fill with configured items
        if(m_pMembers)
        {
            int iCount = 0;
            HRESULT hRes = 0;

            // Get array dimension
            long lLowerBound;
            long lUpperBound;
            ::SafeArrayGetLBound(m_pMembers, 1, &lLowerBound);
            ::SafeArrayGetUBound(m_pMembers, 1, &lUpperBound);
            long arrIndex[2];

            for(long i = lLowerBound; i <= lUpperBound; i++)
            {
                BSTR sBuffer;
                CString strBuffer = _T("");
                arrIndex[0] = i;
                
                //OH: For the Data
                pDataArrayType pDataArray;
                pDataArray = new CArray<CString, CString>;

                // Insert new item to CListCtrl
	            iCount = m_FbMembers.GetItemCount();
                m_FbMembers.InsertItem(iCount, _T(""));

                // Get array dimension
                long lLowerBound;
                long lUpperBound;
                ::SafeArrayGetLBound(m_pMembers, 2, &lLowerBound);
                ::SafeArrayGetUBound(m_pMembers, 2, &lUpperBound);
                ASSERT(lLowerBound == 0);
                ASSERT(lUpperBound == m_iColumns + 2);//OH: was m_iColumns - 1

                for(int k = lLowerBound; k <= lUpperBound -3 ; k++) //OH: the last three are exeptions
                {
                    // Retrieve element value
                    arrIndex[1] = k;
                    hRes = ::SafeArrayGetElement(m_pMembers, arrIndex, (void *)&sBuffer);

                    ASSERT(SUCCEEDED(hRes));

                    if(SUCCEEDED(hRes))
                    {
                        strBuffer = sBuffer;
                        ::SysFreeString(sBuffer);
        		        m_FbMembers.SetItemText(iCount, k, strBuffer);
                    }
                }
                
                for(int j = m_iColumns; j < m_iColumns + 3 ; j++) //OH: Hint and Index and Range
                {
                    // Retrieve element value
                    arrIndex[1] = j;
                    hRes = ::SafeArrayGetElement(m_pMembers, arrIndex, (void *)&sBuffer);
                    ASSERT(SUCCEEDED(hRes));
                    if(SUCCEEDED(hRes))
                    {
                        strBuffer = sBuffer;
                        ::SysFreeString(sBuffer);
        		        pDataArray->Add(strBuffer);
                    }
                }
                m_FbMembers.SetItemData(iCount, (DWORD)pDataArray);
            }
        }
	}
    if(m_FbMembers.GetItemCount() > 0)
    {
        BOOL bSelected = m_FbMembers.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
    if (m_PropertySheet->IsReadOnly())
    {
        SetReadOnly();
    }
	return CPropertyPage::OnSetActive();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropDeclBase::OnKillActive() 
{
	int iCount = m_FbMembers.GetItemCount();
    int iCountDel = m_tObjDataList.GetSize();
	CString strName,strClass,strType,strInitVal;
    BOOL bRet = TRUE;

    m_DataArray.RemoveAll();

	try 
    {	
        if(m_pMembers)
        {
            ::SafeArrayDestroy(m_pMembers);
            m_pMembers = NULL;
        }
	
        SAFEARRAYBOUND Bound[2];
        Bound[0].lLbound = 0;
        Bound[0].cElements = iCount + iCountDel;
        Bound[1].lLbound = 0;
        Bound[1].cElements = m_iColumns + 3; //OH: was m_iColumns 

        m_pMembers = ::SafeArrayCreate(VT_BSTR, ARRAY_LEN(Bound), Bound);
        if(iCount > 0 && m_pMembers == NULL) 
        {
            return FALSE;
        }

        LONG arrIndex[ARRAY_LEN(Bound)];
        BSTR sElem = NULL;
		CString strElem = _T("");
        
        pDataArrayType pDataArray;

		//Non deleted variables list
        for(int i=0;i<iCount;i++) 
        {
            arrIndex[0] = i;

            pDataArray = (pDataArrayType)m_FbMembers.GetItemData(i);
            m_DataArray.Add(pDataArray);
            
			for(int k = 0; k < m_iColumns; k++) //OH: the last three are an exeption
			{
				strElem = m_FbMembers.GetItemText(i, k);
				arrIndex[1] = k;
				sElem = strElem.AllocSysString();
				SafeArrayPutElement(m_pMembers, arrIndex, (void *)sElem);
				SysFreeString(sElem);
			}
            for(int j = m_iColumns; j < m_iColumns + 3; j++) //OH: the last three are an exeption
            {
                strElem = pDataArray->GetAt(j - m_iColumns);
                arrIndex[1] = j;
                sElem = strElem.AllocSysString();
                SafeArrayPutElement(m_pMembers, arrIndex, (void *)sElem);
                SysFreeString(sElem);
            }
		}
        
        //Deleted variables list
        for(int iDel=0;iDel<iCountDel;iDel++) 
        {
            arrIndex[0] = iDel+iCount;

            pDataArray = m_tObjDataList[iDel].m_pDataArray;
            m_DataArray.Add(pDataArray);
            
			for(int kDel = 0; kDel < m_iColumns; kDel++) //OH: the last three are an exeption
			{
				strElem = m_tObjDataList[iDel].m_sStringArray[kDel];
				arrIndex[1] = kDel;
				sElem = strElem.AllocSysString();
				SafeArrayPutElement(m_pMembers, arrIndex, (void *)sElem);
				SysFreeString(sElem);
			}
            for(int jDel = m_iColumns; jDel < m_iColumns + 3; jDel++) //OH: the last three are an exeption
            {
                strElem = pDataArray->GetAt(jDel - m_iColumns);
                arrIndex[1] = jDel;
                sElem = strElem.AllocSysString();
                SafeArrayPutElement(m_pMembers, arrIndex, (void *)sElem);
                SysFreeString(sElem);
            }
		}
    }

	catch(CMemoryException *e)
    {
		e->Delete();
        if(m_pMembers)
        {
            ::SafeArrayDestroy(m_pMembers);
            m_pMembers = NULL;
        }

        bRet = FALSE;
	}

	return CPropertyPage::OnKillActive();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::CObjPropData::operator =
 * Description      assignment operation
 * @param           CObjPropData & other
 * @return          CObjPropDeclBase::CObjPropData &
 * @exception       -
 * @see             -
*/
CObjPropDeclBase::CObjPropData & CObjPropDeclBase::CObjPropData::operator = (CObjPropData & other)
{
    if(this == &other)
    {
        return *this;
    }

    int iCount = other.m_sStringArray.GetSize();
    for (int i = 0; i< iCount; i++)
    {
        m_sStringArray.SetAtGrow(i, other.m_sStringArray[i]);
    }
    m_pDataArray = other.m_pDataArray;

	return *this; 
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDeclBase::DoesVarNameExist
 * Description      checks whether a variable already exists
 * @param           CString strVarName
 * @param           int i
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropDeclBase::DoesVarNameExist(CString strVarName, int i)
{
    //BOOL Res;
    CString strVarNameFromItem;
    if(m_FbMembers)
    {
        int iCount = m_FbMembers.GetItemCount();
        for(int iItem=0; iItem<iCount; iItem++) 
        {
            strVarNameFromItem = m_FbMembers.GetItemText(iItem, i);
            if(strVarName.CompareNoCase((LPCTSTR) strVarNameFromItem) == 0)
            {
                CString errorMsg;
                errorMsg.LoadString(IDS_VAR_EXISTS);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                return FALSE; //it does exists already
            }
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * check init value.
 *
 *  check quotation marks for string types.
 *  in case of output the correct quotation marks are inserted
 *  in case of input quotation marks are deleted
 *
 * @param           [in] rstrValue: value of variable
 * @param           [in] crstrType: type of variable
 * @param           [in] bOutput: is to be written into file?
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDeclBase::CheckStringValue(CString& rstrValue, const CString& crstrType, BOOL bOutput)
{
    BOOL    bString = FALSE;
    BOOL    bChanged = FALSE;
    TCHAR   cQuotChar;
    TCHAR   cWrongQuotChar;

    // get string type and quotation characters
    if(crstrType.CompareNoCase(_T("STRING")) == 0)
    {
        cQuotChar = _T('\'');
        cWrongQuotChar = _T('\"');
        bString = TRUE;
    }
    else if(crstrType.CompareNoCase(_T("WSTRING")) == 0)
    {
        cQuotChar = _T('\"');
        cWrongQuotChar = _T('\'');
        bString = TRUE;
    }

    if(bString)
    {
        CString strInitVal(rstrValue);

        if(bOutput)
        {
            // I assume that no quotation marks are present
            strInitVal = cQuotChar + strInitVal + cQuotChar;
            bChanged = TRUE;
        }
        else
        {
            // replace quotation marks at the beginning and at the end
            if(strInitVal[0] == cQuotChar || strInitVal[0] == cWrongQuotChar)
            {
                CString strTmp(strInitVal);
                strInitVal = strTmp.Mid(1);
                bChanged = TRUE;
            }
            int iLastChar = strInitVal.GetLength() - 1;
            // replace quotation marks at the beginning and at the end
            if(strInitVal[iLastChar] == cQuotChar || strInitVal[iLastChar] == cWrongQuotChar)
            {
                CString strTmp(strInitVal);
                strInitVal = strTmp.Left(iLastChar);
                bChanged = TRUE;
            }
        }
        if(bChanged)
        {
            rstrValue = strInitVal;
        }
    }
}



/*
    // check first character
    if(strInitVal[0] != cQuotChar)
    {
        // check for correct quote and replace if wrong
        if(strInitVal[0] == cWrongQuotChar)
        {
            strInitVal.SetAt(0, cQuotChar);
        }
        // else prepend correct quote
        else
        {
            strInitVal = cQuotChar + strInitVal;
            ++iLength;
        }
        bChanged = TRUE;
    }
    if(strInitVal[iLength-1] != cQuotChar)
    {
        if(strInitVal[iLength-1] == cWrongQuotChar)
        {
            strInitVal.SetAt(iLength-1, cQuotChar);
        }
        else
        {
            strInitVal += cQuotChar;
        }
        bChanged = TRUE;
    }
*/

