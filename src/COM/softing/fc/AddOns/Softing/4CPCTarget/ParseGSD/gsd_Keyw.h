#ifndef __GSD_KEYW_H_
#define __GSD_KEYW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

class CGsdKeyword
///////////////////////////////////////////////////////////////////////////////
//  class CGsdKeyword
//      
///////////////////////////////////////////////////////////////////////////////
{
// =Construction
public:
                            CGsdKeyword();
                                // default ctor
    virtual                 ~CGsdKeyword();
                                // dtor

                            CGsdKeyword(CGsdKeyword const & other);
                                // copy ctor
    CGsdKeyword const &    operator = (CGsdKeyword const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    int                     LookupKeyword(LPCSTR pszKeyword, int& iRevision);
    LPCSTR                  KeywordFromToken(int iToken);

// =Overridables
public:

protected:

// =Helpers
private:
    void                    InitializeKeywords(void);

// =Attributes
protected:


private:
};

/////////////////////////////////////////////////////////////////////////////
#endif // __GSD_KEYW_H_
