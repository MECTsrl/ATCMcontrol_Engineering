#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "fc_tools\fc_cdump.h"



FC_CDump::FC_CDump()
{
    m_ulMaxBuffer  = 0;
    m_ulUsedBuffer = 0;
    m_pBuffer = NULL;
    
    m_nIdentLevel = 0;
    m_lineNumber =1;
    m_columnPos=1;
	m_bTextMode = 0;
}


FC_CDump::~FC_CDump()
{
    if(m_pBuffer)
        free(m_pBuffer);
}


void FC_CDump::growBuffer(unsigned long  ulMin)
{
    if(m_pBuffer==NULL)
    {
        m_ulMaxBuffer = 4096;
        if(m_ulMaxBuffer<ulMin)
            m_ulMaxBuffer = ulMin;

        m_pBuffer = (char*)malloc(m_ulMaxBuffer*sizeof(char));
    }
    else
    {
        m_ulMaxBuffer *= 2;
        if(m_ulMaxBuffer<ulMin)
            m_ulMaxBuffer = ulMin;
        m_pBuffer = (char*)realloc(m_pBuffer, m_ulMaxBuffer*sizeof(char));
    }
}

FC_CDump& FC_CDump::operator<<(char dmpMe)
{ 
    if(dmpMe=='\n')
    {
		if(m_bTextMode)
		{
		   if(m_ulUsedBuffer==0 ||
			  (m_ulUsedBuffer>0 && m_pBuffer[m_ulUsedBuffer-1]!='\r')
		   )
			 putchar('\r');
		}
		m_columnPos=0;
		m_lineNumber++;
    }
    else if(m_nIdentLevel>0 && m_columnPos==1)
    {
        for(int i=m_nIdentLevel*nIndentSpaces;i>0;i--)
              putchar(' ');
    }

    putchar(dmpMe);
    return *this;
}

FC_CDump& FC_CDump::operator<<(const char* dmpMe)
{
    for( ;*dmpMe; dmpMe++)
        *this<<*dmpMe;

    return *this;
}

FC_CDump& FC_CDump::dumpULONGAsHex(unsigned long ul, int iMinText)
{
    ultoa(ul,itoa_puff,16);
    for(int i=iMinText - strlen(itoa_puff); i>0; i--)
        *this<<'0';

    return *this<<itoa_puff;
}


FC_CDump& FC_CDump::dumpAsXMLUTF8(const char* dmpMe, bool bDoquote)
{
    if(bDoquote)
        this->putchar('"');
    for( ;*dmpMe; dmpMe++)
    {
        unsigned char uc = (unsigned char)*dmpMe;

        if(uc>=32  && isascii(uc) &&
           uc!='"' && uc!='&' && 
           uc!='<' && uc!='>' &&
           uc!=']' && uc!='['
         )
        {
            *this<<*dmpMe;
        }
        else
        {
            this->putchar('&');
            this->putchar('#');
            *this<<((unsigned long)uc);
            this->putchar(';');
        }
    }

    if(bDoquote)
        this->putchar('"');
    
    return *this;
}

FC_CDump& FC_CDump::xmlAttrVal(const char* dmpMe)
{
    return dumpAsXMLUTF8(dmpMe, true);
}

FC_CDump& FC_CDump::xmlAttrVal(long dmpMe)
{
    char buffer[64];
    ltoa(dmpMe, buffer, 10);
    return dumpAsXMLUTF8(buffer, true);
}

FC_CDump& FC_CDump::xmlAttrVal(unsigned long dmpMe)
{
    char buffer[64];
    ultoa(dmpMe, buffer, 10);
    return dumpAsXMLUTF8(buffer, true);
}

FC_CDump& FC_CDump::dumpAsCSV(const char* pszStr, char csvSeperator)
{
    int   i;
    char  cc;
    int   bNeedEscape;

    //first check if there are non print " or csvSeperator chars in there:
    bNeedEscape = FC_CSVNeedEscape(pszStr, csvSeperator);

    if(bNeedEscape)
        putchar('"');

    for(i=0; (cc = pszStr[i])!=0 ;i++)
    {
        if(cc=='"')
        {
            putchar('"');
        }
        else if(cc=='\n')
        {
            m_columnPos=0;
            m_lineNumber++;
        }
        putchar(cc);
    }

    if(bNeedEscape)
        putchar('"');

    return(*this);
}

int FC_CDump::dmpToFile(const char* pszFileName, int bAppend)
{
    FILE* pFile;
    
    if(bAppend)
        pFile = fopen(pszFileName, "at");
    else
        pFile = fopen(pszFileName, "wt");
    
    if(!pFile)
        return false;

    fwrite(m_pBuffer, m_ulUsedBuffer, 1, pFile);

    fclose(pFile);
    return true;
}




int FC_CSVNeedEscape(const char* pszStr, char csvSeperator)
{
    int  i;
    char cc;

    for(i=0; (cc = pszStr[i])!=0; i++)
    {
        if((unsigned char)cc<32 || cc=='"' || cc==csvSeperator)
            return true;
    }
    
    return false;
}
