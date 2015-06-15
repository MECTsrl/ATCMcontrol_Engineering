
#ifndef GLOBVARCONSTLIST_H
#define GLOBVARCONSTLIST_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "Global.h"
#include <string>
#include "POU.h"

using namespace std;


//================================================================================
class GlobVarConstList : public POU
//================================================================================
{
public:
    GlobVarConstList();

    virtual void setFileSysComment(SysComment* fileSysComment);

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;
    void setPOUType(const string& POUType);

private:
    string m_POUType;
};

#endif
