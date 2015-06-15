

//----  Includes:   -------------------------------------------*
#include "stdafx.h"

#include "CEDocuManager.h"
#include "CEMAIN.h"
#include "CESysDef.h"
#include "cecompman.h"
#include "KADHelper\KADFileExtInfo.h"
#include "CEProjInfo.h"
#include "GenEdView.h"
#include "GenEdDoc.h"
#include "XMLHelper\XMLNodeList.h"
#include "DrawObject.h"
#include "Navigator.h"
#include "CEFileHandler.h"
#include "PrintOptionsDlg.h"

// OSAI feature print XRef 24.03.04 SIS >>
#include "MainFrm.h"
#include "XRefWindow.h"
// OSAI feature print XRef 24.03.04 SIS <<

//----  Static Initializations:   ----------------------------------*
const float con_f_mm_per_inch = 25.4F;
const float con_f_points_per_inch = 72.0F;


//----  Local Defines:   -------------------------------------------*
#define CE_INDEX_FILE   _T("_index.txt")


CCEDocuManager::CCEDocuManager()
{
    m_pFileInfo = NULL;
    m_bPrintProject = FALSE;
}

CCEDocuManager::~CCEDocuManager()
{
}

//------------------------------------------------------------------*
/**
 * init.
 *
 *  set file info pointer
 *
 * @param           pFileInfo: KAD file info pointer
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::Init(CKADFileInfo* pFileInfo)
{
    m_pFileInfo = pFileInfo;
}

//------------------------------------------------------------------*
/**
 * print project.
 *
 * @param           -
 * @return          S_OK: ok
 *                  S_FALSE: cancel by user
 *                  E_FAIL: error occurred.
 * @exception       -
 * @see             PrintProject(2)
*/
HRESULT CCEDocuManager::PrintProject()
{
    // get project name from proj info
    HRESULT                 hr;
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjInfo;
    CComBSTR                sProjName;
    CComBSTR                sProjPath;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(!pProjInfo)
    {
        ASSERT(!"get project info IF failed");
        return E_FAIL;
    }
    hr = pProjInfo->getProjectName(&sProjName);
    hr = pProjInfo->getProjectPath(&sProjPath);

    CString strProjectTitle(sProjName);
    CString strProjectPath(sProjPath);

    SaveAllFiles();
    return PrintProject(strProjectTitle, strProjectPath);
}


//------------------------------------------------------------------*
/**
 * print single document.
 *
 *  
 *
 * @param           crstrFilePathName: file name incl path
 * @return          S_OK: ok
 *                  S_FALSE: cancel by user
 *                  E_FAIL: error occurred.
 * @exception       -
 * @see             -
*/
HRESULT CCEDocuManager::PrintSingleDocument(const CString& crstrFilePathName)
{
	CPrintInfo printInfo;
	CPrjPrintDialog dlgPrintStatus(AfxGetMainWnd());
    CDC dcPrint;
    CGenEdCont* pContainer = GetContainer();
    if(!pContainer)
    {
        return E_FAIL;
    }

    if(crstrFilePathName.IsEmpty())
    {
        return E_FAIL;
    }

	CFile File;
	File.SetFilePath(crstrFilePathName);

    m_bPrintProject = FALSE;

    SaveAllFiles();

    if(PreparePrinting(&printInfo, &dlgPrintStatus, &dcPrint, File.GetFileName()) != S_OK)
    {
        return S_FALSE;
    }

    // now set print info in container
    CString strPrinterName;
    long    lWidth;
    long    lHeight;
    if(!GetPrintInfo(strPrinterName, &lWidth, &lHeight))
    {
        return E_FAIL;
    }
    pContainer->SetPrintInfo(strPrinterName, lWidth, lHeight);

    BOOL bError = FALSE;
    CString        strHelp;
    CPrintUserData UserData;
    int iPathLen;

    printInfo.m_lpUserData = (LPVOID)&UserData;
    UserData.SetProjectName(_T(""));

    strHelp = crstrFilePathName;
    iPathLen = strHelp.ReverseFind('\\');
    UserData.SetObjectName(strHelp.Mid(iPathLen+1));

    // SIS, 04.03.02: KUK special feature 
    // ensure that print options for 4gr files are set correctly
    if(crstrFilePathName.Right(4).CompareNoCase(_T(".4gr")) == 0)
    {
        CPrintSettings* pPrintSettings = GetPrintSettings();
        if(pPrintSettings)
        {
            if(pPrintSettings->m_uiPrintFlags & ePF_grheader)
            {
                SetGREditorHeaderFlag(TRUE);
            }
            else
            {
                SetGREditorHeaderFlag(FALSE);
            }
        }
    }

    if (S_OK != PrintProjectFile(crstrFilePathName, &dcPrint, &printInfo, 
                                 &dlgPrintStatus))
    {
        bError = TRUE;
    }

	if (!bError)
		dcPrint.EndDoc();
	else
		dcPrint.AbortDoc();

	AfxGetMainWnd()->EnableWindow();    // enable main window
	dlgPrintStatus.DestroyWindow();
	dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor

    if(bError)
    {
        return E_FAIL;
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * print active document.
 *
 * @param           -
 * @return          S_OK: ok
 *                  S_FALSE: cancel by user
 *                  E_FAIL: error occurred.
 * @exception       -
 * @see             PrintDocument(1)
*/
HRESULT CCEDocuManager::PrintActiveDocument()
{
    CView* pView = ((CFrameWnd*) AfxGetMainWnd())->GetActiveFrame()->GetActiveView();
    if(NULL == pView) 
    {
        return E_FAIL;
    }

    if(!pView->IsKindOf(RUNTIME_CLASS(CGenEdView))) 
    {
        return E_FAIL;
    }

    CGenEdDoc *pGenEdDoc = ((CGenEdView*)pView)->GetDocument();
    if(NULL == pGenEdDoc)
    {
        return E_FAIL;
    }
    CString strDocFile;
	strDocFile = pGenEdDoc->GetDocumentFile();
	if(strDocFile.IsEmpty()) 
    {
        return E_FAIL;
    }
    return PrintSingleDocument(strDocFile);
}


// OSAI feature print XRef 24.03.04 SIS >>
HRESULT CCEDocuManager::PrintXRef(int iMode /*= -1*/)	// XREFPRINT 21.07.05 SIS
{
    HRESULT         hr = E_FAIL;
    CMainFrame*     pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CXRefWindow*	pXRef = pMainFrame->GetXRef();
	ASSERT(pXRef);
    if(!pXRef)
    {
        return hr;
    }

	CString	strFilePathName;

    if(pXRef->PrintFile(strFilePathName, 80, iMode))	// XREFPRINT 21.07.05 SIS
    {
        hr = PrintSingleDocument(strFilePathName);
    }
    return hr;
}
// OSAI feature print XRef 24.03.04 SIS >>


//------------------------------------------------------------------*
/**
 * get page count
 *
 * @param           pItem: editor server item
 * @param           crstrFilePathName: file name
 * @param           rlPageCount: number of pages
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 *                  other hr from PRLoadDocument or PRGetPageCount
 * @exception       -
 * @see             -
*/
HRESULT CCEDocuManager::GetPageCount(CGenEdDoc* pDoc, const CString& crstrFilePathName, long& rlPageCount)
{
    HRESULT hr;
    CString strFileName = crstrFilePathName;

    if(!pDoc)
    {
        return E_FAIL;
    }
    CGenEdCntrItem* pItem = pDoc->GetEditorServer();

    CComPtr<ICEPrint> pPrint;
    pPrint.Attach(pItem->GetICEPrint());
    if (!pPrint)
    {
        return E_FAIL;
    }

    CComBSTR    sFileName(crstrFilePathName);
    hr = pPrint->PRLoadDocument(sFileName);
    if (S_OK != hr)
    {
        return hr;
    }

    hr = pPrint->PRGetPageCount(&rlPageCount);
    if (S_OK != hr)
    {
        return hr;
    }

    return S_OK;
}


//------------------------------------------------------------------*
/**
 * print project file.
 *
 * @param           crstrFilePathName: file name incl path
 * @param           pdcPrint: printer device context
 * @param           pPrintInfo: print info object
 * @param           pDlgPrintStatus: print status dialog
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 * @exception       -
 * @see             
*/
HRESULT CCEDocuManager::PrintProjectFile
(
    const CString&  crstrFilePathName,
    CDC*            pdcPrint,
    CPrintInfo*     pPrintInfo, 
    CDialog*        pDlgPrintStatus
)
{
    BOOL bError = FALSE;

    CGenEdCont* pContainer = GetContainer();
    CGenEdDoc* pDoc = NULL;
    if(NULL == pContainer) 
    {
        return E_FAIL;
    }
    
    pDoc = pContainer->GetServerForFile(crstrFilePathName);
    if(NULL == pDoc) 
    {
        return E_FAIL;
    } 
    else 
    {
        bError |= (S_OK != PrintDocument(pDoc, crstrFilePathName, pdcPrint, pPrintInfo, pDlgPrintStatus));
    }

    return (bError ? E_FAIL : S_OK);
}



//------------------------------------------------------------------*
/**
 * print document.
 *
 * @param           pDoc: editor server item
 * @param           crstrFilePathName: file name incl path
 * @param           pDCPrint: printer device context
 * @param           pPrintInfo: print info object
 * @param           pDlgPrintStatus: print status dialog
 * @return          S_OK: ok
 *                  S_FALSE: aborted
 *                  E_FAIL. error occurred
 *                  hr from PRLoadDocument, PRSetPrintInfo, PRGetPages
 * @exception       -
 * @see             -
*/
HRESULT CCEDocuManager::PrintDocument
(
    CGenEdDoc*      pDoc,
    const CString&  crstrFilePathName,
    CDC*            pDCPrint,
    CPrintInfo*     pPrintInfo, 
    CDialog*        pDlgPrintStatus
)
{
    CWaitCursor tWaitCursor;
    HRESULT hr = S_OK;
    SAFEARRAY * pPages = NULL;

    CPrintUserData* pPrintUserData = (CPrintUserData*)(pPrintInfo->m_lpUserData);
    ASSERT(pPrintUserData);

    CGenEdCntrItem* pItem = pDoc->GetEditorServer();
    if(!pItem)
    {
        return E_FAIL;
    }

    CComPtr<ICEPrint> pPrint;
    pPrint.Attach(pItem->GetICEPrint());
    if (!pPrint)
    {
        return E_FAIL;
    }

    CComBSTR    sFileName(crstrFilePathName);
    hr = pPrint->PRLoadDocument(sFileName);
    if (S_OK != hr)
    {
        //FC_DEBUGPRINT(_T("CEMAIN> LoadDocument failed\n"));
        return hr;
    }


	// set up drawing rect to entire page (in logical coordinates)
    SetPrintingArea(pDCPrint, pPrintInfo);
    CSize Size(pPrintInfo->m_rectDraw.Width(), pPrintInfo->m_rectDraw.Height());
    pDCPrint->LPtoHIMETRIC(&Size);
    
    hr = pPrint->PRSetPrintInfo(CComBSTR(pPrintInfo->m_pPD->GetDeviceName()), Size.cx, Size.cy);
    if (S_OK != hr)
    {
       // FC_DEBUGPRINT(_T("CEMAIN> SetPrintInfo failed\n"));
        return hr;
    }

    hr = pPrint->PRGetPages(&pPages);
    if (S_OK != hr)
    {
        //FC_DEBUGPRINT(_T("CEMAIN> GetPages failed\n"));
        return hr;
    }

    long lUBound;
    long li;
    UINT uiDim = ::SafeArrayGetDim(pPages);
    assert(1 == uiDim);


    ::SafeArrayGetUBound(pPages, 1, &lUBound);


    if(!m_bPrintProject)
    {
        pPrintUserData->SetPageCount(lUBound+1);
    }

    CString strTemp;
    VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));
    UINT uiPageNum;

    for (li=0; li<=lUBound; li++)
    {
        BSTR sFileName;
        CString strFileName;
		CString strBuf;

        uiPageNum = pPrintUserData->IncrementPageNum();

        ::SafeArrayGetElement(pPages, &li, &sFileName);
        strFileName = sFileName;
        SysFreeString(sFileName);

        if (pPrintInfo->GetMinPage() <= uiPageNum && 
            pPrintInfo->GetMaxPage() >= uiPageNum)
        {
		    strBuf.Format(strTemp, uiPageNum);
		    pDlgPrintStatus->SetDlgItemText(AFX_IDC_PRINT_PAGENUM, strBuf);

		    // attempt to start the current page
		    if (pDCPrint->StartPage() < 0)
		    {
                hr = E_FAIL;
                //FC_DEBUGPRINT(_T("CEMAIN> StartPage failed\n"));
                break;
		    }

            pPrintInfo->m_rectDraw = GetPrintArea(pDCPrint, ePA_client);
            PrintStdHeader(pDCPrint, pPrintInfo);
            PrintStdFooter(pDCPrint, pPrintInfo);

            HENHMETAFILE hemf = GetEnhMetaFile(strFileName);
            if (NULL != hemf)
            {
                if (!PlayEnhMetaFile(pDCPrint->m_hDC, hemf, &pPrintInfo->m_rectDraw))
                {
                    DWORD dw = GetLastError(); 
                    //FC_DEBUGPRINT(_T("CEMAIN> Playing metafile failed\n"));
                    // no reaction on errors at the moment
    //                hr = E_FAIL;
    //                break;
                }
                DeleteEnhMetaFile(hemf);
            }

		    if (pDCPrint->EndPage() < 0)
		    {
                hr = E_FAIL;
                //FC_DEBUGPRINT(_T("CEMAIN> EndPage failed\n"));
                break;
		    }
        }

        // delete the metafile
        DeleteFile(strFileName);

		if (!PrjPrintAbortProc(pDCPrint->m_hDC, 0))
        {
            hr = S_FALSE;
            break;
        }
    }

    if (pPages)
    {
        ::SafeArrayDestroy(pPages);  
        pPages = NULL;
    }

    return hr;
}

// SIS, 15.10.01, adapted from V1.3
//------------------------------------------------------------------*
/**
 * print project.
 *
 * @param           crstrProjName: project name
 * @param           crstrProjPath: project path
 * @return          S_OK: ok
 *                  S_FALSE: cancel by user
 *                  E_FAIL: error occurred.
 * @exception       -
 * @see             -
*/
HRESULT CCEDocuManager::PrintProject(const CString& crstrProjName, const CString& crstrProjPath)
{
    CString         strProjectFile(crstrProjName + _T(".4cp"));
    HRESULT         hr;
	CPrintInfo      printInfo;
	CPrjPrintDialog dlgPrintStatus(AfxGetMainWnd());
    CDC             dcPrint;
    CPrintUserData  UserData;
    CString         strFileName;
    CString         strFilePath;
    long            lPageCount = 0;
    long            lIndexPageCount = 0;
    CString         strIndexFilePathName(crstrProjPath + _T('\\') + CE_INDEX_FILE);
    TPPFileCollList tFileCollList;

    m_bPrintProject = TRUE;

    CPrintSettings* pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(!pPrintSettings)
    {
        return E_FAIL;
    }

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return E_FAIL;
    }


    // generate tree view files and insert them
    InsertTreeViewFiles(crstrProjPath, pPrintSettings, tFileCollList);
	
    // OSAI feature print XRef 24.03.04 SIS >>
	// generate cross reference file and insert it
	InsertCrossRefFile(crstrProjPath, pPrintSettings, tFileCollList);
    // OSAI feature print XRef 24.03.04 SIS <<

    // select the files to be printed
    if(!SelectPrintProjectFiles(strProjectFile, tFileCollList))
    {
        return S_FALSE;
    }

    CGenEdDoc* pDoc = NULL;
    CGenEdCont* pContainer = GetContainer();
    if(pContainer == NULL) 
    {
        return E_FAIL;
    }

    printInfo.m_lpUserData = (LPVOID)&UserData;
    UserData.SetProjectName(crstrProjName);
    UserData.SetProjectPath(crstrProjPath);

    if(PreparePrinting(&printInfo, &dlgPrintStatus, &dcPrint, crstrProjName) != S_OK)
    {
        FreeFileCollections(tFileCollList);
        return S_FALSE;
    }

    // now set print info in container
    CString strPrinterName;
    long    lWidth;
    long    lHeight;
    if(!GetPrintInfo(strPrinterName, &lWidth, &lHeight))
    {
        return E_FAIL;
    }
    pContainer->SetPrintInfo(strPrinterName, lWidth, lHeight);


//    int*    pInt = new int[1];
//
//    int iMapMode = dcPrint.GetMapMode();
//
//    int iHorzSize = dcPrint.GetDeviceCaps(HORZRES);    // horizontal size in pixels
//    int iCharacterPerLine = 80;
//
//    if(dcPrint.GetOutputCharWidth(_T('a'), _T('a'), pInt))  // width in twips
//    {
//        TRACE("output char width = %d\n", *pInt);
//        iCharacterPerLine = iHorzSize / *pInt;
//    }

    // SIS, 04.03.02: KUK special feature 
    // ensure that print options for 4gr files are set correctly
    if(pPrintSettings->m_uiPrintFlags & ePF_grheader)
    {
        SetGREditorHeaderFlag(TRUE);
    }
    else
    {
        SetGREditorHeaderFlag(FALSE);
    }

    // count pages of all files in collection list
    lPageCount = CountPages(crstrProjPath, tFileCollList);

    if(pPrintSettings->m_uiPrintFlags & ePF_contents)
    {
        // create index file temporary to get page count
        if(CreateIndexFile(strIndexFilePathName, crstrProjPath, tFileCollList, 1))
        {
            // now get page count of index file
            pDoc = pContainer->GetServerForFile(strIndexFilePathName);
            ASSERT(pDoc);
            if(pDoc)
            {
                hr = GetPageCount(pDoc, strIndexFilePathName, lIndexPageCount);
                if(hr != S_OK)
                {
                    AfxMessageBox(IDS_PAGE_NUM_INCORRECT);
                    lIndexPageCount = 1;
                }
                lPageCount += lIndexPageCount;
            }
            // create index file
            CreateIndexFile(strIndexFilePathName, crstrProjPath, tFileCollList, lIndexPageCount);
        }
        else
        {
            FreeFileCollections(tFileCollList);
            return E_FAIL;
        }
    }

    UserData.SetPageCount(lPageCount);

    if(pPrintSettings->m_uiPrintFlags & ePF_coverpage)
    {
        PrintCoverPage(&dcPrint, &printInfo);
    }

    if(pPrintSettings->m_uiPrintFlags & ePF_contents)
    {
        CPrintUserData* pPrintUserData = (CPrintUserData*)printInfo.m_lpUserData;
        ASSERT(pPrintUserData);
        pPrintUserData->SetObjectName(_T("Index"));
        PrintProjectFile(strIndexFilePathName, &dcPrint, &printInfo, &dlgPrintStatus);
    }

    if(PrintProjectFiles(&printInfo, &dlgPrintStatus, &dcPrint, pFileHandler, tFileCollList))
    {
		dcPrint.EndDoc();
    }
	else
    {
		dcPrint.AbortDoc();
    }

	AfxGetMainWnd()->EnableWindow();    // enable main window
	dlgPrintStatus.DestroyWindow();
	dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor

#ifndef _DEBUG
    ::DeleteFile(strIndexFilePathName);
#endif

    // free collections
    FreeFileCollections(tFileCollList);

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * print project files.
 *
 *  run through file collection and print selected files.
 *
 * @param           pPrintInfo: print info
 * @param           pPrintDlg: print dialog
 * @param           pDCPrint: printer device context
 * @param           pFileHandler: file handler
 * @param           rtFileCollList: file collection list
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::PrintProjectFiles
(
    CPrintInfo*         pPrintInfo,
    CPrjPrintDialog*    pPrintDlg,
    CDC*                pDCPrint,
    CCEFileHandler*     pFileHandler,
    TPPFileCollList&    rtFileCollList
)
{
    BOOL bError = FALSE;

    CPrintUserData*     pUserData = (CPrintUserData*)pPrintInfo->m_lpUserData;
    CPPFileData*        pFileData = NULL;
    CString             strFileName;
    CString             strObjName;
    CString             strFilePath;
    CPPFileCollection*  pFileColl;
    POSITION pos = rtFileCollList.GetHeadPosition();
    while(pos)
    {
        POSITION    datapos;

        pFileColl = rtFileCollList.GetNext(pos);
        pFileColl->GetFirstSelectedFileDataPosition(datapos);
        while(datapos)
        {
            pFileData = pFileColl->GetFileDataAt(datapos);
            strFileName = pFileHandler->GetFileName(pFileData->m_strFileName);
            strObjName = pFileData->m_strObjName.IsEmpty() ? strFileName : pFileData->m_strObjName;
            strFilePath = pFileHandler->GetFilePathName(pFileData->m_strFileName);
            pUserData->SetObjectName(strObjName);

            if(S_OK != PrintProjectFile(strFilePath, pDCPrint, pPrintInfo, 
                                         pPrintDlg) != S_OK)
            {
                return FALSE;
            }
			if(pFileData->m_bTemporary)
			{
				::DeleteFile(strFilePath);
			}
            pFileColl->GetNextSelectedFileDataPosition(datapos);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * insert tree view files.
 *
 * @param           crstrProjPath: project path
 * @param           pPrintSettings: print settings
 * @param           rtFileCollList: list of file collections
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::InsertTreeViewFiles
(
    const CString& crstrProjPath,
    CPrintSettings* pPrintSettings,
    TPPFileCollList& rtFileCollList
)
{
    // first get navigator
    CNavigator* pNavigator = dynamic_cast<CNavigator*>(CEGetDockingTool(CE_COMPMAN_NAV, false));
    if(!pNavigator)
    {
        return;
    }

    UINT            uiPrintFlags = pPrintSettings->m_uiPrintFlags;
    CStringArray    astrFiles;
    CStringArray    astrObjs;
    CString         strFilePathName;
    int             iNumFiles;
    CString         strObjName;

    if(uiPrintFlags & ePF_fileview)
    {
        if(pNavigator->PrintTreeView(NAV_FILEVIEW, pPrintSettings, strFilePathName))
        {
            astrFiles.Add(strFilePathName);
            strObjName.LoadString(IDS_PP_FILE_VIEW);
            astrObjs.Add(strObjName);
        }
    }
    if(uiPrintFlags & ePF_classview)
    {
        if(pNavigator->PrintTreeView(NAV_CLASSVIEW, pPrintSettings, strFilePathName))
        {
            astrFiles.Add(strFilePathName);
            strObjName.LoadString(IDS_PP_CLASS_VIEW);
            astrObjs.Add(strObjName);
        }
    }
    if(uiPrintFlags & ePF_instview)
    {
        if(pNavigator->PrintTreeView(NAV_INSTVIEW, pPrintSettings, strFilePathName))
        {
            astrFiles.Add(strFilePathName);
            strObjName.LoadString(IDS_PP_INST_VIEW);
            astrObjs.Add(strObjName);
        }
    }
    if(uiPrintFlags & ePF_hwview)
    {
        if(pNavigator->PrintTreeView(NAV_HWVIEW, pPrintSettings, strFilePathName))
        {
            astrFiles.Add(strFilePathName);
            strObjName.LoadString(IDS_PP_HW_VIEW);
            astrObjs.Add(strObjName);
        }
    }

    iNumFiles = astrFiles.GetSize();

    if(iNumFiles > 0)
    {
        CString strName;
        CPPFileCollection*  pFileColl = NULL;

        strName.LoadString(IDS_PP_TREE_VIEWS);
        pFileColl = new CPPFileCollection;
        if(!pFileColl)
        {
            ::AfxThrowMemoryException();
        }
        for(int iFile = 0; iFile < iNumFiles; ++iFile)
        {
            pFileColl->AddFile(astrFiles[iFile], astrObjs[iFile], TRUE, TRUE);
        }
        pFileColl->SetName(strName);
        rtFileCollList.AddTail(pFileColl);
    }
}


// OSAI feature print XRef 24.03.04 SIS >>
//------------------------------------------------------------------*
/**
 * insert cross ref file.
 *
 * @param           crstrProjPath: project path
 * @param           pPrintSettings: print settings
 * @param           rtFileCollList: list of file collections
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::InsertCrossRefFile
(
    const CString& crstrProjPath,
    CPrintSettings* pPrintSettings,
    TPPFileCollList& rtFileCollList
)
{
    UINT            uiPrintFlags = pPrintSettings->m_uiPrintFlags;
    CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
	ASSERT(pMainFrame);
    if(!pMainFrame)
    {
		return;
	}

    CXRefWindow*	pXRef = pMainFrame->GetXRef();
	ASSERT(pXRef);
    if(!pXRef)
    {
        return;
    }

    if(uiPrintFlags & ePF_crossref)
    {
		CString	strFilePathName;

        if(pXRef->PrintFile(strFilePathName, 80))
        {
			CPPFileCollection*  pFileColl = NULL;
			CString	strObjName;
			CString	strCollName;

            strObjName.LoadString(IDS_PP_CROSS_REF);
			strCollName = strObjName;
			pFileColl = new CPPFileCollection;
			if(!pFileColl)
			{
				::AfxThrowMemoryException();
			}
			pFileColl->AddFile(strFilePathName, strObjName, TRUE, TRUE);
			pFileColl->SetName(strObjName);
			rtFileCollList.AddTail(pFileColl);
        }
    }
}
// OSAI feature print XRef 24.03.04 SIS <<

//------------------------------------------------------------------*
/**
 * get print settings.
 *
 * @param           -
 * @return          pointer to print settings.
 * @exception       -
 * @see             -
*/
CPrintSettings* CCEDocuManager::GetPrintSettings()
{
    CSettings*  pSettings = ((CCEMAINApp*)AfxGetApp())->GetSettings();
    if(pSettings)
    {
        return &pSettings->m_printSettings;
    }
    return NULL;
}

//------------------------------------------------------------------*
/**
 * count pages.
 *
 * @param           crstrProjPath: project path
 * @param           rtFileCollList: file collection
 * @return          total number of pages
 * @exception       -
 * @see             -
*/
long CCEDocuManager::CountPages
(   
    const CString& crstrProjPath,
    TPPFileCollList& rtFileCollList
)
{
    HRESULT         hr;
    BOOL            bPageCountWarning = FALSE;
    CWaitCursor     tCursor;
    CString         strFileName;
    CString         strFilePath;

    long            lPageTmp;
    long            lPageCount(0);

    CGenEdDoc* pDoc = NULL;
    CGenEdCont *pContainer = GetContainer();
    if(pContainer == NULL) 
    {
        return 0;
    }

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return 0;
    }

    CPPFileCollection*  pFileColl;
    CPPFileData*        pFileData;
    POSITION pos = rtFileCollList.GetHeadPosition();
    while(pos)
    {
        POSITION    datapos;

        pFileColl = rtFileCollList.GetNext(pos);
        pFileColl->GetFirstSelectedFileDataPosition(datapos);
        while(datapos)
        {
            pFileData = pFileColl->GetFileDataAt(datapos);
            strFilePath = pFileHandler->GetFilePathName(pFileData->m_strFileName);
            strFileName = pFileHandler->GetFileName(strFilePath);

            pDoc = pContainer->GetServerForFile(strFileName);
            hr = GetPageCount(pDoc, strFilePath, lPageTmp);
            if(hr != S_OK)
            {
                lPageTmp = 1;
                bPageCountWarning = TRUE;
            }
            pFileData->m_lPageCount = lPageTmp;
            pFileColl->GetNextSelectedFileDataPosition(datapos);
            lPageCount += lPageTmp;
        }
    }
    if(bPageCountWarning)
    {
        AfxMessageBox(IDS_PAGE_NUM_INCORRECT);
    }
    return lPageCount;
}


//------------------------------------------------------------------*
/**
 * create index file.
 *
 * @param           crstrIndexFilePathName: index file name incl. path
 * @param           crstrProjPath: project path
 * @param           rtFileCollList: file collection list
 * @param           lPageOffset: page offset to be used (page count of index file itself)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::CreateIndexFile
(   
    const CString& crstrIndexFilePathName,
    const CString& crstrProjPath,
    TPPFileCollList& rtFileCollList,
    long lPageOffset
)
{
    CStdioFile      tIndexFile;
    CString         strIndexLine;
    CString         strTmp;
    TCHAR*          pBuffer;
    CWaitCursor     tCursor;
    CString         strCollName;
    CString         strFileName;
    CString         strObjName;
    CString         strFilePath;

    // format line
    int             iLineLength = 80;   // TODO: get line length from CDC
    int             iRightOffset = 5;   // space for numbers
    int             iLeftOffset = 2;    // indent for files
    int             iTmp;

    long            lPageNum(1);

    CGenEdDoc* pDoc = NULL;
    CGenEdCont *pContainer = GetContainer();
    if(pContainer == NULL) 
    {
        return FALSE;
    }

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return FALSE;
    }

    long lPageCount = lPageOffset;

    try
    {
        ::DeleteFile(crstrIndexFilePathName);
        if(tIndexFile.Open(crstrIndexFilePathName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
        {
            CFileStatus tStatus;
            CFile::GetStatus(crstrIndexFilePathName, tStatus);
            tStatus.m_attribute |= CFile::hidden;
            tStatus.m_mtime = NULL;
            CFile::SetStatus(crstrIndexFilePathName, tStatus);
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->Delete();
        TRACE("Cannot open index file\n");
        return FALSE;
    }

    // write empty line
    strTmp = _T("\n");
    tIndexFile.WriteString(strTmp);

    // write "CONTENTS"
    strTmp.LoadString(IDS_PP_CONTENTS);
    strTmp += _T("\n");
    tIndexFile.WriteString(strTmp);

    // underline CONTENTS
    iTmp = strTmp.GetLength();
    pBuffer = strTmp.GetBuffer(iTmp);
    ::memset(pBuffer, _T('-'), iTmp-1);
    strTmp.ReleaseBuffer();
    strTmp += _T("\n");
    tIndexFile.WriteString(strTmp);

    // write empty line
    strTmp = _T("\n");
    tIndexFile.WriteString(strTmp);

    // TODO: this can take a long time: show state dialog
    CPPFileCollection*  pFileColl;
    CPPFileData*        pFileData;
    POSITION pos = rtFileCollList.GetHeadPosition();
    while(pos)
    {
        POSITION    datapos;

        pFileColl = rtFileCollList.GetNext(pos);
        strCollName = pFileColl->GetName() + _T("\n");
        tIndexFile.WriteString(strCollName);
        pFileColl->GetFirstSelectedFileDataPosition(datapos);
        while(datapos)
        {
            pFileData = pFileColl->GetFileDataAt(datapos);
            strFileName = pFileHandler->GetFileName(pFileData->m_strFileName);
            strObjName = pFileData->m_strObjName.IsEmpty() ? strFileName : pFileData->m_strObjName;
            strFilePath = pFileHandler->GetFilePathName(pFileData->m_strFileName);
            lPageNum = lPageCount + 1;
            lPageCount += pFileData->m_lPageCount;

            // fill strIndexLine with dots
            strIndexLine.Empty();
            pBuffer = strIndexLine.GetBuffer(iLineLength);
            ::memset(pBuffer, _T(' '), iLeftOffset);
            ::memset(pBuffer + iLeftOffset, _T('.'), iLineLength - iRightOffset - iLeftOffset);
            iTmp = strObjName.GetLength() + iLeftOffset - 1;
            if(iTmp > iLineLength - iRightOffset - 2)
            {
                iTmp = iLineLength - iRightOffset - 2;
            }
            ::memcpy(pBuffer + iLeftOffset, strObjName.GetBuffer(0), iTmp);
            pBuffer[iTmp+1] = _T(':');
            pBuffer[iTmp+2] = _T(' ');
            pBuffer[iLineLength - iRightOffset] = 0;
            strIndexLine.ReleaseBuffer();
            strTmp.Format(" %5d\n", lPageNum);
            strIndexLine += strTmp;
            tIndexFile.WriteString(strIndexLine);
            pFileColl->GetNextSelectedFileDataPosition(datapos);
        }
        tIndexFile.WriteString(_T("\n"));
    }
    if(lPageCount == 0)
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        return TRUE;
    }

    tIndexFile.Close();
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * select print project files.
 *
 * @param           [in] crstrPrjFile: 4cp file name without path
 * @param           [in/out] rtFileCollList: file collection list
 * @return          
 * @exception       -
 * @see             
*/
BOOL CCEDocuManager::SelectPrintProjectFiles(const CString& crstrPrjFile, TPPFileCollList& rtFileCollList)
{
//    if(!GetPrintableFilesFromProjMan(crstrPrjFile, rastrFiles))
//    {
//        return FALSE;
//    }

    if(!GetPrintableFilesFromTrees(rtFileCollList))
    {
        return FALSE;
    }


    // now show select dialog
//    CProjectPrintSelDlg tDlg;
//    tDlg.SetFiles(rastrFiles);
//    if(tDlg.DoModal() != IDOK)
//    {
//        return FALSE;
//    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * free file collections.
 *
 * @param           [in] rtFileCollList: list of file collections
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::FreeFileCollections(TPPFileCollList& rtFileCollList)
{
    CPPFileCollection*  pFileColl;
    POSITION pos = rtFileCollList.GetHeadPosition();

    while(pos)
    {
        pFileColl = rtFileCollList.GetNext(pos);
        delete pFileColl;
    }
}

//------------------------------------------------------------------*
/**
 * get all files from project manager.
 *
 * @param           [in] eFC: file category (source, help, additional)
 * @param           [in/out] rtFileNames: file names
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::GetAllFilesFromProjMan(enum E_FV_FileCategory eFC, CSBLLexTree<bool>& rtFileNames)
{
    // get project manager
    CComQIPtr<ICEProjectManager>    pProjMan;     // pointer to project manager
    CComPtr<IUnknown>               pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return(FALSE);
    }

    // get all source files
    SAFEARRAY*          parrFiles;
    bool*               pbTmp = (bool*) 1;  // dummy pointer for insert

    HRESULT hr = pProjMan->GetAllFilesOfCategory(eFC ,&parrFiles);
    if(hr != S_OK)
    {
        return FALSE;        
    }   

    BSTR HUGEP *pbstr;
    BSTR bstr;
    CString str;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrFiles, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return FALSE;
    }
    unsigned long iSize = ((parrFiles)->rgsabound[0]).cElements;
    TRACE("    GetAllFilesFromProjMan: %d Files\n", iSize);

    for (i = 0; i < iSize; i++)
    {
        bstr = pbstr[i];
        str = bstr;
        if(str[0] != _T('$'))
        {
            rtFileNames.insertData(str, pbTmp);
        }
    }
    ::SafeArrayUnaccessData(parrFiles);
    ::SafeArrayDestroy(parrFiles);

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * get printable files from project manager.
 *
 * @param           [in] crstrPrjFile: project file name without path
 * @param           [in] rastrFiles: file names of printable files
 *                                   without path
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::GetPrintableFilesFromProjMan(const CString& crstrPrjFile, CStringArray& rastrFiles)
{
    // get kad file info
    CSBLLexTree<bool>   tFileNames;
    if(!GetAllFilesFromProjMan(FV_FILE_SOURCE, tFileNames))
    {
        return FALSE;
    }
    // tFileNames now contains all source files incl 4cp file
    // in alphabetical order
    // remove project file and add it to string array as first entry
    VERIFY(tFileNames.deleteData(crstrPrjFile));
    CStringArray    astrSourceFiles;
    tFileNames.getAllStringsFromSubstring("", astrSourceFiles);

    // first add 4cp file, if printable
    if(IsFilePrintable(crstrPrjFile))
    {
        rastrFiles.Add(crstrPrjFile);
    }
    // run over all source files and add printable ones
    int iNumFiles = astrSourceFiles.GetSize();
    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        if(IsFilePrintable(astrSourceFiles[iFile]))
        {
            rastrFiles.Add(astrSourceFiles[iFile]);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get printable files from trees.
 *
 * @param           [in] rastrFiles: file names of printable files
 *                                   without path
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::GetPrintableFilesFromTrees(TPPFileCollList& rtFileCollList)
{
    // get root node of class and target trees and add printable files
    CXMLDocument    tClassDoc;
    CXMLDocument    tTargetDoc;

    if(!GetXMLDocuments(tClassDoc, tTargetDoc))
    {
        return FALSE;
    }

    CXMLNode    tClassRoot;
    if(tClassDoc.GetRootNode(tClassRoot))
    {
        GetPrintableFilesFromClassTree(tClassRoot, rtFileCollList);
    }

    CXMLNode    tTargetRoot;
    if(tTargetDoc.GetRootNode(tTargetRoot))
    {
        GetPrintableFilesFromTargetTree(tTargetRoot, rtFileCollList);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get XML documents of class and target tree from ProjMan.
 *
 * @param           [out] rtClassDoc: XML document of class tree
 * @param           [out] rtTargetDoc: XML document of target tree
 * @return          successful?
*/
BOOL CCEDocuManager::GetXMLDocuments(CXMLDocument& rtClassDoc, CXMLDocument& rtTargetDoc)
{
    HRESULT hr;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        TRACE("CCEDocuManager::GetXMLDocuments: ProjectManager not available\n");
        return FALSE;
    }
    pTmp.Release();

    hr = pProjMan->GetClassViewDOM(&pTmp);
    if(hr == S_OK)
    {
        rtClassDoc.SetIXMLDocument(pTmp);
    }
    else
    {
        return FALSE;
    }
    pTmp.Release();

    hr = pProjMan->GetTargetViewDOM(&pTmp);
    if(hr == S_OK)
    {
        rtTargetDoc.SetIXMLDocument(pTmp);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get printable files from class tree.
 *
 * @param           [in] rtRootNode: class tree root node
 * @param           [in/out] rtFileCollList: list of file collections
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::GetPrintableFilesFromClassTree(CXMLNode& rtRootNode, TPPFileCollList& rtFileCollList)
{
    // run over children and build up print file collections
    CXMLNodeList        tNodeList;          // list of child nodes
    CXMLNode            tNodeChildTmp;      // temporary child node
    long                lNumChildren;
    long                lChild;
    CPPFileCollection*  pFileColl;
    CStringArray        astrFiles;
    CString             strName;
    CWaitCursor         tWaitCursor;

    if(rtRootNode.GetAttribute(CE_XMLATTR_SOURCE, strName))
    {
        if(IsFilePrintable(strName))
        {
            astrFiles.Add(strName);
        }
    }
    if(astrFiles.GetSize() > 0)
    {
        pFileColl = new CPPFileCollection;
        if(!pFileColl)
        {
            ::AfxThrowMemoryException();
        }
        pFileColl->AddFiles(astrFiles);
        strName.LoadString(IDS_PP_PROJECT);
        pFileColl->SetName(strName);
        rtFileCollList.AddTail(pFileColl);
        astrFiles.RemoveAll();
    }

    if(rtRootNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            tNodeList.Item(lChild, tNodeChildTmp);
            tNodeChildTmp.GetAttribute(CE_XMLATTR_TEXT, strName);   // coll name = text of node
            CollectPrintableSourceFiles(tNodeChildTmp, astrFiles);
            if(astrFiles.GetSize() > 0)
            {
                pFileColl = new CPPFileCollection;
                if(!pFileColl)
                {
                    ::AfxThrowMemoryException();
                }
                pFileColl->AddFiles(astrFiles);
                pFileColl->SetName(strName);
                rtFileCollList.AddTail(pFileColl);
            }
            astrFiles.RemoveAll();
        }
    }
}


//------------------------------------------------------------------*
/**
 * get printable files from target tree.
 *
 * @param           [in] rtRootNode: target tree root node
 * @param           [in/out] rtFileCollList: list of file collections
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::GetPrintableFilesFromTargetTree(CXMLNode& rtRootNode, TPPFileCollList& rtFileCollList)
{
    // run over children and build up print file collections
    CXMLNodeList        tNodeList;          // list of child nodes
    CXMLNode            tNodeChildTmp;      // temporary child node
    long                lNumChildren;
    long                lChild;
    CPPFileCollection*  pFileColl;
    CStringArray        astrFiles;
    CWaitCursor         tWaitCursor;
    CString             strName;
    strName.LoadString(IDS_PP_TARGETS);

    if(rtRootNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            tNodeList.Item(lChild, tNodeChildTmp);
            tNodeChildTmp.GetAttribute(CE_XMLATTR_TEXT, strName);   // coll name = text of node
            CollectPrintableSourceFiles(tNodeChildTmp, astrFiles);
        }
    }
    if(astrFiles.GetSize() > 0)
    {
        pFileColl = new CPPFileCollection;
        if(!pFileColl)
        {
            ::AfxThrowMemoryException();
        }
        pFileColl->AddFiles(astrFiles);
        pFileColl->SetName(strName);
        rtFileCollList.AddTail(pFileColl);
    }
}


//------------------------------------------------------------------*
/**
 * collect printable source files.
 *
 * @param           [in] rtNode: xml node to start
 * @param           [in/out] rastrFiles: string array of file names
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::CollectPrintableSourceFiles(CXMLNode& rtNode, CStringArray& rastrFiles)
{
    CString strSource;
    if(rtNode.GetAttribute(CE_XMLATTR_SOURCE, strSource))
    {
        if(IsFilePrintable(strSource))
        {
            rastrFiles.Add(strSource);
        }
    }

    // run over children
    CXMLNodeList    tNodeList;          // list of child nodes
    CXMLNode        tNodeChildTmp;      // temporary child node
    long            lNumChildren;
    long            lChild;

    if(rtNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChildTmp);
            CollectPrintableSourceFiles(tNodeChildTmp, rastrFiles);
        }
    }
}


//------------------------------------------------------------------*
/**
 * is file printable.
 *
 * @param           [in] crstrFilePath: file path, file name or extension
 * @return          printable?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::IsFilePrintable(const CString& crstrFilePath)
{
    CString strExt;
    int     iFound;
    CKADFileExtInfo*    pExtInfo;

    iFound = crstrFilePath.ReverseFind(_T('.'));
    strExt = crstrFilePath.Mid(iFound + 1);
    if(!m_pFileInfo)
    {
        return FALSE;
    }
    pExtInfo = m_pFileInfo->GetFileExtInfo(strExt);

    if(pExtInfo && pExtInfo->m_bPrint)
    {
        return TRUE;
    }
    return FALSE;
}





//------------------------------------------------------------------*
/**
 * save all files.
 *
 *  get editor container and call SaveAll()
 *
 * @param            [in] bAskBefore: ask before saving?
 *
*/
void CCEDocuManager::SaveAllFiles(BOOL bAskBefore /*=FALSE*/)
{
    CGenEdCont* pContainer = GetContainer();
    if(pContainer)
    {
        if(bAskBefore)
        {
            pContainer->SaveAll(true);
        }
        else
        {
            pContainer->SaveAll(false);
        }
    }
    // now inform project manager
    CComPtr<ICEProjInfo> pICEProjInfo; 
    GetProjectInfo(&pICEProjInfo);

    if(pICEProjInfo)
    {
        pICEProjInfo->fileParseNow(NULL);
    }
}


//------------------------------------------------------------------*
/**
 * get container.
 *
 * @param           -
 * @return          pointer to editor container.
 * @exception       -
 * @see             -
*/
CGenEdCont* CCEDocuManager::GetContainer()
{
	CCEMAINApp *pApp;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
	assert (pApp != NULL);
	if (pApp == NULL)
	{
		return (NULL);
	}
    return (pApp->GetContainer());
}

//------------------------------------------------------------------*
/**
 * get project info interface.
 *
 * @param           [in/out] pICEProjInfo: project info interface pointer
 * @return          successful?
*/
BOOL CCEDocuManager::GetProjectInfo(ICEProjInfo** ppICEProjInfo)
{
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(!(!pICEProjectInfo));
    if(!pICEProjectInfo)
    {
        return FALSE;
    }
    *ppICEProjInfo = pICEProjectInfo.Detach();
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * select print font.
 *
 * @param           pDCPrint: printer device context
 * @param           pFont: font pointer
 * @param           iHeight: height in low metric units (0.1 mm)
 * @param           iWeight: weight (default is FW_NORMAL)
 * @param           bItalic: italic? default is FALSE
 * @param           iOrientation: angle between x-axis and text in units of 0.1 degree
 *                                  (counterclockwise)
 * @return          -
 * @exception       -
 * @see             -
*/
CFont* CCEDocuManager::SelectPrintFont(CDC* pDCPrint, CFont* pFont, int iHeight, int iWeight /*=FW_NORMAL*/, BOOL bItalic /*=FALSE*/, BOOL bUnderline /*=FALSE*/, int iOrientation /*=0*/)
{
	pFont->CreateFont(-LMToLP(pDCPrint, iHeight), 0, 0, iOrientation, iWeight, bItalic, bUnderline, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, _T("Arial"));
    return pDCPrint->SelectObject(pFont);
}


//------------------------------------------------------------------*
/**
 * get print rect.
 *
 *  in logical units.
 *
 * @param           pDCPrint: printer device context
 * @param           eArea: print area to be returned
 * @return          print area as rect in logical coordinates.
 * @exception       -
 * @see             -
*/
CRect CCEDocuManager::GetPrintArea(CDC* pDCPrint, EPrintArea eArea)
{
    // print rect in logical coordinates
    CRect   tPrintRect;
    int     iPixHorz = pDCPrint->GetDeviceCaps(HORZRES);
    int     iPixVert = pDCPrint->GetDeviceCaps(VERTRES);

    int     iHeaderOffset = GetHeaderHeight(pDCPrint) + LMToLP(pDCPrint, 30);
    int     iFooterOffset = GetFooterHeight(pDCPrint) + LMToLP(pDCPrint, 30);

	tPrintRect.SetRect(0, 0, iPixHorz, iPixVert);
	pDCPrint->DPtoLP(&tPrintRect);

    SetMargins(pDCPrint, tPrintRect);

    switch(eArea)
    {
    case ePA_full:
        return tPrintRect;
        break;
    case ePA_client:
        tPrintRect.top += iHeaderOffset;
        tPrintRect.bottom -= iFooterOffset;
        break;
    case ePA_header:
        tPrintRect.bottom = tPrintRect.top + iHeaderOffset;
        break;
    case ePA_footer:
        tPrintRect.top = tPrintRect.bottom - iFooterOffset;
        break;
    }
    return tPrintRect;
}



//------------------------------------------------------------------*
/**
 * low metric to logical units.
 *
 *  converts low metric units (0.1 mm) to logical units.
 *
 * @param           pDC: device context.
 * @param           iLM: low metric units
 * @return          logical units
 * @exception       -
 * @see             -
*/
int CCEDocuManager::LMToLP(CDC* pDC, int iLM)
{
    int iOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
    CSize   tSize(0, iLM);

    pDC->LPtoDP(&tSize);
    pDC->SetMapMode(iOldMapMode);
    pDC->DPtoLP(&tSize);
    return tSize.cy;
}


//------------------------------------------------------------------*
/**
 * low metric to logical units
 *
 * @param           pDC: device context.
 * @param           rtSize: size
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::LMToLP(CDC* pDC, CSize& rtSize)
{
    int iOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
    pDC->LPtoDP(&rtSize);
    pDC->SetMapMode(iOldMapMode);
    pDC->DPtoLP(&rtSize);
}

//------------------------------------------------------------------*
/**
 * low metric to logical units
 *
 * @param           pDC: device context.
 * @param           rtRect: rect object
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEDocuManager::LMToLP(CDC* pDC, CRect& rtRect)
{
    int iOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
    pDC->LPtoDP(&rtRect);
    pDC->SetMapMode(iOldMapMode);
    pDC->DPtoLP(&rtRect);
}

//------------------------------------------------------------------*
/**
 * set margins.
 *
 * @param           pDCPrint: printer device context.
 * @param           pPrintInfo: print info
 * @return          
 * @exception       -
 * @see             
*/
void CCEDocuManager::SetMargins(CDC* pDCPrint, CRect& rtRect)
{
    // 10 mm margin in all directions
    int iMarginPix      = LMToLP(pDCPrint, 100);
    int iMarginLeft     = iMarginPix;
    int iMarginRight    = iMarginPix;
    int iMarginTop      = iMarginPix;
    int iMarginBottom   = iMarginPix;

    rtRect.left += iMarginLeft;
    rtRect.right -= iMarginRight;
    rtRect.top += iMarginTop;
    rtRect.bottom -= iMarginBottom;
}



//------------------------------------------------------------------*
/**
 * print cover page.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEDocuManager::PrintCoverPage(CDC * pDCPrint, CPrintInfo* pPrintInfo)
{
    // if not whole project is printed, do not print cover page
    CPrintUserData* pData = (CPrintUserData*)pPrintInfo->m_lpUserData;
    UINT uiNumPages = pData->GetPageCount();
    CString strObjName;
    strObjName.LoadString(IDS_PP_COVERPAGE);
    pData->SetObjectName(strObjName);
    if(pPrintInfo->GetMinPage() > 1 || pPrintInfo->GetMaxPage() < uiNumPages)
    {
        return;
    }

    CPrintSettings*  pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(!pPrintSettings)
    {
        return;
    }

    CCoverPage& rtCoverPage = pPrintSettings->m_tCoverPage;

    CRect   tPrintAreaRect;
    tPrintAreaRect = GetPrintArea(pDCPrint, ePA_client);

    CString     strText;
    CBitmapRect tBitmapDrw;
    CTextRect   tPrjNameCapDrw;
    CTextRect   tPrjNameDrw;
    CDrawGrid   tPrjInfoGrid;

    CFont       tCapFontBig;
    CFont       tFontBig;
    CFont       tCapFont;
    CFont       tFont;
    CFont*      pOldFont;

    // start page
    pDCPrint->StartPage();

    pOldFont = SelectPrintFont(pDCPrint, &tCapFontBig, 60, FW_BOLD);
    SelectPrintFont(pDCPrint, &tCapFont, 45, FW_BOLD);
    SelectPrintFont(pDCPrint, &tFontBig, 60);
    SelectPrintFont(pDCPrint, &tFont, 45);

    // sizing and positioning of bitmap
    CSize       tSize(LMToLP(pDCPrint, PLY_COVPAG_BITMAP_WIDTH), LMToLP(pDCPrint, PLY_COVPAG_BITMAP_HEIGHT));
    CPoint      tPosition(GetRectFixpoint(tPrintAreaRect, eHA_center, eVA_top));
    tPosition.Offset(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_TOP));

    tBitmapDrw.SetHorizontalAlignment(eHA_center);
    tBitmapDrw.SetVerticalAlignment(eVA_top);
    tBitmapDrw.SetPosition(tPosition);
    tBitmapDrw.SetSize(tSize);
    tBitmapDrw.SetBitmapPathName(ReplaceMacros(pPrintInfo, rtCoverPage.m_strBitmapFile));
    tBitmapDrw.SetDrawMode(eBM_fit);
    tBitmapDrw.SetDrawFlags(DF_BITMAP); // don't draw frame
    tBitmapDrw.Draw(pDCPrint);

    // sizing and positioning of project caption
    tBitmapDrw.SetVerticalAlignment(eVA_bottom);
    tPosition = tBitmapDrw.GetPosition();
    tPosition.Offset(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_PRJCAP));

    tSize.cx = tPrintAreaRect.right - tPrintAreaRect.left;
    tSize.cy = LMToLP(pDCPrint, PLY_COVPAG_OFFSET_CAPFLD);
    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjNameCaption);
    tPrjNameCapDrw.SetText(strText);
    tPrjNameCapDrw.SetHorizontalAlignment(eHA_center);
    tPrjNameCapDrw.SetVerticalAlignment(eVA_top);
    tPrjNameCapDrw.SetPosition(tPosition);
    tPrjNameCapDrw.SetSize(tSize);
    tPrjNameCapDrw.SetFont(&tCapFontBig);
    tPrjNameCapDrw.SetHorizontalTextAlignment(eHA_center);
    tPrjNameCapDrw.SetVerticalTextAlignment(eVA_mid);
    tPrjNameCapDrw.SetDrawFlags(DF_TEXT);   // only text
    tPrjNameCapDrw.Draw(pDCPrint);

    // sizing and positioning of project name
    tPosition.Offset(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_PRJCAP));
    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjName);
    tPrjNameDrw.SetText(strText);
    tPrjNameDrw.SetHorizontalAlignment(eHA_center);
    tPrjNameDrw.SetVerticalAlignment(eVA_top);
    tPrjNameDrw.SetPosition(tPosition);
    tPrjNameDrw.SetSize(tSize);
    tPrjNameDrw.SetFont(&tFontBig);
    tPrjNameDrw.SetHorizontalTextAlignment(eHA_center);
    tPrjNameDrw.SetVerticalTextAlignment(eVA_mid);
    tPrjNameDrw.SetDrawFlags(DF_TEXT);   // only text
    tPrjNameDrw.Draw(pDCPrint);
    
    // sizing and positioning of project info grid
    tPosition.Offset(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_INFO));
    tSize.cy = LMToLP(pDCPrint, PLY_COVPAG_OFFSET_INFO_HEIGHT);
    tPrjInfoGrid.SetHorizontalAlignment(eHA_center);
    tPrjInfoGrid.SetVerticalAlignment(eVA_top);
    tPrjInfoGrid.SetHorizontalTextAlignment(eHA_left);
    tPrjInfoGrid.SetVerticalTextAlignment(eVA_top);
    tPrjInfoGrid.SetPosition(tPosition);
    tPrjInfoGrid.SetSize(tSize);
    tPrjInfoGrid.SetGridSize(3, 3);
    tPrjInfoGrid.SetDrawFlags(DF_GRID_RECTS);

    tPrjInfoGrid.SetColumnWidth(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_LEFT));
    tPrjInfoGrid.SetColumnWidth(1, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_LEFT2));
    
    // do not draw grid lines
    UINT    uiOldFlags = tPrjInfoGrid.GetDrawFlags();
    tPrjInfoGrid.SetDrawFlags(uiOldFlags & ~DF_GRID);

    CTextRect*  pPrjNoCapDrw = new CTextRect;
    CTextRect*  pPrjNoDrw = new CTextRect;
    CTextRect*  pPrjManCapDrw = new CTextRect;
    CTextRect*  pPrjManDrw = new CTextRect;
    CTextRect*  pCommentCapDrw = new CTextRect;
    CTextRect*  pCommentDrw = new CTextRect;

    tPrjInfoGrid.SetDrawRect(1,0, pPrjNoCapDrw);
    tPrjInfoGrid.SetDrawRect(2,0, pPrjNoDrw);
    tPrjInfoGrid.SetDrawRect(1,1, pPrjManCapDrw);
    tPrjInfoGrid.SetDrawRect(2,1, pPrjManDrw);
    tPrjInfoGrid.SetDrawRect(1,2, pCommentCapDrw);
    tPrjInfoGrid.SetDrawRect(2,2, pCommentDrw);

    tPrjInfoGrid.SetRowHeight(0, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_INFO_ROWHEIGHT));
    tPrjInfoGrid.SetRowHeight(1, LMToLP(pDCPrint, PLY_COVPAG_OFFSET_INFO_ROWHEIGHT));

    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjNoCaption);
    pPrjNoCapDrw->SetText(strText);
    pPrjNoCapDrw->SetFont(&tCapFont);
    
    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjNo);
    pPrjNoDrw->SetText(strText);
    pPrjNoDrw->SetFont(&tFont);
    
    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjManagerCaption);
    pPrjManCapDrw->SetText(strText);
    pPrjManCapDrw->SetFont(&tCapFont);

    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strPrjManager);
    pPrjManDrw->SetText(strText);
    pPrjManDrw->SetFont(&tFont);

    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strCommentCaption);
    pCommentCapDrw->SetText(strText);
    pCommentCapDrw->SetFont(&tCapFont);

    strText = ReplaceMacros(pPrintInfo, rtCoverPage.m_strComment);
    pCommentDrw->SetText(strText);
    pCommentDrw->SetFont(&tFont);

    tPrjInfoGrid.CalculateLayout();
    tPrjInfoGrid.Draw(pDCPrint);

    PrintStdHeader(pDCPrint, pPrintInfo);
    PrintStdFooter(pDCPrint, pPrintInfo);

    pDCPrint->EndPage();
    pDCPrint->SelectObject(pOldFont);
}

//------------------------------------------------------------------*
/**
 * print standard header.
 *
 * @param           pDCPrint: printer device context
 * @param           pPrintInfo: printer information object
 * @return          -
 * @exception       -
 * @see             PrintStdFooter()
*/
void CCEDocuManager::PrintStdHeader(CDC* pDCPrint, CPrintInfo* pPrintInfo)
{
    CPrintSettings* pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(!pPrintSettings)
    {
        return;
    }
    CPrintHeader& rtPrintHeader = pPrintSettings->m_tPrintHeader;

    CRect   tHeaderArea = GetPrintArea(pDCPrint, ePA_header);

    CBitmapRect tLeftBitmapDrw;
    CBitmapRect tRightBitmapDrw;
    CTextRect   tTitleDrw;
    CSize       tSize(LMToLP(pDCPrint, PLY_HEADER_BITMAP_WIDTH), LMToLP(pDCPrint, PLY_HEADER_BITMAP_HEIGHT));
    CPoint      tPosition;
    CRect       tTitleRect;
    CRect       tLeftBitmapRect;
    CRect       tRightBitmapRect;

    // bitmaps
    tPosition = tHeaderArea.TopLeft();
    tLeftBitmapDrw.SetHorizontalAlignment(eHA_left);
    tLeftBitmapDrw.SetVerticalAlignment(eVA_top);
    tLeftBitmapDrw.SetSize(tSize);
    tLeftBitmapDrw.Align(tHeaderArea);
    tLeftBitmapDrw.SetBitmapPathName(ReplaceMacros(pPrintInfo, rtPrintHeader.m_strLeftBitmapFile));
    tLeftBitmapDrw.SetDrawMode(eBM_fit);

    tRightBitmapDrw.SetHorizontalAlignment(eHA_right);
    tRightBitmapDrw.SetVerticalAlignment(eVA_top);
    tRightBitmapDrw.SetSize(tSize);
    tRightBitmapDrw.Align(tHeaderArea);
    tRightBitmapDrw.SetBitmapPathName(ReplaceMacros(pPrintInfo, rtPrintHeader.m_strRightBitmapFile));
    tRightBitmapDrw.SetDrawMode(eBM_fit);

    // title
    CFont   tTitleFont;
    CFont*  pOldFont = SelectPrintFont(pDCPrint, &tTitleFont, 60, FW_BOLD);
    CString strText;

    tLeftBitmapRect = tLeftBitmapDrw.GetRect();
    tRightBitmapRect = tRightBitmapDrw.GetRect();

    tTitleRect.top = tLeftBitmapRect.top;
    tTitleRect.bottom = tLeftBitmapRect.bottom;
    tTitleRect.left = tLeftBitmapRect.right;
    tTitleRect.right = tRightBitmapRect.left;

    strText = ReplaceMacros(pPrintInfo, rtPrintHeader.m_strTitle);
    tTitleDrw.SetText(strText);
    tTitleDrw.SetRect(tTitleRect);
    tTitleDrw.SetFont(&tTitleFont);
    tTitleDrw.SetHorizontalTextAlignment(eHA_center);

    // now draw
    tLeftBitmapDrw.Draw(pDCPrint);
    tRightBitmapDrw.Draw(pDCPrint);
    tTitleDrw.Draw(pDCPrint);
    
}


//------------------------------------------------------------------*
/**
 * print standard footer.
 *
 * @param           pDCPrint: printer device context
 * @param           pPrintInfo: printer information object
 * @return          -
 * @exception       -
 * @see             PrintStdHeader()
*/
void CCEDocuManager::PrintStdFooter(CDC * pDCPrint, CPrintInfo* pPrintInfo)
{
    CPrintSettings* pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(!pPrintSettings)
    {
        return;
    }
    CPrintFooter& rtPrintFooter = pPrintSettings->m_tPrintFooter;

    CRect   tFooterArea = GetPrintArea(pDCPrint, ePA_footer);

    CSize       tSize;
    CPoint      tPosition;

    CFont       tCapFont;
    CFont       tFont;
    CFont*      pOldFont;

    pOldFont = SelectPrintFont(pDCPrint, &tCapFont, 22, FW_BOLD);
    SelectPrintFont(pDCPrint, &tFont, 22);

    // draw revision part
    CDrawGrid   tRevGrid;
    int         iRevPartWidth;
    iRevPartWidth = PLY_FOOTER_REVNO_WIDTH + PLY_FOOTER_REVCHDESCR_WIDTH + 
                    PLY_FOOTER_REVCHDATE_WIDTH + PLY_FOOTER_REVCHNAME_WIDTH + 
                    PLY_FOOTER_REVCAP_WIDTH + PLY_FOOTER_REVFLD_WIDTH;

    tSize.cx = LMToLP(pDCPrint, iRevPartWidth);
    tSize.cy = LMToLP(pDCPrint, PLY_FOOTER_FIELD_HEIGHT*4);

    tPosition = GetRectFixpoint(tFooterArea, eHA_left, eVA_bottom);

    // set size and position
    tRevGrid.SetSize(tSize);
    tRevGrid.SetHorizontalAlignment(eHA_left);
    tRevGrid.SetVerticalAlignment(eVA_bottom);
    tRevGrid.SetPosition(tPosition);
    tRevGrid.SetGridSize(6, 4);
    tRevGrid.SetTextOffset(LMToLP(pDCPrint, 3));
    tRevGrid.SetColumnWidth(0, LMToLP(pDCPrint, PLY_FOOTER_REVNO_WIDTH));
    tRevGrid.SetColumnWidth(1, LMToLP(pDCPrint, PLY_FOOTER_REVCHDESCR_WIDTH));
    tRevGrid.SetColumnWidth(2, LMToLP(pDCPrint, PLY_FOOTER_REVCHDATE_WIDTH));
    tRevGrid.SetColumnWidth(3, LMToLP(pDCPrint, PLY_FOOTER_REVCHNAME_WIDTH));
    tRevGrid.SetColumnWidth(4, LMToLP(pDCPrint, PLY_FOOTER_REVCAP_WIDTH));

    // fill fields
    CTextRect*  pTextRect;
    CString     strText;

    // column 0
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevNo[0]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(0, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevNo[1]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(0, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevNo[2]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(0, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevNoCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(0, 3, pTextRect);
    }
    // column 1
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDescr[0]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(1, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDescr[1]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(1, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDescr[2]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(1, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDescrCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(1, 3, pTextRect);
    }
    // column 2
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDate[0]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(2, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDate[1]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(2, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDate[2]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(2, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeDateCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(2, 3, pTextRect);
    }
    // column 3
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeName[0]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(3, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeName[1]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(3, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeName[2]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(3, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strChangeNameCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(3, 3, pTextRect);
    }
    // column 4
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevDateCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(4, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevDrawnCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(4, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevApprovedCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(4, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevReleasedCaption);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tCapFont);
        tRevGrid.SetDrawRect(4, 3, pTextRect);
    }
    // column 5
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevDate);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(5, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevDrawn);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(5, 1, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevApproved);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(5, 2, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strRevReleased);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetFont(&tFont);
        tRevGrid.SetDrawRect(5, 3, pTextRect);
    }
    tRevGrid.CalculateLayout();
    tRevGrid.Draw(pDCPrint);

    // info part
    tPosition = tFooterArea.BottomRight();
    tSize.cx = LMToLP(pDCPrint, PLY_FOOTER_PRINTDATE_WIDTH + PLY_FOOTER_PRJPATH_WIDTH);

    CDrawGrid   tInfoBaseGrid;  // base grid (1 column, 2 rows)
    CDrawGrid*  pSubGrid;       // pointer to sub grid where needed
    CDrawGrid*  pSubSubGrid;    // pointer to sub sub grid where needed

    tInfoBaseGrid.SetSize(tSize);
    tInfoBaseGrid.SetHorizontalAlignment(eHA_right);
    tInfoBaseGrid.SetVerticalAlignment(eVA_bottom);
    tInfoBaseGrid.SetPosition(tPosition);
    tInfoBaseGrid.SetGridSize(1, 2);
    tInfoBaseGrid.SetTextOffset(LMToLP(pDCPrint, 3));
    tInfoBaseGrid.SetRowHeight(0, LMToLP(pDCPrint, PLY_FOOTER_FIELD_HEIGHT * 2));
    tInfoBaseGrid.SetFont(&tFont);  // set same font for all text rects

    // upper part containing sub grid for date and time and sub sub grid
    // for project path and object name
    pSubGrid = new CDrawGrid;
    pSubGrid->SetGridSize(2,1);
    pSubGrid->SetColumnWidth(0, LMToLP(pDCPrint, PLY_FOOTER_PRINTDATE_WIDTH));

    tInfoBaseGrid.SetDrawRect(0, 0, pSubGrid);

    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strDateAndTime);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetVerticalTextAlignment(eVA_top);
        pSubGrid->SetDrawRect(0, 0, pTextRect);
    }

    // project path and object name sub sub grid
    pSubSubGrid = new CDrawGrid;
    pSubSubGrid->SetGridSize(1,2);

    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strPrjPath);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strObjName);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 1, pTextRect);
    }
    pSubGrid->SetDrawRect(1, 0, pSubSubGrid);

    // lower part containing subgrid for info 0 - 3 and page count fields (3 columns, 1 row)
    pSubGrid = new CDrawGrid;
    pSubGrid->SetGridSize(3, 1);
    pSubGrid->SetColumnWidth(0, LMToLP(pDCPrint, PLY_FOOTER_INFOFLD_WIDTH));
    pSubGrid->SetColumnWidth(1, LMToLP(pDCPrint, PLY_FOOTER_INFOFLD_WIDTH));
    tInfoBaseGrid.SetDrawRect(0, 1, pSubGrid);

    // sub sub grid for info 0 + 1 fields
    pSubSubGrid = new CDrawGrid;
    pSubSubGrid->SetGridSize(1, 2);
    pSubGrid->SetDrawRect(0, 0, pSubSubGrid);

    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strInfo[0]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strInfo[1]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 1, pTextRect);
    }

    // info 2 field
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strInfo[2]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pTextRect->SetVerticalTextAlignment(eVA_top);
        pSubGrid->SetDrawRect(1, 0, pTextRect);
    }

    // page count and info 3 sub sub grid
    pSubSubGrid = new CDrawGrid;
    pSubSubGrid->SetGridSize(1, 2);
    pSubGrid->SetDrawRect(2, 0, pSubSubGrid);

    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strPageCount);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 0, pTextRect);
    }
    strText = ReplaceMacros(pPrintInfo, rtPrintFooter.m_strInfo[3]);
    if(!strText.IsEmpty())
    {
        pTextRect = new CTextRect;
        pTextRect->SetText(strText);
        pSubSubGrid->SetDrawRect(0, 1, pTextRect);
    }

    tInfoBaseGrid.CalculateLayout();
    tInfoBaseGrid.Draw(pDCPrint);

    CBitmapRect tBitmapDrw;

    CRect   tRevRect = tRevGrid.GetRect();
    CRect   tInfoRect = tInfoBaseGrid.GetRect();
    CRect   tBitmapRect;
    tBitmapRect.top = tRevRect.top;
    tBitmapRect.left = tRevRect.right;
    tBitmapRect.right = tInfoRect.left;
    tBitmapRect.bottom = tRevRect.bottom;

    tBitmapDrw.SetRect(tBitmapRect);
    tBitmapDrw.SetBitmapPathName(ReplaceMacros(pPrintInfo, rtPrintFooter.m_strCenterBitmap));
    tBitmapDrw.SetDrawMode(eBM_fit);
    tBitmapDrw.Draw(pDCPrint);

    pDCPrint->SelectObject(pOldFont);
}


//------------------------------------------------------------------*
/**
 * set printing area.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEDocuManager::SetPrintingArea(CDC* pDCPrint, CPrintInfo* pPrintInfo)
{
    pPrintInfo->m_rectDraw = GetPrintArea(pDCPrint, ePA_client);
}

//------------------------------------------------------------------*
/**
 * get header height.
 *
 * @param           pDCPrint: printer device context
 * @return          header height in logical units
 * @exception       -
 * @see             -
*/
int CCEDocuManager::GetHeaderHeight(CDC* pDCPrint)
{
    int iHeaderHeight = 0;
    CPrintSettings* pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(pPrintSettings)
    {
        iHeaderHeight = LMToLP(pDCPrint, pPrintSettings->GetHeaderHeight());
    }
    return iHeaderHeight;
}


//------------------------------------------------------------------*
/**
 * get footer height.
 *
 * @param           pDCPrint: printer device context
 * @return          footer height in logical units
 * @exception       -
 * @see             -
*/
int CCEDocuManager::GetFooterHeight(CDC* pDCPrint)
{
    int iFooterHeight = 0;
    CPrintSettings* pPrintSettings = GetPrintSettings();
    ASSERT(pPrintSettings);
    if(pPrintSettings)
    {
        iFooterHeight = LMToLP(pDCPrint, pPrintSettings->GetFooterHeight());
    }
    return iFooterHeight;
}

//------------------------------------------------------------------*
/**
 * get print info.
 *
 *  get printer name and page size
 *
 * @param           strPrinterName: printer name
 * @param           plWidth: page width in logical units
 * @param           plHeight: page height in logical units
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::GetPrintInfo(CString &strPrinterName, long* plWidth, long* plHeight) 
{
	CPrintInfo PrintInfo;
    
    if((plWidth == NULL) || (plHeight == NULL)) return(FALSE);
    // prepare printing
    CPrintDialog PrintDlg(FALSE);  // Instantiate a CPrintDialog object.

    // Retrieve the current printer defaults from the Windows .ini file
    // as necessary, or uses the last printer configuration set by the
    // user in Print Setup.
    CCEMAINApp * pApp = (CCEMAINApp *)AfxGetApp();
    ASSERT(pApp != NULL);

    // select printer
    if(!pApp->GetPrinterDeviceDefaults(&PrintDlg.m_pd))
    {
        return(FALSE);
    }

    CDC dcPrint;
    if(PrintDlg.CreatePrinterDC() == NULL) 
    {
        return (FALSE);
    }

    dcPrint.Attach(PrintDlg.GetPrinterDC()); 

    SetPrintingArea(&dcPrint, &PrintInfo);
    CSize Size(PrintInfo.m_rectDraw.Width(), PrintInfo.m_rectDraw.Height());
    dcPrint.LPtoHIMETRIC(&Size);
    
    strPrinterName = PrintDlg.GetDeviceName();
    *plWidth = Size.cx;
    *plHeight = Size.cy;
    return(TRUE);
}


//------------------------------------------------------------------*
/**
 * prepare printing.
 *
 * @param           pPrintInfo: print info object
 * @param           pDlgPrintStatus: print status dialog
 * @param           pdcPrint: printer device context
 * @param           pszDocName: document name
 * @return          S_OK: ok
 *                  S_FALSE: cancel
 *                  E_FAIL: error
 * @exception       -
 * @see             -
*/
HRESULT CCEDocuManager::PreparePrinting
(
    CPrintInfo*         pPrintInfo,
    CPrjPrintDialog*    pDlgPrintStatus,
    CDC*                pdcPrint,
    LPCTSTR             pszDocName
)
{
    CCEMAINApp * pApp = (CCEMAINApp *)AfxGetApp();

    // Page selection disabled
    // pPrintInfo->m_pPD->m_pd.Flags |= PD_NOPAGENUMS;
    pPrintInfo->m_pPD->m_pd.Flags |= PD_DISABLEPRINTTOFILE;

	if (IDOK != pApp->DoPrintDialog(pPrintInfo->m_pPD))
    {
		return S_FALSE;       // do not print
    }

    // print all or a selected range of pages
    if (pPrintInfo->m_pPD->m_pd.Flags & PD_PAGENUMS)
    {
        pPrintInfo->SetMinPage(pPrintInfo->m_pPD->m_pd.nFromPage);
        pPrintInfo->SetMaxPage(pPrintInfo->m_pPD->m_pd.nToPage);
    }
    else
    {
        pPrintInfo->m_pPD->m_pd.nFromPage = 1;
        pPrintInfo->m_pPD->m_pd.nToPage = 0xffff;
        pPrintInfo->SetMinPage(1);
        pPrintInfo->SetMaxPage(0xffff);
    }

	if (NULL == pPrintInfo->m_pPD->m_pd.hDC)
    {
		return E_FAIL;
    }

	// gather file to print to if print-to-file selected
	CString strOutput;
	if (pPrintInfo->m_pPD->m_pd.Flags & PD_PRINTTOFILE)
	{
		// construct CFileDialog for browsing
		CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
		CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
		CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
		CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
		CFileDialog dlg(FALSE, strDef, strPrintDef,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter);
		dlg.m_ofn.lpstrTitle = strCaption;

		if (IDOK != dlg.DoModal())
        {
			return S_FALSE;
        }

		// set output device to resulting path name
		strOutput = dlg.GetPathName();
	}

	// set up document info and start the document printing process
	DOCINFO docInfo;

	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = pszDocName;
	CString strPortName;
	int nFormatID;
	if (strOutput.IsEmpty())
	{
		docInfo.lpszOutput = NULL;
		strPortName = pPrintInfo->m_pPD->GetPortName();
		nFormatID = AFX_IDS_PRINTONPORT;
	}
	else
	{
		docInfo.lpszOutput = strOutput;
		nFormatID = AFX_IDS_PRINTTOFILE;
	}

	// setup the printing DC
	pdcPrint->Attach(pPrintInfo->m_pPD->m_pd.hDC);  // attach printer dc
	pdcPrint->m_bPrinting = TRUE;

	pdcPrint->SetAbortProc(PrjPrintAbortProc);

	// disable main window while printing & init printing status dialog
	AfxGetMainWnd()->EnableWindow(FALSE);

	CString strTemp;

    pDlgPrintStatus->SetDlgItemText(AFX_IDC_PRINT_DOCNAME, pszDocName);
	pDlgPrintStatus->SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
		pPrintInfo->m_pPD->GetDeviceName());
	AfxFormatString1(strTemp, nFormatID, strPortName);
	pDlgPrintStatus->SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);

    pDlgPrintStatus->ShowWindow(SW_SHOW);
    pDlgPrintStatus->UpdateWindow();

	if (pdcPrint->StartDoc(&docInfo) == SP_ERROR)
	{
		// enable main window before proceeding
		AfxGetMainWnd()->EnableWindow(TRUE);

		// cleanup and show error message
    	pDlgPrintStatus->DestroyWindow();
		pdcPrint->Detach();   // will be cleaned up by CPrintInfo destructor
		AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
		return E_FAIL;
	}

    return S_OK;
}


//------------------------------------------------------------------*
/**
 * replace macros.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CCEDocuManager::ReplaceMacros(CPrintInfo* pPrintInfo, const CString& crstrText)
{
    CString strText(crstrText);

    CComPtr<ICEProjInfo>    pProjInfo;
    if(!GetProjectInfo(pProjInfo))
    {
        return strText;
    }

    // some work can be done by project manager
    CComBSTR    sText(strText);
    CComBSTR    sTextOut;
    CString     strPrjFile;
    HRESULT hr = pProjInfo->ReplaceMacros(sText, &sTextOut);
    if(hr == S_OK)
    {
        strText = sTextOut;
    }

    CComBSTR    sPrjPath;
    CComBSTR    sPrjName;

    hr = pProjInfo->getProjectPath(&sPrjPath);
    ASSERT(hr == S_OK);
    if(hr != S_OK)
    {
        return strText;
    }
    
    hr = pProjInfo->getProjectName(&sPrjName);
    ASSERT(hr == S_OK);
    if(hr != S_OK)
    {
        return strText;
    }

    CString strPrjPath = sPrjPath;
    CString strPrjName = sPrjName;
    strPrjFile.Format("%s\\%s.4cp", strPrjPath, strPrjName);

    // now replace $(ObjId), $(PrjDate), $(PrjDate#eng), $(PrjDate#ger), 
    // $(PrintDate), $(PrintDate#eng), $(PrintDate#ger), $(PageCnt), $(Page)
    CString strReplace;
    CString strFind;
    int     iFound;
    strFind = _T("$(Page)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        CPrintUserData* pData = (CPrintUserData*)pPrintInfo->m_lpUserData;
        int iPageNum = pData->GetPageNum();
//        if(iPageNum > 0)
//        {
            strReplace.Format("%d", pData->GetPageNum());
//        }
//        else
//        {
//            strReplace.LoadString(IDS_PAGE_NUMBER);
//        }
        strText.Replace(strFind, strReplace);
    }

    strFind = _T("$(PageCount)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        CPrintUserData* pData = (CPrintUserData*)pPrintInfo->m_lpUserData;
        int iPageNum = pData->GetPageNum();
//        if(iPageNum > 0)
//        {
            strReplace.Format("%d", pData->GetPageCount());
//        }
//        else
//        {
//            strReplace.LoadString(IDS_PAGE_COUNT);
//        }
        strText.Replace(strFind, strReplace);
    }

    // get object resp file name
    strFind = _T("$(ObjName)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        CPrintUserData* pData = (CPrintUserData*)pPrintInfo->m_lpUserData;
        strReplace = pData->GetObjectName();
        strText.Replace(strFind, strReplace);
    }


    // get print time := current time
    CTime   tPrintTime(CTime::GetCurrentTime());
    strFind = _T("$(PrintDate)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrintTime.Format(IDS_DATE_TIME_FORMAT_SHORT_ENG);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(PrintDate#eng)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrintTime.Format(IDS_DATE_TIME_FORMAT_SHORT_ENG);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(PrintDate#ger)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrintTime.Format(IDS_DATE_TIME_FORMAT_SHORT_GER);
        strText.Replace(strFind, strReplace);
    }

    // get project time := last access time of 4cp file.
    CTime   tPrjTime;
    CFileStatus tStatus;
    if(!CFile::GetStatus(strPrjFile, tStatus))
    {
        return strText;
    }
    tPrjTime = tStatus.m_mtime;
    
    strFind = _T("$(PrjDate)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrjTime.Format(IDS_DATE_TIME_FORMAT_SHORT_ENG);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(PrjDate#eng)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrjTime.Format(IDS_DATE_TIME_FORMAT_SHORT_ENG);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(PrjDate#ger)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace = tPrjTime.Format(IDS_DATE_TIME_FORMAT_SHORT_ENG);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(ProgName)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace.LoadString(IDS_PP_PROGRAM_NAME);
        strText.Replace(strFind, strReplace);
    }
    strFind = _T("$(Copyright)");
    iFound = strText.Find(strFind);
    if(iFound != -1)
    {
        strReplace.LoadString(IDS_PP_COPYRIGHT);
        strText.Replace(strFind, strReplace);
    }
    return strText;
}


//------------------------------------------------------------------*
/**
 * get project info.
 *
 * @param           pProjInfo: project info
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEDocuManager::GetProjectInfo(CComPtr<ICEProjInfo>& pProjInfo)
{
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    if(!pICEProjectInfo)
    { 
        return FALSE;
    }
    pProjInfo = pICEProjectInfo;
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * show print options dialog.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEDocuManager::ShowPrintOptionsDialog()
{
    CGenEdCont* pContainer = GetContainer();
    if(!pContainer) 
    {
        return;
    }

    CGenEdDoc*  pDoc = pContainer->GetServerForPrintOptions(_T("4gr"));
    if(pDoc)
    {
        CGenEdCntrItem* pItem = pDoc->GetEditorServer();
        if(pItem)
        {
            ICEPrint*   pPrint;
            pPrint = pItem->GetICEPrint();
            if(pPrint)
            {
                CComBSTR    sPrintOptions;
                pPrint->PRShowPrintOptionsDialog(&sPrintOptions);
                pPrint->PRSetPrintOptions(sPrintOptions);
                pPrint->Release();
            }
        }
        pDoc->OnCloseDocument();
    }

//    // load all print servers
//    pContainer->LoadPrintServers();
//
//    // get print servers with options and store module names
//    HRESULT         hr;
//    CGenEdCntrItem*     pItem;
//    CGenEdDoc* pDoc = pContainer->GetFirstPrintServer();
//    ASSERT(pDoc);
//    if(!pDoc)
//    {
//        return;
//    }
//    pItem = pDoc->GetEditorServer();
//
//    ICEPrint*       pPrint;
//    CSBLLexTree<CGenEdDoc>  tPrintDocAssoc;
//    while(pItem)
//    {
//        pPrint = pItem->GetICEPrint();
//        if(pPrint)
//        {
//            CComBSTR    sPrintOptions;
//            hr = pPrint->PRGetPrintOptions(&sPrintOptions);
//            if(hr == S_OK)
//            {
//                CComBSTR    sModuleName;
//                hr = pPrint->PRGetModuleName(&sModuleName);
//                ASSERT(hr == S_OK);
//                if(hr == S_OK)
//                {
//                    tPrintDocAssoc.insertData((CString)sModuleName, pDoc);
//                }
//            }
//            pPrint->Release();
//        }
//        pDoc = pContainer->GetNextPrintServer();
//        if(pDoc)
//        {
//            pItem = pDoc->GetEditorServer();
//        }
//        else
//        {
//            pItem = NULL;
//        }
//    }
//
//    // fill print options dialog and show it
//    CStringArray        astrPrintModules;
//    CPrintOptionsDlg    tDlg;
//    tPrintDocAssoc.getAllStringsFromSubstring("", astrPrintModules); 
//    tDlg.SetPrintModules(astrPrintModules);
//    if(tDlg.DoModal() == IDOK)
//    {
//        // show print options dialog of module
//        CString strPrintModule = tDlg.GetPrintModule();
//        pDoc = tPrintDocAssoc.getData(strPrintModule);
//        pItem = pDoc->GetEditorServer();
//        ASSERT(pItem);
//        if(pItem)
//        {
//            pPrint = pItem->GetICEPrint();
//            ASSERT(pPrint);
//            if(pPrint)
//            {
//                hr = pPrint->PRShowPrintOptionsDialog();
//            }
//        }
//    }
//
//    // release all print servers.
//    int iNumModules = astrPrintModules.GetSize();
//    for(int iModule = 0; iModule < iNumModules; ++iModule)
//    {
//        pPrint = tPrintDocAssoc.getData(astrPrintModules[iModule]);
//        ASSERT(pPrint);
//        pPrint->Release();
//    }
//    pContainer->UnloadPrintServers();
}


// SIS: 01.03.02: special feature for KUK: handle GR-Editor options
//------------------------------------------------------------------*
/**
 * get grafic editor header flag.
 *
 * @param           bGrHeader: ST-Header to be printed?
 * @return          successful?
 * @exception       -
 * @see             SetGREditorHeaderFlag()
*/
BOOL CCEDocuManager::GetGREditorHeaderFlag(BOOL& rbGrHeader)
{
    CString strOptions;
    rbGrHeader = TRUE;

    CGenEdCont* pContainer = GetContainer();
    CGenEdDoc* pDoc = NULL;
    if(NULL == pContainer) 
    {
        return FALSE;
    }
    
    pDoc = pContainer->GetServerForFile(_T("4gr"));
    if(pDoc == NULL)
    {
        return FALSE;
    }

    CGenEdCntrItem* pItem = pDoc->GetEditorServer();
    ASSERT(pItem);
    if(!pItem)
    {
        return FALSE;
    }

    ICEPrint*   pPrint = pItem->GetICEPrint();
    HRESULT     hr;
    ASSERT(pPrint);
    if(pPrint)
    {
        CComBSTR    sOptions(strOptions);
        hr = pPrint->PRGetPrintOptions(&sOptions);
        if(hr == S_OK)
        {
            CString strOptions(sOptions);
            if(strOptions.Find(_T("STHeader=0")) >= 0)
            {
                rbGrHeader = FALSE;
            }
        }
        pPrint->Release();
    }
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

// SIS: 01.03.02: special feature for KUK: handle GR-Editor options
//------------------------------------------------------------------*
/**
 * set grafic editor header flag.
 *
 * @param           bGrHeader: ST-Header to be printed?
 * @return          successful?
 * @exception       -
 * @see             GetGREditorHeaderFlag()
*/
BOOL CCEDocuManager::SetGREditorHeaderFlag(BOOL bGrHeader /*= TRUE*/)
{
    CString strOptions;
    if(bGrHeader)
    {
        strOptions = _T("STHeader=1");
    }
    else
    {
        strOptions = _T("STHeader=0");
    }

    CGenEdCont* pContainer = GetContainer();
    CGenEdDoc* pDoc = NULL;
    if(NULL == pContainer) 
    {
        return FALSE;
    }
    
    pDoc = pContainer->GetServerForFile(_T("4gr"));
    if(pDoc == NULL)
    {
        return FALSE;
    }

    CGenEdCntrItem* pItem = pDoc->GetEditorServer();
    ASSERT(pItem);
    if(!pItem)
    {
        return FALSE;
    }

    ICEPrint*   pPrint = pItem->GetICEPrint();
    HRESULT hr = E_FAIL;
    ASSERT(pPrint);
    if(pPrint)
    {
        CComBSTR    sOptions(strOptions);
        hr = pPrint->PRSetPrintOptions(sOptions);
        pPrint->Release();
    }
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}
