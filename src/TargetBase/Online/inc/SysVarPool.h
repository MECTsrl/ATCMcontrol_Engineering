/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/SysVarPool.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SysVarPool.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/SysVarPool.h $
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

#ifndef _SYS_VAR_POOL_H_
#define _SYS_VAR_POOL_H_

typedef CMap<CString, LPCTSTR, CSysVar*, CSysVar*&>     sysVarMap;

//------------------------------------------------------------------*
/**
*  class CSysVarPool
*
*  @see    ....
*/
class CSysVarPool : public GenericNode
{
public:
    typedef enum {
        configGlobal,
        srtGlobal,
        consoleGlobal
    } poolType;

// =Construction
public:
                            CSysVarPool(LPCTSTR pszSimplePath, LPCTSTR pszOpcNode, poolType type, int iEnumerator = 0);
                                // default ctor
    virtual                 ~CSysVarPool();
                                // dtor

private:
                            CSysVarPool(CSysVarPool const & other);
                                // copy ctor
    CSysVarPool const &    operator = (CSysVarPool const & other);
                                // assignment

// =Access
public:

// =Operations
public:
//    HRESULT                 FillObjNames(DWORD dwCookie, CStringList &listObj);
    void                    ClearSysVar();
    HRESULT                 AddSysVar(CSysVar *pVar);
    HKEY                    OpenSysVarPath(BOOL bWritable);
    HRESULT                 UpdateValue(LPCTSTR pszName, LPCTSTR pszNewValue) const;
    CSysVar *               LookupSysVar(LPCTSTR pszName);
                                // return ptr to sysvar, must release after use !!

// =Overrides
public:

protected:

// =Helpers
private:
    HKEY                    OpenCommonBasepath(BOOL bWritable);

// =Attributes
public:
    static CCriticalSection m_cs;

protected:

private:
    CString                 m_strSimplePath;
    poolType                m_poolType;
    sysVarMap               m_sysVarMap;
    int                     m_iEnumerator;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
