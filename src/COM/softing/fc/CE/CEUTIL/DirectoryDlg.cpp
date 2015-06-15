
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DirectoryDlg.h"
#include "shlobj.h"
#include "cemain.h"
#include "utilif.h"

//----  Static Initializations:   ----------------------------------*


CDirectoryDlg::CDirectoryDlg()
{
}

CDirectoryDlg::~CDirectoryDlg()
{
}

void CDirectoryDlg::SetInitialDir(const CString& crstrInitialDir)
{
    m_strInitialDir = crstrInitialDir;
}


int CALLBACK BrowsePathCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
    switch(uMsg) 
    {
        case BFFM_INITIALIZED: 
        {
            LPTSTR pszDir = (LPTSTR)pData;
            SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)pszDir);
            break;
        }
        default:
            break;
    }

    return 0;
}

int CDirectoryDlg::DoModal()
{
    LPMALLOC        pMalloc;
    BROWSEINFO      bi;
    LPITEMIDLIST    pidl;
    LPSHELLFOLDER   pDesktopFolder;
    TCHAR           szInitialPath[MAX_PATH];
    OLECHAR         oleInitialPath[MAX_PATH];
    ULONG           chEaten;
    ULONG           dwAttributes;
    HRESULT         hr;
    CFileStatus     FileStatus;

    lstrcpy(szInitialPath, m_strInitialDir);

    /*  IMPORTANT NOTE: Do not forget that the path may not be existent,
        SHBrowseForFolder reacts very vexing to this fact. So assure that the
        path is valid and exists.
    */

    
    if(!CFile::GetStatus(m_strInitialDir, FileStatus)) 
    {
        ::GetCurrentDirectory(MAX_PATH, m_strInitialDir.GetBuffer(MAX_PATH+1));
        m_strInitialDir.ReleaseBuffer();
        lstrcpy(szInitialPath, m_strInitialDir);
    }
    

    //
    // Get a pointer to the Desktop's IShellFolder interface.
    //
    hr = SHGetDesktopFolder(&pDesktopFolder);

    if (FAILED(hr))
    {
        UTIL_DisplaySysError(hr, NULL, NULL);
        return IDABORT;
    }

    //
    // IShellFolder::ParseDisplayName requires the file name be in
    // Unicode.
    //
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szInitialPath, -1,
                        oleInitialPath, MAX_PATH);

    //
    // Convert the path to an ITEMIDLIST.
    //
    hr = pDesktopFolder->ParseDisplayName(NULL,
                                          NULL,
                                          oleInitialPath,
                                          &chEaten,
                                          &pidl,
                                          &dwAttributes);

    //release the desktop folder object
    pDesktopFolder->Release();

    if (FAILED(hr))
    {
        UTIL_DisplaySysError(hr, NULL, NULL);
        return IDABORT;
    }

    //
    // pidl now contains a pointer to an ITEMIDLIST for .\readme.txt.
    // This ITEMIDLIST needs to be freed using the IMalloc allocator
    // returned from SHGetMalloc().
    //
    hr = SHGetMalloc(&pMalloc);

    if(FAILED(hr)) 
    {
        UTIL_DisplaySysError(hr, NULL, NULL);
        return IDABORT;
    }

    HWND    hWndTop;
    HWND    hWndOwner = CWnd::GetSafeOwner_(NULL, &hWndTop);

    ZeroMemory(&bi,sizeof(bi));
    bi.hwndOwner = hWndOwner;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = NULL;
    bi.lpszTitle = "";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_VALIDATE;
    bi.lpfn = BrowsePathCallbackProc;
    bi.lParam = (LPARAM)szInitialPath;

    pidl = SHBrowseForFolder(&bi);

	if (hWndTop != NULL)
    {
        EnableWindow(hWndTop, TRUE);
    }

    if (pidl)
    {
        SHGetPathFromIDList(pidl, szInitialPath);
        pMalloc->Free(pidl);
        pMalloc->Release();
        m_strDirectory = szInitialPath;
    }
    if(m_strDirectory.IsEmpty())
    {
        return IDCANCEL;
    }
    return IDOK;
}


CString CDirectoryDlg::GetDir() const
{
    return m_strDirectory;
}


