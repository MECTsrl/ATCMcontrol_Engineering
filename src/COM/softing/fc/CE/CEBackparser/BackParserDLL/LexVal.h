
#ifndef LEXVAL_H
#define LEXVAL_H

#include "StringPosRange.h"

#include "Global.h"

//================================================================================
class LexVal
//================================================================================
{
public:
    LexVal(const string& text, 
           const StringPos end, 
           const StringPos lineno);
    ~LexVal();

    static int get_instance_counter();
    void set(const char* text, StringPos endPos, StringPos lineno);
    string getText() const;
    StringPosRange getRange() const;
    StringPos getStartPos() const;
    StringPos getEndPos() const;
    StringPos getLineNo() const;

    ostream& print(ostream& out) const;

    LexVal        *m_pPrevious;

protected:
    string         m_text;
    StringPosRange m_range;
    StringPos      m_lineno;

    static m_instance_counter;
};

ostream& operator<<(ostream& out, const LexVal& lv);
ostream& operator<<(ostream& out, const LexVal* lv);

#endif
