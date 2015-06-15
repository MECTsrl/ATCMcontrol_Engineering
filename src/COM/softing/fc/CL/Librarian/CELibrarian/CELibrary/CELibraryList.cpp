/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryList.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryList.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryList.cpp $
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
#include "CELibraryList.h"
#include "CELibraryItem.h"
#include "CELibraryException.h"
#include "CEL_Names.h"
//----  Static Initializations:   ----------------------------------*



// CELibraryList.cpp: Implementierung der Klasse CELibraryList.
//
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LIB_POUCLASS _T("pouclass")
#define LIB_HEADER   _T("header")     

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CString CELibraryList::m_libList = _T("lib.list");

CELibraryList::CELibraryList()
{

}

CELibraryList::~CELibraryList()
{
	for ( int ii=0; ii< m_itemArray.GetSize(); ii++ )
	{
		CELibraryItem* pCeItem = Get(ii);
		if ( pCeItem != 0 ) 
			delete pCeItem;
	}
}

CELibraryItem* CELibraryList::Add(const CString& itemName)
{
	if ( Exist( itemName ) )
		return NULL;

	CELibraryItem* pCeItem = new CELibraryItem(itemName);
	ASSERT (pCeItem != NULL);
	m_itemArray.Add(pCeItem);
	return pCeItem;
}

int CELibraryList::Delete(const CString& itemName)
{
	int index;
	CELibraryItem* pCeItem = Get(itemName,&index);
	
	if ( pCeItem != NULL )
	{
		m_itemArray.RemoveAt(index);
		delete pCeItem;
		return 1;
	}
	else
	{
		return 0;
	}
}

// 13.12.06 SIS >>
void CELibraryList::SetAt(int iIndex, CELibraryItem* pCeItem)
{
	ASSERT(iIndex >= 0 && iIndex < m_itemArray.GetSize());

	m_itemArray.SetAt(iIndex, pCeItem);
}
// 13.12.06 SIS <<

BOOL CELibraryList::Exist(const CString& itemName)
{
	CELibraryItem* pCeItem;
	CString name;
	for ( int ii=0; ii< m_itemArray.GetSize(); ii++ )
	{
		pCeItem = Get(ii);
		name = pCeItem->GetName();
		if ( name.CompareNoCase(itemName) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CELibraryList::GetSize()
{
	return m_itemArray.GetSize();
}

CELibraryItem* CELibraryList::Get( const CString& itemName , int* position)
{
	CELibraryItem* pCeItem=NULL;
	CString name;
	if (position != NULL)
		*position = -1;
	for ( int ii=0; ii< m_itemArray.GetSize() ; ii++)
	{
		pCeItem = Get(ii);
		name = pCeItem->GetName();
		if ( name.CompareNoCase(itemName) == 0 )
		{
			if (position != NULL)
				*position = ii;
			return pCeItem;
		}
	}
	return NULL;
}

CELibraryItem* CELibraryList::Get( int position)
{
	CELibraryItem* pCeItem=NULL;
	ASSERT(position < GetSize() );

	return m_itemArray.GetAt(position);
}

void CELibraryList::DeleteAll()
{
	CELibraryItem* pCeItem;
	for ( int ii=0; ii< m_itemArray.GetSize() ; ii++)
	{
		pCeItem = Get(ii);
		delete pCeItem;
	}
	m_itemArray.RemoveAll();
}

// 13.12.06 SIS >>
// compare library items
// first type is compared
// then name
int CompareLibItems(const void* pItem1, const void* pItem2)
{
	CELibraryItem*	pLibItem1 = *(CELibraryItem**)(pItem1);
	CELibraryItem*	pLibItem2 = *(CELibraryItem**)(pItem2);
	
	if(pLibItem1->GetType() > pLibItem2->GetType())
	{
		return 1;
	}
	else if(pLibItem1->GetType() < pLibItem2->GetType())
	{
		return -1;
	}

	return pLibItem1->GetName().Compare(pLibItem2->GetName());
}
// 13.12.06 SIS <<

int CELibraryList::ReadFromLibAcc(I4CLAccess* pILibAcc)
{
	ASSERT(pILibAcc);

    HRESULT hr;
	CString strName=_T("");
	CString strKey, strValue;
	//int eq;
	CString strTargetName;
	//int len;
	CELibraryItem* pCeItem=NULL;
    unsigned long     ul;
    SAFEARRAY*        parrPOUs;

    CComBSTR sConstPou( _T("CONST"));
    CComBSTR sTargetType;
    CComBSTR sCheckOutDir;
    SAFEARRAY* parrFiles;
    
    //Get all contant files
    hr = pILibAcc->GetFileNames(sConstPou, sTargetType,
                                &parrFiles);
    if (FAILED(hr))
    {
        return 0;
    }

	// 13.12.06 SIS >>
    //Get all POU's in the library
    hr = pILibAcc->GetAllPOUs(&parrPOUs);
    if(FAILED(hr))
    {
        return 0; 
    }

	// determine maximum number of entries
	int				iMaxCount = 0;
	if(parrFiles)	// constant files
	{
		iMaxCount += parrFiles->rgsabound[0].cElements;
	}
	if(parrPOUs)	// pous
	{
		iMaxCount += parrPOUs->rgsabound[0].cElements;
	}

	CELibraryItem**	paItems = new CELibraryItem*[iMaxCount];	// array of library item pointers for sorting
	memset(paItems, 0, sizeof(CELibraryItem*)*iMaxCount);		// initialize to 0
	int				iCount = 0;									// current count of entries in paItems
	// 13.12.06 SIS <<

    //Step through the constant files
    if(parrFiles)
    {
        for(ul=0; ul<parrFiles->rgsabound[0].cElements; ul++)
        {
            BSTR          sConstFile;
            sConstFile = ((BSTR*)(parrFiles->pvData))[ul];
            
            strName = sConstFile;
            //Don't show files wich start with '_'
            if(strName.GetAt(0) == _T('_'))
            {
                continue;
            }
            //Don't show files wich start with '.'
            if(strName.GetAt(0) == _T('.'))
            {
                continue;
            }
            //Don't show CONST
            if(!strName.CompareNoCase(_T("CONST")))
            {
                continue;
            }
            //Don't show cst
            if(!strName.CompareNoCase(_T("cst")))
            {
                continue;
            }
            pCeItem = Add(strName);
			// 13.12.06 SIS >>
			if(pCeItem)
			{
				paItems[iCount] = pCeItem;
				++iCount;
			}
			// 13.12.06 SIS <<
        }
    }

    //Step through the POU's
    for(ul=0; ul<parrPOUs->rgsabound[0].cElements; ul++)
    {
        BSTR          sPOU;
        unsigned long ul2;
        SAFEARRAY*    sTrgTypes;
        BSTR          sLinkFile;
        CString       strLinkFile;
        
        sPOU = ((BSTR*)(parrPOUs->pvData))[ul];
        hr = pILibAcc->GetTargetTypesOfPOU(sPOU,  &sTrgTypes);
        if(hr!=S_OK)
        {
			delete [] paItems;	// 13.12.06 SIS
            return 0; 
        }
        
        strName = sPOU;
        //Don't show files wich start with '_'
        if(strName.GetAt(0) == _T('_'))
        {
            continue;
        }
        //Don't show files wich start with '.'
        if(strName.GetAt(0) == _T('.'))
        {
            continue;
        }
        //Don't show CONST
        if(!strName.CompareNoCase(_T("CONST")))
        {
            continue;
        }
        //Don't show cst
        if(!strName.CompareNoCase(_T("cst")))
        {
            continue;
        }
        //Get the target types of the given POU
        unsigned long iTargetCount = sTrgTypes->rgsabound[0].cElements;
        //Don't show Pou's without Targets
        if(iTargetCount <= 0)
        {
            continue;
        }

        
        pCeItem = Add(strName);
		// 13.12.06 SIS >>
		if(pCeItem)
		{
			paItems[iCount] = pCeItem;
			++iCount;
		}
		// 13.12.06 SIS <<
        for(ul2=0; ul2<iTargetCount; ul2++)
        {
            BSTR sTrg;
            sTrg = ((BSTR*)(sTrgTypes->pvData))[ul2];

            strTargetName = sTrg;

            if (pCeItem !=NULL)
            {
				pCeItem->AddFile(strTargetName);
            }
        }
        SafeArrayDestroy(sTrgTypes);        
        //Get the link file of the given POU
        CString     strDummy = _T(".link");
        BSTR        sLink = strDummy.AllocSysString();
        hr = pILibAcc->GetFileAsBSTR(sPOU, NULL,sLink, &sLinkFile);
        strLinkFile = sLinkFile;
        SysFreeString(sLink);
        sLink = NULL;
        SysFreeString(sLinkFile);
        sLinkFile = NULL;

        strValue = GetPOUType(strLinkFile);
        if (pCeItem!=NULL)
        {
            pCeItem->SetProperty(CEL_ATTR_TYPE, strValue);
        }
        CString strHeaderFile, strByteCount;
        GetPOUHeaderFileNameAndByteCount(strLinkFile, strHeaderFile, strByteCount);
        if (pCeItem!=NULL)
        {
            pCeItem->SetProperty(CEL_ATTR_HEADER_FILNAME, strHeaderFile);
        }
        if (pCeItem!=NULL)
        {
            pCeItem->SetProperty(CEL_ATTR_HEADER_OFFSET_START, _T("0"));
        }
        if (pCeItem!=NULL)
        {
            pCeItem->SetProperty(CEL_ATTR_HEADER_OFFSET_END, strByteCount);
        }
        if(FAILED(hr))
        {
			delete [] paItems;	// 13.12.06 SIS
            return 0; 
        }
    }
    SafeArrayDestroy(parrPOUs);

    ASSERT(!FAILED(hr));

	// 13.12.06 SIS >>
	// sort entries (1. type, 2. name)
	if(iCount > 1)
	{
		qsort(paItems, iCount, sizeof(CELibraryItem*), CompareLibItems);
		for(int iItem = 0; iItem < iCount; ++iItem)
		{
			SetAt(iItem, paItems[iItem]);
		}
	}
	delete [] paItems;
	// 13.12.06 SIS <<
    
    return 1;
}

CString CELibraryList::GetPOUType(CString strLinkFile)
{
    //ASSERT(!strLinkFile.IsEmpty());
    CString strPOUType;

    if(strLinkFile.IsEmpty())
    {
        return _T("");
    }
    int iRes = strLinkFile.Find(LIB_POUCLASS);
    if(iRes == -1)
    {
        return _T("");
    }
    int  iStart = strLinkFile.Find(_T(";"), iRes) + 1;
    int  iEnd = strLinkFile.Find(_T("\n"), iStart);
    strPOUType = strLinkFile.Mid(iStart, iEnd - iStart - 1);

    return strPOUType;
}


void CELibraryList::GetPOUHeaderFileNameAndByteCount(CString strLinkFile, CString & strHeaderFileName, CString & strByteCount)
{
    CString strHelp;
    strHeaderFileName = _T("");
    strByteCount = _T("");

    if(strLinkFile.IsEmpty())
    {
        return ;
    }
    int iRes = strLinkFile.Find(LIB_HEADER);
    if(iRes == -1)
    {
        return ;
    }
    int  iStart = strLinkFile.Find(_T(";"), iRes) + 1;
    int  iEnd = strLinkFile.Find(_T("\n"), iStart);
    strHelp = strLinkFile.Mid(iStart, iEnd - iStart - 1);
    iStart = strHelp.Find(_T(";"), 0) + 1;
    strHeaderFileName = strHelp.Left(iStart - 1);
    strByteCount = strHelp.Right(strHelp.GetLength() - iStart);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryList.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 13.12.06   Time: 15:21
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/celibrary
 * 13.12.06 SIS sort by (1) type and (2) name
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 9  *****************
 * User: Oh           Date: 10/23/01   Time: 6:47p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * adapted to changes in cg_4claccess.idl
 * 
 * *****************  Version 8  *****************
 * User: Oh           Date: 10/12/01   Time: 10:01a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 7  *****************
 * User: Oh           Date: 9/25/01    Time: 3:05p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 6  *****************
 * User: Oh           Date: 9/21/01    Time: 4:00p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * Added: ReadFromLibAcc, the reading of constant files
 * 
 * *****************  Version 5  *****************
 * User: Oh           Date: 7/09/01    Time: 2:41p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 7/05/01    Time: 5:27p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 6/26/01    Time: 3:11p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
