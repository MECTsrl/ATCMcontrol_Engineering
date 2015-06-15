
#include "cgt_misc.h"



CGT_CSVFile::CGT_CSVFile()
{
    m_pszFileContent  = NULL;
    m_ulLength        = 0;
    m_ulFpos          = 0;
    m_nCurrLine       = 1;
    m_lastFieldWasEOL = false;
    m_bDelete         = false;
}
CGT_CSVFile::~CGT_CSVFile()
{
    if(m_bDelete && m_pszFileContent)
        delete(m_pszFileContent);
}
void CGT_CSVFile::setStringContent(
    TCHAR* psz, 
    long   iLen,
    bool   bDelete
)
{
    if(m_bDelete && m_pszFileContent)
        delete(m_pszFileContent);

    m_pszFileContent  = psz;
    m_bDelete         = bDelete;
    m_nCurrLine       = 1;
    m_ulLength        = iLen<0 ? _tcslen(psz) : iLen;
    m_ulFpos          = 0;
    m_lastFieldWasEOL = false;

}

bool CGT_CSVFile::getNextField(
    const TCHAR** ppszField,
    long*         pLen
)
{
    TCHAR        c;
    const TCHAR* pszRet;
    TCHAR*       pszInsertPos;

    *ppszField = NULL;
    if(pLen)
        *pLen  = 0;

    if(isEOF())
        return true;

    pszRet = pszInsertPos = m_pszFileContent + m_ulFpos;

    c = nextChar();
    if(c == '"')
    {
        do 
        {
            c = nextChar();
            if(isEOF())
                goto unexpected_eof; 

            if(c == '"')
            {
                c = nextChar();
                if(isEOF())
                    goto unexpected_eof; 
                if(c != '"')
                    break;
            }
            *pszInsertPos++ = c;
        }
        while (true);
    }
    else
    {
        while(c != '\n' && c != '\r' && c != ';')
        {
            c = nextChar();
            if(isEOF())
                goto unexpected_eof; 
        }
        pszInsertPos = m_pszFileContent + m_ulFpos-1;
    }

    if(c == '\r')
    {
        *pszInsertPos++ = 0;//null terminate
        c = nextChar();
        if(c != '\n')
        {
            //" CR without LF\n"
            return false;
        }
     }
     if(c != '\n' && c != ';')
     {
         return false;
     }
     *pszInsertPos = 0;//null terminate
     
     m_lastFieldWasEOL = (c == '\n');
     
     *ppszField = pszRet;
     if(pLen)
        *pLen = pszInsertPos - pszRet;
     return true;

unexpected_eof:
     return false;
}



long  CGT_CSVFile::getRecordFields(
    long          nMaxFields,
    const TCHAR*  ppszFields[/* nMaxFields*/],
    long*         pLen[/* nMaxFields*/]
)
{
    long  nRead;
    
    assert(nMaxFields>0);

    for(nRead = 0; nRead<nMaxFields; )
    {
        if(!getNextField(&ppszFields[nRead], pLen? pLen[nRead] : NULL))
        {
            nRead = -1;
            break;
        }
        
        nRead++;
        
        if(isLastFieldEOL())
            break;
    }

    return nRead;
}




extern "C"  bool  CGT_LoadCSVFile(
    CGT_CDiagMsg*    pdm,      //error message sink
    CGT_CSVFile*     pReader,  //will be initialised with file
    const TCHAR*     pszPath,  //absolute path or NULL
    const TCHAR*     pszFile   //file name, if pszPath was NULL 
                               //must contain full path
)
{
    bool          bOk;
    TCHAR*        pszContent;
    unsigned long ulLength;

    bOk = CGT_LoadTextFileToMem(
        pdm,
        NULL,
        pszPath,
        pszFile,
        &pszContent, 
        &ulLength, 
        NULL
      );

    if(bOk)
        pReader->setStringContent(pszContent, ulLength, true);

    return bOk;
}


extern "C"  bool  CGT_LoadCSVLocatorFile(
    CGT_CDiagMsg*    pdm,      //error message sink
    CGT_CSVFile*     pReader,  //will be initialised with file
    ICG_IECFront*    pIFront,  //used if locator points to a 4cl lib
    const TCHAR*     pszLocator//file locator
)
{
    bool          bOk;
    TCHAR*        pszContent;
    unsigned long ulLength;

    bOk = CGT_LoadLocatorFileToMem(
        pdm,          //for err msg
        NULL,
        pIFront,      //used if locator points to a 4cl lib
        pszLocator,   //file locator
        &pszContent,  //newly allocated buffer for file content 
        &ulLength,    //NULL or receives the size in TCHARs of
        NULL
    );

    if(bOk)
        pReader->setStringContent(pszContent, ulLength, true);

    return bOk;
}





