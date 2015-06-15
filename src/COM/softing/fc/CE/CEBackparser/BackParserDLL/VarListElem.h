
#ifndef __VARLISTELEM__
#define __VARLISTELEM__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>

using namespace std;

#include "Global.h"
#include "StringPosRange.h"
#include "SysComment.h"

//================================================================================
class VarListElem
//================================================================================
{
public:
    VarListElem(const string& n, const StringPos start);
    virtual ~VarListElem();

    BACKPARSERDLLEXPORT virtual const string& getName() const;
    BACKPARSERDLLEXPORT virtual const StringPosRange& getNameRange() const;

    BACKPARSERDLLEXPORT virtual const string& getAddress() const;
    BACKPARSERDLLEXPORT virtual const StringPosRange& getAddressRange() const;

    BACKPARSERDLLEXPORT virtual const SysComment* getSystemComment() const;
    
    string         name;
    StringPosRange range; // from first of name to last of terminator
                          // terminator may be ","(COMMA) or ":"(COLON) or "AT"
    string         address;
    StringPosRange addressRange; 

    const SysComment *syscomment; 
};

#endif

