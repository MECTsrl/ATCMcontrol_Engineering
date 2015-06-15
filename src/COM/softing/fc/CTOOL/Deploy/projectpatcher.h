/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/projectpatcher.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: projectpatcher.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/projectpatcher.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CDEPLOY
 *
 * =COMPONENT           Deploy
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
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
 *  09.07.2002  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



#if !defined(AFX_PROJECTPATCHER_H__5EED3422_EE6F_4F12_905C_07D850287ADD__INCLUDED_)
#define AFX_PROJECTPATCHER_H__5EED3422_EE6F_4F12_905C_07D850287ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "defs.h"


#define ACT_UNDEF           0
#define ACT_SHOW            1
#define ACT_PATCH           2



typedef struct _TargetInfo
{
    CString strTargetName;
    CString strTargetAddress;
    CString strTargetFile;
    CString strTargetOPCServer;
	CString strTargetOPCInstName;
    CString strTargetResource;
    CString strTargetType;
    CString strTargetNewAddress;
    CString strNewConnectString;
	CString strNewOPCInstName;		// 18.10.06 SIS
} TTargetInfo;



class CProjectPatcher  
{
public:
	CProjectPatcher();
	virtual ~CProjectPatcher();
    
    int DoAction();
    void SetAction(const int iAct);
    void SetSilentMode(const BOOL bSilent);
    void SetProjectDir(const TCHAR* pszProjDir);
    BOOL AddPatchTargetInfo(const TCHAR* pszTargetInfo);
    
	BOOL EvaluateBracketedTerm(const CString& crstrInput, CString& rstrPrefix, CString& rstrContents);

protected:
    int FindProjectTargetInfo();
    void ShowProjectTargetInfo();
    int GetProjectTargets();
    int FillProjectTargetInfo(TTargetInfo* pTInfo);
    CString FindProjectFile();
    CString GetLocalMachineName();

    int UpdatePatchInfo();
    int PatchFileViewXML();
    int PatchAllTargets();
    int PatchTarget(TTargetInfo* pTInfo);
    int PatchTargetFile(TTargetInfo* pTInfo);
    int PatchProjectDBI();
    int PatchTargetLoaderList(TTargetInfo* pTInfo);
    int PatchTargetTRG(TTargetInfo* pTInfo);
    int PatchTargetINST(TTargetInfo* pTInfo);
    int ChangeStateVarInNodes(CXMLNode& node, TTargetInfo* pTInfo);
    int PatchProjectSem();
    int PatchOPCServer(TTargetInfo* pTInfo);
    int PatchOPCServerConfig(TTargetInfo* pTInfo);
    int PatchOPCServerRegistry(TTargetInfo* pTInfo);

    CString FindOPCServer(TTargetInfo* pTInfo);
    CString GetCurrentUser();
    CString GetDLGuid(TTargetInfo* pTInfo);

	// d-188 02.02.05 SIS >>
	BOOL IsWindowsNT(void);
	BOOL IsAdmin(void);
	// d-188 02.02.05 SIS <<
    
protected:
    int         m_iAction;
    BOOL        m_bSilent;
    CString     m_strProjDir;
    CString     m_strProjFile;
    BOOL        m_bPatchOneTarget;
    CString     m_strLocalMachine;
    CList<TTargetInfo*, TTargetInfo*> m_listPatchTargetInfos;
    CList<TTargetInfo*, TTargetInfo*> m_listProjectTargetInfos;

};

#endif // !defined(AFX_PROJECTPATCHER_H__5EED3422_EE6F_4F12_905C_07D850287ADD__INCLUDED_)




/*
 *----------------------------------------------------------------------------*
 * $History: projectpatcher.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 18.10.06   Time: 19:00
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 18.10.06 SIS bug fix new opc config
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 2.02.05    Time: 17:30
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * d-188 02.02.05 SIS
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 16.12.04   Time: 18:53
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * support RS232
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 17.09.02   Time: 13:30
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 17.09.02   Time: 11:08
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * Moved from Bruneck Target to product.
 * 
 *==
 *----------------------------------------------------------------------------*
*/
