
#if !defined __PROPERTYSHEETBASE__
#define __PROPERTYSHEETBASE__

#include <afxdlgs.h>

#include "DialogType.h"
#include "GlobConst.h"
//#include "Operation.h"

class COperation;

class CPropertySheetBase : public CPropertySheet
{
public:
	CPropertySheetBase(DIALOGTYPE DialogType, LPCTSTR pszCaption);
	virtual ~CPropertySheetBase();

    DLLEXPORT void SetOperation(COperation *Operation);
    DLLEXPORT int DoModal();
    BOOL Execute();
    virtual BOOL OnInitDialog();
    void SetApplied();
    void ResetApplied();
    BOOL HasBeenApplied();
    BOOL IsReadOnly();


//private:
    COperation *m_pOperation;
    BOOL m_Applied;
    BOOL m_bGenerateHelp;//Used by Help and Info
};

#endif

