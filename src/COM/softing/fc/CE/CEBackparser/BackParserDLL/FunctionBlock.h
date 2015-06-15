
#ifndef FUNCTIONBLOCK_H
#define FUNCTIONBLOCK_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "Global.h"
#include <string>
#include "POU.h"

using namespace std;


//================================================================================
class FunctionBlock : public POU
//================================================================================
{
public:
    
    FunctionBlock(const string& name,
                  const StringPosRange& nameRange,
                  StringPos lineNo,
                  StringPos nextVarSectInsertPos);

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;
};

#endif

