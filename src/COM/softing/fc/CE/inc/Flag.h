#ifndef __FLAG_H_INCLUDED_
#define __FLAG_H_INCLUDED_
typedef struct tagFlag
{
    ULONG  u;
} FLAG;

template <class T>
class CFlag : public tagFlag
{
    static tagFlag m_avail;
#ifdef _DEBUG
    static tagFlag m_locked;
#endif
    
public:
    CFlag()           { u  =  0l;          }
    CFlag(T eFlag)    { u  =  1l << eFlag; }
    CFlag(T A,T B)    { u  = (1l << A) | (1l<<B); }
    CFlag(T A,T B,T C){ u  = (1l << A) | (1l<<B) | (1l<<C); }
    
protected:
    CFlag(ULONG ul)   { u  =  ul;          }
    
public:
    // single flag operations
    void Set(T eFlag) { u |=   1l << eFlag; }
    void Clr(T eFlag) { u &= ~(1l << eFlag); }
    void Tgl(T eFlag) { u ^=  (1l << eFlag); }
    bool Chk(T eFlag) { return (u & (1l << eFlag)) != 0l; }
    
    // multiple flag operations
    void Set(CFlag F) { u |=  F; }
    void Clr(CFlag F) { u &= ~F; }
    void Tgl(CFlag F) { u ^=  F; }
    bool Chk(CFlag F) { return (u & F) != 0l; }
    
    CFlag operator+(CFlag flag) const;
    CFlag operator-(CFlag flag) const;
    operator ULONG() const;
    const CFlag& operator =(const ULONG ul) { u  =  ul; return *this; }
    const CFlag& operator =(const T  eFlag) { u  =  1l << eFlag; return *this; }
    //   const CFlag& operator+=(const ULONG ul) { u |=  ul; return *this; }
    //   const CFlag& operator-=(const ULONG ul) { u &= ~ul; return *this; }
    
    const CFlag& operator =(const CFlag flag) { u  =  flag.u; return *this; }
    const CFlag& operator+=(const CFlag flag) { u |=  flag.u; return *this; }
    const CFlag& operator-=(const CFlag flag) { u &= ~flag.u; return *this; }
    const CFlag& operator^=(const CFlag flag) { u ^=  flag.u; return *this; }
    
    bool operator==(const CFlag flag) { return (u & flag.u) == flag.u; }
    bool operator!=(const CFlag flag) { return (u & flag.u) != flag.u; }
    bool operator==(const ULONG ul  ) { return (u & ul) == ul;         }
    bool operator!=(const ULONG ul  ) { return (u & ul) != ul;         }
    
    static void Lock (T  eFlag);    // reserves a flag permanently  (false: already locked)
    static bool Alloc(T& eFlag);    // get a new flag handle        (false: nothing available)
    static void Free (T  eFlag);    // release e flag handle
    static void Use  (T  eFlag);    // mark a specific flag as used
    
    friend CArchive& AFXAPI operator<<(CArchive& ar, const CFlag& flag);
    friend CArchive& AFXAPI operator>>(CArchive& ar, CFlag& flag);
};

template <class T>
CFlag<T> CFlag<T>::operator+(CFlag<T> flag) const
{ return (flag.u | u); }

template <class T>
CFlag<T> CFlag<T>::operator-(CFlag<T> flag) const
{ return (u & ~flag.u); }

template <class T>
CFlag<T>::operator ULONG() const
{ return u; }

template <class T>
void CFlag<T>::Lock (T  eFlag)
{ m_locked |= (1l<<eFlag); }

template <class T>
bool CFlag<T>::Alloc(T& eFlag)
{
    for(eFlag=32; eFlag>=0; eFlag--)
    {
        if( m_avail & (1l<<eFlag))
        {
            m_avail &= ~(1l<<eFlag);
            return true;
        }
    }
    ASSERT(0&&"all flags used");
    return false;
}

template <class T>
void CFlag<T>::Free (T  eFlag)
{
    for(eFlag=32; eFlag>=0; eFlag--)
    {
        if( m_avail & (1l<<eFlag))
        {
            ASSERT( (m_locked&(1l<<eFlag)==0)&&"cannot Free locked flags");
            m_avail |= (1l<<eFlag);
        }
    }
}

template <class T>
CArchive& AFXAPI operator<<(CArchive& ar, const CFlag<T>& flag)
{
    ar << flag.u;
    return ar;
}

template <class T>
CArchive& AFXAPI operator>>(CArchive& ar, CFlag<T>& flag)
{
    ar >> flag.u;
    return ar;
}

#endif