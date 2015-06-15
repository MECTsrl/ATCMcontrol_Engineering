
#ifndef __ANYDBI_H_
#define __ANYDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDBIContext;


class CAnyDBI
{
public:
    CAnyDBI ();
    ~CAnyDBI ();

    void AddParent (CAnyDBI *pAnyDBI);
    void RemoveParent (CAnyDBI *pAnyDBI);

    void Corrupt ();

protected:
    CDBIContext                 *m_pContext;
    CList<CAnyDBI *, CAnyDBI *> m_Parents;

    void SetContext (CDBIContext *pContext);
};

#endif
