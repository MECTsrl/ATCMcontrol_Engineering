#if !defined(HLPTEMPL_H__24E1F6D2_0C94_11E2_9F8C_555664651111__INCLUDED_)
#define HLPTEMPL_H__24E1F6D2_0C94_11E2_9F8C_555664651111__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//=============================================== CSet<T>:: declaration
// - supports both associative AND random access
//#include <afxtempl.h>

template<class T>
class CSet :virtual public CArray<T,T> // deriving ":protected" leads to C2584 !!
{
public:
   CSet():CArray<T,T>(){};
   CSet(int htSize);
   int  Find(const T P) const;     // true: found
   int  Add(const T P);            // true: inserted; false: already inside
   int  Remove(const T P);         // true: removed;  false: not found
   bool RemoveAt(int idx);         // true: removed;  false: not found
   void RemoveAll();
   bool Toggle(T P);               // true: inserted  false: removed
   int  Size() const;
   bool IsEmpty() const;
   void Add(const CSet& S);
   void Subtract (const CSet& S); // subtract the other set
   T&   GetRef(int idx);
   T& operator[](int idx){return CArray<T,T>::operator[](idx);}
protected:
   void UpdateMap();
   CMap<T,T,int,int> Map;    // for associated access
#ifdef _DEBUG
   void DbgChk();
#endif
};//CSet<T>

//----------------------------------------------- CSet<T>:: implementation
template<class T> inline
T& CSet<T>::GetRef(int idx)
{ 
   return CArray<T,T>::operator[](idx);
}
template<class T> inline
void CSet<T>::UpdateMap()
{
   Map.RemoveAll();
   for(int i=0; i<GetSize(); i++)
      Map[GetAt(i)]=i;
}
template<class T> inline
CSet<T>::CSet(int htSize)     // ARG: hash table size
{
   Map.InitHashTable(htSize);
}
template<class T> inline
int CSet<T>::Find(T P)  const //  index of P (zero based; not found: -1)
{ 
   int i;
   return Map.Lookup(P,i) ?i :-1;
}
template<class T> inline
int CSet<T>::Add(T P)         // merge a single element (returns the index)
{
   int i;
   if(!Map.Lookup(P,i))
   {
      i = CArray<T,T>::Add(P);
      Map[P] = i;
   }
#ifdef _DEBUG
   DbgChk();
#endif
   return i;
}
template<class T> inline
int CSet<T>::Remove(T P)      // remove a single element
{
   int idx;
#ifdef _DEBUG
   DbgChk();
#endif
   if(Map.Lookup(P,idx))
   {
      CArray<T,T>::RemoveAt(idx);
      UpdateMap();
      return TRUE;
   }
   return FALSE;
}
template<class T> inline
bool CSet<T>::RemoveAt(int idx)// remove at index (true: if found)
{
   if(idx>=0 && idx<Map.GetCount())
   {
      CArray<T,T>::RemoveAt(idx);
      UpdateMap();
      return true;
   }
   return false;
}
template<class T> inline
void CSet<T>::RemoveAll()     // clean up all elements
{ 
   Map.RemoveAll();
   CArray<T,T>::RemoveAll();
}
template<class T> inline
bool CSet<T>::Toggle(T P)    // true: inserted; false: removed
{
   int idx = Find(P);
   if(idx>=0)
   {
      RemoveAt(idx);
      return false;
   }else
   {
      Add(P);
      return true;
   }
}
template<class T> inline
int CSet<T>::Size() const     // element count
{
   return Map.GetCount();
}
template<class T> inline
bool CSet<T>::IsEmpty() const // true if no elements
{
   return Map.IsEmpty();
}
template<class T> inline
void CSet<T>::Add(const CSet& S) // unite with anoter set
{
   T t;
   for(POSITION pos = S.Map.GetStartPosition(); pos!=NULL;) 
   {
      int idx;
      S.Map.GetNextAssoc( pos, t, idx );
      Add(t);
   }
#ifdef _DEBUG
   DbgChk();
#endif
}
template<class T> inline
void CSet<T>::Subtract(const CSet& S) // subtract the other set
{
   int idx = 0;
   bool bUpdate = false;
   while(idx<S.GetSize())
   {
      T t = S.GetAt(idx);
      int i = Find(t);
      if(i>=0)
      {
         Map.RemoveKey(t);
         bUpdate |= true;
      }
      idx++;
   }
   if(bUpdate)
   {
      int N = Map.GetCount();
      if(N>0)
      {
         CArray<T,T> tmp;
         tmp.SetSize(N);   // memory allocation
         tmp.RemoveAll();
         for(POSITION pos = Map.GetStartPosition(); pos!=NULL; )  // append remaining elements
         {
            T t;
            Map.GetNextAssoc(pos, t, idx);
            tmp.Add(t);
         }
         Copy(tmp);
         UpdateMap();
      }else
      {
         RemoveAll();
      }
   }
#ifdef _DEBUG
   DbgChk();
#endif
}

#ifdef _DEBUG
template<class T> inline
void CSet<T>::DbgChk()        // internal consistency check
{ 
#ifdef ASSERT
   ASSERT(GetSize()==Map.GetCount());
#endif
}
#endif

#endif //!defined(HLPTEMPL_H__24E1F6D2_0C94_11E2_9F8C_555664651111__INCLUDED_)