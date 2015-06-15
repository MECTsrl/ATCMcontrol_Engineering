#ifndef __KADTARGETINFO_H_
#define __KADTARGETINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CEKadMan.h"

//----  Forward Class Definitions:   -------------------------------*
class CKADTargetData;
//----  Defines:    ------------------------------------------------*
typedef CMap<CString, LPCTSTR, CKADTargetData*, CKADTargetData*>    T_TargetDataMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CKADTargetInfo
{
public:
    CKADTargetInfo();
    ~CKADTargetInfo();

    BOOL Load(CComPtr<IUnknown>& pIKadMan);

    CKADTargetData* GetTargetDataForType(const CString& crstrType);

protected:
    BOOL LoadTargetData(CComPtr<ITarget>& pITarget);
    BOOL LoadTargetOnlineData(CComPtr<ITargetOnline>& pITargetOnline, CKADTargetData* pTargetData);

protected:
    T_TargetDataMap     m_tTargetMap;   // maps target type to target data object
};

#endif // __KADTARGETINFO_H_

