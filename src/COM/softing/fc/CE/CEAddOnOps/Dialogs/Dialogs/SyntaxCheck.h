
#if !defined __SYNTAXCHECK__
#define __SYNTAXCHECK__

BOOL IsIdentifier(CString String);
BOOL CheckTypeOfLiteral(CString Type, CString Literal, CString &ErrorMessage);
BOOL CheckString(CString Value, CString Datatype, CString &ErrorMessage);

#endif

