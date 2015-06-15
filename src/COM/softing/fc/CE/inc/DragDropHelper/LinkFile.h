
#if !defined(AFX_LINKFILE_H__FCFB3B93_1142_4437_B152_280A0D3D8EEB__INCLUDED_)
#define AFX_LINKFILE_H__FCFB3B93_1142_4437_B152_280A0D3D8EEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLinkFile  
{
public:
	CLinkFile(const CString& strLinkFileContent);
	virtual ~CLinkFile();

    BOOL ExistsKey(const CString strKey);
    CString GetValue(const CString strKey);
    CString GetValueWithAttribute(const CString strKey, const CString strAttribute);

private:
    typedef struct StructKeyValue
    {
        CString strKey;
        CString strValue;
    } TKeyValue;

    CList<TKeyValue*, TKeyValue*> m_listKeyValue;

    TKeyValue* FindKey(const CString strKey);

};

#endif // !defined(AFX_LINKFILE_H__FCFB3B93_1142_4437_B152_280A0D3D8EEB__INCLUDED_)

