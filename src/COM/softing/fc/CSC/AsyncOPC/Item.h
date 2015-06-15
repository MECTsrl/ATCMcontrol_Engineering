/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/Item.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Item.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/Item.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
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
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __ITEM_H_
#define __ITEM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class COPCConnection;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CItem
/////////////////////////////////////////////////////////////////////////////
// CItem
class ATL_NO_VTABLE CItem : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IUnknown
{
public:
	                        CItem();
  	virtual                 ~CItem();

BEGIN_COM_MAP(CItem)
END_COM_MAP()

    HRESULT                 Init(LPCTSTR strName);
    HRESULT                 Cleanup();
    HRESULT                 GetQVT(VARIANT *pValue, WORD *pQuality, FILETIME *pTime);
    HRESULT                 SetQVT(VARIANT vValue, WORD wQuality, FILETIME fTime);
    HRESULT                 WriteValue(VARIANT vValue);
    HRESULT                 GetTagName(BSTR *pName);
    long                    m_hServer;

private:
    CComVariant             m_Value;
    WORD                    m_Quality;
    FILETIME                m_Time;
    CCriticalSection        m_cs;

    CString                 m_pureName;
    COPCConnection         *m_pConnection;
};

#endif // __ITEM_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: Item.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
	
