

// Xref.cpp: implementation of the CXref class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Xref.h"
#include "resource.h"   // OSAI feature print XRef 24.03.04 SIS >>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// OSAI feature print XRef 24.03.04 SIS >>
#define COL_WIDTH_VARNAME   (35)    // column width name (variable)
#define COL_WIDTH_TYPE      (12)    // column width type (variable)
#define COL_WIDTH_LOCATION  (30)    // column width location (variable)
#define COL_WIDTH_ACCESS    (3)     // column width access (fix)
#define VAR_LINE_LENGTH (COL_WIDTH_VARNAME + COL_WIDTH_TYPE + COL_WIDTH_LOCATION)   // variable line width
#define LINE_LENGTH (VAR_LINE_LENGTH + COL_WIDTH_ACCESS)                            // total line width
// OSAI feature print XRef 24.03.04 SIS <<

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CXref,CObject, 2)


CXref::CXref()
{

}

CXref::~CXref()
{
    Clear();

}
void CXref::Serialize( CArchive& archive )
{
    CObject::Serialize(archive);
    m_NameDictionary.Serialize(archive);
    m_TypeDictionary.Serialize(archive);
    m_ScopeDictionary.Serialize(archive);
    m_FileScopeDictionary.Serialize(archive);
    m_Files.Serialize(archive);
}

void CXref::Dump( CDumpContext& dc ) const
{
    m_NameDictionary.Dump(dc);
    m_TypeDictionary.Dump(dc);
    m_ScopeDictionary.Dump(dc);
    m_FileScopeDictionary.Dump(dc);
    m_Files.Dump(dc);
}

CSortedStringMap *  CXref::GetNames(void)
{
    return &m_NameDictionary;
}

CSortedStringMap *  CXref::GetTypes(void)
{
    return &m_TypeDictionary;
}
CSortedStringMap *  CXref::GetScopes(void)
{
    return &m_ScopeDictionary;
}
CSortedStringMap *  CXref::GetFiles(void)
{
    return &m_FileScopeDictionary;
}

BOOL CXref::GetFileScopeByLocation(CString sLocation, CString &sFilescope)
{
    StringId idFilescope;
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * Item = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        if (Item->IsKindOf(RUNTIME_CLASS(CXrefFileScope)))
        {
            if ((idFilescope = Item->MatchLocation(sLocation)) != STRING_NO_FOUND)
            {
                sFilescope = m_FileScopeDictionary.GetItem(idFilescope);
                break;
            }
        }
    }
    return !sFilescope.IsEmpty();

}

BOOL CXref::FindDefinitions(CStringArray &DefinitionList,const CString &sName, const CString &sScope, const CString &sType, const CString &sFileName)
{
    CXrefFind MyFind(this);
    MyFind.Build(sName,sScope,sType,sFileName);
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * Item = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        if (Item->IsKindOf(RUNTIME_CLASS(CXrefFileScope)))
        {
            Item->FindDefinitions(DefinitionList,MyFind);
        }
    }
    return TRUE;
}


// OSAI-Feature 22.03.04 SIS >>
BOOL CXref::FindDefs(CSortedStringMap& rastrDefs, const CString &sName, const CString &sScope, const CString &sType, const CString &sFileName)
{
    CXrefFind MyFind(this);
    MyFind.Build(sName,sScope,sType,sFileName);
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * pFileScope = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        pFileScope->FindDefs(rastrDefs, MyFind);
    }
    return TRUE;
}
// OSAI-Feature 22.03.04 SIS <<

CXrefDefinition * CXref::FindDefinition(CXrefFind &FindScope)
{
    CXrefDefinition *pSingleDefinition = NULL;
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * Item = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        if (Item->IsKindOf(RUNTIME_CLASS(CXrefFileScope)))
        {
            CXrefDefinition *pDefinition;
            pDefinition = Item->FindDefinition(FindScope);
            if (pDefinition)
            {
                pSingleDefinition = pDefinition;
                break;
            }
        }
    }
    return pSingleDefinition;

}


BOOL CXref::FindReferences(CStringArray &DeclarationList,const CString &sName, BOOL bWrite, BOOL bRead)
{
    CString sNamePart;
    CString sFilePart;
    CString sScope(szAll);
    CString sType(szAll);
    int iIndexTab = sName.Find('\t');
    CXrefDefinition * pXrefDefinition;

    sNamePart = sName.Left(iIndexTab);
    sFilePart = sName.Mid(iIndexTab+1);
    CXrefFind MyFind(this);
    MyFind.Build(sNamePart,sScope,sType,sFilePart);
    MyFind.m_bRead = bRead;
    MyFind.m_bWrite = bWrite;

    pXrefDefinition = FindDefinition(MyFind);
    if (pXrefDefinition)
    {
        if (pXrefDefinition->IsGlobal())
        {
            // alter search item
            MyFind.m_IsGlobalDefiniton = TRUE;
        }
    }
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * Item = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        if (Item->IsKindOf(RUNTIME_CLASS(CXrefFileScope)))
        {
            Item->FindReferences(DeclarationList,MyFind);
        }
    }
    return TRUE;

}


// OSAI-Feature 22.03.04 SIS >>
// XREFPRINT 20.07.05 SIS >>
// changed order of parameters bWrite, bRead -> bRead, bWrite
// only to be consistent with other methods
BOOL CXref::FindRefs(CSortedStringMap& astrRefs, const CString& crstrName, const CString& crstrFile, BOOL bRead, BOOL bWrite)
// XREFPRINT 20.07.05 SIS <<
{
    CString             sScope(szAll);
    CString             sType(szAll);
    CXrefDefinition*    pXrefDefinition;
    CXrefFind           MyFind(this);

    MyFind.Build(crstrName, sScope, sType, crstrFile);
    MyFind.m_bRead = bRead;
    MyFind.m_bWrite = bWrite;

    pXrefDefinition = FindDefinition(MyFind);
    if (pXrefDefinition)
    {
        if (pXrefDefinition->IsGlobal())
        {
            // alter search item
            MyFind.m_IsGlobalDefiniton = TRUE;
        }
    }
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
    {
        CXrefFileScope * Item = dynamic_cast<CXrefFileScope*> (m_Files.GetNext(pos));
        if (Item->IsKindOf(RUNTIME_CLASS(CXrefFileScope)))
        {
            Item->FindRefs(astrRefs, MyFind);
        }
    }
    return TRUE;
}
// OSAI-Feature 22.03.04 SIS <<


void CXref::Clear(void)
{
    POSITION pos = m_Files.GetHeadPosition();
    while (pos != NULL)
        delete m_Files.GetNext(pos);

}

BOOL CXref::PerformLink(CString sPathName)
{
    CString sLinkList;
    CMapStringToOb  FileHashTable;
    CXrefFileScope *pFileScope;
    CString sFileFind(sPathName);
    sFileFind += "\\*.4xl";
    CFileFind   FileFinder;
    BOOL bWorking = FileFinder.FindFile(sFileFind);
    while (bWorking)
    {
        bWorking= FileFinder.FindNextFile();
        sLinkList = FileFinder.GetFilePath();
        ifstream    LinkList(sLinkList);
        if (LinkList.fail())
            return FALSE;
        while (!LinkList.eof())
        {
            CString     sFilePath;
            CStringSep sFileName;
            LinkList >> sFileName >> ws;
            if (sFileName.IsEmpty())
                continue;
            else
            {
                if (!FileHashTable.Lookup(sFileName,( CObject*& ) pFileScope))
                {
                    sFilePath = sPathName;
                    sFilePath += "\\";
                    sFilePath += sFileName;
                    ifstream    SingleFile(sFilePath);
                    CXrefFileScope *CurrFileScope = new CXrefFileScope;
                    CStringSep sBuffer;
                    while ( SingleFile >> sBuffer >> ws && sBuffer.CompareNoCase(szInfoSection))
                        ;
                    CurrFileScope->ReadFromFile(SingleFile,*this);
                    m_Files.AddTail((CObject*)CurrFileScope);
                    FileHashTable.SetAt(sFileName,CurrFileScope);
                }
            }
        }
    }
    
    return TRUE;
}
BOOL CXref::LoadFromFile(CString &sDirectory)
{
    // todo : exception handling
    sDirectory += "\\XREF.4LNK";
    CFile theFile;
    if ( !theFile.Open(sDirectory, CFile::modeRead ))
        return FALSE;
    CArchive archive(&theFile, CArchive::load);
    Serialize(archive);
    archive.Close();
    theFile.Close();
    return TRUE;
}

BOOL CXref::SaveToFile(CString &sDirectory)
{
    // todo : exception handling
    sDirectory += "\\XREF.4LNK";
    CFile theFile;
    if ( !theFile.Open(sDirectory, CFile::modeWrite | CFile::modeCreate ))
        return FALSE;
    CArchive archive(&theFile, CArchive::store);
    Serialize(archive);
    archive.Close();
    theFile.Close();
    return TRUE;
}


// XREFPRINT 21.07.05 SIS >>
void CXref::GetSelectionString
(
	CString& rstrContent, 
	int iLineLength, 
	const CString& crstrSelName,
	const CString& crstrSelScope, 
	const CString& crstrSelType, 
	const CString& crstrSelFile, 
	BOOL bRead,
	BOOL bWrite
)
// XREFPRINT 21.07.05 SIS <<
{
    CString strTmp;
    CSortedStringMap    astrDefs;       // array of all definition strings
    CString             strVar;         // current variable name
    CString             strFile;        // file scope
    CString             strType;        // current type string
    CString             strLocation;    // location (file)
    CString             strAccess;      // access

    int iCWVarName  = COL_WIDTH_VARNAME;
    int iCWType     = COL_WIDTH_TYPE;
    int iCWLocation = COL_WIDTH_LOCATION;
    int iCWAccess   = COL_WIDTH_ACCESS;
    if(iLineLength > 0 && iLineLength != LINE_LENGTH)
    {
        iLineLength -= COL_WIDTH_ACCESS;        // temporarily subtract fix column width of access column
        iCWVarName  = MulDiv(iCWVarName, iLineLength, VAR_LINE_LENGTH);
        iCWType     = MulDiv(iCWType, iLineLength, VAR_LINE_LENGTH);
        iCWLocation = MulDiv(iCWLocation, iLineLength, VAR_LINE_LENGTH);
        iLineLength += COL_WIDTH_ACCESS;        // add fix column width of access column
    }

	if(bRead)
	{
		strAccess += _T("R");
	}
	if(bWrite)
	{
		strAccess += _T("W");
	}
	if(strAccess.IsEmpty())
	{
		strAccess = _T("-");
	}
    strTmp.Format(_T("Variable: \"%s\" Storage Class: \"%s\" Type: \"%s\" File Scope: \"%s\" Read/Write: \"%s\"\n"), crstrSelName, crstrSelScope, crstrSelType, crstrSelFile, strAccess);
    rstrContent = strTmp;
    strTmp = GetColumnString(_T(""), iLineLength, FALSE, _T('=')) + _T("\n");
    rstrContent += strTmp;

    strVar.LoadString(IDS_COL_VARIABLE);
    strType.LoadString(IDS_COL_TYPE);
    strLocation.LoadString(IDS_COL_REFERENCES);
    strAccess.LoadString(IDS_COL_ACCESS);

    strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(strVar, iCWVarName), GetColumnString(strType, iCWType), GetColumnString(strLocation, iCWLocation - 3, TRUE), GetColumnString(strAccess, iCWAccess + 3));
    rstrContent += strTmp;
    strTmp = GetColumnString(_T(""), iLineLength, FALSE, _T('=')) + _T("\n");
    rstrContent += strTmp;

    rstrContent.GetBuffer(10000);
    rstrContent.ReleaseBuffer();

	// XREFPRINT 21.07.05 SIS >>
    FindDefs(astrDefs, crstrSelName, crstrSelScope, crstrSelType, crstrSelFile);
	// XREFPRINT 21.07.05 SIS <<

    int     iDefCount = astrDefs.GetCount();
    for(int iDef = 0; iDef < iDefCount; ++iDef)
    {
        strTmp = astrDefs.GetSortedItem(iDef);
        strTmp = SplitField(strTmp, strVar);
        strTmp = SplitField(strTmp, strFile);
        strTmp = SplitField(strTmp, strType);
        strTmp = SplitField(strTmp, strLocation);
        strAccess = strTmp;
        strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(strVar, iCWVarName), GetColumnString(strType,iCWType), GetColumnString(strLocation, iCWLocation, TRUE), GetColumnString(strAccess, iCWAccess));
        rstrContent += strTmp;

        CSortedStringMap    astrRefs;
        FindRefs(astrRefs, strVar, strFile, bRead, bWrite);
        int iRefCount = astrRefs.GetCount();
        for(int iRef = 0; iRef < iRefCount; ++iRef)
        {
            strTmp = astrRefs.GetItem(iRef);
            strTmp = SplitField(strTmp, strLocation);
            strTmp = SplitField(strTmp, strAccess);
            strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(_T(""), iCWVarName), GetColumnString(_T(""),iCWType), GetColumnString(strLocation, iCWLocation, TRUE), GetColumnString(strAccess, iCWAccess));
            rstrContent += strTmp;
        }
    }
}


// OSAI feature print XRef 24.03.04 SIS >>
void CXref::GetContentString(CString& rstrContent, int iLineLength)
{
    CString strTmp;
    CSortedStringMap    astrDefs;       // array of all definition strings
    CString             strVar;         // current variable name
    CString             strFile;        // file scope
    CString             strType;        // current type string
    CString             strLocation;    // location (file)
    CString             strAccess;      // access

    int iCWVarName  = COL_WIDTH_VARNAME;
    int iCWType     = COL_WIDTH_TYPE;
    int iCWLocation = COL_WIDTH_LOCATION;
    int iCWAccess   = COL_WIDTH_ACCESS;
    if(iLineLength > 0 && iLineLength != LINE_LENGTH)
    {
        iLineLength -= COL_WIDTH_ACCESS;        // temporarily subtract fix column width of access column
        iCWVarName  = MulDiv(iCWVarName, iLineLength, VAR_LINE_LENGTH);
        iCWType     = MulDiv(iCWType, iLineLength, VAR_LINE_LENGTH);
        iCWLocation = MulDiv(iCWLocation, iLineLength, VAR_LINE_LENGTH);
        iLineLength += COL_WIDTH_ACCESS;        // add fix column width of access column
    }

    strVar.LoadString(IDS_COL_VARIABLE);
    strType.LoadString(IDS_COL_TYPE);
    strLocation.LoadString(IDS_COL_REFERENCES);
    strAccess.LoadString(IDS_COL_ACCESS);

    strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(strVar, iCWVarName), GetColumnString(strType, iCWType), GetColumnString(strLocation, iCWLocation - 3, TRUE), GetColumnString(strAccess, iCWAccess + 3));
    rstrContent = strTmp;
    strTmp = GetColumnString(_T(""), iLineLength, FALSE, _T('=')) + _T("\n");
    rstrContent += strTmp;

    rstrContent.GetBuffer(10000);
    rstrContent.ReleaseBuffer();

    //                        Name   Scope  Type   Access
    FindDefs(astrDefs, szAny, szAll, szAll, szAll);


    int     iDefCount = astrDefs.GetCount();
    for(int iDef = 0; iDef < iDefCount; ++iDef)
    {
        strTmp = astrDefs.GetSortedItem(iDef);
        strTmp = SplitField(strTmp, strVar);
        strTmp = SplitField(strTmp, strFile);
        strTmp = SplitField(strTmp, strType);
        strTmp = SplitField(strTmp, strLocation);
        strAccess = strTmp;
        strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(strVar, iCWVarName), GetColumnString(strType,iCWType), GetColumnString(strLocation, iCWLocation, TRUE), GetColumnString(strAccess, iCWAccess));
        rstrContent += strTmp;

        CSortedStringMap    astrRefs;
        FindRefs(astrRefs, strVar, strFile, TRUE, TRUE);
        int iRefCount = astrRefs.GetCount();
        for(int iRef = 0; iRef < iRefCount; ++iRef)
        {
            strTmp = astrRefs.GetItem(iRef);
            strTmp = SplitField(strTmp, strLocation);
            strTmp = SplitField(strTmp, strAccess);
            strTmp.Format(_T("%s%s%s%s\n"), GetColumnString(_T(""), iCWVarName), GetColumnString(_T(""),iCWType), GetColumnString(strLocation, iCWLocation, TRUE), GetColumnString(strAccess, iCWAccess));
            rstrContent += strTmp;
        }
    }
}


//------------------------------------------------------------------*
/**
 * GetColumnString
 *   get fix length string to fill column
 *   text is left aligned
 *   empty space is filled with spaces
 *   cutting is from left or right depending on bCutFromLeft
 *     ".." is inserted when cutting
 *
 * @param           crstrText: input text
 * @param           iLen: length of output string
 * @return          aligned text
 * @exception       -
 * @see             -
*/
CString GetColumnString(const CString& crstrText, int iLen, BOOL bCutFromLeft /*=FALSE*/, TCHAR cFill /*=_T(' ')*/)
{
    CString strOut;
    int     iStrlen = crstrText.GetLength();

    if(iLen < iStrlen)
    {
        if(bCutFromLeft)
        {
            strOut = _T("..") + crstrText.Mid(iStrlen - iLen + 2);
        }
        else
        {
            strOut = crstrText.Left(iLen - 2) + _T("..");
        }
    }
    else
    {
        LPCTSTR lpszOut = strOut.GetBuffer(iLen*sizeof(TCHAR));                         // work on buffer
        ::memset((void*)lpszOut, cFill, iLen*sizeof(TCHAR));                          // initialize with spaces
        ::memcpy((void*)lpszOut, (void*)(LPCTSTR)crstrText, crstrText.GetLength());     // copy string
        strOut.ReleaseBuffer(iLen);
    }
    return strOut;
}


//------------------------------------------------------------------*
/**
 * SplitField
 *   used to split a string containing parts delimited by tab characters ('\t')
 *   splits the first part of the string and returns the rest
 *
 * @param           crstrInput: input string
 * @param           rstrField: first part
 * @return          rest string.
 * @exception       -
 * @see             -
*/
CString SplitField(const CString& crstrInput, CString& rstrField)
{
    CString strRest;
    int iFound = crstrInput.Find(_T('\t'));
    if(iFound == -1)
    {
        rstrField = crstrInput;
        return strRest;
    }
    rstrField = crstrInput.Left(iFound);
    strRest = crstrInput.Mid(iFound + 1);
    return strRest;
}
// OSAI feature print XRef 24.03.04 SIS <<

