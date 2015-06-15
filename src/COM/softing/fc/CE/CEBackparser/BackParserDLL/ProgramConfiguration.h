
#ifndef __PROGRAMCONFIGURATION__
#define __PROGRAMCONFIGURATION__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
using namespace std;

#include "Global.h"
#include "IECSRCVisitor.h"
#include "StringPosRange.h"
#include "SysComment.h"

//================================================================================
class ProgramConfiguration
//================================================================================
{
public:
    ProgramConfiguration(const string& name, StringPosRange nameRange);
    virtual ~ProgramConfiguration();
    
    void setEndPos(StringPos end);

    virtual void invite(const IECSRCVisitor& visitor,
                        int withTask = 0) const;

    void setStart(StringPos start);
    void setEnd(StringPos end);
    BACKPARSERDLLEXPORT StringPosRange getRange() const;
    BACKPARSERDLLEXPORT string getName() const;
    BACKPARSERDLLEXPORT StringPosRange getNameRange() const;
    BACKPARSERDLLEXPORT string getTaskName() const;
    BACKPARSERDLLEXPORT StringPosRange getTaskNameRange() const;
    BACKPARSERDLLEXPORT string getProgramClass() const;
    BACKPARSERDLLEXPORT StringPosRange getProgramClassRange() const;
    void setTask(const string& taskName, StringPosRange taskNameRange);
    void setProgramClass(const string& programClass, StringPosRange programClassRange);
    void SetSysComment(SysComment *comment);
    BACKPARSERDLLEXPORT virtual const SysComment* GetFileSystemComment() const;

protected:
    StringPosRange m_range;
    string m_name;
    StringPosRange m_nameRange;
    string m_taskName;
    StringPosRange m_taskNameRange;
    string m_programClass;
    StringPosRange m_programClassRange;
    SysComment *m_sys_cmnt;
};

#endif

