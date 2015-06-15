
#include "stdafx.h"

#include <atlbase.h>
#include "extern.h"

static ICEProjInfo *spICEProjectInfo = NULL;

//------------------------------------------------------------------*
/**
 * Function name    SetProjInfo
 * Description      sets *spICEProjectInfo
 * @param           ICEProjInfo *m_pICEProjectInfo
 * @return          -
 * @exception       -
 * @see             -
*/
void SetProjInfo(ICEProjInfo *pICEProjectInfo)
{
    spICEProjectInfo = pICEProjectInfo;
}

//------------------------------------------------------------------*
/**
 * Function name    GetInstallationPath
 * Description      returns the installation path
 * @return          CString
 * @exception       -
 * @see             -
*/
CString GetInstallationPath()
{
    if (spICEProjectInfo == NULL)
    {
        return CString("");
    }

    CComBSTR InstallationPath;
    HRESULT hr = spICEProjectInfo->getInstallationPath(&InstallationPath);
    if (hr == S_OK)
    {
        return CString(InstallationPath);
    }
    else
    {
        return CString("");
    }
}

//------------------------------------------------------------------*
/**
 * Function name    GetProjectPath
 * Description      returns the project path
 * @return          CString
 * @exception       -
 * @see             -
*/
CString GetProjectPath()
{
    if (spICEProjectInfo == NULL)
    {
        return CString("");
    }

    CComBSTR ProjectPath;
    HRESULT hr = spICEProjectInfo->getProjectPath(&ProjectPath);
    if (hr == S_OK)
    {
        return CString(ProjectPath) + _T("\\");
    }
    else
    {
        return CString("");
    }
}

//------------------------------------------------------------------*
/**
 * Function name    GetDocuPath
 * Description      returns the documentation path
 * @return          CString
 * @exception       -
 * @see             -
*/
CString GetDocuPath()
{

	CString ProjectPath = GetProjectPath();
	
    //OH: this is not needed -> CString DocuPath = ProjectPath + _T("Docu\\");
	return ProjectPath;
}

CString delimiter = _T("");

static TCHAR** Types = NULL;
static int TypesLength = 0;

static TCHAR** ProgClasses = NULL;
static int ProgClassesLength = 0;

void InitOutputArray(TCHAR ***Data, int *Length)
{
    *Data = new TCHAR*[1];
    *Data[0] = new TCHAR [delimiter.GetLength() + 1];
    _tcscpy(*Data[0], delimiter);
    *Length = 1;
}

class Deleter
{
public:
    ~Deleter() 
    { 
        del(&Types, &TypesLength);
        del(&ProgClasses, &ProgClassesLength);
    }

    static void del(TCHAR ***Data, int *Length)
    {
        if (*Data == NULL)
        {
            return;
        }
        for (int i = 0; i < *Length; i++)
        {
            delete (*Data)[i];
            (*Data)[i] = NULL;
        }
        delete *Data; 
        *Data = NULL;
        *Length = 0;
    }
};

static Deleter td;

//------------------------------------------------------------------*
/**
 * Function name    GetArrayFromProjInfo
 * Description      returns the result of a ProjInfo method
 *                  This result is converted from s SAFEARRAY to a TCHAR**
 *                  If parameter Data is already filled (=! NULL), 
 *                  the function returns without any operation.
 * @param           HRESULT ProjInfoMethod(SAFEARRAY**)
 * @param           TCHAR ***Data
 * @param           int *Length
 * @return          -
 * @exception       -
 * @see             -
*/
void GetArrayFromProjInfo(HRESULT ProjInfoMethod(SAFEARRAY**), TCHAR ***Data, int *Length)
{

    if (*Data != NULL)
    {
        return;
    }

    InitOutputArray(Data, Length);

    if (spICEProjectInfo == NULL)
    {
        return;
    }

    SAFEARRAY *array = NULL;
    HRESULT hr = ProjInfoMethod(&array);
    if (array == NULL)                  // 1)
    {
        return;               
    }

    if (hr != S_OK)
    {
        // array != 0 owing to 1)
        ::SafeArrayDestroy(array);    
        array = NULL;
        return;
    }

    long lLowerBound;
    long lUpperBound;
    ::SafeArrayGetLBound(array, 1, &lLowerBound);
    ::SafeArrayGetUBound(array, 1, &lUpperBound);

    if (lUpperBound < lLowerBound)
    {
        ::SafeArrayDestroy(array);    
        array = NULL;
        return;
    }

    Deleter::del(Data, Length);;

    *Length = lUpperBound - lLowerBound + 2;
    *Data = new TCHAR* [*Length];

    int i = 0;
    for (long l = lLowerBound; l <= lUpperBound; l++)
    {
        CComBSTR elem;
        hr = SafeArrayGetElement(array, &l, &elem);
        (*Data)[i] = new TCHAR [elem.Length() + 1];
        _tcscpy((*Data)[i], CString(elem.m_str));
        i++;
    }
    (*Data)[i] = new TCHAR [delimiter.GetLength() + 1];
    _tcscpy((*Data)[i], delimiter);

    ::SafeArrayDestroy(array);    
    array = NULL;
    return;
}

//------------------------------------------------------------------*
/**
 * Function name    GetStringArrayFromProjInfo
 * Description      returns the result of a ProjInfo method
 *                  This result is converted from s SAFEARRAY to a CStrinArray
 * @param           HRESULT ProjInfoMethod(SAFEARRAY**)
 * @param           CStringArray* pStringArray
 * @param           int *Length
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT GetStringArrayFromProjInfo(HRESULT ProjInfoMethod(SAFEARRAY**), CStringArray* pStringArray)
{
    if(spICEProjectInfo == NULL) {
        return(S_FALSE);
    }

    SAFEARRAY *pArray = NULL;
    HRESULT hr = ProjInfoMethod(&pArray);
    if(pArray == NULL) {
        return(S_FALSE);
    }

    if(hr != S_OK) {
        // array != 0 owing to 1)
        ::SafeArrayDestroy(pArray);    
        pArray = NULL;
        return(S_FALSE);
    }

    long lLowerBound;
    long lUpperBound;
    ::SafeArrayGetLBound(pArray, 1, &lLowerBound);
    ::SafeArrayGetUBound(pArray, 1, &lUpperBound);

    if(lUpperBound < lLowerBound) {
        ::SafeArrayDestroy(pArray);    
        pArray = NULL;
        return(S_FALSE);
    }

    for(long l = lLowerBound; l <= lUpperBound; l++) {
        CComBSTR elem;
        hr = SafeArrayGetElement(pArray, &l, &elem);

        if(hr == S_OK) {
            pStringArray->Add(CString(elem.m_str));
        }
    }
    ::SafeArrayDestroy(pArray);    
    pArray = NULL;
    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * Function name    ProjectInfoGetSupportedIECDataTypes
 * Description      calls spICEProjectInfo->getSupportedIECDataTypes
 * @param           SAFEARRAY** Array
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT ProjectInfoGetSupportedIECDataTypes(SAFEARRAY** Array)
{
    return spICEProjectInfo->getSupportedIECDataTypes(Array);
}

//------------------------------------------------------------------*
/**
 * Function name    GetSupportedBaseTypes
 * Description      returns the supported base types
 * @return          const TCHAR* const *
 * @exception       -
 * @see             -
*/
const TCHAR* const * GetSupportedBaseTypes()
{
    GetArrayFromProjInfo(ProjectInfoGetSupportedIECDataTypes, &Types, &TypesLength);
    return Types;
}

//------------------------------------------------------------------*
/**
 * Function name    ProjectInfoGetClassesOfType
 * Description      calls spICEProjectInfo->getClassesOfType
 * @param           SAFEARRAY** Array
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT ProjectInfoGetClassesOfType(SAFEARRAY** Array)
{
    return spICEProjectInfo->getClassesOfType(L"PROGRAM", Array);
}

//------------------------------------------------------------------*
/**
 * Function name    GetProgramClasses
 * Description      returns the program classes
 * @return          const TCHAR* const *
 * @exception       -
 * @see             -
*/
const TCHAR* const * GetProgramClasses()
{
    Deleter::del(&ProgClasses, &ProgClassesLength);;

    GetArrayFromProjInfo(ProjectInfoGetClassesOfType, &ProgClasses, &ProgClassesLength);

    // Delete file name extension by replacing "." with 0.
    for (int i = 0; i <= ProgClassesLength - 1; i++)
    {
        TCHAR *c = NULL;
        c = _tcsrchr(ProgClasses[i], '.');
        if (c != NULL)
        {
            *c = _T('\0');
        }
    }

    return ProgClasses;
}

//------------------------------------------------------------------*
/**
 * Function name    ProjectInfoGetSupportedIECLanguages
 * Description      calls spICEProjectInfo->getSupportedIECLanguages
 * @param           SAFEARRAY** Array
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT ProjectInfoGetSupportedIECLanguages(SAFEARRAY** Array)
{
    return spICEProjectInfo->getSupportedIECLanguages(Array);
}

//------------------------------------------------------------------*
/**
 * Function name    GetSupportedIECLanguages
 * Description      returns the supported iec languages
 * @return          CStringArray*
 * @exception       -
 * @see             -
*/
HRESULT GetSupportedIECLanguages(CStringArray* pStringArray)
{

    return(GetStringArrayFromProjInfo(ProjectInfoGetSupportedIECLanguages, pStringArray));
}

//------------------------------------------------------------------*
/**
 * Function name    IsRetainSupported
 * Description      returns true if retain is supported by any installed target
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL IsRetainSupported()
{
    if (spICEProjectInfo == NULL)
    {
        // If the interface spICEProjectInfo can not be used,
        // enable the user to choose retain.
        return TRUE;
    }

    BOOL RetainSupported = TRUE;
    HRESULT hr = spICEProjectInfo->getSupportRetain(&RetainSupported);
    if (hr != S_OK)
    {
        // If an error has occured, enable the user to choose retain.
        return TRUE;
    }
    else
    {
        return RetainSupported;
    }

    return TRUE;
}

