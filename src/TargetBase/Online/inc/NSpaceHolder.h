/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/NSpaceHolder.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: NSpaceHolder.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/NSpaceHolder.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _NSPACEHOLDER_H_
#define _NSPACEHOLDER_H_

class   IEC_Variable;
class   IEC_Resource;

#define MAX_RESOURCE    2
#define END_EVENT	0
#define WORK_EVENT	1

extern const TCHAR g_szSystemFolderName[];
extern CComAutoCriticalSection     tagCS;


//------------------------------------------------------------------*
/**
 *  class NameSpaceHolder
 *
 *  @see    ....
*/
class NameSpaceHolder : public GenericObject //, public CWorkerThread
{
// =Construction
public:
                            NameSpaceHolder();
                                // default ctor
    virtual                 ~NameSpaceHolder();
                                // dtor
private:
                            NameSpaceHolder(NameSpaceHolder const & other);
                                // copy ctor
    NameSpaceHolder const &    operator = (NameSpaceHolder const & other);
                                // assignment

// =Access
public:

    static CSysVarPool const *  GetGlobalSysVars()
                            {
                                if (sm_pGlobalSysVar != NULL) {
                                    sm_pGlobalSysVar->addRef();
                                }
                                return sm_pGlobalSysVar;
                            }
    static CSysVarPool const *  GetGlobalSecuritySysVars()
                            {
                                if (sm_pGlobalSecuritySysVar != NULL) {
                                    sm_pGlobalSecuritySysVar->addRef();
                                }
                                return sm_pGlobalSecuritySysVar;
                            }
    static CSysVarPool const *  GetResourceSysVars(int iEnumRes = 0)
                            {
                                if (iEnumRes >= sm_cntResource) {
                                    return NULL;
                                }
                                if (sm_pResSysVar[iEnumRes] != NULL) {
                                    sm_pResSysVar[iEnumRes]->addRef();
                                }
                                return sm_pResSysVar[iEnumRes];
                            }
    static CSysVarPool const *  GetResourceDLoadSysVars(int iEnumRes = 0)
                            {
                                if (iEnumRes >= sm_cntResource) {
                                    return NULL;
                                }
                                if (sm_pResDLoadSysVar[iEnumRes] != NULL) {
                                    sm_pResDLoadSysVar[iEnumRes]->addRef();
                                }
                                return sm_pResDLoadSysVar[iEnumRes];
                            }
    static CSysVarPool const *  GetResourceDebugSysVars(int iEnumRes = 0)
                            {
                                if (iEnumRes >= sm_cntResource) {
                                    return NULL;
                                }
                                if (sm_pResDebugSysVar[iEnumRes] != NULL) {
                                    sm_pResDebugSysVar[iEnumRes]->addRef();
                                }
                                return sm_pResDebugSysVar[iEnumRes];
                            }
    static CSysVarPool const *  GetConsoleDLoadSysVars()
                            {
                                if (sm_pConsoleDLoadSysVar != NULL) {
                                    sm_pConsoleDLoadSysVar->addRef();
                                }
                                return sm_pConsoleDLoadSysVar;
                            }

    // namespace statistics
    static DWORD            GetItemsPerSecond(void)
                            {
                                return sm_dwItemsPerSecond;
                            }

// =Operations
public:
    HRESULT                 InitInstance();
    HRESULT                 StartEngine();
    HRESULT                 ExitInstance();
    BOOL                    CreateAndConnectNamespace(LPCTSTR pszConfiguration);
    void                    MarkAllTags(WORD quality, BOOL bMarkDeleted = FALSE);
    
    BOOL                    CheckForNamespaceUpdate(void);
                                // CD1: used to update namespace after new download
    HRESULT                 GetNamedResource(LPCTSTR pszResourceName, IEC_Resource **ppRes);
    HRESULT                 GetDefaultResource(IEC_Resource **ppRes);
    static HRESULT          GetDynResourceList(BranchList *pDynList);
    static BOOL             SplitVariableName(LPCTSTR pszFullPathName, CStringList& listParsedName);
    static BOOL             GetResourceName(LPCTSTR pszFullPathName, CString &strResourceName);
    static BOOL             IsSystemBranchName(LPCTSTR pszBranchName);
    static BOOL             IsConsoleBranchName(LPCTSTR pszBranchName);
    static BOOL             IsDynamicResource(CStringList &listParsedPath, OUT IEC_Resource **ppRes);
    static HRESULT          CreateSystemVariables(void);
    static HRESULT          CreateResourceSystemVariables(int iEnumRes);
    static void             DeleteSystemVariables(void);
    static HRESULT          DeleteResourceSystemVariables(int iEnumRes);

// =Overridables
protected:

// =Helpers
private:
    void                    ReopenResources();

    void                    Cleanup();
    HRESULT                 createDeviceThread(void);
    DWORD                   deviceThread(void);
    static DWORD WINAPI     deviceThreadMain(IN LPVOID context);

    void checkRequestTime(GenericRequest* req, BOOL bSucceeded);

// =Attributes
public:
    static TCHAR            sm_cStdSeparatorChar;
    static LPCTSTR          sm_pszSeparatorList;

protected:

private:
    GenericPointer<OPCEngine>   m_pEngine;

    static CSysVarPool      *sm_pGlobalSysVar;
    static CSysVarPool      *sm_pGlobalSecuritySysVar;
    static CSysVarPool      *sm_pConsoleDLoadSysVar;

    static CCriticalSection sm_csDynRes;
    static int              sm_cntResource;
    static CSysVarPool      *sm_pResSysVar[MAX_RESOURCE];
    static CSysVarPool      *sm_pResDLoadSysVar[MAX_RESOURCE];
    static CSysVarPool      *sm_pResDebugSysVar[MAX_RESOURCE];

    static BranchList       sm_dynResList;

    DWORD                   m_CheckRate;
                                // check rate

    // OPC cache update worker thread
	CEvent                  m_workEvent;    // read and write request event
	CEvent                  m_endEvent;     // end event for device I/O working thread
	HANDLE                  m_deviceThread; // handle of device I/O working thread
	RequestList             m_readList;     // read request queue
	RequestList             m_writeList;    // write request queue
	CCriticalSection        m_readListLock; // read request queue critical section
	CCriticalSection        m_writeListLock;// write request queue critical section
    
    static DWORD            sm_dwItemsPerSecond;
};

BOOL                        GetGlobalNamespace(NameSpaceHolder **ppNamespace);
                                // get global namespace, protected by critical section
                                // this call increments the ref. count, so caller must release
BOOL                        SetGlobalNamespace(NameSpaceHolder *pNewNamespace);
                                // set new global namespace, protected by critical section


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
