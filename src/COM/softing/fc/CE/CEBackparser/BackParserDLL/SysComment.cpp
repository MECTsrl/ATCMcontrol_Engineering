
#include "stdafx.h"

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "SysComment.h"

CAttribContainer::CAttribContainer() : m_strKey() , m_strValue()
{
}

CAttribContainer::CAttribContainer(string & strKey, string & strValue)
    : m_strKey(strKey) , m_strValue(strValue)
{
}

CAttribContainer::CAttribContainer(const CAttribContainer & other)
{
    if(this == &other) {
        return;
    }
    
    m_strKey = other.m_strKey;
    m_strValue = other.m_strValue;
}

CAttribContainer & CAttribContainer::operator =(const CAttribContainer &other)
{
    if(this == &other) 
        return *this;    //reject self assignment

    m_strKey = other.m_strKey;
    m_strValue = other.m_strValue;
    return *this;
}

//------------------------------------------------------------------*
/**
 * Function name    SysComment::SysComment
 * Description      constructor for class SysComment
 * @param           StringPos startPos
 * @param           StringPos lineNo
 * @return          -
 * @exception       -
 * @see             -
*/
SysComment::SysComment(StringPos startPos, StringPos lineNo)
        : m_range(startPos), m_lineno(lineNo)
{}

//------------------------------------------------------------------*
/**
 * Function name    SysComment::setEndPos
 * Description      sets the end position of the system comment
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void SysComment::setEndPos(StringPos end) 
{
    m_range.setLast(end); 
}

//------------------------------------------------------------------*
/**
 * Function name    SysComment::avm_set
 * Description      sets an attribute value pair
 * @param           const string& a
 * @param           const string& v
 * @return          -
 * @exception       -
 * @see             -
*/
void SysComment::avm_set(string& a,string& v)
{
    CAttribContainer cont(a,v);
    string  aTmp;
    CString strTmp(a.c_str());
    strTmp.MakeUpper();
    aTmp = strTmp;
    m_avm.erase(aTmp);
    m_avm.insert(avm_type::value_type(aTmp,cont));
}

//------------------------------------------------------------------*
/**
 * Function name    SysComment::avm_get
 * Description      returns the value for an attribute
 * @param           const string& a
 * @return          const string
 * @exception       -
 * @see             -
*/
const string SysComment::avm_get(const string& a) const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    CAttribContainer cont;

    string  aTmp;
    CString strTmp(a.c_str());
    strTmp.MakeUpper();
    aTmp = strTmp;

    av_citer i = m_avm.find(aTmp);
    if(i == m_avm.end()) {
        return "";
    }

    cont = i->second;
    return cont.m_strValue; 
}


//------------------------------------------------------------------*
/**
 * Remove attribute identified by key strKey from the list of 
 * attributes of a system comment object.
 * Checks first whether a attribute with that key exists within
 * the system comment object.
 * @param		const string & strKey - key (e.g. "URL")
 * @see			avm_set(), avm_get()
*/
void SysComment::avm_remove(const string & strKey)
{
    string  aTmp;
    CString strTmp(strKey.c_str());
    strTmp.MakeUpper();
    aTmp = strTmp;

    av_citer i = m_avm.find(aTmp);
    if(i == m_avm.end()) {
        return;
    }

    m_avm.erase(aTmp);
}
//------------------------------------------------------------------*
/**
 * Function name    SysComment::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void SysComment::invite(const IECSRCVisitor& visitor) const
{
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    SysComment::toString
 * Description      returns a string representing the system comment for 
 *                  the XML description of the pou
 * @param           const string& separator
 * @param           avm_filter_type filter
 * @return          string
 * @exception       -
 * @see             -
*/
string SysComment::toString(const string& separator, avm_filter_type filter) const 
{
    string res = "";

    CString KeyHelpUrl;
    KeyHelpUrl.LoadString(IDS_KEY_HELPURL);

    CString KeyUrl; 
    KeyUrl.LoadString(IDS_KEY_URL);

    for(avm_filter_type::const_iterator ci_key = filter.begin();
        ci_key != filter.end();
        ++ci_key)
    {
        string value = avm_get(*ci_key);
        if ( value.empty() )
        {
            continue;
        }

        if ( ! res.empty() )
            res += separator;

        string key;
        if (CString((*ci_key).c_str()).CompareNoCase(KeyHelpUrl) == 0)
        {
            key = "USER_HELP";
        }
        else if (CString((*ci_key).c_str()).CompareNoCase(KeyUrl) == 0)
        {
            key = "URL";
        }
        else
        {
            key = *ci_key;
        }

        res += key + "=\"" + value + "\"";
    }
    return res;
}


//------------------------------------------------------------------*
/**
 * Get first attribute from list of attributes of a system comment
 * object. The attribute consists of a key (e.g. "URL") and a value.
 *
 * @param		string & strKey - key of attribute
 * @param		string & strValue - value of attribute
 * @return		bool - true - first attribute found, false - no
 *              attribute found.
 * @see			GetNextAttrib()
*/
bool SysComment::GetFirstAttrib(string & strKey, string & strValue)
{
    CAttribContainer cont;
    strKey.empty();
    strValue.empty();
    
    m_iter = m_avm.begin();
    if(m_iter == m_avm.end()) {
        return false;
    }

    cont = m_iter->second;

    strKey = cont.m_strKey;
    strValue = cont.m_strValue;
    return true;
}


//------------------------------------------------------------------*
/**
 * Get next attribute from list of attributes of a system comment
 * object. The attribute consists of a key (e.g. "URL") and a value.
 * Call GetFirstAttrib() prior to calling this method.
 *
 * @param		string & strKey - key of attribute
 * @param		string & strValue - value of attribute
 * @return		bool - true - attribute found, false - no
 *              attribute found.
 * @see			GetFirstAttrib()
*/
bool SysComment::GetNextAttrib(string & strKey, string & strValue)
{
    CAttribContainer cont;
    strKey.empty();
    strValue.empty();

    if(m_iter == m_avm.end()) {
        return false;
    }
    
    m_iter++;

    if(m_iter == m_avm.end()) {
        return false;
    }

    cont = m_iter->second;

    strKey = cont.m_strKey;
    strValue = cont.m_strValue;
    return true;
}


//------------------------------------------------------------------*
/**
 * Function name    SysComment::GetRange
 * Description      returns the range of the system comment
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& SysComment::GetRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_range;
}
