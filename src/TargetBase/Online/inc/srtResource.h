/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/srtResource.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: srtResource.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/srtResource.h $
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

#ifndef _SRT_RESOURCE_H_
#define _SRT_RESOURCE_H_

#include "config.h"

class   IEC_Variable;

//------------------------------------------------------------------*
/**
 *  class SRT_Resource
 *
 *  @see    ....
*/
class SRT_Resource : public IEC_Resource
{
// =Construction
public:
//                            SRT_Resource();
                                // default ctor
                            SRT_Resource(LPCTSTR pszConfigName, int iEnumRes = 0);
protected:
    virtual                 ~SRT_Resource();
                                // dtor

private:
                            SRT_Resource(SRT_Resource const & other);
                                // copy ctor
    SRT_Resource const &    operator = (SRT_Resource const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 CreateSysVars();

    HRESULT                 GetCurrentDownloadCounter(long *pCounter);
    long                    GetCachedDownloadCounter();
                                // CD1: used to update namespace after new download
    HRESULT                 OpenResource(LPCTSTR pszResourceName, IUnknown **pUnk);
                                // return IResource ptr of given resource name
                                // create instance if necessary

    HRESULT                 RefreshVariable(IEC_Variable *pTag);


// =Overrides
public:
    virtual HRESULT         OpenResourceInstance();
    virtual BOOL            CheckForNamespaceUpdate(void);
    virtual HRESULT         FillChildrenList(OUT CStringList &listChildren,
                                             IN LPCTSTR pszCurrentPath, IN CStringList &parsedPath,
                                             OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                             VARTYPE DataTypeFilter, DWORD AccessRightsFilter);
    virtual BOOL            isDynamicNode(CStringList &listParsedName);
    virtual BOOL            isDynamicTag(CStringList &listParsedName);
    virtual GenericTag *    createDynamicTag(IN CStringList &parsedPath, LPCTSTR pszFullName);

protected:

// =Helpers
private:
    HRESULT                 WalkToSystemBranch(CStringList &parsedPath, GenericBranch **ppBranch);

    HRESULT                 BuildFlatList(CStringList &listObjNames, LPCTSTR pszCurrentPath, LPCTSTR pszRelPath,
                                          OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                          VARTYPE DataTypeFilter, DWORD AccessRightsFilter);
    HRESULT                 BuildFlatList(CStringList &listObjNames, GenericBranch *pCurrBranch, LPCTSTR pszCurrentPath,
                                          OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                          VARTYPE DataTypeFilter, DWORD AccessRightsFilter);

    BOOL                    CheckFilterCriteria(CSC_VARDEF *pObject, LPCTSTR pszName, OPCBROWSETYPE browseTypeFilter, LPCTSTR filterCriteria, VARTYPE dataTypeFilter, DWORD accessRightsFilter);
    BOOL                    CheckFilterCriteria(GenericElement *pElem, LPCTSTR pszName, OPCBROWSETYPE browseTypeFilter, LPCTSTR filterCriteria, VARTYPE dataTypeFilter, DWORD accessRightsFilter);

// =Attributes
protected:

private:
    int                     m_iEnumRes;
    CString                 m_sConfigName;
    long                    m_downloadCounter;
                                // CD1: workaround to trigger for namespace changes
    CCriticalSection        m_cs; // protect m_downloadCounter
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
