
#include "stdafx.h"

#include "VarDecl.h"

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::VarDecl
 * Description      constructor for class VarDecl
 * @param           const string& name
 * @param           const StringPosRange start
 * @return          -
 * @exception       -
 * @see             -
*/
VarDecl::VarDecl(const string& name, const StringPos start)
{
    addVarListElem(name, start);
    m_varComment = "";
    m_varCommentRange = StringPosRange(-1, -2);
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::~VarDecl
 * Description      destructor for class VarDecl
 * @return          -
 * @exception       -
 * @see             -
*/
VarDecl::~VarDecl()
{
    for( vars_citer ci_vl = m_var_list.begin();
        ci_vl != m_var_list.end();
        ++ci_vl )
   {
       delete *ci_vl;
   }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setEndPos
 * Description      sets the end position
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setEndPos(StringPos end) 
{ 
    m_end_pos = end; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getStartPos
 * Description      returns the start position
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos VarDecl::getStartPos() const 
{
    return (*m_var_list.begin())->range.getFirst(); 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getEndPos
 * Description      returns the end position
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos VarDecl::getEndPos() const 
{ 
    return m_end_pos; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getRange
 * Description      returns the range of the variable declaration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange VarDecl::getRange() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return StringPosRange(getStartPos(), getEndPos()); 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::addVarListElem
 * Description      adds a variable to the list of variables
 * @param           const string& name
 * @param           const StringPos start
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::addVarListElem(const string& name, const StringPos start)
{
    VarListElem *vle = new VarListElem(name, start);
    m_var_list.push_back(vle);
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setLastVarListElemEnd
 * Description      sets the end Position of the last variable in the
 *                  list of variables
 * @param           const StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setLastVarListElemEnd(const StringPos end)
{
    vars_type::size_type pos_of_last = m_var_list.size() - 1;
    if ( pos_of_last >= 0 )
    {
        m_var_list.at(pos_of_last)->range.setLast(end);
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setLastVarListElemAddress
 * Description      sets the address of the last variable in the
 *                  list of variables
 * @param           const string& address
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setLastVarListElemAddress(const string& address)
{
    vars_type::size_type pos_of_last = m_var_list.size() - 1;
    if ( pos_of_last >= 0 )
    {
        m_var_list.at(pos_of_last)->address = address;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setLastVarListElemAddressRange
 * Description      sets the range of the last variable in the
 *                  list of variables
 * @param           const StringPosRange addressRange
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setLastVarListElemAddressRange(const StringPosRange addressRange)
{
    vars_type::size_type pos_of_last = m_var_list.size() - 1;
    if ( pos_of_last >= 0 )
    {
        m_var_list.at(pos_of_last)->addressRange = addressRange;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setLastVarListElemSysComment
 * Description      sets the system comment of the last variable in the
 *                  list of variables
 * @param           const SysComment* comment
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setLastVarListElemSysComment(const SysComment* comment)
{
    vars_type::size_type pos_of_last = m_var_list.size() - 1;
    if ( pos_of_last >= 0 )
    {
        m_var_list.at(pos_of_last)->syscomment = comment;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setType
 * Description      sets the st type of the variable declaration
 * @param           const string& type
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setType(const string& type)
{
    m_type = type;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getType
 * Description      returns the st type of the variable declaration
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDecl::getType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_type;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setTypeRange
 * Description      sets the range of the st type of the variable declaration
 * @param           const StringPosRange range
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setTypeRange(const StringPosRange range)
{
    m_typeRange = range;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getTypeRange
 * Description      returns the range of the st type of the variable declaration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange VarDecl::getTypeRange() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_typeRange; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setEdge
 * Description      sets the edge of the variable declaration
 * @param           const string& edge
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setEdge(const string& edge)
{
    m_edge = edge;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getEdge
 * Description      returns the edge of the variable declaration
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDecl::getEdge() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_edge;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setEdgeRange
 * Description      sets the range of the edge of the variable declaration
 * @param           const StringPosRange range
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setEdgeRange(const StringPosRange range)
{
    m_edgeRange = range;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getEdgeRange
 * Description      returns the range of the edge of the variable declaration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange VarDecl::getEdgeRange() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_edgeRange; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::isOfUserDefinedType
 * Description      returns the value of the member m_isOfUserDefinedType
 * @return          bool
 * @exception       -
 * @see             -
*/
bool VarDecl::isOfUserDefinedType() const
{
    return m_isOfUserDefinedType;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getVariables
 * Description      returns the list of variables
 * @return          vars_type
 * @exception       -
 * @see             -
*/
vars_type VarDecl::getVariables() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_var_list;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::invite(const IECSRCVisitor& visitor) const
{
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setInitializer
 * Description      sets the initializer
 * @param           const string& initializer
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setInitializer(const string& initializer)
{
    m_initializer = initializer;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getInitializer
 * Description      returns the initializer
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDecl::getInitializer() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_initializer;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setInitializerFirst
 * Description      sets the start position of the initilizer
 * @param           const StringPos first
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setInitializerFirst(const StringPos first)
{
    m_initializerRange.setFirst(first);
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setInitializerLast
 * Description      sets the end position of the initilizer
 * @param           const StringPos last
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setInitializerLast(const StringPos last)
{
    m_initializerRange.setLast(last);
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getInitializerLast
 * Description      returns the end position of the initilizer
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos VarDecl::getInitializerLast()
{
    return m_initializerRange.getLast();
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getInitializerRange
 * Description      returns the range of the initilizer
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange VarDecl::getInitializerRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_initializerRange;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setVarComment
 * Description      sets the variable comment
 * @return          const string&
 * @exception       -
 * @see             -
*/
void VarDecl::setVarComment(const string& varComment)
{
    m_varComment = varComment;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getVarComment
 * Description      returns the variable comment
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarDecl::getVarComment() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_varComment;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::getVarCommentRange
 * Description      returns the range of the variable comment
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange VarDecl::getVarCommentRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_varCommentRange;
}

//------------------------------------------------------------------*
/**
 * Function name    VarDecl::setVarCommentRange
 * Description      sets the range of the variable comment
 * @param           const StringPosRange range
 * @return          -
 * @exception       -
 * @see             -
*/
void VarDecl::setVarCommentRange(const StringPosRange range)
{
    m_varCommentRange = range;
}

