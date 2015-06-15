
#if !defined __DLLINTERFACE__
#define __DLLINTERFACE__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>

#include "Global.h"
#include "POU.h"
#include "FunctionBlock.h"
#include "Function.h"
#include "StResource.h"
#include "Configuration.h"
#include "GlobVarConstList.h"
#include "StStruct.h"

BACKPARSERDLLEXPORT void doBackparse(string AbsPathFileName, string& XmlText,BackparserProjectInfo & info);
BACKPARSERDLLEXPORT string parse_string(const string& buf, Resource** res);
BACKPARSERDLLEXPORT string parse_string(const string& buf, Configuration** con);
BACKPARSERDLLEXPORT string parse_string(const string& buf, FunctionBlock** fb);
BACKPARSERDLLEXPORT string parse_string(const string& buf, Function** f);
BACKPARSERDLLEXPORT string parse_string(const string& buf, POU** pou);
BACKPARSERDLLEXPORT string parse_string(const string& buf, GlobVarConstList** gvcl);
BACKPARSERDLLEXPORT string parse_string(const string& buf, StStruct** st);

BACKPARSERDLLEXPORT HRESULT ExtractHeader(const string& SrcFile, string& HeaderFile);
BACKPARSERDLLEXPORT HRESULT MergeHeader(const string& MergeFile, const string& HeaderFile, BOOL DelHeaderFile);

BACKPARSERDLLEXPORT HRESULT GetDates(const string &FileName, string& CreateDate, string& ModificationDate);

string getFileBaseName(const string& path);
string getFileExtension(const string& path);
void readFileContent(const string& fileName, string& buf);
void GetLanguage(const string &FileName, string& Language);

#endif
