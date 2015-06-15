
#ifndef __CKEYANDVALUE_H_
#define __CKEYANDVALUE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CEProjMan.h"


class CCEProjectManager;

typedef enum _TIniLineType
{
    INI_TYPE_KEY_VALUE,
    INI_TYPE_COMMENT,
    INI_TYPE_EMPTY
} TIniLineType;


//------------------------------------------------------------------*
/**
 *  class CKeyAndValue
 *  holds one key and value line in the projetc file 
 *  "key=value"
 *
 *  @see    ....
*/
class CKeyAndValue
{
public:
    CKeyAndValue() 
    {m_lineType = INI_TYPE_KEY_VALUE;};
    CKeyAndValue(const CString strKey) 
        {m_strKey = strKey; m_lineType = INI_TYPE_KEY_VALUE;};
    CKeyAndValue(const CString strKey, const CString strValue)
        {m_strKey = strKey; m_strValue = strValue; m_lineType = INI_TYPE_KEY_VALUE;};
    ~CKeyAndValue()
        {};
    //Attributes
private:
    CString     m_strKey;
    CString     m_strValue;
    TIniLineType m_lineType;
    //Methods
public:
    CString GetKey() const
        {return m_strKey;};
    void SetKey(const CString strKey)
        {m_strKey = strKey;};
    CString GetValue() const
        {return m_strValue;};
    void SetValue(const CString strValue)
        {m_strValue = strValue;};
    void SetLineType(TIniLineType lineType)
        {m_lineType = lineType;};
    TIniLineType GetLineType() const
        {return m_lineType;};
};

//------------------------------------------------------------------*
/**
 *  class CSection
 *  holds all information (key and value lines) under one section 
 *  in the project file
 *
 *  @see    ....
*/
class CSection
{
public:
    CSection(CCEProjectManager* pProjMan);
    CSection(CCEProjectManager* pProjMan, const CString strSectionName);
    ~CSection();
    //Attribute
private:
    CString                            m_strSectionName;
    CList<CKeyAndValue*,CKeyAndValue*> m_keyAndValueList;
    POSITION                           m_iterPos;
    CCEProjectManager*                 m_pProjMan;
    //Methods
public:
    CString GetSectionName();
    void SetSectionName(const CString strSectionName);

    BOOL GetFirstKeyAndValue(CKeyAndValue** ppKeyAndValue, BOOL bAll=FALSE);
    BOOL GetNextKeyAndValue (CKeyAndValue** ppKeyAndValue, BOOL bAll=FALSE);
    
    BOOL FindKeyAndValue(const CString strKey, const CString strValue,
                         CKeyAndValue** ppKeyAndValue);
    
    BOOL AddKeyAndValue(CKeyAndValue* pKeyAndValue, BOOL bTail=FALSE);
    BOOL AddKeyAndValue(const CString strKey, const CString strValue, BOOL bTail=FALSE);

    BOOL InsertKeyAndValueAfter(const CString strInsertKey, const CString strInsertValue,
                                CKeyAndValue* pAfterKeyAndValue);
    BOOL InsertKeyAndValueAfter(CKeyAndValue* pInsertKeyAndValue,
                                CKeyAndValue* pAfterKeyAndValue);

    BOOL DeleteKeyAndValue(const CString strKey, const CString strValue);

    void CleanUp();
};



//------------------------------------------------------------------*
/**
 *  class CProjectFile
 *  holds all sections of the project file
 *
 *  @see    ....
*/
class CProjectFile
{
public:
    CProjectFile(CCEProjectManager* pProjMan);
    CProjectFile(CCEProjectManager* pProjMan, const CString strFileName);
    ~CProjectFile();
    //Attribute
private:
    CString                     m_strFileName;
    CList<CSection*, CSection*> m_sectionList;
    POSITION                    m_iterPos;
    CCEProjectManager*          m_pProjMan;
    CTime                       m_timeLastParse;
    CSection                    m_firstSection;
    //Methods
public:
    HRESULT SetProjectFile(const CString strFileName);
    HRESULT ReparseProjectFile(BOOL bForce=FALSE);
    HRESULT WriteProjectFile();
    HRESULT WriteProjectFile(const CString strFileName);
  
    BOOL GetFirstSection(CSection** ppSection);
    BOOL GetNextSection(CSection** ppSection);
  
    BOOL FindSection(const CString strSectionName, CSection** ppSection);
  
    BOOL AddSection(CSection* pSection);
    BOOL AddSection(const CString strSectionName);
   
    BOOL InsertSectionAfter(CSection* pInsertSection, CSection* pAfterSection);
    BOOL InsertSectionAfter(const CString strInsertSectionName, CSection* pAfterSection);
   
    BOOL DeleteSection(const CString strSectionName);

    void UpdateFileViewInfo();

private:
    void WriteKeyValueLine(const CKeyAndValue* pKeyAndValue, CArchive& arch);

    void UpdateFileViewInfo(const CString& strSection, E_FV_FileCategory catFile,
                            CMap<CString, LPCTSTR, CString, CString&>& map);

    CMap<CString, LPCTSTR, CString, CString&> m_mapAddFiles;
    CMap<CString, LPCTSTR, CString, CString&> m_mapHelpFiles;

    BOOL m_bUpdateFVInfo;

};
#endif // __CKEYANDVALUE_H_

