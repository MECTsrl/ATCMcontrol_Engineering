/* $Header: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/ATargetData.h 1     28.02.07 19:09 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ATargetData.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/ATargetData.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 19:09 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _ATARGETDATA_H_
#define _ATARGETDATA_H_

#include "BaseTargetData.h"

/* ---------------------------------------------------------------------------- */
/**
 * class CATargetData
 *
 */
class CATargetData : public CBaseTargetData
{
public:
    CATargetData(CBaseTargetObject* pTargetObject,
                    ICEProjInfo* pICEProjInfo,
                    const CString& crstrKADFileName);

    virtual CString GetCommChannelDescription(const CString& crstrCommChannel);

    virtual CString GetStandardProtocol(const CString& crstrCommChannel);
    virtual CString GetStandardProtocolParameter(const CString& crstrCommChannel);
    virtual CString GetStandardConnectParameter(const CString& crstrCommChannel);


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */
    virtual HRESULT Load(const CString& crstrFileName, CXMLDocument& doc);
    virtual HRESULT Save(const CString& crstrFileName, CXMLDocument& doc);
	
    HRESULT AddFBus(const CString strFBusFileName);
    HRESULT RemoveFBus(const CString strFBusFileName);
    HRESULT RenameFBus(const CString strOldFBusFileName, 
                       const CString strNewFBusFileName);

    POSITION ListFind(CList<CString, CString&>& list, CString strFind);
    POSITION ListFindID(CList<CString, CString&>& list, CString strFind);

    void AddFBusSubNodes(CXMLNode& rtRootNode, CXMLDocument& rtXMLDoc);

protected:
    CList<CString, CString&> m_listFBusLinks;

};

#endif

/* ---------------------------------------------------------------------------- */
