/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cdump.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $RCSFile$
 *
 * =PROJECT             1N0096ITN
 *
 * =SWKE                BL4FF
 *
 * =COMPONENT           cff2xml
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
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
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  File created: 18.05.2004
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FC_CDUMP_H_
#define __FC_CDUMP_H_


//----  Aggregate Includes:   --------------------------------------*
#include <stdlib.h>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//TODO: use TCHAR and make unicode aware.

#define FC_XML_STANDARD_HDR "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

class FC_CDump
{
    //never use:
    FC_CDump(FC_CDump&);
    void operator=(FC_CDump&);

    char itoa_puff[64];
    enum {nIndentSpaces = 4};
    
    unsigned long m_ulMaxBuffer;
    unsigned long m_ulUsedBuffer;
    char*         m_pBuffer;
    int           m_nIdentLevel;
    long          m_lineNumber;
    int           m_columnPos;
	int           m_bTextMode;//if TRUE convert \n to \r\n

    inline void putchar(int c)
    {  
        if(m_ulUsedBuffer>=m_ulMaxBuffer)
            growBuffer(0);
        
        m_pBuffer[m_ulUsedBuffer++]=(char)c;
        m_columnPos++;
    }
    void growBuffer(unsigned long ulMin);

public:
    FC_CDump();
    ~FC_CDump();            

    void incrIndent()    {++m_nIdentLevel;}
    void decrIndent()    {--m_nIdentLevel;}
    int  getIndent()const{return m_nIdentLevel;}
    void setIndent(int i){m_nIdentLevel=i;}
    long lineNo()const   {return(m_lineNumber);}
    int  column()const   {return(m_columnPos);}
	void setTextMode(int bMode)   {m_bTextMode = bMode;}//if TRUE convert \n to \r\n

    const char* getPChars()const{return m_pBuffer;}
    long         getNChars()const{return m_ulUsedBuffer;}

    const char* getSzBuffer()
    {
        if(m_ulUsedBuffer>=m_ulMaxBuffer)
            growBuffer(0);
        
        m_pBuffer[m_ulUsedBuffer]=0;
        return m_pBuffer;
    }
    FC_CDump& operator<<(char dmpMe);
    FC_CDump& operator<<(const char* dmpMe);
    FC_CDump& operator<<(int dmpMe)
    {
      return *this<<itoa(dmpMe,itoa_puff,10);
    }
    FC_CDump& operator<<(unsigned long dmpMe)
    {
      return *this<<ultoa(dmpMe,itoa_puff,10);
    }
    FC_CDump& operator<<(long dmpMe)
    { 
      return *this<<ltoa(dmpMe,itoa_puff,10);
    }
    FC_CDump& operator<<(__int64 dmpMe)
    { 
        //CAUTION: the runtime function _i64tot does not work with negative input
        //handle negative input by hand! TODO: still a problem ??
        if(dmpMe<0)
            return *this<<'-'<<_ui64toa((unsigned __int64)-dmpMe,itoa_puff,10);
        else
            return *this<<_ui64toa((unsigned __int64)dmpMe,itoa_puff,10);
    }
    FC_CDump& dumpULONGAsHex(unsigned long ul, int iMinText);
    FC_CDump& dumpAsXMLUTF8(const char* dmpMe, bool bDoquote=false);
    FC_CDump& xmlAttrVal(const char* dmpMe);
    FC_CDump& xmlAttrVal(long dmpMe);
    FC_CDump& xmlAttrVal(unsigned long dmpMe);

    FC_CDump& dumpAsCSV(const char* pszStr, char csvSeperator = ';');

    //write content to named file, return FALSE if IO error:
    int dmpToFile(const char* pszFileName, int bAppend);
};

//return TRUE if pszStr contains \n or csvSeperator
int FC_CSVNeedEscape(const char* pszStr, char csvSeperator = ';');


#endif // __FC_CDUMP_H_


/*
 *----------------------------------------------------------------------------*
 *  $Log: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cdump.h $
 * 
 * 1     28.02.07 19:01 Ln
 * 
 * 7     7.11.06 13:58 Ef
 * implemented setTextMode
 * 
 * 6     6.11.06 12:16 Ef
 * 
 * 5     31.10.06 9:55 Ef
 * 
 * 4     24.08.06 17:52 Ef
 * 
 * 3     24.08.06 14:25 Ef
 * 
 * 2     24.08.06 14:15 Ef
 * 
 * 1     24.08.06 14:07 Ef
 *  Revision 1.1  2004/05/26 15:52:45  EF
 *  initial version from EF
 *
 *==
 *----------------------------------------------------------------------------*
*/
