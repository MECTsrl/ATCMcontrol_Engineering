


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <tchar.h>
#include <typeinfo.h>
#include "helper.h"
#include "LDFBD.h"
#include "Hash4GR.h"
#include "LDFBDTools.h"
#include "expr.h"
#include "st_ParseContext.h"
//----  Static Initializations:   ----------------------------------*





							
//classes used to check & compile FBD/LD 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef Position EvaluationOrder;
class Network;
class LDFBDBody;
class VarReadBox;
class NetworkElement;


struct PINBOX_TYP
{
    NetworkElement* pNWE;
    PinElement*     pPin;
};
class Value
{
    //context info members:
    const PinElement*       m_pCreatorPin;
    NetworkElement*         m_pCreatorNWE;

    //network info members:
    bool                    m_bEvaluated;
    Network*                m_pNetwork;
    FC_CGArray<PINBOX_TYP>  m_inPinsConnected;//pInsConnected contains all in pin connected
                                              //even the indirect connections. 
                                              //Example:  
                                              //value-inoutin--inoutout--inoutin--inoutout--varRead
                                              //pInsConnected->getUsed()==3

    bool                    m_bPowerRailTrue; //true: value is literal true from ladder Power.

    //code gen members:
    ST_TypeSet_C            m_datatype;
    int                     m_iTmpVarNumber;
    const NetworkElement*   m_pImmediateResult;
    Value*                  m_pAliasVal;
    bool                    m_bReferenceTmpVar;//if true: a tmp var will only hold 
                                               //a reference to the defining expr.
    const CG_ExprTree*      m_pReferencedExpr; //must be set before get first tmp
                                               //of a reference type, this is the 
                                               //expresion where the ref points to.
public:
    Value(
        const PinElement*     pCreatorPin, 
        NetworkElement*       pCreatorNWE, 
        int                   nInPinsConnected
    ) 
        : m_bEvaluated(false), 
          m_pCreatorPin(pCreatorPin),
          m_pCreatorNWE(pCreatorNWE),
          m_pNetwork(NULL), 
          m_inPinsConnected(nInPinsConnected),
          m_iTmpVarNumber(0),
          m_pImmediateResult(NULL),
          m_bPowerRailTrue(false),
          m_bReferenceTmpVar(false),
          m_pReferencedExpr(NULL),
          m_pAliasVal(NULL)
    {
        //assert: must be out pin or special case unconnected EN may create its own
        //value, see when Value::setPowerRailTrue() is called...
        assert(pCreatorPin->lPinType & (ANY_OUT_PIN|PIN_TYPE_EN));
    }
    virtual ~Value() {}



    void setNetwork(Network *pNetwork)
    {
        assert(pNetwork && (!m_pNetwork || m_pNetwork==pNetwork));
        m_pNetwork = pNetwork;
    }
    void addInPin(PinElement* pInPin, NetworkElement* pNWE)
    {
        PINBOX_TYP tmp = {pNWE, pInPin};
        assert(pInPin->lPinType & ANY_IN_PIN);
        assert(m_inPinsConnected.getUsed() < m_inPinsConnected.getMaxAlloc());
        m_inPinsConnected.add(tmp);
    }
    
    void  setDataType(CG_BITSET lTypeSet)               {m_datatype = lTypeSet;}
    void  setDataType(const ST_TypeSet_C& typeSet)      {m_datatype = typeSet;}
    void  intersectDataType(const ST_TypeSet_C& typeSet){m_datatype &= typeSet;}
    const ST_TypeSet_C& getDataType()const{return m_datatype;}

    bool isEvaluated()const{return m_bEvaluated;}
    void setEvaluated(){assert(!m_bEvaluated);m_bEvaluated=true;} 
    
    void setImmediateResult(const NetworkElement* p)
    {
        assert( p && m_iTmpVarNumber==0 && !m_pImmediateResult);
        m_pImmediateResult = p;
    }
    void setReferenceTmpVar()
    {
        assert(!m_bReferenceTmpVar && !m_pReferencedExpr);
        m_bReferenceTmpVar = true;
    }
    bool isReferenceTmpVar()
    {
        return m_bReferenceTmpVar;
    }
    void setReferencedExpr(const CG_ExprTree* pToThis)
    {
        assert(m_bReferenceTmpVar && !m_pReferencedExpr && pToThis);
        m_pReferencedExpr = pToThis;
    }

    void setAliasVal(Value* p)
    {
        assert(p && m_iTmpVarNumber==0 && !m_pImmediateResult && !m_pAliasVal);
        
        for(m_pAliasVal = p; m_pAliasVal->m_pAliasVal; m_pAliasVal = m_pAliasVal->m_pAliasVal)
            ;
        m_pAliasVal->m_inPinsConnected.append(&m_inPinsConnected);
    }
    void setPowerRailTrue(PinElement* pInPin, NetworkElement* pNWE)
    {
        assert((pInPin->lPinType &PIN_TYPE_EN)!=0 && !m_bPowerRailTrue && !m_pAliasVal || !m_pImmediateResult);
        m_bEvaluated = true;
        m_bPowerRailTrue = true;
        m_datatype = CG_TBS_BOOL;
        addInPin(pInPin, pNWE);
    }

    Network*                      getNetwork()const        {return m_pNetwork;}
    bool                          isImmediateResult()      {return m_pImmediateResult || m_pAliasVal;}
    Value*                        hasAliasVal()const       {return m_pAliasVal;}
    NetworkElement*               getCreatorNWE()const     {return m_pCreatorNWE;}
    const PinElement*             getCreatorPin()const     {return m_pCreatorPin;}
    const FC_CGArray<PINBOX_TYP>* getInPinsConnected()const{return &m_inPinsConnected;}
    inline bool                   isPowerRailTrue()const;
    

    int                   defineTmpVar(LDFBDBody* pBody);
    ST_Expression_C*      newVarExpr(LDFBDBody* pBody);
    const NetworkElement* getImmediateResultNetwork(){return m_pImmediateResult;}
    int                   getTmpVarNumber()          {return m_iTmpVarNumber;}
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class NetworkElement
{
protected:
    CG_Edipos             m_edp;                //position info for diagnostics of this box. 
    BoxElement*           m_pBoxElem;           //basic box info struct from XML input  
    bool                  m_bTypeError;         //true if boy has any type error msg done
    const FC_CGArray<CG_BITSET>*
                          m_pDifferentPolyTypes;//NULL or array containing all 
                                                //different polymorphic pin types 
    CGT_CDiagMsg*         m_pdm;
    st_ParseContext*      m_ppc;
    bool                  m_isOptimized;        //set true in optimize1, then don't call generateCode
private:
    Network*              m_pNetwork;
    bool                  m_bEvaluated;
    const NetworkElement* m_pEnEnoCondition;


public:
    //note: m_extentWithConnectedLines:
    //During init and init2 this extend
    //will grow as lines are connected.
    //It is used to determine the evaluation order
    //of NWs, not the order inside NW! 
    //Evaluation order inside NW is controlled by
    //m_pBoxElem->ext.ul (upper left corner of box)    
    Extension m_extentWithConnectedLines; 

    
    NetworkElement(BoxElement* pBoxElem, LDFBDBody* pBody);
    virtual ~NetworkElement(){}

    int                 findNetwork();
    void                setNetwork(Network *pNetwork);
    Network*            getNetwork()const  {return m_pNetwork;}
    const BoxElement*   getBox()const      {return m_pBoxElem;}
    const CG_Edipos*    getEdiPos()const   {return &m_edp;}
    Position            getPosition()const 
    {
        Position p={m_pBoxElem->ext.left, m_pBoxElem->ext.top}; 
        return p;
    }
    bool                hasTypeError()const{return m_bTypeError;}
    virtual PinElement* getEN()const       {return NULL;}
    virtual PinElement* getENO()const      {return NULL;}
    void                setENENOCondition(const NetworkElement* pEnEnoNWE)
    {
        assert(!m_pEnEnoCondition);
        m_pEnEnoCondition = pEnEnoNWE;
    }
    const NetworkElement* getENENOCondition()const{return m_pEnEnoCondition;}
    //canBeEvaluated returns true if this box is ready to be evaluated, 
    //but false if it has been already evaluated.
    bool canBeEvaluated()const;
    bool isEvaluated()const{return m_bEvaluated;}
    void setEvaluated();
    
    //isLvalue must not be called before initTypeResolution!!
    virtual bool isLvalue()const{return false;}

    //get temp vars for every output:
    bool init(LDFBDBody* pBody);
    //find temp vars for every input:
    bool init2(LDFBDBody* pBody);

    //parse expressions of varin/out boxes, get definitions of FBIs/FUNs
    //and set the initial data type of all temp vars. The initial datatype is
    //taken from the output pin that created the temp vars in init1
    //As an important second effect all Value::setAliasVal calles are 
    //made here two (except thoses for EN-ENO pairs)
    virtual bool initTypeResolution(LDFBDBody* pBody)=0;

    //check datatypes of all values connected to boxes 
    //if at least one datatype of a value gets 'less polymorph'
    //the function retunrs true, this means new type information
    //must be propagated through the network.
    bool typeResolutionIteration(LDFBDBody* pBody, int* pnTypeErrors);


    //if there is still a polymorph tmp var fix the type with SelectBestType()
    virtual void fixTypes(LDFBDBody* pBody);

    //call fixType2 after fixType to propagate new type info down the 
    //expression inside varin/out boxes. after fixType2 and if there are no 
    //errors you may call generateCode.
    virtual void fixTypes2(LDFBDBody* pBody){/* defauft do nothing*/}

    //optimize tmp var usage if 
    //a) VarRead contains a literal
    //b) VarRead/VarWrite is box directly connected to in/output pin
    virtual void optimize1(LDFBDBody* pBody){/*default do nothing*/}

    bool isOptimised()const{return m_isOptimized;}
    //call generateCode after: 
    //init->init2->initTypeResolution->N calls to typeResolutionIteration
    //->fixTypes->fixTypes2->optimize1
    //But only if isOptimised()==false:
    //generateCode produces the final ST Statements and handels the BP stuff.
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)=0;
    
    //call after generateCode, handle return and jump stuff:
    virtual CG_STStmt* postProcess(LDFBDBody* pBody){return NULL;/* default do nothing*/}
};


inline bool Value::isPowerRailTrue()const
{
    return m_bPowerRailTrue || m_pCreatorNWE->getBox()->boxType==BOX_TYPE_POWERRAIL;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class VarReadBox : public NetworkElement
{
    ST_Expression_C* m_pExpr;
    bool             m_passedInitTypeResolution;
public:
    VarReadBox(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody), 
        m_pExpr(NULL), m_passedInitTypeResolution(false)
    {
        assert(pB->pins.getUsed()==1 && pB->pins.get(0)->lPinType==PIN_TYPE_OUT);
        assert(pB->boxType==BOX_TYPE_VARREAD  || pB->boxType==BOX_TYPE_VARFEEDBACK);
    }
    ~VarReadBox()
    {
        delete(m_pExpr);
    }
    virtual bool isLvalue()const
    {
        assert(m_pExpr || m_ppc->getErrorsInFile()>0);
        return m_pExpr ? m_pExpr->is_Lvalue()!=0 : false;
    }
    virtual bool initTypeResolution(LDFBDBody* pBody);
    bool hasPassedInitTypeResolution(){return m_passedInitTypeResolution;}
    virtual void fixTypes(LDFBDBody* pBody);
    virtual void fixTypes2(LDFBDBody* pBody);
    virtual void optimize1(LDFBDBody* pBody);

    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class VarWriteBox : public NetworkElement
{
protected:
    ST_Expression_C* m_pExpr;
public:
    VarWriteBox(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody), m_pExpr(NULL)
    {
        assert(pB->pins.getUsed()==1 && pB->pins.get(0)->lPinType==PIN_TYPE_IN);
        assert(pB->boxType==BOX_TYPE_VARWRITE || pB->boxType==BOX_TYPE_VARFEEDBACK);
    }
    ~VarWriteBox()
    {
        delete(m_pExpr);
    }
    virtual bool isLvalue()const
    {
        assert(m_pExpr || m_ppc->getErrorsInFile()>0);
        return m_pExpr ? m_pExpr->is_Lvalue()!=0 : false;
    }
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual void fixTypes2(LDFBDBody* pBody);
    virtual void optimize1(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FeedBackVarOutBox : public VarWriteBox
{
    VarReadBox*        m_pFeedbackVarInBox;
public:
    FeedBackVarOutBox(BoxElement* pB, VarReadBox* pFeedIn, LDFBDBody* pBody) : 
      VarWriteBox(pB, pBody), 
        m_pFeedbackVarInBox(pFeedIn)
    {
        assert(pFeedIn && pB->boxType==BOX_TYPE_VARFEEDBACK);
    }
    virtual bool initTypeResolution(LDFBDBody* pBody);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FBBox : public NetworkElement
{
    const CG_SymEntry* m_pFBIEntry;
    //both !=NULL if EN/ENO:
    PinElement*        m_pEN; 
    PinElement*        m_pENO;
public:
    FBBox(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody), 
        m_pFBIEntry(NULL), m_pEN(NULL), m_pENO(NULL){}
    ~FBBox(){}
    virtual PinElement* getEN() const{return m_pEN;}
    virtual PinElement* getENO()const{return m_pENO;}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FunBox : public NetworkElement
{
    ST_CSymtabPrv*       m_pFunType;
    int                  m_nInputs;
    int                  m_nOutputs;
    PinElement*          m_pEN; 
    PinElement*          m_pENO;
    int                  m_nArgListOrderedPins;
    PinElement**         m_pArgListOrderedPins; //sorted pin array, sorted in the
                                                //ST function arglist order, first
                                                //array elem is function result, 
                                                //if fun result is void then 
                                                //m_pArgListOrderedPins[0]==NULL
                                                //size_is m_nArgListOrderedPins (<= nPins)

    bool initTypeInfo(LDFBDBody* pBody);
public:
    FunBox(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody), 
        m_pFunType(NULL), m_nInputs(-1), m_nOutputs(-1),
        m_pEN(NULL),m_pENO(NULL), m_nArgListOrderedPins(0), m_pArgListOrderedPins(NULL)
    {}
    ~FunBox()
    {
        delete(m_pArgListOrderedPins);
    }
    virtual PinElement* getEN() const{return m_pEN;}
    virtual PinElement* getENO()const{return m_pENO;}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WiredOr : public NetworkElement
{
    int m_nInputs;
    int m_nOutputs;
public:
    WiredOr(BoxElement* pB, LDFBDBody* pBody) 
        : NetworkElement(pB, pBody), m_nInputs(0), m_nOutputs(0){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Contact : public NetworkElement
{
    ST_Expression_C* m_pExpr;
public:
    Contact(BoxElement* pB, LDFBDBody* pBody) : 
      NetworkElement(pB, pBody), m_pExpr(NULL)
    {
        assert(pB->pins.getUsed()==2 && pB->pins.get(0)->lPinType==PIN_TYPE_IN
                                     && pB->pins.get(1)->lPinType==PIN_TYPE_OUT);
        assert(pB->boxType==BOX_TYPE_CONTACT);
    }
    ~Contact()
    {
        delete m_pExpr;
    }
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Coil : public NetworkElement
{
protected:
    ST_Expression_C* m_pExpr;
public:
    Coil(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody), m_pExpr(NULL)
    {
        assert(pB->pins.getUsed()==2 && pB->pins.get(0)->lPinType==PIN_TYPE_IN
                                     && pB->pins.get(1)->lPinType==PIN_TYPE_OUT);
        assert(pB->boxType==BOX_TYPE_COIL);
    }
    ~Coil()
    {
        delete m_pExpr;
    }
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Powerrail : public NetworkElement
{
public:
    Powerrail(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    void optimize1(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Return : public NetworkElement
{
public:
    Return(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
    virtual CG_STStmt* postProcess(LDFBDBody* pBody);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SFCReturn : public NetworkElement
{
public:
    SFCReturn(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
    virtual CG_STStmt* postProcess(LDFBDBody* pBody);
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Jump : public NetworkElement
{
public:
    Jump(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
    virtual CG_STStmt* postProcess(LDFBDBody* pBody);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Label : public NetworkElement
{
public:
    Label(BoxElement* pB, LDFBDBody* pBody) : NetworkElement(pB, pBody){}
    virtual bool initTypeResolution(LDFBDBody* pBody);
    virtual CG_STStmt* generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Network /*final*/
{
private:
    Extension                     m_extent;
    FC_CGArray<NetworkElement*>   m_NWelements;
    
public:
    Network()
    {
        m_extent.left = m_extent.top = m_extent.right = m_extent.bottom = -1;
    }
    void  add(NetworkElement *elem)
    {
        if(m_extent.left==-1)
            m_extent=elem->m_extentWithConnectedLines;
        else
            MaxExtension(&m_extent, elem->m_extentWithConnectedLines);

        m_NWelements.add(elem);
    }
    const Extension &getExtension() const{return m_extent;}

    CG_STStmt* generateNetworkCode(LDFBDBody* pBody);

    bool checkEnEnoUsage(
        LDFBDBody*                   pBody, 
        FC_CGArray<NetworkElement*>& NWEsortedByExecOrder,
        NetworkElement*              pNWEFirstEN
    );
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class LDFBDBody /*final*/
{
private:
    st_ParseContext*            m_ppc;        //generates COM IF structs
    LDFBD_SignalHash*           m_pGraphic;   //holds all graphical input information
    FC_CGArray<Network*>        m_networkList;//list of networks, sorted by evaluation order
    
    FC_CGArray<NetworkElement*> m_networkElemList;//list of intermediate objects
    FC_CStrList*                m_pMultipleWrites;
    CGT_CDiagMsg*               m_pdm;
    CG_CIECFront*               m_pFront;
    ST_CSymtabPrv*              m_pSym;
    long                        m_codeGenOpt;  //CG_LOADBDY_* bits
public:
    LDFBDBody(
        LDFBD_SignalHash* pGraphic, 
        st_ParseContext*  ppc,
        long              codeGenOpt  //CG_LOADBDY_* bits
    );
    ~LDFBDBody();

    FC_CGArray<Value*>          m_valueList;//list of tmp vars

    
    LDFBD_SignalHash* getGraphic()const{return m_pGraphic;}
    st_ParseContext* getPPC()const{return m_ppc;}

    long getCodeGenOpts()const    {return m_codeGenOpt;}
    CGT_CDiagMsg*  getDM()const   {return m_pdm;}
    CG_CIECFront*  getFront()const{return m_pFront;}
    ST_CSymtabPrv* getSym()const  {return m_pSym;}

    
    const CG_Edipos* boxEdp(CG_Edipos* pEdp, const Position& pos)const
    {
        InitBoxEdp(pEdp, m_ppc->getEDPSrcFile(), pos.x, pos.y);
        return pEdp;
    }
    const CG_Edipos* boxEdp(CG_Edipos* pEdp, const Extension& ext)const
    {
        InitBoxEdp(pEdp, m_ppc->getEDPSrcFile(), ext.left, ext.top);
        return pEdp;
    }
    const CG_Edipos* lineEdp(CG_Edipos* pEdp, const Extension& ext)const
    {
        InitLineEdp(pEdp, m_ppc->getEDPSrcFile(), ext.left, ext.top, ext.right, ext.bottom);
        return pEdp;
    }
    const CG_Edipos* signalEdp(CG_Edipos* pEdp, const Position& pos)const
    {
        InitSignalEdp(pEdp, m_ppc->getEDPSrcFile(), pos.x, pos.y);
        return pEdp;
    }
    void signalDiag0(HRESULT errId, const Position& pos)const;
    void signalDiag1(HRESULT errId, const Position& pos, const TCHAR* psz1)const;


    //get a signal and handle warnings if signal is cyclic 
    //or has unconnected lines
    void getSignalAndWarn(Position              start,
                          long                  pinType,
                          FC_CGArray<LineElement*>* pReachedLines,
                          FC_CGArray<PinElement*>*  pReachedPins,
                          Extension*            pExtension);
    

    //createNetworkElements return false only if hard error, 
    //where check must be aborted:
    bool createNetworkElements();
    
    void createNetworkList();
    CG_STStmt* genNetworkCode();
    
    ST_Expression_C* parseExpr(const BoxElement* pBox);

    bool setSignalFlag(Position start, long pinType, long lFlag);

    //reparseExpr is used to copy expression trees using the parser,
    //but no diagnostics will be displayed. You must(!) call parseExpr
    //before reparse, otherwise error messages will be lost. This situation
    //is detected by the error handling function and will cause an 
    //internal compiler error...
    ST_Expression_C* reparseExpr(const NetworkElement* pNWE);
    int getFreeTmp(CG_BITSET tbs, bool bRefTmpVar)
    {
        return m_ppc->getFreeTmp(tbs, bRefTmpVar);
    }
    ST_Expression_C* getNewTrueExpr(const CG_Edipos* pEdp)
    {
        ST_BINVAL_TYP*  pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, 1);
        return new ST_Literal_C(pEdp, m_ppc, pBinVal, CG_TBS_BOOL, true, NULL);
    }
    ST_Expression_C* getNewFalseExpr(const CG_Edipos* pEdp)
    {
        ST_BINVAL_TYP*  pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, 0);
        return new ST_Literal_C(pEdp, m_ppc, pBinVal, CG_TBS_BOOL, true, NULL);
    }

    //reporting errors/warnings, different for lines & boxes
    void diagnosticMsgL(const LineElement* pLine, HRESULT err_id);
    void diagnosticMsgRegion(const Extension& ext, HRESULT err_id);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class CStmtList
{
    CG_STStmt*            m_pFirst;
    CG_STStmt*            m_pLast;
public:
    CStmtList(){m_pFirst = m_pLast = NULL;} 

    void append(CG_STStmt* pStmt)
    {
        if(!m_pFirst)
            m_pFirst = pStmt;
        if(m_pLast)
            m_pLast->pNext = pStmt;

        m_pLast = pStmt;
        while(m_pLast->pNext)
            m_pLast = (CG_STStmt*)m_pLast->pNext;//HACK const cast
    }

    CG_STStmt* getFirst()const{return m_pFirst;}
    CG_STStmt* getLast()const{return m_pLast;}
};


class CENStmtList
{
    LDFBDBody*            m_pBody;
    CG_STStmt*            m_pFirst;
    CG_STStmt*            m_pLast;
    const NetworkElement* m_pFirstENsource;
    CG_STStmt*            m_pEnIfStmt;
public:
    CENStmtList(LDFBDBody* pBody)
        : m_pBody(pBody), m_pFirst(NULL), m_pLast(NULL), 
          m_pFirstENsource(NULL), m_pEnIfStmt(NULL)
    {}
   
    void append(
        const NetworkElement* pENsource,
        CG_STStmt*            pStmt
    );
    CG_STStmt* getFirst(){return m_pFirst;}
};

void CENStmtList::append(
    const NetworkElement* pENsource,
    CG_STStmt*            pStmt
)
{
    Value*           pENVal;
    ST_Expression_C* pCond;

    assert(pStmt || m_pBody->getPPC()->getErrorsInFile()>0);
    if(!pStmt)
        return;


    pENVal = NULL;
    if(pENsource)
    {
        pENVal = pENsource->getEN()->pValue;
        if(pENVal)
        {
            if(pENVal->isPowerRailTrue())
                pENVal = NULL;//is same as if unconnected...
        }//else: unconnected EN
    }


    if(pENVal)
    {
        if(!m_pFirstENsource)
            m_pFirstENsource = pENsource;

        if(pENsource != m_pFirstENsource)
            CGT_IERR2(m_pBody->getDM(), &pStmt->edp, _T("second EN/ENO flow not supported"));

        if(m_pEnIfStmt)
        {
            //the if(EN) is already done, append at last ifs.pThen position:
            assert(m_pEnIfStmt->styp==CG_IF);
            CG_STStmt** ppLastThen = (CG_STStmt**)&m_pEnIfStmt->ifs.pThen;
            while((*ppLastThen)->pNext)
                ppLastThen = (CG_STStmt**)&((*ppLastThen)->pNext);

            (*ppLastThen)->pNext = pStmt;
        }
        else
        {
            //get the EN value of the first EN/ENO box and create if(EN):
            pCond = pENVal->newVarExpr(m_pBody);
            m_pEnIfStmt = m_pBody->getPPC()->newIfElseStmt(pCond->getEdp(), pCond, pStmt, NULL);

            //add this if(EN) to the main stmt list of the network:
            if(!m_pFirst)
                m_pFirst = m_pEnIfStmt;
            if(m_pLast)
                m_pLast->pNext = m_pEnIfStmt;

            m_pLast = m_pEnIfStmt;
        }
    }
    else
    {
        //this stmt is not inside the if(EN)
        //set m_pEnIfStmt to NULL (can be set!!), it may happen that in 
        //the same network a next if(EN) follows this stmt.
        m_pEnIfStmt = NULL;

        if(!m_pFirst)
            m_pFirst = pStmt;
        if(m_pLast)
            m_pLast->pNext = pStmt;

        m_pLast = pStmt;
        while(m_pLast->pNext)
            m_pLast = (CG_STStmt*)m_pLast->pNext;//HACK const cast
    }
}










//static helper functions:
static inline const CG_Edipos* InitBoxEDP(CG_Edipos* pEdp, const LDFBDBody* pBody, const BoxElement* pBox)
{
    if (pBox->boxType==BOX_TYPE_WIREDOR
     || pBox->boxType==BOX_TYPE_POWERRAIL)
        return pBody->lineEdp(pEdp, pBox->ext);

    return pBody->boxEdp(pEdp, pBox->ext);
}


//++++++++++++++++++++Member functions of class Value+++++++++++++++++
int Value::defineTmpVar(LDFBDBody* pBody)
{
    assert(m_datatype.IsMonomorphType());
    assert(m_iTmpVarNumber==0 && !m_pAliasVal);
    
    if(!m_pImmediateResult)
    {
        m_iTmpVarNumber = pBody->getFreeTmp(m_datatype.GetTypeSet(), m_bReferenceTmpVar);
        assert(m_iTmpVarNumber>0);
        return m_iTmpVarNumber;
    }
    return -1;
}
ST_Expression_C* Value::newVarExpr(LDFBDBody* pBody)
{
    ST_Expression_C* pTmp;
    
    if(m_iTmpVarNumber==-1)
    {
        assert(pBody->getDM()->getTotalErrors()>0);
        pTmp = new ST_ErrorExpr_C(m_pCreatorNWE->getEdiPos(), pBody->getPPC(), m_datatype);
    }
    else if(m_pImmediateResult)
    {
        pTmp = pBody->reparseExpr(m_pImmediateResult);
        if(!pTmp)
            CGT_IERR2(pBody->getDM(), m_pCreatorNWE->getEdiPos(), _T("pBody->reparseExpr failed"));
    }
    else if(m_pAliasVal)
    {
        pTmp = m_pAliasVal->newVarExpr(pBody);
    }
    else if(m_bPowerRailTrue || m_pCreatorNWE->getBox()->boxType==BOX_TYPE_POWERRAIL)
    {
        return pBody->getNewTrueExpr(m_pCreatorNWE->getEdiPos());
    }
    else
    {
        assert(!m_bReferenceTmpVar || m_pReferencedExpr);
        pTmp = new ST_TmpVar_C(m_pCreatorNWE->getEdiPos(), pBody->getPPC(), m_iTmpVarNumber, m_datatype, m_pReferencedExpr, 0);
    }
    
    return pTmp;     
}



//++++++++++++++++++++Member functions of class NetworkElement+++++++++++++++++
NetworkElement::NetworkElement(BoxElement* pBoxElem, LDFBDBody* pBody) : 
  m_pNetwork(NULL),
  m_bEvaluated(false),  
  m_pBoxElem(pBoxElem), 
  m_bTypeError(false),
  m_pDifferentPolyTypes(NULL),
  m_extentWithConnectedLines(pBoxElem->ext), //<-initially m_extentWithConnectedLines 
                                            //is the Boy extend itself
  m_pdm(pBody->getDM()),
  m_pEnEnoCondition(NULL), 
  m_ppc(pBody->getPPC()),
  m_isOptimized(false)
{
    InitBoxEDP(&m_edp, pBody, m_pBoxElem);
}

/** bool NetworkElement::findNetwork()
 *  returns
 *    true    if this is added to a Network
 *    false   otherwise, already added to a network or none found
 *  uses m_var(In|Out)[i]->getNetwork()
 *  and calls this->setNetwork()
 */
int NetworkElement::findNetwork()
{
    int     i;
    PinElement* pPin;
    int         nPins;
    Network *pnewNetwork;
    
    if (m_pNetwork)
        return false;
    
    pnewNetwork = NULL;
    nPins = m_pBoxElem->pins.getUsed();

    for(i=0;i<nPins;i++) 
    {
        pPin = m_pBoxElem->pins.get(i);
        if(pPin->pValue)
        {
            pnewNetwork = pPin->pValue->getNetwork();
            if(pnewNetwork)
                break;
        }
    }
    if(pnewNetwork)
    {
        setNetwork(pnewNetwork);
        return true;
    }
    
    return false;
}
/** 
 *  void NetworkElement::setNetwork(Network *pNetwork)
 *  uses m_var(In|Out)[i]->setNetwork()
 *  sets all TempVar's network
 *  updates this->m_pNetwork
*/
void NetworkElement::setNetwork(Network *pNetwork)
{
    int         i;
    int         nPins;
    PinElement* pPin;

    assert(pNetwork);
    assert(!m_pNetwork);
    
    m_pNetwork = pNetwork;
    nPins = m_pBoxElem->pins.getUsed();
    
    for (i=0;i<nPins;i++) 
    {
        pPin = m_pBoxElem->pins.get(i);
        if (pPin->pValue)
            pPin->pValue->setNetwork(pNetwork);
    }
   
    pNetwork->add(this);
}
bool NetworkElement::canBeEvaluated()const
{
    int         i;
    int         nPins;
    PinElement* pPin;

    if(m_bEvaluated)
        return false;
    
    nPins = m_pBoxElem->pins.getUsed();

    for(i=0;i<nPins;i++)
    {
        pPin = m_pBoxElem->pins.get(i);

        if(pPin->lPinType & ANY_IN_PIN)
            if(pPin->pValue && !pPin->pValue->isEvaluated())
                return false;//at least one input is not yet evaluated
    }
    return true;//this box is ready for calculation...
}
void NetworkElement::setEvaluated()
{
    int         i;
    int         nPins;
    PinElement* pPin;

    assert(!m_bEvaluated);
    m_bEvaluated=true;
    nPins = m_pBoxElem->pins.getUsed();

    for(i=0;i<nPins;i++) 
    {
        pPin = m_pBoxElem->pins.get(i);
        if((pPin->lPinType & ANY_OUT_PIN) && pPin->pValue)
            pPin->pValue->setEvaluated();
    }
}
bool NetworkElement::init(LDFBDBody* pBody)
{
    int                        i;
    const int nPins =          m_pBoxElem->pins.getUsed();     
    LDFBD_SignalHash*          pGraphic = pBody->getGraphic();
    FC_CGArray<PinElement*>    reachedPins;
    int                        nPinsConnected;
    PinElement*                pPin;
    int                        nOutputsConnected;
    int                        nInputConnected;
    bool                       bSignalError = false;

    //create values for outputs now, inputs are done in second pass:
    for (i=0; i<nPins; i++)
    {
        pPin = m_pBoxElem->pins.get(i);
        assert(!pPin->pValue);
        if((pPin->lPinType & ANY_OUT_PIN)==0)
            continue;//skip all inputs

        //this pPin is any output:
        pBody->getSignalAndWarn(GET_PIN_COORDS(pPin), NULL, &reachedPins, &m_extentWithConnectedLines);
        nPinsConnected   = reachedPins.getUsed();
        assert(nPinsConnected>=1);//assert: at least the pin itself!

        if(nPinsConnected<=1)
            continue;//nothing todo if out pin not connected...

        //check reached pins:
        nOutputsConnected= 0;
        nInputConnected  = 0;
        while(--nPinsConnected>=0)
        {
            if(reachedPins.get(nPinsConnected)->lPinType & ANY_OUT_PIN)
                nOutputsConnected++;
            else
                nInputConnected++;
        }
        assert(nOutputsConnected>=1);//<-at least this input

        if(nOutputsConnected>1)
        {
            //don't display error for every pin connected to the signal, 
            //therefore we use pBody->setSignalFlag method:
            if(pBody->setSignalFlag(GET_PIN_COORDS(pPin), LINE_FLAG_SIGNAL_ERROR))
            {
                pBody->signalDiag0(CG_E_MULTIBLE_OUTPUTS, pPin->pos);
            }
            bSignalError = true;
        }

        //set the pValue pointer: 
        assert(!pPin->pValue);
        pPin->pValue = new Value(pPin, this, nInputConnected);
        pBody->m_valueList.add(pPin->pValue);
    }

    return !bSignalError;
}
bool NetworkElement::init2(LDFBDBody* pBody)
{
    int                        i;
    const int nPins =          m_pBoxElem->pins.getUsed();     
    LDFBD_SignalHash*          pGraphic = pBody->getGraphic();
    FC_CGArray<PinElement*>    reachedPins(16);
    PinElement*                pPin;
    PinElement*                pOutPinConnected;
    int                        nPinsConnected;
    int                        nOutputsConnected;
    int                        nInputConnected;
    bool                       bSignalError = false;


    //create values for inputs:
    for (i=0; i<nPins; i++)
    {
        pPin = m_pBoxElem->pins.get(i);
        if((pPin->lPinType & ANY_IN_PIN)==0)
            continue;
        
        assert(!pPin->pValue);

        pBody->getSignalAndWarn(GET_PIN_COORDS(pPin), NULL, &reachedPins, &m_extentWithConnectedLines);
        nPinsConnected   = reachedPins.getUsed();
        assert(nPinsConnected>=1);//assert: at least the pin itself!

        if(nPinsConnected<=1)
        {
            //special case EN is left open:
            //create virtual value 'power rail true', this way we void the 
            //problem of undefined pValue pointers and can handle the special case
            //here once and for ever (hopefully). 
            //The Value::newVarExpr method will optimize anyway.
            if(pPin->lPinType & PIN_TYPE_EN)
            {
                pPin->pValue = new Value(pPin, this, 1);
                pPin->pValue->setPowerRailTrue(pPin, this);
                pBody->m_valueList.add(pPin->pValue);
            }

            continue;//nothing todo if in pin not connected...
        }

        //check reached pins:
        pOutPinConnected = NULL;
        nOutputsConnected= 0;
        nInputConnected  = 0;
        
        while(--nPinsConnected>=0)
        {
            PinElement* pTmp = reachedPins.get(nPinsConnected);
            if(pTmp->lPinType & ANY_OUT_PIN)
            {
                nOutputsConnected++;
                //if more than one remember last output found, 
                //this is an error anyway...
                pOutPinConnected = pTmp;
            }
            else
            {
                nInputConnected++;
            }
        }
        assert(nInputConnected>=1);//<-at least this input

        if(nOutputsConnected!=1)
        {
            //don't display error for every pin connected to the signal, 
            //therefore we use pBody->setSignalFlag method:
            if(pBody->setSignalFlag(GET_PIN_COORDS(pPin), LINE_FLAG_SIGNAL_ERROR))
            {
                pBody->signalDiag0(
                    nOutputsConnected>1 ? 
                       CG_E_MULTIBLE_OUTPUTS : 
                       CG_E_NO_OUTPUTS, pPin->pos);
            }
            bSignalError = true;
        }

        //if any output found set pPin->pValue to 
        //the output's pValue. 
        if(pOutPinConnected)
        {
            //assert: out pins must have been processed in ::init1
            assert(pOutPinConnected->pValue);
            pPin->pValue = pOutPinConnected->pValue;
            pOutPinConnected->pValue->addInPin(pPin, this);
        }
    }

    return !bSignalError;
}
bool NetworkElement::typeResolutionIteration(LDFBDBody* pBody, int* pnTypeErrors)
{
    int                n;
    Value*             pVal;
    int                nPins = m_pBoxElem->pins.getUsed();
    const PinElement*  pPin;
    bool               bReducedPolymorphism = false;
    ST_TypeSet_C       test;

    assert(!m_bTypeError);
    
    //1. check simple case that signal type  matches the pin formal type
    //if( signalType & pinformaltype) != 0) then  
    //    signalType &= pinformaltype
    //    
    //else
    //    error msg
    //
    for(n=0;n<nPins;n++)
    {
        pPin = m_pBoxElem->pins.get(n);
        pVal = pPin->pValue;

        if(pVal==NULL)
            continue;//nothing connected, continue;

        //check if signal does match with formal in/out pin type:
        test = *pPin->pFormalParamType;
        test &= pVal->getDataType();
        if(test.IsEmptyType())
        {
            //connected signal does not match the formal input type:
            //try to produce a meaningful diagnostic:
            m_pdm->msg3(CG_E_TYPE_MISMATCH_SIMPLE, &m_edp,
                      CreatePinErrorMsgName(m_pdm->getResHMODULE(), m_pBoxElem, pPin), 
                      ST_TypeSetToErrMsgString(*pPin->pFormalParamType), 
                      ST_TypeSetToErrMsgString(pVal->getDataType()));
            
            m_bTypeError = true;
            continue;//do not intersect with bad type continue next pin. Don't 
                     //care if this arg is a polymorph one, one error is enough...
        }

        if(pVal->getDataType() != test)
        {   //if != the polymorphism has been reduced:
            bReducedPolymorphism = true;
            pVal->intersectDataType(test);

            //assert: algorithm should not produce tmp vars with empty type
            //even after errors, this assertion checks mainly the error tolerance
            assert(!pVal->getDataType().IsEmptyType());
        }
        
    }
            

    


    //2. handle polymorphic stuff if any, but only if no simple error so far:
    if(!m_bTypeError && m_pDifferentPolyTypes)
    {
        //loop all different poly types:
        for(int ipt=m_pDifferentPolyTypes->getUsed()-1;ipt>=0;ipt--)
        {
            const CG_BITSET polyTypeToProcess = m_pDifferentPolyTypes->get(ipt);

            ST_TypeSet_C intersectedType = polyTypeToProcess;
            for(n=0;n<nPins;n++)
            {
                pPin = m_pBoxElem->pins.get(n);
                pVal = pPin->pValue;
                if(pVal && pPin->pFormalParamType->GetTypeSet()==polyTypeToProcess)
                    intersectedType &= pVal->getDataType().GetTypeSet();
            }


            if(intersectedType==0)
            {
                //not all connected signals have the same type.
                pBody->getDM()->msg1(CG_E_TYPE_MISMATCH_POLY, &m_edp, 
                    ST_TypeSetToErrMsgString(polyTypeToProcess));
                m_bTypeError = true;
                continue;//do not propagate empty type...
            }
        
            //propagate the intersection to all pins with same formal type:
            for(n=0;n<nPins;n++)
            {
                pPin = m_pBoxElem->pins.get(n);
                pVal = pPin->pValue;
                if(pVal && pPin->pFormalParamType->GetTypeSet()==polyTypeToProcess)
                {
                    if(intersectedType != pVal->getDataType().GetTypeSet())
                    {
                        bReducedPolymorphism = true;
                        pVal->intersectDataType(intersectedType);
                        
                        //assert: algorithm should not produce tmp vars with empty type
                        //even after errors, this assertion checks mainly the error tolerance
                        assert(!pVal->getDataType().IsEmptyType());
                    }
                }
            }
        }
    }
   
    if(m_bTypeError)
        (*pnTypeErrors)++;

    return bReducedPolymorphism;
}
void NetworkElement::fixTypes(LDFBDBody* pBody)
{
    int                        i;
    const int nPins =          m_pBoxElem->pins.getUsed();     
    PinElement*                pPin;
    Value*                     pValue;
    int                        n_outs=0;

    //if any in/out pin is still polymorph select best type & warn:
    for (i=0; i<nPins; i++)
    {
        pPin = m_pBoxElem->pins.get(i);
        
        //##d-1811: use alias if any:
        pValue = pPin->pValue;
        if(pValue && pValue->hasAliasVal())//##d-1811: HACK correct solution is to 
            pValue = pValue->hasAliasVal();//propagate the type to the pins Value too as for functions

        if(pValue && (pPin->lPinType & ANY_OUT_PIN))
        {
            //this pin defines a value var, check the data type:
            if(!pValue->getDataType().IsMonomorphType())
            {
                //assert: don't call this function if hard errors:
                assert(!pValue->getDataType().IsEmptyType());

                n_outs++;
                //assert: need other err msg if we support more than one fun result!
                assert(n_outs==1); 
                
                pBody->getDM()->msg2(CG_W_TYPE_RESULT, &m_edp,
                    m_pBoxElem->text, ST_TypeSetToErrMsgString(pValue->getDataType()));
                
                //fix type:
                pValue->setDataType(
                    ST_SelectBestType(
                          pBody->getDM(),
                          &pBody->getFront()->m_features,
                          pValue->getDataType(), 
                          &m_edp)
                              );
            }
        }
    }
}





//++++++++++++++++++++Member functions of class VarReadBox++++++++++++++++++++++++++++
bool VarReadBox::initTypeResolution(LDFBDBody* pBody)
{
    PinElement* pPin = m_pBoxElem->pins.get(0);

    assert(!m_passedInitTypeResolution && !m_pExpr);
    m_passedInitTypeResolution = true;

    if(!pPin->pValue)
    {
        int iResId;
        if(m_pBoxElem->boxType==BOX_TYPE_VARFEEDBACK)
            iResId = CG_RES_STR_FEEDBACK_R;
        else
            iResId = CG_RES_STR_VARREAD;

        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp, 
             m_pdm->res1(iResId), m_pBoxElem->text);

        return false;//don't continue after this error
    }


    if(pBody->getGraphic()->isLD())
    {
        int      nInPins = pPin->pValue->getInPinsConnected()->getUsed();   
        BOX_TYPE bt = BOX_TYPE_UNDEF;
        if(nInPins>0)
            bt = pPin->pValue->getInPinsConnected()->getAddrOf(0)->pNWE->getBox()->boxType;

        if(nInPins>1 || (bt!=BOX_TYPE_FB && bt!=BOX_TYPE_FNC))
        {
            m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, m_pdm->res1(CG_RES_LD_VARW_BAD_USE));
            return false;//don't continue after this error
        }
    }


    m_pExpr = pBody->parseExpr(m_pBoxElem);
    if(m_pExpr)
    {
        pPin->pValue->setDataType(m_pExpr->GetType());
        pPin->pFormalParamType = m_pExpr->getTypePtr(); 
    }

    return m_pExpr!=NULL;
}
void VarReadBox::fixTypes(LDFBDBody* pBody)
{
    PinElement* pPin = m_pBoxElem->pins.get(0);
    
    //this pin defines a tmp var, check the data type:
    if(pPin->pValue && !pPin->pValue->getDataType().IsMonomorphType())
    {
        //don't warn if any literal:
        if(m_pExpr && !m_pExpr->isAny64Literal())
        {
            pBody->getDM()->msg2(CG_W_TYPE_RESULT, &m_edp,
                m_pBoxElem->text, ST_TypeSetToErrMsgString(pPin->pValue->getDataType()));
        }
            
        //fix type:
        pPin->pValue->setDataType(
                ST_SelectBestType(
                    pBody->getDM(),
                    &pBody->getFront()->m_features,
                    pPin->pValue->getDataType(), 
                    &m_edp)
                        );
    }
}
void VarReadBox::fixTypes2(LDFBDBody* pBody)
{
    assert(m_pExpr);
    m_pExpr->Propagate(m_pBoxElem->pins.get(0)->pValue->getDataType());
}
void VarReadBox::optimize1(LDFBDBody* pBody)
{
    PinElement*                     pPin;
    Value*                          pVal;
    const FC_CGArray<PINBOX_TYP>*   pInsConnected;

    pPin = m_pBoxElem->pins.get(0);
    pVal = pPin ? pPin->pValue : NULL;
    assert(!m_isOptimized && pVal);
    if(!pVal)
        return;

    //assert: immediate result of e.g. VarWrite cannot be set already 
    //since optimize1 is called in execution order:
    assert(!pVal->isImmediateResult());

    //a) optimize literals:
    if(m_pExpr->getLitValue())
    {
        pVal->setImmediateResult(this);
        m_isOptimized = true;
        return;
    }

    //b) optimization if varRead is connected to a single input without using Lines
    pInsConnected = pVal->getInPinsConnected();

    if(pInsConnected->getUsed()>1)//Note: pInsConnected count also pins
                                  //connected indirectly via inoutin----inoutout 
    {
        //assert: this is one of the en/eno restrictions: varRead must be 
        //directly connected to input (after error we should not pass here):

        // assert(!getENENOCondition());
        return;
    }
    //do always:
    pVal->setImmediateResult(this);
    m_isOptimized = true;
    return; //don't want the code depend so much on debug settings
            //also we get problems forcing tmp vars of complex type
            //wich are not supported !! such a LDFBS will only compile with bp0/1 !!!

    
    long lOpt = pBody->getCodeGenOpts();
    if(getENENOCondition())//HACK? always optimize, otherwise inputs must  
    {                            //eventually be evaluated outside of if(EN)
        pVal->setImmediateResult(this);
        m_isOptimized = true;
    }
    else if((lOpt & CG_LOADBDY_TMPVAR_OPT1)!=0 /*&& always to this even if lines
        POS_EQ(pInsConnected->getAddrOf(0)->pPin->pos, pPin->pos)*/
       )
    {
        pVal->setImmediateResult(this);
        m_isOptimized = true;
    }
}
CG_STStmt* VarReadBox::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    PinElement* pPin = m_pBoxElem->pins.get(0); 
    Value*      pVal = pPin->pValue;
    CG_STStmt*  pStmt;

    assert(pVal && m_pExpr && !m_isOptimized);

    if(pVal->isReferenceTmpVar())
        pVal->setReferencedExpr(m_pExpr->makeCGExpr(&m_ppc->getCBody()->m_exprMem, NULL));

    pVal->defineTmpVar(pBody);
    pStmt = m_ppc->newAssignStmt(NULL, pVal->newVarExpr(pBody), m_pExpr);
    m_pExpr = NULL;
    return pStmt;
}
//++++++++++++++++++++Member functions of class VarWriteBox++++++++++++++++++++++++++++
bool VarWriteBox::initTypeResolution(LDFBDBody* pBody)
{
    PinElement* pPin = m_pBoxElem->pins.get(0);

    assert(!m_pExpr);
   
    if(!pPin->pValue)
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
                    m_pdm->res1(CG_RES_STR_VARWRITE), m_pBoxElem->text);
        
        return false;//don't continue after this error
    }

    if(pBody->getGraphic()->isLD())
    {
        BOX_TYPE bt = pPin->pValue->getCreatorNWE()->getBox()->boxType;
        if(bt!=BOX_TYPE_FB && bt!=BOX_TYPE_FNC)
        {
            m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, m_pdm->res1(CG_RES_LD_VARW_BAD_USE));
            return false;//don't continue after this error
        }
    }

        
    m_pExpr = pBody->parseExpr(m_pBoxElem);
    if(m_pExpr)
    {
        pPin->pFormalParamType = m_pExpr->getTypePtr();

        if(!m_pExpr->is_Lvalue())//check lval here for a better errmsg...
            m_pdm->msg1(CG_E_VAROUT_NO_LVAL, &m_edp, m_pBoxElem->text);
    }
    
    return m_pExpr!=NULL;
}
void VarWriteBox::fixTypes2(LDFBDBody* pBody)
{
    PinElement* pPin = m_pBoxElem->pins.get(0);
    assert(pPin->pValue->getDataType().IsMonomorphType());
    m_pExpr->Propagate(pPin->pValue->getDataType());
}
void VarWriteBox::optimize1(LDFBDBody* pBody)
{
    long                            lOpt;
    PinElement*                     pPin;
    Value*                          pVal;
    const FC_CGArray<PINBOX_TYP>*   pInsConnected;

    pPin = m_pBoxElem->pins.get(0);
    pVal = pPin ? pPin->pValue : NULL;
    assert(!m_isOptimized && pVal);
    if(!pVal)
        return;

    //check if value is already set:
    //currently this might happen in many cases, e.g. VarRead box 
    //connected to this VarWrite Box without lines and signal split.
    //or VarRead was a literal:
    if(pVal->isImmediateResult())
        return;

    //optimization if varWrite is connected to a single output without using Lines
    pInsConnected = pVal->getInPinsConnected();
    if(pInsConnected->getUsed()>1)
    {
        assert(!getENENOCondition());
        return;
    }
    assert(pInsConnected->getAddrOf(0)->pPin == pPin);

    pVal->setImmediateResult(this);
    m_isOptimized = true;
    return; //don't want the code depend so much on debug settings
            //also we get problems forcing tmp vars of complex type
            //wich are not supported !! such a LDFBS will only compile with bp0/1 !!!

    lOpt = pBody->getCodeGenOpts();
    if(getENENOCondition())//HACK? always optimize, otherwise outputs must  
    {                            //eventually be evaluated outside of if(EN)
        pVal->setImmediateResult(this);
        m_isOptimized = true;
    }
    else if((lOpt & CG_LOADBDY_TMPVAR_OPT1)!=0 /*&&  always do it this way even if lines 
        POS_EQ(pVal->getCreatorPin()->pos, pPin->pos)*/
    )
    {
        pVal->setImmediateResult(this);
        m_isOptimized = true;
    }
}


CG_STStmt* VarWriteBox::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    Value*     pVal;
    CG_STStmt* pStmt;

    //don't call if was optimized away:
    assert(!m_isOptimized);
    pVal = m_pBoxElem->pins.get(0)->pValue;
    pStmt = m_ppc->newAssignStmt(NULL, m_pExpr, pVal->newVarExpr(pBody));
    m_pExpr = NULL;
    return pStmt;
}

//++++++++++++++++++++Member functions of class FeedBackVarOutBox++++++++++++++++++++++++++++
bool FeedBackVarOutBox::initTypeResolution(LDFBDBody* pBody)
{
    PinElement* pPin = m_pBoxElem->pins.get(0);

    assert(!m_pExpr && m_pFeedbackVarInBox);
   
    if(!pPin->pValue)
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
            m_pdm->res1(CG_RES_STR_FEEDBACK_L), m_pBoxElem->text);
        
        return false;//don't continue after this error
    }
        
    if(pBody->getGraphic()->isLD())
    {
        m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, m_pdm->res1(CG_RES_STR_REFFEEDBACK));
        return false;//don't continue after this error
    }
        
    //check specal cases of feedback variable use:
    //Check that m_pFeedbackVarInBox is in same NW:
    //Example of the Problem:
    //             +---+                                +---+
    //             |FB1|                                |FB2|
    //             |   |                                |   |--x
    //     x-------|   |--------- feedbackvar-----------|   |
    //             +---+                                +---+
    if(m_pFeedbackVarInBox->getNetwork()!=getNetwork())
    {
        m_pdm->msg1(CG_E_FEEDBACK_WRITTEN_FIRST, &m_edp, m_pBoxElem->text);
        return false;//don't continue after this error 
    }
    
    if(!m_pFeedbackVarInBox->hasPassedInitTypeResolution())
    {
        //check that the varin box has been assigned before reading from 
        //this var out:
        //Example of the Problem:
        //             +---+                                +---+
        //             |FB1|                                |FB2|
        //             |   |--------------------------------|   |--x
        //     x-------|   |--------- feedbackvar-----------|   |
        //             +---+                                +---+
        //
        m_pdm->msg1(CG_E_FEEDBACK_WRITTEN_FIRST, &m_edp, m_pBoxElem->text);
        return false;//don't continue after this error 
    }

    //reparse the expression from the corresponding VarReadBox.
    //NOTE: do not display diagnostics of expr twice, it is important that
    //the corresponding VarReadBox called pBody->parseExpr before (initTypeResolution), 
    //this has been checked above by checking hasPassedInitTypeResolution:
    //this->m_pBoxElem->text is already a string copy of the original in the VarReadBox.
    m_pExpr = pBody->reparseExpr(this);
    if(m_pExpr)
        pPin->pFormalParamType = m_pExpr->getTypePtr();
    
    return m_pExpr!=NULL;
}
//++++++++++++++++++++Member functions of class FBBox++++++++++++++++++++++++++++
static bool CheckInOutInUsage(
    LDFBDBody*         pBody,
    const BoxElement*  pBox,
    PinElement*        pPin
)
{
    FC_CGArray<PinElement*>  reachedPins;
    bool                     bError = false;

    assert(pPin->lPinType==PIN_TYPE_INOUT_IN);
        
    //check: var_in_out input pin must be connected to exactly
    //one pin (value), if this value has a l-value will be checked later in code gen:
    pBody->getSignalAndWarn(GET_PIN_COORDS(pPin), NULL, &reachedPins, NULL);

    //reachedPins: "pPin" + "one connected" =="2":
    if(reachedPins.getUsed()!=2 || !pPin->pValue)
    {
        //unconnected/overconnected var_inout inpin:
        CG_Edipos edp;
        pBody->getDM()->msg1(CG_E_INOUT_CONNECTION, pBody->signalEdp(&edp, pPin->pos),
            CreatePinErrorMsgName(pBody->getDM()->getResHMODULE(), pBox, pPin));
        bError = true;
    }
    else if(pPin->pValue->getCreatorNWE()->getBox()->boxType==BOX_TYPE_VARREAD)
    {
        //if pPin->pValue is created from a varRead box then tell the value 
        //that it's a 'reference' type (will be essential if a tmp var is
        //needed:
        pPin->pValue->setReferenceTmpVar();

        //check that varRead box contains a l-value:
        if(!pPin->pValue->getCreatorNWE()->isLvalue())
        {
            pBody->getDM()->msg1(CG_E_VAROUT_NO_LVAL, 
                pPin->pValue->getCreatorNWE()->getEdiPos(),
                pPin->pValue->getCreatorNWE()->getBox()->text);
            bError = true;
        }
    }
    else if((pPin->pValue->getCreatorPin()->lPinType & PIN_TYPE_INOUT_OUT)==0)
    {
        //inout in pin connected to neither to VarRead nor inout-out pin => error:
        pBody->getDM()->msg1(CG_E_VAROUT_NO_LVAL, 
            pPin->pValue->getCreatorNWE()->getEdiPos(), 
            pPin->pValue->getCreatorNWE()->getBox()->text);
        bError = true;
    }

    if(!bError)
    {
        int                    nPins = pBox->pins.getUsed();
        const PinElement*const*pPins = pBox->pins.getCElements();

        for(int i = 0; i<nPins; i++)
        {
            const PinElement* pPinIOO = pPins[i];
            if(pPinIOO->lPinType==PIN_TYPE_INOUT_OUT && !_tcsicmp(pPinIOO->szName,pPin->szName))
            {
                //in-out-out pin is connected, set the alias value:
                if(pPinIOO->pValue)
                    pPinIOO->pValue->setAliasVal(pPin->pValue);
                break;
            }
        }
        //should never happen since the editor inserts the pairs:
        if(i>=nPins)
        {
            CG_Edipos edp;
            pBody->getDM()->msg1(CG_E_VARINOUT_NOT_PAIR, pBody->boxEdp(&edp, pBox->ext), pPin->szName);
            bError = true;
        }
    }

    return !bError;
}





bool FBBox::initTypeResolution(LDFBDBody* pBody)
{
    int                         i;
    int                         nPins;
    bool                        bError;
    PinElement*                 pPin;
    const CG_SymEntry*          pEntry;
    ST_CSymtabPrv*              pSym = pBody->getSym();
    const ST_CSymtabPrv*        pFBDefinition;

    if(!ST_CheckIdentifier(m_pdm, &m_edp, m_pBoxElem->instName))
        return false;
   
    m_pFBIEntry = pSym->lookUpDeepAndInsertGVImp(m_pBoxElem->instName);
    if(m_pFBIEntry)
    {
        //check if instName is FBI:
        //ckeck that typename displayed in fb-box matches typename in header:
        if(m_pFBIEntry->pType->type != CG_INSTANCE)
        {
            m_pdm->msg0(CG_E_ILLEGAL_CALL, &m_edp);
            m_pFBIEntry = NULL;
        }
        else if(!ST_IS_NAME_EQUAL(m_pBoxElem->text, m_pFBIEntry->pType->pszName))
        {
            m_pdm->msg1(CG_E_FBTYPENAME_MISMATCH, &m_edp, m_pFBIEntry->pType->pszName);
            m_pFBIEntry = NULL;
        }
    }
    else
    {
        m_pdm->msg1(CG_E_UNDEF_VAR, &m_edp, m_pBoxElem->instName);
    }
    if(!m_pFBIEntry)
        return false;

    //check existence of all pin member names
    bError = false;
    nPins = m_pBoxElem->pins.getUsed();
    pFBDefinition = pBody->getFront()->loadHeader(m_pFBIEntry->pType->pSym->pszId, &bError);
    assert(pFBDefinition && !bError);
    if(!pFBDefinition || bError) 
        return false;

    if(!SortPinArray(m_pdm, &m_edp, m_pBoxElem->pins, NULL, NULL, &m_pEN, &m_pENO))
        return false;

    for(i=0;i<nPins;i++)
    {
        pPin = m_pBoxElem->pins.get(i);

        if(pPin->lPinType==PIN_TYPE_EN)
        {
            pPin->HACKformalParamType = CG_TBS_BOOL;
        }
        else if(pPin->lPinType==PIN_TYPE_ENO)
        {
            pPin->HACKformalParamType = CG_TBS_BOOL;
        }
        else
        {
            pEntry = pFBDefinition->lookUpEntry(pPin->szName);
            if(!pEntry)
            {
                bError = true;
                m_pdm->msg2(CG_E_UNDEF_MEMBER, &m_edp, pPin->szName, m_pFBIEntry->pType->pszName);
                continue;
            }
        
            if(!CheckPinUseAgainstHeader(m_pdm, &m_edp, m_pBoxElem, pPin, pFBDefinition, pEntry))
                bError = true;
            if(pPin->lPinType==PIN_TYPE_INOUT_IN && !CheckInOutInUsage(pBody, m_pBoxElem, pPin))
                bError = true;

            pPin->HACKformalParamType.initFromCG_DType(pEntry->pType);
        }

        //assert: type resolution algorithm of FBBox is keept simple
        //no polymorph stuff allowed, except if inout:
        assert(pPin->pFormalParamType->IsMonomorphType() || 
            (pPin->lPinType & (PIN_TYPE_INOUT_IN|PIN_TYPE_INOUT_OUT)));

        //initialize the value's type of output if connected:
        if((pPin->lPinType & ANY_OUT_PIN) && pPin->pValue)
            pPin->pValue->setDataType(pPin->HACKformalParamType);
    }          


    return !bError;
}


CG_STStmt* FBBox::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    int              n;
    Value*           pVal;
    PinElement*      pPin;
    int              nPins = m_pBoxElem->pins.getUsed();
    ST_Expression_C* pTmp;
    bool             bNegate;
    ST_CFBCallExpr*  pCurrFBICall;//##AC
    ST_CParamList*   pParList;    //##AC

    //if -BP2 option active enable breakpoint here:
    //TODO if(GetGC()->isOptionSet(OPTION_BP2))
    //TODO     ppc->dumpBreakPointFunction(&m_edp, -1);

    //open FB call:
    pParList     = new ST_CParamList(&m_edp, m_ppc, nPins);//##AC

    //loop all in/outputs:
    for(n=0;n<nPins;n++)
    {
        pPin = m_pBoxElem->pins.get(n);
        pVal = pPin->pValue;
        if(!pVal || pPin->lPinType & (PIN_TYPE_EN|PIN_TYPE_ENO) )
            continue;

        bNegate = (pPin->lPinFlags & PIN_FLAG_NEGATE)!=0;

        if(pPin->lPinType==PIN_TYPE_IN)
        {
            pTmp = pVal->newVarExpr(pBody);
            if(bNegate)
                pTmp = m_ppc->createOp(&m_edp, CG_NOT, pTmp, NULL);

            //pPin->szName is the name of the fb member from the pin array:
            pParList->addNamedParam(&m_edp, pPin->szName, false, false, pTmp);//##AC
        }
        else if(pPin->lPinType==PIN_TYPE_INOUT_IN)
        {
            //assert: illegal use of negate must have been checked already and 
            //::generateCode should not be called:
            assert(!bNegate);
            
            //check multible write only if pVal is a var read box
            //otherwise we get warnings in the following case too:
            //              +---+         +---+
            //              |FB1|         |FB2|
            //              |   |         |   |
            //      x[i+i]--|i-i|---------|i-i|--
            //              +---+         +---+
            pTmp = pVal->newVarExpr(pBody);
            pParList->addNamedParam(&m_edp, pPin->szName, false, false, pTmp);//##AC
        }
        else if(pPin->lPinType==PIN_TYPE_OUT)
        {
            pVal->defineTmpVar(pBody);
            pTmp = pVal->newVarExpr(pBody);

            //HACK bNegate must be handeled by back end as flag, better insert 
            //assign stmts for FB in/out assigns?
            pParList->addNamedParam(&m_edp, pPin->szName, true, bNegate, pTmp);//##AC
        }
        else if(pPin->lPinType==PIN_TYPE_INOUT_OUT)
        {
            //done in CheckInOutInUsage by use of pValue->setAliasVal()
            assert(!pPin->pValue || pPin->pValue->hasAliasVal());
        }
        else
        {
            assert(!"bad FB box pin type");
        }
    }
    
    //end of fb call:
    pCurrFBICall = new ST_CFBCallExpr(&m_edp, m_ppc, m_pFBIEntry, pParList);     //##AC  
    if(!pCurrFBICall->checkFBCall())                                             //##AC 
    {                                                                            //##AC 
        delete(pCurrFBICall);                                                    //##AC
        return NULL;                                                             //##AC 
    }
    return m_ppc->newCallStmt(pCurrFBICall);
}
//++++++++++++++++++++Member functions of class FunBox++++++++++++++++++++++++++++
bool FunBox::initTypeResolution(LDFBDBody* pBody)
{
    const TCHAR*     pszFun = m_pBoxElem->text;
    const CG_Symtab* pSym;
    long             lres;

    lres = pBody->getFront()->loadHeaderEx(&m_edp, pszFun, ST_WANT_FUN, &m_pFunType, NULL);

    //if trg typ not supported error, carry on. All other errors ignore the fun call:
    if(lres!=ST_LHDR_OK && lres!=ST_LDHD_TRG_NS)
    {
        m_pFunType = NULL;
        if(lres & ST_LDHD_ERRIND) //indirect error update internal err counter
            m_ppc->indirectError();
        return false;
    }
    pSym = m_pFunType->getCGSym();
    assert(pSym->pouTyp==CG_POU_FUN && (pSym->flags & CG_STAB_HDR_NOERR)!=0);

    if(!SortPinArray(m_pdm, &m_edp, m_pBoxElem->pins, &m_nInputs, &m_nOutputs, &m_pEN, &m_pENO))
        return false;
    return initTypeInfo(pBody);
}


bool FunBox::initTypeInfo(LDFBDBody* pBody)
{
    int                    i;
    PinElement*            pPin;
    bool                   bError;
    int                    nPins;
    const CG_SymEntry*     pEntry;
    const CG_Symtab*       pSym;
    bool                   bConErr;    
    int                    nAct;
    int                    nFrm;
    int                    nExtendedInputs;

    bError     = false;
    bConErr    = false;
    pSym       = m_pFunType->getCGSym();
    nPins      = m_pBoxElem->pins.getUsed();


    //check number restriction of input pins:    
    nAct = m_pEN ? m_nInputs-1 : m_nInputs;
    nFrm = pSym->nInput + pSym->nInOut + (pSym->pVarInExt ? 1 : 0);
    bError = pSym->pVarInExt ?  nFrm > nAct : nFrm != nAct;
    if(bError)
    {
        m_pdm->msg2(CG_E_4GR_HEADER_MISMATCH, &m_edp, m_pBoxElem->text, 
            m_pdm->jot1()<<m_pdm->res2(CG_RES_STR_NOOVERLOAD)<<nAct<<m_pdm->res3(CG_RES_STR_PIN_IN));
        return false;
    }

    //check number restriction of output pins:    
    nAct = m_pEN ? m_nOutputs-1 : m_nOutputs;
    nFrm = pSym->nOutput + pSym->nInOut - (pSym->pFirst->pType->type==CG_VOID ? 1 : 0);
    bError = nFrm != nAct;
    if(bError)
    {
        m_pdm->msg2(CG_E_4GR_HEADER_MISMATCH, &m_edp, m_pBoxElem->text, 
            m_pdm->jot1()<<m_pdm->res2(CG_RES_STR_NOOVERLOAD)<<nAct<<m_pdm->res3(CG_RES_STR_PIN_OUT));
        return false;
    }



    //EN/ENO special:
    if(m_pEN)
    {
        assert(m_pEN->lPinType==PIN_TYPE_EN && m_pEN==m_pBoxElem->pins.get(0));
        assert(m_pENO && m_pENO->lPinType==PIN_TYPE_ENO && m_pENO==m_pBoxElem->pins.get(m_nInputs));
        
        m_pEN->HACKformalParamType = CG_TBS_BOOL;
        m_pENO->HACKformalParamType = CG_TBS_BOOL;
        
        if (m_pENO->pValue)
            m_pENO->pValue->setDataType(CG_TBS_BOOL);
    }


    assert(m_nArgListOrderedPins==0);
    if(pSym->pFirst->pType->type==CG_VOID)
    {
        m_pArgListOrderedPins = new PinElement*[nPins+1];
        m_pArgListOrderedPins[0] = NULL;
        m_nArgListOrderedPins = 1;
    }
    else
    {
        m_pArgListOrderedPins = new PinElement*[nPins];
    }
    //memset?, no, if there are any internal errors better chrash...
    //if no errors every elem will be set to a valid pin-pointer
    //memset(m_pArgListOrderedPins, 0 , sizeof(const CG_SymEntry*)*pSym->nParam);

    pEntry = NULL;
    nExtendedInputs = 0;
    int nExtInGroup = 0; // ++**++ JD extInParam
    int nExtInGroupSize = pSym->pVarInExt ? pSym->nParam - pSym->pVarInExt->lParamNum : 0; // ++**++ JD extInParam
    for(i=m_pEN?1:0; i<m_nInputs; i++)
    {
        pEntry = pEntry ? GetNextLDFBDInEntry(pEntry) : GetFirstLDFBDInEntry(pSym);
        assert(pEntry->lParamNum>=0 && pEntry->lParamNum<pSym->nParam);
        pPin = m_pBoxElem->pins.get(i);
       
        if(!pEntry || (pPin->lPinType & ANY_IN_PIN)==0)
            CGT_IERR2(m_pdm, &m_edp, _T("FunBox::initTypeInfo: symtab/pin array out of sync"));

        //check that the pin name matches the function type definition:
        if(!CheckPinUseAgainstHeader(m_pdm, &m_edp, m_pBoxElem, pPin, m_pFunType, pEntry))
            bError = true;
    
        if(!bError)
        {
            int nOff = pEntry->lParamNum  + nExtInGroupSize*nExtInGroup; // + nExtendedInputs;// ++**++ JD extInParam
            m_pArgListOrderedPins[nOff] = pPin;
            
            if(m_nArgListOrderedPins <= nOff)
                m_nArgListOrderedPins = nOff +1;
             
            // ++**++ JD extInParam -->
            if(pEntry->declScope==CG_VAR_IN_EXT)
            {
                nExtendedInputs++;
                if (!pEntry->pNext)
                    nExtInGroup++;
            }
            // ++**++ JD extInParam <--
            
            //assert: only in ext can follow after the first in ext!
            assert(!(nExtendedInputs>0 && pEntry->declScope!=CG_VAR_IN_EXT));
        }
            
        if(pEntry->declScope==CG_VAR_IN_OUT)
        {
            if(pPin->lPinType==PIN_TYPE_INOUT_IN)
            {
                if(!CheckInOutInUsage(pBody, m_pBoxElem, pPin))
                    bError = true;
            }
            else
            {
                assert(bError);//from CheckPinUseAgainstHeader
            }
        }

        pPin->HACKformalParamType.initFromCG_DType(pEntry->pType);

        //check that every input of the function is connected to something
        if(!pPin->pValue)
        {
            bConErr = true;
            bError = true;
        }
    }

    // ++**++ JD extInParam -->
    if (nExtendedInputs>0 && nExtInGroupSize>0 && nExtendedInputs % nExtInGroupSize != 0)
        m_pdm->msg0(CG_E_MULTIPLE_VAR_IN_EXT, &m_edp);
    // ++**++ JD extInParam <--


    pEntry = NULL;
    for(i=m_pEN?1:0; i<m_nOutputs; i++)
    {
        pEntry = pEntry ? GetNextLDFBDOutEntry(pEntry) : GetFirstLDFBDOutEntry(pSym);
            assert(pEntry->lParamNum>=0 && pEntry->lParamNum<pSym->nParam);
        pPin = m_pBoxElem->pins.get(m_nInputs+i);

        if(!pEntry || (pPin->lPinType & ANY_OUT_PIN)==0)
            CGT_IERR2(m_pdm, &m_edp, _T("FunBox::initTypeInfo: symtab/pin array out of sync"));

        //check that the pin name matches the function type definition:
        if(!CheckPinUseAgainstHeader(m_pdm, &m_edp, m_pBoxElem, pPin, m_pFunType, pEntry))
            bError = true;
        
        if(!bError)
        {
            if(pEntry->declScope!=CG_VAR_IN_OUT)
            {
                m_pArgListOrderedPins[pEntry->lParamNum] = pPin; 
                if(m_nArgListOrderedPins <= pEntry->lParamNum)
                    m_nArgListOrderedPins = pEntry->lParamNum +1;
            }
            else
            {    //the var_in_out output side does not apear in the ST arg list
                 //but if no error so far the corresponding input side pin must be set:
                assert(m_pArgListOrderedPins[pEntry->lParamNum]->lPinType==PIN_TYPE_INOUT_IN);
            }
        }

        pPin->HACKformalParamType.initFromCG_DType(pEntry->pType); 
        if (pPin->pValue)
        {
            pPin->pValue->setDataType(pPin->HACKformalParamType);
        }
        else if((pPin->lPinType & PIN_TYPE_INOUT_OUT)==0)
        {
            //check that every output of the function is connected to something, 
            //unless inout out:
            bConErr = true;
            bError = true;
        }
    }

    if(bConErr)
        m_pdm->msg0(CG_E_FUNCT_IO_UNCONNECTED, &m_edp);

    if(bError)
    {
        delete m_pArgListOrderedPins;
        m_pArgListOrderedPins = NULL;
    }
    else
    {
        //finally get the polymorphic type description in form of an array
        //containing all different polymorph function in/outputs, if the
        //array is NULL, there is no polymorphism.
        m_pDifferentPolyTypes = m_pFunType->getPolyTypeArray();
    }
    //assert(m_nArgListOrderedPins<=nPins);

    return !bError;
}
CG_STStmt* FunBox::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    int                   i;
    PinElement*           pPin;
    ST_Expression_C*      pExpr;
    ST_CParamList*        pArgList = NULL; //##AC
    int                   nMaxNegatedOutput = 0;
    const CG_Symtab*      pSym = m_pFunType->getCGSym();
    CG_STStmt*            pStmt;
    CG_STStmt*            pFirst = NULL;
    CG_STStmt*            pLast = NULL;

    assert(m_nArgListOrderedPins>0);



    //create fun arglist, i=1 skip fun return:
    for(i=1; i<m_nArgListOrderedPins; i++)
    {
        pPin = m_pArgListOrderedPins[i];
        if(pPin->lPinType == PIN_TYPE_OUT)
        {
            if(!pPin->pValue->isImmediateResult())
            {
                pPin->pValue->defineTmpVar(pBody);
                
                if(pPin->pValue->getDataType().GetTypeSet() & (CG_TBS_ARRAY|CG_TBS_STRUCT|CG_TBS_INSTANCE))
                {
                     m_pdm->errNS1(pPin->pValue->getCreatorNWE()->getEdiPos(),
                         m_pdm->res1(CG_RES_STR_NONSIMPLEDTYPE));
                }
            }
           
            //remember the highest index of negated output:
            if(pPin->lPinFlags & PIN_FLAG_NEGATE)
                nMaxNegatedOutput = i;

            pExpr = pPin->pValue->newVarExpr(pBody);
        }
        else if(pPin->lPinType==PIN_TYPE_INOUT_IN)
        {
            //assert: illegal use of negate must be checked by generateCode:
            assert((pPin->lPinFlags & PIN_FLAG_NEGATE)==0);
            pExpr = pPin->pValue->newVarExpr(pBody);
        }
        else if(pPin->lPinType & (PIN_TYPE_IN|PIN_TYPE_INEXT)) // normal in/in_ext
        {  
            pExpr = pPin->pValue->newVarExpr(pBody);
            if(pPin->lPinFlags & PIN_FLAG_NEGATE)
                pExpr = m_ppc->createOp(&m_edp, CG_NOT, pExpr, NULL);
        }
        else
        {
            assert(!"bad FunBox in arglist pin-array");
        }

        if(!pArgList)                                                             //##AC
            pArgList = new ST_CParamList(&m_edp, m_ppc, m_nArgListOrderedPins-1); //##AC
                                                                                  //##AC
        pArgList->addPosParam(pExpr);                                             //##AC
    }

    //create fun call expression:
    pExpr = m_ppc->newCallExpr(&m_edp, m_pBoxElem->text, CG_POU_FUN, pArgList);
    
    //dump assignment if not void fun:
    pPin = m_pArgListOrderedPins[0];
    if(pPin)
    {
        assert(pPin->pValue && pPin->lPinType==PIN_TYPE_OUT && pPin->szName[0]==0);

        pPin->pValue->defineTmpVar(pBody);
        if(pPin->lPinFlags & PIN_FLAG_NEGATE)
            pExpr = m_ppc->createOp(&m_edp, CG_NOT, pExpr, NULL);

        pStmt = m_ppc->newAssignStmt(&m_edp, pPin->pValue->newVarExpr(pBody), pExpr);
    }
    else
    {
        pStmt = m_ppc->newCallStmt(pExpr);
    }

    if(pLast)
        pLast->pNext = pStmt;
    pLast = pStmt;
    if(!pFirst)
        pFirst = pStmt;


    // dump the negated outputs:
    for(i=1; i<=nMaxNegatedOutput; i++)
    {
        pPin = m_pArgListOrderedPins[i];
        if(pPin->lPinType==PIN_TYPE_OUT && (pPin->lPinFlags & PIN_FLAG_NEGATE))
        {            
            pExpr = m_ppc->createOp(&m_edp, CG_NOT, pPin->pValue->newVarExpr(pBody), NULL);
            pStmt = m_ppc->newAssignStmt(NULL, pPin->pValue->newVarExpr(pBody), pExpr);

            if(pLast)
                pLast->pNext = pStmt;
            pLast = pStmt;
            if(!pFirst)
                pFirst = pStmt;
        }
    }

    assert(pFirst);
    return pFirst;
}
//++++++++++++++++++++Member functions of class WiredOr++++++++++++++++++++++++++++
bool WiredOr::initTypeResolution(LDFBDBody* pBody)
{
    int         i;
    bool        bOk;
    PinElement* pPin;
    int         nPins;

    bOk = SortPinArray(m_pdm, &m_edp, m_pBoxElem->pins, &m_nInputs, &m_nOutputs, NULL, NULL);
    assert(bOk);//can only fail if EN/ENO misused.
    assert(m_nInputs>0 && m_nOutputs>=1);//<-case of no ins or no outs is handeled in 
                                         //create NW elements. One in One out is a line.

    nPins = m_pBoxElem->pins.getUsed();
    for (i=0; i<nPins; i++)
    {
        pPin = m_pBoxElem->pins.get(i);
        pPin->HACKformalParamType = CG_TBS_BOOL;
        if(pPin->pValue)
        {
            if(pPin->lPinType==PIN_TYPE_WIREDOR_OUT)
            {
                pPin->pValue->setDataType(CG_TBS_BOOL);
            }
            else
            {
                assert(pPin->lPinType==PIN_TYPE_WIREDOR_IN);

                BOX_TYPE boxType = pPin->pValue->getCreatorNWE()->getBox()->boxType;
                switch(boxType)
                {
                case BOX_TYPE_POWERRAIL:
                    if(m_nInputs>1)
                    {
                        m_pdm->msg1(CG_W_LD_LAYOUT, &m_edp, 
                            m_pdm->res1(CG_RES_STR_OR_NOEFFECT));
                    }//else: if only one input it is not really a wired or and gets opti anyway...
                break;
                case BOX_TYPE_COIL:
                case BOX_TYPE_CONTACT:
                case BOX_TYPE_WIREDOR:
                break;
                default:
                    FC_CString  jot(MAX_PATH);
        
                    BoxTypeToDiagString(boxType, jot, m_pdm->getResHMODULE());
                    m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, 
                        m_pdm->res1(CG_RES_STR_OR_BADRSIG)<<_T(" '")<<jot<<_T("'"));
                    return false;//don't continue after this error
                }
            }
        }
        else
        {
            m_pdm->msg0(CG_E_LD_WIREDOR_UNCONNECTED, &m_edp);
            return false;
        }
    }

    //optimize: if only one input and the in-expr. can be cloned:
    if(m_nInputs==1)
    {
        PinElement* pFirstInPin = m_pBoxElem->pins.get(0);
        for(i=m_nInputs; i<nPins; i++)
        {
            pPin = m_pBoxElem->pins.get(i);
            assert(pPin->lPinType & ANY_OUT_PIN);
            pPin->pValue->setAliasVal(pFirstInPin->pValue);
        }
        m_isOptimized = true;
    }

    return true;
}
CG_STStmt* WiredOr::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    int              i;
    PinElement*      pPin;
    int              nPins;
    ST_Expression_C* pResult;
    ST_Expression_C* pTmp;
    int              nOutPinTmp;
    int              nTmpNum;
    CG_STStmt*       pFirst;
    CG_STStmt*       pLast;
    Value*           pVal;

    //assert: nInputs==1 was optimized in initTypeResolution by 
    //setting all outputs the alias:
    assert(!m_isOptimized && m_nInputs>1);

    //note: that at least one input (and nOut>0) exists 
    //      is checked in createNetworkElements.

    //general case tmp = in1 or in2 or in3 ...
    //a) build the result OR expression:
    pResult = NULL;
    nPins = m_pBoxElem->pins.getUsed();
    for(i=0; i<m_nInputs; i++)
    {
        pPin = m_pBoxElem->pins.get(i);
        assert(pPin->lPinType==PIN_TYPE_WIREDOR_IN);
        
        if(pResult)
            pResult = m_ppc->createOp(&m_edp, CG_OR, pResult, pPin->pValue->newVarExpr(pBody));
        else 
            pResult = pPin->pValue->newVarExpr(pBody);
    }
    assert(pResult);


    //loop all outputs and try to get one that is a tmp anyway:
    nTmpNum    = -1;
    nOutPinTmp = -1;
    for(i=m_nInputs; i<nPins; i++)
    {
        pVal = m_pBoxElem->pins.get(i)->pValue;
        int n = pVal->defineTmpVar(pBody);
        if(n>0 && nTmpNum<0)
        {
            nTmpNum = n;
            nOutPinTmp = i;
        }
    }

    //if all outputs are immediate (then they have no tmp), create tmp here:
    if(nTmpNum<0)
        nTmpNum = pBody->getFreeTmp(CG_TBS_BOOL, false);

    //assign result to the tmp, this defines the first stmt:
    pTmp = new ST_TmpVar_C(&m_edp, pBody->getPPC(), nTmpNum, CG_TBS_BOOL, 0, 0);
    pLast = pFirst = m_ppc->newAssignStmt(NULL, pTmp, pResult);

    //loop all outputs:
    for(i=m_nInputs; i<nPins; i++)
    {
        if(i==nOutPinTmp)
            continue; //was already assigned in pFirst stmt
        
        pTmp = new ST_TmpVar_C(&m_edp, pBody->getPPC(), nTmpNum, CG_TBS_BOOL, 0, 0);
        pVal = m_pBoxElem->pins.get(i)->pValue;
        CG_STStmt* pStmt = m_ppc->newAssignStmt(NULL, pVal->newVarExpr(pBody), pTmp);
        pLast->pNext = pStmt;
        pLast = pStmt;
    }

    return pFirst;
}
//++++++++++++++++++++Member functions of class Contact++++++++++++++++++++++++++++
bool Contact::initTypeResolution(LDFBDBody* pBody)
{
    BOX_TYPE          boxType;
    PinElement* pPin0 = m_pBoxElem->pins.get(0);
    PinElement* pPin1 = m_pBoxElem->pins.get(1);

    assert(pPin0->lPinType == PIN_TYPE_IN);
    assert(pPin1->lPinType == PIN_TYPE_OUT);
    assert(!m_pExpr);

    //essential error!
    if(m_pBoxElem->boxFlags & (BOX_FLAG_F_EDGE|BOX_FLAG_R_EDGE))
    {
        if(m_ppc->getCGSym()->pouTyp==CG_POU_FUN)
            m_pdm->msg0(CG_E_LD_TRIGGER_ELEM_IN_FUN, &m_edp);
    }

    if(!pPin0->pValue)
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp, m_pdm->res1(CG_RES_STR_CONTACT), m_pBoxElem->text);
        return false;//don't continue after this error
    }
    if(!pPin1->pValue)
    {
        m_pdm->msg1(CG_E_LD_CONTACT_UNUSED, &m_edp, m_pBoxElem->text);
        return false;//don't continue after this error
    }

    boxType = pPin0->pValue->getCreatorNWE()->getBox()->boxType;
    switch(boxType)
    {
    case BOX_TYPE_POWERRAIL:
    case BOX_TYPE_COIL:
    case BOX_TYPE_CONTACT:
    case BOX_TYPE_WIREDOR:
    break;
    default:
        FC_CString jot(MAX_PATH);
        
        BoxTypeToDiagString(boxType, jot, m_pdm->getResHMODULE());
        m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, 
            m_pdm->res1(CG_RES_STR_CONTACT)<<_T(" '")<<m_pBoxElem->text<<_T("': ")<<
            m_pdm->res2(CG_RES_STR_BADLEFTSIGNAL)<<_T(" '")<<jot<<_T("'"));
        return false;//don't continue after this error
    }



    pPin0->HACKformalParamType = CG_TBS_BOOL;
    pPin1->HACKformalParamType = CG_TBS_BOOL;
    assert(pPin1->pValue);
    pPin1->pValue->setDataType(CG_TBS_BOOL);

    m_pExpr = pBody->parseExpr(m_pBoxElem);
    //Note: the checkTypedExpr does also fix the type with using Propagate
    //since we know the exact non poly type know there is no need 
    //to get more type info from the network, we can therfore fix the type
    //expr's type here and now. 
    //For the same reason we don't need to implement the NetworkElement::fixTypes2 method.
    return m_pExpr && m_pExpr->checkTypedExpr(CG_TBS_BOOL, true);
}

CG_STStmt* Contact::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    PinElement*      pPin0 = m_pBoxElem->pins.get(0);
    PinElement*      pPin1 = m_pBoxElem->pins.get(1);
    Value*           pInVal = pPin0->pValue;
    Value*           pOutVal= pPin1->pValue;
    ST_Expression_C* pExpr;
    ST_Expression_C* pTmp;
    int              iTrigTmp;
    CG_STStmt*       pStmt;

    assert(pInVal && pOutVal && m_pExpr);
    
    switch(m_pBoxElem->boxFlags)
    {
    case BOX_FLAG_NORMAL:
        //out := in & m_pExpr
        if(pInVal->isPowerRailTrue())
            pExpr = m_pExpr;
        else
            pExpr = m_ppc->createOp(&m_edp, CG_AND, pInVal->newVarExpr(pBody), m_pExpr);

        pOutVal->defineTmpVar(pBody);
        pStmt = m_ppc->newAssignStmt(NULL, pOutVal->newVarExpr(pBody), pExpr);
        m_pExpr = NULL;
    break;
    case BOX_FLAG_NEGATE:
        //out := in & !m_pExpr
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, m_pExpr, NULL);
        
        if(!pInVal->isPowerRailTrue())
            pExpr = m_ppc->createOp(&m_edp, CG_AND, pInVal->newVarExpr(pBody), pExpr);
        
        pOutVal->defineTmpVar(pBody);
        pStmt = m_ppc->newAssignStmt(NULL, pOutVal->newVarExpr(pBody), pExpr);
        m_pExpr = NULL;
    break;
    case BOX_FLAG_R_EDGE:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //stmt1:           out     := in & (m_pExpr & not tmp_presistent)
        //stmt2:    tmp_presistent := m_pExpr
        iTrigTmp = m_ppc->getFreeTriggerTmpNum();

        //stmt1: out     := in & (m_pExpr & not tmp_presistent):
        pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, pTmp, NULL);
        pExpr = m_ppc->createOp(&m_edp, CG_AND, m_pExpr, pExpr);

        if(!pInVal->isPowerRailTrue())
            pExpr = m_ppc->createOp(&m_edp, CG_AND, pInVal->newVarExpr(pBody), pExpr);
        
        pOutVal->defineTmpVar(pBody);
        pStmt = m_ppc->newAssignStmt(NULL, pOutVal->newVarExpr(pBody), pExpr);
        m_pExpr = NULL;
        
        if(pStmt)//can be NULL after error!
        {
            //stmt2:    tmp_presistent := m_pExpr
            pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
            pStmt->pNext = m_ppc->newAssignStmt(NULL, pTmp, pBody->reparseExpr(this));
        }
    break;
    case BOX_FLAG_F_EDGE:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //stmt1:           out     := in & (!m_pExpr & tmp_presistent)
        //stmt2:    tmp_presistent := m_pExpr
        iTrigTmp = m_ppc->getFreeTriggerTmpNum();

        //stmt1: out     := in & (!m_pExpr & tmp_presistent):
        pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_F_TRIG);
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, m_pExpr, NULL);
        pExpr = m_ppc->createOp(&m_edp, CG_AND, pExpr, pTmp);
    
        if(!pInVal->isPowerRailTrue())
            pExpr = m_ppc->createOp(&m_edp, CG_AND, pInVal->newVarExpr(pBody), pExpr);

        pOutVal->defineTmpVar(pBody);
        pStmt = m_ppc->newAssignStmt(NULL, pOutVal->newVarExpr(pBody), pExpr);
        m_pExpr = NULL;
        
        //stmt2:    tmp_presistent := m_pExpr
        if(pStmt)//can be NULL after error!
        {
            pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_F_TRIG);
            pStmt->pNext = m_ppc->newAssignStmt(NULL, pTmp, pBody->reparseExpr(this));
        }
    break;
    default:
        assert(!"bad boxFlags for Contact");
    }

    return pStmt;
}
//++++++++++++++++++++Member functions of class Coil++++++++++++++++++++++++++++
bool Coil::initTypeResolution(LDFBDBody* pBody)
{
    BOX_TYPE            boxType;
    PinElement* pPin0 = m_pBoxElem->pins.get(0);
    PinElement* pPin1 = m_pBoxElem->pins.get(1);

    assert(pPin0->lPinType == PIN_TYPE_IN);
    assert(pPin1->lPinType == PIN_TYPE_OUT);
    assert(!m_pExpr);

    if(!pPin0->pValue)
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
             m_pdm->res1(CG_RES_STR_VARWRITE), m_pBoxElem->text);

        return false;//don't continue after this error
    }

    //essential error!
    if(m_pBoxElem->boxFlags & (BOX_FLAG_F_EDGE|BOX_FLAG_R_EDGE|BOX_FLAG_SET|BOX_FLAG_RESET))
    {
        if(m_ppc->getCGSym()->pouTyp==CG_POU_FUN)
            m_pdm->msg0(CG_E_LD_TRIGGER_ELEM_IN_FUN, &m_edp);
    }

    boxType = pPin0->pValue->getCreatorNWE()->getBox()->boxType;
    switch(boxType)
    {
    case BOX_TYPE_POWERRAIL:
    case BOX_TYPE_COIL:
    case BOX_TYPE_CONTACT:
    case BOX_TYPE_WIREDOR:
    break;
    default:
        FC_CString jot(MAX_PATH);
        
        BoxTypeToDiagString(boxType, jot, m_pdm->getResHMODULE());
        m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, 
            m_pdm->res1(CG_RES_STR_COIL)<<_T(" '")<<m_pBoxElem->text<<_T("': ")<<
            m_pdm->res2(CG_RES_STR_BADLEFTSIGNAL)<<_T(" '")<<jot<<_T("'"));
        return false;//don't continue after this error
    }
    if(pPin1->pValue)
    {
        m_pdm->msg1(CG_E_LD_LAYOUT, &m_edp, 
            m_pdm->res1(CG_RES_STR_COIL)<<_T(" '")<<m_pBoxElem->text<<_T("': ")<<
            m_pdm->res2(CG_RES_STR_OPENRIGHTSIDE));
        return false;//don't continue after this error
    }


    pPin0->HACKformalParamType = CG_TBS_BOOL;
    pPin1->HACKformalParamType = CG_TBS_BOOL;
    if(pPin1->pValue)
        pPin1->pValue->setDataType(CG_TBS_BOOL);

    m_pExpr = pBody->parseExpr(m_pBoxElem);
    if(m_pExpr)
    {
        if(!m_pExpr->is_Lvalue())//check lval here for a better errmsg...
            m_pdm->msg1(CG_E_VAROUT_NO_LVAL, &m_edp, m_pBoxElem->text);
        //continue after this error...
    }

    //Note: the checkTypedExpr does also fix the type with using Propagate
    //since we know the exact non poly type know there is no need 
    //to get more type info from the network, we can therfore fix the type
    //expr's type here and now. 
    //For the same reason we don't need to implement the NetworkElement::fixTypes2 method.
    return m_pExpr && m_pExpr->checkTypedExpr(CG_TBS_BOOL, true);
}
CG_STStmt* Coil::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    PinElement*      pPin0 = m_pBoxElem->pins.get(0);
    PinElement*      pPin1 = m_pBoxElem->pins.get(1);
    Value*           pInVal = pPin0->pValue;
    Value*           pOutVal= pPin1->pValue;
    ST_Expression_C* pExpr;
    ST_Expression_C* pTmp;
    int              iTrigTmp;
    CG_STStmt*       pStmt;

    assert(pInVal && m_pExpr);
    

    switch(m_pBoxElem->boxFlags)
    {
    case BOX_FLAG_NORMAL:
        //m_pExpr := pInVal;
        pStmt = m_ppc->newAssignStmt(&m_edp, m_pExpr, pInVal->newVarExpr(pBody));
        m_pExpr = NULL;
    break;
    case BOX_FLAG_NEGATE:
        //m_pExpr := not pInVal;
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, pInVal->newVarExpr(pBody), NULL);
        pStmt = m_ppc->newAssignStmt(&m_edp, m_pExpr, pExpr);
        m_pExpr = NULL;
    break;
    case BOX_FLAG_R_EDGE:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //stmt1: m_pExpr = inVal & not inValOld;
        //stmt2: inValOld = inVal
        iTrigTmp = m_ppc->getFreeTriggerTmpNum();

        //stmt1: m_pExpr  := inVal & not inValOld:
        pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, pTmp, NULL);
        pExpr = m_ppc->createOp(&m_edp, CG_AND, pInVal->newVarExpr(pBody), pExpr);
        pStmt = m_ppc->newAssignStmt(NULL, m_pExpr, pExpr);
        m_pExpr = NULL;
        if(pStmt)//can be NULL after error!
        {
            //stmt2: inValOld = inVal
            pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
            pStmt->pNext = m_ppc->newAssignStmt(NULL, pTmp, pInVal->newVarExpr(pBody));
        }
    break;
    case BOX_FLAG_F_EDGE:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //stmt1: m_pExpr = !inVal & inValOld;
        //stmt2: inValOld = inVal
        iTrigTmp = m_ppc->getFreeTriggerTmpNum();

        //stmt1: m_pExpr  := !inVal & inValOld:
        pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
        pExpr = m_ppc->createOp(&m_edp, CG_NOT, pInVal->newVarExpr(pBody), NULL);
        pExpr = m_ppc->createOp(&m_edp, CG_AND, pExpr, pTmp);
        pStmt = m_ppc->newAssignStmt(NULL, m_pExpr, pExpr);
        m_pExpr = NULL;
        if(pStmt)//can be NULL after error!
        {
            //stmt2: inValOld = inVal
            pTmp = new ST_TmpVar_C(&m_edp, m_ppc, iTrigTmp, CG_TBS_BOOL, false, CG_TMP_R_TRIG);
            pStmt->pNext = m_ppc->newAssignStmt(NULL, pTmp, pInVal->newVarExpr(pBody));
        }
    break;
    case BOX_FLAG_SET:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //if pInVal then  m_pExpr = true; end_if;
        pStmt = m_ppc->newAssignStmt(&m_edp, m_pExpr, pBody->getNewTrueExpr(&m_edp));
        if(pStmt)//may be NULL after error!
            pStmt = m_ppc->newIfElseStmt(&m_edp, pInVal->newVarExpr(pBody), pStmt, NULL);
        m_pExpr = NULL;
    break;
    case BOX_FLAG_RESET:
        assert(m_ppc->getCGSym()->pouTyp!=CG_POU_FUN);
        //if pInVal then  m_pExpr = false; end_if;
        pStmt = m_ppc->newAssignStmt(&m_edp, m_pExpr, pBody->getNewFalseExpr(&m_edp));
        if(pStmt)//may be NULL after error!
            pStmt = m_ppc->newIfElseStmt(&m_edp, pInVal->newVarExpr(pBody), pStmt, NULL);
        m_pExpr = NULL;
    break;
    default:
        assert(!"bad boxFlags for coil");
    }
    

    if(pOutVal)
        pOutVal->setAliasVal(pInVal);

    return pStmt;
}
//++++++++++++++++++++Member functions of class Powerrail++++++++++++++++++++++++++++
bool Powerrail::initTypeResolution(LDFBDBody* pBody)
{
    PinElement*         pPin = m_pBoxElem->pins.get(0);
    Value*              pVal = pPin->pValue;

    assert(pPin->lPinType==PIN_TYPE_OUT);
    if(pVal)
    {
        pPin->HACKformalParamType = CG_TBS_BOOL;
        pVal->setDataType(CG_TBS_BOOL);
    }
    else if(m_pdm->hasErr() || m_pdm->getDisplayedWarnings()>0)
    {
        //this may happen if 
        // |
        // +---------
        // | 
        // But then we must have produced an error or the 
        // warning C4301: unconnected line ends
    }
    else
    {
        CGT_IERR2(m_pdm, &m_edp, _T("LD Powerrail has no pValue"));
    }
    return pVal!=NULL;
}
void Powerrail::optimize1(LDFBDBody* pBody)
{
    PinElement*   pPin;
    Value*        pVal;

    pPin = m_pBoxElem->pins.get(0);
    pVal = pPin ? pPin->pValue : NULL;
    assert(!m_isOptimized && pVal);

    //assert: immediate result of e.g. VarWrite cannot be set already 
    //since optimize1 is called in execution order:
    assert(!pVal->isImmediateResult());
    m_isOptimized = true;
    return;
}
CG_STStmt* Powerrail::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    assert(!"Powerrail always is immediate !");
    return NULL;
}
//++++++++++++++++++++Member functions of class Return++++++++++++++++++++++++++++
bool Return::initTypeResolution(LDFBDBody* pBody)
{
    PinElement*         pPin = m_pBoxElem->pins.get(0);
    Value*              pVal = pPin->pValue;
    
    assert(pPin->lPinType==PIN_TYPE_IN);
    if(pVal)
    {    
        pPin->HACKformalParamType = CG_TBS_BOOL;
    }
    else
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
                    m_pdm->res1(CG_RES_STR_RETURN), _T(""));
    }
    return pVal!=NULL;
}
CG_STStmt* Return::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    post->add(this);
    return NULL;
}
CG_STStmt* Return::postProcess(LDFBDBody* pBody)
{
    ST_Expression_C* pCond;
    CG_STStmt*       pStmt;
    Value*           pCondVal;

    pStmt    = m_ppc->newReturnStmt(&m_edp);
    pCondVal = m_pBoxElem->pins.get(0)->pValue; 
    if(pCondVal)
    {
        pCond = pCondVal->newVarExpr(pBody);
        if(pCond)
            pStmt = m_ppc->newIfElseStmt(&m_edp, pCond, pStmt, NULL);
    }
    else
    {
        //happens only if connected to ENO, then the if(EN) is used as condition.
        assert(getENENOCondition()); 
    }
    return pStmt;
}
//++++++++++++++++++++Member functions of class SFC Return++++++++++++++++++++++++++++
bool SFCReturn::initTypeResolution(LDFBDBody* pBody)
{
    PinElement*         pPin = m_pBoxElem->pins.get(0);
    Value*              pVal = pPin->pValue;
    
    assert(pPin->lPinType==PIN_TYPE_IN);
    if(pVal)
        pPin->HACKformalParamType = CG_TBS_BOOL;
    else
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
                    m_pdm->res1(CG_RES_STR_RETURN), _T(""));
    }
    return pVal!=NULL;
}
CG_STStmt* SFCReturn::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    post->add(this);
    return NULL;
}
CG_STStmt* SFCReturn::postProcess(LDFBDBody* pBody)
{
    //breakpoint at every return enabled even if only -BP1
    //therefore we don't need to do it here
    //TODO BP??
    return m_ppc->newTransAssignStmt(&m_edp, m_pBoxElem->pins.get(0)->pValue->newVarExpr(pBody));
}
//++++++++++++++++++++Member functions of class Jump++++++++++++++++++++++++++++
bool Jump::initTypeResolution(LDFBDBody* pBody)
{
    PinElement*         pPin = m_pBoxElem->pins.get(0);
    Value*              pVal = pPin->pValue;

    assert(pPin->lPinType==PIN_TYPE_IN);
    if(pVal)
        pPin->HACKformalParamType = CG_TBS_BOOL;
    else
    {
        m_pdm->msg2(CG_E_BOX_UNCONNECTED, &m_edp,
            m_pdm->res1(CG_RES_STR_JUMP), m_pBoxElem->text);
    }

    //check the Label identifier:
    ST_CheckIdentifier(pBody->getDM(), &m_edp, m_pBoxElem->text);
    //ignore error here, code gen will not be called if m_ppc->getErrorsInFile()>0
    
    return pVal!=NULL;
}
CG_STStmt* Jump::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    post->add(this);
    return NULL;
}
CG_STStmt* Jump::postProcess(LDFBDBody* pBody)
{
    ST_Expression_C*      pCond;
    CG_STStmt*            pStmt;
    Value*                pCondVal;

    pStmt     = m_ppc->newGotoStmt(&m_edp, m_pBoxElem->text);
    pCondVal  = m_pBoxElem->pins.get(0)->pValue;
    if(pCondVal)
    {
        pCond = pCondVal->newVarExpr(pBody);
        if(pCond)
            pStmt = m_ppc->newIfElseStmt(&m_edp, pCond, pStmt, NULL);
    }
    else
    {
        //happens only if connected to ENO, then the if(EN) is used as condition.
        assert(getENENOCondition()); 
    }

    return pStmt;
}
//++++++++++++++++++++Member functions of class Label++++++++++++++++++++++++++++
bool Label::initTypeResolution(LDFBDBody* pBody)
{
    //check the Label identifier:
    ST_CheckIdentifier(m_pdm, &m_edp, m_pBoxElem->text);
    //ignore error here, code gen will not be called if m_ppc->getErrorsInFile()>0
    return true;
}
CG_STStmt* Label::generateCode(LDFBDBody* pBody, FC_CGArray<NetworkElement*>* post)
{
    return m_ppc->newLabel(&m_edp, m_pBoxElem->text, NULL);
}












//++++++++++++++++++++Member functions of class Network++++++++++++++++++++++++
/** compareNWE( (void *) elem1, (void *) elem2 );
 *  Use this function to sort NWEs by evaluation order 
 *  The routine must compare the NWEs 'from top to bottom
 *  and left to right'.
 *
 *  Return one of the following values:
 *
 *  Return Value	Description
 *
 *  < 0	elem1 less than elem2 or "should be evaluated before elem2"
 *  0	elem1 equivalent to elem2 
 *  > 0	elem1 greater than elem2 or "should be evaluated after elem2" 
 */
static int CompareNWE(const void *elem1, const void *elem2)
{
    const NetworkElement *nwe1 = *reinterpret_cast<const NetworkElement **>(const_cast<void *>(elem1));
    const NetworkElement *nwe2 = *reinterpret_cast<const NetworkElement **>(const_cast<void *>(elem2));
    assert(nwe1 && dynamic_cast<NetworkElement*>(const_cast<NetworkElement*>(nwe1)) == nwe1);
    assert(nwe2 && dynamic_cast<NetworkElement*>(const_cast<NetworkElement*>(nwe2)) == nwe2);
    
    Position pos1=nwe1->getPosition(); 
    Position pos2=nwe2->getPosition(); 
    
    //compare y-coordinate first:
    if(pos1.y<pos2.y)
        return -1;
    if(pos1.y>pos2.y)
        return +1;

    //y-coordinate is equal: now left to right
    if(pos1.x<pos2.x)
        return -1;
    if(pos1.x>pos2.x)
        return +1;

    return 0;
}


#ifdef DEBUG 
#include <stdio.h>
static DumpNWE(const NetworkElement* pNWE)
{
    FC_CString        jot;
    const BoxElement* pBox = pNWE->getBox();

    BoxTypeToDiagString(pBox->boxType, jot, NULL);//TODO
    
    _tprintf(_T("NWE(%3d;%3d): text=%-10s,%s  %s\n"), 
        pBox->ext.left, pBox->ext.top, 
        pBox->text.psz(), pBox->instName.psz(), jot.psz());
}
#endif

CG_STStmt* Network::generateNetworkCode(
    LDFBDBody* pBody 
)
{
    int                         i;
    bool                        bOk;
    NetworkElement*             pNWE;
    NetworkElement*             pNWEFirstEN;
    int                         nTypeErrors;
    int                         nNWElements = m_NWelements.getUsed();
    FC_CGArray<NetworkElement*> NWEsortedByExecOrder;
    FC_CGArray<NetworkElement*> post(256);
    CENStmtList                 NWstmtList(pBody);
    CG_STStmt*                  pStmt;

    

    //TO_DEBUGPRINT4(_T("4CCG> Network(%d,%d,%d,%d)::generateNetworkCode\n"), 
    //    m_extent.left, m_extent.top, m_extent.right, m_extent.bottom);
    //CAUTION: don't dump that RTCE  !!!!!! ??????????????
    //dmp<<_T("\n//Network(")<<m_extent.left<<_T(',')<<m_extent.top<<_T(',')<<m_extent.right
    //    <<_T(',')<<m_extent.bottom<<_T(")\n");

   
    //sort 1: by graphical evaluation order
    qsort(reinterpret_cast<void*>(m_NWelements.getElements()),
          nNWElements, sizeof(m_NWelements.get(0)), CompareNWE);

//Debud code    for(i=0; i<nNWElements; i++) 
//    {
//        pNWE = m_NWelements.get(i);
//        DumpNWE(pNWE);
//    }

    //sort 2) by dataflow
    //while (there are elements that can be executed)
    //   find first element that can be executed and execute it
    NWEsortedByExecOrder.allocSize(nNWElements);
    for(i=0; i<nNWElements; ) 
    {
        pNWE  = m_NWelements.get(i);
        if(pNWE->canBeEvaluated()) 
        {
            NWEsortedByExecOrder.add(pNWE);
            pNWE->setEvaluated();
            i=0;
            continue;
        }
        i++;
    }

    //search for boxes that should but could not eval.
    //This may happen if e.g.
    //          +---------+
    //          |  +---+  |    
    //          |  |add|  |
    //          +--|   |--+
    //     x-------|   |
    //             +---+   
    //
    for(i=0; i<nNWElements; i++) 
    {
        pNWE  = m_NWelements.get(i);
        if(!pNWE->isEvaluated())
        {
            pBody->getDM()->msg0(CG_E_CANNOT_EVAL_BOX, pNWE->getEdiPos());
            return NULL;
        }
    }
    
    
    //call NetworkElement::initTypeResolution to parse varin/out 
    //expressions and get fun/FB type definitions. Also check if 
    //fun/FB version is ok, if any.
    bOk = true;
    pNWEFirstEN = NULL;
    for (i=0; i<nNWElements; i++) 
    {
        pNWE = NWEsortedByExecOrder.get(i);
        if(!pNWE->initTypeResolution(pBody))
            bOk = false;
        if(!pNWEFirstEN && pNWE->getEN())
            pNWEFirstEN = pNWE;
    }

    //if initTypeResolution failed, return now !
    if(!bOk || pBody->getDM()->getTotalErrors()>=ST_MAX_NUMBER_ERRORS)
        return NULL;
    
    //befor doing the type resolution check the EN/ENO topology:
    if(pNWEFirstEN && !checkEnEnoUsage(pBody, NWEsortedByExecOrder, pNWEFirstEN))
        return NULL;
    
    
    //type resolution, use iterative algorithm:
    nTypeErrors = 0;
    for (i=0; i<nNWElements; ) 
    {
        pNWE = NWEsortedByExecOrder.get(i);
        if(!pNWE->hasTypeError() && pNWE->typeResolutionIteration(pBody, &nTypeErrors))
            i=0;//no pending type errors and new type info found => start again
        else 
            i++;//next network element...
    }

    //if any type errors, return now
    if(nTypeErrors>0 || pBody->getDM()->getTotalErrors()>=ST_MAX_NUMBER_ERRORS)
    {
        assert(pBody->getPPC()->getErrorsInFile()>0);
        return NULL;
    }

    //fix types if still polymorph:
    for(i=0; i<nNWElements; i++) 
        NWEsortedByExecOrder.get(i)->fixTypes(pBody);
    for(i=0; i<nNWElements; i++) 
        NWEsortedByExecOrder.get(i)->fixTypes2(pBody);
        
    //finally generate code, but only if no errors at all.
    if(pBody->getPPC()->getErrorsInFile()==0)
    {
        //optimize tmp var use:
        for (i=0; i<nNWElements; i++) 
            NWEsortedByExecOrder.get(i)->optimize1(pBody);

        for(i=0; i<nNWElements; i++) 
        {
            pNWE = NWEsortedByExecOrder.get(i);
            if(!pNWE->isOptimised())
            {
                pStmt = pNWE->generateCode(pBody, &post);
                if(pStmt) 
                    NWstmtList.append(pNWE->getENENOCondition(), pStmt);
            }
        }
        if(pBody->getDM()->getTotalErrors()>=ST_MAX_NUMBER_ERRORS)
            return NULL;

        //now do the jumps & returns here!
        for (i=0; i<post.getUsed(); i++) 
        {
            pNWE = post.get(i);
            //assert: don't want them to be part of the enabled part the the network:
            //must be checked by checkEnEnoUsage, also there is nothing to optimize
            //assert(!pNWE->getENENOCondition() && !pNWE->isOptimised());
            pStmt= pNWE->postProcess(pBody);
            if(pStmt)
                NWstmtList.append(pNWE->getENENOCondition(), pStmt);
        }
    }
    
    //done    
    return NWstmtList.getFirst();
}

bool Network::checkEnEnoUsage(
    LDFBDBody*                   pBody, 
    FC_CGArray<NetworkElement*>& NWEsortedByExecOrder,
    NetworkElement*              pNWEFirstEN
)
{
    int                             i;
    int                             nNWElements;
    NetworkElement*                 pLastEnEno;
    NetworkElement*                 pCurrEnEno;
    NetworkElement*                 pNextEnEno;
    bool                            bError;
    Value*                          pENExprVal;
    
    
    bError = false;

    pENExprVal = pNWEFirstEN->getEN()->pValue;
    //assert: value was set even if unconnected EN, see special case in NetworkElement::init2
    assert(pENExprVal);



    for(pCurrEnEno = pNWEFirstEN,  pLastEnEno = NULL ; 
        pCurrEnEno && pCurrEnEno->getENO(); 
        pLastEnEno = pCurrEnEno,  pCurrEnEno = pNextEnEno
       )
    {
        const PinElement*               pPin;
        const FC_CGArray<PINBOX_TYP>*   pInPinCon;
        BOX_TYPE                        boxt;
        PINBOX_TYP                      PinBox;

        //check the connections of the ENO:
        pCurrEnEno->setENENOCondition(pNWEFirstEN);

        pPin = pCurrEnEno->getENO();
        //simply check that lPinFlags is 0, no attribs allowed for en/eno pins:
        if(pPin->lPinFlags || pCurrEnEno->getEN()->lPinFlags)
        {
            pBody->getDM()->msg1(CG_E_EN_USAGE, pCurrEnEno->getEdiPos(), 
                pBody->getDM()->res1(CG_RES_STR_EN_NOTNEGATED));
            bError = true;
            //continue after this error:
        }

        if(pPin->pValue)
        {
            pInPinCon = pPin->pValue->getInPinsConnected();
            if(pInPinCon->getUsed()>1)
            {
                //will not happen in LD only in FBD
                pBody->signalDiag1(CG_E_EN_USAGE, pPin->pos, 
                    pBody->getDM()->res1(CG_RES_STR_ENO_NOTSPLIT));
                bError = true;
                break;
            }
        

            //check allowed connection at eno: ret,jmp and next en
            PinBox = pInPinCon->get(0);
            boxt   = PinBox.pNWE->getBox()->boxType;

            if(boxt==BOX_TYPE_JUMP || boxt==BOX_TYPE_RETURN)
            {
                pNextEnEno = NULL;

                //we must pPin->pValue to NULL here, otherwise the ENO pPin->pValue
                //will not be prepared to generate a value, note that the EN/ENO
                //pins are not considered in FunFBBox::genereateCode.
                PinBox.pPin->pValue = NULL;//HACK??
                //set the EN/ENO condition instead:
                PinBox.pNWE->setENENOCondition(pNWEFirstEN);
            }
            else if(PinBox.pPin->lPinType==PIN_TYPE_EN)
            {
                assert(PinBox.pPin == PinBox.pNWE->getEN());
                //get the next EN/ENO fun/fb box:
                pNextEnEno = PinBox.pNWE;
            }
            else
            {
                pBody->getDM()->msg1(CG_E_EN_USAGE, pCurrEnEno->getEdiPos(), 
                    pBody->getDM()->res1(CG_RES_STR_ENO_NOTCONNECT));
                bError = true;
                break;
            }
        }
        else
        {
            pNextEnEno = NULL;
        }


        //check the connections of all other non EN/ENO in/outputs:
        const FC_CGArray<PinElement*>*  pPins = & pCurrEnEno->getBox()->pins;
        int                             nPins = pPins->getUsed();
        for(i=0;i<nPins;i++)
        {
            pPin = pPins->get(i);
            if(!pPin->pValue || pPin->lPinType & (PIN_TYPE_EN|PIN_TYPE_ENO))
                continue;

            //must be 
            pInPinCon = pPin->pValue->getInPinsConnected();
            assert(pInPinCon->getUsed()>0); //<- there is a pPin->pValue !
            
            // HACK don't check connections on INOUT pins at all
            if(!(pPin->lPinType & PIN_TYPE_INOUT_IN) && pInPinCon->getUsed()>1)
            {
                pBody->signalDiag1(CG_E_EN_USAGE, pPin->pos, 
                    pBody->getDM()->res1(CG_RES_STR_EN_SIGNALSPLIT));
                bError = true;
                break;
            }

            if(pPin->lPinType & ANY_IN_PIN)
            {
                NetworkElement* pValSrcNWE = pPin->pValue->getCreatorNWE();
                BOX_TYPE boxTyp            = pValSrcNWE->getBox()->boxType;

                if(boxTyp!=BOX_TYPE_VARREAD && pValSrcNWE!=pLastEnEno)
                {
                    pBody->getDM()->msg1(CG_E_EN_USAGE, pCurrEnEno->getEdiPos(), 
                        pBody->getDM()->jot1()<<_T("'")<<pPin->szName<<_T("' ")<<
                        pBody->getDM()->res2(CG_RES_STR_EN_BADINPUT));
                    bError = true;
                    break;
                }
                if(pValSrcNWE!=pLastEnEno)
                    pValSrcNWE->setENENOCondition(pNWEFirstEN);
            }
            else
            {
                assert(pPin->pValue->getCreatorNWE()==pCurrEnEno);
                NetworkElement* pValDstNWE = pInPinCon->get(0).pNWE;
                boxt  = pValDstNWE->getBox()->boxType;
                if(boxt!=BOX_TYPE_VARWRITE && pValDstNWE!=pNextEnEno)
                {
                    FC_CString jot;
                    if(pPin->szName[0])
                        jot<<_T("'")<<pPin->szName<<_T("' ");
                    else
                    {
                        jot.loadRes(CG_RES_STR_FCT_RES, pBody->getDM()->getResHMODULE())<<_T(" '")<<pCurrEnEno->getBox()->text<<_T("' ");
                    }

                    pBody->getDM()->msg1(CG_E_EN_USAGE, pCurrEnEno->getEdiPos(), 
                        jot<<pBody->getDM()->res1(CG_RES_STR_EN_BADOUTPUT));
                    bError = true;
                    break;
                }
                if(pValDstNWE!=pNextEnEno)
                    pValDstNWE->setENENOCondition(pNWEFirstEN);
            }
        }

        if(bError)
            break;
    }

    if(bError)
        return false;


    //loop all NWEs and check that all EN-boxex have now a condition
    //and that this condition is the first and only one.
    nNWElements = NWEsortedByExecOrder.getUsed();
    for(i=0; i<nNWElements; i++)
    {
        const NetworkElement* pNWE = NWEsortedByExecOrder.get(i);
        if(!pNWE->getEN())
            continue;
        if(pNWE->getENENOCondition() != pNWEFirstEN)
        {
            //assert: actually it must be NULL not any other NEW...
            assert(pNWE->getENENOCondition()==NULL);
            pBody->getDM()->msg1(CG_E_EN_USAGE, pNWE->getEdiPos(), 
                pBody->getDM()->res1(CG_RES_STR_EN_NOTCONNECT));
            bError = true;
        }
    }

    return !bError;
}







//++++++++++++++++++++Member functions of class LDFBDBody++++++++++++++++++++++
LDFBDBody::LDFBDBody(
    LDFBD_SignalHash* pGraphic, 
    st_ParseContext*  ppc,
    long              codeGenOpt  //CG_LOADBDY_* bits
) 
{
    assert(pGraphic && ppc);
    
    m_ppc = ppc;
    m_pdm = ppc->getDM();
    m_pFront = ppc->getFront();
    m_pSym = ppc->getSym();
    m_pGraphic = pGraphic;
    m_pMultipleWrites = NULL;
    m_codeGenOpt = codeGenOpt;
}
LDFBDBody::~LDFBDBody()
{
    int n;

    for(n=m_networkList.getUsed()-1;n>=0;n--)
        delete(m_networkList.get(n));

    for(n=m_networkElemList.getUsed()-1;n>=0;n--)
        delete(m_networkElemList.get(n));
    
    for(n=m_valueList.getUsed()-1;n>=0;n--)
        delete(m_valueList.get(n));

    delete(m_pMultipleWrites);
}



void LDFBDBody::signalDiag0(HRESULT errId, const Position& pos)const
{
    CG_Edipos edp;
    InitSignalEdp(&edp, m_ppc->getEDPSrcFile(), pos.x, pos.y);
    m_pdm->msg0(errId, &edp);
}
void LDFBDBody::signalDiag1(HRESULT errId, const Position& pos, const TCHAR* psz1)const
{
    CG_Edipos edp;
    InitSignalEdp(&edp, m_ppc->getEDPSrcFile(), pos.x, pos.y);
    m_pdm->msg1(errId, &edp, psz1);
}

void LDFBDBody::getSignalAndWarn(
    Position                  start,
    long                      pinType,
    FC_CGArray<LineElement*>* pReachedLines,
    FC_CGArray<PinElement*>*  pReachedPins,
    Extension*                pExtension
)
{
    int iRet;
    
    iRet = m_pGraphic->getSignal(start, pinType, pReachedLines, pReachedPins, pExtension);

    //iRet: 0=ok, 1=signal cyclic, 2=unconnected line ends 3=both
    if(iRet!=0)
    {
        //problem: don't dump same error twice for same signal.
        //solution: call setSignalFlag, return is true if flag was 
        //not set for at least one line of the signal.
        if(setSignalFlag(start, pinType, LINE_FLAG_UNCONNECT_WARN))
        {
            if(iRet & 1)
                signalDiag0(CG_W_SIGNAL_CYCLIC, start);
            if(iRet & 2)
                signalDiag0(CG_W_UNCONNECTED_LINE, start);
        }
    }
}
bool LDFBDBody::setSignalFlag(Position start, long pinType, long lFlag)
{
    int                      n;
    LineElement*             pLine;
    bool                     bFlagWasNotSetInAllLines = false;
    FC_CGArray<LineElement*> lines(64);

    assert(lFlag);

    m_pGraphic->getSignal(start, pinType, &lines, NULL, NULL);
    
    n=lines.getUsed();
    assert(n>0);
    if(n<=0)
    {   // should currently never happen, but to be save
        // the caller should display an error anyway.
        // NOTE: the situation could be a signal consisting 
        // only of two pins connected. Maybe in future this function
        //is called in such situations, then remove this assert.
        return true;
    }

    for(n=n-1;n>=0;n--)
    {
        pLine = lines.get(n);
        if((pLine->flags & lFlag)==0)
        {
            bFlagWasNotSetInAllLines = true;
            pLine->flags |= lFlag;
        }
    }

    return bFlagWasNotSetInAllLines;
}

ST_Expression_C* LDFBDBody::parseExpr(const BoxElement* pBox)
{
    CG_Edipos        edp;
    ST_Expression_C* pExpr;

    assert(pBox->boxType==BOX_TYPE_VARREAD || pBox->boxType==BOX_TYPE_VARWRITE ||
            pBox->boxType==BOX_TYPE_VARFEEDBACK || pBox->boxType==BOX_TYPE_CONTACT
            || pBox->boxType==BOX_TYPE_COIL);
    
    pExpr = m_ppc->parse4GRBox(boxEdp(&edp, pBox->ext), pBox->text, false);
    assert(pExpr || m_ppc->getErrorsInFile()>0);
    return pExpr;
}
ST_Expression_C* LDFBDBody::reparseExpr(const NetworkElement* pNWE)
{
    CG_Edipos         edp;
    ST_Expression_C*  pExpr;
    const BoxElement* pBox = pNWE->getBox();

    assert(pBox->boxType==BOX_TYPE_VARREAD || pBox->boxType==BOX_TYPE_VARWRITE ||
            pBox->boxType==BOX_TYPE_VARFEEDBACK || pBox->boxType==BOX_TYPE_CONTACT
            || pBox->boxType==BOX_TYPE_COIL);

    pExpr = m_ppc->parse4GRBox(boxEdp(&edp, pBox->ext), pBox->text, true);
    assert(pExpr || m_ppc->getErrorsInFile()>0);
    return pExpr;
}


/////////////////////////////////////////////////////////////////////
/**
 * Converts the XML Structure into the intermediate structure
 * that is used to check & compile a LD/FBD body.
 *
 * @param c     
 * @return 0-9  
 *              
 * @see         ...
 */
bool LDFBDBody::createNetworkElements()
{
    int                     n;
    NetworkElement*         pBox;
    bool                    bResult = true;
    BoxElement*             pBoxElem;
    BoxElement*             pBoxElem2;
    VarReadBox*             pVarInBox;
    PinElement*             pPin1;
    PinElement*             pPin2;
    int                     nBoxes;
    int                     nConnectors = m_pGraphic->theConnPairs.getUsed();
    bool                    bSignalError = false;

    //reset reached flag now
    n=m_pGraphic->theLines.getUsed();
    while(n-->0)
        m_pGraphic->theLines.get(n)->flags &= ~LINE_FLAG_REACHED;
    
    //handle connectors, convert them into lines
    for(n=0;n<nConnectors;n++)
    {
        CG_Edipos       edp;
        const TCHAR*    pszConn;
        Position        origin;
        CONNECTOR_PAIR& pair = m_pGraphic->theConnPairs.get(n);
        
        if(pair.pConnector)
        {
            origin = pair.pConnector->origin;
            pszConn =  pair.pConnector->text;
        }
        else if(pair.pContinuation)
        {
            origin = pair.pContinuation->origin;
            pszConn = pair.pContinuation->text;
        }
        else
        {
            assert(!"bad conn pair struct");
            continue;
        }
        boxEdp(&edp, origin);

        //check error conditions
        //1. check: Identifier syntax
        if(!ST_CheckIdentifier(m_pdm, &edp, pszConn))
        {
            bResult = false;
            continue;
        }

        //2. connector pair name must not be a Label name:
        if(m_pGraphic->labelNames.findI(pszConn))
        {
            bResult = false;
            m_pdm->msg2(CG_E_CONNPAIR_NAME_IS_VAR, &edp, 
                pszConn, m_pdm->res1(ST_RES_STR_LABEL));
            continue;
        }
    
        //3. connector pair name must not be a local var name:
        if(m_pSym->lookUpEntry(pszConn))
        {
            bResult = false;
            m_pdm->msg2(CG_E_CONNPAIR_NAME_IS_VAR, &edp, 
                pszConn, m_pdm->res1(ST_RES_STR_VARIABLE));
            continue;
        }

        //4. connector pair name must not be a CONST name:
        if(m_pFront->lookUpCONST(pszConn))
        {
            bResult = false;
            m_pdm->msg2(CG_E_CONNPAIR_NAME_IS_VAR, &edp, 
                pszConn, m_pdm->res1(ST_RES_STR_NAMED_CONST));
            continue;
        }
        
        //5. check: must be a pair:
        if(!pair.pConnector  || !pair.pContinuation)
        {
            bResult = false;
            int errId = pair.pConnector ? CG_E_MISSING_CONNECTOR 
                                        : CG_E_MISSING_CONTINUATION;
            m_pdm->msg1(errId, &edp, pszConn);
            continue;
        }

        //6. reuse of connector pair name is not allowed:
        if(pair.bDuplyDefined)
        {
            bResult = false;
            m_pdm->msg1(CG_E_CONNPAIR_NOT_UNIQUE, &edp, pszConn);
            continue;
        }

   
        //insert a simple line:
        LineElement* pLine = new LineElement();

        pLine->beg = pair.pConnector->pinpos;
        pLine->end = pair.pContinuation->pinpos;
        pLine->flags = 0;
        //calculate extension of line (minimal sourrounding rect:
        GetExtensionFrom2Pos(pLine->beg, pLine->end, &pLine->ext);
        m_pGraphic->addLineElement(pLine);
    }



    if(bResult && m_pGraphic->isLD()){
        /*
         * generate some artificial LADDER elements
         */
        FC_CGArray<Position>     inputPositions(64), outputPositions(64);
        FC_CGArray<LineElement*> reachedLines(64);
        int                      nLines = m_pGraphic->theLines.getUsed();

        for(n=0;n<nLines;n++)
        {
            LineElement *pLine=m_pGraphic->theLines.get(n);
            if(pLine->flags & LINE_FLAG_REACHED)
                continue;

            if(pLine->flags & LINE_FLAG_VERTICAL)
            {
                Extension Extent = pLine->ext;
                int       result = m_pGraphic->getWiredOr(pLine->beg, &inputPositions, 
                                                 &outputPositions, &reachedLines, &Extent);

                if(inputPositions.getUsed() == 0)
                {
                    bResult = false;
                    CG_Edipos       edp;
                    lineEdp(&edp, Extent);
                    m_pdm->msg1(CG_E_NOT_SUPPORTED, &edp, m_pdm->res1(CG_RES_STR_OR_NOINPUT));
                    continue;
                }
 
                if(outputPositions.getUsed() == 0)
                {
                    bResult = false;
                    CG_Edipos       edp;
                    lineEdp(&edp, Extent);
                    m_pdm->msg1(CG_E_NOT_SUPPORTED, &edp, m_pdm->res1(CG_RES_STR_OR_NOOUTPUT));
                    continue;
                }


                //special case: 
                //'wired or' with one in and one out pin, then insert a normal line
                if(inputPositions.getUsed()==1 && outputPositions.getUsed()==1 && reachedLines.getUsed()==1)
                {
                    Position inPos  = inputPositions.get(0);
                    Position outPos = outputPositions.get(0);
                    if(!POS_EQ(inPos, outPos))
                    {
                        LineElement* pLine = reachedLines.get(0);
                        pLine->flags &= ~(LINE_FLAG_REACHED|LINE_FLAG_VERTICAL);
                        pLine->beg    = inPos;
                        pLine->end    = outPos;
                        continue;
                    }//else could be like this:
                     //  ----+----
                     //      |
                }


                pBoxElem = new BoxElement();
                pBoxElem->boxType = BOX_TYPE_WIREDOR;
                pBoxElem->boxFlags = BOX_FLAG_NORMAL;
                pBoxElem->ext = Extent;
                m_pGraphic->addBoxElement(pBoxElem);

                for (int i=0; i<inputPositions.getUsed(); i++)
                {
                    pPin1 = new PinElement();
                    InitPinElement(pPin1);
                    pPin1->lPinType = PIN_TYPE_WIREDOR_IN;
                    pPin1->lPinFlags= 0;
                    pPin1->pos = inputPositions.get(i);
                    pBoxElem->pins.add(pPin1);

                    m_pGraphic->addPinElement(pPin1);
                }

                for (int j=0; j<outputPositions.getUsed(); j++)
                {
                    pPin2 = new PinElement();
                    InitPinElement(pPin2);
                    pPin2->lPinType = PIN_TYPE_WIREDOR_OUT;
                    pPin2->lPinFlags= 0;
                    pPin2->pos = outputPositions.get(j);
                    pBoxElem->pins.add(pPin2);

                    m_pGraphic->addPinElement(pPin2);
                }
            }
            else if(pLine->flags & LINE_FLAG_POWERRAIL)
            {
                int result = m_pGraphic->chkPwrRail(pLine->beg);
                pLine->flags |= LINE_FLAG_REACHED;
                if (result == 1)
                {
                    pBoxElem = new BoxElement();
                    pBoxElem->boxType = BOX_TYPE_POWERRAIL;
                    pBoxElem->boxFlags = BOX_FLAG_NORMAL;
                    pBoxElem->ext = pLine->ext;
                    m_pGraphic->addBoxElement(pBoxElem);

                    pPin1 = new PinElement();
                    InitPinElement(pPin1);
                    pPin1->lPinType = PIN_TYPE_OUT;
                    pPin1->lPinFlags= 0;
                    pPin1->pos = pLine->beg;
                    pBoxElem->pins.add(pPin1);

                    m_pGraphic->addPinElement(pPin1);
                }
                else if (result == 3)
                {
                    bResult = false;
                    CG_Edipos       edp;
                    lineEdp(&edp, pLine->ext);
                    m_pdm->msg1(CG_E_NOT_SUPPORTED, &edp, m_pdm->res1(CG_RES_STR_BAD_POWERRAIL));
                    continue;
                }
            }
        }
    }




    if(bResult)
    {
        //add all boxes. Note: during for loop m_pGraphic->theBoxes
        //will increase, therefore call getSize only once:
        nBoxes = m_pGraphic->theBoxes.getUsed();
        for(n=0;n<nBoxes;n++)
        {
            pBoxElem = m_pGraphic->theBoxes.get(n);

            switch(pBoxElem->boxType)
            {
            case BOX_TYPE_VARFEEDBACK:
                //change a varinout to a varin and a new varout:
                //CAUTION: reuse the pins, because they have already been
                //added to the hash table.

                if(m_pGraphic->isLD())
                {
                    bResult = false;
                    CG_Edipos       edp;
                    boxEdp(&edp, pBoxElem->ext);
                    m_pdm->msg1(CG_E_NOT_SUPPORTED, &edp, m_pdm->res1(CG_RES_STR_REFFEEDBACK));
                    continue;
                }

                pPin1 = pBoxElem->pins.get(0);
                pPin2 = pBoxElem->pins.get(1);
                assert(pPin1 && pPin1->lPinType==PIN_TYPE_IN);
                assert(pPin2 && pPin2->lPinType==PIN_TYPE_OUT);

                //change pBoxElem to a varin box,  but leave pBoxElem->boxType as is:
                pBoxElem->pins.set(0, pPin1->lPinType==PIN_TYPE_OUT ? pPin1 : pPin2);
                pBoxElem->pins.setUsed(1);

                //change the box extend, it will prevent overlapping networks error
                //in the case: 
                //             +---+                                +---+
                //             |FB1|                                |FB2|
                //             |   |                                |   |--x
                //     x-------|   |--------- feedbackvar-----------|   |
                //             +---+                                +---+
                //(NOTE: this kind of NW will produce an "illegal use of feedback var" error
                //anyway, but prior to this error "overlapping NW" would be misleading)

                //create a new varout box:
                pBoxElem2 = new BoxElement();
                pBoxElem2->ext = pBoxElem->ext;
                pBoxElem2->ext.right = pBoxElem->ext.left;
                pBoxElem->ext.left = pBoxElem->ext.right;


                //add var in box:
                pBox = pVarInBox = new VarReadBox(pBoxElem, this);

                if(!pBox->init(this))
                    bSignalError = true;
                m_networkElemList.add(pBox);

                //init varout box elem:
                pBoxElem2->boxType = BOX_TYPE_VARFEEDBACK;
                pBoxElem2->boxFlags = BOX_FLAG_NORMAL;
                pBoxElem2->pins.add(pPin1->lPinType==PIN_TYPE_IN ? pPin1 : pPin2);
                pBoxElem2->text.load(pBoxElem->text);
                assert(pBoxElem->instName[0]==0);

                //don't call addBoxElement(pBoxElem2) before pBoxElem2 is initialized:
                m_pGraphic->addBoxElement(pBoxElem2);

                //add var out box, will be appended after the switch:
                pBox = new FeedBackVarOutBox(pBoxElem2, pVarInBox, this);
            break;
            case BOX_TYPE_VARREAD:  pBox = new VarReadBox(pBoxElem, this);  break;
            case BOX_TYPE_VARWRITE: pBox = new VarWriteBox(pBoxElem, this); break;
            case BOX_TYPE_WIREDOR:  pBox = new WiredOr(pBoxElem, this);     break;
            case BOX_TYPE_COIL:     pBox = new Coil(pBoxElem, this);        break;
            case BOX_TYPE_CONTACT:  pBox = new Contact(pBoxElem, this);     break;
            case BOX_TYPE_POWERRAIL:pBox = new Powerrail(pBoxElem, this);   break;
            case BOX_TYPE_FB:       pBox = new FBBox(pBoxElem, this);       break;
            case BOX_TYPE_FNC:      pBox = new FunBox(pBoxElem, this);      break;
            case BOX_TYPE_LABEL:    pBox = new Label(pBoxElem, this);       break;
            case BOX_TYPE_JUMP:     pBox = new Jump(pBoxElem, this);        break;
            case BOX_TYPE_RETURN:   pBox = new Return(pBoxElem, this);      break;
            case BOX_TYPE_SFC_RETURN: pBox = new SFCReturn(pBoxElem, this); break;
            default:
                assert(!"bad enum");
                bResult = false;
                pBox = NULL;
            }

            if(pBox) {
                if (!pBox->init(this))
                    bSignalError = true;
                m_networkElemList.add(pBox);
            }
        }
    }

    if(bResult)
    {
        //call NetworkElement::init2 to find varIn(s)
        for(n=0; n<m_networkElemList.getUsed(); n++) 
        {
             if(!m_networkElemList.get(n)->init2(this))
                 bSignalError = true;
        }

        //check for lines that are not connected 
        //to any box or just rubber:
        for(n=0; n<m_pGraphic->theLines.getUsed(); n++)
        {
            LineElement* pLine = m_pGraphic->theLines.get(n);

            if(pLine->flags & LINE_FLAG_RUBBERLINE)
            {
                diagnosticMsgL(pLine, CG_W_RUBBER_LINE);
            }
            if((pLine->flags & LINE_FLAG_REACHED)==0)
            {
                //line was not reached by any call to getSignal:
                //produce a warning by means of this->getSignalAndWarn:
                getSignalAndWarn(pLine->beg, 0, NULL, NULL, NULL); 
            }
        }
    }

    //return false if bSignalError, errors produced by XXX::init 
    //or XXX::init2 do set bSignalError==true and are unrecoverable...
    //check: don't return false only if at least one error 
    //in this file context:
    if(bSignalError && m_ppc->getErrorsInFile()<=0)
        CGT_IERR(m_pdm, "??");

    return bResult && !bSignalError;
}
void LDFBDBody::createNetworkList()
{
    int         i;
    Network*    pnewNetwork;


    for(;;) 
    {
        for (i=0; i<m_networkElemList.getUsed(); i++) 
        {
            if (!m_networkElemList.get(i)->getNetwork())
                break;
        }
        assert(i<=m_networkElemList.getUsed());
        if (i==m_networkElemList.getUsed())
            break;
        
        pnewNetwork = new Network;
        m_networkList.add(pnewNetwork);
        m_networkElemList.get(i)->setNetwork(pnewNetwork);

        i=0;
        while (i<m_networkElemList.getUsed()) 
        {
            if (m_networkElemList.get(i)->findNetwork()) 
            {
                i=0;
                continue;
            }
            i++;
        }
    }
}



/** compareNW( (void *) elem1, (void *) elem2 );
 *  Use this function to sort NWs by evaluation order 
 *  The routine must compare the NWs 'from top to bottom
 *  and left to right'.
 *
 *  Return one of the following values:
 *
 *  Return Value	Description
 *
 *  < 0	elem1 less than elem2 or "should be evaluated before elem2"
 *  0	elem1 equivalent to elem2 
 *  > 0	elem1 greater than elem2 or "should be evaluated after elem2" 
 */
static int CompareNW(const void *elem1, const void *elem2)
{
    const Network *nw1 = *reinterpret_cast<const Network **>(const_cast<void *>(elem1));
    const Network *nw2 = *reinterpret_cast<const Network **>(const_cast<void *>(elem2));
    assert(nw1 && typeid(*nw1) == typeid(Network));
    assert(nw2 && typeid(*nw2) == typeid(Network));
    
    const Extension& ext1=nw1->getExtension(); 
    const Extension& ext2=nw2->getExtension(); 
    
    //compare y-coordinate first:
    if(ext1.top<ext2.top)
        return -1;
    if(ext1.top>ext2.top)
        return +1;

    //y-coordinate is equal: now left to right
    if(ext1.left<ext2.left)
        return -1;
    if(ext1.left>ext2.left)
        return +1;

    return 0;
}

CG_STStmt* LDFBDBody::genNetworkCode()
{
    int          i;
    CStmtList    stmtList;
    
    //sort NWs according to there position: 
    qsort(reinterpret_cast<void*>(m_networkList.getElements()),
          m_networkList.getUsed(), sizeof(m_networkList.get(0)), CompareNW);


    //check for overlapping networks
    for (i=0; i<m_networkList.getUsed()-1; i++) 
    {
        Extension intersection;
        const Extension& ext1=m_networkList.get(i+0)->getExtension(); 
        const Extension& ext2=m_networkList.get(i+1)->getExtension(); 

        if(IntersectRect(&intersection, &ext1, &ext2))
            diagnosticMsgRegion(intersection, CG_E_OVERLAPPING_NWS);
    }
    
    //code gen for every NW:
    for (i=0; i<m_networkList.getUsed(); i++) 
    {
        CG_STStmt* pStmt = m_networkList.get(i)->generateNetworkCode(this);
        if(m_pdm->getTotalErrors()>=ST_MAX_NUMBER_ERRORS)
            break;
        if(pStmt)
        {
            CG_STStmt* pNW = m_ppc->getCBody()->newStmt(CG_LDFBDNETWORK, &pStmt->edp);
            pNW->netw.lang = m_pGraphic->isLD() ? CG_STAB_BDY_IS_LD : CG_STAB_BDY_IS_FBD;
            pNW->netw.pStmts = pStmt;
            stmtList.append(pNW);

            WarnMultibleWriteInNW(m_ppc->getDM(), pStmt);
        }//else NW could be empty (only lines!) or in error

        m_ppc->getBodyTMP()->freeAllTmpVars();
    }

    return stmtList.getFirst();
}
//display error & warnings at different locations:
void LDFBDBody::diagnosticMsgL(const LineElement* pLine, HRESULT err_id)
{
    CG_Edipos   edp;

    InitLineEdp(&edp, m_pSym->getCGSym()->edp.pszFile,
                pLine->beg.x, pLine->beg.y, 
                pLine->end.x, pLine->end.y);

    m_pdm->msg0(err_id, &edp);
}
void LDFBDBody::diagnosticMsgRegion(const Extension& ext, HRESULT err_id)
{
    CG_Edipos   edp;

    edp.pszFile = m_ppc->getEDPSrcFile();
    edp.typ = CG_EDP_4GR_REGION;
    edp.gr.x = ext.left;
    edp.gr.y = ext.top;
    edp.gr.l = ext.right - ext.left;
    edp.gr.c = ext.bottom - ext.top;

    m_pdm->msg0(err_id, &edp);
}









//Entry point:
CG_STStmt* ST_LDFBD_To_CBody(
    st_ParseContext*      pParse,     //context
    SFC_LDFBD_SignalHash& grElements, //full 4gr body also if SFC,FBD,LD
    long                  codeGenOpt  //CG_LOADBDY_* bits
)
{
    bool                 bRet;
    CG_STStmt*           pFirst = NULL;


    if(grElements.isSFC())
    {
        // sfc 
        CG_Edipos         ediPos;
        ST_Expression_C*  pExpr;
        CG_STStmt*        pCGStmt;
        const TCHAR*      pszSTTA;//ST trans/action
        LDFBD_SignalHash* p4GRTA; //graphics trans/action elem list

        pParse->addSFC();

        grElements.sortSFCElements();
        
        grElements.extractActionNets();
        if (pParse->getErrorsInFile()!=0)
            return false;

        grElements.calcSFCConnections();
        grElements.extractTransitionNets();
        if (pParse->getErrorsInFile()!=0)
            return false;

        grElements.doSteps();
        if (pParse->getErrorsInFile()!=0)
            return false;
        
        grElements.checkStepPredSucc();
        if (pParse->getErrorsInFile()!=0)
            return false;
        
        grElements.announceActionNames();
        // compile transitions
        int transAnz = grElements.getTransAnz();
        for (int tNr=0; tNr<transAnz; tNr++)
        {
            grElements.doTransitionStart(tNr);
            grElements.getTransEdiPos(tNr, &ediPos);

            pCGStmt = NULL;        
            if (grElements.isTransExpr(tNr))
            {//ST
                pszSTTA = grElements.getTransExpr(tNr);
                if(grElements.getTransNet(tNr) != NULL)
                    pParse->getDM()->msg0(CG_E_SFC_TRANS_EXPR_NET, &ediPos);

                pExpr = pParse->parse4GRBox(&ediPos, pszSTTA, false); 
                if(pExpr)
                    pCGStmt = pParse->newTransAssignStmt(&ediPos, pExpr);
            }
            else
            {//LD/FBD:
                p4GRTA = grElements.getTransNet(tNr);
                if (p4GRTA)
                {
                    LDFBDBody theLDFBDBody(p4GRTA, pParse, codeGenOpt);
                    bRet = theLDFBDBody.createNetworkElements();
                    if(bRet)
                    {
                        theLDFBDBody.createNetworkList();
                        pCGStmt = theLDFBDBody.genNetworkCode();
                    }
                }
                else 
                {
                    pParse->getDM()->msg0(CG_E_SFC_TRANS_NO_EXPR_NET, &ediPos);
                }
            }
            assert(pCGStmt || pParse->getErrorsInFile());
            if(pCGStmt)
                pParse->m_pSFC->sfcEndTransition(&ediPos, pCGStmt);
        }

        if (pParse->getErrorsInFile()!=0)
            return false;

        // compile actions
        int actAnz = grElements.getAnzActions();
        for (int aNr=0; aNr<actAnz; aNr++)
        {
            grElements.getActionEdiPos(aNr, &ediPos);
            pParse->m_pSFC->sfcStartAction();
            
            pCGStmt = NULL;
            if (grElements.isSTAction(aNr))
            {//ST:
                pszSTTA = grElements.getSTAction(aNr);
                pCGStmt = pParse->parseSTAction(&ediPos, pszSTTA, false);
            }
            else
            {//LD/FBD:
                p4GRTA = grElements.getActionNet(aNr);
                LDFBDBody theLDFBDBody(p4GRTA, pParse, codeGenOpt);
                bRet = theLDFBDBody.createNetworkElements();
                if(bRet)
                {
                    theLDFBDBody.createNetworkList();
                    pCGStmt = theLDFBDBody.genNetworkCode();
                }
            }
            //assert(pCGStmt || pParse->getErrorsInFile()); can be NULL if LD/FBD was empty!            
            pParse->m_pSFC->sfcEndAction(&ediPos, pCGStmt, grElements.getActionName(aNr), &ediPos);
            
        }

        if (pParse->getErrorsInFile()!=0)
            return false;

        grElements.testUnusedElements();
    }
    else
    {
        LDFBD_SignalHash      LDFBDElements;
        bRet = grElements.convertToLDFBD(&LDFBDElements);
        assert(bRet);//cannot fail...
        LDFBDBody theLDFBDBody(&LDFBDElements, pParse, codeGenOpt);
    
        //header as 0 errors and XML of Body could be 
        //loaded too, now check the graphics:
        bRet = theLDFBDBody.createNetworkElements();
        if(bRet)
        {
            theLDFBDBody.createNetworkList();
            pFirst = theLDFBDBody.genNetworkCode();
            pParse->endOfSTBody(pFirst);
        }
    }


    return pFirst;
}









