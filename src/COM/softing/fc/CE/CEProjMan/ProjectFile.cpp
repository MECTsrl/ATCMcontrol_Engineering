

//----  Includes:   -------------------------------------------*

#include "stdafx.h"

#include "ProjectFile.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"

#include "CEProjectManager.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





//Methods for CProjectFile

//------------------------------------------------------------------*
/**
 * CProjectFile     - Constructor of CProjectFile
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CProjectFile::CProjectFile(CCEProjectManager* pProjMan)
    : m_firstSection(pProjMan)
{
    PROJ_MAN_ASSERT(pProjMan);
    m_pProjMan = pProjMan;
}


//------------------------------------------------------------------*
/**
 * CProjectFile     - Constructor of CProjectFile
 *
 * @param           - The name of the file to open(for reading and 
                    writing)    
 * @return          
 * @exception       -
 * @see             
*/
CProjectFile::CProjectFile(CCEProjectManager* pProjMan,
                           CString strFileName)
    : m_firstSection(pProjMan)
{
    PROJ_MAN_ASSERT(pProjMan);
    m_pProjMan = pProjMan;
    m_strFileName = strFileName;
    ReparseProjectFile(TRUE);
}


//------------------------------------------------------------------*
/**
 * ~CProjectFile    - Destructor of CProjectFile. Clears all storages
                    used by the CProjectFile.
 *
 * @param
 * @return          
 * @exception       -
 * @see             
*/
CProjectFile::~CProjectFile()
{
    POSITION pos = m_sectionList.GetHeadPosition();
    while(pos)
    {
        CSection* pSection;
        pSection = m_sectionList.GetNext(pos);
        delete pSection;
    }
    m_sectionList.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * SetProjectFile   - Calls ReparseProjectFile() to open and parse the
                    file with the given filename. It writes all Section,
                    Keys and Values in m_listSectionList
 *
 * @param           - Name of the file to open (for reading)
 * @return          
 * @exception       - TRUE if the operation was successfull, else FALSE.
 * @see             
*/
HRESULT CProjectFile::SetProjectFile(CString strFileName)
{
    m_strFileName = strFileName;
    return ReparseProjectFile(TRUE);
}


//------------------------------------------------------------------*
/**
 * ReparseProjectFile   - Opens and parses the file. The name of the 
                        file to open is m_strFileName.
 *
 * @param           
 * @return          TRUE if the operation was successfull, else FALSE.
 * @exception       -
 * @see             
*/
HRESULT CProjectFile::ReparseProjectFile(BOOL bForce)
{
    HRESULT hr = S_OK;
    CFile projFile;
    CFileStatus fileStatus;
    CTime modTime;

    if (!CFile::GetStatus(m_strFileName, fileStatus))
    {
        m_pProjMan->OutputErrorMessage(E_CEPROJMAN_NO_PROJ_FILE, m_strFileName);
        return E_CEPROJMAN_NO_PROJ_FILE;
    }

    modTime = fileStatus.m_mtime;
    
    if (!bForce)
    {
        if (modTime==m_timeLastParse)
        {
            return S_OK;
        }
    }


    if (!projFile.Open(m_strFileName, CFile::modeRead | CFile::shareDenyRead))
    {
        m_pProjMan->OutputErrorMessage(E_CEPROJMAN_NO_PROJ_FILE, m_strFileName);
        return E_CEPROJMAN_NO_PROJ_FILE;
    }

    
    try 
    {
        CArchive projArch(&projFile, CArchive::load);
    
        // clean up old contents
        POSITION pos = m_sectionList.GetHeadPosition();
        while(pos)
        {
            CSection* pSection;
            pSection = m_sectionList.GetNext(pos);
            delete pSection;
        }
        m_sectionList.RemoveAll();

        m_firstSection.CleanUp();

        CSection* pNewSection = NULL;
        int lineNr = 0;

        CString strLine;
        CString strLineNum;
        while (projArch.ReadString(strLine))
        {
            lineNr++;
            strLine.TrimLeft();
            strLine.TrimRight();

            if (strLine.IsEmpty()) // a empty line
            {
                CKeyAndValue* pKeyAndValue = NULL;
                pKeyAndValue = new CKeyAndValue();
                if (!pKeyAndValue)
                {
                    continue;
                }
                pKeyAndValue->SetLineType(INI_TYPE_EMPTY);
                if (pNewSection)
                {
                    pNewSection->AddKeyAndValue(pKeyAndValue, TRUE);
                }
                else
                {
                    m_firstSection.AddKeyAndValue(pKeyAndValue, TRUE);
                }
                continue;
            }

            if (strLine[0]==';') // a comment line
            {
                CKeyAndValue* pKeyAndValue = NULL;
                pKeyAndValue = new CKeyAndValue();
                if (!pKeyAndValue)
                {
                    continue;
                }
                pKeyAndValue->SetLineType(INI_TYPE_COMMENT);
                pKeyAndValue->SetValue(strLine);
                if (pNewSection)
                {
                    pNewSection->AddKeyAndValue(pKeyAndValue, TRUE);
                }
                else
                {
                    m_firstSection.AddKeyAndValue(pKeyAndValue, TRUE);
                }
                continue;
            }

            if (strLine[0]=='[') // a new section
            {
                pNewSection = new CSection(m_pProjMan);
                m_sectionList.AddTail(pNewSection);
                PROJ_MAN_ASSERT(pNewSection);
                int iPosition = strLine.Find("]");
                if (iPosition==0)
                {
                    strLineNum.Format("%i", lineNr);
                    m_pProjMan->OutputErrorMessage(E_CEPROJMAN_PROJ_FILE_SYNTAX_ERROR, 
                                                   m_strFileName,
                                                   strLineNum);
                    continue;
                }
                pNewSection->SetSectionName(strLine.Mid(1,iPosition-1));
            }
            else    // should be a key value line
            {
                CString strKey;
                CString strValue;
            
                int iPosition = strLine.Find('=');
                if (iPosition<0)
                {
                    strLineNum.Format("%i", lineNr);
                    m_pProjMan->OutputErrorMessage(E_CEPROJMAN_PROJ_FILE_SYNTAX_ERROR, 
                                                   m_strFileName,
                                                   strLineNum);
                    continue;
                }
                strKey = strLine.Left(iPosition);
                strValue = strLine.Right(strLine.GetLength()-iPosition-1);
                strKey.TrimLeft();
                strKey.TrimRight();
                strValue.TrimLeft();
                strValue.TrimRight();
                if (!pNewSection)
                {
                    strLineNum.Format("%i", lineNr);
                    m_pProjMan->OutputErrorMessage(E_CEPROJMAN_PROJ_FILE_SYNTAX_ERROR, 
                                                   m_strFileName,
                                                   strLineNum);
                    continue;
                }
                pNewSection->AddKeyAndValue(strKey, strValue, TRUE);
            }

        }

        m_timeLastParse = modTime;

        projArch.Close();
        projFile.Close();

        //UpdateFileViewInfo();
        m_bUpdateFVInfo = TRUE;
        
        return hr;    
    }
    catch (CException e)
    {
        projFile.Close();
        m_pProjMan->OutputErrorMessage(E_CEPROJMAN_NO_PROJ_FILE, m_strFileName);
        return E_CEPROJMAN_NO_PROJ_FILE;  
    }
}


//------------------------------------------------------------------*
/**
 * WriteFile        - Writes the values stored in m_listSectionList in
                     the file, the name given as member
 *
 * @param           - The name of the file to open for writing.
 * @return          - TRUE if the operation was successfull, else FALSE.
 * @exception       -
 * @see             
*/
HRESULT CProjectFile::WriteProjectFile()
{
    return WriteProjectFile(m_strFileName);
}



//------------------------------------------------------------------*
/**
 * WriteFile        - Writes the values stored in m_listSectionList in
                    the file with the name given as a param (strFileName)
 *
 * @param           - The name of the file to open for writing.
 * @return          - S_OK if the operation was successfull, 
 *                    E_CEPROJMAN_NO_PROJ_FILE
 * @exception       -
 * @see             
*/
HRESULT CProjectFile::WriteProjectFile(const CString strFileName)
{
    if (strFileName.IsEmpty())
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;
    CFile projFile;

    if (m_pProjMan->GetClassTree()!=NULL)
    {
        m_pProjMan->GetClassTree()->AddSourceFileToReparse(strFileName);
    }
    if (m_pProjMan->GetTargetTree()!=NULL)
    {
        m_pProjMan->GetTargetTree()->AddSourceFileToReparse(strFileName);
    }

    if (!projFile.Open(m_strFileName, CFile::modeCreate|CFile::modeWrite))
    {
        m_pProjMan->OutputErrorMessage(E_CEPROJMAN_NO_PROJ_FILE, m_strFileName);
        ReparseProjectFile(TRUE);
        return E_CEPROJMAN_NO_PROJ_FILE;
    }

    
    try 
    {
        CArchive projArch(&projFile, CArchive::store);

        // first section
        CKeyAndValue* pKav = NULL;
        BOOL bGotOne;
        bGotOne = m_firstSection.GetFirstKeyAndValue(&pKav, TRUE);
        while(bGotOne)
        {
            WriteKeyValueLine(pKav, projArch);
            bGotOne = m_firstSection.GetNextKeyAndValue(&pKav, TRUE);
        }

    
        POSITION secPos = m_sectionList.GetHeadPosition();
        while(secPos)
        {
            CSection* pSec = m_sectionList.GetNext(secPos);
            CString strSecLine;
            strSecLine = _T("[") + pSec->GetSectionName() + _T("]\r\n");
            projArch.WriteString(strSecLine);

            pKav = NULL;
            bGotOne = pSec->GetFirstKeyAndValue(&pKav, TRUE);
            while(bGotOne)
            {
                WriteKeyValueLine(pKav, projArch);
                bGotOne = pSec->GetNextKeyAndValue(&pKav, TRUE);
            }
            //CString strNewLine = _T("\r\n");
            //projArch.WriteString(strNewLine);
        }

        projArch.Flush();
        projArch.Close();
        projFile.Close();
        ReparseProjectFile(TRUE);
        m_pProjMan->Fire_ProjectFileChanged();
        return hr;    
    }
    catch (CException e)
    {
        projFile.Close();
        m_pProjMan->OutputErrorMessage(E_CEPROJMAN_NO_PROJ_FILE, m_strFileName);
        ReparseProjectFile(TRUE);
        return E_CEPROJMAN_NO_PROJ_FILE;  
    }
}


void CProjectFile::WriteKeyValueLine(const CKeyAndValue* pKeyAndValue, CArchive& arch)
{
    if (pKeyAndValue->GetLineType() == INI_TYPE_KEY_VALUE)
    {
        CString strKVLine;
        strKVLine = pKeyAndValue->GetKey() + _T("=") + pKeyAndValue->GetValue() +_T("\r\n");
        arch.WriteString(strKVLine);
    }
    else if (pKeyAndValue->GetLineType() == INI_TYPE_COMMENT)
    {
        CString strKVLine;
        strKVLine = pKeyAndValue->GetValue() + _T("\r\n");
        arch.WriteString(strKVLine);
    }
    else if (pKeyAndValue->GetLineType() == INI_TYPE_EMPTY)
    {
        CString strKVLine;
        strKVLine = _T("\r\n");
        arch.WriteString(strKVLine);
    }
}


//------------------------------------------------------------------*
/**
 * GetFirstSection  - Looks for the first section in m_listSectionList.
 *
 * @param           
 * @return          - A pointer on the first section in 
 *                    m_listSectionList. NULL If the list is empty.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::GetFirstSection(CSection** ppSection)
{
    m_iterPos = m_sectionList.GetHeadPosition();
    if (m_iterPos==NULL)
    {
        return FALSE;
    }

    *ppSection = m_sectionList.GetNext(m_iterPos);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetNextSection   - Starting from a given position, this function 
                    looks for the next section in m_listSectionList.
 *
 * @param           - A pointer on the section to start with.
 * @return          - Returns a pointer on the next section in the list.
                    a NULL if there is none.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::GetNextSection(CSection** ppSection)
{
    if (m_iterPos==NULL)
    {
        return FALSE;
    }

    *ppSection = m_sectionList.GetNext(m_iterPos);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * FindSection      - Looks for a section in the list. The name of the
                    section to look for is given in strSectionName.
 *
 * @param           - The name of the section to look for.
 * @return          - A pointer on the section with the given name.
                    NULL if there is none.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::FindSection(const CString strSectionName, CSection** ppSection)
{
    *ppSection = NULL;
    POSITION pos = m_sectionList.GetHeadPosition();
    while(pos)
    {
        CSection* pSec = m_sectionList.GetNext(pos);
        if (pSec->GetSectionName().CompareNoCase(strSectionName)==0)
        {
            *ppSection = pSec;
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * AddSection       - Adds a section at the end of the list.
 *
 * @param           - Name of the section to add.
 * @return          - TRUE if the operation was successfull, else FALSE.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::AddSection(const CString strSectionName)
{
    CSection* pSection = new CSection(m_pProjMan, strSectionName);
    return AddSection(pSection);
}


//------------------------------------------------------------------*
/**
 * AddSection       - Adds a section at the end of the list.
 *
 * @param           - A pointer on the section to add to the list.
 * @return          - TRUE if the operation was successfull, else FALSE.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::AddSection(CSection* pSection)
{
    m_sectionList.AddTail(pSection);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * InsertSectionAfter   - Inserts a section in the list after the 
                        section pointed to by pAfterSection. The name
                        of the section to insert is strInsertSection 
 *
 * @param               - Name of the section to be inserted.
                        - Pointer on the section after which the new 
                        section will be inserted.
 * @return              - TRUE if the operation was succesfull, else FALSE.
 * @exception           -
 * @see             
*/
BOOL CProjectFile::InsertSectionAfter(const CString strInsertSectionName, CSection* pAfterSection)
{
    CSection* pSection = new CSection(m_pProjMan, strInsertSectionName);
    BOOL ret = InsertSectionAfter(pSection, pAfterSection);
    if (!ret)
    {
        delete pSection;
    }
    return ret;
}

//------------------------------------------------------------------*
/**
 * InsertSectionAfter   - Inserts a section in the list after the 
                        section pointed to by pAfterSection. The 
                        section to be inserted is pointed to by 
                        pInsertSection.
 *
 * @param               - Pointer on the section to be inserted.
                        - Pointer on the section after wich the new 
                        section will be inserted.
 * @return              - TRUE if the operation was succesfull, else FALSE.
 * @exception           -
 * @see             
*/
BOOL CProjectFile::InsertSectionAfter(CSection* pInsertSection, CSection* pAfterSection)
{
    if (pAfterSection==NULL)
    {
        m_sectionList.AddHead(pInsertSection);
        return TRUE;
    }

    POSITION pos;
    pos = m_sectionList.GetHeadPosition();
    while (pos)
    {
        POSITION thisPos = pos;
        CSection* pSec = m_sectionList.GetNext(pos);
        if (pSec->GetSectionName().CompareNoCase(pAfterSection->GetSectionName())==0)
        {
            m_sectionList.InsertAfter(thisPos, pInsertSection);
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * DeleteSection    - Delete the section with the name strSectionName
                    from the list.
 *
 * @param           - Name of the section to delete.
 * @return          - TRUE if the operation was succesfull, else FALSE.
 * @exception       -
 * @see             
*/
BOOL CProjectFile::DeleteSection(CString strSectionName)
{
    POSITION pos = m_sectionList.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CSection* pSec = m_sectionList.GetNext(pos);
        if (pSec->GetSectionName().CompareNoCase(strSectionName)==0)
        {
            m_sectionList.RemoveAt(thisPos);
            delete pSec;
            return TRUE;
        }
    }
    return FALSE;
}




void CProjectFile::UpdateFileViewInfo()
{
    if (m_bUpdateFVInfo)
    {
        UpdateFileViewInfo(_T("USER_HELP"), FV_FILE_HELP, m_mapHelpFiles);
        UpdateFileViewInfo(_T("ADD_FILES"), FV_FILE_ADDITIONAL, m_mapAddFiles);
        m_bUpdateFVInfo = FALSE;
    }
}


void CProjectFile::UpdateFileViewInfo(const CString& strSection, E_FV_FileCategory catFile, 
                                      CMap<CString, LPCTSTR, CString, CString&>& map)
{
    CList<CString, CString&> listCPFiles;
    CList<CString, CString&> listAddedFiles;
    CList<CString, CString&> listRemovedFiles;

    CSection* pSection = NULL;
    if (FindSection(strSection, &pSection))
    {
        CKeyAndValue* pKeyAndValue = NULL;
        BOOL bMore = pSection->GetFirstKeyAndValue(&pKeyAndValue);
        while(bMore)
        {
            if ((pKeyAndValue->GetKey()).CompareNoCase(_T("file"))==0)
            {
                listCPFiles.AddTail(pKeyAndValue->GetValue());
            }
            bMore = pSection->GetNextKeyAndValue(&pKeyAndValue);
        }
    }

    POSITION pos;

    // fill list new contents now
    // first test files that have already been in the map, no changes here
    // or not found in the map -> added files
    // remove files from map --> in map rest removed files only
    pos = listCPFiles.GetHeadPosition();
    while(pos)
    {
        CString strOrigKey;
        CString strKey;
        CString strValue;

        strOrigKey = listCPFiles.GetNext(pos);
        
        strKey = strOrigKey;
        strKey.MakeLower();
        if (!map.Lookup(strKey, strValue))
        {
            listAddedFiles.AddTail(strOrigKey);
        }
        else
        {
            map.RemoveKey(strKey);
        }
    }

    pos = map.GetStartPosition();
    while(pos)
    {
        CString strKey;
        CString strValue;
        map.GetNextAssoc(pos, strKey, strValue);
        listRemovedFiles.AddTail(strValue);
    }

    map.RemoveAll();

    pos = listCPFiles.GetHeadPosition();
    while(pos)
    {
        CString strKey;
        CString strValue;

        strValue = listCPFiles.GetNext(pos);

        strKey = strValue;
        strKey.MakeLower();
        map.SetAt(strKey, strValue);
    }

    // send messages to file view
    CString strFile;
    pos = listRemovedFiles.GetHeadPosition();
    while(pos)
    {
        strFile = listRemovedFiles.GetNext(pos);
        CComBSTR sFile(strFile);
        m_pProjMan->Fire_OnFileRemoved(sFile, catFile);
    }
    pos = listAddedFiles.GetHeadPosition();
    while(pos)
    {
        strFile = listAddedFiles.GetNext(pos);
        CComBSTR sFile(strFile);
        m_pProjMan->Fire_OnFileAdded(sFile, catFile);
    }
}









//------------------------------------------------------------------*
//Methods for CSection
//





//------------------------------------------------------------------*
/**
 * CSection         - Constructor of CSection.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CSection::CSection(CCEProjectManager* pProjMan)
{
    PROJ_MAN_ASSERT(pProjMan);
    m_pProjMan = pProjMan;
    m_iterPos = NULL;
}


//------------------------------------------------------------------*
/**
 * CSection         - Constructor of CSection
 *
 * @param           - Name of the section.
 * @return          
 * @exception       -
 * @see             
*/
CSection::CSection(CCEProjectManager* pProjMan,
                   CString strSectionName)
{
    PROJ_MAN_ASSERT(pProjMan);
    m_pProjMan = pProjMan;
    m_strSectionName = strSectionName;
    m_iterPos = NULL;
}


//------------------------------------------------------------------*
/**
 * ~CSection        - Destructor of CSection.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CSection::~CSection()
{
    CleanUp();
}


void CSection::CleanUp()
{
    POSITION pos = m_keyAndValueList.GetHeadPosition();
    while(pos)
    {
        CKeyAndValue* pKeyAndValue;
        pKeyAndValue = m_keyAndValueList.GetNext(pos);
        delete pKeyAndValue;
    }
    m_keyAndValueList.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * GetSectionName   - Returns the name of the actual section.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CSection::GetSectionName()
{
    return m_strSectionName;
}


//------------------------------------------------------------------*
/**
 * SetSectionName   - Writes the name of the actual section
 *
 * @param           - Name to be written.
 * @return          
 * @exception       -
 * @see             
*/
void CSection::SetSectionName(const CString strSectionName)
{
    m_strSectionName = strSectionName;
}


//------------------------------------------------------------------*
/**
 * GetFirstKey      - Looks for the first key and value in 
                    m_listKeyAndValueList.  
 *
 * @param           
 * @return          - Pointer on the first key and value. NULL if the
                    list is empty.
 * @exception       -
 * @see             
*/
BOOL CSection::GetFirstKeyAndValue(CKeyAndValue** ppKeyAndValue, BOOL bAll)
{
    m_iterPos = m_keyAndValueList.GetHeadPosition();
    if (m_iterPos==NULL)
    {
        return FALSE;
    }
            
    return GetNextKeyAndValue(ppKeyAndValue, bAll);
}


//------------------------------------------------------------------*
/**
 * GetNextKey       - Starting from a given position, this function 
                    looks for the next key and value in m_listKeyAndValueList.
 *
 * @param           - A pointer on the key and value to start with.
 * @return          - Returns a pointer on the next key and value in 
                    the list. NULL if there is none.
 * @exception       -
 * @see             
*/
BOOL CSection::GetNextKeyAndValue(CKeyAndValue** ppKeyAndValue, BOOL bAll)
{
    *ppKeyAndValue = NULL;

    if (m_iterPos==NULL)
    {
        return FALSE;
    }

    *ppKeyAndValue = m_keyAndValueList.GetNext(m_iterPos);

    
    if ((*ppKeyAndValue)->GetLineType() == INI_TYPE_KEY_VALUE || bAll==TRUE)
    {
        return TRUE;
    }
    else
    {
        return GetNextKeyAndValue(ppKeyAndValue);
    }
}


//------------------------------------------------------------------*
/**
 * FindKeyAndValue  - Looks for key (strKey) and value (strValue) in 
 *                    m_listKeyAndValueList, the search is ignore case
 *                    for key and value.
 *
 * @param           - Key to look for.
                    - Associated value
 * @return          - Pointer on the key and value found in the list.
 * @exception       -
 * @see             
*/
BOOL CSection::FindKeyAndValue(const CString strKey, const CString strValue,
                               CKeyAndValue** ppKeyAndValue)
{
    *ppKeyAndValue = NULL;
    POSITION pos = m_keyAndValueList.GetHeadPosition();
    while (pos)
    {
        CKeyAndValue* pKav = m_keyAndValueList.GetNext(pos);
        if (pKav->GetLineType() == INI_TYPE_KEY_VALUE)
        {
            if ((pKav->GetKey().CompareNoCase(strKey)==0) && 
                (pKav->GetValue().CompareNoCase(strValue)==0))
            {
                *ppKeyAndValue = pKav;
                return TRUE;
            }
        }
    }
    return FALSE;
}






//------------------------------------------------------------------*
/**
 * AddKeyAndValue   - Adds a key and value at the end of the list. The
                    new element is pointed to by pKeyAndValue.
 *
 * @param           - Pointer on the new element to be added.
 * @return          - TRUE if the operation was succesfull, else FALSE.
 * @exception       -
 * @see             
*/
BOOL CSection::AddKeyAndValue(CKeyAndValue* pKeyAndValue, BOOL bTail)
{
    // find last non key and value line in section, 
    // before all comment and empty lines at the end of the section
    POSITION posInsert = NULL;

    if (pKeyAndValue->GetLineType() != INI_TYPE_KEY_VALUE || bTail)
    {
        m_keyAndValueList.AddTail(pKeyAndValue);
        return TRUE;
    }

    POSITION pos = m_keyAndValueList.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CKeyAndValue* pKV = m_keyAndValueList.GetNext(pos);
        if (pKV)
        {
            if(pKV->GetLineType() == INI_TYPE_KEY_VALUE)
            {
                posInsert = thisPos;
            }
        }
    }

    if (posInsert==NULL)
    {
        m_keyAndValueList.AddHead(pKeyAndValue);
    }
    else
    {
        m_keyAndValueList.InsertAfter(posInsert, pKeyAndValue);
    }
    
    //m_keyAndValueList.AddTail(pKeyAndValue);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * AddKeyAndValue   - Adds a key and value at the end of the list.
 *
 * @param           - key (strKey) and value (strValue) to be added.
 * @return          - TRUE if the operation was succesfull, else FALSE.
 * @exception       -
 * @see             
*/
BOOL CSection::AddKeyAndValue(const CString strKey, const CString strValue, BOOL bTail)
{
    CKeyAndValue* pKeyAndValue = new CKeyAndValue(strKey, strValue);
    return AddKeyAndValue(pKeyAndValue, bTail); 
}


//------------------------------------------------------------------*
/**
 * InsertKeyAndValueAfter   - Inserts a key (strKey) and value (strValue)
                            in the list after the key and value pointed
                            to by pAfterKeyAndValue.
 *
 * @param               - Key and value to be inserted.
                        - Pointer on the key and value after which the new 
                        element will be inserted.
 * @return              - TRUE if the operation was succesfull, else FALSE.
 *
 * @exception       -
 * @see             
*/
BOOL CSection::InsertKeyAndValueAfter(const CString strInsertKey, const CString strInsertValue, 
                                      CKeyAndValue* pAfterKeyAndValue)
{
    CKeyAndValue* pKeyAndValue = new CKeyAndValue(strInsertKey, strInsertValue);
    BOOL ret = InsertKeyAndValueAfter(pKeyAndValue, pAfterKeyAndValue);
    if (!ret)
    {
        delete pKeyAndValue;
    }
    return ret;
}

//------------------------------------------------------------------*
/**
 * InsertKeyAndValueAfter   - Inserts a key and value pointed to by
                            pInsertKeyAndValue in the list after the 
                            key and value pointed to by pAfterKeyAndValue.
 *
 * @param                   - Key and value to be inserted.
                            - Pointer on the key and value after which 
                            the new element will be inserted.
 * @return                  - TRUE if the operation was succesfull, else FALSE.
 *
 * @exception       -
 * @see             
*/
BOOL CSection::InsertKeyAndValueAfter(CKeyAndValue* pInsertKeyAndValue, CKeyAndValue* pAfterKeyAndValue)
{
    if (pAfterKeyAndValue==NULL)
    {
        m_keyAndValueList.AddHead(pInsertKeyAndValue);
        return TRUE;
    }

    POSITION pos;
    pos = m_keyAndValueList.GetHeadPosition();
    while (pos)
    {
        POSITION thisPos = pos;
        CKeyAndValue* pKav = m_keyAndValueList.GetNext(pos);
        if (  (pKav->GetKey().CompareNoCase(pAfterKeyAndValue->GetKey())==0)
            &&(pKav->GetValue().CompareNoCase(pAfterKeyAndValue->GetValue())==0))
        {
            m_keyAndValueList.InsertAfter(thisPos, pInsertKeyAndValue);
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * DeleteKeyAndValue    - Delete the key (strKey) and value (strValue)
 *                        from the list.
 *
 * @param               - Key and value to delete.
 * @return              - TRUE if the operation was succesfull, else FALSE
 *
 * @exception       -
 * @see             
*/
BOOL CSection::DeleteKeyAndValue(const CString strKey, const CString strValue)
{
    POSITION pos = m_keyAndValueList.GetHeadPosition();
    while(pos)
    {
        POSITION thisPos = pos;
        CKeyAndValue* pKav = m_keyAndValueList.GetNext(pos);
        if (  (pKav->GetKey().CompareNoCase(strKey)==0)
            &&(pKav->GetValue().CompareNoCase(strValue)==0))
        {
            m_keyAndValueList.RemoveAt(thisPos);
            delete pKav;
            return TRUE;
        }
    }
    return FALSE;
}
