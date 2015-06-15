
#include <tchar.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "helper.h"
#include "cgt_CDiagMsg.h"
#include "typeset.h"


/*======================== ST_TypeSet_C ====================== */
ST_TypeSet_C::ST_TypeSet_C() /* empty TYPE */
{
  lTypeSet = CG_TBS_EMPTY;
  lpTypeDescr = NULL;
}

ST_TypeSet_C::ST_TypeSet_C(CG_BITSET lType) /* any TYPE */
{
  lTypeSet = lType;
  lpTypeDescr = NULL;
}
ST_TypeSet_C::ST_TypeSet_C(CG_BITSET lType, int iLen) /* ST_ANY_STRING with string length */
{
    lTypeSet = lType;
    //iLen may be 0 for string lit ""
    assert(iLen>=0 && (lType & CG_TBS_ANY_STRING));
    lpTypeDescr = new TYPEDESCR_TYP;
    assert(lpTypeDescr);
    if(lpTypeDescr)
    {
        lpTypeDescr->attrib = STRING_LEN;
        lpTypeDescr->stringLength = iLen;
    }
}
ST_TypeSet_C::ST_TypeSet_C(CG_BITSET lType, const ST_SUBRANGE_TYP *range) /* SUBRANGE, lType in ANY_INT */
{
  lTypeSet = lType;
  lpTypeDescr = NULL;
  if (range)
    {
      assert(lType | CG_TBS_ANY_INT == CG_TBS_ANY_INT);
      assert(CountBits(lType) == 1);
      lpTypeDescr = new TYPEDESCR_TYP;
      assert(lpTypeDescr);
      if (lpTypeDescr)
        {
          lpTypeDescr->attrib = SUBRANGE_DEF;
          lpTypeDescr->Subrange = *range;
        }
    }
}

ST_TypeSet_C::ST_TypeSet_C(CG_BITSET lType, const TCHAR* szName) /* STRUCTURE|INSTANCE */
{
  lTypeSet = lType;
  lpTypeDescr = NULL;
  if (szName)
    {
      assert(lType == CG_TBS_STRUCT || lType == CG_TBS_INSTANCE 
             || lType == CG_TBS_PROGRAM || lType == CG_TBS_ENUM);
      lpTypeDescr = (TYPEDESCR_TYP*)new char[offsetof(TYPEDESCR_TYP, szTypeName[_tcslen(szName)+1])];
      assert(lpTypeDescr);
      if (lpTypeDescr)
        {
          lpTypeDescr->attrib = TYPE_NAME;
          strcpy(lpTypeDescr->szTypeName, szName);
        }
    }
}

ST_TypeSet_C::ST_TypeSet_C(CG_BITSET lType, int dim, const ST_SUBRANGE_TYP *range, const ST_TypeSet_C &BaseType) /* ARRAY */
{
    lTypeSet = lType;
    lpTypeDescr = NULL;

    assert(lType == CG_TBS_ARRAY);
    assert(range || dim==0);
    assert(BaseType.IsMonomorphType());
    
    lpTypeDescr = (TYPEDESCR_TYP*)new char[offsetof(TYPEDESCR_TYP, ArrayRange[dim])];
    assert(lpTypeDescr);

    if (lpTypeDescr)
    {
        lpTypeDescr->attrib = ARRAY_DEF;
        lpTypeDescr->lpBaseType = new ST_TypeSet_C (BaseType);
        assert(lpTypeDescr->lpBaseType);
        lpTypeDescr->ArrayDim = dim;
        for (int i=0; i<dim; i++)
            lpTypeDescr->ArrayRange[i] = range[i];
    }
}

ST_TypeSet_C::ST_TypeSet_C(const ST_TypeSet_C &that)
{
  lTypeSet = that.lTypeSet;
  lpTypeDescr = CopyTypeDescr(that.lpTypeDescr);
}


void ST_TypeSet_C::initFromCG_DType(const CG_DType* pType) //TODO this function is sh???
{
    int i;

    if (lpTypeDescr) 
    {
        if (lpTypeDescr->attrib == ARRAY_DEF)
            delete lpTypeDescr->lpBaseType;
        delete lpTypeDescr;
        lpTypeDescr = NULL;
    }
    

    if(pType->bIsArray)
    {
        lTypeSet = CG_TBS_ARRAY;

        lpTypeDescr = (TYPEDESCR_TYP*)new char[offsetof(TYPEDESCR_TYP, ArrayRange[pType->nDim])];
        lpTypeDescr->attrib = ARRAY_DEF;

        lpTypeDescr->ArrayDim = pType->nDim;
        for (i=0; i<lpTypeDescr->ArrayDim; i++)
        {
            lpTypeDescr->ArrayRange[i].lRangeStart = pType->range[i].lRangeStart;
            lpTypeDescr->ArrayRange[i].ulRangeLength = pType->range[i].ulRangeLength;
        }

        if(pType->pSym)
        {
            assert(pType->pSym->pouTyp==CG_POU_SDT);
            lpTypeDescr->lpBaseType = new ST_TypeSet_C(CG_TBS_STRUCT, pType->pSym->pszId);
        }
        else if(pType->lStrLen>=0)
        {
            lpTypeDescr->lpBaseType = new ST_TypeSet_C(pType->tbs, pType->lStrLen);
        }
        else
        {
            lpTypeDescr->lpBaseType = new ST_TypeSet_C(pType->tbs);
        }
    }
    else
    {
        lTypeSet = pType->tbs;
        switch(pType->type)
        {
            case CG_INSTANCE:
            case CG_STRUCT:
            case CG_PROGRAM:
            //##AC beg
                lpTypeDescr = (TYPEDESCR_TYP*)new char
                    [offsetof(TYPEDESCR_TYP, szTypeName[_tcslen(pType->pszName)+1])];
                lpTypeDescr->attrib = TYPE_NAME;
                lstrcpy(lpTypeDescr->szTypeName, pType->pszName);
            break;
            //##AC end
            case CG_STRING: 
            case CG_WSTRING:
            case CG_QVT_STRING:
            case CG_QVT_WSTRING:
                if(pType->lStrLen>=0)
                {
                    lpTypeDescr = new TYPEDESCR_TYP;
                    lpTypeDescr->attrib = STRING_LEN;
                    lpTypeDescr->stringLength = pType->lStrLen;
                }
        }
    }
}

ST_TypeSet_C::~ST_TypeSet_C()
{
  if (lpTypeDescr)
    {
      if (lpTypeDescr->attrib == ARRAY_DEF)
        delete lpTypeDescr->lpBaseType;
      delete lpTypeDescr;
    }
}


ST_TypeSet_C &ST_TypeSet_C::operator =(const ST_TypeSet_C &that)
{
  if (this!=&that)
    {
      lTypeSet = that.lTypeSet;
      if (lpTypeDescr) {
        if (lpTypeDescr->attrib == ARRAY_DEF)
          delete lpTypeDescr->lpBaseType;
        delete lpTypeDescr;
      }
      lpTypeDescr = CopyTypeDescr(that.lpTypeDescr);
    }
  return *this;
}

ST_TypeSet_C &ST_TypeSet_C::operator &=(const ST_TypeSet_C &that)
{
    if (this!=&that)
    {
        lTypeSet &= that.lTypeSet;

        if(lTypeSet == CG_TBS_VOID)
        {
            if(lpTypeDescr) 
            {
              if(lpTypeDescr->attrib == ARRAY_DEF)
                  delete lpTypeDescr->lpBaseType;
              delete lpTypeDescr;
              lpTypeDescr = NULL;
            }
        }
        else if(lpTypeDescr && lpTypeDescr->attrib==STRING_LEN ||
           that.lpTypeDescr && that.lpTypeDescr->attrib==STRING_LEN)
        {
            //no string len info if types are intersected,
            //unless other type has same length:
            if (lpTypeDescr)
            {
                assert(!that.lpTypeDescr || lpTypeDescr->attrib==that.lpTypeDescr->attrib);
                if(!that.lpTypeDescr ||
                    that.lpTypeDescr->stringLength!=lpTypeDescr->stringLength)
                {
                    delete lpTypeDescr;
                    lpTypeDescr = NULL;
                }
            }
        }
        else if(that.lpTypeDescr)
        {
            if(!lpTypeDescr)
            {
                lpTypeDescr = CopyTypeDescr(that.lpTypeDescr);
            }
            else if(lpTypeDescr->attrib==ARRAY_DEF &&
                    that.lpTypeDescr->attrib==ARRAY_DEF &&
                    (lpTypeDescr->ArrayDim==0 || that.lpTypeDescr->ArrayDim==0) &&
                    lpTypeDescr->lpBaseType && that.lpTypeDescr->lpBaseType &&
                    *lpTypeDescr->lpBaseType == *that.lpTypeDescr->lpBaseType)
            {
                // handle case where either type is an assumed size array:
                // the resulting type should contain the range information!
                if(lpTypeDescr->ArrayDim==0)
                {
                    delete lpTypeDescr->lpBaseType;
                    delete lpTypeDescr;
                    lpTypeDescr = CopyTypeDescr(that.lpTypeDescr);
                }
            }
            else if(!IsEqualTypeDescr(lpTypeDescr, that.lpTypeDescr))
            {
                lTypeSet = CG_TBS_EMPTY;
                if(lpTypeDescr->attrib == ARRAY_DEF)
                    delete lpTypeDescr->lpBaseType;
                delete lpTypeDescr;
                lpTypeDescr = NULL;
            }
        }
    }

    return *this;
}

int ST_TypeSet_C::operator == (const ST_TypeSet_C &that) const
{
  return lTypeSet == that.lTypeSet && IsEqualTypeDescr(lpTypeDescr, that.lpTypeDescr);
}

int ST_TypeSet_C::operator != (const ST_TypeSet_C &that) const
{
  return lTypeSet != that.lTypeSet || !IsEqualTypeDescr(lpTypeDescr, that.lpTypeDescr);
}

int ST_TypeSet_C::IsEmptyType() const
{
  return lTypeSet == CG_TBS_EMPTY;
}

int ST_TypeSet_C::IsMonomorphType() const
{
    if(CountBits(lTypeSet) != 1)
        return 0;

    if(lTypeSet & (CG_TBS_ARRAY|CG_TBS_STRUCT|CG_TBS_ENUM|CG_TBS_INSTANCE|CG_TBS_PROGRAM))
    {
        if(!lpTypeDescr)
            return 0;
    }

    return 1;
}

int ST_TypeSet_C::GetArrayDim() const /* if TYPESET is ARRAY */
{
  if (lpTypeDescr && lpTypeDescr->attrib==ARRAY_DEF)
    return lpTypeDescr->ArrayDim;
  else
    return -1;
}

int ST_TypeSet_C::GetArrayRange(int n, ST_SUBRANGE_TYP *range) const
{
  if (lpTypeDescr && lpTypeDescr->attrib==ARRAY_DEF)
    {
      assert(n>=0 && n<lpTypeDescr->ArrayDim);
      *range = lpTypeDescr->ArrayRange[n];
      return 1;
    }
  else
    return 0;
}

const ST_TypeSet_C &ST_TypeSet_C::GetArrayBase() const
{
  if (lpTypeDescr && lpTypeDescr->attrib==ARRAY_DEF && lpTypeDescr->lpBaseType)
    return *lpTypeDescr->lpBaseType;
  else {
    static const ST_TypeSet_C none = CG_TBS_EMPTY;
    return none;
  }
}

unsigned long ST_TypeSet_C::getArrayLength()const
{
    int             i;
    int             iRet;
    unsigned long   arrayLenTot=1;
    ST_SUBRANGE_TYP range;

    if(GetTypeSet()==CG_TBS_ARRAY)
    {
        assert(GetArrayDim()>0);
        for(i=0; i<GetArrayDim(); i++)
        {
            iRet = GetArrayRange(i, &range);
            assert(iRet);
            arrayLenTot *= (range.ulRangeLength + 1);
        }
    }
    else
    {
        assert(!"ST_TypeSet_C::getArrayLength(): non array");
    }
    
    return arrayLenTot;
}

const char *ST_TypeSet_C::GetTypeName() const /* if TYPESET is STRUCTURE|INSTANCE */
{
  if (lpTypeDescr && lpTypeDescr->attrib==TYPE_NAME)
    return lpTypeDescr->szTypeName;
  else
    return NULL;
}

int ST_TypeSet_C::GetSubrange(ST_SUBRANGE_TYP *range) const /* if TYPESET in ANY_INT */
{
  if (lpTypeDescr && lpTypeDescr->attrib==SUBRANGE_DEF)
    {
      *range = lpTypeDescr->Subrange;
      return 1;
    }
  else
    return 0;
}

int ST_TypeSet_C::CountBits(CG_BITSET l) /* helper */
{
  register CG_BITSET ul = l;
  register int result = 0;
  while (ul) {
    if (ul&1)
        result++;
    ul>>=1;
  }
  return result;
}

ST_TypeSet_C::TYPEDESCR_TYP *ST_TypeSet_C::CopyTypeDescr(const TYPEDESCR_TYP *typ)
{
  TYPEDESCR_TYP *result = NULL;
  if (typ)
    switch(typ->attrib) {
      case TYPE_NAME:
        result = (TYPEDESCR_TYP*)new char[offsetof(TYPEDESCR_TYP, szTypeName[_tcslen(typ->szTypeName)+1])];
        assert(result);
        if (result)
          {
            result->attrib = TYPE_NAME;
            strcpy(result->szTypeName, typ->szTypeName);
          }
        break;
      case SUBRANGE_DEF:
        result = new TYPEDESCR_TYP;
        assert(result);
        if (result)
          {
            result->attrib = SUBRANGE_DEF;
            result->Subrange = typ->Subrange;
          }
        break;
      case ARRAY_DEF:
        result = (TYPEDESCR_TYP*)new char[offsetof(TYPEDESCR_TYP, ArrayRange[typ->ArrayDim])];
        assert(result);
        if (result)
          {
            result->attrib = ARRAY_DEF;
            if (typ->lpBaseType)
              result->lpBaseType = new ST_TypeSet_C (*typ->lpBaseType);
            else
              result->lpBaseType = NULL; /* this should never happen */
            assert(result->lpBaseType);
            result->ArrayDim = typ->ArrayDim;
            for (int i=0; i<typ->ArrayDim; i++)
              result->ArrayRange[i] = typ->ArrayRange[i];
          }
        break;
      case STRING_LEN:
        result = new TYPEDESCR_TYP;
        assert(result);
        if (result)
          {
            result->attrib = STRING_LEN;
            result->stringLength = typ->stringLength;
          }
        break;
      default:
        assert(!"unknown tag");
    }
  return result;
}

int ST_TypeSet_C::IsEqualTypeDescr(const TYPEDESCR_TYP *typ1, const TYPEDESCR_TYP *typ2)
{
  if (typ1 != NULL && typ2 != NULL && typ1->attrib == typ2->attrib)
    switch(typ1->attrib) {
      default:
        assert(!"unknown tag");
        break;
      case TYPE_NAME:
        /***** NOTE: I assume here that struct/enum/function block class names are *not* case sensitive */
        return stricmp(typ1->szTypeName, typ2->szTypeName) == 0;
      case SUBRANGE_DEF:
        return typ1->Subrange.lRangeStart==typ2->Subrange.lRangeStart &&
               typ1->Subrange.ulRangeLength==typ2->Subrange.ulRangeLength;
      case STRING_LEN:
          return typ1->stringLength == typ2->stringLength;
      case ARRAY_DEF:
        if (typ1->ArrayDim == typ2->ArrayDim)
          {
            for (int i=0; i<typ1->ArrayDim; i++)
              if (!(typ1->ArrayRange[i].lRangeStart==typ2->ArrayRange[i].lRangeStart &&
                    typ1->ArrayRange[i].ulRangeLength==typ2->ArrayRange[i].ulRangeLength))
                return 0;
            return typ1->lpBaseType && typ2->lpBaseType && *typ1->lpBaseType == *typ2->lpBaseType;
          }
    }
  return typ1 == NULL && typ2 == NULL;
}











/**
 *   Best is SIGNED then UNSIGNED then BITSTRING
 *   Best is 32 Bit then 64 Bit then 16 Bit then 8 Bit then 1 Bit
 *   Best is LREAL then REAL
 *   Best WSTRING then STRING
 *   But in any case return only supported types!
 */
ST_TypeSet_C ST_SelectBestType(
    CGT_CDiagMsg*         pdm,
    const CFeatureSet*    pFeat,
    const ST_TypeSet_C&   type, 
    const CG_Edipos*      pEdiPos
)
{
    CG_BITSET lTypeSet = type.GetTypeSet();

    if(lTypeSet==CG_TBS_VOID || lTypeSet==CG_TBS_EMPTY || lTypeSet==CG_TBS_GOTOLABEL)//#AC
        return type;

    if(lTypeSet & (CG_TBS_ARRAY|CG_TBS_STRUCT|CG_TBS_ENUM|CG_TBS_INSTANCE|CG_TBS_PROGRAM))
        return type;

    lTypeSet &= pFeat->m_pCFeat->allTypes;

    if(pFeat->m_pCFeat->preferedFloat==64)
    {
        if(lTypeSet & CG_TBS_LREAL)
            return CG_TBS_LREAL;
        if(lTypeSet & CG_TBS_REAL)
            return CG_TBS_REAL;
    }
    else if(pFeat->m_pCFeat->preferedFloat==32)
    {
        if(lTypeSet & CG_TBS_REAL)
            return CG_TBS_REAL;
        if(lTypeSet & CG_TBS_LREAL)
            return CG_TBS_LREAL;
    }
    else
    {
        assert(!"bad best float size");
    }


    if(pFeat->m_pCFeat->preferedInt==32)
    {
        if(lTypeSet & CG_TBS_DINT)
            return CG_TBS_DINT;

        if(lTypeSet & CG_TBS_LINT)
            return CG_TBS_LINT;

        if(lTypeSet & CG_TBS_INT)
            return CG_TBS_INT;

        if(lTypeSet & CG_TBS_DWORD)
            return CG_TBS_DWORD;

        if(lTypeSet & CG_TBS_LWORD)
            return CG_TBS_LWORD;

        if(lTypeSet & CG_TBS_WORD)
            return CG_TBS_WORD;

        if(lTypeSet & CG_TBS_BYTE)
            return CG_TBS_BYTE;
    }
    else if(pFeat->m_pCFeat->preferedInt==16)
    {
        if(lTypeSet & CG_TBS_INT)
            return CG_TBS_INT;

        if(lTypeSet & CG_TBS_LINT)
            return CG_TBS_LINT;

        if(lTypeSet & CG_TBS_DINT)
            return CG_TBS_DINT;

        if(lTypeSet & CG_TBS_WORD)
            return CG_TBS_WORD;

        if(lTypeSet & CG_TBS_DWORD)
            return CG_TBS_DWORD;

        if(lTypeSet & CG_TBS_LWORD)
            return CG_TBS_LWORD;

        if(lTypeSet & CG_TBS_BYTE)
            return CG_TBS_BYTE;
    }
    else
    {
        assert(!"bad best int size");
    }

    if(lTypeSet & CG_TBS_BOOL)
        return CG_TBS_BOOL;

    if(lTypeSet & (CG_TBS_STRING|CG_TBS_WSTRING))
    {
        //string literals cannot be both:
        assert(lTypeSet==CG_TBS_STRING || lTypeSet==CG_TBS_WSTRING);
        //explicit string length should not be forgotten:
        if(type.GetStringLength()>=0)
            return ST_TypeSet_C(lTypeSet, type.GetStringLength());
        else
            return lTypeSet;            
    }

    if (!lTypeSet)
    {
        TCHAR buffer[MAX_TYPE_DUMP_BUFF];
        ST_TypeToString(buffer, type.GetTypeSet());
        pdm->msg1(CG_E_TYPE_NOT_SUPPORTED, pEdiPos, buffer);
    }

    return type;
}

const TCHAR* ST_BitsetToStr(FC_CString& buff, CG_BITSET type)
{
    TCHAR tmp[MAX_TYPE_DUMP_BUFF];
    
    ST_TypeToString(tmp, type);
    buff.load(tmp);

    return buff;
}

const char* ST_TypeToString(TCHAR szType[MAX_TYPE_DUMP_BUFF], CG_BITSET type)
{
    szType[0] = 0;

    if((type & CG_TBS_ANY) == CG_TBS_ANY)
    {
        strcat(szType, "|ANY");
        type &= ~CG_TBS_ANY;
    }
    if((type & CG_TBS_ANY_ELEMENTARY) == CG_TBS_ANY_ELEMENTARY)
    {
        strcat(szType, "|ANY_ELEMENTARY");
        type &= ~CG_TBS_ANY_ELEMENTARY;
    }
    if((type & CG_TBS_ANY_DERIVED) == CG_TBS_ANY_DERIVED)
    {
        strcat(szType, "|ANY_DERIVED");
        type &= ~CG_TBS_ANY_DERIVED;
    }
    if((type & CG_TBS_ANY_MAGNITUDE) == CG_TBS_ANY_MAGNITUDE)
    {
        strcat(szType, "|ANY_MAGNITUDE");
        type &= ~CG_TBS_ANY_MAGNITUDE;
    }
    if((type & CG_TBS_ANY_NUM) == CG_TBS_ANY_NUM)
    {
        strcat(szType, "|ANY_NUM");
        type &= ~CG_TBS_ANY_NUM;
    }
    if((type & CG_TBS_ANY_REAL) == CG_TBS_ANY_REAL)
    {
        strcat(szType, "|ANY_REAL");
        type &= ~CG_TBS_ANY_REAL;
    }
    if((type & CG_TBS_ANY_INT) == CG_TBS_ANY_INT)
    {
        strcat(szType, "|ANY_INT");
        type &= ~CG_TBS_ANY_INT;
    }
    if((type & CG_TBS_ANY_BIT) == CG_TBS_ANY_BIT)
    {
        strcat(szType, "|ANY_BIT");
        type &= ~CG_TBS_ANY_BIT;
    }
    if((type & CG_TBS_ANY_STRING) == CG_TBS_ANY_STRING)
    {
        strcat(szType, "|ANY_STRING");
        type &= ~CG_TBS_ANY_STRING;
    }
    if((type & CG_TBS_ANY_DATE) == CG_TBS_ANY_DATE)
    {
        strcat(szType, "|ANY_DATE");
        type &= ~CG_TBS_ANY_DATE;
    }
    if((type & CG_TBS_ANY_QVT) == CG_TBS_ANY_QVT)
    {
        strcat(szType, "|ANY_QVT");
        type &= ~CG_TBS_ANY_QVT;
    }
  
    if (type & CG_TBS_VOID)   //##AC
        strcat(szType, "|VOID");

    if (type & CG_TBS_BOOL)
        strcat(szType, "|BOOL");
    if (type & CG_TBS_BYTE)
        strcat(szType, "|BYTE");
    if (type & CG_TBS_WORD)
        strcat(szType, "|WORD");
    if (type & CG_TBS_DWORD)
        strcat(szType, "|DWORD");
    if (type & CG_TBS_LWORD)
        strcat(szType, "|LWORD");
    if (type & CG_TBS_REAL)
        strcat(szType, "|REAL");
    if (type & CG_TBS_LREAL)
        strcat(szType, "|LREAL");
    if (type & CG_TBS_SINT)
        strcat(szType, "|SINT");
    if (type & CG_TBS_INT)
        strcat(szType, "|INT");
    if (type & CG_TBS_DINT)
        strcat(szType, "|DINT");
    if (type & CG_TBS_LINT)
        strcat(szType, "|LINT");
    if (type & CG_TBS_USINT)
        strcat(szType, "|USINT");
    if (type & CG_TBS_UINT)
        strcat(szType, "|UINT");
    if (type & CG_TBS_UDINT)
        strcat(szType, "|UDINT");
    if (type & CG_TBS_ULINT)
        strcat(szType, "|ULINT");
    
    if (type & CG_TBS_DT)
        strcat(szType, "|DT");
    if (type & CG_TBS_TOD)
        strcat(szType, "|TOD");
    if (type & CG_TBS_DATE)
        strcat(szType, "|DATE");
    if (type & CG_TBS_TIME)
        strcat(szType, "|TIME");
    
    if (type & CG_TBS_STRING)
        strcat(szType, "|STRING");
    if (type & CG_TBS_WSTRING)
        strcat(szType, "|WSTRING");
    

    if (type & CG_TBS_QVT_BOOL)
        strcat(szType, "|QVT_BOOL");
    if (type & CG_TBS_QVT_BYTE)
        strcat(szType, "|QVT_BYTE");
    if (type & CG_TBS_QVT_WORD)
        strcat(szType, "|QVT_WORD");
    if (type & CG_TBS_QVT_DWORD)
        strcat(szType, "|QVT_DWORD");
    if (type & CG_TBS_QVT_LWORD)
        strcat(szType, "|QVT_LWORD");
    if (type & CG_TBS_QVT_REAL)
        strcat(szType, "|QVT_REAL");
    if (type & CG_TBS_QVT_LREAL)
        strcat(szType, "|QVT_LREAL");
    if (type & CG_TBS_QVT_SINT)
        strcat(szType, "|QVT_SINT");
    if (type & CG_TBS_QVT_INT)
        strcat(szType, "|QVT_INT");
    if (type & CG_TBS_QVT_DINT)
        strcat(szType, "|QVT_DINT");
    if (type & CG_TBS_QVT_LINT)
        strcat(szType, "|QVT_LINT");
    if (type & CG_TBS_QVT_USINT)
        strcat(szType, "|QVT_USINT");
    if (type & CG_TBS_QVT_UINT)
        strcat(szType, "|QVT_UINT");
    if (type & CG_TBS_QVT_UDINT)
        strcat(szType, "|QVT_UDINT");
    if (type & CG_TBS_QVT_ULINT)
        strcat(szType, "|QVT_ULINT");

    if (type & CG_TBS_QVT_DT)
        strcat(szType, "|QVT_DT");
    if (type & CG_TBS_QVT_TOD)
        strcat(szType, "|QVT_TOD");
    if (type & CG_TBS_QVT_DATE)
        strcat(szType, "|QVT_DATE");
    if (type & CG_TBS_QVT_TIME)
        strcat(szType, "|QVT_TIME");

    if (type & CG_TBS_QVT_STRING)
        strcat(szType, "|QVT_STRING");
    if (type & CG_TBS_QVT_WSTRING)
        strcat(szType, "|QVT_WSTRING");

    if (type & CG_TBS_INSTANCE)
        strcat(szType, "|INSTANCE");
    if (type & CG_TBS_STRUCT)
        strcat(szType, "|STRUCTURE");
    if (type & CG_TBS_PROGRAM)
        strcat(szType, "|PROGRAM");
    if (type & CG_TBS_ARRAY)
        strcat(szType, "|ARRAY");
    if (type & CG_TBS_ENUM)
        strcat(szType, "|ENUM");

    if (type & CG_TBS_GOTOLABEL)
        strcat(szType, "|GOTOLABEL");

    if (!szType[0])
        strcat(szType, "|<no type>"); //##AC

    int iLen = _tcslen(szType);
    assert(iLen<MAX_TYPE_DUMP_BUFF);
    memmove(szType, szType+1,iLen);

    return szType;
}


//##AC use  CGT_ enum <=> bit set
