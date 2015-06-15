
#if !defined __GLOBCONST__
#define __GLOBCONST__

#if defined _ADDONOPSDIALOGS
#define DLLEXPORT   __declspec( dllexport )
#else
#define DLLEXPORT
#endif


#define CREATE_DELETE_DECL(TYPE)                                \
DLLEXPORT static TYPE* CreateNewObject(DIALOGTYPE DialogType);  \
DLLEXPORT static void DeleteObject(TYPE *Object);               \


#define CREATE_DELETE_IMPL(TYPE)                        \
TYPE* TYPE::CreateNewObject(DIALOGTYPE DialogType)      \
{                                                       \
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));       \
    TYPE *Object;                                       \
    Object = new TYPE(DialogType);                      \
    return Object;                                      \
}                                                       \
                                                        \
void TYPE::DeleteObject(TYPE *Object)                   \
{                                                       \
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));       \
    delete Object;                                      \
}


#endif

