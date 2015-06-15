

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <fstream>
#include "DllInterface.h"
#include "InsertGvlImport.h"
#include "FileSelDlg.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CInsertGvlImport::CInsertGvlImport(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
    //m_sName = "Task";
}


CInsertGvlImport::~CInsertGvlImport()
{
    if (m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


BOOL CInsertGvlImport::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    BOOL    bRes = FALSE;

    Init();
    SetFileName();
    if(!ParseResource())
    {
        return FALSE;
    }

    int iCount = m_aStrGvlFilesSel.GetSize();
    CString strFileName;
    BOOL bInsert = FALSE;
    if(iCount == 0)
    {
        return FALSE;
    }

    for(int i=iCount-1; i>=0; --i)
    {
        strFileName = m_aStrGvlFilesSel.GetAt(i);
        bInsert = !DoesImportExist(strFileName);
        if(bInsert)
        {
            GenImport(m_ImportText, strFileName);
            InsertImport();
            bRes = TRUE;
        }
        else
        {
            if(!m_bSilent)
            {
                CString errorMsg;
                errorMsg.Format(IDS_IMPORT_EXISTS, strFileName);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
            }
        }
    }
    if(bRes)
    {
        CString strResourceText = CString(m_ResourceText.c_str());
        WriteFile(strResourceText);
        InformProjectManager();
    }
    if (m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    m_ResourceText.empty();
    return bRes;
}

void CInsertGvlImport::Init()
{
    CStringArray aStrGvlFiles;
    CString strFullFileName;
    strFullFileName = (CString)m_ProjectPath + _T("\\") + _T("*.gvl");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    hFind = FindFirstFile(strFullFileName, &findFileData);
    BOOL bFound = (hFind==INVALID_HANDLE_VALUE)?FALSE:TRUE;

    while (bFound)
    {
        CString strFileName = findFileData.cFileName;
        if (!strFileName.IsEmpty())
        {
            aStrGvlFiles.Add(strFileName);
        }
        bFound = FindNextFile(hFind, &findFileData);
    }

    FindClose(hFind);

    CFileSelDlg     tFileSelDlg;
    CString         strTitle;
    strTitle.LoadString(IDS_GLOB_VARSELEC);

    if(!m_bAutomationMode)
    {
        tFileSelDlg.Initialize(strTitle, aStrGvlFiles);
        if(tFileSelDlg.DoModal() == IDOK)
        {
            tFileSelDlg.GetSelection(m_aStrGvlFilesSel);
        }
    }

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)

    if(m_bAutomationMode)
    {
        SetFileName();
        if(!ParseResource())
        {
            return;
        }
    }
}

// SIS: TODO put this and probably other methods
// in a base class for resource objects
// do not use for reparsing
BOOL CInsertGvlImport::ParseResource()
{
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    m_ResourceText);
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    ASSERT(m_ResourceText.length() != 0);
    parse_string(m_ResourceText, &m_Resource);
    if(m_Resource)
    {
        return TRUE;
    }
    return FALSE;
}


void CInsertGvlImport::GenImport(CString & StrOut, CString strFileName)
{
    CString StrHelp, Strlf(NEWLINE), StrIndent(SPACETAB);

    StrOut = Strlf + StrIndent;
    StrHelp.Format(IDS_KEY_IMPORTFILE, strFileName);
    StrOut += StrHelp;
    //StrOut += Strlf;
}


void CInsertGvlImport::InsertImport()
{
    ASSERT(m_Resource);
    if(m_Resource)
    {
        StringPos insertpos = GetNextInsertPosition(m_ResourceText, m_Resource->getNextImpDirInsertPos());
        m_ResourceText.insert(insertpos, m_ImportText);
    }
}

void CInsertGvlImport::ReadFileContent(const CString fileName, string& buf)
{
    CStdioFile cFile(fileName, CFile::modeRead);
    int iLength = cFile.GetLength();
    char* pbuf;
    pbuf = new char[iLength+1];
    int iCount;
    iCount = cFile.Read(pbuf, iLength);
    pbuf[iCount]=0;
    ASSERT(iCount > 0);
    buf = pbuf;
    delete pbuf;
    cFile.Close();
}

void CInsertGvlImport::SetFileName()
{
    m_sFileName = m_sSourceFile;
}

//------------------------------------------------------------------*
/**
 * does import exist?.
 *
 * @param           [in] strImportName: import file name without path
 * @return          does import exist?
 * @exception       -
 * @see             -
*/
BOOL CInsertGvlImport::DoesImportExist(CString strImportName)
{
    if(!m_Resource)
    {
        return FALSE;
    }

    import_directives_citer iter;
    const import_directives_type Imports = m_Resource->getImportDirectives();
    if(Imports.size() != 0)
    {
        for(iter = Imports.begin(); iter < Imports.end(); iter++)
        {
            CString  strImportNameFromList = ((*iter)->getFileName()).c_str();
            if(strImportName.CompareNoCase((LPCTSTR)strImportNameFromList) == 0)
            {
                return TRUE; //it exists already
            }
        }
    }
    return FALSE;
}

void CInsertGvlImport::AddImportFile(const CString& crstrImportFile)
{
    if(!DoesImportExist(crstrImportFile))
    {
        m_aStrGvlFilesSel.Add(crstrImportFile);
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::InformProjectManager
 * Description		invoke the project manager to parse the resource file	
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGvlImport::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}

