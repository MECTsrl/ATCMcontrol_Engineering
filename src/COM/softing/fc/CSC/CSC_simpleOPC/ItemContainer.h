/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemContainer.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ItemContainer.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemContainer.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __ITEMCONTAINER_H_
#define __ITEMCONTAINER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "opcDA.h"
#include "ValueFormatter.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

typedef CMap<CString, LPCTSTR, OPCHANDLE, OPCHANDLE&>   handleMap;

class CItems;
//////////////////////////////////////////////////////////////////////
class CItemContainer  
{
public:
                            CItemContainer(LPCTSTR pszConfigName, LPCTSTR pszMachineName);
    virtual                 ~CItemContainer();

// =Access:
    CString                 getConfigName(void) const
                            {
                                return m_strConfigName;
                            }
    CString                 getMachineName(void) const
                            {
                                return m_strMachineName;
                            }

// =Operations:
    HRESULT                 Connect();
    HRESULT                 Disconnect();
    HRESULT                 AddItem(LPCTSTR pszItem);
    HRESULT                 AddItemList(LPCTSTR sCommonPath, LPCTSTR pszItems);
    HRESULT                 RemoveItem(LPCTSTR pszItem);
    HRESULT                 RemoveItemList(LPCTSTR sCommonPath, LPCTSTR pszItems);
    HRESULT                 UpdateItems();
    HRESULT                 ReadItem(LPCTSTR pszItem, VARIANT *pVal, WORD &wQuality, FILETIME &ftTimeStamp);
	HRESULT					ReadItemList(const CString& strCommonPath, const CString& strItems, CString& strValues);
	HRESULT					ReadItemListTyped(const CString& strCommonPath, const CString& strItems, CString& strValues);
	HRESULT					ReadCachedItemList(const CString& strCommonPath, const CString& strItems, CString& strValues);
	HRESULT					ReadCachedItemListTyped(const CString& strCommonPath, const CString& strItems, CString& strValues);
	HRESULT                 WriteItem(LPCTSTR pszItem, VARIANT val);
    BOOL                    IsEmpty();

// =Helpers
private:
    static HRESULT          ConnectToServer(LPCTSTR pszMachineName, CLSID clsid, IID iidInitial, IUnknown **ppUnk);
	HRESULT					ReadCache(int iCount, OPCHANDLE* phServer, OPCITEMSTATE* pItemValues, HRESULT* pErrors);
    
// =Attributes
protected:
    CString                 m_strConfigName;
    CString                 m_strMachineName;
    CComPtr<IOPCServer>     m_pServer;
    CComPtr<IOPCItemMgt>    m_pGroup;
    CComPtr<IOPCSyncIO>     m_pSync;
    OPCHANDLE               m_hGroup;
    DWORD                   m_dwReqUpdateRate;
    DWORD                   m_dwRevUpdateRate;
//    handleMap               m_handleMap;
	CValueFormatter			m_valueFormatter;
	CItems*					m_pItems;
};

#endif // __ITEMCONTAINER_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: ItemContainer.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
