


#include "stdafx.h"
#include "DragDropHelper\LinkFile.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkFile::CLinkFile(const CString& strLinkFileContent)
{
    CString strRest;
    CString strLine;
    CString strKey;
    CString strValue;

    strRest = strLinkFileContent;
    strRest.TrimLeft();
    strRest.TrimRight();


    while(!strRest.IsEmpty())
    {
        int pos = strRest.Find(_T('\n'));
        if (pos<0)
        {
            pos = strRest.GetLength();
        }
        strLine = strRest.Left(pos);
        strRest = strRest.Mid(pos);
        strLine.TrimLeft();
        strLine.TrimRight();
        strRest.TrimLeft();
        strRest.TrimRight();

        pos = strLine.Find(_T(';'));
        if (pos>0)
        {
            strKey = strLine.Left(pos);
            strValue = strLine.Mid(pos+1);
            strKey.TrimLeft();
            strKey.TrimRight();
            strValue.TrimLeft();
            strValue.TrimRight();
            if (!strKey.IsEmpty() && !strValue.IsEmpty())
            {
                TKeyValue* pKeyValue;
                pKeyValue = new TKeyValue;
                pKeyValue->strKey = strKey;
                pKeyValue->strValue = strValue;
                m_listKeyValue.AddTail(pKeyValue);
            }
        }

    }
}

CLinkFile::~CLinkFile()
{
    POSITION pos;
    pos = m_listKeyValue.GetHeadPosition();
    while(pos)
    {
        TKeyValue* pKeyValue;
        pKeyValue = m_listKeyValue.GetNext(pos);
        delete pKeyValue;
    }
    m_listKeyValue.RemoveAll();
}


BOOL CLinkFile::ExistsKey(const CString strKey)
{
    if (FindKey(strKey)!=NULL)
    {
        return TRUE;
    }
    return FALSE;
}

CString CLinkFile::GetValue(const CString strKey)
{
    TKeyValue* pKeyValue = FindKey(strKey);
    if (pKeyValue==NULL)
    {
        return _T("");
    }
    else
    {
        return pKeyValue->strValue;
    }
}


CString CLinkFile::GetValueWithAttribute(const CString strKey, const CString strAttribute)
{
    POSITION pos;
    pos = m_listKeyValue.GetHeadPosition();
    while(pos)
    {
        TKeyValue* pKeyValue;
        pKeyValue = m_listKeyValue.GetNext(pos);
        if (pKeyValue->strKey.CompareNoCase(strKey)==0)
        {
            CString strAttWithVal;
            CString strAttr;
            CString strValue;
            strAttWithVal = pKeyValue->strValue;
            int p = strAttWithVal.Find(_T(';'));
            if (p>0)
            {
                strAttr  = strAttWithVal.Left(p);
                strValue = strAttWithVal.Mid(p+1);
                if (strAttribute.CompareNoCase(strAttr)==0)
                {
                    return strValue;
                }
            }
        }
    }
    return _T("");
    
}



CLinkFile::TKeyValue* CLinkFile::FindKey(const CString strKey)
{
    POSITION pos;
    pos = m_listKeyValue.GetHeadPosition();
    while(pos)
    {
        TKeyValue* pKeyValue;
        pKeyValue = m_listKeyValue.GetNext(pos);
        if (pKeyValue->strKey.CompareNoCase(strKey)==0)
        {
            return pKeyValue;
        }
    }
    return NULL;
}

