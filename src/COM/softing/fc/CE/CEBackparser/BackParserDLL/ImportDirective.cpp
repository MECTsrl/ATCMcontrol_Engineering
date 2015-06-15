
#include"stdafx.h"

#include "ImportDirective.h"

//------------------------------------------------------------------*
/**
 * Function name    ImportDirective::ImportDirective
 * Description      constructor for class ImportDirective
 * @param           const string& filename
 * @param           StringPosRange range
 * @param           const string& type
 * @return          -
 * @exception       -
 * @see             -
*/
ImportDirective::ImportDirective(const string& fileName, 
                                 StringPosRange range,
                                 const string& type)
    : m_range(range), m_type(type)
{
    string csid = "#import";
    string strHelp;
    
    strHelp.append(fileName);

    TCHAR * pch = const_cast<char*>(strHelp.c_str());
    _tcslwr(pch);

    string strFN1;
    if(strHelp.find(csid) != -1) {
        m_fileName = fileName.substr(csid.length(), fileName.length() - csid.length());
    }
    else {
        m_fileName = fileName;
    }
    
    delChar(' ');
    delChar('\"');

    strFN1 = m_fileName;

    string strEndMark = "\"";
    int iPos = strFN1.find(strEndMark);
    if (iPos > 0)
    {
        m_fileName = strFN1.substr(0, iPos);
    }
    else
    {
        m_fileName = strFN1;
    }

    delChar(' ');
    delChar('\"');
}

//------------------------------------------------------------------*
/**
 * Function name    ImportDirective::delChar
 * Description      deletes characters c from the beginning and the end 
 *                  of the member _fileName
 * @param           char c
 * @return          -
 * @exception       -
 * @see             -
*/
void ImportDirective::delChar(char c)
{
    int first = m_fileName.find_first_not_of(c);
    if (first < 0)
    {
        first = 0;
    }
    int last = m_fileName.find_last_not_of(c);
    m_fileName = m_fileName.substr(first, last - first + 1);
}

//------------------------------------------------------------------*
/**
 * Function name    ImportDirective::setEndPos
 * Description      sets the end position
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void ImportDirective::setEndPos(StringPos end) 
{ 
    m_range.setLast(end); 
}

//------------------------------------------------------------------*
/**
 * Function name    ImportDirective::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void ImportDirective::invite(const IECSRCVisitor& visitor) const
{
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getFileName
 * Description      returns the file name
 * @return          string
 * @exception       -
 * @see             -
*/
const string ImportDirective::getFileName() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_fileName;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getImportDirectiveRange
 * Description      returns the range of the import directive
 * @return          const StringposRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& ImportDirective::getImportDirectiveRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_range;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::setType
 * Description      sets the type
 * @param           const string& type
 * @return          -
 * @exception       -
 * @see             -
*/
void ImportDirective::setType(const string& type)
{
    m_type = type;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getType
 * Description      returns the type
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& ImportDirective::getType() const
{
    return m_type;
}

