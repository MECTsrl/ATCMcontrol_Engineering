/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Security.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Security.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Security.h $
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

#ifndef __SECURITY_H_
#define __SECURITY_H_

#include <msxml.h>      // this includes header of IE4 delivered msxml control !!

class CSecurity;

extern CSecurity    g_security;


//------------------------------------------------------------------*
/**
 *  class CSecurity
 *
 *  @see    ....
*/
class CSecurity
{
// =Construction
public:
                            CSecurity();
                                // default ctor
    virtual                 ~CSecurity();
                                // dtor
private:
                            CSecurity(CSecurity const & other);
                                // copy ctor
    CSecurity const &    operator = (CSecurity const & other);
                                // assignment
public:
// =Enums
    typedef enum {
        NoSecurity = 0,
        NoRights,
        ReadOnly,
        ReadAndWrite,
        Engineering
    } SECURITY_RIGHTS;

// =Access
public:
    BOOL                    IsSecurityEnabled(void) const
                            {
                                // cast this to non const to allow lock for const method
                                CSingleLock lock(&const_cast<CSecurity*>(this)->m_csSecurity, TRUE);
                                return m_bSecEnabled;
                            }

// =Operations
public:
    HRESULT                 InitSecurityInfo();
    HRESULT                 ReReadSecurityInfo();
    SECURITY_RIGHTS         GetUserRights(LPCTSTR pszPassword);
    static LPCTSTR          GetSecurityText(SECURITY_RIGHTS right);

// =Overrides
public:

protected:

// =Helpers
private:
    HRESULT                 GetSecurityFile(OUT CString &strFileName, OUT CString &strVersionGUID);
    HRESULT                 ExtractVersionGUID(FILE *hfFile, OUT CString &strVersionGUID);
    HRESULT                 ExtractPasswords(LPCTSTR pszSecFile, OUT CString &strReadOnlyPW, CString &strReadWritePW);
    HRESULT                 ParseXML(LPCTSTR pszSecFile, IXMLDocument **ppDoc);
    HRESULT                 GetElemWithMatchingName(IXMLElementCollection *pColl, LPCTSTR pszName, long lStart, OUT long &lFound, OUT IXMLElement **ppElem);

// =Attributes
protected:
    BOOL                    m_bSecEnabled;
    CString                 m_strReadOnlyPW;
    CString                 m_strReadWritePW;
    CString                 m_strEngineeringPW;

private:
    CString                 m_strBasePath;
    CString                 m_strConfigPath;
    CString                 m_strLastVersionGUID;
    CCriticalSection        m_csSecurity;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
