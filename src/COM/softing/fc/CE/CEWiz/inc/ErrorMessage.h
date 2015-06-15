#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

typedef struct {
    int iResourceId;
} TErrorInfo;

void ErrorMessage(int errorCode, ...);

#define ERROR_MESSAGE_MAX_LENGTH 5000

// implements the function to display error messages
#define ERROR_MESSAGE_FUNC_IMPL                                               \
    void ErrorMessage(int errorCode, ...)                              \
    {                                                                         \
        CWinApp *pApp;                                                        \
        CWnd *pWnd;                                                           \
        va_list argList;                                                      \
        CString formatString;                                                 \
        TCHAR cMessage[ERROR_MESSAGE_MAX_LENGTH];                             \
                                                                              \
        ASSERT((sizeof(errorInfo)/sizeof(errorInfo[0])) == EC_MAX);           \
        va_start(argList,errorCode);                                          \
        formatString.LoadString(errorInfo[errorCode].iResourceId);            \
        _vstprintf(cMessage,formatString,argList);                            \
        pApp = AfxGetApp();                                                   \
        pWnd = AfxGetMainWnd();                                               \
        ::MessageBox(pWnd->GetSafeHwnd(),cMessage,pApp->m_pszAppName,MB_OK);  \
        va_end(argList);                                                      \
    }

#endif
