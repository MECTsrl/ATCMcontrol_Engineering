
#ifndef __EMPTYFILE__
#define __EMPTYFILE__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "Global.h"
#include <string>
#include "POU.h"

using namespace std;


//================================================================================
class EmptyFile : public POU
//================================================================================
{
public:
    EmptyFile();

    virtual void setFileSysComment(SysComment* fileSysComment);

    BACKPARSERDLLEXPORT virtual string getPOUType() const;

    virtual void invite(const IECSRCVisitor& visitor) const;
    void setPOUType(const string& POUType);

private:
    string m_POUType;
};

#endif
