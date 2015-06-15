#ifndef __SIZEDDIALOGBAR_H_
#define __SIZEDDIALOGBAR_H_

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
 *  class CSizedDialogBar
 *
 *  virtual base class for docking tools derived from SECDialogBar
 *  used to recover last size
 *  since order of creation of docking tools is not determined,
 *  identical size and position of windows can not be guaranteed.
 *
 *  - load sizing data from registry with LoadSizeData()
 *  - save sizing data to registry with SaveSizeData()
 *  - registry path must be set before using these methods
 *  - GetStandardDockingHeight() is used to set first docking height
 *
*/
class CSizedDialogBar : public SECDialogBar
{
    DECLARE_DYNCREATE(CSizedDialogBar)

public:

    CSizedDialogBar();
    virtual ~CSizedDialogBar();

    // get current docking height
    int GetDockingHeight();

    // save sizing data in profile
    void SaveSizeData();
    
    // load sizing data from profile
    void LoadSizeData();

	virtual void OnBarEndDock();
   	virtual void OnBarEndFloat();
	virtual void OnBarEndMDIFloat();

 
protected:
    // get standard docking height
    virtual int GetStandardDockingHeight();


protected:
    CString m_strRegKey;        // registry key where to save size data
    int     m_nHeight;          // height resp. width of dialog bar


};


#endif // __SIZEDDIALOGBAR_H_