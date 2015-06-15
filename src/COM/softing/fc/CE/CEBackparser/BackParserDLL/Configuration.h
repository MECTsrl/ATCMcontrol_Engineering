#ifndef __CONFIGURATION__
#define __CONFIGURATION__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include "POU.h"

using namespace std;

//================================================================================
class Configuration : public POU
//================================================================================
{
public:
    
    Configuration(const string& name,
                  const StringPosRange& nameRange,
                  StringPos lineNo,
                  StringPos nextVarSectInsertPos);
    virtual ~Configuration();

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;

    void setTarget(const string& target,
                   const StringPosRange& targetRange);
    const string& getTarget() const;
    const StringPosRange& getTargetRange() const;

    void setProjectInfo(BackparserProjectInfo &);
private:
    string         m_target;
    StringPosRange m_target_range;
    string         m_strProjectName;
    string         m_strProjectPath;
};

#endif
