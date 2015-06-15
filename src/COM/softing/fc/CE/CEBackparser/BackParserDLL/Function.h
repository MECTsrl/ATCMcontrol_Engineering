
#ifndef FUNCTION_H
#define FUNCTION_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "Global.h"
#include <string>
#include "POU.h"

using namespace std;


//================================================================================
class Function : public POU
//================================================================================
{
public:
    
    Function(const string& name,
             const StringPosRange& nameRange,
             StringPos lineNo,
             StringPos nextVarSectInsertPos);

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;

    void setFunctionType(const string& functionType);
    BACKPARSERDLLEXPORT virtual const string& getFunctionType() const;
    void setFunctionTypeRange(const StringPosRange functionTypeRange);
    BACKPARSERDLLEXPORT virtual const StringPosRange& getFunctionTypeRange() const;

protected:
    string                          m_function_type;
    StringPosRange                  m_function_type_range;
};

#endif

