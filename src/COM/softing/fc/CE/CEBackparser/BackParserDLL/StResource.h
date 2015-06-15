#ifndef __STRESOURCE__
#define __STRESOURCE__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include "POU.h"

using namespace std;

//================================================================================
class Resource : public POU
//================================================================================
{
public:
    
    Resource(const string& name,
             const StringPosRange& nameRange,
             StringPos lineNo,
             StringPos nextVarSectInsertPos);
    virtual ~Resource();

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;

    void setTarget(const string& target,
                   const StringPosRange& targetRange);
    BACKPARSERDLLEXPORT const string& getTarget() const;
    BACKPARSERDLLEXPORT const StringPosRange& getTargetRange() const;

private:
    string         m_target;
    StringPosRange m_target_range;
};

#endif
