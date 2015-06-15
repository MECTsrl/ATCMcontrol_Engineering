
#ifndef __STRINGPOSRANGE__
#define __STRINGPOSRANGE__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include "Global.h"

using namespace std;


typedef string::size_type StringPos;

//================================================================================
class StringPosRange : protected pair<StringPos, StringPos>
//================================================================================
{
public:
    BACKPARSERDLLEXPORT StringPosRange(StringPos f = string::npos, StringPos l = string::npos);
 
    BACKPARSERDLLEXPORT StringPos getFirst() const;
    BACKPARSERDLLEXPORT StringPos getLast()  const;
    BACKPARSERDLLEXPORT StringPos getLength();                          
    void setFirst(StringPos f);
    void setLast(StringPos l);
    BACKPARSERDLLEXPORT bool isValid() const;
    bool isInvalid() const;
    string getSubstr(const string& s) const;
    ostream& print(ostream& out) const;
};

ostream& operator<<(ostream& out, const StringPosRange& r);

#endif
