/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBListCtrl.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FBListCtrl.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBListCtrl.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "afxadv.h"
#include "CELibrarian.h"
#include "FBListCtrl.h"
#include "cel_names.h" 
#include "CELibrarianDoc.h"
#include "CELibrary.h" 
#include "CELibrarianView.h"
#include "FBStextDlg.h"
#include "utilif.h"
#include "CETypeId.h"
#include "CEFile.h" 
#include "CEPassWordDlg.h"
#include "DragItem.h"
#include "ObjConvert.h"
#include "KadManager.h"
#include "KadManager_i.c"
#include "DragDropHelper\LinkFile.h"
#include "DragDropHelper\PouVarObject.h"
#include "fc_tools\fc_fileIO.h"
//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////
/**
 *	constructor
 *
 * @param view		pointer to view class
 * @exception   
 * @see         
 */
CFBListCtrl::CFBListCtrl(CELibrarianView* view)
{
	m_pview = view;
	//
	// sorting state
	//
	m_SortState.column = -1;
	m_SortState.ascending = true;
	m_uiClipFormatXML = RegisterClipboardFormat(CB_FORMAT_4CXML);
	m_uiClipFormat = RegisterClipboardFormat(CELIB_CLIP_FORMAT);

    //Get the KADInfo
    CComPtr<IUnknown>   pKadManager;

    HRESULT hr = CoCreateInstance(CLSID_KadManagerComObj,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_IUnknown,
                                  (LPVOID *)&pKadManager);
    if(!pKadManager)
    {
        return;
    }
    m_pKADInfo = new CKADInfo;
    m_pKADInfo->Load(pKadManager);
}

CFBListCtrl::~CFBListCtrl()
{
    if(m_pKADInfo)
    {
        delete m_pKADInfo;
    }
}

BEGIN_MESSAGE_MAP(CFBListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFBListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDR_CL_FBCONTEXT_VIEW, OnCeFbcontextView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(IDR_CL_FBCONTEXT_SHOWHELP, OnClFbcontextShowhelp)
	ON_UPDATE_COMMAND_UI(IDR_CL_FBCONTEXT_SHOWHELP, OnUpdateClFbcontextShowhelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// Implementierung

/////////////////////////////////////////////////////////////////////
/**
 * initialize the 3 columns of the control: Name, Type, File
 *
 * @return		void 
 * @exception
 * @see      
 */
void CFBListCtrl::InitColumns()
{	
	CString name,type,targetName;

	name.LoadString(IDS_CE_FBNAME);
	type.LoadString(IDS_CE_FBTYPE);
	targetName.LoadString(IDS_CE_FBFILE);
	InsertColumn(0,name,LVCFMT_LEFT,150);
	InsertColumn(1,type,LVCFMT_LEFT,150,1);
	InsertColumn(2,targetName,LVCFMT_LEFT,300,2);
}

/////////////////////////////////////////////////////////////////////
/**
 * refresh the contents of the control with the contents of the
 * CELibrary
 *
 * @return 0-9  void 
 * @exception   
 * @see         
 */
void CFBListCtrl::Refresh()
{
	CELibrary* pCELibrary = GetCELibrary();
	CString name,type,fileNames;
	TCHAR pname[512], ptype[512];
	DeleteAllItems();
	int iIndex=-1;

	for (int ii = 0;ii< pCELibrary->GetPOUCount();ii++)
	{
		pCELibrary->GetPOU(ii,pname,ptype);
		name = pname;
		type = ptype;
		//
		// don't show pou "."
		//
		if ( name == "." ) continue;
		
		iIndex++;
		//
		// get bitmap : using the KADHelper
		//
        int iID, iDummy;
        BOOL bRes = m_pKADInfo->GetImageIdsForType(type, "", iID, iDummy);
        if(!bRes)
        {
            iID = 0;
        }
		AddItem(iIndex,0,(LPCTSTR) name,iID);
		AddItem(iIndex,1,(LPCTSTR) type);
    	//
		// target names //OH: Now it's still file name 
		//
		fileNames="";
		for (int jj=0;jj< pCELibrary->GetPOUFileCount(name);jj++)
		{
			fileNames = fileNames + pCELibrary->GetPOUFileName(name, jj) + " ; ";
		}
		AddItem(iIndex,2,(LPCTSTR) fileNames);
	}
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the document class of CELibrarian
 *
 * @return		pointer to document class CELibrarianDoc 
 * @exception
 * @see
 */
CELibrarianDoc* CFBListCtrl::GetDocument()
{
	return m_pview->GetDocument();	
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the view class of CELibrarian
 *
 * @return		pointer to view class CELibrarianView 
 * @exception
 * @see
 */
CELibrarianView* CFBListCtrl::GetView()
{
	return m_pview;
}

/////////////////////////////////////////////////////////////////////
/**
 * Get pointer to the CELibrary
 * @return		pointer to CELibrary 
 * @exception   
 * @see         
 */
CELibrary* CFBListCtrl::GetCELibrary()
{
	return GetDocument()->GetCELibrary();
}

/////////////////////////////////////////////////////////////////////
/**
 * add an item to the library and if successful to the control
 *
 * @param name			name of item
 * @param tpye			type of item
 * @param fileName		fileName of item
 * @param fullFilePath	full file name of item
 * @return				1 ok, item added;
 *						0 not ok, item can't be added
 * @exception   
 * @see        
 */
int CFBListCtrl::AddFB(const CString & name, const CString type, 
					   const CStringArray& fullFilePaths )
{	
	//
	// check parameters
	//
	if ( name == "" )
	{
		AfxMessageBox( IDS_CE_ERR_NONAME, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	} 
	else if ( type == "" )
	{
		AfxMessageBox( IDS_CE_ERR_NOTYPE, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	else if ( fullFilePaths.GetSize() == 0)
	{
		AfxMessageBox( IDS_CE_ERR_NOFILE, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
    else //OH: This wasn't here
    {
        return 1;
    }
}

/////////////////////////////////////////////////////////////////////
/**
 * add an item (by link files) to the library and if successful to the control
 *
 * @param name			name of item
 * @param tpye			type of item
 * @param fileName		fileName of item
 * @param fullFilePaths	Array of link files
 * @return				1 ok, item added;
 *						0 not ok, item can't be added
 * @exception   
 * @see        
 */
int CFBListCtrl::AddFBLink(const CString & name, const CString type, 
					   const CStringArray& fullFilePaths )
{	
	//
	// check password
	//
	if ( DoPassWordDlg() == FALSE) return 0;

	int rv = 0;
	//
	// check parameters
	//
	if ( name == "" )
	{
		AfxMessageBox( IDS_CE_ERR_NONAME, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	} 
	else if ( fullFilePaths.GetSize() == 0)
	{
		AfxMessageBox( IDS_CE_ERR_NOFILE, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

    else //OH: This wasn't here
    {
        return 1;
    }
}


int CFBListCtrl::AddAddFile(const CStringArray& fullFilePaths)
{
    //int numFiles;

    // check password
	if ( DoPassWordDlg() == FALSE) return 0;

	int rv = 0;
	
    if ( fullFilePaths.GetSize() == 0)
	{
		AfxMessageBox( IDS_CE_ERR_NOFILE, MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
    else //OH: This wasn't here
    {
        return 1;
    }
}


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFBListCtrl

/////////////////////////////////////////////////////////////////////
/**
 * action to take place on column click: sort items 
 *
 * @param pNMHDR
 * @param pResult
 * @return void
 * @exception
 * @see
 */
void CFBListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//
	// sort by clicked column
	//
	int column = pNMListView->iSubItem;
	//SortItems((PFNLVCOMPARE)CompareItem,66);
	
	if (m_SortState.column == -1)
	{
		m_SortState.column = column;
	}
	else if (m_SortState.column == column)
	{
		m_SortState.ascending = ! m_SortState.ascending;
	}
	else
	{
		m_SortState.column = column;
		m_SortState.ascending = true;
	}

	SortItems(column, m_SortState.ascending);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * not used
 * @param
 * @return
 * @exception
 * @see
 */
int CALLBACK CFBListCtrl::CompareItem(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
	//don't know what to do here
	lparam1 = lparam1;
	lparam2 = lparam2;
	lparamSort = lparamSort;
	return 1;
}

/////////////////////////////////////////////////////////////////////
/**
 * action to take place on right mouse click on item:
 * show context menu
 *
 * @param pNMHDR
 * @param pResult
 * @return void
 * @exception
 * @see
 */
void CFBListCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//
	// context menu 
	//
	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	int selectedRow = HitTest( curPoint, &nFlags);
	if (selectedRow >=0) DoPopupMenu(IDR_CL_FBCONTEXT);
	
	pNMHDR = pNMHDR;
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * show a popup ( context ) menu
 *
 * @param nMenuId	the menu id from resources
 * @return			void
 * @exception
 * @see
 */
void CFBListCtrl::DoPopupMenu(UINT nMenuID)
{
	CMenu popMenu;
	
	BOOL rv = popMenu.LoadMenu(nMenuID);

    if (!GetDocument()->CanShowUserHelp())
        popMenu.EnableMenuItem(IDR_CL_FBCONTEXT_SHOWHELP, MF_GRAYED);

	CPoint posMouse;
	GetCursorPos(&posMouse);
	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);
}

/////////////////////////////////////////////////////////////////////
/**
 * delete an item from library and control, if delete on context menu
 * is selected
 *
 * @return void 
 * @exception   
 * @see         
 */
void CFBListCtrl::OnDeleteSelected() 
{
	//
	// check password
	//
	if ( DoPassWordDlg() == FALSE) return ;
	
	CString message;
	// text: Do you really want do delete the selected items ?
	message.LoadString(IDS_CE_QDELETE);
	int rv = AfxMessageBox( message, MB_ICONQUESTION|MB_OKCANCEL);
	if ( rv != IDOK) return;

}

/////////////////////////////////////////////////////////////////////
/**
 * show content of first selected item 
 *
 * @return		void 
 * @exception
 * @see
 */
void CFBListCtrl::OnCeFbcontextView() 
{	
	TCHAR* pText = NULL;
    CString strFileName = _T("");
	for (int ii = 0; ii < GetItemCount();ii++)
	{
		UINT state = GetItemState(ii,LVIS_SELECTED);
		if (state == LVIS_SELECTED) 
		{
			CFBStextDlg dlg;
			CString fbName = GetItemString(ii,0);
			//try
			//{
            if(fbName.Find(_T("."), 0) == -1)
            {
				unsigned long ulFileByteSize;
				int rv=GetCELibrary()->GetPOUPublicHeaderByteSize(fbName, &ulFileByteSize);
				if ( rv != CELibrary::Err_Ok)
				{
					CString error;
					// no header information available
                    if (!GetDocument()->CanShowUserHelp())
                    {
					    error.LoadString(IDS_CE_ERR_NOHEADER);
					    dlg.m_stEdit=error;
					    dlg.DoModal();
                    }
					return;
				}
				pText = (TCHAR*) malloc(ulFileByteSize+1);
				rv=GetCELibrary()->GetPOUPublicHeaderAsBuffer(fbName, pText, ulFileByteSize);
				if ( rv != CELibrary::Err_Ok )
				{
					ErrorMessage(rv);
					return;
				}
				pText[ulFileByteSize] = '\0';
                //Write the header text in a file
                    //Set the path name and the tilte
                CString strHelp = GetDocument()->GetPathName();
                int iPos = strHelp.ReverseFind(_T('\\'));
                CString strPathName = strHelp.Left(iPos);
                strHelp = strHelp.Right(strHelp.GetLength() - iPos - 1);
                iPos = strHelp.ReverseFind(_T('.'));
                CString strTitle = strHelp.Left(iPos);
                    //Set the folder name to be created
                CString strFolderName = strPathName + _T("\\") + strTitle + _T("\\") + _T("Header");
                int iDirCreated = FC_CreateSubDirA(strFolderName);
                ASSERT(iDirCreated == 1);
                    //Set the file name to be created
                strFileName = strFolderName + _T("\\") + fbName + _T(".hdr");
                
                //Test if the file allready exists and that it has the same modificaion
                //date the correspanding lib
                
                //get the modification date of the library
                strHelp = GetDocument()->GetPathName();
                CFileStatus rStatus;
                CTime time;
                
                BOOL bStatus = CFile::GetStatus(strHelp, rStatus);
                
                if(!bStatus)
                {//the library doesn't exist
                    return;
                }
                else
                {//Get the moddate
                     time = rStatus.m_mtime;
                }
                
                bStatus = CFile::GetStatus(strFileName, rStatus);
                
                if((!bStatus)||(time != rStatus.m_mtime))
                {//file doesn't exist or it doesn't have the same moddate like the lib
                    //create it new
                    CFile fHeaderFile(strFileName, CFile::modeReadWrite | CFile::modeCreate);
                    fHeaderFile.Write(pText, ulFileByteSize + 1);
                    fHeaderFile.Close();
                    //Set the moddate to the moddate of the library
                    CFileStatus rStatusHeader;
                    fHeaderFile.GetStatus(rStatusHeader);
                    rStatusHeader.m_mtime = time;
                    CFile::SetStatus(strFileName,rStatusHeader); 
                }
            }
            else
            {
                    //Set the path name and the tilte
                CString strHelp = GetDocument()->GetPathName();
                int iPos = strHelp.ReverseFind(_T('\\'));
                CString strPathName = strHelp.Left(iPos);
                strHelp = strHelp.Right(strHelp.GetLength() - iPos - 1);
                iPos = strHelp.ReverseFind(_T('.'));
                CString strTitle = strHelp.Left(iPos);
                    //Set the folder name to be created
                CString strFolderName = strPathName + _T("\\") + strTitle + _T("\\") + _T("Header");
                    //Set the full file name
                strFileName = strFolderName + _T("\\") + fbName;
                    // get constant file
                CComBSTR sConstPou(_T("CONST"));
                CComBSTR sTargetType;
                CComBSTR sCheckOutDir(strFolderName);
                CComBSTR sFileName(fbName);
                CComBSTR sErrorDesc;
                CELibrary*  pCELibrary = GetCELibrary();
                HRESULT hr = pCELibrary->m_pILibAcc->CheckOut(sConstPou, sTargetType, sFileName,
                                                              sCheckOutDir, NULL, &sErrorDesc);
                if (hr!=S_OK)
                {
                    continue;
                }
            }
                
            //Show the file with ST editor
            GetDocument()->ShowHeader(strFileName);
            if(pText)
            {
                free(pText);
            }
		}
	}
}


/////////////////////////////////////////////////////////////////////
/**
 * show error message from CELibrary
 *
 * @param errId errorId
 * @return		void
 * @exception
 * @see
 */
void CFBListCtrl::ErrorMessage(long errId)
{
}

/////////////////////////////////////////////////////////////////////
/**
 * show last error message from CELibrary
 *
 * @param errId errorId
 * @return		void
 * @exception
 * @see
 */
void CFBListCtrl::LastErrorMessage()
{
}

void CFBListCtrl::UpdateViewsWithSelectedItem()
{
	CString fbName, fbActual;
	int found = 0;
	//
	// if exactly one item is selected, set this item in the 
	// document and update other views
	//
	for (int ii = 0; ii < GetItemCount();ii++)
	{
		UINT state = GetItemState(ii,LVIS_SELECTED);
		if (state == LVIS_SELECTED) 
		{
			if ( found )
			{
				found = 0;
				fbName = "";
				break;
			}
			else
			{
				found = 1;
				fbName = GetItemString(ii,0);
			}
		}
	}

	//
	// get actual item from document, if it differs from selected: update other views
	//
	fbActual = GetDocument()->GetActualItem();
	if ( fbActual != fbName )
	{
		GetDocument()->SetActualItem(fbName);
		GetDocument()->UpdateAllViews(GetView());
	}
}

void CFBListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	UpdateViewsWithSelectedItem();	
	*pResult = 0;
}

void CFBListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
}

void CFBListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar==VK_DOWN || nChar==VK_UP || nChar==VK_PRIOR || nChar==VK_NEXT || nChar==VK_END || nChar==VK_HOME)
	{
		UpdateViewsWithSelectedItem();
	}
	else if (nChar == VK_DELETE)
	{
		//OnDeleteSelected();
		//UpdateViewsWithSelectedItem();
	}
	else if (nChar == VK_RETURN)
	{
		OnCeFbcontextView();
	}
}

BOOL CFBListCtrl::DoPassWordDlg()
{
	return TRUE;
}

void CFBListCtrl::OnBegindragOld(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen

	CString strPath;
    CFileStatus fStatus;
	//
	*pResult = 0;
	
	//
	// get index item which is dragged
	//
	CPoint CurrPoint;
	GetCursorPos(&CurrPoint);
	ScreenToClient(&CurrPoint);
	int index = HitTest(CurrPoint);
	if (index < 0)
		return;

	//
	// construct drag item
	//
	CString itemName = GetItemString(index,0);
	CString itemType = GetItemString(index,1);
    long OHdummy = 0; //OH: this wasn't here
	CDragItem DragItem( OHdummy /*OH: was UTIL_Keyword2ClassId(itemType)*/, itemName, itemType);
    	
	CSharedFile	GlobFile;
	COleDataSource	SrcItem;
	
	try {
		GetView()->LockDrop(TRUE);
		CArchive ar(&GlobFile, CArchive::store);
		DragItem.Serialize(ar);
		ar.Close();
		SrcItem.CacheGlobalData(m_uiClipFormat, GlobFile.Detach());
		SrcItem.DoDragDrop();
		GetView()->LockDrop(FALSE);
	}
	catch(CException *e) {
		e->Delete();
		GetView()->LockDrop(FALSE);
		return;
	}
	GetView()->LockDrop(FALSE);
}

void CFBListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    CString strPath = _T("");
    CFileStatus fStatus;
	//
	*pResult = 0;

	//
	// get index item which is dragged
	//
    int index = pNMListView->iItem;
    ASSERT(index >= 0);
	if (index < 0)
    {
		return;
    }
	//
	// construct drag item
	//
	CString strItemName = GetItemString(index,0);
	CString strItemType = GetItemString(index,1);
	int itemType= 0;
	
    COleDataSource SrcItem;
    CString     strDummy = _T(".link");
    BSTR        sLink = strDummy.AllocSysString();
    BSTR        sLinkFileContent;
    BSTR        sPOU = strItemName.AllocSysString();
    CELibrary*  pCELibrary = GetCELibrary();
    HRESULT hr = pCELibrary->m_pILibAcc->GetFileAsBSTR(sPOU, NULL,sLink, &sLinkFileContent);
    if(hr != S_OK)
    {
        return;
    }
    CString strLinkFileContent = sLinkFileContent;
    ::SysFreeString(sLink);
    ::SysFreeString(sLinkFileContent);
    ::SysFreeString(sPOU);
    CLinkFile cLinkFile(strLinkFileContent);
    CPouObject PouObj;
    PouObj.InitFromLinkFile(&cLinkFile);
    strPath = PouObj.m_strName;
    CString strDaD = PouObj.GetDragAndDropString();
    BSTR sDaD = strDaD.AllocSysString();
    itemType = PouObj.GetDBObjectType();
    CXMLDragItem DragItem(strPath,itemType,sDaD);
    ::SysFreeString(sDaD);		
	CSharedFile	GlobFile;	
	try 
    {
	    GetView()->LockDrop(TRUE);
	    CArchive ar(&GlobFile, CArchive::store);
	    DragItem.Serialize(ar);
	    ar.Close();
        SrcItem.CacheGlobalData(m_uiClipFormatXML, GlobFile.Detach()); 
		SrcItem.DoDragDrop();
		GetView()->LockDrop(FALSE);
	}
	catch(CException *e) 
    {
		e->Delete();
		GetView()->LockDrop(FALSE);
		return;
	}
	GetView()->LockDrop(FALSE);
}

void CFBListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCeFbcontextView();
	
	*pResult = 0;
}

BOOL CFBListCtrl::DoSelectFB(const CString& strName)
{
	BOOL bFound=FALSE;
	
	for (int ii = 0; ii < GetItemCount();ii++)
	{
		SetItemState(ii,0,LVIS_SELECTED); 
	}

	for (ii = 0; ii < GetItemCount();ii++)
	{
		CString fbName = GetItemString(ii,0);
		if (fbName.CompareNoCase(strName) == 0)
		{
			SetItemState(ii,LVIS_SELECTED,LVIS_SELECTED);
			SetItemState(ii,LVIS_FOCUSED,LVIS_FOCUSED);
			EnsureVisible(ii,FALSE);
			bFound=TRUE;
			break;
		}
	}	

	UpdateViewsWithSelectedItem();
	return bFound;
}

BOOL CFBListCtrl::DoShowFBHeader(const CString& strName)
{
	CFBStextDlg dlg;
	CString fbName = strName;
	return TRUE;
}

void CFBListCtrl::OnClFbcontextShowhelp() 
{
	for (int ii = 0; ii < GetItemCount();ii++)
	{
		UINT state = GetItemState(ii,LVIS_SELECTED);
		if (state == LVIS_SELECTED) 
		{
			CString fbName = GetItemString(ii,0);
		    GetDocument()->ShowUserHelp(fbName);	
		}
	}
}

void CFBListCtrl::OnUpdateClFbcontextShowhelp(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(GetDocument()->CanShowUserHelp());	
}


/*
 *----------------------------------------------------------------------------*
 *  $History: FBListCtrl.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 12  *****************
 * User: Jd           Date: 15.01.02   Time: 11:40
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * Remove message box after pressing delete key and rempoved edit menu.
 * 
 * *****************  Version 11  *****************
 * User: Oh           Date: 10/12/01   Time: 10:01a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 10  *****************
 * User: Oh           Date: 9/21/01    Time: 4:01p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * Added: in OnCeFbcontextView, the editing of the constant files
 * 
 * *****************  Version 9  *****************
 * User: Oh           Date: 7/12/01    Time: 10:32a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 8  *****************
 * User: Oh           Date: 7/10/01    Time: 4:12p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 7  *****************
 * User: Oh           Date: 7/10/01    Time: 3:59p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 6  *****************
 * User: Oh           Date: 7/10/01    Time: 11:58a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 5  *****************
 * User: Jd           Date: 10.07.01   Time: 11:37
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * Removed old code from drag and drop.
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 7/06/01    Time: 9:16a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/05/01    Time: 5:27p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
