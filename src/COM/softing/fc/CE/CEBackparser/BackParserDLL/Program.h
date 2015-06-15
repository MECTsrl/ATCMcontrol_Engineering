
#ifndef PROGRAM_H
#define PROGRAM_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "Global.h"
#include <string>
#include "POU.h"

using namespace std;


//================================================================================
class Program : public POU
//================================================================================
{
public:
    
    Program(const string& name,
            const StringPosRange& nameRange,
            StringPos lineNo,
            StringPos nextVarSectInsertPos);

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;
};

#endif

