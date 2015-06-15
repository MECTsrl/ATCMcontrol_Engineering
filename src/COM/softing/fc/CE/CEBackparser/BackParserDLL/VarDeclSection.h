
#ifndef VARDECLSECTION_H
#define VARDECLSECTION_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include <vector>

#include "Global.h"

using namespace std;

#include "IECSRCVisitor.h"
#include "StringPosRange.h"
#include "VarDecl.h"

typedef vector<const VarDecl*> var_declarations_type;
typedef var_declarations_type::const_iterator var_declarations_citer;

//================================================================================
class VarDeclSection
//================================================================================
{
public:

    VarDeclSection(const string& type, 
                   StringPosRange typeRange,
                   const string& modifier, 
                   StringPosRange modifierRange,
                   StringPos start);
    ~VarDeclSection();

    void setEndPos(StringPos end);

    BACKPARSERDLLEXPORT const string& getType() const;
    BACKPARSERDLLEXPORT const StringPosRange& getTypeRange() const;
    BACKPARSERDLLEXPORT const string& getModifier() const;
    BACKPARSERDLLEXPORT const StringPosRange& getModifierRange() const;
    BACKPARSERDLLEXPORT const var_declarations_type& getVarDeclList() const;
    const StringPosRange& getRange() const;
    void addVarDecl(const VarDecl* varDecl);
    void invite(const IECSRCVisitor& visitor) const;

protected:

    var_declarations_type m_var_decl_list;

    string m_type;
    StringPosRange m_typeRange;    

    string m_modifier;
    StringPosRange m_modifierRange;    

    StringPosRange m_range;
};

#endif
