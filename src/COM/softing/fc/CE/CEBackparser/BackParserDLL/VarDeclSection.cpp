
#include "stdafx.h"

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "VarDeclSection.h"
#include "Global.h"


//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::VarDeclSection
 * Description      constructor for class VarDeclSection
 * @param           const string& type
 * @param           StringPosRange typeRange
 * @param           const string& modifier
 * @param           StringPosRange modifierRange
 * @param           StringPos start
 * @return          -
 * @exception       -
 * @see             -
*/
VarDeclSection::VarDeclSection(const string& type, 
                               StringPosRange typeRange,
                               const string& modifier, 
                               StringPosRange modifierRange,
                               StringPos start)
    : m_type(type), m_modifier(modifier), m_range(start)
{
    m_typeRange = typeRange;
    m_modifierRange = modifierRange;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::~VarDeclSection
 * Description      destructor for class VarDeclSection
 * @return          -
 * @exception       -
 * @see             -
*/
VarDeclSection::~VarDeclSection()
{
   for( vector<const VarDecl*>::iterator i = m_var_decl_list.begin();
        i != m_var_decl_list.end();
        ++i )
   {
       delete *i;
   }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::setEndPos
 * Description      sets end position
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDeclSection::setEndPos(StringPos end) 
{ 
    m_range.setLast(end); 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getType
 * Description      returns the st type
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDeclSection::getType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_type;     
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getTypeRange
 * Description      returns the range of the st type
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& VarDeclSection::getTypeRange() const 
{ 
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_typeRange; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getModifier
 * Description      returns modifier
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDeclSection::getModifier() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_modifier; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getModifierRange
 * Description      returns the range of the modifier
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& VarDeclSection::getModifierRange() const 
{ 
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_modifierRange; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getVarDeclList
 * Description      returns the list of variable declarations
 * @return          const var_declarations_type&
 * @exception       -
 * @see             -
*/
const var_declarations_type& VarDeclSection::getVarDeclList() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_var_decl_list;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::getRange
 * Description      returns the range
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& VarDeclSection::getRange() const 
{ 
    return m_range; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::addVarDecl
 * Description      adds a variable declaration
 * @param           const VarDecl* varDecl
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDeclSection::addVarDecl(const VarDecl* varDecl)
{
    if ( varDecl != 0 )
    {
        m_var_decl_list.push_back(varDecl);
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDeclSection::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDeclSection::invite(const IECSRCVisitor& visitor) const
{
    visitor.visit(*this);

    for(var_declarations_citer ci_vd = m_var_decl_list.begin();
        ci_vd != m_var_decl_list.end();
        ++ci_vd)
    {
        if ( ci_vd != 0 )
        {
            if ( *ci_vd != 0 )
                (*ci_vd)->invite(visitor);
        }
//        else
//                //TODO: why are there null pointers in the list ?
//                DebugOutput << "CI_VD IS NULL" << endl; DebugOutput.flush();
    }
}

