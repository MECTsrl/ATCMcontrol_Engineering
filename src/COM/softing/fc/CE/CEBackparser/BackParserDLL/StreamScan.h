
#if !defined __STREAMSCAN__
#define __STREAMSCAN__

#include "lex_yacc\Grammar.h"

class stream_scan : public yy_scan
{
public:
    stream_scan(istream& in, ostream& out, const string& inContents);
    virtual ~stream_scan();

	virtual int	yygetc();
    virtual void yyerror(char * msg, ...);

    string getSubString(StringPosRange range);

protected:
    istream* yy_istr;
    ostream* yy_ostr;

    string m_istrContents;
};

#endif

