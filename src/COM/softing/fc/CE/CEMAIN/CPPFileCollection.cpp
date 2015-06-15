
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CPPFileCollection.h"

//----  Static Initializations:   ----------------------------------*


CPPFileCollection::CPPFileCollection()
{
}

CPPFileCollection::~CPPFileCollection()
{
	// delete all file data objects
	POSITION pos = m_tFileList.GetHeadPosition();
	CPPFileData*	pData;
	while(pos)
	{
		pData = m_tFileList.GetNext(pos);
		delete pData;
	}
}

//------------------------------------------------------------------*
/**
 * set name.
 *
 * @param           [in] crstrName: name of collection
 * @return          -
 * @exception       -
 * @see             GetName()
*/
void CPPFileCollection::SetName(const CString& crstrName)
{
    m_strName = crstrName;
}


//------------------------------------------------------------------*
/**
 * get number of files.
 *
 * @param           -
 * @return          number of files in collection
 * @exception       -
 * @see             -
*/
int CPPFileCollection::GetNumFiles()
{
    return m_tFileList.GetCount();
}

//------------------------------------------------------------------*
/**
 * get name.
 *
 * @param           -
 * @return          name of collection
 * @exception       -
 * @see             SetName()
*/
CString CPPFileCollection::GetName()
{
    return m_strName;
}

//------------------------------------------------------------------*
/**
 * add file.
 *
 * @param           [in] crstrFileName: file name of file to be added
 * @param           [in] bSelected: file is selected? Default is TRUE
 * @return          new file data?
 * @exception       -
 * @see             RemoveFile()
*/
CPPFileData* CPPFileCollection::AddFile(const CString& crstrFileName, const CString& crstrObjName, BOOL bSelected /*=TRUE*/, BOOL bTemporary /*=FALSE*/)
{
    CPPFileData* pData = new CPPFileData(crstrFileName, crstrObjName, bSelected, bTemporary);
    m_tFileList.AddTail(pData);
	return pData;
}

//------------------------------------------------------------------*
/**
 * add files.
 *
 * @param           [in] rastrFiles: string array of file names to be added
 * @param           [in] bSelected: file is selected? Default is TRUE
 * @return          successful?
*/
void CPPFileCollection::AddFiles(CStringArray& rastrFiles, BOOL bSelected /*=TRUE*/, BOOL bTemporary /*=FALSE*/)
{
    int iNumFiles = rastrFiles.GetSize();
    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        AddFile(rastrFiles[iFile], rastrFiles[iFile], bSelected, bTemporary);
    }
}


//------------------------------------------------------------------*
/**
 * remove file.
 *
 * @param           [in] crstrFileName: file name of file to be removed
 * @return          successful?
 * @exception       -
 * @see             AddFile()
*/
BOOL CPPFileCollection::RemoveFile(const CString& crstrFileName)
{
    POSITION pos = GetFilePosition(crstrFileName);
    if(pos)
    {
		CPPFileData*	pData = m_tFileList.GetAt(pos);
		ASSERT(pData);
		delete pData;
        m_tFileList.RemoveAt(pos);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * select file.
 *
 * @param           [in] crstrFileName: file name (incl path) of file
 *                                      to select
 * @return          successful?
 * @exception       -
 * @see             UnselectFile()
*/
BOOL CPPFileCollection::SelectFile(const CString& crstrFileName)
{
    POSITION pos = GetFilePosition(crstrFileName);
    if(pos)
    {
        CPPFileData* pData;
        pData = m_tFileList.GetAt(pos);
        pData->m_bSelected = TRUE;
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * unselect file.
 *
 * @param           [in] crstrFileName: file name (incl path) of file
 *                                      to unselect
 * @return          successful?
 * @exception       -
 * @see             SelectFile()
*/
BOOL CPPFileCollection::UnselectFile(const CString& crstrFileName)
{
    POSITION pos = GetFilePosition(crstrFileName);
    if(pos)
    {
        CPPFileData* pData;
        pData = m_tFileList.GetAt(pos);
        pData->m_bSelected = FALSE;
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * select all files.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             UnselectAllFiles()
*/
void CPPFileCollection::SelectAllFiles()
{
    POSITION    pos;
    CPPFileData* pData;

    pos = m_tFileList.GetHeadPosition();
    while(pos)
    {
        pData = m_tFileList.GetAt(pos);
        pData->m_bSelected = TRUE;
        m_tFileList.GetNext(pos);
    }
}

//------------------------------------------------------------------*
/**
 * unselect all files.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             SelectAllFiles()
*/
void CPPFileCollection::UnselectAllFiles()
{
    POSITION    pos;
    CPPFileData* pData;

    pos = m_tFileList.GetHeadPosition();
    while(pos)
    {
        pData = m_tFileList.GetAt(pos);
        pData->m_bSelected = FALSE;
        m_tFileList.GetNext(pos);
    }
}


//------------------------------------------------------------------*
/**
 * get all files.
 *
 * @param           [in/out] rastrFiles: file name string array of all files
 * @return          number of files
 * @exception       -
 * @see             -
*/
int CPPFileCollection::GetAllFiles(CStringArray& rastrFiles)
{
    POSITION    pos;
    CPPFileData* pData;

    pos = m_tFileList.GetHeadPosition();
    while(pos)
    {
        pData = m_tFileList.GetAt(pos);
        rastrFiles.Add(pData->m_strFileName);
        m_tFileList.GetNext(pos);
    }
    return (rastrFiles.GetSize());
}


//------------------------------------------------------------------*
/**
 * get all selected files.
 *
 * @param           [in/out] rastrFiles: file name string array of all selected files
 * @return          number of files
 * @exception       -
 * @see             -
*/
int CPPFileCollection::GetAllSelectedFiles(CStringArray& rastrFiles)
{
    POSITION    pos;
    CPPFileData* pData;

    pos = m_tFileList.GetHeadPosition();
    while(pos)
    {
        pData = m_tFileList.GetAt(pos);
        if(pData->m_bSelected)
        {
            rastrFiles.Add(pData->m_strFileName);
        }
        m_tFileList.GetNext(pos);
    }
    return (rastrFiles.GetSize());
}


//------------------------------------------------------------------*
/**
 * get file position.
 *
 *  search for file name and return position in list.
 *
 * @param           [in] crstrFileName: file name (incl path)
 * @return          position of file or NULL if not found
 * @exception       -
 * @see             -
*/
POSITION CPPFileCollection::GetFilePosition(const CString& crstrFileName)
{
    POSITION		pos;
    CPPFileData*	pData;

    pos = m_tFileList.GetHeadPosition();
    while(pos)
    {
        pData = m_tFileList.GetAt(pos);
        if(pData->m_strFileName.CompareNoCase(crstrFileName))
        {
            return pos;
        }
        m_tFileList.GetNext(pos);
    }
    return pos;
}

//------------------------------------------------------------------*
/**
 * get first selected file data position.
 *
 * @param           rpos: list position of file data
 * @return          successful?
 * @exception       -
 * @see             GetNextSelectedFileDataPosition()
*/
BOOL CPPFileCollection::GetFirstSelectedFileDataPosition(POSITION& rpos)
{
    CPPFileData* pFileData;

    rpos = m_tFileList.GetHeadPosition();
    while(rpos)
    {
        pFileData = m_tFileList.GetAt(rpos);
        if(pFileData->m_bSelected)
        {
            return TRUE;
        }
        m_tFileList.GetNext(rpos);
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * get file data at.
 *
 * @param           rpos: start position
 * @return          successful?
 * @exception       -
 * @see             GetFirstSelectedFileDataPosition()
*/
CPPFileData* CPPFileCollection::GetFileDataAt(POSITION pos)
{
	CPPFileData* pFileData = NULL;
    if(pos)
    {
        pFileData = m_tFileList.GetAt(pos);
    }
    return pFileData;
}



//------------------------------------------------------------------*
/**
 * get next selected file data position.
 *
 * @param           rpos: list position of file data
 * @return          successful?
 * @exception       -
 * @see             GetFirstSelectedFileDataPosition(), GetNextSelectedFileDataPosition()
*/
BOOL CPPFileCollection::GetNextSelectedFileDataPosition(POSITION& rpos)
{
    CPPFileData* pFileData;

    m_tFileList.GetNext(rpos);
    while(rpos)
    {
        pFileData = m_tFileList.GetAt(rpos);
        if(pFileData->m_bSelected)
        {
            return TRUE;
        }
        m_tFileList.GetNext(rpos);
    }
    return FALSE;
}


