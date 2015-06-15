
#include "stdafx.h"

#include "StreamScan.h"

//------------------------------------------------------------------*
/**
 * Function name    stream_scan::stream_scan
 * Description      constructor for class stream_scan
 * @param           istream& in
 * @param           ostream& out
 * @param           const string& inContents
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
stream_scan::stream_scan(istream& in, ostream& out, const string& inContents)
//================================================================================
    : yy_scan(512),yy_istr(&in), yy_ostr(&out), m_istrContents(inContents)
{
    yylval.lexval = NULL;
}

//------------------------------------------------------------------*
/**
 * Function name    stream_scan::~stream_scan
 * Description      destructor for class stream_scan
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
stream_scan::~stream_scan()
//================================================================================
{
    while (yylval.lexval != NULL)
    {
        LexVal *nextToBeDeleted;
        nextToBeDeleted = yylval.lexval->m_pPrevious;
        delete yylval.lexval;
        yylval.lexval = nextToBeDeleted;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    stream_scan::yygetc
 * Description      reads a character and returns it
 * @return          int
 * @exception       -
 * @see             -
*/
//================================================================================
int	stream_scan::yygetc()
//================================================================================
{	// scanner source of input characters
	int c = yy_istr->get();
	if ( c != EOF )
		yypos++;
	return c;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::yyerror
 * Description      writes an error message
 * @param           char* msg
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void stream_scan::yyerror(char * msg, ...)
//================================================================================
{
    if (yylineno)
	    (*yy_ostr) << "line " << yylineno << ": ";
    (*yy_ostr) << msg << endl;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getSubString
 * Description      returns the substring specified by parameter range
 * @param           StringPosRange range
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string stream_scan::getSubString(StringPosRange range)
//================================================================================
{
    return m_istrContents.substr(range.getFirst(), 
                                 range.getLast() - range.getFirst() + 1);
}

