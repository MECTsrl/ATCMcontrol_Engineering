
#ifndef __HASH4GR_H_
#define __HASH4GR_H_


//----  Aggregate Includes:   --------------------------------------*
#include "helper.h"
#include "typeset.h"
//----  Forward Class Definitions:   -------------------------------*
class st_ParseContext;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*






//structures to describe the graphic 
//elements at a low level
typedef RECT  Extension;
typedef POINT Position;


//LINE_FLAG_xxx are used to avoid that every line gets a error/warning
//only the whole signal should produce a diagnostic
const long LINE_FLAG_REACHED        = 0x1;//line is reached from at least one net work element
const long LINE_FLAG_UNCONNECT_WARN = 0x2;//warning 'line unconnected' submitted for this line
const long LINE_FLAG_RUBBERLINE     = 0x4;//warning 'still  rubber' submitted for this line
const long LINE_FLAG_SIGNAL_ERROR   = 0x8;//error 'signal connects more than one output' submitted for this line
const long LINE_FLAG_POWERRAIL      = 0x10;//HINT: this line is a powerrail
const long LINE_FLAG_VERTICAL       = 0x20;//vertical line: WIRED_OR in LADDER

struct LineElement
{
    Position  beg;  //absolute coordinates of line begin
    Position  end;  //absolute coordinates of line end
    Extension ext;  //smallest rect that sourrounds the line
    long      flags;//used for code gen: Note: The lowest bit LINE_FLAG_REACHED
                    //is set in LDFBD_SignalHash::getSignal for every line reached.
                    //The lowest bit may be used like: 
                    //if((flags & LINE_FLAG_REACHED) == 0)
                    //  error(line never reached);
};



//PIN_TYPE bitset:
const long PIN_TYPE_UNDEF       = 0x0000L;
const long PIN_TYPE_IN          = 0x0001L;
const long PIN_TYPE_OUT         = 0x0002L;
const long PIN_TYPE_INOUT       = 0x0004L;
const long PIN_TYPE_INOUT_IN    = 0x0008L;
const long PIN_TYPE_INOUT_OUT   = 0x0010L;
const long PIN_TYPE_INEXT       = 0x0020L;
const long PIN_TYPE_OUTEXT      = 0x0040L;
const long PIN_TYPE_EN          = 0x0080L;
const long PIN_TYPE_ENO         = 0x0100L;
const long PIN_TYPE_WIREDOR_IN  = 0x0200L;
const long PIN_TYPE_WIREDOR_OUT = 0x0400L;
//PIN_FLAGS bitset:
const long PIN_FLAG_R_EDGE   = 0x0001L;
const long PIN_FLAG_F_EDGE   = 0x0002L;
const long PIN_FLAG_NEGATE   = 0x0004L;


const long ANY_IN_PIN   = PIN_TYPE_IN|PIN_TYPE_INOUT_IN|PIN_TYPE_INEXT|PIN_TYPE_EN|PIN_TYPE_WIREDOR_IN;
const long ANY_OUT_PIN  = PIN_TYPE_OUT|PIN_TYPE_INOUT_OUT|PIN_TYPE_OUTEXT|PIN_TYPE_ENO|PIN_TYPE_WIREDOR_OUT;

//use this macro to give the generalized
//pin coords to the getSignalXXX function(s)
#define GET_PIN_COORDS(p)      (p)->pos, (p)->lPinType


class Value;
struct PinElement
{
    int           lPinType;       //in,out,inout,...
    int           lPinFlags;      //r,f_edge,..
    FC_CStr       szName;         //name of input/output may be "" if not named 
                                  //e.g. if pin of extended function or a var boxes
    FC_CStr       szType;         //data type name ("BOOL", "myFB",.)wenn the box was created
    Position      pos;            //absolute coordinates of pin 
    
    
    //for code gen only:
    Value*              pValue;         //tmp var for this in/output
    ST_TypeSet_C        HACKformalParamType;//for code gen only, will be initialized to the 
    const ST_TypeSet_C* pFormalParamType; //H ACK
                                        //formal paramter
                                        //type of a fun/FB in/output or if varbox to the 
                                        //type of the expression
    int nrSFCCon;                       //number of pin in array, to find sfcConElem
};
inline void InitPinElement(PinElement* pPin)
{
    pPin->lPinType = 0;
    pPin->lPinFlags = 0;
    pPin->pos.x = 0;
    pPin->pos.y = 0;
    pPin->pValue = NULL;
    pPin->pFormalParamType = &pPin->HACKformalParamType; 
    pPin->nrSFCCon = -1;
}


enum BOX_TYPE
{
    BOX_TYPE_VARREAD,
    BOX_TYPE_VARWRITE,
    BOX_TYPE_VARFEEDBACK,
    BOX_TYPE_CONNECTOR,
    BOX_TYPE_CONTINUATION,
    BOX_TYPE_POWERRAIL,
    BOX_TYPE_COIL,
    BOX_TYPE_CONTACT,
    BOX_TYPE_WIREDOR,
    BOX_TYPE_FB,
    BOX_TYPE_FNC,
    BOX_TYPE_LABEL,
    BOX_TYPE_JUMP,
    BOX_TYPE_RETURN,
    BOX_TYPE_SFC_RETURN,
    BOX_TYPE_UNDEF, 
};

const long BOX_FLAG_NORMAL   = 0x0000L;
const long BOX_FLAG_R_EDGE   = 0x0001L;
const long BOX_FLAG_F_EDGE   = 0x0002L;
const long BOX_FLAG_NEGATE   = 0x0004L;
const long BOX_FLAG_SET      = 0x0008L;
const long BOX_FLAG_RESET    = 0x0010L;

struct BoxElement
{
    BOX_TYPE    boxType;  //enum 'what kind of box'
    int         boxFlags; //flags for COIL/CONTACT otherwise unused
    Extension   ext;      //smallest rect that sourrounds the box
                          //including all pins of the box
                          //note 1:"ext.left;ext.top" is the origin attribute of the box
                          //        ext.right = ext.left + width
                          //        ext.bottom = ext.top + height
                          //note 2: ext.ul is used to determine the evaluation order
    FC_CStr     text;     //function name or type name of FB or expression if varin/out
    FC_CStr     instName; //instance name of FB or "" if box is not a FBbox
    
    FC_CGArray<PinElement*> pins;
    bool sfcError;
};


struct ConnPairElement
{
    BOX_TYPE    boxType;  //enum 'what kind of box' continuation or connector
    FC_CStr     text;     //name of connector
    Position    origin;   //absolute coordinates of connector box (upper left corner)
    Position    pinpos;   //absolute coordinates of pin 
    bool        used;
};

//some usefull functions:
inline bool POS_EQ(Position a, Position b)
{
    return a.x==b.x && a.y==b.y;
}
inline void GetExtensionFrom2Pos(Position pos1, Position pos2, Extension* pExt)
{
    assert(pExt);
    pExt->left = min(pos1.x, pos2.x);
    pExt->top = min(pos1.y, pos2.y);
    pExt->right = max(pos1.x, pos2.x);
    pExt->bottom = max(pos1.y, pos2.y);
}
inline void MaxExtension(Extension* pExt1, const Extension& ext2)
{
    assert(pExt1);
    assert(pExt1->left<=pExt1->right && pExt1->top<=pExt1->bottom);
    assert(ext2.left<=ext2.right && ext2.top<=ext2.bottom);
    
    if(ext2.left<pExt1->left) pExt1->left=ext2.left; 
    if(ext2.top<pExt1->top) pExt1->top=ext2.top; 
    
    if(ext2.right>pExt1->right) pExt1->right=ext2.right; 
    if(ext2.bottom>pExt1->bottom) pExt1->bottom=ext2.bottom; 
}



//store a pair of connectors:
struct CONNECTOR_PAIR
{
    ConnPairElement* pConnector;    //    NULL       or  a Connetor
    ConnPairElement* pContinuation; //a Continuation or     NULL
    bool             bDuplyDefined; //set true by LDFBD_SignalHash::addConnectorElement 
                                    //if either a connector or a continuation 
                                    //of same name exists
};


//form a graphics edipos:
//pszFile should be a global registered string if edp is used longer 
//or passed to other function.
inline void InitBoxEdp(CG_Edipos* pEdp, const TCHAR* pszFile, int x, int y)
{
    pEdp->pszFile = pszFile;
    pEdp->typ     = CG_EDP_4GR_BOX;
    pEdp->gr.x    = x;
    pEdp->gr.y    = y;
    pEdp->gr.l    = 0;
    pEdp->gr.c    = 0;
}
inline void InitSignalEdp(CG_Edipos* pEdp, const TCHAR* pszFile, int x, int y)
{
    pEdp->pszFile = pszFile;
    pEdp->typ     = CG_EDP_4GR_SIGNAL;
    pEdp->gr.x    = x;
    pEdp->gr.y    = y;
    pEdp->gr.l    = 0;
    pEdp->gr.c    = 0;
}
inline void InitLineEdp(CG_Edipos* pEdp, const TCHAR* pszFile, int x1, int y1, int x2, int y2)
{
    pEdp->pszFile = pszFile;
    pEdp->typ     = CG_EDP_4GR_LINE;
    pEdp->gr.x    = x1;
    pEdp->gr.y    = y1;
    pEdp->gr.l    = x2;
    pEdp->gr.c    = y2;
}







//LDFBD_SignalHash is used to store graphic elements 
//and to find sets of connected elements 
class LDFBD_SignalHash  /* final*/
{
private://types & data:
    enum CONNECTOR_TYP
    {
        CON_TYP_LINE,
        CON_TYP_PIN,
    };
    struct Connector
    {
        CONNECTOR_TYP conType;
        union
        {
            PinElement*  pPin;
            LineElement* pLine;
        };
    };
    //An array of Connectors is present at every
    //Position (x,y) where at least a line or a box is located:
    //The default length is 2, only rarely we need more.
    typedef FC_CGArray<Connector>  ConnectorArray;
    

    //Pointers to an 'array of ConnectorsAtPos' are 
    //the elements of hash array 'm_xyHashArray'.
    //If the hash index made from (x,y) coordinates
    //is not unique search the array 
    //'array of ConnectorsAtPos' for the correct 
    //Position 'pos'.
    struct ConnectorsAtPos 
    {
        Position        pos;
        ConnectorArray  con;
    };
    typedef FC_CGArray<ConnectorsAtPos*> HashArrayType;
    
    //The hash array:
    enum{HASH_ARRAY_SIZE=1024};
    HashArrayType* m_xyHashArray[HASH_ARRAY_SIZE];

    
private://methods:
    int getHashIndex(Position pos)
    {
        int i = (pos.x + pos.y) % HASH_ARRAY_SIZE;
        return  i>=0 ? i : -i;
    }
    ConnectorArray* getConnectorArrayInPos(Position pos, bool bInsert);

    bool  m_isLD;


public://data:
    //arrays to hold pointers to pins, lines, boxes & connpairs
    FC_CGArray<PinElement*>    thePins;
    FC_CGArray<BoxElement*>    theBoxes;
    FC_CGArray<LineElement*>   theLines;
    FC_CGArray<CONNECTOR_PAIR> theConnPairs;

    //for convenience all label names in the pou:
    FC_CStrList                labelNames;

public://methods:
	LDFBD_SignalHash();
	~LDFBD_SignalHash();

    void addBoxElement(BoxElement* pBox);
    void addPinElement(PinElement* pPin);
    void addLineElement(LineElement* pLine);
    void addConnectorElement(ConnPairElement* pConnPair);

    bool isLD()const {return m_isLD;}

    //get all connected lines and pins starting at 'start'
    //return: 0=ok, 1=signal cyclic, 2=unconnected line ends, 
    //3=cyclic &  unconnected line ends
    int getSignal(Position                  start,
                  long                      pinType,
                  FC_CGArray<LineElement*>* pReachedLines,
                  FC_CGArray<PinElement*>*  pReachedPins,
                  Extension*                pMaxSignalExtension);

    //LD ONLY: get the wired or at position 'start'
    //return like above
    int getWiredOr(
        Position                  start,
        FC_CGArray<Position>*     pInputPositions,
        FC_CGArray<Position>*     pOutputPositions,
        FC_CGArray<LineElement*>* pReachedLines,
        Extension*                pMaxSignalExtension
    );

    //LD ONLY: check what is connected at the powerrail
    //return 0=nothing, 1=connected to the right 2=connected to the left 3=both
    int chkPwrRail(Position               start);

private:
    int getFBDstyleSignal(Position                 start,
                  long                      pinType,
                  FC_CGArray<LineElement*>* pReachedLines,
                  FC_CGArray<PinElement*>*  pReachedPins,
                  Extension*                pMaxSignalExtension);

    int getLDstyleSignal(Position                 start,
                  long                      pinType,
                  FC_CGArray<LineElement*>* pReachedLines,
                  FC_CGArray<PinElement*>*  pReachedPins,
                  Extension*                pMaxSignalExtension);
};


// elements for the sfc

struct SFCStepElement
{
    bool        initStep;
    
    BoxElement  box;
    bool used;
};

struct SFCActionElement
{
    BoxElement  box;
    bool        isSTAction;
    TCHAR*      ppszSTAction;
};

struct SFCTransitionElement
{
    int priority;
    BoxElement  box;
};

struct SFCActionBlockElement
{
    FC_CStr     qualType;
    FC_CStr     qualTime;

    BoxElement  box;
    bool        used;
};

enum SFC_CONDIV_TYPE
{
    SFC_ALT_CONDIV,
    SFC_SIM_CONDIV,
    SFC_NONE_CONDIV
};

struct SFCConDivElement
{
    SFC_CONDIV_TYPE type;

    Position  beg;  //absolute coordinates of line begin
    Position  end;  //absolute coordinates of line end
    Extension   ext;

    bool used;
    bool sfcError;
};

enum SFC_ELEM_TYPE
{ 
    SFC_BOX, SFC_LINE, SFC_STEP, SFC_TRANS, SFC_ACTBLK, SFC_NO
};

struct SFCElem
{
    SFC_ELEM_TYPE type;
    int           elemNr;
};

//SFC_LDFBD_SignalHash is used to store graphic elements 
//and to find sets of connected elements 
class SFC_LDFBD_SignalHash  /* final*/
{
private://types & data:
    enum CONNECTOR_TYP
    {
        CON_TYP_LINE,
        CON_TYP_PIN,
    };
    struct Connector
    {
        CONNECTOR_TYP conType;
        union
        {
            PinElement*  pPin;
            LineElement* pLine;
        };
    };
    //An array of Connectors is present at every
    //Position (x,y) where at least a line or a box is located:
    //The default length is 2, only rarely we need more.
    typedef FC_CGArray<Connector>  ConnectorArray;
    

    //Pointers to an 'array of ConnectorsAtPos' are 
    //the elements of hash array 'm_xyHashArray'.
    //If the hash index made from (x,y) coordinates
    //is not unique search the array 
    //'array of ConnectorsAtPos' for the correct 
    //Position 'pos'.
    struct ConnectorsAtPos 
    {
        Position        pos;
        ConnectorArray  con;
    };
    typedef FC_CGArray<ConnectorsAtPos*> HashArrayType;
    
    //The hash array:
    enum{HASH_ARRAY_SIZE=1024};
    HashArrayType* m_xyHashArray[HASH_ARRAY_SIZE];

    bool             m_isSFC;
    st_ParseContext* pBodyCon;
    CGT_CDiagMsg*    m_pdm;

private://methods:
    int getHashIndex(Position pos)
    {
        int i = (pos.x + pos.y) % HASH_ARRAY_SIZE;
        return  i>=0 ? i : -i;
    }
    ConnectorArray* getConnectorArrayInPos(Position pos, bool bInsert);
    


public://data:
    //arrays to hold pointers to pins, lines, boxes & connpairs
    FC_CGArray<PinElement*>            thePins;
    FC_CGArray<BoxElement*>            theBoxes;
    FC_CGArray<LineElement*>           theLines;
    FC_CGArray<ConnPairElement*>       theConnectors;
    FC_CGArray<SFCStepElement*>        theSteps;
    FC_CGArray<SFCTransitionElement*>  theTransitions;
    FC_CGArray<SFCActionElement*>      theActions;
    FC_CGArray<SFCActionBlockElement*> theActionBlocks;
    FC_CGArray<SFCConDivElement*>      theConDivs;

    FC_CGArray<SFCElem>                theSFCCons;

    FC_CStrList                        theActionNames;
    FC_CGArray<LDFBD_SignalHash*>      theActionNets;

    FC_CGArray<LDFBD_SignalHash*>      theTransNets;
    FC_CStrList                        theTransExpr;
    FC_CGArray<bool>                   transIsExpr;

    //for convenience all label names in the pou:
    FC_CStrList                        labelNames;

public://methods:
	SFC_LDFBD_SignalHash(
        st_ParseContext* ppc   //context.
    );
	~SFC_LDFBD_SignalHash();

    //various helpers for diag messages:
    const CG_Edipos* boxEdp(CG_Edipos* pEdp, const Position& pos);
    const CG_Edipos* boxEdp(CG_Edipos* pEdp, const Extension& ext);
    void boxDiag0(HRESULT errId, int x, int y);
    void boxDiag0(HRESULT errId, const Position& pos);
    void boxDiag0(HRESULT errId, const Extension& ext);
    void boxDiag1(HRESULT errId, int x, int y, const TCHAR* psz1);
    void boxDiag1(HRESULT errId, const Position& pos, const TCHAR* psz1);
    void boxDiag1(HRESULT errId, const Extension& ext, const TCHAR* psz1);
    void signalDiag0(HRESULT errId, const Position& pos);

    void addBoxElement(BoxElement* pBox);
    void addPinElement(PinElement* pPin);
    void addLineElement(LineElement* pLine);
    void addConnectorElement(ConnPairElement* pConnPair);
    void addStepElement(SFCStepElement* pStep);
    void addTransitionElement(SFCTransitionElement* pTransition);
    void addActionElement(SFCActionElement* pAction);
    void addActionBlockElement(SFCActionBlockElement* pActionBlock);
    void addConDivElement(SFCConDivElement* pConDiv);
    void testUnusedElements();

    void setSFC() {m_isSFC = true;}
    bool isSFC() const{return m_isSFC;}


    enum PINPOS {TOP,BOTTOM,LEFT,RIGHT};

    //get all connected lines and pins starting at 'start'
    //return: 0=ok, 1=signal cyclic, 2=unconnected line ends, 
    //3=cyclic &  unconnected line ends
    int getSFCSignal(
        Position                       start, 
        FC_CGArray<LineElement*>*      pReachedLines,
        FC_CGArray<PinElement*>*       pReachedPins,
        FC_CGArray<SFCConDivElement*>* pReachedConDivs,
        FC_CGArray<PINPOS>*            pConDivPinPos,
        Extension*                     pMaxSignalExtension,
        bool*                          pStartOnConDiv
    );
    void getSFCSignalAndWarn(
        Position                       start, 
        FC_CGArray<LineElement*>*      pReachedLines,
        FC_CGArray<PinElement*>*       pReachedPins,
        FC_CGArray<SFCConDivElement*>* pReachedConDivs,
        FC_CGArray<PINPOS>*            pConDivPinPos,
        Extension*                     pMaxSignalExtension,
        bool*                          pStartOnConDiv
    );

    bool setSFCSignalFlag(Position start, long lFlag);

    // check if there have been sfc elements and if not transfer
    // all elements to LDFBD_SignalHash to continue like before
    bool convertToLDFBD(LDFBD_SignalHash* pGraphicElements);
    
    enum SFC_SORT_TYPE {SFCS_STEP, SFCS_TRANS, SFCS_ACTION, SFCS_ACTBLK, SFCS_CONDIV};
    Position getSFCPosition(SFC_SORT_TYPE type, int nr);
    void swapSFCElements(SFC_SORT_TYPE type, int p1, int p2);
    void sortArray(SFC_SORT_TYPE type, int start, int end);
    void sortSFCElements();
    // find all actions, get all elements inside the rect and construct a
    // list of LDFBD_SignalHashs out of these elements
    void extractActionNets();
    void announceActionNames();
    const TCHAR* getActionName(int nr);

    // all pins have to know the element they are connected to
    void calcPinConnections(
        FC_CGArray<PinElement*>* pins, 
        SFC_ELEM_TYPE type, int nr
    );
    // calc connection pin - box
    void calcSFCConnections();
    // look at all steps, add names, find and add action blocks 
    void doSteps();
    // extract transition expression, ST-expression or FBD net 
    // store in a list
    void extractTransitionNets();
    // fill the transition structure of a sfc
    void doTransitionStart(int tNr);
    void doTransitionEnd(int tNr);
    int  getTransAnz() {return theTransitions.getUsed();}
    bool isTransExpr(int tNr) {return transIsExpr.get(tNr);}
    const TCHAR* getTransExpr(int tNr) {return theTransExpr.get(tNr);}
    LDFBD_SignalHash* getTransNet(int tNr) {return theTransNets.get(tNr);}
    void getTransEdiPos(int tNr, CG_Edipos* ediPos);


    int  getAnzActions() {return theActionNets.getUsed();}
    LDFBD_SignalHash* getActionNet(int nr) {return theActionNets.get(nr);}
    void getActionEdiPos(int aNr, CG_Edipos* ediPos);
    bool isSTAction(int aNr) {return theActions.get(aNr)->isSTAction;}
    TCHAR* getSTAction(int aNr) {return theActions.get(aNr)->ppszSTAction;}

    bool followTransLDFBDSignal(int transNr);
    bool followActBlkSignal(
        int stepNr,
        FC_CGArray<int>* pActBlkNrs
    );
    bool checkStepPredSucc();
    bool getConDivPredSucc(
        SFCConDivElement* pConDiv,
        FC_CGArray<int>*  pPredElemNrs,
        SFC_ELEM_TYPE*    pPredElemType,
        FC_CGArray<int>*  pSuccElemNrs,
        SFC_ELEM_TYPE*    pSuccElemType
    );
    bool getTransPred(
        int              transNr,
        FC_CGArray<int>* pPredStepNrs,
        SFC_CONDIV_TYPE* pPredType
    );
    bool getTransSucc(
        int transNr,
        FC_CGArray<int>* pSuccStepNrs,
        SFC_CONDIV_TYPE* pSuccType
    );

    bool        getPinPos(PINPOS pinpos, BoxElement* pBox, Position *ppos);
    PINPOS      isWhichPin(const BoxElement* pBox, const Position *pPos);
    BoxElement* getBoxOfSFCElementPin(const PinElement *pPin);   
    bool        checkTransNetElement(const BoxElement* pBox);
};




 
#endif // __HASH4GR_H_

