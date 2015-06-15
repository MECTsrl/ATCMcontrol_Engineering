/* $Header: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/GHFile.cpp 1     28.02.07 19:07 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: GHFile.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/GHFile.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / AddOnHandler
 *
 * =COMPONENT			AddOnBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:07 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "GHFile.h"
#include "utilif.h"

//----  Static Initializations:   ----------------------------------*


//------------------------------------------------------------------*
/**
 * constructor of CGHFile
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CGHFile::CGHFile()
{
}

//------------------------------------------------------------------*
/**
 * destructor of CGHFile
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CGHFile::~CGHFile()
{
}


//------------------------------------------------------------------*
/**
 * reset pou protection data.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::ResetPouProtectData()
{
    m_tPouProtectDataList.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * set file name.
 *
 * @param           crstrFileName: file name incl path
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::SetFileName(const CString& crstrFileName)
{
    m_strFileName = crstrFileName;
}



//------------------------------------------------------------------*
/**
 * read file.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CGHFile::Read()
{
    CString strTmp;
    CFileStatus tStatus;
    if(!CFile::GetStatus(m_strFileName, tStatus))
    {
        return FALSE;
    }
    long    lSize = tStatus.m_size;

    CFile   tFile;
    try
    {
        tFile.Open(m_strFileName, CFile::modeRead);
        tFile.Read(strTmp.GetBuffer(lSize+1), lSize);
        strTmp.ReleaseBuffer();
        strTmp.SetAt(lSize, 0);
        tFile.Close();
    }
    catch(CFileException* pE)
    {
        pE->Delete();
        return FALSE;
    }

    // read data
    HRESULT hr;
    CString strFingerprintCalc;     // fingerprint data calculated from contents
    CString strDecrypted;           // decrypted data
    CString strFingerprintFile;     // fingerprint read from file

    hr = UTIL_DecryptString(strTmp, strDecrypted.GetBuffer(lSize), NULL);
    strDecrypted.ReleaseBuffer();

    if(hr != S_OK)
    {
        return FALSE;
    }

    // strip fingerprint data from end
    if(!StripLine(strTmp, strDecrypted, TRUE))
    {
        return FALSE;
    }
    strFingerprintFile = strTmp;

    // calculate 4gh fingerprint
    CString strDecryptedTmp = strDecrypted + _T("\r\n");    // use trailing \r\n
    hr = UTIL_GetStringFingerprint(strDecryptedTmp, strFingerprintCalc.GetBuffer(33));
    strFingerprintCalc.ReleaseBuffer();

    // compare calculated fingerprint data with file fingerprint data
    if(strFingerprintCalc.Compare(strFingerprintFile) != 0)
    {
        return FALSE;
    }

    // get password
    if(!StripLine(strTmp, strDecrypted))
    {
        return FALSE;
    }
    m_strPassword = strTmp;

    // get project guid
    if(!StripLine(strTmp, strDecrypted))
    {
        return FALSE;
    }
    m_strProjectGuid = strTmp;

    // get protected pous
    while(StripLine(strTmp, strDecrypted))
    {
        AddPouProtectData(strTmp);
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * write gh file.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CGHFile::Write()
{
    CString strTmp;

    if(m_strFileName.IsEmpty())
    {
        return FALSE;
    }

    // build up write string
    HRESULT             hr;
    CString             strFingerprintCalc;     // fingerprint data calculated from contents
    CString             strEncrypted;           // decrypted data
    CPouProtectData     tData;                  // pou protect data
    CString             strPouData;             // helper string for pou data

    strTmp += m_strPassword + _T("\r\n");
    strTmp += m_strProjectGuid + _T("\r\n");

    POSITION    pos = m_tPouProtectDataList.GetHeadPosition();
    while(pos)
    {
        tData = m_tPouProtectDataList.GetNext(pos);
        strPouData.Format("%s,%s\r\n", tData.m_strFileName, tData.m_strFingerprint);
        strTmp += strPouData;
    }

    hr = UTIL_GetStringFingerprint(strTmp, strFingerprintCalc.GetBuffer(33));
    strFingerprintCalc.ReleaseBuffer();
    ASSERT(hr == S_OK);
    if(hr != S_OK)
    {
        return FALSE;
    }

    strTmp += strFingerprintCalc;

    // encrytion
    long    lSize = strTmp.GetLength()*2+17;

    hr = UTIL_EncryptString(strTmp, strEncrypted.GetBuffer(lSize*2+17), NULL);
    strEncrypted.ReleaseBuffer();

    if(hr != S_OK)
    {
        return FALSE;
    }

    CFileStatus tStatus;
    if(CFile::GetStatus(m_strFileName, tStatus))
    {
        tStatus.m_attribute &= ~CFile::readOnly;
        CFile::SetStatus(m_strFileName, tStatus);
    }

    // remove old file if present
    if(!::DeleteFile(m_strFileName))
    {
        DWORD   dwLastError = ::GetLastError();
    }

    CFile   tFile;
    try
    {

        if(!tFile.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate))
        {
            return FALSE;
        }
        tFile.Write(strEncrypted.GetBuffer(0), strEncrypted.GetLength());
        strEncrypted.ReleaseBuffer();
        tFile.Close();

        // now make hidden and read only
        if(CFile::GetStatus(m_strFileName, tStatus))
        {
            tStatus.m_attribute |= CFile::hidden;
            tStatus.m_attribute |= CFile::readOnly;
            CFile::SetStatus(m_strFileName, tStatus);
        }
        else
        {
            ASSERT(0);
        }

    }
    catch(CFileException* pE)
    {
        pE->Delete();
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * strip line.
 *
 * @param           rstrLine: stripped line
 * @param           rstrRest: rest
 * @param           bFromEnd: search from end? (default FALSE)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CGHFile::StripLine(CString& rstrLine, CString& rstrRest, BOOL bFromEnd /*=FALSE*/)
{
    if(rstrRest.IsEmpty())
    {
        rstrLine.Empty();
        return FALSE;
    }

    int iFound;
    if(bFromEnd)
    {
        iFound = rstrRest.ReverseFind(_T('\r'));
        if(iFound < 0)
        {
            rstrLine = rstrRest;
            rstrRest.Empty();
            return TRUE;
        }

        rstrLine = rstrRest.Mid(iFound + 2);
        rstrRest = rstrRest.Left(iFound);
    }
    else
    {
        iFound = rstrRest.Find(_T('\r'));
        if(iFound < 0)
        {
            rstrLine = rstrRest;
            rstrRest.Empty();
            return TRUE;
        }

        rstrLine = rstrRest.Left(iFound);
        rstrRest = rstrRest.Mid(iFound + 2);
    }


    return TRUE;
}


//------------------------------------------------------------------*
/**
 * add pou protect data.
 *
 * @param           crstrLine: line from 4gh file
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CGHFile::AddPouProtectData(const CString& crstrLine)
{
    int iFound = crstrLine.Find(_T(','));
    if(iFound <= 0)
    {
        return FALSE;
    }
    AddPouProtectData(crstrLine.Left(iFound), crstrLine.Mid(iFound+1));
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * add pou protect data.
 *
 * @param           crstrFileName: file name of POU
 * @param           crstrFingerprint: fingerprint string of POU
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::AddPouProtectData(const CString& crstrFileName, const CString& crstrFingerprint)
{
    CPouProtectData    tData;

    tData.m_strFileName = crstrFileName;
    tData.m_strFingerprint = crstrFingerprint;

    m_tPouProtectDataList.AddTail(tData);
}


//------------------------------------------------------------------*
/**
 * set password.
 *
 * @param           crstrPassword: new password
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::SetPassword(const CString& crstrPassword)
{
    m_strPassword = crstrPassword;
}


//------------------------------------------------------------------*
/**
 * get password.
 *
 * @param           -
 * @return          password
 * @exception       -
 * @see             -
*/
CString CGHFile::GetPassword()
{
    return m_strPassword;
}


//------------------------------------------------------------------*
/**
 * set project guid.
 *
 * @param           crstrProjectGuid: new project guid
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::SetProjectGuid(const CString& crstrProjectGuid)
{
    m_strProjectGuid = crstrProjectGuid;
}


//------------------------------------------------------------------*
/**
 * get project guid.
 *
 * @param           -
 * @return          project guid
 * @exception       -
 * @see             -
*/
CString CGHFile::GetProjectGuid()
{
    return m_strProjectGuid;
}




//------------------------------------------------------------------*
/**
 * get first pou protection data position.
 *
 * @param           -
 * @return          position of first protect data in list.
 * @exception       -
 * @see             -
*/
POSITION CGHFile::GetFirstPouProtectDataPosition()
{
    return m_tPouProtectDataList.GetHeadPosition();
}


//------------------------------------------------------------------*
/**
 * get next pou protect data.
 *
 * @param           rtPos: position of pou protect data in list
 * @param           rtData: pou protect data
 * @return          -
 * @exception       -
 * @see             -
*/
void CGHFile::GetNextPouProtectData(POSITION& rtPos, CPouProtectData& rtData)
{
    rtData = m_tPouProtectDataList.GetNext(rtPos);
}

/* ---------------------------------------------------------------------------- */
