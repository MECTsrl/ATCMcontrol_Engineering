#ifndef __TASKCONFIGURATION__
#define __TASKCONFIGURATION__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include "Global.h"

using namespace std;
#include "stdafx.h"
#include "IECSRCVisitor.h"
#include "StringPosRange.h"
#include "SysComment.h"

//================================================================================
class TaskConfiguration 
//================================================================================
{
public:
    TaskConfiguration(const string& Name, StringPosRange nameRange);
    virtual ~TaskConfiguration();
    
    void setEndPos(StringPos end);

    virtual void invite(const IECSRCVisitor& visitor) const;

    BACKPARSERDLLEXPORT string getName() const;
    BACKPARSERDLLEXPORT StringPosRange getNameRange() const;

    void setStart(StringPos start);
    void setEnd(StringPos end);
    BACKPARSERDLLEXPORT StringPosRange getRange() const;

    void SetSysComment(SysComment *comment);
    BACKPARSERDLLEXPORT virtual const SysComment* GetFileSystemComment() const;

    void setInterval(const string& value);
    BACKPARSERDLLEXPORT string getInterval() const;
    void setIntervalRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getIntervalRange() const;

    void setPriority(const string& value);
    BACKPARSERDLLEXPORT string getPriority() const;
    void setPriorityRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getPriorityRange() const;

    void setAutostart(const string& value);
    BACKPARSERDLLEXPORT string getAutostart() const;
    void setAutostartRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getAutostartRange() const;

	// NFTASKHANDLING 19.05.05 SIS >>
    void setLoaded(const string& value);
    BACKPARSERDLLEXPORT string getLoaded() const;
    void setLoadedRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getLoadedRange() const;
	// NFTASKHANDLING 19.05.05 SIS <<

    void setIosync(const string& value);
    BACKPARSERDLLEXPORT string getIosync() const;
    void setIosyncRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getIosyncRange() const;

    void setFftask(const string& value);
    BACKPARSERDLLEXPORT string getFftask() const;
    void setFftaskRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getFftaskRange() const;

    // SIS: event driven tasks >>
    void setEvent(const string& value);
    BACKPARSERDLLEXPORT string getEvent() const;
    void setEventRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getEventRange() const;
    // SIS: event driven tasks <<

    void setWatchdog(const string& value);
    BACKPARSERDLLEXPORT string getWatchdog() const;
    void setWatchdogRange(StringPosRange spr);
    BACKPARSERDLLEXPORT StringPosRange getWatchdogRange() const;

    void setInitialisationListRange(StringPosRange range);
    BACKPARSERDLLEXPORT StringPosRange getInitialisationListRange() const;

    void setTaskAttribute(const string& name, const string& value);
    BACKPARSERDLLEXPORT int getNumTaskAttributes() const; 
    BACKPARSERDLLEXPORT void getTaskAttibute(int num, string& strName, string& strValue) const;

protected:
    StringPosRange m_range;

    string m_name;
    StringPosRange m_nameRange;

    string m_interval;
    StringPosRange m_intervalRange;

    string m_priority;
    StringPosRange m_priorityRange;

    string m_autostart;
    StringPosRange m_autostartRange;

	// NFTASKHANDLING 19.05.05 SIS >>
    string m_loaded;
    StringPosRange m_loadedRange;
	// NFTASKHANDLING 19.05.05 SIS <<

    string m_iosync;
    StringPosRange m_iosyncRange;

    string m_fftask;
    StringPosRange m_fftaskRange;

    // SIS: event driven tasks >>
    string m_event;
    StringPosRange m_eventRange;
    // SIS: event driven tasks <<

    string m_watchdog;
    StringPosRange m_watchdogRange;

    StringPosRange m_initialisationListRange;

    SysComment *m_sys_cmnt;

    CArray<string, string> m_arrAttributeNames;
    CArray<string, string> m_arrAttributeValues;
};

#endif

