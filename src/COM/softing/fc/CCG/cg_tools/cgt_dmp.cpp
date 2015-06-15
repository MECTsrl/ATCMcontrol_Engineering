#include <malloc.h>
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_libmem.h"
#include "fc_tools\fc_string.h"
#include "cgt_misc.h"


CGT_CDmp::CGT_CDmp()
{
    m_ulMaxBuffer  = 0;
    m_ulUsedBuffer = 0;
    m_pBuffer = NULL;
    
    nIdentLevel = 0;
    lineNumber =1;
    columnPos=1;
}


CGT_CDmp::~CGT_CDmp()
{
    if(m_pBuffer)
        FC_LibFreeMemory(m_pBuffer);
}


void CGT_CDmp::growBuffer(unsigned long  ulMin)
{
    if(m_pBuffer==NULL)
    {
        m_ulMaxBuffer = 4096;
        if(m_ulMaxBuffer<ulMin)
            m_ulMaxBuffer = ulMin;

        FC_LibNewMemory((void**)&m_pBuffer, m_ulMaxBuffer*sizeof(TCHAR));            
    }
    else
    {
        m_ulMaxBuffer *= 2;
        if(m_ulMaxBuffer<ulMin)
            m_ulMaxBuffer = ulMin;
        FC_LibResizeMemory((void**)&m_pBuffer, m_ulMaxBuffer*sizeof(TCHAR));
    }
}
void CGT_CDmp::append(CGT_CDmp& dmp)
{
    unsigned long ulNewSize;

    ulNewSize = m_ulUsedBuffer + dmp.m_ulUsedBuffer;
    
    if(ulNewSize >= m_ulMaxBuffer)
        growBuffer(ulNewSize);

    memcpy(m_pBuffer+m_ulUsedBuffer, dmp.m_pBuffer, dmp.m_ulUsedBuffer);
    m_ulUsedBuffer = ulNewSize;
}
CGT_CDmp& CGT_CDmp::operator<<(TCHAR dmpMe)
{ 
    if(dmpMe=='\n')
    {
       if(m_ulUsedBuffer==0 ||
          (m_ulUsedBuffer>0 && m_pBuffer[m_ulUsedBuffer-1]!='\r')
       )
         putchar(_T('\r'));

       columnPos=0;
       lineNumber++;
    }
    else if(nIdentLevel>0 && columnPos==1)
    {
        for(int i=nIdentLevel*nIndentSpaces;i>0;i--)
              putchar(_T(' '));
    }

    putchar(dmpMe);
    return *this;
}
#ifndef UNICODE
CGT_CDmp& CGT_CDmp::operator<<(const wchar_t* dmpMe)
{
    TCHAR* pszBuffer;
    int    iLen = wcslen(dmpMe);
    
    iLen = iLen*3+1;
    pszBuffer = (TCHAR*)_alloca(iLen);
    
    FC_StringToMultiByte(dmpMe, pszBuffer, iLen);
    return *this<<pszBuffer;
}
#endif


CGT_CDmp& CGT_CDmp::operator<<(const TCHAR* dmpMe)
{
    for( ;*dmpMe; dmpMe++)
        *this<<*dmpMe;

    return *this;
}
CGT_CDmp& CGT_CDmp::lo(const TCHAR* dmpMe)
{
    for( ;*dmpMe; dmpMe++)
        *this<<(TCHAR)_totlower(*dmpMe);

    return *this;
}
CGT_CDmp& CGT_CDmp::up(const TCHAR* dmpMe)
{
    for( ;*dmpMe; dmpMe++)
        *this<<(TCHAR)towupper(*dmpMe);

    return *this;
}
CGT_CDmp& CGT_CDmp::sep(int nPos)
{
   *this<<_T(' ');

    for(;columnPos<nPos;)
            this->putchar(_T(' '));

    return *this;
}


//HACK Function ?????
static TCHAR* DumpCharAsThreeOctal(int c, char* buff)
{
    static TCHAR conv[]={'0','1','2','3','4','5','6','7'};
    buff[3] = 0;                        
    buff[2] = conv[(c>>0) & 0x07];
    buff[1] = conv[(c>>3) & 0x07];
    buff[0] = conv[(c>>6) & 0x03];        
    return(buff);
}

CGT_CDmp& CGT_CDmp::dumpULONGAsHex(unsigned long ul, int iMinText)
{
    _ultot(ul,itoa_puff,16);
    for(int i=iMinText - _tcslen(itoa_puff); i>0; i--)
        *this<<'0';

    return *this<<itoa_puff;
}


CGT_CDmp& CGT_CDmp::dumpAsXMLUTF8(const TCHAR* dmpMe, bool bDoquote)
{
    //TODO ganz deppad:
    //used a number &#222

    if(bDoquote)
        this->putchar(_T('"'));
    for( ;*dmpMe; dmpMe++)
    {
        unsigned char uc = (unsigned char)*dmpMe;

        if(uc>=32      && _istascii(uc) &&
           uc!=_T('"') && uc!=_T('&') && 
           uc!=_T('<') && uc!=_T('>') &&
           uc!=_T(']') && uc!=_T('[')
         )
        {
            *this<<*dmpMe;
        }
        else
        {
            this->putchar(_T('&'));
            this->putchar(_T('#'));
            *this<<((unsigned long)uc);
            this->putchar(_T(';'));
        }
    }

    if(bDoquote)
        this->putchar(_T('"'));
    
    return *this;
}

CGT_CDmp& CGT_CDmp::xmlAttrVal(const TCHAR* dmpMe)
{
    return dumpAsXMLUTF8(dmpMe, true);
}



CGT_CDmp& CGT_CDmp::dumpAsCSV(
    const TCHAR* pBinStr, 
    int          iChars, 
    bool         bDumpEscapes
)
{
    int   i;
    TCHAR cc;
    bool  bNeedEscape;

    
    //first check if there are non print " or ; chars in there:
    if(bDumpEscapes)
       bNeedEscape = CGT_CDmpCSVNeedEscape(pBinStr,iChars)!=0;
    else
       bNeedEscape = false; 


    if(bNeedEscape)
        *this<<_T('"');

    for(i=0; ;i++)
    {
        if(iChars>=0 && i>=iChars)
            break;

        cc = pBinStr[i];
        if(iChars<0 && cc==0)
            break;

        
        if(cc==_T('"'))
        {
            putchar(_T('"'));
        }
        else if(cc==_T('\n'))
        {
            columnPos=0;
            lineNumber++;
        }

        putchar(pBinStr[i]);
    }

    if(bNeedEscape)
        *this<<_T('"');

    //done          
    return(*this);
}






extern "C" bool CGT_CDmpCSVNeedEscape(
    const TCHAR* pBinStr, 
    int          iChars
)
{
    for(int i=0; ;i++)
    {
        if(iChars>=0 && i>=iChars)
            break;
        else if(iChars<0 && pBinStr[i]==0)
            break;


#ifdef UNICODE
        if(pBinStr[i]<32 || pBinStr[i]==L'"' || pBinStr[i]==L';')
            return true;
#else
        if((unsigned char)pBinStr[i]<32 || pBinStr[i]=='"' || pBinStr[i]==';')
            return true;
#endif
    }
    
    return false;
}
