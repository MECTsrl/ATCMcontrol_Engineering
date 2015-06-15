#ifndef __KADFILEEXTINFO_H_
#define __KADFILEEXTINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADFileExtInfo
 *
 *  - used like a struct but please:
 *  - use SetExtension to set extension string. This ensures that
 *    lower case is used
 *  - use SetEditorGUID to set handler for editor. This creates new m_pEditorGUID
 *
 *  @see    CKADFileInfo
*/
class CKADFileExtInfo
{
public:
    CKADFileExtInfo();
    ~CKADFileExtInfo();

    void SetExtension(CString strExt);
    void SetEditorGUID(GUID& tGuid);
    void SetParserGUID(GUID& tGuid);

public:

    CString     m_strExt;           // extension
    CString     m_strIcon;          // icon (bmp-file)
    GUID*       m_pEditorGUID;      // editor guid
    BOOL        m_bProgEditor;      // is program editor
    CString     m_strExeEditor;     // editor exe file
    CString     m_strFilterText;    // category filter text for files of this sort
    GUID*       m_pParserGUID;      // parser guid
    BOOL        m_bVisible;         // visibility flag for file view
    BOOL        m_bFindInFiles;     // find in files flag
    CString     m_strOrderKey;      // order key used for filters
    BOOL        m_bPrint;           // to be printed in print project?
};


#endif // __KADFILEEXTINFO_H_