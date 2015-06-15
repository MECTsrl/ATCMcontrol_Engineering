
#if !defined __DIALOGGENBASE__
#define __DIALOGGENBASE__

#pragma warning(disable: 4355)

#include "ObjPropGenBase.h"

class CDialogGenBase  
{
public:
	CDialogGenBase(CObjPropGenBase &ObjPropGenBase);
	virtual ~CDialogGenBase();

	DLLEXPORT void SetName(CString Name);
	DLLEXPORT CString GetName();
	DLLEXPORT void SetAlias(CString Alias);
	DLLEXPORT CString GetAlias();
	DLLEXPORT void SetCreateDate(CString CreateDate);
	DLLEXPORT void SetCreateUser(CString CreateUser);
	DLLEXPORT void SetLastModDate(CString LastModDate);
	DLLEXPORT void SetLastModUser(CString LastModUser);

	CObjPropGenBase &m_ObjPropGenBase;
};

#endif
