
#include "stdafx.h"

#include "StringPosRange.h"

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::StringPosRange
 * Description      constructor for class StringPosRange
 * @param           StringPos f
 * @param           StringPos l
 * @return          -
 * @exception       -
 * @see             -
*/
StringPosRange::StringPosRange(StringPos f, StringPos l)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    setFirst(f);
    setLast(l);
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::getFirst
 * Description      returns first
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos StringPosRange::getFirst() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return first; 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::getLast
 * Description      returns last
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos StringPosRange::getLast() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return second;  
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::getLength
 * Description      returns the length of the object
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos StringPosRange::getLength() 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return ( isValid() ? getLast() - getFirst() + 1
                       : string::npos ); 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::setFirst
 * Description      sets first
 * @param           StringPos f
 * @return          -
 * @exception       -
 * @see             -
*/
void StringPosRange::setFirst(StringPos f) 
{
    first  = f; 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::setLast
 * Description      sets last
 * @param           StringPos l
 * @return          -
 * @exception       -
 * @see             -
*/
void StringPosRange::setLast(StringPos l)  
{
    second = l; 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::isValid
 * Description      returns true if first and last are valid
 * @return          bool
 * @exception       -
 * @see             -
*/
bool StringPosRange::isValid() const 
{ 
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return ! isInvalid(); 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::isInvalid
 * Description      returns true if first or last in invalid
 * @return          bool
 * @exception       -
 * @see             -
*/
bool StringPosRange::isInvalid() const 
{
    return  getFirst() == string::npos || getLast() == string::npos; 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::getSubstr
 * Description      returns the substring of s which is specified by
 *                  this StringPosRange object
 * @param           const string& s
 * @return          string
 * @exception       -
 * @see             -
*/
string StringPosRange::getSubstr(const string& s) const
{ 
    return s.substr(getFirst(), getLast() - getFirst()); 
}

//------------------------------------------------------------------*
/**
 * Function name    StringPosRange::print
 * Description      prints this to an ostream
 * @param           ostream& out
 * @return          ostream&
 * @exception       -
 * @see             -
*/
ostream& StringPosRange::print(ostream& out) const 
{ 
    return out << "(" << getFirst() << ", " << getLast() << ")"; 
}

//------------------------------------------------------------------*
/**
 * Function name    operator<<
 * Description      prints a StringPosRange object to an ostream
 * @param           ostream& out
 * @param           const StringPosRange& r
 * @return          ostream&
 * @exception       -
 * @see             -
*/
ostream& operator<<(ostream& out, const StringPosRange& r)
{ 
    return r.print(out); 
}
