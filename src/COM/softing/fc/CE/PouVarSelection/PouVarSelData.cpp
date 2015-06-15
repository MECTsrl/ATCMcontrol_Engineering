





#include "stdafx.h"
#include "PouVarSelData.h"

#include "PouVarSelCtrl.h"
#include "PouSelDlg.h"
#include "VarSelDlg.h"
#include "DragDropHelper\LinkFile.h"
//#include "MangledName.h"

#include "CEProjInfo.h"
#include "CEProjInfo_i.c"
#include "CEBackparse.h"
#include "CEBackparse_i.c"
#include "CEKadMan.h"
#include "CEKadMan_i.c"

#include "CG_AutoDecl.h"
#include "CG_AutoDecl_i.c"

#include "CG_4CLAccess.h"
#include "CG_4CLAccess_i.c"

#include "DragDropHelper\PouVarDefs.h"

#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_string.h"
#include "utilif.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @exception       -
 * @see             
*/
CPouVarSelData::CPouVarSelData()
{
    HRESULT hr;

    m_pPouVarSelCtrl = NULL;
    m_pPouSelDlg = NULL;
    m_pVarSelDlg = NULL;
    m_pProjManUnk = NULL;
    m_pKADManUnk = NULL;
    m_pLibOpen = NULL;
    m_pProjEventHandler = NULL;
    m_dwProjEventHandlerCookie = NULL;
    
    hr = CoCreateInstance(CLSID_FC4CLAccess,
		                  NULL,
		                  CLSCTX_INPROC_SERVER,
		                  __uuidof(I4CLOpen), 
						  (LPVOID *)&m_pLibOpen);
    if (hr!=S_OK || m_pLibOpen==NULL)
    {
        // TODO: error handling
        ;
    }
    
    m_pIAutoDecl = NULL;
}



//------------------------------------------------------------------*
/**
 * destructror
 *
 * @exception       -
 * @see             
*/
CPouVarSelData::~CPouVarSelData()
{
    UnadviseProjEventHandler();

    if (m_pProjManUnk!=NULL)
    {
        m_pProjManUnk->Release();
    }

    if (m_pKADManUnk!=NULL)
    {
        m_pKADManUnk->Release();
    }

    if (m_pLibOpen != NULL)
    {
        m_pLibOpen->Release();
    }

    POSITION pos = m_listLibClassInfos.GetHeadPosition();
    while(pos)
    {
        CClassInfo* pClassInfo = NULL;
        pClassInfo = m_listLibClassInfos.GetNext(pos);
        if (pClassInfo)
        {
            delete pClassInfo;
        }
    }
    m_listLibClassInfos.RemoveAll();

    pos = m_mapGVLGroups.GetStartPosition();
    while(pos)
    {
        CGroupContainer* pCont = NULL;
        CString str;
        m_mapGVLGroups.GetNextAssoc(pos, str, pCont);
        if (pCont)
        {
            delete pCont;
        }
    }
    m_mapGVLGroups.RemoveAll();

    pos = m_mapConstGroups.GetStartPosition();
    while(pos)
    {
        CGroupContainer* pCont = NULL;
        CString str;
        m_mapConstGroups.GetNextAssoc(pos, str, pCont);
        if (pCont)
        {
            delete pCont;
        }
    }
    m_mapConstGroups.RemoveAll();

    pos = m_mapVarDecl.GetStartPosition();
    while(pos)
    {
        CVarDeclInfo* pVarDecl = NULL;
        CString str;
        m_mapVarDecl.GetNextAssoc(pos, str, pVarDecl);
        if (pVarDecl)
        {
            delete pVarDecl;
        }
    }
    m_mapVarDecl.RemoveAll();


    ClearLibraries();
    ClearProjectPous();
    ClearProjectConstants();
    ClearGlobalInstances();
    ClearLocalInstances();

    if(m_pIAutoDecl)
        m_pIAutoDecl->Release();
}


//------------------------------------------------------------------*
/**
 * Init.
 *
 * @param           pPouVarSelCtrl: pointer to the ActiveX control class
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::Init(CPouVarSelCtrl* pPouVarSelCtrl)
{
    HRESULT hr;

    m_pPouVarSelCtrl = pPouVarSelCtrl;
    ASSERT(m_pPouVarSelCtrl);
    if (m_pPouVarSelCtrl)
    {
        // get pointers to the two dlg pages
        m_pPouSelDlg = m_pPouVarSelCtrl->GetPouSelDlg();
        ASSERT(m_pPouSelDlg);
        m_pVarSelDlg = m_pPouVarSelCtrl->GetVarSelDlg();
        ASSERT(m_pVarSelDlg);
   
        // get interface pointer for kad manager and project manager
        hr = m_pPouVarSelCtrl->Fire_GetKadManager(&m_pKADManUnk);  
        ASSERT(m_pKADManUnk);

        hr = m_pPouVarSelCtrl->Fire_GetProjectManager(&m_pProjManUnk);

        ASSERT(m_pProjManUnk);
        if (m_pProjManUnk)
        {
            CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
            pProjInfo = m_pProjManUnk;
            if (!(!pProjInfo))
            {
                CComBSTR sProjName;
                hr = pProjInfo->getProjectName(&sProjName);
                if (hr == S_OK)
                {
                    m_strProjName = sProjName;
                    m_strClassTreeIdPath = (CString)CE_XML_IDPATH_SEP + 
                                           m_strProjName +
                                           (CString)CE_XML_IDPATH_SEP_DOT + 
                                           (CString)CE_XML_IDPATH_CLASSVIEW;
                }

                InitStaticData();
                UpdateLibraries();
                UpdateProjectData();             
            }
        }
    }

    ASSERT(m_pProjManUnk!=NULL);
    if (m_pProjManUnk!=NULL)
    {
        AdviseProjEventHandler();
    }
}



//------------------------------------------------------------------*
/**
 * InitStaticData initializes all static lists.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::InitStaticData()
{
    HRESULT hr;
    // this information should be available in the kad:
    //m_listLibPouClasses.AddTail((CString)_T("FUNCTION_BLOCK"));
    //m_listLibPouClasses.AddTail((CString)_T("FUNCTION"));
    //m_listLibPouClasses.AddTail((CString)_T("Constants"));

    CComQIPtr<IClasses, &IID_IClasses> pKadMan;
    pKadMan = m_pKADManUnk;

    if (!pKadMan)
    {
        return;
    }

    CComPtr<IClass> classGroup;
    CComPtr<IEnumClasses> pEnumClasses;
    hr = pKadMan->GetClassEnumerator(&pEnumClasses);
    if (hr!=S_OK)
    {
        return;
    }

    pEnumClasses->Reset();

    ULONG n;
    hr = pEnumClasses->Next(1, &classGroup, &n);
    if (!SUCCEEDED(hr))
    {
        return;
    }

    while (classGroup)
    {
        CComBSTR sAttrText;
        CComBSTR sAttrSection;
        CComBSTR sAttrClassesType;
        BOOL     bPouSel;

        classGroup->GetGroupText(&sAttrText);
        classGroup->GetSection(&sAttrSection);
        classGroup->GetClassType(&sAttrClassesType);
        hr = classGroup->GetPouSel(&bPouSel);
                
        if (hr==S_OK && bPouSel)
        {
            CClassInfo* pClassInfo = new CClassInfo;
            if (pClassInfo)
            {
                pClassInfo->strClassGroupIdPath = m_strClassTreeIdPath + 
                           (CString)CE_XML_IDPATH_SEP + (CString)sAttrSection;
                pClassInfo->strClassText = sAttrText;
                pClassInfo->strClassType = sAttrClassesType;

                m_listLibClassInfos.AddTail(pClassInfo);
                m_listLibPouClasses.AddTail(pClassInfo->strClassText);
            }
        }

        classGroup.Release();
        hr = pEnumClasses->Next(1, &classGroup, &n);
    }

    // add const node
    CClassInfo* pClassInfo = new CClassInfo;
    if (pClassInfo)
    {
        pClassInfo->strClassGroupIdPath = _T("");
        pClassInfo->strClassText = _T("Constants");
        pClassInfo->strClassType = _T("CONST");

        m_listLibClassInfos.AddTail(pClassInfo);
        m_listLibPouClasses.AddTail(pClassInfo->strClassText);
    }

    // add data type
    pClassInfo = NULL;
    pClassInfo = new CClassInfo;
    if (pClassInfo)
    {
        pClassInfo->strClassGroupIdPath = m_strClassTreeIdPath +
                                          (CString)CE_XML_IDPATH_SEP + 
                                         _T("Classes.DataTypes");
        pClassInfo->strClassType = _T("STRUCT");

        m_listLibClassInfos.AddTail(pClassInfo);
    }

    FillVarDeclInfoFromKAD();
}


void CPouVarSelData::HandleUpdateClassTree(const CString& strChangedIdPath)
{
    if (m_pPouVarSelCtrl->IsDisabled())
    {
        return;
    }

    
    BOOL bUpdLibs = FALSE;
    BOOL bUpdProjPous = FALSE;
    BOOL bUpdProjConsts = FALSE;
    BOOL bUpdGlobInst = FALSE;
    BOOL bUpdLocInst = FALSE;
    BOOL bChanged = FALSE;
    CString strLibIdPath = m_strClassTreeIdPath + 
                           (CString)CE_XML_IDPATH_SEP + 
                           (CString)CE_CP_LIBS;

    CString strConstPath = m_strClassTreeIdPath + 
                           (CString)CE_XML_IDPATH_SEP + 
                           (CString)CE_CP_CONSTS;
    CString strGVLPath = m_strClassTreeIdPath + 
                         (CString)CE_XML_IDPATH_SEP + 
                         (CString)CE_CP_GVLS;

   
    if (!bUpdLibs)
    {
        if (IsPrefixOfIdPath(strLibIdPath, strChangedIdPath)
            || strChangedIdPath.Compare(m_strClassTreeIdPath)==0)
        {
            UpdateLibraries();
            bUpdLibs = TRUE;
            bChanged = TRUE;
        }
    }

    if (!bUpdProjPous)
    {
        if (IsPrefixOfIdPath(m_strClassTreeIdPath, strChangedIdPath))
        {
            UpdateProjectPous(strChangedIdPath);
            bUpdProjPous = TRUE;
            bChanged = TRUE;
        }
    }

    if (!bUpdProjConsts)
    {
        if (IsPrefixOfIdPath(strConstPath, strChangedIdPath)
            || strChangedIdPath.Compare(m_strClassTreeIdPath)==0)
        {
            UpdateProjectConstants();
            bUpdProjConsts = TRUE;
            bChanged = TRUE;
        }
    }

    if (!bUpdGlobInst)
    {
        if (IsPrefixOfIdPath(strGVLPath, strChangedIdPath)
            || strChangedIdPath.Compare(m_strClassTreeIdPath)==0)
        {
            UpdateGlobalInstances();
            bUpdGlobInst = TRUE;
            bChanged = TRUE;
        }
    }

    if (!bUpdLocInst)
    {
        if (bUpdGlobInst || //EF added ||bUpdGlobInst, #import gvl changed=> causes a local inst change!
            (!m_strActivePouIdPath.IsEmpty() && 
             IsPrefixOfIdPath(m_strActivePouIdPath, strChangedIdPath)) ||
            strChangedIdPath.Compare(m_strClassTreeIdPath)==0)
        {
            UpdateLocalInstances();
            bUpdLocInst = TRUE;
            bChanged = TRUE;
        }
    }
   

    if (bChanged)
    {
        FinishUpdate();
    }
}




//------------------------------------------------------------------*
/**
 * UpdateLibraries.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateLibraries()
{
    BOOL changed = FALSE;

    // store old libraries
    CList<CString, CString&> listOldLibs;

    POSITION pos = m_listLibraryPaths.GetHeadPosition();
    while(pos)
    {
        listOldLibs.AddTail(m_listLibraryPaths.GetNext(pos));
    }

    m_listLibraryPaths.RemoveAll();
    GetLibrariesFromProject();

    // find new and removed libs
    pos = m_listLibraryPaths.GetHeadPosition();
    while(pos)
    {
        CString strLibPath = m_listLibraryPaths.GetNext(pos);
        // new library
        if (!listOldLibs.Find(strLibPath))
        {           
            CLibraryContent* pLibCont = new CLibraryContent();
            ASSERT(pLibCont);
            if (pLibCont)
            {
                pLibCont->strLibPath = strLibPath;
                m_listLibraries.AddTail(pLibCont);
                ReadLibraryInfos(pLibCont);
            }

            changed = TRUE;
        }
    }
    pos = listOldLibs.GetHeadPosition();
    while(pos)
    {
        CString strLibPath = listOldLibs.GetNext(pos);
        // removed library
        if (!m_listLibraryPaths.Find(strLibPath))
        {
            CString strLibName = RemoveLibraryInfo(strLibPath);
            changed = TRUE;
        }
    }

    CList<CString, CString&> listReloadLibs;

    pos = m_listLibraries.GetHeadPosition();
    while(pos)
    {
        CLibraryContent* pLibCont = NULL;
        pLibCont = m_listLibraries.GetNext(pos);
        if (!pLibCont)
        {
            continue;
        }
        CFileStatus status;
        if (CFile::GetStatus(pLibCont->strLibPath, status))
        {
            if (status.m_mtime != pLibCont->timeModTime)
            {
                listReloadLibs.AddTail(pLibCont->strLibPath);
                CString strLibName = RemoveLibraryInfo(pLibCont->strLibPath);
                changed = TRUE;
            }
        }
    }

    pos = listReloadLibs.GetHeadPosition();
    while(pos)
    {
        CString strLibName = listReloadLibs.GetNext(pos);
        CLibraryContent* pLibCont = new CLibraryContent();
        ASSERT(pLibCont);
        if (pLibCont)
        {
            pLibCont->strLibPath = strLibName;
            m_listLibraries.AddTail(pLibCont);
            ReadLibraryInfos(pLibCont);
            changed = TRUE;
        }
    }

    return;
}


//------------------------------------------------------------------*
/**
 * GetLibrariesFromProject gets all libraries included in the project.
 * The list m_listLibraryPaths will be filled with full path and file 
 * name of the libraries.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::GetLibrariesFromProject()
{
    // get all libraries from project
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return;
    }

    SAFEARRAY* parrLibraries;
    
    HRESULT hr = pProjInfo->getAllLibraries(&parrLibraries);
    if(hr!=S_OK)
    {
        return;
    }   

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrLibraries, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return;
    }
    for (i = 0; i < ((parrLibraries)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        CComBSTR sFullLib;
        hr = pProjInfo->ReplaceMacros(bstr, &sFullLib);
        if (hr==S_OK)
        {
            str = (CString)sFullLib;
        }
        else
        {
            str = (CString)bstr;
        }
        m_listLibraryPaths.AddTail(str);
    }

    ::SafeArrayUnaccessData(parrLibraries);		
    ::SafeArrayDestroy(parrLibraries);
}


//------------------------------------------------------------------*
/**
 * ReadLibraryInfos fills the library information for one library.
 *
 * @param           pLibCont: pointer to the library information, 
 *                          which will be filled with information.
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::ReadLibraryInfos(CLibraryContent* pLibCont)
{
    HRESULT hr;

    CString strLibPath = pLibCont->strLibPath;

    // extract lib name
    int lastSlash=-1;
    int lastPoint=-1;
    lastSlash = strLibPath.ReverseFind(_T('\\'));
    lastSlash = (lastSlash<0) ? 0 : lastSlash;
    lastPoint = strLibPath.ReverseFind(_T('.'));
    lastPoint = (lastPoint<0) ? strLibPath.GetLength() : lastPoint;

    CString strLibName ;
    strLibName = strLibPath.Mid(lastSlash+1, lastPoint-lastSlash-1);
    pLibCont->strLibName = strLibName;

    if (m_pLibOpen==NULL)
    {
        return;
    }
    
    CFileStatus fileStatus;
    CFile::GetStatus(strLibPath, fileStatus);
    pLibCont->timeModTime = fileStatus.m_mtime;

    BOOL bWrite = FALSE;
    CComBSTR sFileName(strLibPath);
    CComBSTR sError;
    I4CLAccess* pIAccess = NULL;
    hr = m_pLibOpen->Open(bWrite, sFileName, &sError, &pIAccess);
    if (hr!=S_OK || pIAccess==NULL)
    {
        return;
    }

    SAFEARRAY* parrPous;
    
    hr = pIAccess->GetAllPOUs(&parrPous);
    if(hr!=S_OK)
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return;
    }   

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrPous, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return;
    }
    for (i = 0; i < ((parrPous)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = (CString)bstr;
        if ((str[0]==_T('_')) )
        {
            // internal object, don't show
            continue;
        }
        if (str.CompareNoCase(PVS_LIB_CONST_POU)==0)
        {
            ReadLibraryConstants(pLibCont, pIAccess);
            continue;
        }

        CPouObject* pPouObj = new CPouObject;
        ASSERT(pPouObj);
        if (pPouObj)
        {
            CComBSTR sPouName(str);

            // get supported targets
            SAFEARRAY*    sTrgTypes;

            hr = pIAccess->GetTargetTypesOfPOU(sPouName, &sTrgTypes);
            if(!FAILED(hr))
            {     
                unsigned long ul2;
                //Get the target types of the given POU
                for(ul2=0; ul2<sTrgTypes->rgsabound[0].cElements; ul2++)
                {
                    BSTR sTrg;
                    sTrg = ((BSTR*)(sTrgTypes->pvData))[ul2];

                    CString strTargetName = sTrg;

                    pPouObj->m_listTargets.AddTail(strTargetName);
                }
                SafeArrayDestroy(sTrgTypes);   
            }

            if ((pPouObj->m_listTargets).IsEmpty())
            {
                delete pPouObj;
                continue;
            }


            // read link file
            CString strLinkFileName;
            strLinkFileName = PVS_LINK_FILE;
            
            CComBSTR sLinkFileName(strLinkFileName);
            CComBSTR sFileCont;
            hr = pIAccess->GetFileAsBSTR(sPouName, NULL, sLinkFileName, &sFileCont);
            if (hr!=S_OK)
            {
                delete pPouObj;
                continue;
            }
            CString strFileCont(sFileCont);

            CLinkFile linkFile(strFileCont);
            if (pPouObj->InitFromLinkFile(&linkFile))
            {
                pPouObj->m_strName = str;
                pPouObj->m_strOrigin = strLibPath;
                pLibCont->pouList.AddTail((CPouVarObject*)pPouObj);
            }
            else
            {
                delete pPouObj;
            }

           
       }
    }

    ::SafeArrayUnaccessData(parrPous);		
    ::SafeArrayDestroy(parrPous);

    m_pLibOpen->Close(pIAccess);
    pIAccess->Release();
}


//------------------------------------------------------------------*
/**
 * ReadLibraryConstants reads all constants from a library.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::ReadLibraryConstants(CLibraryContent* pLibCont, I4CLAccess* pIAccess)
{
    ASSERT(pIAccess);
    if (!pIAccess)
    {
        return;
    }

    CComBSTR sConstPou(PVS_LIB_CONST_POU);
    CComBSTR sTargetType;
    CComBSTR sCheckOutDir;
    SAFEARRAY* parrFiles;

    ICEBackparse* pBackparser = NULL;

    HRESULT hr;
    hr = pIAccess->GetFileNames(sConstPou, sTargetType,
                                &parrFiles);
    if (hr!=S_OK)
    {
        return;
    }

    // get checkout dir
    ASSERT(m_pProjManUnk);
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = m_pProjManUnk;
    if (!pProjInfo)
    {
        return;
    }
    pProjInfo->getProjectPath(&sCheckOutDir);
    sCheckOutDir += PVS_LIB_CHECKOUDIR;

    // create backparser
    ASSERT(m_pKADManUnk);
    CComQIPtr<IFiles, &IID_IFiles> kadFiles;
    kadFiles = m_pKADManUnk;
    if (!kadFiles)
    {
        return;
    }
    IFile* pIFile;
    CComBSTR sExtension(PVS_LIB_CONSTEXT);
    hr = kadFiles->GetFileByExtension(sExtension, &pIFile);
    if (hr!=S_OK)
    {
        return;
    }
    GUID parserGUID;
    hr = pIFile->GetParser(&parserGUID);
    pIFile->Release();
    if (hr!=S_OK)
    {
        return;
    }
    hr = CoCreateInstance(parserGUID, NULL, CLSCTX_INPROC_SERVER,
		                  __uuidof(ICEBackparse), //IID_ICEBackparse,
						  (LPVOID *)&pBackparser);
    if (hr!=S_OK)
    {
        return;
    }

    // get constant file names, extract and parse
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    hr = ::SafeArrayAccessData(parrFiles, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        pBackparser->Release();
        return;
    }

    for (i = 0; i < ((parrFiles)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = (CString)bstr;

        CString strFileName(sCheckOutDir);
        strFileName += str;

        CString strFileExtension;
        int iFind = strFileName.ReverseFind(_T('.'));
        if (iFind>0)
        {
            strFileExtension = strFileName.Mid(iFind);
        }

        if (strFileExtension.CompareNoCase(_T(".cst"))!=0)
        {
            continue;
        }
        
        // get constant file
        CComBSTR sErrorDesc;
        hr = pIAccess->CheckOut(sConstPou, sTargetType, bstr,
                                sCheckOutDir, NULL, &sErrorDesc);
        if (hr!=S_OK)
        {
            continue;
        }

        // call backparser
        CComBSTR sFileName(strFileName);
        CComBSTR sXMLText;
        hr = pBackparser->doBackparse(sFileName, pProjInfo, &sXMLText);
        if (hr!=S_OK)
        {
            // remove constant file
            try
            {
                CFile::Remove(strFileName);
            }
            catch (CException* e)
            {
               e->Delete();
            }
            continue;
        }

        // remove constant file
        try
        {
            CFile::Remove(strFileName);
        }
        catch (CException* e)
        {
           e->Delete();
        }
        
        CXMLDocument xmlDoc;
        if (!xmlDoc.LoadXMLString((CString)sXMLText))
        {
            continue;
        }

        CXMLNode xmlFileNode;
        CXMLNodeList xmlConstNodeList;
        CXMLNode xmlConstNode;

        if (!xmlDoc.GetRootNode(xmlFileNode))
        {
            continue;
        }
        if (!xmlFileNode.GetChildNodeList(xmlConstNodeList))
        {
            continue;
        }
        xmlConstNodeList.Reset();
        while (xmlConstNodeList.Next(xmlConstNode))
        {
            CVarObject* pConstObj = new CVarObject();

            if (pConstObj->InitFromXMLNode(&xmlConstNode))
            {
                pConstObj->m_strOrigin = pLibCont->strLibName;
                pLibCont->pouList.AddTail(pConstObj);
            }
            else
            {
                delete pConstObj;
            }
        }
    }

    ::SafeArrayUnaccessData(parrFiles);		
    ::SafeArrayDestroy(parrFiles);

    pBackparser->Release();
    return;
}



//------------------------------------------------------------------*
/**
 * UpdateProjectData reinitializes all data coming from the project..
 *
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateProjectData()
{
    UpdateProjectPous(m_strClassTreeIdPath);
    UpdateProjectConstants();
    UpdateGlobalInstances();
    UpdateLocalInstances();
}


//------------------------------------------------------------------*
/**
 * UpdateProjectPous gets all pous from the project.
 *
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateProjectPous(const CString strUpdatePath)
{
    HRESULT hr;
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return;
    }

    ClearProjectPous();

    // update project pous
    POSITION posClassTypes;
    posClassTypes = m_listLibClassInfos.GetHeadPosition();

    while(posClassTypes)
    {
        CClassInfo* pClassInfo = NULL;
        pClassInfo = m_listLibClassInfos.GetNext(posClassTypes);
        if (!pClassInfo)
        {
            continue;
        }

        CString strIdPathClassGroup = pClassInfo->strClassGroupIdPath;
        CString strClassType = pClassInfo->strClassType;

        if (strIdPathClassGroup.IsEmpty())
        {
            // do not handle consts here
            continue;
        }

        CComBSTR sId = strIdPathClassGroup;
        IUnknown* pXMLPtr = NULL;
        hr = pProjInfo->getXMLNodePtr(sId, &pXMLPtr);

        if (hr!=S_OK || pXMLPtr==NULL)
        {
            return;
        }

        CXMLNode xmlClassGroupNode;
        if (!xmlClassGroupNode.SetIXMLNode(pXMLPtr))
        {
            pXMLPtr->Release();
            return;
        }
        pXMLPtr->Release(); 

        // all sub nodes are GVL-File nodes
        CXMLNodeList xmlClassNodeList;
        if (!xmlClassGroupNode.GetChildNodeList(xmlClassNodeList))
        {
            return;
        }

        CXMLNode xmlClassNode;
        xmlClassNodeList.Reset();
        while(xmlClassNodeList.Next(xmlClassNode))
        {
            CPouObject* pPouObj = new CPouObject();
            ASSERT(pPouObj);
            if (!pPouObj)
            {
                continue;
            }
            if (pPouObj->InitFromXMLNode(&xmlClassNode))
            {
                m_listProjectPous.AddTail((CPouVarObject*)pPouObj);
            }
            else
            {
                delete pPouObj;
            }
        }

    }
}


//------------------------------------------------------------------*
/**
 * UpdateProjectConstants get all constants from the project.
 *
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateProjectConstants()
{
    HRESULT hr;
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return;
    }

    // get xml node of constants group
    CString strConstGroupIdPath = m_strClassTreeIdPath + 
                                (CString)CE_XML_IDPATH_SEP + 
                                (CString)CE_CP_CONSTS;
    
    CComBSTR sId = strConstGroupIdPath;
    IUnknown* pXMLPtr;
    hr = pProjInfo->getXMLNodePtr(sId, &pXMLPtr);

    CXMLNode xmlConstGroupNode;
    if (!xmlConstGroupNode.SetIXMLNode(pXMLPtr))
    {
        if(pXMLPtr)
        {
            pXMLPtr->Release();
        }
        return;
    }

    pXMLPtr->Release();
        
    // all sub nodes are Constant-File nodes
    CXMLNodeList xmlConstFileList;
    if (!xmlConstGroupNode.GetChildNodeList(xmlConstFileList))
    {
        return;
    }


    // reset used attribute of group container info:
    POSITION posMap;
    posMap = m_mapConstGroups.GetStartPosition();
    while(posMap)
    {
        CGroupContainer* pCont = NULL;
        CString strKey;
        m_mapConstGroups.GetNextAssoc(posMap, strKey, pCont);
        if (pCont)
        {
            pCont->bUsed = FALSE;
        }
    }

    // look for all new constant files
    CXMLNode xmlConstFile;
    xmlConstFileList.Reset();
    while(xmlConstFileList.Next(xmlConstFile))
    {
        CString strFileName;
        CString strIdPath;
        CString strParseTime;
        CString strLowerFileName;

        xmlConstFile.GetAttribute(CE_XMLATTR_PARSERSOURCE, strFileName);
        xmlConstFile.GetAttribute(CE_XMLATTR_MODTIME, strParseTime);
        xmlConstFile.GetIdPath(strIdPath);

        strLowerFileName = strFileName;
        strLowerFileName.MakeLower();

        BOOL bReparse = TRUE;
        CGroupContainer* pConstContainer = NULL;
        // lookup gvl file
        if (m_mapConstGroups.Lookup(strLowerFileName, pConstContainer))
        {
            pConstContainer->bUsed = TRUE;
            if (pConstContainer->strLastParseTime.Compare(strParseTime)==0)
            {
                // if info exists and parse time has not changed, do not reparse
                bReparse = FALSE;
            }
        }
    
        if (!bReparse)
        {
            continue;
        }

        // do reparsing
        // container info already exists?
        if (pConstContainer==NULL)
        {
            pConstContainer = new CGroupContainer;
            pConstContainer->strFileName = strFileName;
            pConstContainer->strIdPath = strIdPath;
            pConstContainer->strLastParseTime = strParseTime;
            pConstContainer->firstMemberPos = NULL;
            pConstContainer->lastMemberPos = NULL;
            pConstContainer->bUsed = TRUE;
            m_mapConstGroups.SetAt(strLowerFileName, pConstContainer);
        }
        else
        {
            // delete old members
            POSITION pos = pConstContainer->firstMemberPos;
            BOOL bLast = FALSE;
            while (pos && !bLast)
            {
                if (pos == pConstContainer->lastMemberPos)
                {
                    bLast = TRUE;
                }
                POSITION posThis = pos;
                CPouVarObject* pVarObj = NULL;
                pVarObj = m_listProjectConstants.GetNext(pos);
                delete pVarObj;
                m_listProjectConstants.RemoveAt(posThis);
            }

            pConstContainer->firstMemberPos = NULL;
            pConstContainer->lastMemberPos = NULL;
        }

        // read new contents of gvl file

        CXMLNodeList xmlConstVars;
        if (!xmlConstFile.GetChildNodeList(xmlConstVars))
        {
            continue;
        }

        CXMLNode xmlVar;
        xmlConstVars.Reset();
        BOOL bFirst = TRUE;
        POSITION posVar = NULL;
        while(xmlConstVars.Next(xmlVar))
        {
            CString strType;
            xmlVar.GetAttribute(CE_XMLATTR_TYPE, strType);
            if (strType.Compare(PVS_XML_CONST)==0)
            {
                CVarObject* pConstObj = new CVarObject();
                ASSERT(pConstObj);
                if (!pConstObj)
                {
                    continue;
                }
        
                if (pConstObj->InitFromXMLNode(&xmlVar))
                {
                    pConstObj->m_strOrigin = _T("");
                    posVar = m_listProjectConstants.AddTail(pConstObj);
                    if (bFirst)
                    {
                        pConstContainer->firstMemberPos = posVar;
                        bFirst = FALSE;
                    }
                }
                else
                {
                    delete pConstObj;
                }
            }
        }
        pConstContainer->lastMemberPos = posVar;
    }


    // walk over all group files and delete no longer used
    posMap = m_mapConstGroups.GetStartPosition();
    while(posMap)
    {
        CGroupContainer* pCont = NULL;
        CString strKey;
        m_mapConstGroups.GetNextAssoc(posMap, strKey, pCont);
        if (pCont && !(pCont->bUsed))
        {  
            POSITION pos = pCont->firstMemberPos;
            BOOL bLast = FALSE;
            while (pos && !bLast)
            {
                if (pos == pCont->lastMemberPos)
                {
                    bLast = TRUE;
                }
                POSITION posThis = pos;
                CPouVarObject* pVarObj = NULL;
                pVarObj = m_listProjectConstants.GetNext(pos);
                delete pVarObj;
                m_listProjectConstants.RemoveAt(posThis);
            }

            pCont->firstMemberPos = NULL;
            pCont->lastMemberPos = NULL;
            delete pCont;
            m_mapConstGroups.RemoveKey(strKey);   
        }
    }

}



//------------------------------------------------------------------*
/**
 * UpdateGlobalInstances get all global variables from the project.
 *
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateGlobalInstances()
{
    HRESULT hr;
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return;
    }

    // get xml node of gvl group
    CString strGVLGroupIdPath = m_strClassTreeIdPath + 
                                (CString)CE_XML_IDPATH_SEP + 
                                (CString)CE_CP_GVLS;
    
    CComBSTR sId = strGVLGroupIdPath;
    IUnknown* pXMLPtr;
    hr = pProjInfo->getXMLNodePtr(sId, &pXMLPtr);
    if(hr != S_OK)
    {
        return;
    }

    CXMLNode xmlGVLGroupNode;
    if (!xmlGVLGroupNode.SetIXMLNode(pXMLPtr))
    {
        pXMLPtr->Release();
        return;
    }

    pXMLPtr->Release();
        
    // all sub nodes are GVL-File nodes
    CXMLNodeList xmlGVLFileList;
    if (!xmlGVLGroupNode.GetChildNodeList(xmlGVLFileList))
    {
        return;
    }

    // reset used attribute of group container info:
    POSITION posMap;
    posMap = m_mapGVLGroups.GetStartPosition();
    while(posMap)
    {
        CGroupContainer* pCont = NULL;
        CString strKey;
        m_mapGVLGroups.GetNextAssoc(posMap, strKey, pCont);
        if (pCont)
        {
            pCont->bUsed = FALSE;
        }
    }

    // look for all new gvl files
    CXMLNode xmlGVLFile;
    xmlGVLFileList.Reset();
    while(xmlGVLFileList.Next(xmlGVLFile))
    {
        CString strFileName;
        CString strIdPath;
        CString strParseTime;
        CString strLowerFileName;

        xmlGVLFile.GetAttribute(CE_XMLATTR_PARSERSOURCE, strFileName);
        xmlGVLFile.GetAttribute(CE_XMLATTR_MODTIME, strParseTime);
        xmlGVLFile.GetIdPath(strIdPath);

        strLowerFileName = strFileName;
        strLowerFileName.MakeLower();

        BOOL bReparse = TRUE;
        CGroupContainer* pGVLContainer = NULL;
        // lookup gvl file
        if (m_mapGVLGroups.Lookup(strLowerFileName, pGVLContainer))
        {
            ASSERT(pGVLContainer);
            pGVLContainer->bUsed = TRUE;
            if (pGVLContainer->strLastParseTime.Compare(strParseTime)==0)
            {
                // if info exists and parse time has not changed, do not reparse
                bReparse = FALSE;
            }
        }
    
        if (!bReparse)
        {
            continue;
        }

        // do reparsing
        // container info already exists?
        if (pGVLContainer==NULL)
        {
            pGVLContainer = new CGroupContainer;
            pGVLContainer->strFileName = strFileName;
            pGVLContainer->strIdPath = strIdPath;
            pGVLContainer->strLastParseTime = strParseTime;
            pGVLContainer->firstMemberPos = NULL;
            pGVLContainer->lastMemberPos = NULL;
            pGVLContainer->bUsed = TRUE;
            m_mapGVLGroups.SetAt(strLowerFileName, pGVLContainer);
        }
        else
        {
            // delete old members
            POSITION pos = pGVLContainer->firstMemberPos;
            BOOL bLast = FALSE;
            while (pos && !bLast)
            {
                if (pos == pGVLContainer->lastMemberPos)
                {
                    bLast = TRUE;
                }
                POSITION posThis = pos;
                CPouVarObject* pVarObj = NULL;
                pVarObj = m_listGlobalInstances.GetNext(pos);
                delete pVarObj;
                m_listGlobalInstances.RemoveAt(posThis);
            }

            pGVLContainer->firstMemberPos = NULL;
            pGVLContainer->lastMemberPos = NULL;
        }

        // read new contents of gvl file

        CXMLNodeList xmlGVLVars;
        if (!xmlGVLFile.GetChildNodeList(xmlGVLVars))
        {
            continue;
        }

        CXMLNode xmlVar;
        xmlGVLVars.Reset();
        BOOL bFirst = TRUE;
        POSITION posVar = NULL;
        while(xmlGVLVars.Next(xmlVar))
        {
            CString strType;
            xmlVar.GetAttribute(CE_XMLATTR_TYPE, strType);
            if (strType.Compare(PVS_XML_VAR_GLOBAL)==0)
            {
                CVarObject* pGlobObj = new CVarObject();
                ASSERT(pGlobObj);
                if (!pGlobObj)
                {
                    continue;
                }
        
                if (pGlobObj->InitFromXMLNode(&xmlVar))
                {
                    pGlobObj->m_strOrigin = _T("");
                    pGlobObj->m_strUserData = pGVLContainer->strFileName;
                    posVar = m_listGlobalInstances.AddTail(pGlobObj);
                    if (bFirst)
                    {
                        pGVLContainer->firstMemberPos = posVar;
                        bFirst = FALSE;
                    }
                }
                else
                {
                    delete pGlobObj;
                }
            }
        }
        pGVLContainer->lastMemberPos = posVar;
    }


    // walk over all group files and delete no longer used
    posMap = m_mapGVLGroups.GetStartPosition();
    while(posMap)
    {
        CGroupContainer* pCont = NULL;
        CString strKey;
        m_mapGVLGroups.GetNextAssoc(posMap, strKey, pCont);
        if (pCont && !(pCont->bUsed))
        {  
            POSITION pos = pCont->firstMemberPos;
            BOOL bLast = FALSE;
            while (pos && !bLast)
            {
                if (pos == pCont->lastMemberPos)
                {
                    bLast = TRUE;
                }
                POSITION posThis = pos;
                CPouVarObject* pVarObj = NULL;
                pVarObj = m_listGlobalInstances.GetNext(pos);
                delete pVarObj;
                m_listGlobalInstances.RemoveAt(posThis);
            }

            pCont->firstMemberPos = NULL;
            pCont->lastMemberPos = NULL;
            delete pCont;
            m_mapGVLGroups.RemoveKey(strKey);   
        }
    }

    //prepare the varsel to show the current state of gvls of the project:
    m_pVarSelDlg->FillScopeFilterList();
}
    
  


//------------------------------------------------------------------*
/**
 * UpdateLocalInstances with a change of the selected pou.
 *
 * @param          strPouName: the selected pou name 
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateLocalInstances(const CString& strPouFileName)
{
    m_strActivePouFileName = strPouFileName;
    UpdateLocalInstances();
}


//------------------------------------------------------------------*
/**
 * UpdateLocalInstances get all variables of a pou.
 *
 * @return          
 * @exception       -
 * @see             
*/
void CPouVarSelData::UpdateLocalInstances()
{
TAKE_TIME_INIT;
TAKE_TIME_START;

    ClearLocalInstances();


    // find pou file in class view
    m_bActivePouSupportVarDecl = FALSE;
    m_strActivePouName.Empty();
    m_strActivePouType.Empty();
    m_strActivePouIdPath.Empty();
    if (m_strActivePouFileName.IsEmpty())
    {
        return;
    }

    HRESULT hr;
   
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;

    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return;
    }

    CComBSTR sIdPath;
    CComBSTR sPouName = m_strActivePouFileName;

    hr = pProjInfo->getIdPathFromFileName(sPouName, &sIdPath); 
    if (hr!=S_OK)
    {
        return;
    }

    m_strActivePouIdPath = sIdPath;

    IUnknown* pUnk = NULL;

    hr = pProjInfo->getXMLNodePtr(sIdPath, &pUnk);

    if (hr!=S_OK)
    {
        return;
    }

    CXMLNode node;
    
    if (!node.SetIXMLNode(pUnk))
    {
        return;
    }


    ProcessLocalInstance(node);

    pUnk->Release();

TAKE_TIME_END(_T("PouVarSelData, UpdateLocalInstances: "));

}


void CPouVarSelData::ProcessLocalInstance(CXMLNode& node)
{
    BOOL found = FALSE;
    CList<CString, CString&> listGlobVarImports;

    // get source file name
    node.GetAttribute(CE_XMLATTR_ID, m_strActivePouName);
    node.GetAttribute(CE_XMLATTR_TYPE, m_strActivePouType);
    //node.GetIdPath(m_strActivePouIdPath);

    m_bActivePouSupportVarDecl = UpdateVarDeclInfo();
    if (!m_bActivePouSupportVarDecl)
    {
        return;
    }

    // extract all vars direct under that node
    CXMLNodeList nodeList;
    if (!node.GetChildNodeList(nodeList))
    {
        return;
    }

    CXMLNode childNode;
    nodeList.Reset();
    while(nodeList.Next(childNode))
    {
        CVarObject* pVarObj = new CVarObject();
        ASSERT(pVarObj);
        if (!pVarObj)
        {
            return;
        }
        if (pVarObj->InitFromXMLNode(&childNode))
        {
            pVarObj->m_strOrigin = _T("");
            m_listLocalInstances.AddTail(pVarObj);
            pVarObj->m_strUserData = m_strActivePouFileName;
        }
        else
        {
            delete pVarObj;

            // check if node is a global var import
            CString strType;
            CString strId;
            if (childNode.GetAttribute(CE_XMLATTR_ID, strId) &&
                childNode.GetAttribute(CE_XMLATTR_TYPE, strType))
            {
                if (strType.Compare(_T("GLOB_VAR_FILE_REF"))==0)
                {
                    listGlobVarImports.AddTail(strId);
                }
            }
        }
    }

    // Add global vars which come via import 
    POSITION posGlobImport = listGlobVarImports.GetHeadPosition();
    while(posGlobImport)
    {
        CString strGlobVarFile;
        strGlobVarFile = listGlobVarImports.GetNext(posGlobImport);
        strGlobVarFile.MakeLower();
        CGroupContainer* pContainer = NULL;
        if (m_mapGVLGroups.Lookup(strGlobVarFile, pContainer))
        {
            POSITION posGlobInst = pContainer->firstMemberPos;
            BOOL bFinish = FALSE;
            while(posGlobInst && (!bFinish))
            {
                if (posGlobInst == pContainer->lastMemberPos)
                {
                    bFinish = TRUE;
                }
                CVarObject* pGlobVarObj = NULL;
                pGlobVarObj = m_listGlobalInstances.GetNext(posGlobInst);
                if (pGlobVarObj)
                {
                    CVarObject* pVarObj = new CVarObject();
                    ASSERT(pVarObj);
                    if (!pVarObj)
                    {
                        return;
                    }

                    *pVarObj = *pGlobVarObj;
                    pVarObj->m_strOrigin = _T("");
                    pVarObj->m_strUserData = pContainer->strFileName;
                    m_listLocalInstances.AddTail(pVarObj);
                }
            }
        }
    }
}



void CPouVarSelData::FillVarDeclInfoFromKAD()
{
    HRESULT hr;
    ULONG lFetched;
    CComQIPtr<ITargets, &IID_ITargets> pTargets;

    
    //loop all targets and find any AutoDecl feature:
    pTargets = m_pKADManUnk;
    if (pTargets)
    {
        CComPtr<IEnumTargets> pTargetEnum;
        hr = pTargets->GetTargetEnumerator(&pTargetEnum);
        if (pTargetEnum)
        {
            CComQIPtr<ITarget> pTarget;
            while (S_OK == pTargetEnum->Next(1, &pTarget, &lFetched))
            {
                if (!pTarget)
                    continue;
                CComBSTR sTrgType;
                CComPtr<IIECFeatures> pIECFeatures;
                
                pTarget->GetExt(&sTrgType);
                pTarget->GetIECFeatures(&pIECFeatures);
                
                pTarget.Release();
                
                if(!pIECFeatures)
                    continue;
                GUID    clsid;
                CComBSTR sIniFile;
                if(pIECFeatures->GetAutoDecl(&clsid, &sIniFile)!=S_OK)
                    continue;
                CString strFullIni = sIniFile;
                if(!FC_StringIsAbsPath(strFullIni))
                {
                    UINT  nStrLen = 2*MAX_PATH;
                    TCHAR szIPath[2*MAX_PATH];
                    szIPath[0] = 0;
                    UTIL_GetInstallPath(szIPath, &nStrLen);
                    CString tmp = sIniFile;
                    strFullIni = szIPath;
                    strFullIni += '\\';
                    strFullIni += tmp;
                }
                ASSERT(!m_pIAutoDecl);
                CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
                            IID_ICG_AutoDecl, (void**)&m_pIAutoDecl);
                ASSERT(!(!m_pIAutoDecl));
                if(m_pIAutoDecl)
                {
                    CComBSTR sConfFile = strFullIni;
                    hr= m_pIAutoDecl->init(sTrgType, sConfFile);  
                    ASSERT(hr == S_OK);//if not, ?....
                }

                break; //stop on first target that has auto decl
                       //note that using auto decl feature is only 
                       //meaningfull for a single target system.
            }
        }
    }





    //get VAR_DECL info for all types:
    CComQIPtr<ITypes, &IID_ITypes> pTypes;
    pTypes = m_pKADManUnk;

    ASSERT(!(!pTypes));
    if (!pTypes)
    {
        return;
    }
    
    CComPtr<IEnumTypes> pTypeEnum;
    hr = pTypes->GetTypeEnumerator(&pTypeEnum);
    if (hr!=S_OK)
    {
        return;
    }
    ASSERT(!(!pTypeEnum));
    if (!pTypeEnum)
    {
        return;
    }

    CComPtr<IType> pType;

    while (S_OK == pTypeEnum->Next(1, &pType, &lFetched))
    {
        ASSERT(pType!=NULL);
        if (!pType)
        {
            continue;
        }

        CComBSTR sId;
        hr = pType->GetId(&sId);

        CString strType = sId;
        strType.MakeLower();

        CComPtr<IVarDecl> pIVarDecl;
        hr = pType->GetVarDecl(&pIVarDecl);
        
        pType.Release();

        if (hr!=S_OK)
        {
            continue;
        }


        CVarDeclInfo* pVarDeclInfo = NULL;
        pVarDeclInfo = new CVarDeclInfo;
        ASSERT(pVarDeclInfo);
        if (!pVarDeclInfo)
        {
            return;
        }

        hr = pIVarDecl->GetHandler(&(pVarDeclInfo->guidActiveVarHandler));
        if (hr!=S_OK)
        {
            delete pVarDeclInfo;
            continue;
        }
    
	    hr = pIVarDecl->GetRetain(&(pVarDeclInfo->bRetain));
        if (hr!=S_OK)
        {
            pVarDeclInfo->bRetain = TRUE;
        }
	    hr = pIVarDecl->GetInput(&(pVarDeclInfo->bInput));
        if (hr!=S_OK)
        {
            pVarDeclInfo->bInput  = TRUE;
        }
	    hr = pIVarDecl->GetOutput(&(pVarDeclInfo->bOutput));
        if (hr!=S_OK)
        {
            pVarDeclInfo->bOutput  = TRUE;
        }
	    hr = pIVarDecl->GetInOut(&(pVarDeclInfo->bInOut));
        if (hr!=S_OK)
        {
            pVarDeclInfo->bInOut  = TRUE;
        }

        m_mapVarDecl.SetAt(strType, pVarDeclInfo);

    }


  /*  CComBSTR sId = m_strActivePouType;
    CComPtr<IType> pType;
    hr = pTypes->GetTypeById(sId, &pType);
    if (hr!=S_OK)
    {
        return FALSE;
    }  

    CComPtr<IVarDecl> pIVarDecl;
    hr = pType->GetVarDecl(&pIVarDecl);
    if (hr!=S_OK)
    {
        return FALSE;
    }

    hr = pIVarDecl->GetHandler(&m_guidActiveVarHandler);
    if (hr!=S_OK)
    {
        return FALSE;
    }
    
	hr = pIVarDecl->GetRetain(&m_bRetain);
    if (hr!=S_OK)
    {
        m_bRetain = TRUE;
    }
	hr = pIVarDecl->GetInput(&m_bInput);
    if (hr!=S_OK)
    {
        m_bInput  = TRUE;
    }
	hr = pIVarDecl->GetOutput(&m_bOutput);
    if (hr!=S_OK)
    {
        m_bOutput  = TRUE;
    }
	hr = pIVarDecl->GetInOut(&m_bInOut);
    if (hr!=S_OK)
    {
        m_bInOut  = TRUE;
    }

    return TRUE;
    */
}


BOOL CPouVarSelData::UpdateVarDeclInfo()
{
    CString strType = m_strActivePouType;

    strType.MakeLower();

    CVarDeclInfo* pVarDeclInfo = NULL;

    if (!m_mapVarDecl.Lookup(strType, pVarDeclInfo))
    {
        return FALSE;
    }
    
    ASSERT(pVarDeclInfo);
    if (!pVarDeclInfo)
    {
        return FALSE;
    }

    m_guidActiveVarHandler = pVarDeclInfo->guidActiveVarHandler;
    m_bRetain = pVarDeclInfo->bRetain;
    m_bInput = pVarDeclInfo->bInput;
    m_bOutput = pVarDeclInfo->bOutput;
    m_bInOut = pVarDeclInfo->bInOut;

    return TRUE;
}


void CPouVarSelData::FinishUpdate()
{
    m_pPouSelDlg->InvalidateSelectionData();
    m_pPouSelDlg->InvalidateDlgData();
    m_pVarSelDlg->InvalidateDlgData();
}






//------------------------------------------------------------------*
/**
 * ClearLibraries clears the library list.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::ClearLibraries()
{
    POSITION pos;
    pos = m_listLibraries.GetHeadPosition();
    while(pos)
    {
        CLibraryContent* pLibCont = NULL;
        pLibCont = m_listLibraries.GetNext(pos);
        POSITION posInner = (pLibCont->pouList).GetHeadPosition();
        while(posInner)
        {
            CPouVarObject *pObj= NULL;
            pObj = (pLibCont->pouList).GetNext(posInner);
            ASSERT(pObj);
            delete pObj;
        }
        (pLibCont->pouList).RemoveAll();
        delete pLibCont;
    }
    m_listLibraries.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * RemoveLibraryInfo removes one library with all information 
 * from the list of libraries.
 *
 * @param           strPath: the full path adn file name for the library.
 * @return          The name of the library.
 * @exception       -
 * @see             
*/
CString CPouVarSelData::RemoveLibraryInfo(CString strPath)
{
    CString strLibName = _T("");
    POSITION pos;
    pos = m_listLibraries.GetHeadPosition();
    while(pos!=NULL)
    {
        POSITION posFound = pos;
        CLibraryContent* pLibCont = NULL;
        pLibCont = m_listLibraries.GetNext(pos);
        ASSERT(pLibCont);
        if ((pLibCont->strLibPath).CompareNoCase(strPath)==0)
        {
            strLibName = pLibCont->strLibName;
            POSITION posInner = (pLibCont->pouList).GetHeadPosition();
            while(posInner)
            {
                CPouVarObject *pObj= NULL;
                pObj = (pLibCont->pouList).GetNext(posInner);
                ASSERT(pObj);
                delete pObj;
            }
            (pLibCont->pouList).RemoveAll();

            delete pLibCont;
            m_listLibraries.RemoveAt(posFound);

            break;
        }
    }

    return strLibName;
}


//------------------------------------------------------------------*
/**
 * ClearProjectPous.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::ClearProjectPous()
{
    POSITION pos;
    pos = m_listProjectPous.GetHeadPosition();
    while(pos!=NULL)
    {
        CPouVarObject *pObj= NULL;
        pObj = m_listProjectPous.GetNext(pos);
        ASSERT(pObj);
        delete pObj;
    }
    m_listProjectPous.RemoveAll();   
}


//------------------------------------------------------------------*
/**
 * ClearProjectConstants.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::ClearProjectConstants()
{
    POSITION pos;
    pos = m_listProjectConstants.GetHeadPosition();
    while(pos!=NULL)
    {
        CPouVarObject *pObj= NULL;
        pObj = m_listProjectConstants.GetNext(pos);
        ASSERT(pObj);
        delete pObj;
    }
    m_listProjectConstants.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * ClearGlobalInstances.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::ClearGlobalInstances()
{
    POSITION pos;
    pos = m_listGlobalInstances.GetHeadPosition();
    while(pos!=NULL)
    {
        CPouVarObject *pObj= NULL;
        pObj = m_listGlobalInstances.GetNext(pos);
        ASSERT(pObj);
        delete pObj;
    }
    m_listGlobalInstances.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * ClearLocalInstances.
 *
 * @exception       -
 * @see             
*/
void CPouVarSelData::ClearLocalInstances()
{
    POSITION pos;
    pos = m_listLocalInstances.GetHeadPosition();
    while(pos!=NULL)
    {
        CVarObject *pObj= NULL;
        pObj = m_listLocalInstances.GetNext(pos);
        ASSERT(pObj);
        delete pObj;
    }
    m_listLocalInstances.RemoveAll();
}






//------------------------------------------------------------------*
/**
 *  connection point to project manager is activated
 *
*/
HRESULT CPouVarSelData::AdviseProjEventHandler()
{
    HRESULT hr;

    ASSERT(m_pProjManUnk!=NULL);
    ASSERT (m_dwProjEventHandlerCookie == NULL);

  
    ASSERT(m_pProjEventHandler == NULL);
    hr = CComObject<CProjManEventsImpl>::CreateInstance (&m_pProjEventHandler);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_pProjEventHandler!= NULL);
    if (m_pProjEventHandler == NULL)
    {
        return (E_UNEXPECTED);
    }
    m_pProjEventHandler->AddRef ();

    hr = AtlAdvise (m_pProjManUnk, m_pProjEventHandler->GetUnknown(), 
                    IID__ICEProjectManagerEvents, &m_dwProjEventHandlerCookie);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        m_pProjEventHandler->Release ();
        m_pProjEventHandler = NULL;
        return (E_UNEXPECTED);
    }

    m_pProjEventHandler->SetPouVarSelDataPtr(this);

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 *  connection point to project manager is deactivated
 *
*/
void CPouVarSelData::UnadviseProjEventHandler()
{
    HRESULT hr;

    ASSERT(m_pProjManUnk!=NULL);
  
    ASSERT (m_dwProjEventHandlerCookie != NULL);
    if (m_dwProjEventHandlerCookie == NULL)
    {
        return;
    }

    hr = AtlUnadvise (m_pProjManUnk, 
                      IID__ICEProjectManagerEvents, 
                      m_dwProjEventHandlerCookie);

    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        ;
    }

    if (m_pProjEventHandler != NULL)
    {
        m_pProjEventHandler->SetPouVarSelDataPtr(NULL);
        m_pProjEventHandler->Release();
        m_pProjEventHandler = NULL;
    }
}


CString CPouVarSelData::GetHeader(CPouVarObject* pPouVarObj, int& iHeaderLen)
{
    HRESULT hr;
    iHeaderLen = 0;

    CString strOrigin = pPouVarObj->m_strOrigin;

    if (strOrigin.IsEmpty())
    {
        CString str;
        str.LoadString(IDS_LOCAL_HEADER);
        return str;
    }

    if (m_pLibOpen==NULL)
    {
        return _T("");
    }
    
    BOOL bWrite = FALSE;
    CComBSTR sFileName(strOrigin);
    CComBSTR sError;
    I4CLAccess* pIAccess = NULL;
    hr = m_pLibOpen->Open(bWrite, sFileName, &sError, &pIAccess);
    if (hr!=S_OK || pIAccess==NULL)
    {
        return _T("");
    }

    // only for pou object header file available
    CPouObject* pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
    if (pPouObj == NULL)
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return _T("");
    }
    CComBSTR sHeaderFileName(pPouObj->m_strHeaderFile);
    CComBSTR sPouName(pPouObj->m_strName);
    CComBSTR sFileCont;
    hr = pIAccess->GetFileAsBSTR(sPouName, NULL, sHeaderFileName, &sFileCont);
    CString strFileCont(sFileCont);

    if (hr!=S_OK || strFileCont.GetLength()<=0)
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return _T("");
    }
    iHeaderLen = pPouObj->m_iHeaderEndPos;

    m_pLibOpen->Close(pIAccess);
    pIAccess->Release();

    return strFileCont;
}



void CPouVarSelData::CheckOutUserHelp(CPouVarObject* pPouVarObj)
{
    HRESULT hr;

    CString strOrigin = pPouVarObj->m_strOrigin;

    if (strOrigin.IsEmpty())
    {
        return;
    }

    if (m_pLibOpen==NULL)
    {
        return;
    }
    
    CString strCheckoutPath;
    int iPoint = strOrigin.ReverseFind(_T('.'));
    if (iPoint<0)
    {
        return;
    }

    strCheckoutPath = strOrigin.Left(iPoint);

    // check if necessary to check out files
    CFileStatus fstatLib;
    CFileStatus fstatDir;

    CString strUHMarkFile = strCheckoutPath + (CString)_T("\\") + PVS_USER_HELP_MARK_FILE;

    CFile::GetStatus(strOrigin, fstatLib);
    if (CFile::GetStatus(strUHMarkFile, fstatDir))
    {
       if (fstatDir.m_mtime==fstatLib.m_mtime)
       {
           return;
       }
    }

    // check out files
    BOOL bWrite = FALSE;
    CComBSTR sFileName(strOrigin);
    CComBSTR sError;
    I4CLAccess* pIAccess = NULL;
    hr = m_pLibOpen->Open(bWrite, sFileName, &sError, &pIAccess);
    if (hr!=S_OK || pIAccess==NULL)
    {
        return;
    }

    CComBSTR sPouName = PVS_LIB_HELP;
    CComBSTR sFileSpec = _T("*");
    CComBSTR sTargetType = _T("");
    CComBSTR sCheckoutDir = strCheckoutPath;
    CComBSTR sErrorDesc;

    hr = pIAccess->CheckOut(sPouName, sTargetType, sFileSpec,
                            sCheckoutDir, NULL, &sErrorDesc);
    if (hr!=S_OK)
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return;
    }
    
    try
    {
        // write mark file and patch modified time
        CFile markFile(strUHMarkFile, CFile::modeCreate|CFile::modeWrite);

        markFile.Close();

        CFile::GetStatus(strUHMarkFile, fstatDir);
        fstatDir.m_mtime = fstatLib.m_mtime;
        CFile::SetStatus(strUHMarkFile, fstatDir);
    }
    catch(CFileException* e)
    {
        e->Delete();
    }

    m_pLibOpen->Close(pIAccess);
    pIAccess->Release();
}


CString CPouVarSelData::CheckOutHeader(CPouVarObject* pPouVarObj)
{
    CPouObject* pPouObj = NULL;
    pPouObj = dynamic_cast<CPouObject*>(pPouVarObj);
    if (!pPouObj)
    {
        return _T("");
    }

    CString strHeader = GetHeader(pPouVarObj, pPouObj->m_iHeaderEndPos);

    return WriteHeaderFile(pPouObj->m_strOrigin, pPouObj->m_strName, strHeader);
}

//------------------------------------------------------------------*
/**
 * write header file.
 *
 * @param           crstrLibFilePath: library file path
 * @param           crstrPouName: name of pou
 * @param           crstrFileContents: contents of header file as string
 * @return          header file name incl full path.
 * @exception       -
 * @see             -
*/
CString CPouVarSelData::WriteHeaderFile(const CString& crstrLibFilePath, const CString& crstrPouName, 
                                          const CString& crstrFileContents)
{
    CString strFolderName;
    CString strHeaderFilePath;
    CString strFullPath = crstrLibFilePath;

    if(crstrFileContents.IsEmpty())
    {
        return _T("");
    }

    if (crstrLibFilePath.IsEmpty())
    {
        return _T("");
    }

    int i = strFullPath.ReverseFind(_T('.'));
    if (i>0)
    {
        strFullPath = strFullPath.Left(i);
    }

    strFolderName = strFullPath + _T("\\") + (CString)PVS_HEADER_DIR;
    strHeaderFilePath = strFolderName + (CString)_T("\\") 
        + crstrPouName + (CString)PVS_HEADER_EXT;
    

    CFileStatus libStatus;
    CFile::GetStatus(crstrLibFilePath, libStatus);

    CFileStatus hdrStatus;
    BOOL bStatus = CFile::GetStatus(strHeaderFilePath, hdrStatus);

    // create new hdr file if necessary
    if (!bStatus || libStatus.m_mtime != hdrStatus.m_mtime)
    {
        int iDirCreated = FC_CreateSubDir(strFolderName);
        //create it new
        TRY
        {
            CFile fHeaderFile(strHeaderFilePath, CFile::modeReadWrite | CFile::modeCreate);
            fHeaderFile.Write(crstrFileContents, crstrFileContents.GetLength());
            fHeaderFile.Close();
        }
        CATCH_ALL(e)
        {
            return _T("");
        }
        END_CATCH_ALL
        //Set the moddate to the moddate of the library
        CFileStatus rStatusHeader;
        CFile::GetStatus(strHeaderFilePath, rStatusHeader);
        rStatusHeader.m_mtime = libStatus.m_mtime;
        CFile::SetStatus(strHeaderFilePath, rStatusHeader);  
    }

    return strHeaderFilePath;
}






CVarObject* CPouVarSelData::FindLocalInstance(const CString& strInstName)
{
    POSITION pos;
    pos = m_listLocalInstances.GetHeadPosition();

    while(pos)
    {
        CVarObject* pVarObj = NULL;
        pVarObj = m_listLocalInstances.GetNext(pos);
        ASSERT(pVarObj);
        if (!pVarObj)
        {
            return NULL;
        }
        if (pVarObj->m_strName.CompareNoCase(strInstName)==0)
        {
            return pVarObj;
        }
    }
    return NULL;
}


CVarObject* CPouVarSelData::FindGlobalInstance(const CString& strInstName)
{
    POSITION pos;
    pos = m_listGlobalInstances.GetHeadPosition();

    while(pos)
    {
        CVarObject* pVarObj = NULL;
        pVarObj = m_listGlobalInstances.GetNext(pos);
        ASSERT(pVarObj);
        if (!pVarObj)
        {
            return NULL;
        }
        if (pVarObj->m_strName.CompareNoCase(strInstName)==0)
        {
            return pVarObj;
        }
    }
    return NULL;
}

BOOL CPouVarSelData::IsPredefinedVar(const CString& strIdentifier, CString& strTypeHint)
{
    if(!m_pIAutoDecl)
        return FALSE;

    CComBSTR sIECAddr;
    CComBSTR sIECType;
    BOOL     bReadOnly;
    CComBSTR sName = strIdentifier;
    HRESULT hr = m_pIAutoDecl->getAddressInfo(       
		            sName,      //IEC identifier to test 
		            &sIECAddr,  //Target specific direct address to be used.
		            &sIECType,  //The IEC data type of the predefined variable
		            &bReadOnly  //true if not writable.
		         );

    if(hr==S_OK && sIECType!=NULL && sIECAddr!=NULL && sIECAddr[0]!=0)
    {
        strTypeHint = sIECType;
        return TRUE;
    }
    else
    {
        strTypeHint = _T("");
        return FALSE;
    }
}

CVarObject* CPouVarSelData::FindGlobVarByAddress(const CString& strAddress)
{
    POSITION pos;
    pos = m_listGlobalInstances.GetHeadPosition();
    while(pos)
    {
        CVarObject* pVarObj = NULL;
        pVarObj = m_listGlobalInstances.GetNext(pos);
        ASSERT(pVarObj);
        if (pVarObj && pVarObj->m_strAddress.CompareNoCase(strAddress)==0)
            return pVarObj;
    }
    return NULL;
}


CPouVarObject* CPouVarSelData::FindPou(const CString& strPouName)
{
    POSITION pos;
    // first in project pous
    pos = m_listProjectPous.GetHeadPosition();

    while(pos)
    {
        CPouVarObject* pPouVar = NULL;
        pPouVar = m_listProjectPous.GetNext(pos);
        ASSERT(pPouVar);
        if (!pPouVar)
        {
            return NULL;
        }
        if (pPouVar->m_strName.CompareNoCase(strPouName)==0)
        {
            return pPouVar;
        }
    }

    // then in libs
    POSITION posLib;
    posLib = m_listLibraries.GetHeadPosition();

    while (posLib)
    {
        CLibraryContent* pLibCont = NULL;
        pLibCont = m_listLibraries.GetNext(posLib);
        ASSERT(pLibCont);
        if (!pLibCont)
        {
            return NULL;
        }
        pos = pLibCont->pouList.GetHeadPosition();
        while(pos)
        {
            CPouVarObject* pPouVar = NULL;
            pPouVar = pLibCont->pouList.GetNext(pos);
            ASSERT(pPouVar);
            if (!pPouVar)
            {
                return NULL;
            }
            if (pPouVar->m_strName.CompareNoCase(strPouName)==0)
            {
                return pPouVar;
            }
        }
    }
    return NULL;
}




CString CPouVarSelData::FindProgramFile(const CString& strProgramType)
{
    CString strFileName;
    
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;

    pProjInfo = m_pProjManUnk;
    ASSERT(!(!pProjInfo));
    if (!pProjInfo)
    {
        return _T("");
    }

    SAFEARRAY* parrClasses;
    CComBSTR sType = _T("PROGRAM");
    HRESULT hr = pProjInfo->getClassesOfTypeAsIdPaths(sType, &parrClasses);
    if (hr!=S_OK)
    {
        return _T("");
    }

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return _T("");
    }

    CString strIdPath;
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        CString str;
        bstr = pbstr[i];
        str = bstr;

        CString strIdPart;
        int partPos;
        partPos = str.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
        if (partPos!=-1)
        {
            strIdPart = str.Mid(partPos+1);
            if (strIdPart.CompareNoCase(strProgramType)==0)
            {
                strIdPath = str;
                break;
            }
        }

    }

    ::SafeArrayUnaccessData(parrClasses);		
    ::SafeArrayDestroy(parrClasses);


    if (strIdPath.IsEmpty())
    {
        return _T("");
    }


    CComBSTR sIdPath = strIdPath;
    CComBSTR sFileName;
    hr = pProjInfo->getFileNameFromIdPath(sIdPath, &sFileName);
    if (hr!=S_OK)
    {
        return _T("");
    }

    strFileName = sFileName;

    return strFileName;
}



CString CPouVarSelData::GetActivePouFileName()
{
    return m_strActivePouFileName;
}

CString CPouVarSelData::GetActivePouName()
{
    return m_strActivePouName;
}

CString CPouVarSelData::GetActivePouType()
{
    return m_strActivePouType;
}

BOOL CPouVarSelData::ActivePouSupportVarDecl()
{
    return m_bActivePouSupportVarDecl;
}

GUID CPouVarSelData::GetActiveVarHandler()
{
    return m_guidActiveVarHandler;
}

BOOL CPouVarSelData::GetActivePouSupportRetain()
{
    return m_bRetain;
}

BOOL CPouVarSelData::GetActivePouSupportInput()
{
    return m_bInput;
}

BOOL CPouVarSelData::GetActivePouSupportOutput()
{
    return m_bOutput;
}

BOOL CPouVarSelData::GetActivePouSupportExternal()
{
    
    return !m_strActivePouName.IsEmpty() && !IsActivePouGVL();
}

BOOL CPouVarSelData::IsActivePouGVL()
{
    
    return !_tcsicmp(FC_StringGetFileExt(m_strActivePouFileName), _T("gvl"));
}

BOOL CPouVarSelData::GetActivePouSupportInOut()
{
    return m_bInOut;
}


IUnknown* CPouVarSelData::GetProjManInterface()
{
    return m_pProjManUnk;
}



CString CPouVarSelData::GetTypeForClassText(const CString& strText)
{   
    POSITION pos = m_listLibClassInfos.GetHeadPosition();

    while(pos)
    {
        CClassInfo* pClassInfo = m_listLibClassInfos.GetNext(pos);
        if (pClassInfo)
        {
            if (pClassInfo->strClassText.Compare(strText)==0)
            {
                return pClassInfo->strClassType;
            }
        }
    }

    return _T("");
}


CString CPouVarSelData::GetTextForClassType(const CString& strType)
{
    POSITION pos = m_listLibClassInfos.GetHeadPosition();

    while(pos)
    {
        CClassInfo* pClassInfo = m_listLibClassInfos.GetNext(pos);
        if (pClassInfo)
        {
            if (pClassInfo->strClassType.Compare(strType)==0)
            {
                return pClassInfo->strClassText;
            }
        }
    }

    return _T("");
}



//------------------------------------------------------------------*
/**
 * check out header.
 *
 * @param           crstrLibraryPath: file name incl path of library file
 * @param           crstrHeaderPath: path of header file within library
 *                  <pouname>
 * @param           rstrHeaderFilePath: file name incl path of extracted 
 *                                      header file
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPouVarSelData::CheckOutHeader(const CString& crstrLibraryPath, const CString& crstrHeaderPath, CString& rstrHeaderFilePath)
{
    rstrHeaderFilePath.Empty();

    CString strPouName;
    CString strTargetName;
    CString strHeaderFileName;

    // parse header path: <file name> or
    //                    <pou name>\<file name> or
    //                    <pou name>\<target name>\<file name> or

    // strip file name
    int iFound = crstrHeaderPath.ReverseFind(_T('\\'));
    if(iFound >= 0)
    {
        CString strRest;
        strHeaderFileName = crstrHeaderPath.Mid(iFound + 1);
        strRest = crstrHeaderPath.Left(iFound);
        // strip pou name
        iFound = strRest.Find(_T('\\'));
        if(iFound >= 0)
        {
            strPouName = strRest.Left(iFound);
            strTargetName = strRest.Mid(iFound + 1);    // rest is target name
        }
        else
        {
            strPouName = strRest;
        }
    }
    else
    {
        strHeaderFileName = crstrHeaderPath;
    }

    // open library and get header
    HRESULT hr;
    BOOL bWrite = FALSE;
    CComBSTR sFileName(crstrLibraryPath);
    CComBSTR sError;
    I4CLAccess* pIAccess = NULL;
    hr = m_pLibOpen->Open(bWrite, sFileName, &sError, &pIAccess);
    if (hr!=S_OK || pIAccess==NULL)
    {
        return FALSE;
    }

    CComBSTR sPouName(strPouName);
    CComBSTR sTargetName(strTargetName);

    //HACK: try always to open any .hdr file if possible. The problem is that 
    //if you click on an error like 
    //C:\4c\Engineering\lib\uselibxx.4cl::Function2\4cwin\Function2.st(3,10) : error C1331: function 'Function1' is called in precompiled body, but no definition found
    //the file Function2.st gets extracted from the lib and shown. But that should be 
    //knowhow protected...

    //first try with extension hdr:
    CComBSTR sFileCont;
    CComBSTR sHeaderFileName;

    CString oldExt = FC_StringGetFileExt(strHeaderFileName);

    iFound = strHeaderFileName.ReverseFind(_T('.'));
    ASSERT(iFound >= 0);
    strHeaderFileName = strHeaderFileName.Left(iFound+1);
    strHeaderFileName += "hdr";
    
    sHeaderFileName = strHeaderFileName;
    hr = pIAccess->GetFileAsBSTR(sPouName, sTargetName, sHeaderFileName, &sFileCont);

    if(hr!=S_OK)    
    {   //no hdr try open with original extension:
        iFound = strHeaderFileName.ReverseFind(_T('.'));
        ASSERT(iFound >= 0);
        strHeaderFileName = strHeaderFileName.Left(iFound+1);
        strHeaderFileName += oldExt;

        sHeaderFileName = strHeaderFileName;
        hr = pIAccess->GetFileAsBSTR(sPouName, sTargetName, sHeaderFileName, &sFileCont);
    }
    
    CString strFileCont(sFileCont);

    if (hr!=S_OK || strFileCont.GetLength()<=0)
    {
        m_pLibOpen->Close(pIAccess);
        pIAccess->Release();
        return FALSE;
    }

    m_pLibOpen->Close(pIAccess);
    pIAccess->Release();

    rstrHeaderFilePath = WriteHeaderFile(crstrLibraryPath, strPouName, strFileCont);
    if(rstrHeaderFilePath.IsEmpty())
    {
        return FALSE;
    }

    return TRUE;
}





