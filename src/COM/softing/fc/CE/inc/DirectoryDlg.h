#ifndef __DIRECTORYDLG_H_
#define __DIRECTORYDLG_H_

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
 *  class CDirectoryDlg
 *  
 *  directory selection dialog.
 *  uses modeless ::SHBrowseForFolder()
 *  
 *  1. create CDirectory on the stack
 *      modal behaviour of the dialog can be obtained by passing
 *      the CWnd pointer of the owner window to the constructor.
 *      This window is disabled in the DoModal function before calling
 *      SHBrowseForFolder() and enabled afterwards.
 *  2. call SetInitialDir() to set the starting directory of the dialog
 *      if you dont, you start at the current directory
 *  3. call DoModal()
 *      the return values are as usual (IDOK, IDCANCEL)
 *  4. call GetDir() to get the selected folder
 *
 */
class CDirectoryDlg
{
public:
    CDirectoryDlg();

    virtual ~CDirectoryDlg();

    void SetInitialDir(const CString& crstrInitialDir);

    virtual int DoModal();

    CString GetDir() const;

protected:
    CString     m_strInitialDir;    // initial directory
    CString     m_strDirectory;     // selected directory
};

#endif // __DIRECTORYDLG_H_
