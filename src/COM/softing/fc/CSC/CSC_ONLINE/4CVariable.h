/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariable.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4CVariable.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariable.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
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
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef _4CVARIABLE_H_
#define _4CVARIABLE_H_

//----  Aggregate Includes:   --------------------------------------*
#include "opcDA.h"

//----  Forward Class Definitions:   -------------------------------*
class CFCVariableCollection;
class CFCVarSession;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CFCVariable
class ATL_NO_VTABLE CFCVariable : 
	public CComObjectRootEx<CComMultiThreadModel>
{

public:
	                        CFCVariable();
	                       ~CFCVariable();

BEGIN_COM_MAP(CFCVariable)
END_COM_MAP()


// IFCVariable
public:

// =Operations
public:
    void Init(LPCTSTR name, CFCVariableCollection *pVarColl);
    void Init2(OPCHANDLE hClient, CFCVarSession *pSession);
    HRESULT getContent(VARIANT *pVal, short *pQuality, DATE *pTimeStamp);
    HRESULT setContent(VARIANT value, short quality, DATE timestamp);
    OPCHANDLE getClientHandle() const {return m_hClient;}
    void setServerHandle(OPCHANDLE hServer) {m_hServer = hServer;}
    OPCHANDLE getServerHandle() const {return m_hServer;}

// =Attributes
public:
    CString                 m_name;
	CFCVariableCollection  *m_pVarColl;
    CFCVarSession          *m_pSession;

private:
    OPCHANDLE               m_hServer;
    OPCHANDLE               m_hClient;
    CComVariant             m_Value;
    WORD                    m_wQuality;
    DATE                    m_TimeStamp;
};

/**
 * convert FILETIME to DATE more accurate than
 * SystemTimeToVariantTime() would do it.
 * @param IN  pFileTime
 * @param OUT pDT
 */
HRESULT FileTimeToVariantTime(const FILETIME *pFileTime, DATE *pDT);

/////////////////////////////////////////////////////////////////////
#endif // _4CVARIABLE_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: 4CVariable.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:41
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 22  *****************
 * User: Ed           Date: 11.02.00   Time: 16:16
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Variablen-Connection mit eigenem Thread
 * 
 * *****************  Version 19  *****************
 * User: Je           Date: 7.07.98    Time: 19:38
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * * corrected bug in SubscribeStringList
 * * added VARIANT <--> IEC string conversions of baselib wrapped in COM
 * * new ReadStringListTyped returns IEC-formatted strings
 * * support for WatchWindow to get IEC-formatted strings in OnDataChanged
 * * each variable has datatype string as member
 * * ReadWithHandleTyped() and CachedValueTyped() return IEC-formatted
 * strings
 * 
 * *****************  Version 18  *****************
 *==
 *----------------------------------------------------------------------------*
*/


