/////////////////////////////////////////////////////////////////////////////
// FileStream.h: SFL IStream implementation on a standard file
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       
// Description:  IStream implementation on a standard file
//


#pragma once


#include <objidl.h>


namespace stingray {
	namespace foundation {


/* IDL representation
[
	object,
	uuid(54A1C3E0-8A17-11d2-B34D-006008AFB3D7),
	pointer_default(unique)
]
interface IFileStream: IUnknown
{
	HRESULT Open([in, string] LPOLESTR strFileName, [in, defaultvalue(0)] BOOL bCreate);
	HRESULT Close();
};
*/

#define IID_IFileStream __uuidof(stingray::foundation::IFileStream)

interface __declspec(uuid("54A1C3E0-8A17-11d2-B34D-006008AFB3D7")) IFileStream : 
	public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Open( 
		/* [string][in] */ LPOLESTR strFileName,
		/* [defaultvalue][in] */ BOOL bCreate = 0) = 0;

	virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;

};



/////////////////////////////////////////////////////////////////////////////
// CFileStream

class ATL_NO_VTABLE CFileStream :
		public CComObjectRootEx<CComSingleThreadModel>,
		public IFileStream,
		public IStream
{
protected:
	HANDLE m_hFile;


public:
	CFileStream() : m_hFile(0){}
	virtual ~CFileStream(){Close();}

	BEGIN_COM_MAP(CFileStream)
	  COM_INTERFACE_ENTRY(IFileStream)
	  COM_INTERFACE_ENTRY(IStream)
	END_COM_MAP()

// IFileStream
	STDMETHODIMP Open( 
            /* [string][in] */ LPOLESTR strFileName,
            /* [defaultvalue][in] */ BOOL bCreate = 0)
	{
		ATLASSERT(strFileName);
		USES_CONVERSION;
		return OpenFile(OLE2T(strFileName), bCreate) ? S_OK : E_FAIL;
	}

	STDMETHODIMP Close(void)
	{
		return CloseFile() ? S_OK : E_FAIL;		
	}
	

// IStream
	STDMETHODIMP Read(void *pv, ULONG cb, ULONG *pcbRead)
	{
		if (m_hFile == NULL) return E_FAIL;
		if (!pv) return E_POINTER;
		if (cb == 0) return S_OK;	// Don't bother reading 0 bytes from disk

		HRESULT hr = S_OK;

		// Read the data.
		unsigned long nRead = 0;
		try {
			if (!::ReadFile(m_hFile, pv, cb, &nRead, NULL)) {
				hr = S_FALSE;
			}
		}
		catch (...)
		{
			hr = E_UNEXPECTED;
		}

		// Return the number of bytes read.
		if (pcbRead != NULL)
			*pcbRead = nRead;

		return hr;
	}

	STDMETHODIMP Write(const void *pv, ULONG cb, ULONG *pcbWritten)
	{
		if (m_hFile == NULL) return E_FAIL;
		if (!pv) return E_POINTER;
		if (cb == 0) return S_OK;	// Don't bother reading 0 bytes from disk

		HRESULT hr = S_OK;

		// Write the data.
		unsigned long nWritten = 0;
		try {
			if (!::WriteFile(m_hFile, pv, cb, &nWritten, NULL)) {
				hr = STG_E_CANTSAVE;
			}

			// Win32s will not return an error all the time (usually DISK_FULL)
			if (nWritten != cb)
				hr = STG_E_MEDIUMFULL;
		}
		catch (...)	{
			hr = E_UNEXPECTED;
		}

		// Return the number of bytes written.
		if (pcbWritten != NULL)
			*pcbWritten = nWritten;

		return hr;
	}


	STDMETHODIMP Seek(LARGE_INTEGER uliOffset, DWORD dwOrigin, ULARGE_INTEGER* puliNew)
	{
		if (m_hFile == NULL) return E_FAIL;

		HRESULT hr = S_OK;

		// Do the seek.
		LARGE_INTEGER liNew = uliOffset;
		try {
			DWORD dwFrom = 0L;
			switch (dwOrigin) {
				case STREAM_SEEK_SET:
					dwFrom = FILE_BEGIN;
					break;
				case STREAM_SEEK_CUR:
					dwFrom = FILE_CURRENT;
					break;
				case STREAM_SEEK_END:
					dwFrom = FILE_END;
					break;
				default:
					hr = STG_E_INVALIDFUNCTION;
			}

			if (hr == S_OK) {
				liNew.LowPart = ::SetFilePointer(m_hFile, liNew.LowPart, &(liNew.HighPart), dwFrom);
			}

			if (liNew.LowPart == (DWORD)-1)
				hr = STG_E_INVALIDPOINTER;
		}
		catch (...) {
			hr = E_UNEXPECTED;
		}

		// Return the new position.
		if (puliNew) puliNew->QuadPart = liNew.QuadPart;

		return hr;
	}

	STDMETHODIMP SetSize(ULARGE_INTEGER uliSize)
	{
		if (m_hFile == NULL) return E_FAIL;
		HRESULT hr = S_OK;

		LARGE_INTEGER liPos;
		liPos.QuadPart = uliSize.QuadPart;

		// Do the set length.
		hr = Seek(liPos, STREAM_SEEK_SET, 0);

		if (SUCCEEDED(hr)) {
			if (!::SetEndOfFile(m_hFile)) hr = E_FAIL;
		}

		return hr;
	}


	STDMETHODIMP CopyTo(LPSTREAM pStm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead,
						ULARGE_INTEGER* pcbWritten)
	{
		if (m_hFile == NULL) return E_FAIL;

		HRESULT hr = S_OK;

		// If cb is max, copy to end of stream.
		DWORD dwLen = cb.LowPart;
		if (cb.HighPart == (DWORD)-1 && cb.LowPart == (DWORD)-1)
			dwLen = Length();
		else if (cb.HighPart != 0)
			return E_NOTIMPL;

		// Copy the data.
		BYTE* pb = NULL;
		DWORD dwRead = 0L;
		try {
			BYTE* pb = new BYTE[dwLen];
			hr = Read(pb, dwLen, &dwRead);
			if (SUCCEEDED(hr)) {
				pStm->Write(pb, dwRead, 0);
			}
		}
		catch (...) {
			delete [] pb;
		}
		delete [] pb;

		// Return read and write sizes.
		if (pcbRead)
			ULISet32(*pcbRead, dwRead);

		if (pcbWritten)
			ULISet32(*pcbWritten, dwRead);

		return hr;
	}

	STDMETHODIMP Commit(DWORD grfCommitFlags)
	{
		if (m_hFile == NULL) return E_FAIL;
		ATLASSERT(grfCommitFlags == STGC_DEFAULT);
		if ( !::FlushFileBuffers(m_hFile) )
			return STG_E_MEDIUMFULL;
		return S_OK;
	}

	STDMETHODIMP Revert()
	{
		if (m_hFile == NULL) return E_FAIL;
		return E_NOTIMPL;
	}


	STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
							ULARGE_INTEGER cb,
							DWORD dwLockType)
	{
		if (m_hFile == NULL) return E_FAIL;
		ATLASSERT(dwLockType == LOCK_EXCLUSIVE);
		if ( !::LockFile(m_hFile, libOffset.LowPart, 0, cb.LowPart, 0) )
			return HRESULT_FROM_WIN32(GetLastError());
		return S_OK;
	}


	STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
								ULARGE_INTEGER cb,
								DWORD dwLockType)
	{
		if (m_hFile == NULL) return E_FAIL;
		ATLASSERT(dwLockType==LOCK_EXCLUSIVE);
		if ( !::UnlockFile(m_hFile, libOffset.LowPart, 0, cb.LowPart, 0) )
			return HRESULT_FROM_WIN32(GetLastError());
		return S_OK;
	}

	STDMETHODIMP Stat(STATSTG *pstatstg, DWORD grfStatFlag)
	{
		pstatstg;
		grfStatFlag;

		if (m_hFile == NULL) return E_FAIL;
		return E_NOTIMPL;
	}


	STDMETHODIMP Clone(LPSTREAM*)
	{
		if (m_hFile == NULL) return E_FAIL;
		return E_NOTIMPL;
	}

protected:
	unsigned long Length()
	{
		ULARGE_INTEGER dwLen;
		LARGE_INTEGER dwCur;
		LARGE_INTEGER dwOrg; dwOrg.QuadPart = 0;
		Seek(dwOrg, STREAM_SEEK_CUR, reinterpret_cast<ULARGE_INTEGER*>(&dwCur));
		Seek(dwOrg, STREAM_SEEK_END, &dwLen);
		Seek(dwCur, STREAM_SEEK_SET, 0);
		return dwLen.LowPart;
	}

	BOOL OpenFile(LPCTSTR lpszFileName, BOOL bCreate)
	{
		if (m_hFile != NULL) CloseFile();

		DWORD dwAccess = GENERIC_READ|GENERIC_WRITE;
		DWORD dwShareMode = 0;	// Exclusive

		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = FALSE;

		// map creation flags
		DWORD dwCreateFlag;
		if (bCreate)
			dwCreateFlag = CREATE_ALWAYS;
		else
			dwCreateFlag = OPEN_EXISTING;
		
		// attempt file creation
		HANDLE hFile = ::CreateFile(lpszFileName, dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) return FALSE;

		m_hFile = hFile;
		return TRUE;
	}


	BOOL CloseFile()
	{
		BOOL bRet = TRUE;
		if (m_hFile != NULL)
		{
			bRet = ::CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		return bRet;
	}

};


};	// namespace stingray::toolkit
};	// namespace stingray
