/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcDynServer.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcDynServer.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcDynServer.h $
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

#ifndef __OPCDYNSERVER_H_
#define __OPCDYNSERVER_H_

#include <gensrv.h>

class   IEC_Resource;

//------------------------------------------------------------------*
/**
 *  class CDynamicOpcServer
 *
 *  @see    ....
*/
class CDynamicOpcServer :
            public GenericServer
{
// =Construction
public:
                            CDynamicOpcServer(BOOL bPublicGroups);
                                // default ctor
    virtual                 ~CDynamicOpcServer();
                                // dtor

                            CDynamicOpcServer(CDynamicOpcServer const & other);
                                // copy ctor
    CDynamicOpcServer const &    operator = (CDynamicOpcServer const & other);
                                // assignment

// =Access
public:
    void                    SetSessionRights(CSecurity::SECURITY_RIGHTS rights)
                            {
                                m_sessionRights = rights;
                            }
    CSecurity::SECURITY_RIGHTS  GetSessionRights(void) const
                            {
                                return m_sessionRights;
                            }
    void                    SavePassword(LPCTSTR pszPassword)
                            {
                                CSingleLock lock(&m_csSecurity, TRUE);
                                m_strPassword = pszPassword;
                            }
    CString                 GetSavedPassword(void) const
                            {
                                // cast this to non const to allow lock for const method
                                CSingleLock lock(&const_cast<CDynamicOpcServer*>(this)->m_csSecurity, TRUE);
                                return m_strPassword;
                            }

// =Operations
public:

// =Overrides
public:
	// dynamic namespace
	// callbacks for dynamic namespace
	virtual BOOL            isDynamicNode(IN LPCTSTR fullName);
	virtual BOOL            isDynamicTag(IN LPCTSTR fullName);
	virtual HRESULT         getDynamicChildNames(IN OPCBROWSETYPE browseType, 
			                    IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			                    IN DWORD accessRightsFilter, OUT CStringList &nameList);
	virtual HRESULT         getDynamicBrowseAccessPaths(IN LPCTSTR name, OUT CStringList &pathList);
    virtual DWORD           getBandwidth(void);
    virtual GenericTag *    createTag(IN LPCTSTR fullName);

    // own overloads to propagate callbacks
    GenericTag             *createDynamicTag(IN LPCTSTR fullName);

protected:

// =Helpers
private:
    HRESULT                 GetDynamicResource(OUT IEC_Resource **ppRes, OUT CStringList &listParsedNames, LPCTSTR pszPath);

// =Attributes
protected:
    CCriticalSection        m_csSecurity;
    CSecurity::SECURITY_RIGHTS  m_sessionRights;
    CString                 m_strPassword;

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
