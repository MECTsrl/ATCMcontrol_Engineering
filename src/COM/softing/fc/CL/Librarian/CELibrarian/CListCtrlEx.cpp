/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CListCtrlEx.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CListCtrlEx.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CListCtrlEx.cpp $
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
#include "CListCtrlEx.h"
//----  Static Initializations:   ----------------------------------*



CListCtrlEx::CListCtrlEx(void)
{
	m_nColumnCount = 0;
}

CListCtrlEx::~CListCtrlEx(void)
{
}

/////////////////////////////////////////////////////////////////////
/**
 * add a column to the list control
 *
 * @param strItem	name of column
 * @param nItem		number of column
 * @param nSubItem	see CListCtrl
 * @param nMask		see CListCtrl
 * @param nFmt		see CListCtrl			
 * @return			TRUE ok, FALSE not ok
 * @exception
 * @see 
 */
BOOL CListCtrlEx::AddColumn(LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = GetStringWidth(lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	m_nColumnCount += 1;
	return InsertColumn(nItem,&lvc);
}

/////////////////////////////////////////////////////////////////////
/**
 * add an item to the control
 *
 * @param nItem			number of line
 * @param nSubItem		number of column 
 * @param strItem		item text
 * @param nImageIndex	index of image, -1 if no image
 * @return		TRUE ok, FALSE not ok
 *              ok-Fall; Fehlerfall....
 * @exception
 * @see
 */
BOOL CListCtrlEx::AddItem(int nItem,int nSubItem, LPCTSTR strItem, int nImageIndex)
{
	LV_ITEM lvItem;
	BOOL rv;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}

	if(nSubItem == 0) {
		rv = InsertItem(&lvItem);
		return rv;
	}
	return SetItem(&lvItem);
}

/////////////////////////////////////////////////////////////////////
/**
 * set the item text
 *
 * @param nItem			number of line
 * @param nSubItem		number of column 
 * @param strItem		item text
 * @return				TRUE ok, FALSE not ok
 * @exception
 * @see
 */
BOOL CListCtrlEx::SetItemString(int nItem,int nSubItem, LPCTSTR strItem)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	return SetItem(&lvItem);
}

/////////////////////////////////////////////////////////////////////
/**
 * get item text
 *
 * @param row			number of line
 * @param column		number of column 
 * @return  			item text
 * @exception
 * @see
 */
CString CListCtrlEx::GetItemString(int row, int column)
{
	char buff[1000];
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = row;
	lvItem.iSubItem = column;
	//lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.pszText = buff;
	lvItem.cchTextMax = 1000;
	this->CListCtrl::GetItem(&lvItem);
	return CString(lvItem.pszText);
}

/////////////////////////////////////////////////////////////////////
/**
 * set the item image
 *
 * @param nItem			number of line
 * @param iImage		image index
 * @return				TRUE ok, FALSE not ok
 * @exception
 * @see
 */
BOOL CListCtrlEx::SetItemImage(int nItem, int iImage)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	lvItem.iImage = iImage;
	return SetItem(&lvItem);
}

/////////////////////////////////////////////////////////////////////
/**
 * get the item image index
 *
 * @param nItem			number of line
 * @return				image index
 * @exception
 * @see
 */
int CListCtrlEx::GetItemImage(int nItem)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	this->CListCtrl::GetItem(&lvItem);
	return lvItem.iImage;
}

/////////////////////////////////////////////////////////////////////
/**
 * sort items alphabetecally
 *
 * @param column	number of column to sort by
 * @param ascending	TRUE sort ascending
 *					FALSE sort descending
 * @return			TRUE ok, FALSE not ok
 * @exception
 * @see
 */
BOOL CListCtrlEx::SortItems(UINT column, BOOL ascending)
{
	CString s1, s2, t1, t2, name1, name2;
	int i1,i2;
	//
	// bubble sort will do
	//
	INT nn = GetItemCount();
	for (int ii=0; ii<nn-1; ii++)
	{
		for (int jj=ii+1; jj<nn; jj++)
		{
			s1 = GetItemString(ii,column);
			s2 = GetItemString(jj,column);
            name1 = GetItemString(ii,0);
            name2 = GetItemString(jj,0);
			if ( ((s2.CompareNoCase(s1) < 0) && ascending) || 
                 ((s2.CompareNoCase(s1) > 0) && !ascending) ||
                 ((s2.CompareNoCase(s1) ==0) && (name2.CompareNoCase(name1) < 0)) )
			{
				for (int kk=0;kk<m_nColumnCount;kk++)
				{
					if (kk==0)
					{
						i1 = GetItemImage(ii);
						i2 = GetItemImage(jj);
						SetItemImage(ii,i2);
						SetItemImage(jj,i1);
					}

					t1 = GetItemString(ii,kk);
					t2 = GetItemString(jj,kk);
					SetItemString(ii, kk, t2);
					SetItemString(jj, kk, t1);
				}
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////
/**
 * insert a column
 *
 * @param nCol		number of column
 * @param pColumn	see CListCtrl
 * @return			1 ok, 0 not ok
 * @exception
 * @see
 */
int CListCtrlEx::InsertColumn( int nCol, const LV_COLUMN* pColumn )
{
	m_nColumnCount++;
	return CListCtrl::InsertColumn( nCol, pColumn );
}

/////////////////////////////////////////////////////////////////////
/**
 * insert a column, wrapper for CListCtrl::InsertColumn
 *
 * @param xx		see CListCtrl
 * @return			see CListCtrl
 * @exception
 * @see
 */
int CListCtrlEx::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth , int nSubItem )
{
	m_nColumnCount++;	
	return CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem );
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CListCtrlEx.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 25.01.02   Time: 10:37
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * If librarian list is sorted by another table column than name take the
 * name as second sorting creteria.
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
