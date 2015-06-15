
#if !defined __DIALOGGENVARBASE__
#define __DIALOGGENVARBASE__

#include "DialogGenBase.h"
#include "ObjPropGenVarBase.h"
#include "GlobConst.h"

class CDialogGenVarBase : public CDialogGenBase  
{
public:
   	CDialogGenVarBase(CObjPropGenVarBase &ObjPropGenVarBase);
	virtual ~CDialogGenVarBase();

//   	DLLEXPORT void SetTypes(CStringArray &Type);
   	DLLEXPORT void SetType(CString Type);
	DLLEXPORT CString GetType();
   	DLLEXPORT void SetValue(CString Value);
	DLLEXPORT CString GetValue();
   	DLLEXPORT void SetVarComment(CString VarComment);
	DLLEXPORT CString GetVarComment();

protected:
	CObjPropGenVarBase &m_ObjPropGenVarBase;
};

#endif

