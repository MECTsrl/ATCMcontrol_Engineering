#ifndef __CPPFILECOLLECTION_H_
#define __CPPFILECOLLECTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CPPFileData;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef CList<CPPFileData*, CPPFileData*>    TPPFileList;

//------------------------------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CPPFileData
 *
 *  @see    ....
*/
class CPPFileData
{
public:
    CPPFileData() : m_bSelected(FALSE), m_lPageCount(0), m_bTemporary(FALSE) { };
    CPPFileData(const CString& crstrFileName, const CString& crstrObjName, BOOL bSelected, BOOL bTemporary) : 
      m_strFileName(crstrFileName),
      m_strObjName(crstrObjName),
      m_bSelected(bSelected),
	  m_bTemporary(bTemporary)
	  {};

public:
    CString m_strFileName;
    CString m_strObjName;
    BOOL    m_bSelected;
	BOOL	m_bTemporary;
    long    m_lPageCount;
};


/**
 *  class CPPFileCollection
 *
 *  @see    ....
*/
class CPPFileCollection
{
public:
    CPPFileCollection();
    ~CPPFileCollection();

    int GetNumFiles();

    void SetName(const CString& crstrName);
    CString GetName();

    CPPFileData* AddFile(const CString& crstrFileName, const CString& crstrObjName, BOOL bSelected = TRUE, BOOL bTemporary = FALSE);
    void AddFiles(CStringArray& rastrFiles, BOOL bSelected = TRUE, BOOL bTemporary = FALSE);
    BOOL RemoveFile(const CString& crstrFileName);

    BOOL SelectFile(const CString& crstrFileName);
    BOOL UnselectFile(const CString& crstrFileName);

    void SelectAllFiles();
    void UnselectAllFiles();

    int GetAllFiles(CStringArray& rastrFiles);
    int GetAllSelectedFiles(CStringArray& rastrFiles);

    BOOL GetFirstSelectedFileDataPosition(POSITION& rpos);
    BOOL GetNextSelectedFileDataPosition(POSITION& rpos);
    CPPFileData* GetFileDataAt(POSITION pos);

protected:
    POSITION GetFilePosition(const CString& crstrFileName);

protected:
    CString         m_strName;      // name of collection
    TPPFileList     m_tFileList;    // list of file data items

    // main frame has full access
    friend class CMainFrame;
};

#endif // __CPPFILECOLLECTION_H_