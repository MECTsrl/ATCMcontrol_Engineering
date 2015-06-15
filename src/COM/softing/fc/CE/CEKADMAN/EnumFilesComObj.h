// EnumFilesComObj.h : Declaration of the CEnumFilesComObj

#ifndef __ENUMFILESCOMOBJ_H_
#define __ENUMFILESCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "FileComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumFilesComObj

typedef TCEnumComObj<IEnumFiles,IFile,CFileComObj>
    CEnumFilesComObj;

#endif //__ENUMFILESCOMOBJ_H_
