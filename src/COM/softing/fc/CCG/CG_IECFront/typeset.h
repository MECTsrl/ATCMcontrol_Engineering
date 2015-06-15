#ifndef TYPESET_H
#define TYPESET_H






class CGT_CDiagMsg;
class CFeatureSet;

/*======================== ST_TypeSet_C ====================== */
/*
 * class ST_TypeSet_C
 * purpose: handle any set of IEC Types (based on iecdefs.h)
 * including ANY_INT, ANY_SIMPLE, ANY_ARRAY, ANY_STRUCT, ANY_ANYTHING...
 * plus      sets of fully qualified ARRAYs/STRUCTs [which may
 *           contain only *one* element]
 * plus      subranges of ANY_INT [currently not used]
 *           eg. SINT is a proper super set of SINT(-10..10)
 */





struct ST_SUBRANGE_TYP {
  long          lRangeStart;    /* first element of range */
  unsigned long ulRangeLength;  /* number of elements in range - 1 */
};

class ST_TypeSet_C{ /* do not overload this! I want to make arrays of ST_TypeSet_C! */
  private:
    enum ATTRIB_TYP {TYPE_NAME, SUBRANGE_DEF, ARRAY_DEF, STRING_LEN};
    /* TYPE_NAME only for ST_STRUCT|ST_INSTANCE */
    /* SUBRANGE_DEF only for ST_ANY_INT */
    /* ARRAY_DEF only for ST_ARRAY */

//warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma warning( disable : 4200)
    struct TYPEDESCR_TYP {
      ATTRIB_TYP attrib; /* selects from the union */
      union {
        char szTypeName[1];
        ST_SUBRANGE_TYP Subrange;
        struct {
          ST_TypeSet_C *lpBaseType;
          int ArrayDim;
          ST_SUBRANGE_TYP ArrayRange[];
        };
        int  stringLength;
      };
    };
#pragma warning( default : 4200)

    static int CountBits(CG_BITSET l); /* helper */
    static TYPEDESCR_TYP *CopyTypeDescr(const TYPEDESCR_TYP *typ);
    static int IsEqualTypeDescr(const TYPEDESCR_TYP *typ1, const TYPEDESCR_TYP *typ2);

    CG_BITSET      lTypeSet;
    TYPEDESCR_TYP *lpTypeDescr;

  public:
    ST_TypeSet_C(); /* empty TYPE */
    ST_TypeSet_C(CG_BITSET lType); /* any TYPE */
    ST_TypeSet_C(CG_BITSET lType, int iLen); /* ST_ANY_STRING with string length */
    ST_TypeSet_C(CG_BITSET lType, const ST_SUBRANGE_TYP *range); /* SUBRANGE, lType in ANY_INT */
    ST_TypeSet_C(CG_BITSET lType, const char *szName); /* STRUCTURE|INSTANCE */
    ST_TypeSet_C(CG_BITSET lType, int dim, const ST_SUBRANGE_TYP *range, 
                 const ST_TypeSet_C &BaseType); /* ARRAY, if dim==0 and range==NULL assumed size ARRAY */
    ST_TypeSet_C(const ST_TypeSet_C &that);
    void initFromCG_DType(const CG_DType* pType);//init from COM interface type description

    ~ST_TypeSet_C();

    ST_TypeSet_C &operator =(const ST_TypeSet_C &that);
    ST_TypeSet_C &operator &=(const ST_TypeSet_C &that);
    int operator == (const ST_TypeSet_C &that) const;
    int operator != (const ST_TypeSet_C &that) const;

    int IsEmptyType() const;
    int IsFBorDUT() const{ return lTypeSet==CG_STRUCT || lTypeSet==CG_INSTANCE; }
    int IsMonomorphType() const;
    CG_BITSET GetTypeSet() const { return lTypeSet; }
    bool hasExtTypeDescr()const { return lpTypeDescr!=NULL;}

    int GetArrayDim() const; /* if TYPESET is ARRAY */
    int GetStringLength()const//return -1 or if [len] >=0
                              //CAUTION: we need 0 as length for lit ""!!
    { 
        return lpTypeDescr && lpTypeDescr->attrib==STRING_LEN ? 
               lpTypeDescr->stringLength : -1;

    }    
    int GetArrayRange(int n, ST_SUBRANGE_TYP *range) const;
    const ST_TypeSet_C &GetArrayBase() const;
    unsigned long getArrayLength()const;


    const TCHAR* GetTypeName() const; /* if TYPESET is STRUCTURE|INSTANCE */
    int GetSubrange(ST_SUBRANGE_TYP *range) const; /* if TYPESET in ANY_INT */
};


ST_TypeSet_C ST_SelectBestType(
    CGT_CDiagMsg*       pdm,
    const CFeatureSet*  pFeat, 
    const ST_TypeSet_C& type, 
    const CG_Edipos*    pEdiPos
);


//useful:
const int MAX_TYPE_DUMP_BUFF = 256;
const TCHAR* ST_BitsetToStr(FC_CString& buff, CG_BITSET type);
const TCHAR* ST_TypeToString(TCHAR buffer[MAX_TYPE_DUMP_BUFF], CG_BITSET type);

#define ST_DTypesToTBS(dt)   CGT_DTypesToTBS(dt) //#AC
#define ST_TBSToDTypes(type) CGT_TBSToDTypes(type) //##AC


#endif // TYPESET_H
