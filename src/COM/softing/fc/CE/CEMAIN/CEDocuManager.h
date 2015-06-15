#ifndef __CEDOCUMANAGER_H_
#define __CEDOCUMANAGER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define CEDOCU_MAPMODE      MM_LOMETRIC


//----  Aggregate Includes:   --------------------------------------*
#include "GenEdCntrItem.h"
#include "KADHelper\KADFileInfo.h"
#include "CPPFileCollection.h"
#include "XMLHelper\XMLNode.h"
#include "SBLLexTree.h"
#include "GenEdCont.h"
#include "Printing.h"
#include "Settings.h"

//----  Forward Class Definitions:   -------------------------------*
class CCEFileHandler;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

typedef CList<CPPFileCollection*, CPPFileCollection*> TPPFileCollList;

//------------------------------------------------------------------*
/**
 *  class CCEDocuManager
 *
 *  The central class for print documentation.
 *  
 *  - print selection
 *  - print settings
 *  - print control
 *
 *  @see    
*/
class CCEDocuManager
{
    // print area enumerator
    enum EPrintArea
    {
        ePA_full,   // 0: full printable paper area
        ePA_header, // 1: paper area reserved for header
        ePA_footer, // 2: paper area reserved for footer
        ePA_client, // 3: print area available to clients (full - header - footer)
    };

public:

    CCEDocuManager();
    ~CCEDocuManager();

    void Init(CKADFileInfo* pFileInfo);

    BOOL GetPrintInfo(CString &strPrinterName, long* plWidth, long* plHeight);
    
    void SelectPrintFiles();

    HRESULT PrintProject();

    HRESULT PrintSingleDocument(const CString& crstrFilePathName);
    HRESULT PrintActiveDocument();

    // OSAI feature print XRef 24.03.04 SIS >>
    HRESULT PrintXRef(int iMode = -1);		// XREFPRINT 21.07.05 SIS
    // OSAI feature print XRef 24.03.04 SIS <<

    void SetBackgroundFlag(BOOL bBackground = TRUE);

    void ShowPrintOptionsDialog();

    // SIS: 01.03.02: special feature for KUK: handle GR-Editor options
    BOOL GetGREditorHeaderFlag(BOOL& rbGrHeader);
    BOOL SetGREditorHeaderFlag(BOOL bGrHeader = TRUE);

    BOOL IsFilePrintable(const CString& crstrFilePath);
    
protected:

    HRESULT GetPageCount(CGenEdDoc* pDoc, const CString& crstrFileName, long& rlPageCount);

    HRESULT PrintProjectFile(   const CString&  crstrFileName,
                                CDC*            pdcPrint,
                                CPrintInfo*     pPrintInfo,
                                CDialog*        pDlgPrintStatus);

    HRESULT PrintDocument(  CGenEdDoc*      pDoc,
                            const CString&  crstrFileName,
                            CDC*            pDCPrint,
                            CPrintInfo*     pPrintInfo, 
                            CDialog*        pDlgPrintStatus);

    HRESULT PrintProject(const CString& crstrProjName, const CString& crstrProjPath);

    void SaveAllFiles(BOOL bAskBefore = FALSE);

    BOOL SelectPrintProjectFiles(const CString& crstrPrjFile, TPPFileCollList& rtFileCollList);

    BOOL GetAllFilesFromProjMan(enum E_FV_FileCategory eFC, CSBLLexTree<bool>& rtFileNames);
    BOOL GetPrintableFilesFromProjMan(const CString& crstrPrjFile, CStringArray& rastrFiles);
    BOOL GetPrintableFilesFromTrees(TPPFileCollList& rtFileCollList);
    void GetPrintableFilesFromClassTree(CXMLNode& rtRootNode, TPPFileCollList& rtFileCollList);
    void GetPrintableFilesFromTargetTree(CXMLNode& rtRootNode, TPPFileCollList& rtFileCollList);
    void CollectPrintableSourceFiles(CXMLNode& rtNode, CStringArray& rastrFiles);
    
    void FreeFileCollections(TPPFileCollList& rtFileCollList);
    
    BOOL CreateIndexFile(   const CString& crstrIndexFilePathName,
                            const CString& crstrProjPath,
                            TPPFileCollList& rtFileCollList,
                            long lPageOffset);

    long CountPages(const CString& crstrProjPath, TPPFileCollList& rtFileCollList);

    BOOL GetXMLDocuments(CXMLDocument& rtClassDoc, CXMLDocument& rtTargetDoc);

    CGenEdCont* GetContainer();

    BOOL GetProjectInfo(ICEProjInfo** ppICEProjInfo);

    HRESULT PreparePrinting(CPrintInfo * pPrintInfo, CPrjPrintDialog * pDlgPrintStatus,
                        CDC* pdcPrint, LPCTSTR pszDocName);

    CFont* SelectPrintFont( CDC*    pDCPrint, 
                            CFont*  pFont, 
                            int     iHeight, 
                            int     iWeight = FW_NORMAL, 
                            BOOL    bItalic = FALSE,
                            BOOL    bUnderline = FALSE,
                            int     iOrientation = 0);

    void SetMargins(CDC* pDCPrint, CRect& rtRect);

    void SetPrintingArea(CDC* pDCPrint, CPrintInfo* pPrintInfo);

    void PrintCoverPage(CDC* pDCPrint, CPrintInfo* pPrintInfo);
    void PrintStdHeader(CDC* pDCPrint, CPrintInfo* pPrintInfo);
    void PrintStdFooter(CDC* pDCPrint, CPrintInfo* pPrintInfo);

    CString ReplaceMacros(CPrintInfo* pPrintInfo, const CString& crstrText);
    BOOL GetProjectInfo(CComPtr<ICEProjInfo>& pProjInfo);

    CPrintSettings* GetPrintSettings();

    int LMToLP(CDC* pDC, int iLM);
    void LMToLP(CDC* pDC, CSize& rtSize);
    void LMToLP(CDC* pDC, CRect& rtRect);

    CRect GetPrintArea(CDC* pDCPrint, EPrintArea eArea);
    int GetHeaderHeight(CDC* pDCPrint);
    int GetFooterHeight(CDC* pDCPrint);

    void InsertTreeViewFiles(   const CString& crstrProjPath,
                                CPrintSettings* pPrintSettings,
                                TPPFileCollList& rtFileCollList);

    // OSAI feature print XRef 24.03.04 SIS >>
    void InsertCrossRefFile(    const CString& crstrProjPath,
                                CPrintSettings* pPrintSettings,
                                TPPFileCollList& rtFileCollList);
    // OSAI feature print XRef 24.03.04 SIS <<


    BOOL PrintProjectFiles( CPrintInfo*         pPrintInfo,
                            CPrjPrintDialog*    pPrintDlg,
                            CDC*                pDCPrint,
                            CCEFileHandler*     pFileHandler,
                            TPPFileCollList&    rtFileCollList);
protected:
    CKADFileInfo*           m_pFileInfo;        // file extension info from KAD
    BOOL                    m_bPrintProject;    // print project active?
};

#endif // __CEDOCUMANAGER_H_

