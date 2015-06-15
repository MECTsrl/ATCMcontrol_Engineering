
#ifndef VARDECL_H
#define VARDECL_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include <vector>

using namespace std;

#include "Global.h"
#include "IECSRCVisitor.h"
#include "StringPosRange.h"
#include "VarListElem.h"

typedef vector<VarListElem*> vars_type;
typedef vars_type::const_iterator vars_citer;

//================================================================================
class VarDecl
//================================================================================
{
public:

    VarDecl(const string& name, const StringPos start);
    virtual ~VarDecl();

    void setEndPos(StringPos end);

    StringPos getStartPos() const;
    StringPos getEndPos()   const;

    BACKPARSERDLLEXPORT StringPosRange getRange() const;

    void addVarListElem(const string& name, const StringPos start);
    void setLastVarListElemEnd(const StringPos end);
    void setLastVarListElemAddress(const string& address);
    void setLastVarListElemAddressRange(const StringPosRange addressRange);
    void setLastVarListElemSysComment(const SysComment* comment);


    void setType(const string& type);
    BACKPARSERDLLEXPORT const string& getType() const; 
    void setTypeRange(const StringPosRange range);
    BACKPARSERDLLEXPORT StringPosRange getTypeRange() const;

    void setEdge(const string& edge);
    BACKPARSERDLLEXPORT const string& getEdge() const; 
    void setEdgeRange(const StringPosRange range);
    BACKPARSERDLLEXPORT StringPosRange getEdgeRange() const;

    void setInitializer(const string& initializer);
    BACKPARSERDLLEXPORT const string& getInitializer() const; 
    void setInitializerFirst(const StringPos first);
    void setInitializerLast(const StringPos last);
    StringPos getInitializerLast();
    BACKPARSERDLLEXPORT StringPosRange getInitializerRange() const;

    void setVarComment(const string& varComment);
    BACKPARSERDLLEXPORT const string& getVarComment() const; 
    void setVarCommentRange(const StringPosRange range);
    BACKPARSERDLLEXPORT StringPosRange getVarCommentRange() const;

    bool isOfUserDefinedType() const;
    BACKPARSERDLLEXPORT vars_type getVariables() const;
    void invite(const IECSRCVisitor& visitor) const;

protected:

    StringPos m_end_pos;

    string m_type;
    StringPosRange m_typeRange;
    bool   m_isOfUserDefinedType;

    string m_edge;
    StringPosRange m_edgeRange;

    string m_initializer;
    StringPosRange m_initializerRange;

    string m_varComment;
    StringPosRange m_varCommentRange;

    vars_type m_var_list;
};

#endif

