/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_csvread.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_csvread.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_csvread.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== CSV utility implementation 
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.03.02  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
//need assert for fc_cgarray.h
#include <memory.h>
#include "fc_tools\fc_libmem.h"
#define assert(exp) if(exp); else FC_libassert(__FILE__, __LINE__, #exp)
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_string.h"

//----  Static Initializations:   ----------------------------------*






class CSVFile
{
    TCHAR*        m_pszFileContent;
    unsigned long m_ulLength;
    unsigned long m_ulFpos;
    bool          m_lastFieldWasEOL;
    long          m_nCurrLine;
    TCHAR         m_chSep;    //',' or ';'

    TCHAR nextChar()
    {
        TCHAR c = m_ulFpos<m_ulLength ? m_pszFileContent[m_ulFpos++] : 0;
        if(c==_T('\n'))
            m_nCurrLine++;
        return c;
    }

public:
    CSVFile(
        TCHAR*  psz, //pointer to raw text, the buffer will be manipulated
        long    iLen,//lstrlen(psz) or -1 the function calls lstrlen.
        TCHAR   chSep
    )
    {
        FC_LIBASSERT(chSep==';' || chSep==',');

        m_pszFileContent  = psz;
        m_nCurrLine       = 1;
        m_ulLength        = iLen<0 ? _tcslen(psz) : iLen;
        m_ulFpos          = 0;
        m_lastFieldWasEOL = false;
        m_chSep           = chSep;
    }

    
    //------------------------------------------------------------------*
    /**
     * getNextField sets *ppszField to a 0-term string that is the 
     * value of the next field. 
     * The pointer point into the memory passed in setStringContent, 
     * this memory is therefore changed the 0-char is inserted and 
     * possible escapes are processed in place.
     * This function can also be used if the field has embedded 
     * 0-chars then the pLen parameter must be used.
     * 
     *
     * @param   ppszField address of pointer to string to be set to 
     *                    next field's value.
     * @param   pLen      optional long* to receive the number of 
     *                    TCHARs of the field, works also if embedded 
     *                    0-chars.
     *
     * @return  false if any errors, error can be invalid escape syntax
     *          or unexpected EOF or embeded 0-char. To get the line info
     *          use getLineno() after error condition.
     *          TODO supply a last error code ? or return int ?
    */
    bool  getNextField(
        TCHAR**   ppszField,
        long*     pLen = NULL
    );

    bool  hasMoreFields() {return m_ulFpos<m_ulLength;}
    bool  isEOF()         {return m_ulFpos>=m_ulLength;}
    bool  isLastFieldEOL(){return m_lastFieldWasEOL;}
    long  getLineno()const{return m_nCurrLine;}
};


bool CSVFile::getNextField(
    TCHAR**       ppszField,
    long*         pLen
)
{
    TCHAR        c;
    TCHAR*       pszRet;
    TCHAR*       pszInsertPos;

    *ppszField = NULL;
    if(pLen)
        *pLen  = 0;

    if(isEOF())
        return true;

    pszRet = pszInsertPos = m_pszFileContent + m_ulFpos;

    c = nextChar();
    if(c == _T('"'))
    {
        do 
        {
            c = nextChar();
            if(isEOF())
                goto unexpected_eof; 

            if(c == _T('"'))
            {
                c = nextChar();
                if(isEOF())
                    goto unexpected_eof; 
                if(c != _T('"'))
                    break;
            }
            *pszInsertPos++ = c;
        }
        while (true);
    }
    else
    {
        while(c != _T('\n') && c != _T('\r') && c != m_chSep)
        {
            c = nextChar();
            if(isEOF())
                goto unexpected_eof; 
        }
        pszInsertPos = m_pszFileContent + m_ulFpos-1;
    }

    if(c == _T('\r'))
    {
        *pszInsertPos++ = 0;//null terminate
        c = nextChar();
        if(c != _T('\n'))
        {
            //" CR without LF\n"
            return false;
        }
     }
     if(c != _T('\n') && c != m_chSep)
     {
         return false;
     }
     *pszInsertPos = 0;//null terminate
     
     m_lastFieldWasEOL = (c == _T('\n'));
     
     *ppszField = pszRet;
     if(pLen)
        *pLen = pszInsertPos - pszRet;
     return true;

unexpected_eof:
     return false;
}


FC_TOAPI FC_CSVMemFile* FC_TODECL FC_CSVSplitEx(
    TCHAR*         pszFileContent,  //CSV file content
    long           lFileChars,      //number of TCHARs of pszFileContent 
    TCHAR          chSeperator,     //either ';' or ','
    int            bTrimAllFields,  //if TRUE all fields are trimmed on both sides
    int            bHashIsComment   //if TRUE and the first non white 
                                    //space char in the fist field is a '#' 
                                    //then skip this record
)
{
    FC_CSVMemFile*              pMem;
    FC_CSVField                 field;
    FC_CGArray<FC_CSVField>     fields(128);
    FC_CGArray<FC_CSVRecord*>   records(1024);
    CSVFile                     parse(pszFileContent, lFileChars, chSeperator);

    if(!FC_LibNewMemory((void**)&pMem, sizeof(FC_CSVMemFile)))
        return NULL;
    memset(pMem, 0, sizeof(FC_CSVMemFile));


    while(parse.hasMoreFields())
    {
        field.lLineNo = parse.getLineno();
        
        if(!parse.getNextField(&field.pszField, &field.lLength))
        {
            pMem->lErrLine   = field.lLineNo;
            pMem->lErrReason = 1;
            break;
        }
        
        if(bTrimAllFields)
        {
            FC_StringTrim(field.pszField);
        }

        fields.add(field);

        if(parse.isLastFieldEOL())
        {
            long            n;
            FC_CSVRecord*   pRec;
            int             bCommentRec;

            n = fields.getUsed();

            if(bHashIsComment)
            {
                FC_CSVField firstField = fields.get(0);
                const TCHAR* pszFirstChar = firstField.pszField;
                FC_StringSkipWhite(&pszFirstChar);
                bCommentRec = *pszFirstChar == '#';
            }
            else
            {
                bCommentRec = 0;
            }

            if(!bCommentRec)
            {
                if(!FC_LibNewMemory((void**)&pRec, sizeof(FC_CSVField)*n + sizeof(long)))
                {
                    for(n=0; n<records.getUsed(); n++)
                        FC_LibFreeMemory(records.get(n));

                    FC_LibFreeMemory(pMem);
                    return NULL;
                }
            
                pRec->nFields = n;
                memcpy(&pRec->fields[0], fields.getCElements(), sizeof(FC_CSVField)*n);
                records.add(pRec);
            }

            fields.setUsed(0);
        }
    }

    pMem->nRec = records.getUsed();
    pMem->ppRec = records.getMem();

    return pMem;
}




FC_TOAPI FC_CSVMemFile* FC_TODECL FC_CSVSplit(
    TCHAR*         pszFileContent, 
    long           lFileChars
) 
{
    return FC_CSVSplitEx(pszFileContent, lFileChars, ';', FALSE, FALSE);
}





FC_TOAPI void FC_TODECL FC_CSVFreeMemFile(FC_CSVMemFile* pMem)
{
    if(pMem)
    {
        for(int i=0; i<pMem->nRec; i++)
            FC_LibFreeMemory(pMem->ppRec[i]);

        FC_LibFreeMemory(pMem->ppRec);
        FC_LibFreeMemory(pMem);
    }
}


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_csvread.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.11.06   Time: 20:45
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 *==
 *----------------------------------------------------------------------------*
*/
