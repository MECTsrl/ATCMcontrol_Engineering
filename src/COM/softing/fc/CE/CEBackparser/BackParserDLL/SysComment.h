
#ifndef SYSCOMMENT_H
#define SYSCOMMENT_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <map>
#include <set>
#include <string>
#include <iostream>

using namespace std;

#include "Global.h"
#include "StringPosRange.h"
#include "IECSRCVisitor.h"
#include "keyrc.h"

class CAttribContainer
{
public:
    string m_strKey;
    string m_strValue;

    CAttribContainer();
    CAttribContainer(string & strKey, string & strValue);
    CAttribContainer(const CAttribContainer &);

    CAttribContainer & operator =(const CAttribContainer &);
};

typedef map<string, CAttribContainer>  avm_type; // type for an attribute-value-map

typedef set<string> avm_filter_type; // attribute filter

typedef avm_type::iterator        av_iter;
typedef avm_type::const_iterator  av_citer;

//================================================================================
class SysComment
//================================================================================
{
public:
    SysComment(StringPos startPos=string::npos, StringPos lineNo=string::npos);
    
    void setEndPos(StringPos end);

//    av_iter  avm_begin()       { return m_avm.begin(); }
//    av_citer avm_begin() const { return m_avm.begin(); }

    BACKPARSERDLLEXPORT void avm_set(string& a,string& v);
    BACKPARSERDLLEXPORT const string avm_get(const string& a) const;
    BACKPARSERDLLEXPORT void avm_remove(const string & strKey);
    virtual void invite(const IECSRCVisitor& visitor) const;
    BACKPARSERDLLEXPORT const StringPosRange& GetRange() const;
    string toString(const string& separator, avm_filter_type filter) const ;
    BACKPARSERDLLEXPORT bool GetFirstAttrib(string& strKey, string & strValue);
    BACKPARSERDLLEXPORT bool GetNextAttrib(string& strKey, string & strValue);

protected:
    avm_type       m_avm;
    av_iter        m_iter;
    StringPosRange m_range;
    StringPos      m_lineno;
};

#endif

