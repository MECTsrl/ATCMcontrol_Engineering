

#include "stdafx.h"
#include "resource.h"
#include "CeWatchList.h"
#include "CeWatchSheetInsVar.h"
#include "PageInsVarTree.h"
#include "PageInsVarList.h"
#include "CeWatchBackEnd.h"
#include "CeSymbol.h"
#include "CeWatchEdit.h"

#include "fc_todebug\fc_assert.h"
#include "dragitem.h"
#include <afxadv.h>
IMPLEMENT_SERIAL (CWatchExpressionDragItem, CObject, CB_FORMAT_4CWATCHEXPR_VERSION)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCeWatchList

//*****************************************************************************	
CCeWatchList::CCeWatchList()
//*****************************************************************************	
{
	m_nColumns = 0;
	m_xIndent = -1;
	m_pEditContainer = NULL;
	m_RootVar.SetName("Root");
	m_pBackEnd = NULL;
	m_pRootSymbol = NULL;	// 10.10.06 SIS

	m_pDragImage = NULL;
	m_iSrcItem = -1;

	m_pDropTarget = new CCeWatchDropTarget;
	m_uiWatchExpressionCBFormat = RegisterClipboardFormat (CB_FORMAT_4CWATCHEXPR);
}

//*****************************************************************************	
CCeWatchList::~CCeWatchList()
//*****************************************************************************	
{
	if (m_pEditContainer != NULL)
		delete m_pEditContainer;

	if (m_pDropTarget != NULL)
		delete m_pDropTarget;

	// 10.10.06 SIS >>
	if(m_pRootSymbol)
	{
		delete m_pRootSymbol;
	}
	// 10.10.06 SIS <<
	
	SetBackEnd(NULL); 
}

//*****************************************************************************	
void  CCeWatchList::SetBackEnd(CCeWatchBackEnd* pBackEnd) 
//*****************************************************************************	
{ 
	m_pBackEnd = pBackEnd;
	// 10.10.06 SIS >>
	if (m_pBackEnd != NULL)
	{
		LoadSymbols();
	}
	// 10.10.06 SIS <<

	UpdateConnections();
}

// 10.10.06 SIS >>
BOOL CCeWatchList::LoadSymbols()
{
	if(!m_pBackEnd)
	{
		return FALSE;
	}
	if(m_pRootSymbol)
	{
		delete m_pRootSymbol;
	}
	CWaitCursor wait;   // display wait cursor

	m_pRootSymbol = new CCeSymbol(true);
	if(!m_pRootSymbol->Load(m_pBackEnd->GetDebugInfo()))
	{
		delete m_pRootSymbol;
		m_pRootSymbol = NULL;
		return FALSE;
	}
	return TRUE;
}
// 10.10.06 SIS <<

BEGIN_MESSAGE_MAP(CCeWatchList, CListCtrl)
	//{{AFX_MSG_MAP(CCeWatchList)
    ON_MESSAGE(WM_CEWATCH_ENDEDIT, OnEndLabelEdit)
    ON_MESSAGE(WM_CEWATCH_UPDATECONNECTIONS, OnUpdateConnections)
	ON_WM_CHAR()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_REMOVE_ALL_VARIABLES, OnRemoveAllVariables)
	ON_COMMAND(IDM_REMOVE_SELECTED_VARIABLES, OnRemoveSelectedVariables)
	ON_COMMAND(IDM_ADD_VARIABLE_FROM_LIST, OnAddVariableFromList)
	ON_COMMAND(IDM_ADD_VARIABLE, OnAddVariable)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_COMMAND(IDM_LOAD_WATCHES, OnLoadWatches)
	ON_COMMAND(IDM_STORE_WATCHES, OnStoreWatches)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteItem)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_COMMAND(IDM_EDIT_FORMAT, OnEditFormat)
	ON_COMMAND(IDM_EDIT_NAME, OnEditName)
	ON_COMMAND(IDM_EDIT_VALUE, OnEditValue)
	ON_COMMAND(IDM_DEC_RANGE, OnDecRange)
	ON_COMMAND(IDM_INC_RANGE, OnIncRange)
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*****************************************************************************	
void CCeWatchList::Init() 
//*****************************************************************************	
{
	// create header
	CString strHeader;
	m_nColumns = 0;
	for (int ii = 0; ii < CEWATCH_NCOLUMNS; ii++)
	{
		strHeader.LoadString(IDS_HEADER_NAME_TYPICAL + ii);
		int w = GetStringWidth(strHeader);
		strHeader.LoadString(IDS_HEADER_NAME + ii);
		m_HeaderWidth[ii] = GetStringWidth(strHeader) + 4;
		w = (w > m_HeaderWidth[ii] ? w : m_HeaderWidth[ii]) + GetStringWidth("WW");
		InsertColumn(ii, strHeader, LVCFMT_LEFT, w, -1);
	
		m_nColumns++;
	}

	// attach image list
	VERIFY(m_ImageList.Create(IDB_VAR_STATE, 16, 0, RGB(255, 0, 255)));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	SetImageList(&m_ImageList, LVSIL_SMALL);

//@@@@@@@@@@@@@@@@
//	Load("d:\\4control\\bin\\demo\\IntelligentLightsSimple\\IntelligentLightsSimple.4cw");
//@@@@@@@@@@@@@@@@

	// register drop target (drag and drop from ST-editor)
	ASSERT(m_pDropTarget != NULL);
    if (m_pDropTarget != NULL)
        m_pDropTarget->Register(this);
}

//*****************************************************************************	
bool CCeWatchList::Load(LPCTSTR strFileName) 
//*****************************************************************************	
{
	DeleteAllVar();
	bool rv = false;
	ASSERT(AfxIsValidString(strFileName)); 
	ASSERT(strlen(strFileName) > 0);

	CArray<CString*, CString*> strItemData;

	if (strlen(strFileName) > 0)
	{
		CStdioFile file;
		if (file.Open (strFileName, CFile::modeRead))
		{
			CString str;
			if (file.ReadString(str))
			{
				if (str == CEWATCH_FILE_VERSION)
				{
					file.ReadString(str);
					int nCount = atoi(str);
					if ((nCount > 0) || (str == "0"))
					{
						strItemData.SetSize(nCount);
						for (int ii = 0; ii < nCount; ii++)
						{
							CString* pStr = new CString();
							file.ReadString(*pStr);
							strItemData.SetAt(ii, pStr);
						}
						rv = m_RootVar.ReadData(file);
					}
				}
                else {
                    //Version conflict.
                    CString strMessage;
                    strMessage.Format(IDS_ERROR_VERSION,strFileName,str,CEWATCH_FILE_VERSION);
                    ::AfxMessageBox(strMessage,MB_OK);
                }
			}
			file.Close();
		}
	}

	if (rv)
	{
		for (int ii = 0; ii < m_RootVar.GetChildrenCount(); ii++)
			InsertVarItem(m_RootVar.GetChild(ii), -1, &strItemData);
	}

	for (int ii = 0; ii < strItemData.GetSize(); ii++)
	{
		if (strItemData.GetAt(ii) != NULL)
			delete strItemData.GetAt(ii);
	}
	strItemData.RemoveAll();

	UpdateConnections();
	GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
	return rv;
}

//*****************************************************************************	
bool CCeWatchList::Store(LPCTSTR strFileName) 
//*****************************************************************************	
{
	// adjust different orders of the visual elements and 
	// the internal structure

	CCeWatchElement* pVar = NULL;
	int count = 0;
	for (int ii = 0; ii < GetItemCount();  ii++)
	{
		pVar = GetVar(ii);
		if (pVar->GetLevel() == 1)
		{
			m_RootVar.SetChild(count, pVar);
			count++;
		}
	}
	ASSERT(count == m_RootVar.GetChildrenCount());

// store data
	ASSERT(AfxIsValidString(strFileName)); 
	ASSERT(strlen(strFileName) > 0);
	if (strlen(strFileName) > 0)
	{
		CStdioFile file;
		CString str;
		if (file.Open (strFileName, CFile::modeCreate | CFile::modeWrite))
		{
			file.WriteString(CEWATCH_FILE_VERSION);
			file.WriteString("\n");
			
			// store list item data
			str.Format("%d\n", GetItemCount());
			file.WriteString(str);
			for (int ii = 0; ii < GetItemCount(); ii++)
			{
				str.Format("%d;", (int)IsExpanded(ii));
				str += GetWatchIndices(ii)->AsString() + "\n";
				file.WriteString(str);
			}
			
			// store watch element data
			bool rv = m_RootVar.WriteData(file);
			file.Close();
			return rv;
		}
	}
	return false;
}

// SIS: save and load in XML
bool CCeWatchList::SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument)
{
    // save expand information in watch elements
    SaveExpandInfo();

    // run over all items
    int iItemCount = GetItemCount();
    CCeDisplayElement*  pDisplayElement;
    CCeWatchElement*    pWatchElement;
	CString				strIndexRange;	// 15.12.05 SIS

	for (int iItem = 0; iItem < iItemCount;  iItem++)
	{
        pDisplayElement = GetDisplayElement(iItem);
        ASSERT(pDisplayElement);
        if(pDisplayElement)
        {
            pWatchElement = pDisplayElement->GetWatchElement();
            ASSERT(pWatchElement);
            if(pWatchElement && pWatchElement->GetLevel() == 1)
            {
                CXMLNode    tNodeNew;
                if(rtDocument.CreateNode(tNodeNew, CEWATCH_XMLTAG_VARNODE))
                {
					// 15.12.05 SIS >>
					strIndexRange = pDisplayElement->GetWatchIndices().AsString(TRUE);
                    if(pWatchElement->SaveToNode(tNodeNew, rtDocument,strIndexRange))
                    {
                        rtNode.AppendChild(tNodeNew);
                    }
					// 15.12.05 SIS <<
                }
            }
        }
    }
    return true;
}


//------------------------------------------------------------------*
/**
 * save expand info.
 *
 *  save expand info in watch items.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCeWatchList::SaveExpandInfo()
{
    // run over all items
    int iItemCount = GetItemCount();
    CCeDisplayElement*  pDisplayElement;

	for (int iItem = 0; iItem < iItemCount;  iItem++)
	{
        pDisplayElement = GetDisplayElement(iItem);
        ASSERT(pDisplayElement);
        if(pDisplayElement)
        {
            pDisplayElement->SaveExpandInfo();
        }
    }
}


bool CCeWatchList::LoadFromNode(CXMLNode& rtNode)
{
    bool bReturn(false);

	DeleteAllVar();

    bReturn = m_RootVar.LoadFromNode(rtNode, m_pRootSymbol);	// 10.10.06 SIS

    if(bReturn)
	{
		for (int ii = 0; ii < m_RootVar.GetChildrenCount(); ii++)
        {
			AddVarItem(m_RootVar.GetChild(ii));
        }
	}

	UpdateConnections();
	GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);

	return bReturn;
}



//*****************************************************************************	
int CCeWatchList::InsertVarItem(CCeWatchElement* pVar, int index/*=-1*/, CArray<CString*, CString*>* pItemData/*=NULL*/, bool bEnsureVisible /*=false*/) 
//*****************************************************************************	
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_PARAM ;
	lvItem.iSubItem = 0;

	if (index >= 0)
		lvItem.iItem = index;
	else
		lvItem.iItem = GetItemCount();

	CCeDisplayElement* pDisplayElement = new CCeDisplayElement();

	pDisplayElement->SetWatchElement(pVar);
	if (lvItem.iItem > 0)
		pDisplayElement->SetVerticalLineMask(GetDisplayElement(lvItem.iItem-1)->GetVerticalLineMask());
	

	int iIndexMin, iIndexMax;

	if (pVar->GetType().IsArray())
	{
		iIndexMin = pVar->GetType().GetArrayLowerBound();
		iIndexMax = pVar->GetType().GetArrayUpperBound();
		// 15.12.05 SIS >>
		CString strIndexRange = pVar->GetIndexRange();
		if(!strIndexRange.IsEmpty())
		{
			pDisplayElement->GetWatchIndices().Create(strIndexRange, iIndexMin, iIndexMax);
		}
		else
		{
			// #2149 27.10.04 SIS >>
			// use new define CEWATCH_STD_ARRAY_LEN
			pDisplayElement->GetWatchIndices().Create(iIndexMin, min(iIndexMax, iIndexMin + CEWATCH_STD_ARRAY_LEN - 1), iIndexMin, iIndexMax);
			// #2149 27.10.04 SIS <<
		}
		// 15.12.05 SIS <<
	}

	if (pItemData != NULL)
	{
		if (lvItem.iItem < pItemData->GetSize())
		{
			CString* pStr = pItemData->GetAt(lvItem.iItem);
			int iLen = pStr->GetLength();
			if (pStr != NULL &&  iLen > 0)
			{
				int i1 = pStr->Find(';');
				ASSERT(i1 >= 0);
				if (i1 >= 0)
				{
					CString s1, s2;
					s1 = pStr->Left(i1);
					s2 = pStr->Mid(i1+1);	

				if (s1.GetLength() > 0)
					pDisplayElement->SetExpanded(pStr->GetAt(0) != '0');

				if (s2.GetLength() > 0 && pVar->GetType().IsArray())
					pDisplayElement->GetWatchIndices().Create(s2, iIndexMin, iIndexMax);
				}
			}
		}
	}

	lvItem.lParam = (LPARAM)pDisplayElement;
	int iNewIten = InsertItem(&lvItem);
	UpdateVertLineMasks(lvItem.iItem);
	if (bEnsureVisible)
		EnsureVisible(iNewIten, FALSE);

	int count = 1;
	if (IsExpanded(lvItem.iItem))
	{
		long jj;
		if (pVar->GetType().IsArray())
		{
			CCeWatchIndices* pI = GetWatchIndices(lvItem.iItem);
			ASSERT(pI != NULL);
			long iIndex;
			while (pI->GetNextIndex(iIndex, count == 1))
			{
				jj = iIndex - pI->GetMinIndex();
				ASSERT(jj >= 0 && jj < pVar->GetChildrenCount());
				count += InsertVarItem(pVar->GetChild(jj), lvItem.iItem + count, pItemData);
			}
		}
		else
		{
			for (jj = 0; jj < pVar->GetChildrenCount(); jj++)
			{
				count += InsertVarItem(pVar->GetChild(jj), lvItem.iItem + count, pItemData);
			}
		}
	}
	return count;
}


//*****************************************************************************	
int CCeWatchList::AddVarItem(CCeWatchElement* pVar) 
//*****************************************************************************	
{
	// 10.10.06 SIS >>
	if(pVar == NULL)
	{
		return -1;
	}
	// 10.10.06 SIS <<
	LV_ITEM lvItem;
	lvItem.mask = LVIF_PARAM ;
	lvItem.iSubItem = 0;

    lvItem.iItem = GetItemCount();

	CCeDisplayElement* pDisplayElement = new CCeDisplayElement();

	pDisplayElement->SetWatchElement(pVar);
	if (lvItem.iItem > 0)
    {
		pDisplayElement->SetVerticalLineMask(GetDisplayElement(lvItem.iItem-1)->GetVerticalLineMask());
    }

	int iIndexMin, iIndexMax;

	if (pVar->GetType().IsArray())
	{
		iIndexMin = pVar->GetType().GetArrayLowerBound();
		iIndexMax = pVar->GetType().GetArrayUpperBound();
		// 15.12.05 SIS >>
		CString	strIndexRange = pVar->GetIndexRange();
		if(!strIndexRange.IsEmpty())
		{
			pDisplayElement->GetWatchIndices().Create(strIndexRange, iIndexMin, iIndexMax);
		}
		else
		{
			pDisplayElement->GetWatchIndices().Create(iIndexMin, min(iIndexMax, iIndexMin + 4), iIndexMin, iIndexMax);
		}
		// 15.12.05 SIS <<
	}

	pDisplayElement->SetExpanded(pVar->GetInitExpanded());

//	if (s2.GetLength() > 0 && pVar->GetType().IsArray())
//		pDisplayElement->GetWatchIndices().Create(s2, iIndexMin, iIndexMax);

	lvItem.lParam = (LPARAM)pDisplayElement;
	int iNewIten = InsertItem(&lvItem);
	UpdateVertLineMasks(lvItem.iItem);

	int count = 1;
	if (IsExpanded(lvItem.iItem))
	{
		long jj;
		if (pVar->GetType().IsArray())
		{
			CCeWatchIndices* pI = GetWatchIndices(lvItem.iItem);
			ASSERT(pI != NULL);
			long iIndex;
			while (pI->GetNextIndex(iIndex, count == 1))
			{
				jj = iIndex - pI->GetMinIndex();
				ASSERT(jj >= 0 && jj < pVar->GetChildrenCount());
				// 10.10.06 SIS >>
				if(jj >= pVar->GetChildrenCount())
				{
					break;
				}
				// 10.10.06 SIS <<
				count += AddVarItem(pVar->GetChild(jj));
			}
		}
		else
		{
			for (jj = 0; jj < pVar->GetChildrenCount(); jj++)
			{
				count += AddVarItem(pVar->GetChild(jj));
			}
		}
	}
	return count;
}


//*****************************************************************************	
bool CCeWatchList::MoveVarItem(
	int SrcItem, 
	int DestItem) 
// Moves item SrcItem and all child items to DestItem
//*****************************************************************************	
{
	CCeWatchElement* pVar = GetVar(SrcItem);	
	CCeWatchElement* pChildVar = NULL;
	int ii;
	ASSERT(pVar != NULL);
	if (pVar != NULL)
	{
		// save data of list elements
		unsigned iParentLevel = pVar->GetLevel();
		CArray<CCeDisplayElement*, CCeDisplayElement*> ItemDataArray;
		ItemDataArray.Add(GetDisplayElement(SrcItem));
		for (ii = SrcItem + 1; (ii < GetItemCount()) && (GetVar(ii)->GetLevel() > iParentLevel); ii++)
			ItemDataArray.Add(GetDisplayElement(ii));

		ASSERT(DestItem < SrcItem || DestItem >= SrcItem + ItemDataArray.GetSize());
		if (DestItem < SrcItem || DestItem >= SrcItem + ItemDataArray.GetSize())
		{
			// delete source items
			for (int ii = 0; ii < ItemDataArray.GetSize(); ii++)
			{
				SetItemData(SrcItem, NULL); 
				DeleteItem(SrcItem);
			}
			
			if (DestItem > SrcItem)
				DestItem -= ItemDataArray.GetSize();

			// insert new items
			LV_ITEM lvItem;
			lvItem.mask = LVIF_PARAM ;
			lvItem.iSubItem = 0;

			if (DestItem >= 0 && DestItem < GetItemCount())
				lvItem.iItem = DestItem;
			else
				lvItem.iItem = GetItemCount();
			
			for (ii = 0; ii < ItemDataArray.GetSize(); ii++)
			{
				lvItem.lParam = (LPARAM)ItemDataArray.GetAt(ii);;
				InsertItem(&lvItem);
				lvItem.iItem++;
			}

			return true;
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchList message handlers

//*****************************************************************************	
void CCeWatchList::DeleteSubTree(int iItem)
//*****************************************************************************	
{
	CCeWatchElement* pVarParent = GetVar(iItem);
	ASSERT(IsValidWatchElement(pVarParent));

	if (pVarParent != NULL)
	{
		if (IsExpanded(iItem))
		{
			unsigned iParentLevel = pVarParent->GetLevel();
			int iChildItemMax = iItem + 1;
			while (iChildItemMax < GetItemCount())
			{
				CCeWatchElement* pVarChild = GetVar(iChildItemMax);
				ASSERT(IsValidWatchElement(pVarChild));
				if (pVarChild->GetLevel() <= iParentLevel)
					break;
				SetExpanded(iChildItemMax, false);
				iChildItemMax++;
			}
			iChildItemMax--;
			for (int ii = iChildItemMax; ii > iItem; ii--)
				DeleteItem(ii);

			SetExpanded(iItem, false);
			RedrawItems(iItem, iItem);
		}
	}	
}

//*****************************************************************************	
void CCeWatchList::InsertSubTree(int iItem)
//*****************************************************************************	
{
	CCeWatchElement* pVar = GetVar(iItem);
	ASSERT(IsValidWatchElement(pVar));
	if (pVar != NULL && pVar->GetChildrenCount() > 0)
	{
		if (!IsExpanded(iItem))
		{
			int count = 0;
			long jj;
			// insert array (may be a subset)
			if (pVar->GetType().IsArray())
			{
				CCeWatchIndices* pI = GetWatchIndices(iItem);
				long iIndex;
				long lCount = pI->GetMaxIndex()-pI->GetMinIndex() + 1;
				// 10.10.06 SIS >>
				// if child count changed -> delete old children and insert new ones
				if(lCount != pVar->GetChildrenCount())
				{
					pVar->DeleteAllChildren();
					if(m_pRootSymbol)
					{
						CCeSymbol* pSymbol = m_pRootSymbol->FindChildRecursive(pVar->GetName());
						if(pSymbol && CCeWatchElement::IsVarKindVisible(pSymbol->GetType().GetVarKind()))
						{
							pVar->AddChildren(pSymbol);
						}
					}
				}
				// 10.10.06 SIS <<
				while (pI->GetNextIndex(iIndex, count == 0))
				{
					jj = iIndex - pI->GetMinIndex();
					ASSERT(jj >= 0 && jj < pVar->GetChildrenCount());
					count += InsertVarItem(pVar->GetChild(jj), iItem + 1 + count);
				}
			}
			// insert structure (all elements)
			else
			{
				for (jj = 0; jj < pVar->GetChildrenCount(); jj++)
				{
					count+= InsertVarItem(pVar->GetChild(jj), iItem + 1 + count);
				}
			}
			SetExpanded(iItem, true);
			RedrawItems(iItem, iItem);
		}
	}
}

//*****************************************************************************	
void CCeWatchList::ToggleSubTree(int iItem)
//*****************************************************************************	
{
	CCeWatchElement* pVar = GetVar(iItem);
	ASSERT(IsValidWatchElement(pVar));
	if (pVar != NULL && pVar->GetChildrenCount() > 0)
	{
		if (IsExpanded(iItem))
			DeleteSubTree(iItem);
		else
			InsertSubTree(iItem);
	}
}

//*****************************************************************************	
CCeWatchElement* CCeWatchList::GetVar(int iItem) 
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	ASSERT(IsValidDisplayElement(pDisplayElement));
	return pDisplayElement != NULL ? pDisplayElement->GetWatchElement() : NULL;
}

//*****************************************************************************	
unsigned long CCeWatchList::GetVertLineMask(int iItem) 
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	return pDisplayElement != NULL ? pDisplayElement->GetVerticalLineMask() : 0;
}

//*****************************************************************************	
CString CCeWatchList::GetFormat(int iItem)
//*****************************************************************************	
{
	CCeWatchElement* pVar = GetVar(iItem);
	return pVar != NULL ? pVar->GetFormat() : CString("");
}

//*****************************************************************************	
LONG CCeWatchList::GetExpressionHandle(int iItem)
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	return pDisplayElement != NULL ? pDisplayElement->GetExpressionHandle() : -1;
}

//*****************************************************************************	
CCeWatchIndices* CCeWatchList::GetWatchIndices(int iItem)
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	return pDisplayElement != NULL ? &(pDisplayElement->GetWatchIndices()) : NULL;
}


//*****************************************************************************	
bool CCeWatchList::IsExpanded(int iItem) 
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	return pDisplayElement != NULL ? pDisplayElement->IsExpanded() : false;
}

//*****************************************************************************	
void CCeWatchList::SetExpanded(int iItem, bool bExpanded) 
//*****************************************************************************	
{
	CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
	ASSERT(pDisplayElement != NULL);
	if (pDisplayElement != NULL)
		pDisplayElement->SetExpanded(bExpanded);
}

//*****************************************************************************	
CCeDisplayElement* CCeWatchList::GetDisplayElement(int iItem) 
//*****************************************************************************	
{
	ASSERT(iItem >= 0 && iItem < GetItemCount());
	if (iItem >= 0 && iItem < GetItemCount())
		return (CCeDisplayElement*)GetItemData(iItem);
	else
		return NULL;
}

//*****************************************************************************	
CCeDisplayElement* CCeWatchList::GetSelectedDisplayElement() 
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
		return GetDisplayElement(iItem);
	else
		return NULL;
}

//*****************************************************************************	
void CCeWatchList::GetSelectedDisplayElements (CArray<CCeDisplayElement *, CCeDisplayElement *> &displays)
//*****************************************************************************	
{
    POSITION pPosition;

    displays.RemoveAll ();

    pPosition = GetFirstSelectedItemPosition ();
    while (pPosition != NULL)
    {
        int iItem;
        CCeDisplayElement *pDisplay;

        iItem = GetNextSelectedItem (pPosition);

        pDisplay = GetDisplayElement (iItem);
        if (pDisplay != NULL)
            displays.Add (GetDisplayElement (iItem));
    };
}

//*****************************************************************************	
CCeWatchElement* CCeWatchList::GetSelectedVar() 
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
		return GetVar(iItem);
	else
		return NULL;
}

//*****************************************************************************	
int CCeWatchList::GetSelectedItem() 
//*****************************************************************************	
{
	if ( GetSelectedCount() == 1)
	{
		POSITION pos =  GetFirstSelectedItemPosition();
		ASSERT(pos != NULL);
		return  GetNextSelectedItem(pos);
	}
	return -1;
}

// 15.12.05 SIS >>
//*****************************************************************************	
int CCeWatchList::GetNextInsertPos(int iItemStart) 
//*****************************************************************************	
{
	if(iItemStart == -1)
	{
		return iItemStart;	// insert as last
	}

	int iItemCount = GetItemCount();
	CCeDisplayElement*	pDisplayElement;
	CCeWatchElement*	pWatchElement;
	for(int iItem = iItemStart; iItem < iItemCount; ++iItem)
	{
		pDisplayElement = GetDisplayElement(iItem);
		pWatchElement = pDisplayElement->GetWatchElement();
		if(pWatchElement && pWatchElement->GetLevel() <= 1)
		{
			break;
		}
	}
	return iItem;
}
// 15.12.05 SIS <<

//*****************************************************************************	
bool CCeWatchList::DeleteVar(int iItem)
//*****************************************************************************	
{
	CCeWatchElement* pVar = GetVar(iItem);
	ASSERT(pVar != NULL);
	if (pVar != NULL)
	{
		CCeWatchElement* pVarParent = pVar->GetParent();
		ASSERT(pVarParent != NULL);
		if (pVarParent != NULL)
		{
			// only objects wich are directly below the root object 
			// can be deleted
			if (pVarParent->GetParent() == NULL)
			{
				DeleteSubTree(iItem);
				DeleteItem(iItem);
				pVarParent->DeleteChild(pVar);
                if(GetItemCount() > iItem)
                {
                    SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
                }
                else if(iItem > 0)
                {
                    SetItemState(iItem-1, LVIS_SELECTED, LVIS_SELECTED);
                }

				return true;
			}
		}
	}
	return false;
}


//*****************************************************************************	
bool CCeWatchList::DeleteSelectedVar()
//*****************************************************************************	
{
	bool rv = false;
	CCeWatchElement* pVar = NULL;
    CList<int, int> tDeleteList;
    int iItem;

    // collect all selected items in reverse order
    POSITION    pos = GetFirstSelectedItemPosition();
    while(pos)
    {
        iItem = GetNextSelectedItem(pos);
        tDeleteList.AddHead(iItem);
    }

    // delete items
    POSITION    listpos = tDeleteList.GetHeadPosition();
    while(listpos)
    {
        iItem = tDeleteList.GetNext(listpos);
        rv |= DeleteVar(iItem);
    }

	GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
	return rv;
}

//*****************************************************************************	
bool CCeWatchList::DeleteAllVar()
//*****************************************************************************	
{
	DeleteAllItems();
	bool rv = m_RootVar.DeleteAllChildren();
	GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
	return rv;
}


//*****************************************************************************	
void CCeWatchList::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
//*****************************************************************************	
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iItem >= 0)
	{
		bool bUpdateConn = false;		
		CCeWatchElement* pVar = GetVar(pNMListView->iItem);
		if (pVar != NULL)
		{
			if (pVar->GetChildrenCount() > 0)
			{
				// Hit Test of the "+" or "-" chararcter
				CRect LabelRect;
				GetItemRect(pNMListView->iItem, LabelRect, LVIR_LABEL); 

				int xmin = LabelRect.left + (pVar->GetLevel()-1) * m_xIndent;
				int xmax = xmin + m_xIndent;
				if (pNMListView->ptAction.x >= xmin && pNMListView->ptAction.x <= xmax)
				{
					ToggleSubTree(pNMListView->iItem);
					bUpdateConn = true;
				}
			}
		}

		// not very nice, but the only way I found to check 
		// for scrolling caused by a click into the last line
		if (pNMListView->iItem == GetTopIndex() + GetCountPerPage())
			bUpdateConn = true;

		if (bUpdateConn)
			UpdateConnections();
	}
	*pResult = 0;
}

//*****************************************************************************	
void CCeWatchList::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
//*****************************************************************************	
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//if (pNMListView->iItem >= 0 && (pNMListView->uNewState & LVIS_SELECTED))
		GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
	//else
	//	GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
	*pResult = 0;
}

//*****************************************************************************	
bool CCeWatchList::IncRange(bool bDec) 
//*****************************************************************************	
{
	if (CheckIncRange(bDec))
	{
		int iItem = GetSelectedItem();
		GetWatchIndices(iItem)->Inc(bDec);
		RedrawItems(iItem, iItem);
		if (IsExpanded(iItem))
		{
			DeleteSubTree(iItem);
			InsertSubTree(iItem);
			UpdateConnections();
		}
		return true;
	}
	return false;
}

//*****************************************************************************	
bool CCeWatchList::ExpandTree(bool bCollapse)
//*****************************************************************************	
{
	int iItem = GetSelectedItem();		
	if (iItem >= 0)
	{
		CCeWatchElement* pVar = GetVar(iItem);
		ASSERT(pVar != NULL);
		if (pVar != NULL && pVar->GetChildrenCount() > 0)
		{
			if (( IsExpanded(iItem) &&  bCollapse) ||
				(!IsExpanded(iItem) && !bCollapse))
			{
				ToggleSubTree(iItem);
				UpdateConnections();
				return true;
			}
		}
	}
	return false;
}


//*****************************************************************************	
void CCeWatchList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
//*****************************************************************************	
{
	if (nChar == '*' || nChar == '_')
	{
		if (IncRange(nChar == '_'))
			return;
	}
	else if (nChar == '+' || nChar == '-')
	{
		if (ExpandTree(nChar == '-'))
			return;
	}

	CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

//*****************************************************************************	
void CCeWatchList::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
//*****************************************************************************	
{
	switch (nChar)
	{
	case VK_DELETE:
		OnRemoveSelectedVariables();
		UpdateConnections();
		break;
	case VK_INSERT:
		OnAddVariable();
		UpdateConnections();
		break;
	case VK_NEXT:
	case VK_PRIOR:
	case VK_UP:
	case VK_DOWN:
		UpdateConnections();
		break;
	}
	
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}


//*****************************************************************************	
void CCeWatchList::OnContextMenu(CWnd* pWnd, CPoint point) 
//*****************************************************************************	
{
	CMenu menu;
	if (!menu.LoadMenu (IDR_WATCH_POPUP))
		return ;

	// SHIFT_F10 15.12.04 SIS >>
	// use center point if not specified (SHIFT + F10)
	if(point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.CenterPoint();
	}
	// SHIFT_F10 15.12.04 SIS <<
		
	CMenu *pPopup = menu.GetSubMenu (0);
	ASSERT (pPopup != NULL);

	int iSelectedItems = GetSelectedCount();
	// 15.12.05 SIS >>
	// bool bInsert = CheckVariableInsert();

	pPopup->EnableMenuItem (IDM_ADD_VARIABLE, MF_ENABLED);
	pPopup->EnableMenuItem (IDM_ADD_VARIABLE_FROM_LIST, MF_ENABLED);
	// 15.12.05 SIS <<
	pPopup->EnableMenuItem (IDM_REMOVE_SELECTED_VARIABLES, CheckVariableDelete() ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_REMOVE_ALL_VARIABLES, GetItemCount() > 0 ? MF_ENABLED : MF_GRAYED);

	pPopup->EnableMenuItem (IDM_EDIT_NAME,    CheckNameEdit()   ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_EDIT_VALUE,   CheckValueEdit()  ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_EDIT_FORMAT,  CheckFormatEdit() ? MF_ENABLED : MF_GRAYED);

	pPopup->EnableMenuItem (IDM_INC_RANGE,  CheckIncRange(false) ? MF_ENABLED : MF_GRAYED);	
	pPopup->EnableMenuItem (IDM_DEC_RANGE,  CheckIncRange(true) ? MF_ENABLED : MF_GRAYED);	

	pPopup->EnableMenuItem (IDM_LOAD_WATCHES, MF_ENABLED);
	pPopup->EnableMenuItem (IDM_STORE_WATCHES, MF_ENABLED);

	::TrackPopupMenu (pPopup->m_hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, m_hWnd, NULL);
}

//*****************************************************************************	
void CCeWatchList::OnRemoveAllVariables() 
//*****************************************************************************	
{
	DeleteAllVar();
}

//*****************************************************************************	
void CCeWatchList::OnRemoveSelectedVariables() 
//*****************************************************************************	
{
	DeleteSelectedVar();
}


//*****************************************************************************	
void CCeWatchList::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
//*****************************************************************************	
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	*pResult = 1;

	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

	BeginLabelEdit(pDispInfo->item.iItem, pt.x);
}


//*****************************************************************************	
bool CCeWatchList::GetEditRect(int iItem, int iEditType, CRect& rect)
//*****************************************************************************	
{
	if (iItem >= 0)
	{
		GetItemRect(iItem, rect, LVIR_LABEL); 
		rect.bottom--;

		if (iEditType == CEWATCH_EDITTYPE_NAME)
		{
			const CCeWatchElement* pVar = GetVar(iItem);
			ASSERT(pVar != NULL);
			if (pVar != NULL)
				rect.left += (int)pVar->GetLevel() * m_xIndent;
			else
				return false;
		}
		else if (iEditType == CEWATCH_EDITTYPE_VALUE)
		{
			rect.left  = rect.right + GetColumnWidth(1);
			rect.right = rect.left  + GetColumnWidth(2);
		}
		else if (iEditType == CEWATCH_EDITTYPE_FORMAT)
		{
			rect.left  = rect.right + GetColumnWidth(1) + GetColumnWidth(2) + GetColumnWidth(3) + GetColumnWidth(4);
			rect.right = rect.left  + GetColumnWidth(5);
		}
		else
		{
			return false;
		}
		rect.left++;
		return true;
	}
	return false;
}


//*****************************************************************************	
void CCeWatchList::BeginLabelEdit(int iItem, int xPos)
//*****************************************************************************	
{
	if (m_pBackEnd != NULL && iItem >= 0)
	{
		ASSERT(m_pEditContainer == NULL);
		CCeDisplayElement* pDisplay = GetDisplayElement(iItem);
		CCeWatchElement* pVar = GetVar(iItem);
		ASSERT(pVar != NULL);
		if (pVar != NULL)
		{
			CRect EditRectName, EditRectValue, EditRectFormat;
			GetEditRect(iItem, CEWATCH_EDITTYPE_NAME,   EditRectName);
			GetEditRect(iItem, CEWATCH_EDITTYPE_VALUE,  EditRectValue);
			GetEditRect(iItem, CEWATCH_EDITTYPE_FORMAT, EditRectFormat);

			// edit name of top level elements or range of arrays
			if (xPos > EditRectName.left && xPos < EditRectName.right)
			{
				// 10.10.06 SIS >>
				// check, if type has changed
				if(m_pRootSymbol)
				{
					CCeSymbol* pSymbol = m_pRootSymbol->FindChildRecursive(pVar->GetName());
					if(pSymbol)
					{
						if(pVar->GetType().GetName().CompareNoCase(pSymbol->GetType().GetName()) != 0)
						{
							pVar->SetType(pSymbol->GetType());
							if(pVar->GetType().IsArray())
							{
								pVar->CheckArrayIndices();
							}
							else
							{
								CCeWatchElement* pNew = new CCeWatchElement(pSymbol);
								InsertVariable(pNew, iItem);
								DeleteVar(iItem + 1);
								UpdateConnections();
							}
							UpdateConnections();
							return;		// type has changed, do not edit
						}
					}
				}
				// 10.10.06 SIS <<
				if (pVar->GetLevel() == 1 || pVar->GetType().IsArray())
				{
					m_pEditContainer = new CCeWatchEditContainer(iItem, CEWATCH_EDITTYPE_NAME, GetDisplayElement(iItem));
					m_pEditContainer->Create(EditRectName, this, 1);
				}
			}
			// change value of variable
			else if (xPos > EditRectValue.left && xPos < EditRectValue.right)
			{
				if (pVar->GetType().IsSimpleType() && pVar->IsConnected())
				{
					m_pEditContainer = new CCeWatchEditContainer(iItem, CEWATCH_EDITTYPE_VALUE, GetDisplayElement(iItem));
					m_pEditContainer->Create(EditRectValue, this, 1);
				}
			}
			// change format of variable
			else if (xPos > EditRectFormat.left && xPos < EditRectFormat.right)
			{
				if (pVar->GetType().IsSimpleType())
				{
					CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
					ASSERT(pDisplayElement != NULL);
					if (pDisplayElement != NULL)
					{
						m_pEditContainer = new CCeWatchEditContainer(iItem, CEWATCH_EDITTYPE_FORMAT, GetDisplayElement(iItem));
						m_pEditContainer->Create(EditRectFormat, this, 1);
					}
				}
			}
		}
	}
}

//*****************************************************************************	
void CCeWatchList::EndLabelEdit(WPARAM wParam /*=IDOK*/) 
//*****************************************************************************	
{
	if (m_pBackEnd != NULL)
	{
		if (m_pEditContainer != NULL)
		{
			if (m_pEditContainer->IsVisible())
			{
				if (wParam == IDOK)
				{
					if (m_pEditContainer->GetEditType() == CEWATCH_EDITTYPE_NAME)
					{
						if (!EndNameEdit())
							return;
					}
					else if (m_pEditContainer->GetEditType() == CEWATCH_EDITTYPE_VALUE)	
					{
						if (!EndValueEdit())
							return;
					}
					else if (m_pEditContainer->GetEditType() == CEWATCH_EDITTYPE_FORMAT)	
					{
						if (!EndFormatEdit())
							return;
					}

					UpdateConnections();
				}
			}
			delete m_pEditContainer;
			m_pEditContainer = NULL;
			SetFocus();
		}
	}
}

//*****************************************************************************	
bool CCeWatchList::EndNameEdit() 
//*****************************************************************************	
{
	ASSERT(m_pEditContainer != NULL);
	ASSERT(m_pBackEnd != NULL);

	int iEditItem = m_pEditContainer->GetEditItem();
	ASSERT(iEditItem >= 0 && iEditItem < GetItemCount());
	if (iEditItem >= 0 && iEditItem < GetItemCount())
	{
		CCeWatchElement* pVar = GetVar(iEditItem);	
		ASSERT(pVar != NULL);
		if (pVar != NULL && m_pBackEnd != NULL)
		{
			CString str;
			m_pEditContainer->GetText(str);
			int iSep = str.Find(',');
			CString strName, strRange;
			if (iSep > 0)
			{
				strName = str.Left(iSep);
				strRange = str.Mid(iSep + 1);
			}
			else
			{
				strName = str;
			}
			strName.TrimLeft();
			strName.TrimRight();

			if (pVar->GetLevel() == 1)
			{
				// 10.10.06 SIS >>
				if (pVar->GetType().IsArray())
				{
					if (strRange != GetWatchIndices(iEditItem)->AsString())
					{
						if (!GetWatchIndices(iEditItem)->Create(strRange, pVar->GetType().GetArrayLowerBound(), pVar->GetType().GetArrayUpperBound()))
						{
							m_pEditContainer->IgnoreFocus(true);
							ErrorMsg(IDS_ERROR_ARRAYINDEX);
							m_pEditContainer->IgnoreFocus(false);
							return true;
						}
					}
				}
				BOOL bInserted = FALSE;
				if(m_pRootSymbol && m_pBackEnd)
				{
					CString			strText;
					CCeWatchType	type;
					if(m_pBackEnd->GetTypeAndText(strName, type, strText))
					{
						bool bExpanded = IsExpanded(iEditItem);
						CCeSymbol* pSymbol = m_pRootSymbol->FindChildRecursive(strText);
						if(pSymbol && CCeWatchElement::IsVarKindVisible(pSymbol->GetType().GetVarKind()))
						{
							CCeWatchElement* pNew = new CCeWatchElement(pSymbol);
							pNew->SetIndexRange(strRange);
							pNew->SetName(strName);
							InsertVariable(pNew, iEditItem);
							bInserted = TRUE;
							DeleteVar(iEditItem + 1);
							if(bExpanded)
							{
								InsertSubTree(iEditItem);
							}
							UpdateConnections();
						}
					}
				}

				if(!bInserted)
				{
					// 10.10.06 SIS <<
					CCeWatchElement *pInsertedElement=NULL;

					InsertExpression(str, pInsertedElement, iEditItem);
					if (pInsertedElement != NULL)
					{
						pVar = pInsertedElement;
					}
					DeleteVar(iEditItem + 1);
				}
			}
			if (pVar->GetType().IsArray())
			{
				if (strRange != GetWatchIndices(iEditItem)->AsString())
				{
					if (GetWatchIndices(iEditItem)->Create(strRange, pVar->GetType().GetArrayLowerBound(), pVar->GetType().GetArrayUpperBound()))
					{
						if (IsExpanded(iEditItem))
						{
							DeleteSubTree(iEditItem);
							InsertSubTree(iEditItem);
						}
					}
					else
					{
						m_pEditContainer->IgnoreFocus(true);
						ErrorMsg(IDS_ERROR_ARRAYINDEX);
						m_pEditContainer->IgnoreFocus(false);
						return true;	// 10.10.06 SIS: return true to close edit control
					}
				}
			}
		}
	}
	return true;
}

//*****************************************************************************	
bool CCeWatchList::EndValueEdit() 
//*****************************************************************************	
{
	ASSERT(m_pEditContainer != NULL);
	ASSERT(m_pBackEnd != NULL);

	int iEditItem = m_pEditContainer->GetEditItem();
	ASSERT(iEditItem >= 0 && iEditItem < GetItemCount());
	if (iEditItem >= 0 && iEditItem < GetItemCount())
	{
		CCeWatchElement* pVar = GetVar(iEditItem);	
		CCeDisplayElement* pDisplay = GetDisplayElement(iEditItem);
		ASSERT(pVar != NULL);
		ASSERT(m_pBackEnd != NULL);
		if (pVar != NULL && m_pBackEnd != NULL && pVar->IsConnected())
		{
			CString str;
			m_pEditContainer->GetText(str);
			int IdsError = 0;
			if (!CCeWatchBackEnd::IsValidValue(pVar, str))
				IdsError = IDS_ERROR_INVALID_WRITE_EXPR;
			else if (!m_pBackEnd->WriteVar(pVar, str))
				IdsError = IDS_ERROR_WRITE_FAILED;

			if (IdsError == 0)
			{
				pVar->AddToHistory(str);
				GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
			}
			else
			{
				m_pEditContainer->IgnoreFocus(true);
				ErrorMsg(IdsError);
//				m_pEditWnd->SetFocus();
//				m_pEditWnd->SetSel(0, -1);
				m_pEditContainer->IgnoreFocus(false);
				return false;
			}
		}
	}
	return true;
}

//*****************************************************************************	
bool CCeWatchList::EndFormatEdit() 
//*****************************************************************************	
{
	ASSERT(m_pEditContainer != NULL);
	ASSERT(m_pBackEnd != NULL);

	int iEditItem = m_pEditContainer->GetEditItem();
	ASSERT(iEditItem >= 0 && iEditItem < GetItemCount());
	if (iEditItem >= 0 && iEditItem < GetItemCount())
	{
		CCeWatchElement* pVar = GetVar(iEditItem);
		ASSERT(pVar != NULL);
		if (pVar != NULL)
		{
			int iTop    = GetTopIndex();
			int iBottom = iTop + GetCountPerPage();
			iBottom = min(iBottom, GetItemCount());
			for (int iItem = iTop; iItem < iBottom; iItem++)
			{
				CCeWatchElement* pVar2 = GetVar(iItem);
				if (pVar2 == pVar)
				{
					CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
					if (pDisplayElement != NULL && pDisplayElement->GetExpressionHandle() >= 0)
					{
						CString str;
						m_pEditContainer->GetText(str);
						if (!m_pBackEnd->SetFormat(pDisplayElement->GetExpressionHandle(), str))
						{
							m_pEditContainer->IgnoreFocus(true);
							ErrorMsg(IDS_ERROR_INVALID_FORMAT_EXPR);
							m_pEditContainer->IgnoreFocus(false);
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

//*****************************************************************************	
LRESULT CCeWatchList::OnEndLabelEdit(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	EndLabelEdit(wParam);
	return 0;
}

//*****************************************************************************	
void CCeWatchList::OnAddVariableFromList() 
//*****************************************************************************	
{
	if (m_pBackEnd != NULL)
	{
		CCeSymbol SymbolTable(true);
		CWaitCursor wait;   // display wait cursor
		if (SymbolTable.Load(m_pBackEnd->GetDebugInfo()))
		{

			CCeWatchSheetInsVar m_Sheet(IDS_INSERTVARIABLE, this);
			CPageInsVarTree m_VarTree(this, &m_Sheet, &SymbolTable);
			CPageInsVarList m_VarList(this, &m_Sheet, &SymbolTable);
			m_Sheet.AddPage(&m_VarTree);
			m_Sheet.AddPage(&m_VarList);
			m_Sheet.DoModal();
		}
		else
		{
			ErrorMsg(IDS_ERROR_LOADSYMBOLS);
		}
	}
	SetFocus();		// SIS 27.1.05: set focus back to dialog after inserting
}

//*****************************************************************************	
void CCeWatchList::OnAddVariable() 
//*****************************************************************************	
{
	// 15.12.05 SIS >>
	// removed CheckVariableInsert() call
	// insert position is calculated correctly now
	// 15.12.05 SIS <<
	CCeWatchElement* pVar = m_RootVar.AddChild("New Variable", CCeWatchType());
	int iItem = GetSelectedItem();
	iItem = GetNextInsertPos(iItem);	// 15.12.05 SIS
	InsertVarItem(pVar, iItem, NULL, true);
	UpdateConnections();
	if (iItem < 0)
		iItem = GetItemCount() - 1;
	
	if (iItem >= 0)
	{
		CRect r;
		GetItemRect(iItem, r, LVIR_LABEL); 
		BeginLabelEdit(iItem, r.right - 1);
	}
}

//*****************************************************************************	
int CCeWatchList::InsertExpression(LPCTSTR pstrName, CCeWatchElement *&pInsertedElement, int iItem /*=-1*/) 
//*****************************************************************************	
{
	CWaitCursor wait;   // display wait cursor
	CCeWatchElement* pVar = NULL;
    int iCount;
	CCeWatchType type;
    CString strText;

    pInsertedElement = NULL;

	if (!m_pBackEnd->GetTypeAndText(pstrName, type, strText))
		type.Set(CEWATCH_BASETYPE_SIMPLE, "???");
    else
        pstrName = strText;
	
	pVar = new CCeWatchElement(pstrName, type);

    iCount = InsertVariable (pVar, iItem);
    if (iCount == 0)
        return (0);

    pInsertedElement = pVar;
    return (iCount);
}

//*****************************************************************************	
int CCeWatchList::InsertExpressionWithStructInfo(LPCTSTR pstrName, CCeWatchElement *&pInsertedElement, int iItem /*=-1*/) 
//*****************************************************************************	
{
	CWaitCursor wait;   // display wait cursor
	CCeWatchElement* pVar = NULL;
    int iCount = 0;
	CCeWatchType type;
    CString strText;
    CString strId;
    CString strIdPath(pstrName);

	if (m_pBackEnd != NULL)
	{
		CCeSymbol SymbolTable(true);
		CWaitCursor wait;   // display wait cursor
		if (SymbolTable.Load(m_pBackEnd->GetDebugInfo()))
		{
            CCeSymbol*  pChild = NULL;
            pChild = SymbolTable.FindChild(strIdPath);
            if(pChild)
            {
                iCount = InsertVariable(pChild, iItem);
                UpdateConnections();
            }
            else
            {
                InsertExpression(pstrName, pInsertedElement);
            }
        }
		else
		{
			ErrorMsg(IDS_ERROR_LOADSYMBOLS);
		}
    }
    return iCount;
}


BOOL CCeWatchList::StripId(CString& rstrIdPath, CString& rstrId)
{
    if(rstrIdPath.IsEmpty())
    {
        return FALSE;
    }

    int iFound;
    CString strPathTmp(rstrIdPath);

    iFound = strPathTmp.Find(_T('.'));
    if(iFound >= 0)
    {
        rstrId = strPathTmp.Left(iFound);
        rstrIdPath = strPathTmp.Mid(iFound + 1);
    }
    else
    {
        rstrId = rstrIdPath;
        rstrIdPath.Empty();
    }
    return TRUE;
}



//*****************************************************************************	
int CCeWatchList::InsertVariable(CCeSymbol *pSymbol, int iItem /*=-1*/) 
//*****************************************************************************	
{
    if (pSymbol != NULL && CCeWatchElement::IsVarKindVisible(pSymbol->GetType().GetVarKind()) && 
        pSymbol->GetType().GetName().CompareNoCase(_T("Unknown")) != 0)
		return InsertVariable(new CCeWatchElement(pSymbol), iItem);
	else
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
		return 0;
    }
}


////*****************************************************************************	
//bool CCeWatchList::IsVarKindVisible(ECeWatchVarKind varKind)
////*****************************************************************************	
//{
//    switch(varKind)
//    {
//    case Var:
//    case VarInput:
//    case VarOutput:
//    case VarGlobal:
//        return true;
//    }
//    return false;
//}

//*****************************************************************************	
int CCeWatchList::InsertVariable(CCeWatchElement *pVar, int iItem /*=-1*/) 
//*****************************************************************************	
{
	ASSERT(IsValidWatchElement(pVar));
	if (pVar != NULL)
	{
        int iCount;

		m_RootVar.AddChild(pVar);
		// 15.12.05 SIS >>
		// determine correct insert position
		if (iItem < 0)
		{
			iItem = GetSelectedItem();
			iItem = GetNextInsertPos(iItem);
		}
		// 15.12.05 SIS <<
		if (iItem < 0)
			iItem = GetItemCount();
		iCount = InsertVarItem(pVar, iItem, NULL, true);
		UpdateConnections();
		GetParent()->PostMessage(WM_CEWATCH_VARLISTCHANGED, 0, NULL);
        return (iCount);
	}
	return 0;
}

//*****************************************************************************	
bool  CCeWatchList::CheckVariableInsert()
//*****************************************************************************	
{
	if (m_pBackEnd != NULL)
	{
		int SelCount = GetSelectedCount();
		if (SelCount > 1) // don't know where 
			return false;
		else if (SelCount == 0) // insert at the end of list
			return true;
		else if (SelCount == 1) // insert above the currently selected variable
		{
			// selected item must be a top level variable
			int iItem = GetSelectedItem();
			ASSERT(iItem >= 0);
			if (iItem >= 0)
			{
				CCeWatchElement* pVar = GetVar(iItem);				
				ASSERT(pVar != NULL);
				if (pVar != NULL)
				{
					if (pVar->GetLevel() == 1)
						return true;
				}
			}
		}
	}
	return false;
}

//*****************************************************************************	
bool  CCeWatchList::CheckVariableDelete()
//*****************************************************************************	
{
	if (GetSelectedCount() > 0) 
	{
		CCeWatchElement* pVar = NULL;
		for (int ii = GetItemCount(); ii >= 0; ii--)
		{
			if ( GetItemState(ii, LVIS_SELECTED))
			{
				pVar = GetVar(ii);
				if (pVar != NULL && pVar->GetLevel() == 1)
					return true;
			}
		}
	}
	return false;
}

//*****************************************************************************	
bool  CCeWatchList::CheckValueEdit()
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
	{
		CCeDisplayElement* pDisplay = GetDisplayElement(iItem);
		if (pDisplay != NULL)
		{
			CCeWatchElement* pVar = pDisplay->GetWatchElement();
			return (pVar != NULL) &&  pVar->IsConnected() && pVar->GetType().IsSimpleType();
		}
	}
	return false;
}

//*****************************************************************************	
bool  CCeWatchList::CheckFormatEdit()
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
	{
		CCeWatchElement* pVar = GetVar(iItem);
		return (pVar != NULL) && pVar->GetType().IsSimpleType();
	}
	return false;
}

//*****************************************************************************	
bool  CCeWatchList::CheckNameEdit()
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
	{
		CCeWatchElement* pVar = GetVar(iItem);
		return (pVar != NULL) && ((pVar->GetLevel() == 1) || pVar->GetType().IsArray());
	}
	return false;
}

//*****************************************************************************	
bool  CCeWatchList::CheckIncRange(bool bDec)
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	if (iItem >= 0)
	{
		CCeWatchElement* pVar = GetVar(iItem);
		return (pVar != NULL) && (pVar->GetType().IsArray());
	}
	return false;
}


//*****************************************************************************	
void CCeWatchList::UpdateConnections()
//*****************************************************************************	
{
	if (IsWindow(GetSafeHwnd()))
		PostMessage(WM_CEWATCH_UPDATECONNECTIONS, 0, NULL);
}

//*****************************************************************************	
void CCeWatchList::OnForceResubscribe()
//*****************************************************************************	
{
	LoadSymbols();	// 10.10.06 SIS
	if (IsWindow(GetSafeHwnd()))
		PostMessage(WM_CEWATCH_UPDATECONNECTIONS, 1, NULL);
}

//*****************************************************************************	
LRESULT CCeWatchList::OnUpdateConnections(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
    BOOL bResubscribe = (BOOL)wParam;
	int iTop    = GetTopIndex();
	int iBottom = iTop + GetCountPerPage() + 1;
	iBottom = min(iBottom, GetItemCount());

	for (int iItem = 0; iItem < GetItemCount(); iItem++)
	{
		CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
		ASSERT(IsValidDisplayElement(pDisplayElement));
		if (pDisplayElement != NULL && pDisplayElement->GetWatchElement() != NULL)
		{
			CCeWatchElement* pVar = pDisplayElement->GetWatchElement();
			if (pVar->GetChildrenCount() == 0 && pVar->GetType().IsSimpleType())
			{
				if (m_pBackEnd == NULL)
				{
					pDisplayElement->SetExpressionHandle(-1);
					RedrawItems(iItem, iItem);
				}
				else
				{
					bool bConnect = (iItem >= iTop && iItem <= iBottom);

                    if (bResubscribe && bConnect && pDisplayElement->GetExpressionHandle() != -1)
                    {
						m_pBackEnd->RemoveVar(pDisplayElement->GetExpressionHandle());
						pDisplayElement->SetExpressionHandle(-1);
                    }

					if (bConnect && pDisplayElement->GetExpressionHandle() == -1)
					{
						CString strExpr = pVar->GetName();

						if (pVar->GetFormat().GetLength() > 0)
							strExpr += ", " + pVar->GetFormat();

						pDisplayElement->SetExpressionHandle(m_pBackEnd->AddExpression(strExpr, pDisplayElement));
						RedrawItems(iItem, iItem);
					}
					else if (!bConnect && pDisplayElement->GetExpressionHandle() >= 0)
					{
						m_pBackEnd->RemoveVar(pDisplayElement->GetExpressionHandle());
						pDisplayElement->SetExpressionHandle(-1);
						RedrawItems(iItem, iItem);
					}
				}
			}
		}
	}
	return 0;
}


//*****************************************************************************	
void CCeWatchList::Deactivate()
//*****************************************************************************	
{
    if(m_pBackEnd == NULL)
    {
        return;
    }

    int iItemCount = GetItemCount();
	for (int iItem = 0; iItem < iItemCount; iItem++)
	{
		CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
		ASSERT(IsValidDisplayElement(pDisplayElement));
		if (pDisplayElement != NULL && pDisplayElement->GetWatchElement() != NULL)
		{
			CCeWatchElement* pVar = pDisplayElement->GetWatchElement();
			if (pVar->GetChildrenCount() == 0 && pVar->GetType().IsSimpleType())
			{
                if (pDisplayElement->GetExpressionHandle() != -1)
                {
					m_pBackEnd->RemoveVar(pDisplayElement->GetExpressionHandle());
					pDisplayElement->SetExpressionHandle(-1);
                    RedrawItems(iItem, iItem);
                }
			}
		}
	}
}

////*****************************************************************************	
//void CCeWatchList::Activate()
////*****************************************************************************	
//{
//    UpdateConnections();
//}

//*****************************************************************************	
void CCeWatchList::UpdateItem(CCeDisplayElement *pDisplayElement)
//*****************************************************************************	
{
	ASSERT(m_pBackEnd != NULL);
	if (IsWindow(GetSafeHwnd()) && m_pBackEnd != NULL)
	{
		ASSERT(pDisplayElement != NULL && pDisplayElement->GetWatchElement() != NULL);
		if (pDisplayElement != NULL && pDisplayElement->GetWatchElement() != NULL)
		{
			ASSERT(IsValidDisplayElement(pDisplayElement));

			m_pBackEnd->UpdateVar(pDisplayElement->GetExpressionHandle(), pDisplayElement->GetWatchElement());
			pDisplayElement->GetWatchElement()->SetFormat(m_pBackEnd->GetFormat(pDisplayElement->GetExpressionHandle()));

			int iTop    = GetTopIndex();
			int iBottom = iTop + GetCountPerPage() + 1;
			iBottom = min(iBottom, GetItemCount());
			for (int iItem = iTop; iItem < iBottom; iItem++)
			{
				if (pDisplayElement == GetDisplayElement(iItem))
				{
					CRect r1, r2;;
					GetItemRect(iItem, r1, LVIR_ICON); 
					GetItemRect(iItem, r2, LVIR_BOUNDS); 

					r2.left += GetColumnWidth(0) + GetColumnWidth(1); 
					InvalidateRect(r1, FALSE);
					InvalidateRect(r2, FALSE);
				}	
			}
		}
	}
}

//*****************************************************************************	
BOOL CCeWatchList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
//*****************************************************************************	
{
	BOOL bResult = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	UpdateConnections();
    return bResult;
}

//*****************************************************************************	
void CCeWatchList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//*****************************************************************************	
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	UpdateConnections();
}

//*****************************************************************************	
void CCeWatchList::OnSize(UINT nType, int cx, int cy) 
//*****************************************************************************	
{
	CListCtrl::OnSize(nType, cx, cy);
	UpdateConnections();
}

//*****************************************************************************	
void CCeWatchList::OnLoadWatches() 
//*****************************************************************************	
{
	GetParent()->PostMessage(WM_CEWATCH_LOADLIST, 0, NULL);
}

//*****************************************************************************	
void CCeWatchList::OnStoreWatches() 
//*****************************************************************************	
{
	GetParent()->PostMessage(WM_CEWATCH_STORELIST, 0, NULL);
}
 

//*****************************************************************************	
void CCeWatchList::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
//*****************************************************************************	
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem >= 0)
	{
		CCeDisplayElement* pDisplayElement = GetDisplayElement(pNMListView->iItem);
		if (pDisplayElement != NULL)
		{
			SetItemData(pNMListView->iItem, NULL);
			if (pDisplayElement->GetExpressionHandle() >= 0 && m_pBackEnd != NULL)
			{
				m_pBackEnd->RemoveVar(pDisplayElement->GetExpressionHandle());
			}
			pDisplayElement->SetWatchElement(NULL);
			delete pDisplayElement;
		}
	}
	
	*pResult = 0;
}


//*****************************************************************************	
void CCeWatchList::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
//*****************************************************************************	
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ASSERT(m_pDragImage == NULL);

	*pResult = 1;
	if (m_pEditContainer == NULL && m_pDragImage == NULL)
	{
		CCeWatchElement* pVar = GetVar(pNMListView->iItem);
		if (pVar->GetLevel() == 1)
		{
			CPoint ptImage;
			m_iSrcItem = pNMListView->iItem;
			m_pDragImage = CreateDragImage(pNMListView->iItem, &ptImage);
			if (m_pDragImage != NULL)
			{
				m_pDragImage->BeginDrag(0, CPoint(8, 8));
				m_pDragImage->DragShowNolock(TRUE);
				SetCapture();
				*pResult = 0;
			}
		}
	}
}

//*****************************************************************************	
void CCeWatchList::OnMouseMove(UINT nFlags, CPoint point) 
//*****************************************************************************	
{
	if (m_pDragImage != NULL)
	{
		POINT pt = point;
		ClientToScreen(&pt);
        m_pDragImage->DragMove(pt);		

		int iDestItem;
		if (GetDropTarget(point, iDestItem, m_iSrcItem))
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		else
			SetCursor(LoadCursor(NULL, IDC_NO));
	}
	
	CListCtrl::OnMouseMove(nFlags, point);
}

//*****************************************************************************	
void CCeWatchList::OnLButtonUp(UINT nFlags, CPoint point) 
//*****************************************************************************	
{
	if (m_pDragImage != NULL)
	{
        m_pDragImage->EndDrag();
		delete m_pDragImage;
		m_pDragImage = NULL;
		ReleaseCapture();

		int iDestItem;
		if (GetDropTarget(point, iDestItem, m_iSrcItem))
		{
			MoveVarItem(m_iSrcItem, iDestItem);
//			UpdateConnections();
		}
		m_iSrcItem = -1;
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}


//*****************************************************************************	
bool CCeWatchList::GetDropTarget(CPoint point, int &iDestItem, int iSrcItem/*=-1*/) 
//*****************************************************************************	
{
	CRect rHeader, rClient;
	GetHeaderCtrl()->GetWindowRect(rHeader);
	GetClientRect(rClient);
	rClient.top += rHeader.Height();

	iDestItem = -1;
	if (rClient.PtInRect(point))
	{
		iDestItem = HitTest(point);
		if (iDestItem >= 0)
		{
			if ((iSrcItem < 0) || (iSrcItem != iDestItem && iSrcItem+1 != iDestItem))
			{
				CCeWatchElement* pVar = GetVar(iDestItem);
				if (pVar != NULL && pVar->GetLevel() == 1)
					return true;
			}
		}
		else
		{
			if ((iSrcItem < 0) || (iSrcItem != GetItemCount() - 1))
            {
                iDestItem = GetItemCount();
				return true; // dropping ok at the end of the list
            }
		}
	}
	return false;
}


//*****************************************************************************	
CImageList* CCeWatchList::CreateDragImage(int item, LPPOINT lpPoint) 
//*****************************************************************************	
{
	CImageList *iList = new CImageList;          //  create a new image-list	
	CRect rc;
	GetItemRect(item, &rc, LVIR_BOUNDS);         //  get the complete row
	rc.OffsetRect(-rc.left, -rc.top);            //  make it (0, 0)-aligned
	rc.right = GetColumnWidth(0);                //  just want the 1st columne

//	iList->Create(rc.Width(), rc.Height(), ILC_COLOR24 | ILC_MASK, 1, 1);
	iList->Create(rc.Width(), rc.Height(), ILC_COLOR24, 1, 1);

	CDC *pDC = GetDC(); //         get device-context
	if (pDC) 
	{      
		CBitmap drawMap, maskMap;
		// create a memory-dc for the image and attach a bitmap to it
		CDC drawDC;	      
		drawDC.CreateCompatibleDC(pDC);
		CFont* pOldFont = drawDC.SelectObject(GetFont());
		drawMap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
		CBitmap *old = drawDC.SelectObject(&drawMap);
		// erase the background and draw the item into the device context

//		drawDC.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));
		DRAWITEMSTRUCT dis;
		dis.hDC       = drawDC.m_hDC;             // device-context to draw in
		dis.rcItem    = rc;                       // the item rectangle
		dis.itemID    = item;                     // the item's id
		dis.itemState = ODS_DEFAULT;              // or ODS_SELECTED
		DrawItem(&dis);                           // draw item into the dc
		drawDC.MoveTo(rc.left, rc.top);
		drawDC.LineTo(rc.right-1, rc.top);
		drawDC.LineTo(rc.right-1, rc.bottom-1);
		drawDC.LineTo(rc.left, rc.bottom-1);
		drawDC.LineTo(rc.left, rc.top);

		drawDC.SelectObject(old);                 // switch back to old bitmap
		drawDC.SelectObject(pOldFont);

		// create a memory-dc for the mask and attach a bitmap to it. to  make
		// the bitmap just one bit depth, we make it compatible to the memory-
		// dc instead of the screen-dc as above      
#ifdef CE_WITHMASK
		CDC maskDC;
		maskDC.CreateCompatibleDC(pDC);      
		maskMap.CreateCompatibleBitmap(&maskDC, rc.Width(), rc.Height());
		pOldFont = maskDC.SelectObject(GetFont());
		old = maskDC.SelectObject(&maskMap);
		// erase the background and draw the item into the device context
//		maskDC.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));
		dis.hDC       = maskDC.m_hDC;             // device-context to draw in
		dis.rcItem    = rc;                       // the item rectangle
		dis.itemID    = item;                     // the item's id
		dis.itemState = ODS_DEFAULT;              // or ODS_SELECTED
		DrawItem(&dis);                           // draw into the dc
		maskDC.MoveTo(rc.left, rc.top);
		maskDC.LineTo(rc.right-1, rc.top);
		maskDC.LineTo(rc.right-1, rc.bottom-1);
		maskDC.LineTo(rc.left, rc.bottom-1);
		maskDC.LineTo(rc.left, rc.top);
		
		maskDC.SelectObject(pOldFont);
		maskDC.SelectObject(old);                 // switch back to old bitmap
#endif
		// add the image (and mask) to the image-list
		iList->Add(&drawMap, &maskMap);     
		ReleaseDC(pDC);   
	}   
	return iList;
}

//*****************************************************************************	
void CCeWatchList::ErrorMsg(unsigned IdsMsg)
//*****************************************************************************	
{
	CString strMessage;
	strMessage.LoadString(IdsMsg);
    ::AfxMessageBox(strMessage, MB_ICONSTOP);
}


//*****************************************************************************	
BOOL CCeWatchList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
//*****************************************************************************	
{
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

	if (pHDN->hdr.hwndFrom == GetHeaderCtrl()->m_hWnd)
	{
		if (pHDN->hdr.code == HDN_DIVIDERDBLCLICKW || pHDN->hdr.code == HDN_DIVIDERDBLCLICKA)
		{
			ASSERT(pHDN->iItem < CEWATCH_NCOLUMNS);
			if (pHDN->iItem < CEWATCH_NCOLUMNS)
			{
				int MaxTextWidth = 0;
				int TextWidth = 0;
				CString str;

				CRect IconRect;
				if (GetItemCount() > 0)
					GetItemRect(0, IconRect, LVIR_ICON); 
				
				// calculate max. width of selected column
				for (int ii = 0; ii < GetItemCount(); ii++)
				{
					TextWidth = 0;
					CCeWatchElement* pVar = GetVar(ii);
					CCeDisplayElement* pDisplayElement = GetDisplayElement(ii);
					switch (pHDN->iItem)
					{
					case 0:
						if (pVar->GetLevel() == 1)
							str = pVar->GetName();
						else
							str = pVar->GetShortName();
			
						if (pVar->GetType().IsArray())
							str += ", [" + GetWatchIndices(ii)->AsString() + "]";
					
						TextWidth = (IconRect.Width() + 1) + pVar->GetLevel() * m_xIndent + 2;
						break;
					case 1:
						str = pVar->GetType().GetName();
						break;
					case 2:
						str = pVar->GetValue();
						break;
					case 3:
						str = pVar->GetTime();
						break;
					case 4:
						str = pVar->GetQualityText();
						break;
					}
					TextWidth += GetStringWidth(str);
					MaxTextWidth = max(TextWidth, MaxTextWidth);
				}
				int BestWidth = max(MaxTextWidth, m_HeaderWidth[pHDN->iItem]);
				SetColumnWidth(pHDN->iItem, BestWidth + 8);
				*pResult = TRUE; 
				return TRUE;     
			}
		}
	}
 	return CListCtrl::OnNotify(wParam, lParam, pResult);
}



//*****************************************************************************	
void CCeWatchList::OnEditFormat() 
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	CRect r;
	if (GetEditRect(iItem, CEWATCH_EDITTYPE_FORMAT, r))
		BeginLabelEdit(iItem, r.right-1);
}

//*****************************************************************************	
void CCeWatchList::OnEditName() 
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	CRect r;
	if (GetEditRect(iItem, CEWATCH_EDITTYPE_NAME, r))
		BeginLabelEdit(iItem, r.right-1);
}

//*****************************************************************************	
void CCeWatchList::OnEditValue() 
//*****************************************************************************	
{
	int iItem = GetSelectedItem();
	CRect r;
	if (GetEditRect(iItem, CEWATCH_EDITTYPE_VALUE, r))
		BeginLabelEdit(iItem, r.right-1);
}

//*****************************************************************************	
void CCeWatchList::OnDecRange() 
//*****************************************************************************	
{
	IncRange(true);
}

//*****************************************************************************	
void CCeWatchList::OnIncRange() 
//*****************************************************************************	
{
	IncRange(false);	
}

//*****************************************************************************	
DROPEFFECT CCeWatchList::OnDragEnter (COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
//*****************************************************************************	
{
    if (pDataObject->IsDataAvailable(m_uiWatchExpressionCBFormat))
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}


//*****************************************************************************	
DROPEFFECT CCeWatchList::OnDragOver (COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
//*****************************************************************************	
{
    if (pDataObject->IsDataAvailable(m_uiWatchExpressionCBFormat))
	{
		int iDestItem;
		if (GetDropTarget(point, iDestItem))
			return DROPEFFECT_COPY;
	}
    return DROPEFFECT_NONE;
}


//*****************************************************************************	
BOOL CCeWatchList::OnDrop (COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
//*****************************************************************************	
{
    if (pDataObject->IsDataAvailable(m_uiWatchExpressionCBFormat))
	{
		int iDestItem;
		if (GetDropTarget(point, iDestItem))
		{
			HGLOBAL hGlobal = pDataObject->GetGlobalData (m_uiWatchExpressionCBFormat);
			if (hGlobal != NULL)
			{
				CSharedFile file;
				CArchive ar (&file, CArchive::load);
				CWatchExpressionDragItem dragItem;
                CCeWatchElement *pInsertedElement=NULL;

				file.SetHandle (hGlobal, FALSE);
				dragItem.Serialize (ar);
				ar.Close ();
				file.Detach ();
				
				//InsertExpression(m_pBackEnd->ScopeExpression(dragItem.GetExpression(), dragItem.GetScope()), pInsertedElement, iDestItem); 		    
				InsertExpressionWithStructInfo(m_pBackEnd->ScopeExpression(dragItem.GetExpression(), dragItem.GetScope()), pInsertedElement, iDestItem); 		    
				return TRUE;
			}
		}
	}
	return FALSE;
}


//*****************************************************************************	
DROPEFFECT CCeWatchDropTarget::OnDragEnter (CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
//*****************************************************************************	
{
    CCeWatchList* pWatchList = dynamic_cast<CCeWatchList *> (pWnd);
    if (pWatchList == NULL)
        return FALSE;

    return (pWatchList->OnDragEnter (pDataObject, dwKeyState, point));
}


//*****************************************************************************	
DROPEFFECT CCeWatchDropTarget::OnDragOver (CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
//*****************************************************************************	
{
    CCeWatchList* pWatchList = dynamic_cast<CCeWatchList *> (pWnd);
    if (pWatchList == NULL)
        return FALSE;

    return (pWatchList->OnDragOver (pDataObject, dwKeyState, point));
}

//*****************************************************************************	
BOOL CCeWatchDropTarget::OnDrop (CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
//*****************************************************************************	
{
    CCeWatchList* pWatchList = dynamic_cast<CCeWatchList *> (pWnd);
    if (pWatchList == NULL)
        return FALSE;

    return (pWatchList->OnDrop (pDataObject, dropEffect, point));
}


void CCeWatchList::OnSetFocus(CWnd* pOldWnd) 
{
	// SHIFT_F10 14.12.04 SIS >>
	// select first item, if none selected
	if(GetSelectedItem() == -1 && GetItemCount() > 0)
	{
	    SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	// SHIFT_F10 14.12.04 SIS <<
	CListCtrl::OnSetFocus(pOldWnd);
	RefreshWindow();	
}

void CCeWatchList::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	RefreshWindow();	
}

//*****************************************************************************	
void CCeWatchList::RefreshWindow()
//*****************************************************************************	
{

	CRect rItems(0,0,0,0);
	CRect rBottom, rRight;
	GetClientRect(rBottom);
	rRight = rBottom;
	rBottom.top = rItems.bottom + 1;
	rRight.left = rItems.right + 1;

	if (GetItemCount() > 0)
	{
		int iTop    = GetTopIndex();
		int iBottom = iTop + GetCountPerPage();
		iBottom = min(iBottom, GetItemCount()-1);

		CRect r1, r2;
		GetItemRect(iTop,    r1, LVIR_BOUNDS); 
		GetItemRect(iBottom, r2, LVIR_BOUNDS); 
		rItems.UnionRect(&r1, &r2);
		InvalidateRect(rItems, FALSE);
	}
	InvalidateRect(rBottom, TRUE);
	InvalidateRect(rRight, TRUE);

}

void CCeWatchList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    CString str;
    BSTR sVal = NULL;
    HRESULT hr = S_OK;
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

    if(pNMListView->iItem < 0 || pNMListView->iSubItem != CEWATCH_SUBITEM_VALUE ) {
        //something wrong
        return;
    }

    CCeWatchElement* pVar = GetVar(pNMListView->iItem);

    if (pVar == NULL || m_pBackEnd == NULL || !pVar->IsConnected()) {
        //var is not there or backend not there or variable is not
        //connected
        return;
    }

    const CCeWatchType type = pVar->GetType();

    if(type.IsSimpleType() && type.GetIECType() == BL_BOOL) {
        //Toggle if it is a simple type and IEC datatype == BOOL
        VARIANT va;

        str = pVar->GetValue();
        if(str.IsEmpty()) {
            return;
        }

        ::VariantInit(&va);
        sVal = str.AllocSysString();
        
        hr = BL_StringToVariant(sVal, (BL_IEC_TYP)type.GetIECType(), NULL, BL_USE_IEC_FORMAT, &va);
        
        FC_FREE_BSTR(sVal);
        
        if(FAILED(hr)) {
            return;
        }

        sVal = ::SysAllocStringLen(NULL,10);
        //toggle the value
        va.boolVal = (va.boolVal == VARIANT_TRUE) ? VARIANT_FALSE : VARIANT_TRUE;

        hr = BL_VariantToString (&va, BL_BOOL, NULL, BL_USE_WINDOWS_FORMAT, sVal, ::SysStringLen(sVal));
        if (FAILED (hr)) {
            FC_FREE_BSTR(sVal);
            return;
        }
        
        str = sVal;
        FC_FREE_BSTR(sVal);
        m_pBackEnd->WriteVar(pVar,str);
    }

}
