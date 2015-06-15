
#ifndef DEBUG_H
#define DEBUG_H

/* SU: Do not use special ASSERT routines here, rely on ATLASSERT.
   Also do not use allocation check routines - CrtDbg* methods are
   much more precise here. Faciltate CrtDbg* methods as appropriate.
   For a simple switch of the ASSERT mechanism create a private define
   and only adjust that define.
*/
#ifndef KADMANAGER_ASSERT
#define KADMANAGER_ASSERT(x) ATLASSERT((x))
#endif

//#define DEBUG_ACTIVE

#ifdef DEBUG_ACTIVE

/*
#define ASSERT(condition,message)									      \
    if (! (condition)) {                                                  \
        TCHAR sMessage[1000];                                             \
        _stprintf(sMessage,                                               \
            _T("File: %s\r\n")                                            \
            _T("Line: %d\r\n")                                            \
            _T("Condition: %s\r\n")                                       \
            _T("Message: %s\r\n\r\n")                                     \
            _T("Do you wish to continue?"),                               \
            __FILE__,__LINE__,_T(#condition),_T(message));                \
        if (::MessageBox(NULL,sMessage,_T("Assertion failed"),            \
                MB_YESNO) == IDNO) {                                      \
            exit(-1);                                                     \
        }                                                                 \
	}
*/

class CAllocCheck {
    private:
        static int allocCounter;
    public:
        CAllocCheck();
        ~CAllocCheck();
};

#define ALLOC_CHECK                                                   \
    private:                                                          \
        CAllocCheck m_allocCheck;

#else 
#define ALLOC_CHECK
//#define ASSERT(condition,message) see above.
#endif
#endif
