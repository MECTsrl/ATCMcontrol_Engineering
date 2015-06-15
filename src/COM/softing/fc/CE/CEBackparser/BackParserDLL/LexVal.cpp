
#include "stdafx.h"

#include "LexVal.h"

int LexVal::m_instance_counter = 0;

//------------------------------------------------------------------*
/**
 * Function name    LexVal::LexVal
 * Description      constructor for class LexVal
 * @param           const string& text
 * @param           const StringPos end
 * @param           const StringPos lineno
 * @return          -
 * @exception       -
 * @see             -
*/
LexVal::LexVal(const string& text, 
               const StringPos end, 
               const StringPos lineno):
    m_text(text), 
    m_lineno(lineno)
{
    m_instance_counter++;
    m_range = StringPosRange(end - m_text.length() + 1, end);

//    DebugOutput << "CREATE LEXVAL: "
//                << m_instance_counter << ", "
//                << text << ", "
//                << (void*)this
//                << endl;
//    DebugOutput.flush();
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::LexVal
 * Description      destructor for class LexVal
 * @return          -
 * @exception       -
 * @see             -
*/
LexVal::~LexVal()
{
//    DebugOutput << "DELETE LEXVAL: "
//         << m_instance_counter << ", "
//         << m_text << ", "
//         << (void*)this
//         << endl;
//    DebugOutput.flush();

    m_instance_counter--;
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::get_instance_counter
 * Description      Return the number of instances
 * @return          int
 * @exception       -
 * @see             -
*/
int LexVal::get_instance_counter()
{
    return m_instance_counter;
}

//------------------------------------------------------------------*
/**
 * Function name    Function::set
 * Description      sets the text, the end position, and the line number
 * @param           const char* text
 * @param           StringPos endPos
 * @param           StringPos lineno
 * @return          -
 * @exception       -
 * @see             -
*/
void LexVal::set(const char* text, StringPos endPos, StringPos lineno)
{
    m_text = text;
    m_range.setLast(endPos);
    m_range.setFirst(endPos - m_text.length());
    m_lineno = lineno;
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::getText
 * Description      returns the text 
 * @return          string
 * @exception       -
 * @see             -
*/
string LexVal::getText() const 
{
    return m_text;   
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::getRange
 * Description      returns the range 
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange LexVal::getRange() const 
{
    return m_range;  
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::getStartPos
 * Description      returns the start position
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos LexVal::getStartPos() const
{
    return getRange().getFirst(); 
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::getEndPos
 * Description      returns the end position
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos LexVal::getEndPos() const 
{
    return getRange().getLast();  
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::getLineNo
 * Description      returns the line number
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos LexVal::getLineNo() const 
{
    return m_lineno; 
}

//------------------------------------------------------------------*
/**
 * Function name    LexVal::print
 * Description      prints the LexVal to an ostream
 * @param           ostream& out
 * @return          ostream&
 * @exception       -
 * @see             -
*/
ostream& LexVal::print(ostream& out) const
{ 
    return out << "line=" << m_lineno << ", range=" << m_range << ", " << m_text; 
}

//------------------------------------------------------------------*
/**
 * Function name    operator<<
 * Description      prints a LexVal to an ostream
 * @param           ostream& out
 * @param           const LexVal& lv
 * @return          ostream&
 * @exception       -
 * @see             -
*/
ostream& operator<<(ostream& out, const LexVal& lv)
{
    return lv.print(out); 
}

//------------------------------------------------------------------*
/**
 * Function name    operator<<
 * Description      prints a LexVal to an ostream
 * @param           ostream& out
 * @param           const LexVal* lv
 * @return          ostream&
 * @exception       -
 * @see             -
*/
ostream& operator<<(ostream& out, const LexVal* lv)
{ 
    return ( lv == 0 ? out << "NULL" : lv->print(out) ); 
}
