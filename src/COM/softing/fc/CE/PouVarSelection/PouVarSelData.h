// PouVarSelData.h: interface for the CPouVarSelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POUVARSELDATA_H__63B71815_3A5B_46C6_8C09_CDD6E7ACF209__INCLUDED_)
#define AFX_POUVARSELDATA_H__63B71815_3A5B_46C6_8C09_CDD6E7ACF209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DragDropHelper\PouVarObject.h"

#include "CG_4CLAccess.h"
#include "ProjManEventsImpl.h"



class CPouVarSelCtrl;
class CPouSelDlg;
class CVarSelDlg;



//------------------------------------------------------------------*
/**
 * CLibraryContent holds all information of one library.
 *
 * @see             
*/
class CLibraryContent
{
public:
    CString strLibName;         // library name, that can be displayed
    CString strLibPath;         // full path adn file name of the library
    CList<CPouVarObject*, CPouVarObject*> pouList;  // list with all pous 
                                                    // of the library
    CTime timeModTime;
};


//------------------------------------------------------------------*
/**
 * CGroupContainer   
 * holds information for objects of same type
 * that are grouped in different files in the class view: 
 * constants, gvls
 *
 * @see             
*/
class CGroupContainer
{
public:
    CString strFileName;
    CString strIdPath;
    CString strLastParseTime;
    POSITION firstMemberPos;
    POSITION lastMemberPos;
    BOOL     bUsed;
};


class CClassInfo
{
public:
    CString strClassType;
    CString strClassGroupIdPath;
    CString strClassText;
};


class CVarDeclInfo
{
public:
    GUID    guidActiveVarHandler;
    BOOL    bRetain;
    BOOL    bInput;
    BOOL    bOutput;
    BOOL    bInOut;
};



//------------------------------------------------------------------*
/**
 * CPouVarSelData holds and updates all data from the project and
 * the libraries. The data is the basis for the data shown in the
 * pou selection and the var selection dialog.
 *
 * @see             
*/
interface ICG_AutoDecl;
class CPouVarSelData  
{
public:
	CPouVarSelData();
	virtual ~CPouVarSelData();

    void Init(CPouVarSelCtrl* pPouVarSelCtrl);

    void HandleUpdateClassTree(const CString& strChangedIdPath);
    void UpdateLibraries();
    void UpdateProjectData();
    void UpdateProjectPous(const CString strUpdatePath);
    void UpdateProjectConstants();
    void UpdateGlobalInstances();
    void UpdateLocalInstances(const CString& strPouFileName);
    void UpdateLocalInstances();

    void FinishUpdate();

    CString GetHeader(CPouVarObject* pPouVarObj, int& iHeaderLen);
    void CheckOutUserHelp(CPouVarObject* pPouVarObj);
    CString CheckOutHeader(CPouVarObject* pPouVarObj);
    BOOL CheckOutHeader(const CString& crstrLibraryPath, const CString& crstrHeaderPath, CString& rstrHeaderFilePath);

    CVarObject*    FindLocalInstance(const CString& strInstName);
    CVarObject*    FindGlobalInstance(const CString& strInstName);
    BOOL           IsPredefinedVar(const CString& strIdentifier, CString& strTypeHint);
    CVarObject*    FindGlobVarByAddress(const CString& strAddress);
    CPouVarObject* FindPou(const CString& strPouName);

    CString FindProgramFile(const CString& strProgramType);

    CString GetActivePouFileName();
    CString GetActivePouName();
    CString GetActivePouType();
    BOOL ActivePouSupportVarDecl();
    GUID GetActiveVarHandler();
    BOOL GetActivePouSupportRetain();
    BOOL GetActivePouSupportInput();
    BOOL GetActivePouSupportOutput();
    BOOL GetActivePouSupportInOut();
    BOOL GetActivePouSupportExternal();
    BOOL IsActivePouGVL();

    IUnknown* GetProjManInterface();

    CString GetTypeForClassText(const CString& strText);
    CString GetTextForClassType(const CString& strType);

private:
    void InitStaticData();
    void GetLibrariesFromProject();
    void ReadLibraryInfos(CLibraryContent* pLibCont);
    void ReadLibraryConstants(CLibraryContent* pLibCont, I4CLAccess* pIAccess);

    void ClearLibraries();
    CString RemoveLibraryInfo(CString strPath);
    void ClearProjectPous();
    void ClearProjectConstants();
    void ClearGlobalInstances();
    void ClearLocalInstances();

    void FillVarDeclInfoFromKAD();
    BOOL UpdateVarDeclInfo();

    HRESULT AdviseProjEventHandler();
    void UnadviseProjEventHandler();

    void ProcessLocalInstance(CXMLNode& node);

    CString WriteHeaderFile(const CString& crstrLibFilePath, const CString& crstrPouName, 
                                          const CString& crstrFileContents);

public:
    // *****************
    // object data lists:

    // list with all libraries and their data
    CList<CLibraryContent*, CLibraryContent*>   m_listLibraries;   
    // list with all pous defined in the project
    CList<CPouVarObject*, CPouVarObject*>       m_listProjectPous;
    // list with all constants defined in the project
    CList<CPouVarObject*, CPouVarObject*>       m_listProjectConstants;
    // list with all global variables defined in the project
    CList<CVarObject*, CVarObject*>             m_listGlobalInstances;
    // list with all variables defined in the selected pou
    CList<CVarObject*, CVarObject*>             m_listLocalInstances;

    // holds all classes of pous that are stored in a library
    CList<CString, CString&>                    m_listLibPouClasses;
    CList<CClassInfo*, CClassInfo*>             m_listLibClassInfos;

    // infos of objects groups
    CMap<CString, LPCTSTR, CGroupContainer*, CGroupContainer*>   m_mapGVLGroups;
    CMap<CString, LPCTSTR, CGroupContainer*, CGroupContainer*>   m_mapConstGroups;


private:
    // *****************
    // internal data:

    // list with the libraries, this list is used to update the library data
    CList<CString, CString&>                    m_listLibraryPaths;

    // the name of the pou that has been selected: this is needed to update
    // the local instances: which are all variables declared in that pou.
    CString m_strActivePouName;
    CString m_strActivePouFileName;
    CString m_strActivePouType;    //RESOURCE, PROGRAM, ...
    CString m_strActivePouIdPath;
    BOOL    m_bActivePouSupportVarDecl;//true if KAD type has VAR_DECL (RESOURCE has no var_decl...)
    GUID    m_guidActiveVarHandler;
    BOOL    m_bRetain;
    BOOL    m_bInput;
    BOOL    m_bOutput;
    BOOL    m_bInOut;

    // var decl infos read from kad
    CMap<CString, LPCTSTR, CVarDeclInfo*, CVarDeclInfo*> m_mapVarDecl;


    // pointer to other needed objects
    CPouVarSelCtrl* m_pPouVarSelCtrl;
    CPouSelDlg*     m_pPouSelDlg;
    CVarSelDlg*     m_pVarSelDlg;
    IUnknown*       m_pProjManUnk;
    IUnknown*       m_pKADManUnk;
    I4CLOpen*       m_pLibOpen;

    CComObject<CProjManEventsImpl>* m_pProjEventHandler;
	DWORD                           m_dwProjEventHandlerCookie;

    CString m_strProjName;
    CString m_strClassTreeIdPath;

    ICG_AutoDecl* m_pIAutoDecl; //used for Osai special predefined var feature
};



#endif // !defined(AFX_POUVARSELDATA_H__63B71815_3A5B_46C6_8C09_CDD6E7ACF209__INCLUDED_)
