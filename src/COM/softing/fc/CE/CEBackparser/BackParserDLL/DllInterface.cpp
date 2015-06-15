
#include "stdafx.h"

#include "DllInterface.h"
#include "ClassViewDescrGenerator.h"
#include "StreamScan.h"
#include "PouParser.h"
#include "EmptyFile.h"

#include "ceextract.h"
#include "ceextract_i.c"

#include <stdlib.h>

#include <sstream>
#include <fstream>

using namespace std;

//------------------------------------------------------------------*
/**
 * Function name    doBackparse
 * Description      generates the XML description for the pou in file FileName
 * @param           string& FileName
 * @param           string& XmlText
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void doBackparse(string FileName, string& XmlText, BackparserProjectInfo & info)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    ::TrcPrint(TRC_INTERFACE,_T("BACKPARSER> parse file %s\n"),FileName.c_str());
    DEBUG_TIMESTAMP_START;
    //=== get base name
    string fileBaseName = getFileBaseName(FileName);
    //=== get extension
    string fileExtension = getFileExtension(FileName);
    CString strFileExtension = fileExtension.c_str();


    string SourceFileName;

    BOOL extractedHeaderFileExists = FALSE;
    string HeaderFileName;
    string Language = "";

    if (CString(fileExtension.c_str()).CompareNoCase(_T("4gr")) == 0)
    {
        HRESULT hr = ExtractHeader(FileName, HeaderFileName);
        if (! SUCCEEDED(hr))
        {
            throw hr;
        }
        extractedHeaderFileExists = TRUE;
        SourceFileName = HeaderFileName;
        GetLanguage(FileName, Language);
    }
    else
    {
        if(CString(fileExtension.c_str()).CompareNoCase(_T("il")) == 0)
            Language = "IL";
        else
            Language = "ST";
        SourceFileName = FileName;
    }

    //=== read file content
    string buf;
    POU* pou = NULL;

    readFileContent(SourceFileName, buf);
    string err = parse_string(buf, &pou);

    if(pou == NULL) {
        throw E_FAIL;
    }

    //pou != NULL
    pou->setProjectInfo(info);
    CString type(pou->getPOUType().c_str());
    pou->setErrorText(err);

    // Handle empty gvl and cst files.
    if (type.Compare(_EMPTYFILE) == 0)
    {
        CString ext(fileExtension.c_str());
        if (ext.CompareNoCase(_EXTENSION_CST) == 0)
        {
            type = _CST_FILE;
        }
        else if (ext.CompareNoCase(_EXTENSION_GVL) == 0)
        {
            type = _GVL_FILE;
        }
        ((EmptyFile*)pou)->setPOUType(string(type));
    }

    // Set name for gvl and cst files. 
    if ((type.Compare(_CST_FILE) == 0) || 
        (type.Compare(_GVL_FILE) == 0))
    {
        pou->setName(fileBaseName);
    }

    DEBUG_TIMESTAMP(_T("parse file"));
    {
        DEBUG_TIMESTAMP_START;
        ClassViewDescrGenerator cvdGenerator(Language, FileName, fileBaseName, fileExtension);

        XmlText.reserve(1000);

        cvdGenerator.gen(*pou, XmlText);
        DEBUG_TIMESTAMP(_T("gen"));
    }
    
    delete pou;

    if (extractedHeaderFileExists == TRUE)
    {
        CFile::Remove(HeaderFileName.c_str());
    }
    DEBUG_TIMESTAMP(_T("backparse"));
}


//------------------------------------------------------------------*
/**
 * Function name    getFileBaseName
 * Description      returns the base name of the file in path
 * @param           string& path
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string getFileBaseName(const string& path)
//================================================================================
{
    string::size_type p_last_backslash = path.find_last_of("\\");
    string::size_type p_last_dot       = path.find_last_of(".");

    if ( p_last_backslash < p_last_dot )
        return path.substr(p_last_backslash + 1, p_last_dot - p_last_backslash - 1);
    else
        return "";
}

//------------------------------------------------------------------*
/**
 * Function name    getFileExtension
 * Description      returns the extension of the file in path
 * @param           string& path
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string getFileExtension(const string& path)
//================================================================================
{
    string::size_type p_last_backslash = path.find_last_of("\\");
    string::size_type p_last_dot       = path.find_last_of(".");
    if ( p_last_backslash < p_last_dot )
        return path.substr(p_last_dot + 1, path.length() - p_last_dot);
    else
        return "";
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding pou object
 * @param           const string& buf
 * @param           POU ** pou
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, POU** pou)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, pou);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding function block
 * @param           const string& buf
 * @param           FunctionBlock ** fb
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, FunctionBlock** fb)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, fb);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding function
 * @param           const string& buf
 * @param           Function** f
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, Function** f)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, f);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding resource
 * @param           const string& buf
 * @param           Resource** res
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, Resource** res)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;

    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, res);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding configuration
 * @param           const string& buf
 * @param           Configuration** con
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, Configuration** con)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, con);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding GlobVarConstList
 * @param           const string& buf
 * @param           GlobVarConstList** gvcl
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, GlobVarConstList** gvcl)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, gvcl);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    parse_string
 * Description      parses the string and creates the corresponding StStruct
 * @param           const string& buf
 * @param           StStruct** st
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string parse_string(const string& buf, StStruct** st)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    istringstream istr(buf);
    ostringstream ostr;
    
    try
    {
        stream_scan  scanner(istr, ostr, buf);
        PouParser parser;

        parser.setdebug(1);

        parser.yyparse(&scanner, st);
    }
    catch(...)
    {
    }

    return ostr.str();
}

//------------------------------------------------------------------*
/**
 * Function name    readFileContent
 * Description      reads the contents of a file
 * @param           const string& fileName
 * @param           string& buf
 * @exception       -
 * @see             -
*/
//================================================================================
void readFileContent(const string& fileName, string& buf)
//================================================================================
{
    int size = 0;
    size_t len = 0;
    const char * pch = NULL;
    char * pb = NULL;

    ifstream input(fileName.c_str());

    if (!input ) {
        throw string("could not open file for reading: \"") + fileName + "\"";
    }
    
    //Reading a file into a buffer is a little bit tricky when using ifstream.
    //calculate the size
    input.seekg(0,ios_base::end);
    streampos pos = input.tellg();
    //return back to begin of file
    input.seekg(0,ios_base::beg);

    //assign size to allocate
    size = pos;
    buf.erase();
    buf.resize(size+1);
    pch = buf.c_str();
    //a little hack to allow read to access string buffer.
    pb = const_cast<char *>(pch);
    //read size bytes.
    input.read(pb,size);
    pb[size] = _T('\0');
    //used to realloc to a smaller size
    len = strlen(pch);
    buf.resize(len);
    
    if (!input.eof()) {
        throw string("error while reading file: \"") + fileName + "\"";
    }
    return;
}

//------------------------------------------------------------------*
/**
 * Function name    ExtractHeader
 * Description      extracts the header ogf a 4gr file
 * @param           const string& SrcFile
 * @param           string& HeaderFile
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
//================================================================================
HRESULT ExtractHeader(const string& SrcFile, string& HeaderFile)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;
    IExtract *pIExtract = NULL;

    hr = CoInitialize(NULL);
	if(FAILED(hr)) {
        return hr;
    }

    try
    {
        hr = CoCreateInstance(CLSID_Extract, 
                              NULL, 
                              CLSCTX_INPROC_SERVER, 
                              IID_IExtract,
                              ((void**)&pIExtract));

    	if(FAILED(hr)) {
            throw hr;
        }

    	BSTR File;
        File = CString(SrcFile.c_str()).AllocSysString();

	    BSTR HFile;
        hr = pIExtract->Extract(File, &HFile);
        ::SysFreeString(File);

        if(FAILED(hr)) {
	        throw hr;
        }

        HeaderFile = CString(HFile);
        ::SysFreeString(HFile);
    }
    catch(...)
    {
    }

    if (pIExtract != NULL)
    {
        pIExtract->Release();
    }
    CoUninitialize( );
    return hr;
}

//------------------------------------------------------------------*
/**
 * Function name    MergeHeader
 * Description      merges a header into a 4gr file
 * @param           const string& MergeFile
 * @param           string& HeaderFile
 * @param           BOOL DelHeaderFile
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
//================================================================================
HRESULT MergeHeader(const string& MergeFile, const string& HeaderFile, BOOL DelHeaderFile)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;
    IExtract *pIExtract = NULL;

    hr = CoInitialize(NULL);
	if(FAILED(hr)) {
        return hr;
    }

    try
    {
        hr = CoCreateInstance(CLSID_Extract, 
                              NULL, 
                              CLSCTX_INPROC_SERVER, 
                              IID_IExtract,
                              ((void**)&pIExtract));

    	if(FAILED(hr)) {
            throw hr;
        }

    	BSTR File;
        File = CString(MergeFile.c_str()).AllocSysString();

	    BSTR HFile;
        HFile = CString(HeaderFile.c_str()).AllocSysString();

        hr = pIExtract->Merge(File, HFile, DelHeaderFile);
        ::SysFreeString(File);
        ::SysFreeString(HFile);

        if(FAILED(hr)) {
	        throw hr;
        }
    }
    catch(...)
    {
    }

    if (pIExtract != NULL)
    {
        pIExtract->Release();
    }
    CoUninitialize( );
    return hr;
}

//------------------------------------------------------------------*
/**
 * Function name    GetDates
 * Description      returns the creation and the modification date of a file
 * @param           const string& FileName
 * @param           string& CreateDate
 * @param           string& modificationDate
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
//================================================================================
HRESULT GetDates(const string &FileName, string& CreateDate, string& ModificationDate)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        CFile file(FileName.c_str(), CFile::modeRead);
        CFileStatus status;
        if (file.GetStatus(status) == FALSE)
        {
            throw;
        }

        CString cd;
        cd.Format(_T("DT#%.4u-%.2u-%.2u-%.2u:%.2u:%.2u"), status.m_ctime.GetYear(), 
                                status.m_ctime.GetMonth(), 
                                status.m_ctime.GetDay(),
                                status.m_ctime.GetHour(),
                                status.m_ctime.GetMinute(),
                                status.m_ctime.GetSecond());
        CreateDate = cd;

        CString md;
        md.Format(_T("DT#%.4u-%.2u-%.2u-%.2u:%.2u:%.2u"), status.m_ctime.GetYear(), 
                                status.m_mtime.GetMonth(), 
                                status.m_mtime.GetDay(),
                                status.m_mtime.GetHour(),
                                status.m_mtime.GetMinute(),
                                status.m_mtime.GetSecond());
        ModificationDate = md;
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * Function name    GetLanguage
 * Description      returns the language of a 4gr file
 * @param           const string& FileName
 * @param           string& Language
 * @exception       -
 * @see             -
*/
//================================================================================
void GetLanguage(const string &FileName, string& Language )
//================================================================================
{
    IExtract2 *pIExtract2 = NULL;
    Language = _T("UNKNOWN");

    HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr)) {
        return;
    }

    hr = CoCreateInstance(CLSID_Extract, 
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IExtract2,
                          ((void**)&pIExtract2));

    if(FAILED(hr)) {
        return;
    }

    BSTR File;
    File = CString(FileName.c_str()).AllocSysString();
    long lOffset = 0;
    BSTR sTagName = L"BODY";
    BSTR sKeyAttrName = L"";
    BSTR sKeyAttrValue = L"";
    BSTR sAttrName = L"LANG";
    BSTR sAttrValue = NULL;
    long plPos;

    pIExtract2->GetXMLAttributeValue(File,
                                     lOffset, 
	                                 sTagName, 
                                     sKeyAttrName, 
                                     sKeyAttrValue, 
                                     sAttrName,
                                     &sAttrValue,
                                     &plPos);

    if (sAttrValue != NULL)
    {
        CString l = sAttrValue;
        l.MakeUpper();
        // Map ALL to SFC and map every language containing SFC to SFC.
        if ((l.Compare("ALL") == 0) || (l.Find("SFC") != -1))
        {
            l = _T("SFC");
        }        
        Language = l;
        ::SysFreeString(sAttrValue);
    }

    ::SysFreeString(File);

    if (pIExtract2 != NULL)
    {
        pIExtract2->Release();
    }
    CoUninitialize( );
    return;
}

