


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



#include <malloc.h>
#include <tchar.h>
#include "Hash4GR.h"
#include "expr.h"
#include "st_parseContext.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LDFBD_SignalHash::LDFBD_SignalHash()
{
    memset(m_xyHashArray, 0, sizeof(m_xyHashArray));
    m_isLD = false;
}

LDFBD_SignalHash::~LDFBD_SignalHash()
{
    int i;
    HashArrayType* pHash;

    //delete hash array content:
    for(i=0;i<HASH_ARRAY_SIZE;i++)
    {
        pHash = m_xyHashArray[i];
        if(pHash)
        {
            for(int j=pHash->getUsed()-1;j>=0;j--)
                delete(pHash->get(j));

            delete(pHash);
        }
    }

    //delete pins, lines, boxes & connpairs
    for(i=thePins.getUsed()-1;i>=0;i--)
        delete(thePins.get(i));
    
    for(i=theBoxes.getUsed()-1;i>=0;i--)
        delete(theBoxes.get(i));
    
    for(i=theLines.getUsed()-1;i>=0;i--)
        delete(theLines.get(i));

    for(i=theConnPairs.getUsed()-1;i>=0;i--)
    {
        const CONNECTOR_PAIR& pair = theConnPairs.get(i);
        delete(pair.pConnector);
        delete(pair.pContinuation);
    }
}

void LDFBD_SignalHash::addBoxElement(BoxElement* pBox)
{
    assert(pBox->boxType!=BOX_TYPE_CONNECTOR && pBox->boxType!=BOX_TYPE_CONTINUATION);

    if(pBox->boxType==BOX_TYPE_LABEL)
        labelNames.add(pBox->text);
    else if(pBox->boxType==BOX_TYPE_COIL
         || pBox->boxType==BOX_TYPE_CONTACT)
        m_isLD = true;

    theBoxes.add(pBox);
}
void LDFBD_SignalHash::addPinElement(PinElement* pPin)
{
    Connector        con;
    ConnectorArray*  pConArray;

    assert(pPin && pPin->lPinType != PIN_TYPE_UNDEF);
    thePins.add(pPin);

    con.conType = CON_TYP_PIN;
    con.pPin  = pPin;

    pConArray = getConnectorArrayInPos(pPin->pos, true);
    assert(pConArray);
    if(pConArray)
        pConArray->add(con);
}
void LDFBD_SignalHash::addLineElement(LineElement* pLine)
{
    Connector        con;
    ConnectorArray*  pConArray;


    assert(pLine);
    theLines.add(pLine);

    con.conType = CON_TYP_LINE;
    con.pLine = pLine;

    if (pLine->flags & LINE_FLAG_POWERRAIL)
        m_isLD = true;

    pConArray = getConnectorArrayInPos(pLine->beg, true);
    assert(pConArray);
    if(pConArray)
        pConArray->add(con);

    if (!POS_EQ(pLine->beg, pLine->end))
    {
        pConArray = getConnectorArrayInPos(pLine->end, true);
        assert(pConArray);
        if(pConArray)
            pConArray->add(con);
    }
}
void LDFBD_SignalHash::addConnectorElement(ConnPairElement* pConnPair)
{
    int             n;
    int             nConn = theConnPairs.getUsed();
    const TCHAR*    pszName;
    CONNECTOR_PAIR  conPair;

    assert(pConnPair->boxType==BOX_TYPE_CONTINUATION || pConnPair->boxType==BOX_TYPE_CONNECTOR);

    for(n=0;n<nConn;n++)
    {
        CONNECTOR_PAIR& pair = theConnPairs.get(n);
        if(pair.pConnector)
        {
            pszName = pair.pConnector->text;
        }
        else if(pair.pContinuation)
        {
            pszName = pair.pContinuation->text;
        }
        else
        {
            assert(!"bad connector pair struct");
            return;
        }

        if(ST_IS_NAME_EQUAL(pszName, pConnPair->text))
        {
            //pair or error (duply defined pair name: 
            if(pConnPair->boxType==BOX_TYPE_CONNECTOR && !pair.pConnector)
            {
                pair.pConnector = pConnPair;
                return;
            }
            if(pConnPair->boxType==BOX_TYPE_CONTINUATION && !pair.pContinuation)
            {
                pair.pContinuation = pConnPair;
                return;
            }

            //remember error case
            pair.bDuplyDefined = true;
            delete(pConnPair);
            return;
        }
    }


    //init connpair struct and append:
    if(pConnPair->boxType==BOX_TYPE_CONTINUATION)
    {
        conPair.pConnector = NULL;
        conPair.pContinuation = pConnPair;
    }
    else
    {
        conPair.pConnector = pConnPair;
        conPair.pContinuation = NULL;
    }
    conPair.bDuplyDefined = false;

    theConnPairs.add(conPair);    
}


LDFBD_SignalHash::ConnectorArray* 
LDFBD_SignalHash::getConnectorArrayInPos(Position pos, bool bInsert)
{
    int               i;
    HashArrayType*    pHash;
    ConnectorsAtPos*  pConnArrayAtPos;


    i = getHashIndex(pos);
    pHash = m_xyHashArray[i];
    if(!pHash)
    {
        if(bInsert)
        {
            pHash = new HashArrayType;
            m_xyHashArray[i] = pHash;
        }
        else
        {
            return NULL;
        }
    }

    //search for correct pos:
    for(i=pHash->getUsed()-1;i>=0;i--)
    {
        pConnArrayAtPos = pHash->get(i);
        if(POS_EQ(pConnArrayAtPos->pos, pos))
            return &(pConnArrayAtPos->con);
    }

    if(bInsert)
    {
        //insert an empty connector array:
        pConnArrayAtPos = new ConnectorsAtPos;
        pConnArrayAtPos->pos = pos;

        pHash->add(pConnArrayAtPos);
        return &pConnArrayAtPos->con;
    }

    return NULL;
}




static int GetIndexOfPointInPointArray(Position point, const FC_CGArray<Position>* pPointArray)
{
    const Position* pPoints = pPointArray->getCElements();
    for(int i=pPointArray->getUsed()-1;i>=0;i--)
    {
        if(POS_EQ(point, pPoints[i]))
            return i;
    }
    
    return -1;
}

int LDFBD_SignalHash::getSignal(Position              start,
                                long                  pinType,
                                FC_CGArray<LineElement*>* pReachedLines,
                                FC_CGArray<PinElement*>*  pReachedPins,
                                Extension*            pMaxSignalExtension)
{
    if (m_isLD && pinType != PIN_TYPE_UNDEF)
        return getLDstyleSignal(start, pinType, pReachedLines, pReachedPins, pMaxSignalExtension);
    else
        return getFBDstyleSignal(start, pinType, pReachedLines, pReachedPins, pMaxSignalExtension);
}

int LDFBD_SignalHash::getFBDstyleSignal(Position              start,
                                long                  pinType,
                                FC_CGArray<LineElement*>* pReachedLines,
                                FC_CGArray<PinElement*>*  pReachedPins,
                                Extension*            pMaxSignalExtension)
{
    int                     jPointsToDo;
    int                     jConnects;
    int                     idx;
    Connector               con;
    Position                currPos;
    Position                newPoint;
    ConnectorArray*         pConArray;
    FC_CGArray<Position>    pointsToDo;
    int                     result = 0;

    //reset the size of recieving arrays to 0:
    if(pReachedLines)
        pReachedLines->resetUsed();
    if(pReachedPins)
        pReachedPins->resetUsed();


    pointsToDo.add(start);
    for(jPointsToDo=0; jPointsToDo<pointsToDo.getUsed(); jPointsToDo++)
    {
        currPos = pointsToDo.get(jPointsToDo);
        pConArray = getConnectorArrayInPos(currPos, false);
        if(!pConArray)
        {
            assert(jPointsToDo==0);//only the start point may have no connection...
            break;                   //
        }

        int nCon = pConArray->getUsed();
        for(jConnects=0;jConnects<nCon;jConnects++)
        {
            con = pConArray->get(jConnects);

            switch(con.conType)
            {
            case CON_TYP_PIN:
                if(pReachedPins)
                    pReachedPins->add(con.pPin);
            break;
            case CON_TYP_LINE:
                newPoint = con.pLine->beg;

                //append line only if current point is start of line
                //this way lines are appended only once to pReachedLines
                if(POS_EQ(currPos, con.pLine->beg))
                {
                    //mark line as reached, this flag can be used to
                    //warn about unreached lines:
                    con.pLine->flags |= LINE_FLAG_REACHED;

                    if(pReachedLines)
                        pReachedLines->add(con.pLine);

                    if(pMaxSignalExtension)
                        MaxExtension(pMaxSignalExtension, con.pLine->ext);


                    //end of line is new point to investigate:
                    newPoint = con.pLine->end;
                }

                //is the newly reached point new point, already seen or evaluated ?
                idx = GetIndexOfPointInPointArray(newPoint, &pointsToDo);

                if(idx==-1)
                    pointsToDo.add(newPoint);
                else if(idx>jPointsToDo)
                    result |= 1; //<-point reached twice on different ways
                //else: point has already been evaluated....

                if(nCon==1) //<-detect unconnected line ends
                    result |= 2;
            break;
            default:
                assert(!"bad enum");
            }
        }
    }

    return result;
}



/*
* This diagram illustrates which pins
* are connected at a wired or:
*
*
*      ANY_OUT        WIREDOR_IN      |      WIREDOR_OUT         ANY_IN
*      ----->           <-----        |        ----->            <-----
*                                     |
*     \____________  ____________/    |     \____________  ____________/
*                  \/                 |                  \/
*
*           ANY_LEFT_PIN                              ANY_RIGHT_PIN
*/
const long ANY_LEFT_PIN   = PIN_TYPE_OUT|PIN_TYPE_INOUT_OUT|PIN_TYPE_OUTEXT|PIN_TYPE_ENO|PIN_TYPE_WIREDOR_IN;
const long ANY_RIGHT_PIN  = PIN_TYPE_IN|PIN_TYPE_INOUT_IN|PIN_TYPE_INEXT|PIN_TYPE_EN|PIN_TYPE_WIREDOR_OUT;

int LDFBD_SignalHash::getLDstyleSignal(Position              start,
                                long                  pinType,
                                FC_CGArray<LineElement*>* pReachedLines,
                                FC_CGArray<PinElement*>*  pReachedPins,
                                Extension*            pMaxSignalExtension)
{
    bool                    output = (pinType & ANY_OUT_PIN) != 0;
    int                     jPointsToDo;
    int                     jConnects;
    int                     idx;
    Connector               con;
    Position                currPos;
    Position                newPoint;
    ConnectorArray*         pConArray;
    FC_CGArray<Position>    pointsToDo;
    int                     result = 0;

    //reset the size of recieving arrays to 0:
    if(pReachedLines)
        pReachedLines->resetUsed();
    if(pReachedPins)
        pReachedPins->resetUsed();


    pointsToDo.add(start);
    for(jPointsToDo=0; jPointsToDo<pointsToDo.getUsed(); jPointsToDo++)
    {
        currPos = pointsToDo.get(jPointsToDo);
        pConArray = getConnectorArrayInPos(currPos, false);
        if(!pConArray)
        {
            assert(jPointsToDo==0);//only the start point may have no connection...
            break;                   //
        }

        bool bWiredOr = false;
        int nCon = pConArray->getUsed();
        for(jConnects=0;jConnects<nCon;jConnects++)
        {
            con = pConArray->get(jConnects);
            if(con.conType == CON_TYP_LINE &&
               con.pLine->flags & LINE_FLAG_VERTICAL)
                bWiredOr = true;
        }

        for(jConnects=0;jConnects<nCon;jConnects++)
        {
            con = pConArray->get(jConnects);

            switch(con.conType)
            {
            case CON_TYP_PIN:
                if(bWiredOr && jPointsToDo==0)
                    if(pinType & ANY_LEFT_PIN  && con.pPin->lPinType & ANY_RIGHT_PIN
                    || pinType & ANY_RIGHT_PIN && con.pPin->lPinType & ANY_LEFT_PIN)
                        continue;

                if(bWiredOr && jPointsToDo!=0)
                    if( output && con.pPin->lPinType & ANY_RIGHT_PIN
                    || !output && con.pPin->lPinType & ANY_LEFT_PIN)
                        continue;

                if(pReachedPins)
                    pReachedPins->add(con.pPin);
            break;
            case CON_TYP_LINE:
                if(con.pLine->flags & LINE_FLAG_VERTICAL)
                    continue;

                if(bWiredOr)
                  if(jPointsToDo!=0 ||
                    (pinType & (PIN_TYPE_WIREDOR_IN|PIN_TYPE_WIREDOR_OUT)) == 0)
                    continue;

                //follow lines only in the direction of the dataflow
                newPoint = output ? con.pLine->end : con.pLine->beg;

                if(!POS_EQ(currPos, newPoint))
                {
                    //mark line as reached, this flag can be used to
                    //warn about unreached lines:
                    con.pLine->flags |= LINE_FLAG_REACHED;

                    if(pReachedLines)
                        pReachedLines->add(con.pLine);

                    if(pMaxSignalExtension)
                        MaxExtension(pMaxSignalExtension, con.pLine->ext);


                    //is the newly reached point new point, already seen or evaluated ?
                    idx = GetIndexOfPointInPointArray(newPoint, &pointsToDo);

                    if(idx==-1)
                        pointsToDo.add(newPoint);
                    else
                        result |= 1; //<-point reached twice on different ways
                    //else: point has already been evaluated....
                }

                if(nCon==1) //<-detect unconnected line ends
                    result |= 2;
            break;
            default:
                assert(!"bad enum");
            }
        }
    }

    return result;
}


int LDFBD_SignalHash::getWiredOr(
    Position                  start,
    FC_CGArray<Position>*     pInputPositions,
    FC_CGArray<Position>*     pOutputPositions,
    FC_CGArray<LineElement*>* pReachedLines,
    Extension*                pMaxSignalExtension
)
{
    int                     jPointsToDo;
    int                     jConnects;
    int                     idx;
    Connector               con;
    Position                currPos;
    Position                newPoint;
    ConnectorArray*         pConArray;
    FC_CGArray<Position>    pointsToDo;
    int                     result = 0;

    assert(m_isLD);

    //reset the size of recieving arrays to 0:
    if(pInputPositions)
        pInputPositions->resetUsed();
    if(pOutputPositions)
        pOutputPositions->resetUsed();
    if(pReachedLines)
        pReachedLines->resetUsed();

    pointsToDo.add(start);
    for(jPointsToDo=0; jPointsToDo<pointsToDo.getUsed(); jPointsToDo++)
    {
        currPos = pointsToDo.get(jPointsToDo);
        pConArray = getConnectorArrayInPos(currPos, false);
        if(!pConArray)
        {
            assert(jPointsToDo==0);//only the start point may have no connection...
            break;                   //
        }

        bool bLeftConn = false;
        bool bRightConn = false;
        int nCon = pConArray->getUsed();

        for(jConnects=0;jConnects<nCon;jConnects++)
        {
            con = pConArray->get(jConnects);

            switch(con.conType)
            {
            case CON_TYP_PIN:
                if(con.pPin->lPinType & ANY_RIGHT_PIN)
                    bRightConn = true;
                else
                    bLeftConn = true;
            break;
            case CON_TYP_LINE:
                if(con.pLine->flags & LINE_FLAG_VERTICAL) {
                    newPoint = con.pLine->beg;

                    //append line only if current point is start of line
                    //this way lines are appended only once to pReachedLines
                    if(POS_EQ(currPos, con.pLine->beg))
                    {
                        //mark line as reached, this flag can be used to
                        //warn about unreached lines:
                        con.pLine->flags |= LINE_FLAG_REACHED;

                        if(pReachedLines)
                            pReachedLines->add(con.pLine);
                        if(pMaxSignalExtension)
                            MaxExtension(pMaxSignalExtension, con.pLine->ext);


                        //end of line is new point to investigate:
                        newPoint = con.pLine->end;
                    }

                    //is the newly reached point new point, already seen or evaluated ?
                    idx = GetIndexOfPointInPointArray(newPoint, &pointsToDo);

                    if(idx==-1)
                        pointsToDo.add(newPoint);
                    else if(idx>jPointsToDo)
                        result |= 1; //<-point reached twice on different ways
                    //else: point has already been evaluated....

                    if(nCon==1) //<-detect unconnected line ends
                        result |= 2;
                }
                else {
                    if(POS_EQ(currPos, con.pLine->beg))
                        bRightConn = true;
                    else
                        bLeftConn = true;
                }
            break;
            default:
                assert(!"bad enum");
            }
        }

        if (bLeftConn && pInputPositions)
            pInputPositions->add(currPos);

        if (bRightConn && pOutputPositions)
            pOutputPositions->add(currPos);
            
    }

    return result;
}



int LDFBD_SignalHash::chkPwrRail(Position               start)
{
    ConnectorArray*         pConArray;
    Connector               con;
    int                     jConnects;
    int                     result = 0;

    assert(m_isLD);

    pConArray = getConnectorArrayInPos(start, false);
    assert(pConArray!=NULL);
    if(!pConArray)
    {
        return 0;
    }

    int nCon = pConArray->getUsed();

    for(jConnects=0;jConnects<nCon;jConnects++)
    {
        con = pConArray->get(jConnects);
        switch(con.conType)
        {
        case CON_TYP_PIN:
            if (con.pPin->lPinType & ANY_IN_PIN)
                result |= 1;
            else
                result |= 2;
        break;
        case CON_TYP_LINE:
            if(con.pLine->flags & (LINE_FLAG_VERTICAL|LINE_FLAG_POWERRAIL))
                continue;

            if(POS_EQ(start, con.pLine->beg))
                result |= 1;
            else
                result |= 2;

        break;
        default:
            assert(!"bad enum");
        }
    }


    return result;
}






////////////////////////////////////////////////////////////////////////////////////////////////////////
//                LD SFC FBD
////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SFC_LDFBD_SignalHash::SFC_LDFBD_SignalHash(st_ParseContext* ppc)
{
    memset(m_xyHashArray, 0, sizeof(m_xyHashArray));

    m_isSFC = false;
    pBodyCon = ppc;
    m_pdm = ppc->getDM();
}

SFC_LDFBD_SignalHash::~SFC_LDFBD_SignalHash()
{
    int i;
    HashArrayType* pHash;

    //delete hash array content:
    for(i=0;i<HASH_ARRAY_SIZE;i++)
    {
        pHash = m_xyHashArray[i];
        if(pHash)
        {
            for(int j=pHash->getUsed()-1;j>=0;j--)
                delete(pHash->get(j));

            delete(pHash);
        }
    }

    //delete pins, lines, boxes & connpairs
    for(i=thePins.getUsed()-1;i>=0;i--)
        //if (thePins.get(i))
        //{
            //if (thePins.get(i)->nrSFCCon != -1)
                delete(thePins.get(i));
        //}
    
    for(i=theBoxes.getUsed()-1;i>=0;i--)
        delete(theBoxes.get(i));
    
    for(i=theLines.getUsed()-1;i>=0;i--)
        delete(theLines.get(i));

    for(i=theConnectors.getUsed()-1;i>=0;i--)
    {
        delete(theConnectors.get(i));
    }

    for(i=0; i<theSteps.getUsed(); i++)
    {
        delete(theSteps.get(i));
    }

    for(i=0; i<theTransitions.getUsed(); i++)
    {
        delete(theTransitions.get(i));
    }
    for(i=0; i<theActions.getUsed(); i++)
    {
        if (theActions.get(i))
        {
            if (theActions.get(i)->ppszSTAction)
            {
                delete(theActions.get(i)->ppszSTAction);
            }
        }
        delete(theActions.get(i));
    }
    for(i=0; i<theActionBlocks.getUsed(); i++)
    {
        delete(theActionBlocks.get(i));
    }
    for(i=0; i<theConDivs.getUsed(); i++)
    {
        delete(theConDivs.get(i));
    }
    for(i=0; i<theActionNets.getUsed(); i++)
    {
        delete(theActionNets.get(i));
    }
    for(i=0; i<theTransNets.getUsed(); i++)
    {
        delete(theTransNets.get(i));
    }


}


const CG_Edipos* SFC_LDFBD_SignalHash::boxEdp(CG_Edipos* pEdp, const Position& pos)
{
    InitBoxEdp(pEdp, pBodyCon->getEDPSrcFile(), pos.x, pos.y);
    return pEdp;
}
const CG_Edipos* SFC_LDFBD_SignalHash::boxEdp(CG_Edipos* pEdp, const Extension& ext)
{
    InitBoxEdp(pEdp, pBodyCon->getEDPSrcFile(), ext.left, ext.top);
    return pEdp;
}

void SFC_LDFBD_SignalHash::boxDiag0(HRESULT errId, int x, int y)
{
    CG_Edipos edp;
    InitBoxEdp(&edp, pBodyCon->getEDPSrcFile(), x, y);
    pBodyCon->getDM()->msg0(errId, &edp);
}
void SFC_LDFBD_SignalHash::boxDiag0(HRESULT errId, const Position& pos)
{
    boxDiag0(errId, pos.x, pos.y);
}
void SFC_LDFBD_SignalHash::boxDiag0(HRESULT errId, const Extension& ext)
{
    boxDiag0(errId, ext.left, ext.top);
}
void SFC_LDFBD_SignalHash::boxDiag1(HRESULT errId, int x, int y, const TCHAR* psz1)
{
    CG_Edipos edp;
    InitBoxEdp(&edp, pBodyCon->getEDPSrcFile(), x, y);
    pBodyCon->getDM()->msg1(errId, &edp, psz1);
}
void SFC_LDFBD_SignalHash::boxDiag1(HRESULT errId, const Position& pos, const TCHAR* psz1)
{
    boxDiag1(errId, pos.x, pos.y, psz1);
}
void SFC_LDFBD_SignalHash::boxDiag1(HRESULT errId, const Extension& ext, const TCHAR* psz1)
{
    boxDiag1(errId, ext.left, ext.top, psz1);
}
void SFC_LDFBD_SignalHash::signalDiag0(HRESULT errId, const Position& pos)
{
    CG_Edipos edp;
    InitSignalEdp(&edp, pBodyCon->getEDPSrcFile(), pos.x, pos.y);
    pBodyCon->getDM()->msg0(errId, &edp);
}




void SFC_LDFBD_SignalHash::addBoxElement(BoxElement* pBox)
{
    assert(pBox->boxType!=BOX_TYPE_CONNECTOR && pBox->boxType!=BOX_TYPE_CONTINUATION);

    if(pBox->boxType==BOX_TYPE_LABEL)
        labelNames.add(pBox->text);

    theBoxes.add(pBox);
}
void SFC_LDFBD_SignalHash::addPinElement(PinElement* pPin)
{
    Connector        con;
    ConnectorArray*  pConArray;

    assert(pPin && pPin->lPinType != PIN_TYPE_UNDEF);
    thePins.add(pPin);

    con.conType = CON_TYP_PIN;
    con.pPin  = pPin;

    pConArray = getConnectorArrayInPos(pPin->pos, true);
    assert(pConArray);
    if(pConArray)
        pConArray->add(con);
}
void SFC_LDFBD_SignalHash::addLineElement(LineElement* pLine)
{
    Connector        con;
    ConnectorArray*  pConArray;


    assert(pLine);
    theLines.add(pLine);

    con.conType = CON_TYP_LINE;
    con.pLine = pLine;

    pConArray = getConnectorArrayInPos(pLine->beg, true);
    assert(pConArray);
    if(pConArray)
        pConArray->add(con);

    if(!POS_EQ(pLine->beg, pLine->end))
    {
        pConArray = getConnectorArrayInPos(pLine->end, true);
        assert(pConArray);
        if(pConArray)
            pConArray->add(con);
    }
}
void SFC_LDFBD_SignalHash::addConnectorElement(ConnPairElement* pConnPair)
{
   assert(pConnPair);

   theConnectors.add(pConnPair);

   pConnPair->used = false;
}


void SFC_LDFBD_SignalHash::addStepElement(SFCStepElement* pStep)
{
    assert(pStep);

    theSteps.add(pStep);

    pStep->used = false;
}


void SFC_LDFBD_SignalHash::addTransitionElement(SFCTransitionElement* pTransition)
{
    assert(pTransition);

    theTransitions.add(pTransition);
}


void SFC_LDFBD_SignalHash::addActionElement(SFCActionElement* pAction)
{
    assert(pAction);

    theActions.add(pAction);
}


void SFC_LDFBD_SignalHash::addActionBlockElement(SFCActionBlockElement* pActionBlock)
{
    assert(pActionBlock);

    theActionBlocks.add(pActionBlock);

    pActionBlock->used = false;
}


void SFC_LDFBD_SignalHash::addConDivElement(SFCConDivElement* pConDiv)
{
    assert(pConDiv);

    theConDivs.add(pConDiv);

    pConDiv->used = false;
}





void SFC_LDFBD_SignalHash::testUnusedElements()
{
    int i;

    for (i=0; i<theConDivs.getUsed(); i++)
    {
        if (!theConDivs.get(i)->used
            && !theConDivs.get(i)->sfcError)
        {
            // check pred and succ of condiv for better error msg

            SFCConDivElement* pConDiv = theConDivs.get(i);
            FC_CGArray<int> predElemNrs;
            SFC_ELEM_TYPE predElemType = SFC_NO;
            FC_CGArray<int> succElemNrs;
            SFC_ELEM_TYPE succElemType = SFC_NO;

            if(getConDivPredSucc(pConDiv,
                                 &predElemNrs,
                                 &predElemType,
                                 &succElemNrs,
                                 &succElemType))
            {
                boxDiag0(CG_E_SFC_UNUSED_ELEM, theConDivs.get(i)->beg);
                theConDivs.get(i)->sfcError = true;
            }
        }
    }

    for (i=0; i<theBoxes.getUsed(); i++)
    {
        if (theBoxes.get(i) != NULL)
        {
            if (!theBoxes.get(i)->sfcError)
            {
                boxDiag0(CG_E_SFC_FBD_ELEM, theBoxes.get(i)->ext);
                theBoxes.get(i)->sfcError = true;
            }
        }
    }

    for (i=0; i<theSteps.getUsed(); i++)
    {
        if (!theSteps.get(i)->used 
            && !theSteps.get(i)->box.sfcError)
        {
            boxDiag0(CG_E_SFC_UNUSED_ELEM, theSteps.get(i)->box.ext);
            theSteps.get(i)->box.sfcError = true;
        }
    }

    for (i=0; i<theActionBlocks.getUsed(); i++)
    {
        if (!theActionBlocks.get(i)->used 
            && !theActionBlocks.get(i)->box.sfcError)
        {
            boxDiag0(CG_E_SFC_UNUSED_ELEM, theActionBlocks.get(i)->box.ext);
            theActionBlocks.get(i)->box.sfcError = true;
        }
    }


    for (i=0; i<theLines.getUsed(); i++)
    {
        if (theLines.get(i))
        {
            if (((theLines.get(i)->flags) & (LINE_FLAG_REACHED|LINE_FLAG_POWERRAIL)) == 0)
            {
                signalDiag0(CG_W_UNCONNECTED_LINE, theLines.get(i)->beg);
            }
        }
    }

    for (i=0; i<theConnectors.getUsed(); i++)
    {
        if (theConnectors.get(i))
        {
            if (!theConnectors.get(i)->used)
            {
                boxDiag0(CG_E_SFC_UNUSED_ELEM, theConnectors.get(i)->origin);
            }
        }
   }

}







SFC_LDFBD_SignalHash::ConnectorArray* 
SFC_LDFBD_SignalHash::getConnectorArrayInPos(Position pos, bool bInsert)
{
    int               i;
    HashArrayType*    pHash;
    ConnectorsAtPos*  pConnArrayAtPos;


    i = getHashIndex(pos);
    pHash = m_xyHashArray[i];
    if(!pHash)
    {
        if(bInsert)
        {
            pHash = new HashArrayType;
            m_xyHashArray[i] = pHash;
        }
        else
        {
            return NULL;
        }
    }

    //search for correct pos:
    for(i=pHash->getUsed()-1;i>=0;i--)
    {
        pConnArrayAtPos = pHash->get(i);
        if(POS_EQ(pConnArrayAtPos->pos, pos))
            return &(pConnArrayAtPos->con);
    }
    
    if(bInsert)
    {
        //insert an empty connector array:
        pConnArrayAtPos = new ConnectorsAtPos;
        pConnArrayAtPos->pos = pos;
        
        pHash->add(pConnArrayAtPos);
        return &pConnArrayAtPos->con;
    }
  
    return NULL;    
}




int SFC_LDFBD_SignalHash::getSFCSignal(Position              start, 
                                       FC_CGArray<LineElement*>* pReachedLines,
                                       FC_CGArray<PinElement*>*  pReachedPins,
                                       FC_CGArray<SFCConDivElement*>* pReachedConDivs,
                                       FC_CGArray<PINPOS>*       pConDivPinPos,
                                       Extension*            pMaxSignalExtension,
                                       bool*                 pStartOnConDiv)
{
    int                     jPointsToDo;
    int                     jConnects;
    int                     idx;
    Connector               con;
    Position                currPos;
    Position                newPoint;
    Position                oldPoint;
    ConnectorArray*         pConArray;
    FC_CGArray<Position>    pointsToDo;
    int                     result = 0;
    bool                    isConDiv;

    //reset the size of recieving arrays to 0:
    if(pReachedLines)
        pReachedLines->resetUsed();
    if(pReachedPins)
        pReachedPins->resetUsed();
    if(pReachedConDivs)
        pReachedConDivs->resetUsed();
    if(pConDivPinPos)
        pConDivPinPos->resetUsed();


    *pStartOnConDiv = false;
    // is start point on condiv
    for (int cdNr=0; cdNr<theConDivs.getUsed(); cdNr++)
    {
        SFCConDivElement* pConDiv = theConDivs.get(cdNr);
        if (start.y == pConDiv->beg.y 
            && start.x>= pConDiv->beg.x
            && start.x<= pConDiv->end.x)
        {
            if(pReachedConDivs)
                pReachedConDivs->add(pConDiv);
            if(pConDivPinPos)
                pConDivPinPos->add(TOP);
            *pStartOnConDiv = true;
        }
    }

    if (*pStartOnConDiv)
        return 0;

    pointsToDo.add(start);
    for(jPointsToDo=0; jPointsToDo<pointsToDo.getUsed(); jPointsToDo++)
    {
        currPos = pointsToDo.get(jPointsToDo);
        pConArray = getConnectorArrayInPos(currPos, false);
        if(!pConArray)
        {
            assert(jPointsToDo==0);//only the start point may have no connection...
            break;                   //
        }
    
        int nCon = pConArray->getUsed();
        for(jConnects=0;jConnects<nCon;jConnects++)
        {
            con = pConArray->get(jConnects);

            switch(con.conType)
            {
            case CON_TYP_PIN:
                if(pReachedPins)
                    pReachedPins->add(con.pPin);
            break;
            case CON_TYP_LINE:
                isConDiv = false;
                 
                oldPoint = con.pLine->end;
                newPoint = con.pLine->beg;

                //append line only if current point is start of line
                //this way lines are appended only once to pReachedLines
                if(POS_EQ(currPos, con.pLine->beg))
                {
                    //mark line as reached, this flag can be used to
                    //warn about unreached lines:
                    con.pLine->flags |= LINE_FLAG_REACHED;

                    if(pReachedLines)
                        pReachedLines->add(con.pLine);

                    if(pMaxSignalExtension)
                        MaxExtension(pMaxSignalExtension, con.pLine->ext);


                    //end of line is new point to investigate:
                    oldPoint = con.pLine->beg;
                    newPoint = con.pLine->end;
                }

                //is the newly reached point new point, already seen or evaluated ?
                idx = GetIndexOfPointInPointArray(newPoint, &pointsToDo);

                if(idx==-1)
                {
                    // is point on a ConDiv element, the stop here,
                    // do not look through a ConDiv


                    for (int cdNr=0; cdNr<theConDivs.getUsed(); cdNr++)
                    {
                        SFCConDivElement* pConDiv = theConDivs.get(cdNr);
                        if (newPoint.y == pConDiv->beg.y
                            && newPoint.x >= pConDiv->beg.x
                            && newPoint.x <= pConDiv->end.x)
                        {
                            isConDiv = true;
                            if(pReachedConDivs)
                                pReachedConDivs->add(pConDiv);

                            if (pConDivPinPos)
                            {
                                if (oldPoint.y < newPoint.y)
                                    pConDivPinPos->add(TOP);
                                else if (oldPoint.y > newPoint.y)
                                    pConDivPinPos->add(BOTTOM);
                                else if (oldPoint.x < newPoint.x)
                                    pConDivPinPos->add(LEFT);
                                else if (oldPoint.x > newPoint.x)
                                    pConDivPinPos->add(RIGHT);
                                else
                                    assert(!((oldPoint.x==newPoint.x)
                                             &&(oldPoint.y==newPoint.y)));
                            }
                        }
                    }

                    if (!isConDiv)
                    {
                        pointsToDo.add(newPoint);
                    }
                }
                else if(idx>jPointsToDo)
                    result |= 1; //<-point reached twice on different ways
                //else: point has already been evaluated....

                if(nCon==1) //<-detect unconnected line ends
                    result |= 2;
            break;
            default:
                assert(!"bad enum");
            }
        }
    }

    return result;
}





bool SFC_LDFBD_SignalHash::convertToLDFBD(LDFBD_SignalHash* pGraphicElements)
{
    if (m_isSFC)
        return false;

    // no sfc: copy all elements to normal signal hash,
    // this can be used to continue like before
    int i;
    for (i=0; i<thePins.getUsed(); i++)
    {
        pGraphicElements->addPinElement(thePins.get(i));
        thePins.set(i, NULL);
    }
    thePins.resetUsed();
    for (i=0; i<theBoxes.getUsed(); i++)
    {
        pGraphicElements->addBoxElement(theBoxes.get(i));
        theBoxes.set(i, NULL);
    }
    theBoxes.resetUsed();
    for (i=0; i<theLines.getUsed(); i++)
    {
        pGraphicElements->addLineElement(theLines.get(i));
        theLines.set(i, NULL);
    }
    theLines.resetUsed();
    for (i=0; i<theConnectors.getUsed(); i++)
    {
        pGraphicElements->addConnectorElement(theConnectors.get(i));
    }
    theConnectors.resetUsed();

    return true;
}


inline bool posLowerThanPos(Position a, Position b)
{
    return (a.y<b.y || (a.y==b.y && a.x<b.x));
}

inline Position SFC_LDFBD_SignalHash::getSFCPosition(SFC_SORT_TYPE type, int nr)
{
    Position pos;

    if (type == SFCS_STEP)
    {
        pos.x = theSteps.get(nr)->box.ext.left;
        pos.y = theSteps.get(nr)->box.ext.top;
    }
    else if (type == SFCS_TRANS)
    {
        pos.x = theTransitions.get(nr)->box.ext.left;
        pos.y = theTransitions.get(nr)->box.ext.top;
    }
    else if (type == SFCS_ACTION)
    {
        pos.x = theActions.get(nr)->box.ext.left;
        pos.y = theActions.get(nr)->box.ext.top;
    }
    else if (type == SFCS_ACTBLK)
    {
        pos.x = theActionBlocks.get(nr)->box.ext.left;
        pos.y = theActionBlocks.get(nr)->box.ext.top;
    }
    else if (type == SFCS_CONDIV)
    {
        pos.x = theConDivs.get(nr)->beg.x;
        pos.y = theConDivs.get(nr)->beg.y;
    }
    else 
    {
        assert(!"Strange enum!");
    }

    return pos;
}


void SFC_LDFBD_SignalHash::swapSFCElements(SFC_SORT_TYPE type, int p1, int p2)
{
    if (type == SFCS_STEP)
    {
        SFCStepElement* stepH;
        stepH = theSteps.get(p1);
        theSteps.set(p1, theSteps.get(p2));
        theSteps.set(p2, stepH);
    }
    else if (type == SFCS_TRANS)
    {
        SFCTransitionElement* transH;
        transH = theTransitions.get(p1);
        theTransitions.set(p1, theTransitions.get(p2));
        theTransitions.set(p2, transH);       
    }
    else if (type == SFCS_ACTION)
    {
      SFCActionElement* actionH;
      actionH = theActions.get(p1);
      theActions.set(p1, theActions.get(p2));
      theActions.set(p2, actionH);
    }
    else if (type == SFCS_ACTBLK)
    {
       SFCActionBlockElement* actblkH;
       actblkH = theActionBlocks.get(p1);
       theActionBlocks.set(p1, theActionBlocks.get(p2));
       theActionBlocks.set(p2, actblkH);
    }
    else if (type == SFCS_CONDIV)
    {
       SFCConDivElement* condivH;
       condivH = theConDivs.get(p1);
       theConDivs.set(p1, theConDivs.get(p2));
       theConDivs.set(p2, condivH);
    }
    else 
    {
        assert(!"Strange enum!");
    }
}

void SFC_LDFBD_SignalHash::sortArray(SFC_SORT_TYPE type, int start, int end)
{
    if (start==end)
        return;

    Position pivot;
    Position p1,p2;
    
    if (end>start)
    {
        pivot = getSFCPosition(type, end);  
        int from = start-1;
        int to = end;
        
        do
        {
            do 
            { from++;  p1 = getSFCPosition(type,from); } 
            while (posLowerThanPos(p1, pivot));
            
            do
            { to--; p2 = getSFCPosition(type,to); }
            while (posLowerThanPos(pivot, p2) && to>from);
            
            swapSFCElements(type, from, to);
        
        } while (from<to);

        swapSFCElements(type, to, from);
        swapSFCElements(type, from, end);

        sortArray(type, start, from-1);
        sortArray(type, from+1, end);

    }
}

// sort all sfc elements in the array according to their graphic position 
void SFC_LDFBD_SignalHash::sortSFCElements()
{
    sortArray(SFCS_STEP, 0, theSteps.getUsed()-1);
    sortArray(SFCS_TRANS, 0, theTransitions.getUsed()-1);
    sortArray(SFCS_ACTION, 0, theActions.getUsed()-1);
    sortArray(SFCS_ACTBLK, 0, theActionBlocks.getUsed()-1);
    sortArray(SFCS_CONDIV, 0, theConDivs.getUsed()-1);
}



// check if one rect (inner) is in another (outer)
// return: 0 no
//         1 full inside the outer rect
//         2 the rects are overlapping 
int isInRect(Extension outer, Extension inner)
{
    bool leftIn = inner.left>outer.left && inner.left<outer.right;
    bool rightIn = inner.right>outer.left && inner.right<outer.right;
    bool upperIn = inner.top>outer.top && inner.top<outer.bottom;
    bool bottomIn = inner.bottom>outer.top && inner.bottom<outer.bottom;
    bool fullIn = leftIn && rightIn && upperIn && bottomIn;
    bool overlapping = (leftIn && upperIn) 
                     ||(leftIn && bottomIn)
                     ||(rightIn && upperIn)
                     ||(rightIn && bottomIn);

    if (fullIn)
        return 1;
    if (overlapping)
        return 2;
    return 0;
}



void SFC_LDFBD_SignalHash::extractActionNets()
{
    int i;
    int elNr, actNr;

    
    for (i=0; i<theActions.getUsed(); i++)
    {
        LDFBD_SignalHash* actNet = new LDFBD_SignalHash();
        theActionNames.add(theActions.get(i)->box.text);
        theActionNets.add(actNet);

        CG_Edipos ediPos;
        boxEdp(&ediPos, theActions.get(i)->box.ext);
        ST_CheckIdentifier(pBodyCon->getDM(), &ediPos, theActions.get(i)->box.text);
    }

    // look at each element if it is in one of the action boxes
    // boxes
    for (elNr=0; elNr<theBoxes.getUsed(); elNr++)
    {
        BoxElement* pBox = theBoxes.get(elNr);
        if (pBox == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pBox->ext);
            if (isIn==1)
            {
                theActionNets.get(actNr)->addBoxElement(pBox);
                theBoxes.set(elNr, NULL);
                break;
            }
            else if (isIn==2)
            {
                assert(isIn!=2);
            }
        }
    }
    // lines
    for (elNr=0; elNr<theLines.getUsed(); elNr++)
    {
        LineElement* pLine = theLines.get(elNr);
        if (pLine == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pLine->ext);
            if (isIn==1)
            {
                theActionNets.get(actNr)->addLineElement(pLine);
                theLines.set(elNr, NULL);
                break;
            }
            else if (isIn==2)
            {
                assert(isIn!=2);
            }
        }
    }
    // connpairs
    for (elNr=0; elNr<theConnectors.getUsed(); elNr++)
    {
        ConnPairElement *pConn = theConnectors.get(elNr);
        if (pConn != NULL)
        {
            Extension ext;
            ext.left = pConn->origin.x;
            ext.right = pConn->origin.x;
            ext.top = pConn->origin.y;
            ext.bottom = pConn->origin.y;

            for (actNr=0; actNr<theActions.getUsed(); actNr++)
            {
       
               int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                   ext);
               if (isIn==1)
               {
                   theActionNets.get(actNr)->addConnectorElement(pConn);
                   theConnectors.set(elNr, NULL);
                   break;
               }
               else if (isIn==2)
               {
                   assert(isIn!=2);
               }
            }
        }
    }
    // pins
    for (elNr=0; elNr<thePins.getUsed(); elNr++)
    {
        PinElement* pPin = thePins.get(elNr);
        if (pPin == NULL)
            continue;
        Extension ext;
        ext.bottom = pPin->pos.y;
        ext.top = pPin->pos.y;
        ext.left = pPin->pos.x;
        ext.right = pPin->pos.x;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                ext);
            if (isIn==1)
            {
                theActionNets.get(actNr)->addPinElement(pPin);
                thePins.set(elNr, NULL);
                break;
            }
            else if (isIn==2)
            {
                assert(isIn!=2);
            }
        }
    }

    // steps
    for (elNr=0; elNr<theSteps.getUsed(); elNr++)
    {
        SFCStepElement* pStep = theSteps.get(elNr);
        if (pStep == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pStep->box.ext);
            if (isIn>0)
            {
                // error sfc element in action code
                if (!theSteps.get(elNr)->box.sfcError)
                {
                    boxDiag1(CG_E_SFC_ELEM_IN_ACTION, theSteps.get(elNr)->box.ext, 
                            theActions.get(actNr)->box.text);
                    theSteps.get(elNr)->box.sfcError = true;
                }
            }
        }
    }
    // transitions
    for (elNr=0; elNr<theTransitions.getUsed(); elNr++)
    {
        SFCTransitionElement* pTrans = theTransitions.get(elNr);
        if (pTrans == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pTrans->box.ext);
            if (isIn>0)
            {
                // error sfc element in action code
                if (!theTransitions.get(elNr)->box.sfcError)
                {
                    boxDiag1(CG_E_SFC_ELEM_IN_ACTION, theTransitions.get(elNr)->box.ext, 
                            theActions.get(actNr)->box.text);
                    theTransitions.get(elNr)->box.sfcError = true;
                }
            }
        }
    }
    // actions
    for (elNr=0; elNr<theActions.getUsed(); elNr++)
    {
        SFCActionElement* pAction = theActions.get(elNr);
        if (pAction == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pAction->box.ext);
            if (isIn>0)
            {
                // error sfc element in action code
                if (!theActions.get(elNr)->box.sfcError)
                {
                    boxDiag1(CG_E_SFC_ELEM_IN_ACTION, theActions.get(elNr)->box.ext, 
                            theActions.get(actNr)->box.text);
                    theActions.get(elNr)->box.sfcError = true;
                }
            }
       
        }
    }
    // action block
    for (elNr=0; elNr<theActionBlocks.getUsed(); elNr++)
    {
        SFCActionBlockElement* pActBlk = theActionBlocks.get(elNr);
        if (pActBlk == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pActBlk->box.ext);
            if (isIn>0)
            {
                // error sfc element in action code
                if (!theActionBlocks.get(elNr)->box.sfcError)
                {
                    boxDiag1(CG_E_SFC_ELEM_IN_ACTION, theActionBlocks.get(elNr)->box.ext, 
                            theActions.get(actNr)->box.text);
                    theActionBlocks.get(elNr)->box.sfcError = true;
                }
            }
       
        }
    }
    // conDivs
    for (elNr=0; elNr<theConDivs.getUsed(); elNr++)
    {
        SFCConDivElement* pConDiv = theConDivs.get(elNr);
        if (pConDiv == NULL)
            continue;
        for (actNr=0; actNr<theActions.getUsed(); actNr++)
        {
            int isIn = isInRect(theActions.get(actNr)->box.ext, 
                                pConDiv->ext);
            if (isIn>0)
            {
                // error sfc element in action code
                if (!theConDivs.get(elNr)->sfcError)
                {
                    boxDiag1(CG_E_SFC_ELEM_IN_ACTION, theConDivs.get(elNr)->ext, 
                            theActions.get(actNr)->box.text);
                    theConDivs.get(elNr)->sfcError = true;
                }
            }
       
        }
    }
}


void SFC_LDFBD_SignalHash::announceActionNames()
{
    // init action names
    ST_Identifier_C* ident;
    CG_Edipos        ediPos;

    for (int elNr=0; elNr<theActionNames.getUsed(); elNr++)
    {
        boxEdp(&ediPos, theActions.get(elNr)->box.ext);
        ident = new ST_Identifier_C(&ediPos, pBodyCon, theActions.get(elNr)->box.text);
        pBodyCon->m_pSFC->sfcAddAction(ident);
        delete ident;

        //delete(theActions.get(elNr));  // delete not yet edi pos is needed later
        //theActions.set(elNr, NULL);
    }
}

const TCHAR* SFC_LDFBD_SignalHash::getActionName(int nr)
{
    return ((theActions.get(nr)->box.text));
}


void SFC_LDFBD_SignalHash::calcPinConnections(FC_CGArray<PinElement*>* pins, 
                                              SFC_ELEM_TYPE type, int nr)
{ 
    int pinNr;

    SFCElem elem;
    

    for (pinNr=0; pinNr<pins->getUsed(); pinNr++)
    {
        PinElement* pPin = pins->get(pinNr);

        // add element to conElem
        elem.elemNr = nr;
        elem.type = type;
        theSFCCons.set(pPin->nrSFCCon, elem);

    }
}



void SFC_LDFBD_SignalHash::calcSFCConnections()
{
    int elNr;
    SFCElem elem;

    elem.type = SFC_NO;
    elem.elemNr = 0;

    theSFCCons.resetUsed();
    // concat pins with SFCCon Array
    for (elNr=0; elNr<thePins.getUsed(); elNr++)
    {
        if (thePins.get(elNr))
            thePins.get(elNr)->nrSFCCon = elNr;
        theSFCCons.add(elem);
    }
    
    // look at each elements
    // boxes
    for (elNr = 0; elNr<theBoxes.getUsed(); elNr++)
    {
        if (theBoxes.get(elNr))
            calcPinConnections(&(theBoxes.get(elNr)->pins), SFC_BOX, elNr); 
    }
    // no lines
    // connectors ???
    // steps
    for (elNr = 0; elNr<theSteps.getUsed(); elNr++)
    {
        calcPinConnections(&(theSteps.get(elNr)->box.pins), SFC_STEP, elNr); 
    }
    // transitions
    for (elNr = 0; elNr<theTransitions.getUsed(); elNr++)
    {
        calcPinConnections(&(theTransitions.get(elNr)->box.pins), SFC_TRANS, elNr); 
    }
    // actionBlocks
    for (elNr = 0; elNr<theActionBlocks.getUsed(); elNr++)
    {
        calcPinConnections(&(theActionBlocks.get(elNr)->box.pins), SFC_ACTBLK, elNr); 
    }
    // no actions (no pins)
    // conDivs
    for (elNr = 0; elNr<theConDivs.getUsed(); elNr++)
    {
        // nothing
    }

    // look at connectors
    // all connectors in actions have been removed,
    // connect connector and continuation
    for (elNr = 0; elNr<theConnectors.getUsed(); elNr++)
    {
        if (theConnectors.get(elNr))
        {
            bool found = false;
            ConnPairElement* pConnPair = theConnectors.get(elNr);
            const TCHAR*     pszName;
            if (pConnPair->boxType==BOX_TYPE_CONTINUATION)
            {
                pszName = pConnPair->text;
                for (int elNrSearch=0; elNrSearch<theConnectors.getUsed(); elNrSearch++)
                {
                    if (elNrSearch == elNr
                        || !theConnectors.get(elNrSearch))
                    {
                        continue;
                    }
                    else
                    {
                        ConnPairElement* pConnSecond = theConnectors.get(elNrSearch);
                        if (ST_IS_NAME_EQUAL(pszName, pConnSecond->text))
                        {
                            if (pConnSecond->boxType == BOX_TYPE_CONTINUATION)
                            {
                                // error
                                FC_CString errorString;
                                errorString.loadRes(CG_RES_STR_CONTINUATION, m_pdm->getResHMODULE())<<_T(": '")<<pszName<<_T("'");
                                boxDiag1(CG_E_SFC_REDEF, pConnSecond->origin, errorString);
                                
                                Position             start;
                                start.x = pConnSecond->pinpos.x;
                                start.y = pConnSecond->pinpos.y;
        
                                setSFCSignalFlag(start, LINE_FLAG_REACHED); 
                            }
                            else
                            {
                                // insert line
                                int x0,y0,x1,y1;
                                LineElement* pLine;
                                pLine = new LineElement();

                                x0 = pConnPair->pinpos.x;
                                y0 = pConnPair->pinpos.y;
                                x1 = pConnSecond->pinpos.x;
                                y1 = pConnSecond->pinpos.y;

                                pLine->beg.x = x0;
                                pLine->beg.y = y0;
        
                                //calculate end position of line from origin and size:
                                pLine->end.x = x1;
                                pLine->end.y = y1;
        
                                pLine->flags = 0;
        
                                assert(x0>=0 && y0>=0 && x1>=0 && y1>=0);

                                //calculate extension of line (minimal sourrounding rect:
                                GetExtensionFrom2Pos(pLine->beg, pLine->end, &pLine->ext);

                                addLineElement(pLine);

                                found = true;
                              
                            }
                            // remove connector
                            delete(pConnSecond);
                            theConnectors.set(elNrSearch, NULL);
                        }
                    }
                }
                // if a partner has been found, delete connector, else it is an unused element
                if (found)
                {
                    delete(pConnPair);
                    theConnectors.set(elNr, NULL);
                }
            }
        }
        
    }

    for (elNr = 0; elNr<theConnectors.getUsed(); elNr++)
    {
        if (theConnectors.get(elNr))
        {
            ConnPairElement* pConnPair = theConnectors.get(elNr);
            const TCHAR*     pszName = pConnPair->text;

            if (pConnPair->boxType==BOX_TYPE_CONNECTOR)
            {
                boxDiag1(CG_E_MISSING_CONTINUATION, pConnPair->origin, pszName);

                Position             start;
                start.x = pConnPair->pinpos.x;
                start.y = pConnPair->pinpos.y;

                setSFCSignalFlag(start, LINE_FLAG_REACHED);
            }
            else
            {
                boxDiag1(CG_E_MISSING_CONNECTOR, pConnPair->origin, pszName);

                Position             start;
                start.x = pConnPair->pinpos.x;
                start.y = pConnPair->pinpos.y;
                setSFCSignalFlag(start, LINE_FLAG_REACHED);
            }
            
            delete(pConnPair);
            theConnectors.set(elNr, NULL);
        }
    }


    // check for rubberband lines
    for (elNr=0; elNr<theLines.getUsed(); elNr++)
    {
        LineElement* pLine = theLines.get(elNr);
        if (pLine)
        {
            if(pLine->flags & LINE_FLAG_RUBBERLINE)
            {
                //Build edp by hand:
                CG_Edipos    edpLine;
                TCHAR        buffer[MAX_PATH];
                const TCHAR* pszFile = pBodyCon->getCGSym()->edp.pszFile;

                wsprintf(buffer, _T("(l,%d,%d,%d,%d)"),  
                         pLine->beg.x, pLine->beg.y, pLine->end.x, pLine->end.y);
                
                CG_InitTextEdipos(&edpLine, pszFile, buffer);
                m_pdm->msg0(CG_W_RUBBER_LINE, &edpLine);
            }
        }
    }

}


void SFC_LDFBD_SignalHash::doSteps()
{
    int              stepNr;
    ST_Identifier_C* ident;
    CG_Edipos        ediPos;


    for (stepNr = 0; stepNr<theSteps.getUsed(); stepNr++)
    {
        pBodyCon->m_pSFC->sfcStartStep();
        SFCStepElement* pStep = theSteps.get(stepNr);

        // init step name
        boxEdp(&ediPos, pStep->box.ext);
        ident = new ST_Identifier_C(&ediPos, pBodyCon, pStep->box.text);
        if (pStep->initStep)
            pBodyCon->m_pSFC->sfcAddInitialStep(ident);
        else
            pBodyCon->m_pSFC->sfcAddStep(ident);

        ST_CheckIdentifier(m_pdm, &ediPos, pStep->box.text);

        // find action blocks
        FC_CGArray<int> actBlkNrs;
        bool bs;

        bs = followActBlkSignal(stepNr, &actBlkNrs);

        if (!bs)
        {
            // Fehler
        }
        else
        {
            for (int abNr=0; abNr<actBlkNrs.getUsed(); abNr++)
            {
                // get action block pointer
                SFCActionBlockElement* pActBlk;
                pActBlk = theActionBlocks.get(actBlkNrs.get(abNr));

                boxEdp(&ediPos, pActBlk->box.ext);              
                // insert qualifier name
                ident = new ST_Identifier_C(&ediPos, pBodyCon, pActBlk->qualType);
    
                ST_CheckIdentifier(m_pdm, &ediPos, pActBlk->box.text);

                pBodyCon->m_pSFC->sfcAddActionAssociationQualifier(ident);

                // todo time qual

                // insert name of associated action 
                ident = new ST_Identifier_C(&ediPos, pBodyCon, pActBlk->box.text);
                pBodyCon->m_pSFC->sfcAddActionAssociation(ident);
            }
        }

        pBodyCon->m_pSFC->sfcEndStep();
        
    }

}



void SFC_LDFBD_SignalHash::extractTransitionNets()
{
    int tNr;

    for (tNr=0; tNr<theTransitions.getUsed(); tNr++)
    {
        bool bt;

        theTransNets.add(NULL);
       
        bt = followTransLDFBDSignal(tNr);

        theTransExpr.add(theTransitions.get(tNr)->box.text);
        if (!_tcscmp(_T(""), theTransitions.get(tNr)->box.text))
            transIsExpr.add(false);
        else
            transIsExpr.add(true);

        if (!bt)
        {
            // error, do nothing
        }
    }
}
  
void SFC_LDFBD_SignalHash::doTransitionStart(int tNr)
{
    ST_Identifier_C* ident;
    CG_Edipos ediPos;

    SFCTransitionElement* pTrans = theTransitions.get(tNr);
    FC_CGArray<int> predStepNrs;
    FC_CGArray<int> succStepNrs;
    SFC_CONDIV_TYPE predCon;
    SFC_CONDIV_TYPE succCon;
    bool            noPredSim;
    bool            noSuccSim;
    
    boxEdp(&ediPos, pTrans->box.ext);
    // start transition
    pBodyCon->m_pSFC->sfcStartTransition(&ediPos);
    
    bool bp, bs;
    bp = getTransPred(tNr, &predStepNrs, &predCon);
    bs = getTransSucc(tNr, &succStepNrs, &succCon);
  

    if (predCon == SFC_SIM_CONDIV)
        noPredSim = false;
    else
        noPredSim = true;

    if (succCon == SFC_SIM_CONDIV)
        noSuccSim = false;
    else
        noSuccSim = true;


    if (bp && bs)
    {
       

        // from steps
        pBodyCon->m_pSFC->sfcStartTransitionFrom();

        for (int i=0; i<predStepNrs.getUsed(); i++)
        {
            SFCStepElement* pStep = theSteps.get(predStepNrs.get(i));
            ident = new ST_Identifier_C(&ediPos, pBodyCon, pStep->box.text);
            pBodyCon->m_pSFC->sfcAddTransitionStep(ident, noPredSim);
            pStep->used = true;
        }

        // to steps
        pBodyCon->m_pSFC->sfcStartTransitionTo();

        for (i=0; i<succStepNrs.getUsed(); i++)
        {
            SFCStepElement* pStep = theSteps.get(succStepNrs.get(i));
            ident = new ST_Identifier_C(&ediPos, pBodyCon, pStep->box.text);
            pBodyCon->m_pSFC->sfcAddTransitionStep(ident, noSuccSim);
            pStep->used = true;
        }

        pBodyCon->m_pSFC->sfcAddTransitionPrio(pTrans->priority);

    }
    else
    {
        // error
        // but init from and to list with nothing
        pBodyCon->m_pSFC->sfcStartTransitionFrom();
        pBodyCon->m_pSFC->sfcStartTransitionTo();
    }
  
}



void SFC_LDFBD_SignalHash::doTransitionEnd(int tNr)
{
    CG_Edipos ediPos;
    boxEdp(&ediPos, theTransitions.get(tNr)->box.ext); 
    // end transition
    assert(!"not implem");//TODO
    pBodyCon->m_pSFC->sfcEndTransition(&ediPos, NULL);
}


void SFC_LDFBD_SignalHash::getTransEdiPos(int tNr,
                                          CG_Edipos* pEdiPos)
{
    boxEdp(pEdiPos, theTransitions.get(tNr)->box.ext); 
}


void SFC_LDFBD_SignalHash::getActionEdiPos(int aNr,
                                           CG_Edipos* pEdiPos)
{
    boxEdp(pEdiPos, theActions.get(aNr)->box.ext); 
}




bool SFC_LDFBD_SignalHash::followTransLDFBDSignal(int transNr)
{
    LDFBD_SignalHash* pTransNet = new LDFBD_SignalHash();
    theTransNets.set(transNr, pTransNet);

    bool ret = true;
    bool isFBD = false;

    bool isSFCElement = false;
    //Position sfcElementPos;

    Position tNetPos;

    BoxElement* pBox = &(theTransitions.get(transNr)->box);
    BoxElement* searchStartBox;
    FC_CGArray<BoxElement*> boxesToDo;
    int iBoxesToDo = 0;

    // start from transition
    getPinPos(LEFT, pBox, &tNetPos);

    // mark pin
    for (int tbp=0; tbp<pBox->pins.getUsed(); tbp++)
    {
        PinElement* pP = pBox->pins.get(tbp);
        if (pP->pos.x == tNetPos.x 
            && pP->pos.y == tNetPos.y)
        {
            pP->nrSFCCon = -1;
        }
    }

    // search first box
    {
        Position              start; 
        FC_CGArray<LineElement*>  reachedLines;
        FC_CGArray<PinElement*>   reachedPins;
        FC_CGArray<SFCConDivElement*> reachedConDivs;
        FC_CGArray<PINPOS>        conDivPinPos;
        Extension             maxSignalExtension;
        bool                  startOnConDiv;

        start = tNetPos;
        
        GetExtensionFrom2Pos(start, start, &maxSignalExtension);
        
        getSFCSignalAndWarn(start, 
                            &reachedLines,
                            &reachedPins,
                            &reachedConDivs,
                            &conDivPinPos,
                            &maxSignalExtension,
                            &startOnConDiv);
        for (int ip = 0; ip<reachedPins.getUsed(); ip++)
        {
            if (reachedPins.get(ip)->nrSFCCon != -1)
            {
                if (theSFCCons.get(reachedPins.get(ip)->nrSFCCon).type == SFC_BOX)
                {
                    BoxElement* pBox = theBoxes.get(theSFCCons.get(reachedPins.get(ip)->nrSFCCon).elemNr);
                    boxesToDo.add(pBox);
                    reachedPins.get(ip)->nrSFCCon = -1;
                }
                else 
                {
                    if (theSFCCons.get(reachedPins.get(ip)->nrSFCCon).type == SFC_TRANS
                        && theSFCCons.get(reachedPins.get(ip)->nrSFCCon).elemNr == transNr
                        && isWhichPin(&(theTransitions.get(theSFCCons.get(reachedPins.get(ip)->nrSFCCon).elemNr)->box), 
                           &(reachedPins.get(ip)->pos)) == LEFT)
                    {
                        // this is the same trans back to input, don't care the starting pin
                    }
                    else
                    {
                        isSFCElement = true;
                    }
                }
            }
            if (reachedConDivs.getUsed()>0)
                isSFCElement = true;
        }

        for (int il=0; il<reachedLines.getUsed(); il++)
        {
            pTransNet->addLineElement(reachedLines.get(il));
            for (int i=0; i<theLines.getUsed(); i++)
            {
                if (theLines.get(i) == reachedLines.get(il))
                {
                    theLines.set(i, NULL);
                    break;
                }
            }
            isFBD = true;
        }
    }


    // find the rest of the net
    for (iBoxesToDo=0; iBoxesToDo<boxesToDo.getUsed(); iBoxesToDo++)
    {           
        searchStartBox = boxesToDo.get(iBoxesToDo);

        pTransNet->addBoxElement(searchStartBox);
        checkTransNetElement(searchStartBox);
        isFBD = true;

        for (int st=0; st<theBoxes.getUsed(); st++)
        {
            if (theBoxes.get(st) == searchStartBox)
            {
                theBoxes.set(st, NULL);
                break;
            }
        }

        for (int iPin = 0; iPin<searchStartBox->pins.getUsed(); iPin++)
        {
            Position              start; 
            FC_CGArray<LineElement*>  reachedLines;
            FC_CGArray<PinElement*>   reachedPins;
            FC_CGArray<SFCConDivElement*> reachedConDivs;
            FC_CGArray<PINPOS>        conDivPinPos;
            Extension             maxSignalExtension;
            bool                  startOnConDiv;
            
            PinElement* pPin;
            pPin = searchStartBox->pins.get(iPin);

            pTransNet->addPinElement(pPin);
            for (int iip=0; iip<thePins.getUsed(); iip++)
            {
                if (thePins.get(iip) == pPin)
                {
                    thePins.set(iip,NULL);
                    break;
                }
            }

            if (pPin->nrSFCCon == -1)
                continue;

            start = pPin->pos;
            pPin->nrSFCCon = -1;

            isSFCElement = false;
            GetExtensionFrom2Pos(start, start, &maxSignalExtension);

            getSFCSignalAndWarn(start, 
                                &reachedLines,
                                &reachedPins,
                                &reachedConDivs,
                                &conDivPinPos,
                                &maxSignalExtension,
                                &startOnConDiv);

            for (int i=0; i<reachedPins.getUsed(); i++)
            {
                PinElement* pNewPin = reachedPins.get(i);
                if (pNewPin->nrSFCCon != -1)
                {
                    SFCElem* sfcElem = &(theSFCCons.get(pNewPin->nrSFCCon));
                    if (sfcElem->type == SFC_STEP 
                        || sfcElem->type == SFC_TRANS
                        || sfcElem->type == SFC_ACTBLK)
                    {
                        if (!isSFCElement)
                        {
                            isSFCElement = true;
                            // position
                        }
                    }
                    else
                    {
                        bool alreadyIn = false;
                        BoxElement* pAddBox = theBoxes.get(sfcElem->elemNr);
                       
                        // has the box been already inserted in the to do array
                        for (int jp=0; jp<boxesToDo.getUsed(); jp++)
                        {
                            if (pAddBox == boxesToDo.get(jp))
                                alreadyIn = true;
                        }
                        if (!alreadyIn)
                        {
                            boxesToDo.add(pAddBox);
                        }
                        pNewPin->nrSFCCon = -1;
                    }
                }
                
            }
            if (reachedConDivs.getUsed()>0)
            {
                if (!isSFCElement)
                {
                    isSFCElement = true;
                    // position
                }
            }
            // insert reached lines to FBD net
            for (int il=0; il<reachedLines.getUsed(); il++)
            {
                pTransNet->addLineElement(reachedLines.get(il));
                for (i=0; i<theLines.getUsed(); i++)
                {
                    if (theLines.get(i) == reachedLines.get(il))
                        theLines.set(i, NULL);
                }
                isFBD = true;
            }
        }
        
    }

    if (isSFCElement)
    {
        if (!theTransitions.get(transNr)->box.sfcError)
        {
            CG_Edipos ediPos;
            getTransEdiPos(transNr, &ediPos);
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                m_pdm->res1(CG_RES_STR_SFCELEM),
                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                m_pdm->res3(CG_RES_STR_TRANS_IN));
            theTransitions.get(transNr)->box.sfcError = true;
        }
    }

    if (!isFBD)
    {
        theTransNets.set(transNr, NULL);
        delete(pTransNet);
    }
    else   
    {
        CG_Edipos ediPos;
        getTransEdiPos(transNr, &ediPos);
        BoxElement* pRetBox = new BoxElement();
        PinElement* pRetPin = new PinElement();
 
        InitPinElement(pRetPin);
        pRetPin->pos = tNetPos;
        pRetPin->lPinType = PIN_TYPE_IN;

        pRetBox->boxType = BOX_TYPE_SFC_RETURN;
        pRetBox->boxFlags = BOX_FLAG_NORMAL;

        pRetBox->ext = theTransitions.get(transNr)->box.ext;
        pRetBox->pins.add(pRetPin);

        pTransNet->addPinElement(pRetPin);
        pTransNet->addBoxElement(pRetBox);
    }

    return ret;
}

bool SFC_LDFBD_SignalHash::followActBlkSignal(int stepNr,
                                              FC_CGArray<int>* pActBlkNrs)
{
    bool ret = true;

    Position start;

    assert(pActBlkNrs);

    pActBlkNrs->resetUsed();

    // find action blocks linked by a line to the step
    SFCStepElement* pStep = theSteps.get(stepNr);

    getPinPos(RIGHT, &pStep->box, &start);

    FC_CGArray<LineElement*> reachedLines;
    FC_CGArray<PinElement*>  reachedPins;
    FC_CGArray<SFCConDivElement*> reachedConDivs;
    FC_CGArray<PINPOS>       conDivPinPos;
    Extension            maxSignalExtension;
    bool                 startOnConDiv;

    GetExtensionFrom2Pos(start, start, &maxSignalExtension);

    getSFCSignalAndWarn(start, 
                        &reachedLines,
                        &reachedPins,
                        &reachedConDivs,
                        &conDivPinPos,
                        &maxSignalExtension,
                        &startOnConDiv);
    assert(pActBlkNrs);
    pActBlkNrs->resetUsed();

    for (int pNr=0; pNr<reachedPins.getUsed(); pNr++)
    {
        PinElement* pPin = reachedPins.get(pNr);

        // pin already reached from somewhere else
        if (pPin->nrSFCCon<0)
        {
            continue;
        }
        // else check element
        SFCElem pinElem =  theSFCCons.get(pPin->nrSFCCon);

        if (pinElem.type == SFC_ACTBLK)
        {
            pActBlkNrs->add(pinElem.elemNr);
            theActionBlocks.get(pinElem.elemNr)->used = true;
        }
        else // error
        {
            if (pPin->pos.x == start.x
                && pPin->pos.y == start.y
                && theSFCCons.get(pPin->nrSFCCon).type == SFC_STEP 
                && theSFCCons.get(pPin->nrSFCCon).elemNr == stepNr)
                continue;

            CG_Edipos ediPos;

            FC_CString errStr;
            BoxElement* pBox;

            if (theSFCCons.get(pPin->nrSFCCon).type == SFC_BOX)
            {
                pBox = theBoxes.get(theSFCCons.get(pPin->nrSFCCon).elemNr);
                errStr.loadRes(CG_RES_STR_NONSFCELEM, m_pdm->getResHMODULE());
                pBox->sfcError = true;
            }
            else 
            {
                pBox = getBoxOfSFCElementPin(pPin);
                errStr.loadRes(CG_RES_STR_WRONGSFCELEM, m_pdm->getResHMODULE());
                pBox->sfcError = true;
            }

            boxEdp(&ediPos, pBox->ext);
            if (!theSteps.get(stepNr)->box.sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                    errStr,
                    m_pdm->res1(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res2(CG_RES_STR_STEPX_OUT)<<m_pdm->res3(CG_RES_STR_ACTBL_ALLOWED));
                theSteps.get(stepNr)->box.sfcError = true;
            }
            ret = false;
        }
    }
    if (reachedConDivs.getUsed()>0)
    {
        // error
        if (!theSteps.get(stepNr)->box.sfcError)
        {
            CG_Edipos ediPos;

            boxEdp(&ediPos, reachedConDivs.get(0)->beg);

            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                m_pdm->res1(CG_RES_STR_CONV_DIV),
                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                m_pdm->jot1()<<m_pdm->res3(CG_RES_STR_STEPX_OUT)<<m_pdm->res3(CG_RES_STR_ACTBL_ALLOWED));
            theSteps.get(stepNr)->box.sfcError = true;
        }
        ret = false;
    }


    // find stacked action blocks
    for (int abNr=0; abNr<pActBlkNrs->getUsed(); abNr++)
    {
        int cx;
        int cy;
        cx = theActionBlocks.get(pActBlkNrs->get(abNr))->box.ext.left;
        cy = theActionBlocks.get(pActBlkNrs->get(abNr))->box.ext.bottom;
        int foundAB=-1;
        for (int toFindAB=0; toFindAB<theActionBlocks.getUsed(); toFindAB++)
        {
            if (theActionBlocks.get(toFindAB)->box.ext.left == cx
                && theActionBlocks.get(toFindAB)->box.ext.top == cy)
            {
                foundAB = toFindAB;
                break;
            }
        }
        if (foundAB!=-1)
        {
            for (int toCmpAB=0; toCmpAB<pActBlkNrs->getUsed(); toCmpAB++)
            {
                if (pActBlkNrs->get(toCmpAB) == foundAB)
                {
                    foundAB = -1;
                    break;
                }
            }
        }
        if (foundAB!=-1)
        {
            pActBlkNrs->add(foundAB);
            theActionBlocks.get(foundAB)->used = true;
        }
    }

    return ret;
}


bool SFC_LDFBD_SignalHash::checkStepPredSucc()
{
    bool ret = true;

    Position start;
    int stepNr;

    SFCStepElement* pStep; 

    FC_CGArray<LineElement*> reachedLines;
    FC_CGArray<PinElement*>  reachedPins;
    FC_CGArray<SFCConDivElement*> reachedConDivs;
    FC_CGArray<PINPOS>       conDivPinPos;
    Extension                maxSignalExtension;
    bool                     startOnConDiv;

    CG_Edipos ediPos;
    BoxElement* pBox;

    for (stepNr=0; stepNr<theSteps.getUsed(); stepNr++)
    {
        // pred
        pStep = theSteps.get(stepNr);
        for (int tb=0; tb<2; tb++)
        {
            if (tb==0)
                getPinPos(TOP, &pStep->box, &start);
            else 
                getPinPos(BOTTOM, &pStep->box, &start);

            GetExtensionFrom2Pos(start, start, &maxSignalExtension);

            getSFCSignalAndWarn(start, 
                                &reachedLines,
                                &reachedPins,
                                &reachedConDivs,
                                &conDivPinPos,
                                &maxSignalExtension,
                                &startOnConDiv);
            // pred,succ of a step:
            // ok: trans, condiv - test from there if it is the right end
            // error: step, actblk, box(fbd)
            for (int pNr=0; pNr<reachedPins.getUsed(); pNr++)
            {
                // pin already reached from somewhere else
                if (reachedPins.get(pNr)->nrSFCCon<0)
                {
                    continue;
                }
                // if not check reached element
                SFCElem elem = theSFCCons.get(reachedPins.get(pNr)->nrSFCCon);
                switch (elem.type)
                {
                case SFC_BOX:
                    pBox = theBoxes.get(elem.elemNr);
                    boxEdp(&ediPos, pBox->ext); 
                              
                    if (!theSteps.get(stepNr)->box.sfcError && !pBox->sfcError)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                            m_pdm->res1(CG_RES_STR_NONSFCELEM),
                            m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                            m_pdm->res3(CG_RES_STR_STEP)<<_T(" '")<<pStep->box.text<<_T("'"));    
                    }
                    theSteps.get(stepNr)->box.sfcError = true;   
                    pBox->sfcError = true;
                    ret = false;
                    break;
                case SFC_STEP:
                    if (stepNr != elem.elemNr)
                    {
                        pBox = &(theSteps.get(elem.elemNr)->box);
                        boxEdp(&ediPos, pBox->ext);

                        if (!theSteps.get(stepNr)->box.sfcError && !pBox->sfcError)
                        {
                            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                                m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<pBox->text<<_T("'"),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_STEP)<<_T(" '")<<pStep->box.text<<_T("'"));    
                        }
                        theSteps.get(stepNr)->box.sfcError = true;
                        pBox->sfcError = true;
                        ret = false;
                    }
                    break;
                case SFC_TRANS:
                    break;
                case SFC_ACTBLK:
                    pBox = &(theActionBlocks.get(elem.elemNr)->box);
                    boxEdp(&ediPos, pBox->ext); 

                    if (!theSteps.get(stepNr)->box.sfcError
                        && !pBox->sfcError)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                m_pdm->res1(CG_RES_STR_ACTBL),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_STEP_INOUT)<<_T(" '")<<pStep->box.text<<_T("'"));    
                    }
                    theSteps.get(stepNr)->box.sfcError = true;
                    pBox->sfcError = true;
                    ret = false;
                    break;
                default:
                    assert(elem.type == SFC_BOX 
                           ||elem.type == SFC_STEP
                           ||elem.type == SFC_TRANS
                           ||elem.type == SFC_ACTBLK);
                    break;
                }
            }            
        }

    }
    return ret;
}

bool SFC_LDFBD_SignalHash::getConDivPredSucc(SFCConDivElement *pConDiv,
                                             FC_CGArray<int>* pPredElemNrs,
                                             SFC_ELEM_TYPE* pPredElemType,
                                             FC_CGArray<int>* pSuccElemNrs,
                                             SFC_ELEM_TYPE* pSuccElemType)
{
    bool ret = true;

    FC_CGArray<SFCElem>   allPredElems;
    FC_CGArray<SFCElem>   allSuccElems;
    Connector         con;
    int               jConnects;
    Position          currPos;
    ConnectorArray*   pConArray;
    BoxElement*       pBox;
    CG_Edipos         ediPos;

    int               startx,endx;
    int               x;
    bool              goingUp;

    //SFCConDivElement *pConDiv = theConDivs.get(conDivNr);
    pConDiv->used = true;

    if (pConDiv->beg.x < pConDiv->end.x)
    {
        startx = pConDiv->beg.x;
        endx = pConDiv->end.x;
    }
    else 
    {
        startx = pConDiv->end.x;
        endx = pConDiv->beg.x;
    }

    currPos.y = pConDiv->beg.y;
    currPos.x = startx;

    // look at each point on the con div
    for (x=startx; x<=endx; x++)
    {
        currPos.x=x;
        pConArray = getConnectorArrayInPos(currPos, false);
        
        // no connector at that position
        if (!pConArray)
            continue;

        int nCon = pConArray->getUsed();
        for (jConnects=0; jConnects<nCon; jConnects++)
        {
            con = pConArray->get(jConnects);

            switch (con.conType)
            {
            case CON_TYP_PIN:
            {
                PinElement*  pPin = con.pPin;
                if (pPin->nrSFCCon<0)
                {
                    break;
                }

                SFCElem elem = theSFCCons.get(pPin->nrSFCCon);

                switch (elem.type)
                {
                case SFC_BOX:
                    pBox = theBoxes.get(elem.elemNr);
                    boxEdp(&ediPos, pBox->ext);

                    if (!pConDiv->sfcError && !pBox->sfcError)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                            m_pdm->res1(CG_RES_STR_FDBELEM),    
                            m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                            m_pdm->res3(CG_RES_STR_CONV_DIV));    
                    }
                    pConDiv->sfcError = true;
                    pBox->sfcError = true;
                    ret = false;
                    break;
                case SFC_ACTBLK:
                    pBox = &(theActionBlocks.get(elem.elemNr)->box);
                    boxEdp(&ediPos, pBox->ext);

                    if (!pConDiv->sfcError && !pBox->sfcError)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                m_pdm->res1(CG_RES_STR_ACTBL),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_CONV_DIV));    
                    }
                    pConDiv->sfcError = true;
                    pBox->sfcError = true;
                    ret = false;
                    break;
                case SFC_STEP:
                    pBox = &(theSteps.get(elem.elemNr)->box);
                    if (isWhichPin(pBox, &currPos) == TOP)
                        allSuccElems.add(elem);
                    else if (isWhichPin(pBox, &currPos) == BOTTOM)
                        allPredElems.add(elem);
                    else
                    {                      
                        boxEdp(&ediPos, pBox->ext); 
                        if(!pConDiv->sfcError && !pBox->sfcError)
                        {
                            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                m_pdm->res1(CG_RES_STR_STEPX_OUT),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_CONV_DIV));
                        }
                        pConDiv->sfcError = true;
                        pBox->sfcError = true;
                        ret = false;
                    }
                    break;
                case SFC_TRANS:
                    pBox = &(theTransitions.get(elem.elemNr)->box);
                    if (isWhichPin(pBox, &currPos) == TOP)
                        allSuccElems.add(elem);
                    else if (isWhichPin(pBox, &currPos) == BOTTOM)
                        allPredElems.add(elem);
                    else
                    {
                        boxEdp(&ediPos, pBox->ext);
        
                        if (!pConDiv->sfcError && !pBox->sfcError)
                        {
                            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                m_pdm->res1(CG_RES_STR_TRANS_IN),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_CONV_DIV));    
                        }
                        pConDiv->sfcError = true;
                        pBox->sfcError = true;
                        ret = false;
                    }
                    break;
                default:
                    assert(!"bad elem type");
                    break;
                }

                break;
            }
            case CON_TYP_LINE:
            {
                LineElement* pLine = con.pLine;
                pLine->flags |= LINE_FLAG_REACHED;
                Position startPoint;
                Position endPoint;

                if (pLine->beg.x==currPos.x && pLine->beg.y==currPos.y)
                { 
                    startPoint = pLine->beg;
                    endPoint = pLine->end;
                }
                else
                {
                    startPoint = pLine->end;
                    endPoint = pLine->beg;
                }
                
                if (startPoint.y>endPoint.y)
                    goingUp = true;
                else if (startPoint.y<endPoint.y)
                    goingUp = false;
                else // startPoint.y == endPoint.y
                {
                    // error - line on conDiv                 
                    signalDiag0(CG_E_SFC_HOR_LINE_CONDIV, startPoint);
                    ret = false;
                    continue;
                }

                // searching from end point of the line
                FC_CGArray<LineElement*> reachedLines;
                FC_CGArray<PinElement*>  reachedPins;
                FC_CGArray<SFCConDivElement*> reachedConDivs;
                FC_CGArray<PINPOS>       conDivPinPos;
                Extension            maxSignalExtension;
                bool                 startOnConDiv;

                GetExtensionFrom2Pos(endPoint, endPoint, &maxSignalExtension);

                getSFCSignalAndWarn(endPoint, 
                                    &reachedLines,
                                    &reachedPins,
                                    &reachedConDivs,
                                    &conDivPinPos,
                                    &maxSignalExtension,
                                    &startOnConDiv);
                for (int pNr=0; pNr<reachedPins.getUsed(); pNr++)
                {
                    // pin reached from somewhere else
                    if (reachedPins.get(pNr)->nrSFCCon<0)
                    {
                        continue;
                    }
                    // if not look at reached element
                    SFCElem elem = theSFCCons.get(reachedPins.get(pNr)->nrSFCCon);
                    switch (elem.type)
                    {
                    case SFC_BOX:
                        pBox = theBoxes.get(elem.elemNr);
                        boxEdp(&ediPos, pBox->ext);

                        if (!pConDiv->sfcError && !pBox->sfcError)
                        {
                            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                m_pdm->res1(CG_RES_STR_NONSFCELEM),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_CONV_DIV));
                        }
                        pConDiv->sfcError = true;
                        pBox->sfcError = true;
                        ret = false;
                        break;
                    case SFC_ACTBLK:
                        pBox = &(theActionBlocks.get(elem.elemNr)->box);
                        boxEdp(&ediPos, pBox->ext);

                        if (!pConDiv->sfcError && !pBox->sfcError)
                        {
                            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                                m_pdm->res1(CG_RES_STR_NONSFCELEM),    
                                m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                m_pdm->res3(CG_RES_STR_CONV_DIV));
                        }
                        pConDiv->sfcError = true;
                        pBox->sfcError = true;
                        ret = false;
                        break;
                    case SFC_STEP:
                        pBox = &(theSteps.get(elem.elemNr)->box);
                        boxEdp(&ediPos, pBox->ext);

                        if (goingUp)
                        {
                            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos))==BOTTOM)
                            {
                                allPredElems.add(elem);
                            }
                            else
                            {
                                if (!pBox->sfcError)
                                {
                                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                        m_pdm->res1(CG_RES_STR_CONV_DIV_IN),    
                                        m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                        m_pdm->res3(CG_RES_STR_STEP_IN));
                                    pBox->sfcError = true;
                                }
                            }

                        }
                        else
                        {
                            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos))==TOP)
                            {
                                allSuccElems.add(elem);
                            }
                            else
                            {
                                if (!pBox->sfcError)
                                {
                                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                                        m_pdm->res1(CG_RES_STR_CONV_DIV_OUT),    
                                        m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                        m_pdm->res3(CG_RES_STR_STEP_OUT));
                                    pBox->sfcError = true;
                                }
                            }
                        }
                        break;
                    case SFC_TRANS:
                        pBox = &(theTransitions.get(elem.elemNr)->box);
                        boxEdp(&ediPos, pBox->ext);
                        if (goingUp)
                        {
                            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos))==BOTTOM)
                            {
                                allPredElems.add(elem);
                            }
                            else
                            {
                                if (!pBox->sfcError)
                                {
                                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                                        m_pdm->res1(CG_RES_STR_CONV_DIV_IN),    
                                        m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                                        m_pdm->res3(CG_RES_STR_TRANS_IN));
                                    pBox->sfcError = true;
                                }
                            }
                        }
                        else
                        {
                            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos))==TOP)
                            {
                                allSuccElems.add(elem);
                            }
                            // else no error, can be a jump in front of a step, 
                            // will be checked from transition 
                        }
                        break;
                    default:
                        break;
                    }
                }
                for (int cdNr=0; cdNr<reachedConDivs.getUsed(); cdNr++)
                {
                    SFCConDivElement* pCD = reachedConDivs.get(cdNr);
                    if (pCD == pConDiv)
                        continue;
                    
                    boxEdp(&ediPos, pCD->beg);

                    if (!pConDiv->sfcError && !pCD->sfcError)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                            m_pdm->res1(CG_RES_STR_CONV_DIV),    
                            m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                            m_pdm->res3(CG_RES_STR_CONV_DIV));
                    }
                    pConDiv->sfcError = true;
                    pCD->sfcError = true;
                    ret = false;

                }
                

                break;  // end CON_TYPE_LINE
            }
            default:
                assert(!"bad enum");
                break;
            }
        }
    }

    // done all points -> examine allPredElems, allSuccElems
    //    same type and type defined as input type
    for (int eNr = 0; eNr<allPredElems.getUsed(); eNr++)
    {
        if (*pPredElemType == SFC_NO && eNr == 0)
            *pPredElemType = allPredElems.get(eNr).type;

        if (*pPredElemType != allPredElems.get(eNr).type)
        {
             boxEdp(&ediPos, pConDiv->beg);

             if (!pConDiv->sfcError)
             {
                m_pdm->msg1(CG_E_SFC_CONDIV_DIV_PREDSUCC, &ediPos,
                    m_pdm->res1(CG_RES_STR_PREDS));
             }
             pConDiv->sfcError = true;
             ret = false;
           
        }
        else
        {
            pPredElemNrs->add(allPredElems.get(eNr).elemNr);
        }  
    }


    for (eNr = 0; eNr<allSuccElems.getUsed(); eNr++)
    {
        if (*pSuccElemType == SFC_NO && eNr == 0)
            *pSuccElemType = allSuccElems.get(eNr).type;

        if (*pSuccElemType != allSuccElems.get(eNr).type)
        {
             boxEdp(&ediPos, pConDiv->beg);

             if (!pConDiv->sfcError)
             {
                m_pdm->msg1(CG_E_SFC_CONDIV_DIV_PREDSUCC, &ediPos, 
                    m_pdm->res1(CG_RES_STR_SUCCS));
             }
             pConDiv->sfcError = true;
             ret = false;
           
        }
        else
        {
            pSuccElemNrs->add(allSuccElems.get(eNr).elemNr);
        }  
    }

    if (pSuccElemNrs->getUsed()==0)
    {
        boxEdp(&ediPos,  pConDiv->beg);
        if (!pConDiv->sfcError)
        {
            m_pdm->msg1(CG_E_SFC_CONDIV_NO_PREDSUCC, &ediPos, 
                m_pdm->res1(CG_RES_STR_SUCC));
        }
        pConDiv->sfcError = true;
        ret = false;
    }

    if (pPredElemNrs->getUsed()==0)
    {
        boxEdp(&ediPos, pConDiv->beg);
        if (!pConDiv->sfcError)
        {
            m_pdm->msg1(CG_E_SFC_CONDIV_NO_PREDSUCC, &ediPos, 
                m_pdm->res1(CG_RES_STR_PRED));
        }
        pConDiv->sfcError = true;
        ret = false;
    }

    if (*pSuccElemType == SFC_STEP && *pPredElemType == SFC_STEP)
    {
        boxEdp(&ediPos, pConDiv->beg);

        if (!pConDiv->sfcError)
        {
            m_pdm->msg0(CG_E_SFC_CONDIV_STEPS_ONLY, &ediPos);
        }

        for (eNr=0; eNr<pPredElemNrs->getUsed(); eNr++)
        {
            SFCStepElement* pStep;
            pStep = theSteps.get(pPredElemNrs->get(eNr));
            pStep->box.sfcError = true;
        }

        for (eNr=0; eNr<pSuccElemNrs->getUsed(); eNr++)
        {
            SFCStepElement* pStep;
            pStep = theSteps.get(pSuccElemNrs->get(eNr));
            pStep->box.sfcError = true;
        }

        pConDiv->sfcError = true;
        ret = false;
    }


    return ret;
}




bool SFC_LDFBD_SignalHash::getTransPred(int transNr,
                                        FC_CGArray<int>* pPredStepNrs,
                                        SFC_CONDIV_TYPE* pPredType)
{
    bool ret = true;

    Position start;
   
    SFCTransitionElement* pTrans; 

    FC_CGArray<LineElement*> reachedLines;
    FC_CGArray<PinElement*>  reachedPins;
    FC_CGArray<SFCConDivElement*> reachedConDivs;
    FC_CGArray<PINPOS>       conDivPinPos;
    Extension            maxSignalExtension;
    bool                 startOnConDiv;

    CG_Edipos ediPos;
    BoxElement* pBox;   

    int anzPred = 0;
    assert(pPredStepNrs && pPredType);
    *pPredType = SFC_NONE_CONDIV;
    pPredStepNrs->resetUsed();

    // ****************** pred *******************
    pTrans = theTransitions.get(transNr);
 
    getPinPos(TOP, &pTrans->box, &start);
    GetExtensionFrom2Pos(start, start, &maxSignalExtension);

    getSFCSignalAndWarn(start, 
                        &reachedLines,
                        &reachedPins,
                        &reachedConDivs,
                        &conDivPinPos,
                        &maxSignalExtension,
                        &startOnConDiv);
    // pred of a trans:
    // ok: trans, condiv - test from there if it is the right end
    // error: step, actblk, box(fbd)
    // top is step input must be connected with trans, condiv output
    for (int pNr=0; pNr<reachedPins.getUsed(); pNr++)
    {
        // pin already reached from somewhere else
        if (reachedPins.get(pNr)->nrSFCCon<0)
        {
            continue;
        }
        // else look at reached element
        SFCElem elem = theSFCCons.get(reachedPins.get(pNr)->nrSFCCon);
        switch (elem.type)
        {
        case SFC_BOX:
            pBox = theBoxes.get(elem.elemNr);
            boxEdp(&ediPos, pBox->ext);

            if (!pTrans->box.sfcError && !pBox->sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                    m_pdm->res1(CG_RES_STR_NONSFCELEM),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_TRANS));
            }
            pTrans->box.sfcError = true;
            pBox->sfcError = true;
            ret = false;
            break;
        case SFC_STEP:
            pBox = &(theSteps.get(elem.elemNr)->box);
            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos)) == TOP)
            {
                // input -> input
                boxEdp(&ediPos, pBox->ext);

                if (!pTrans->box.sfcError && !pBox->sfcError)
                {
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                            m_pdm->res1(CG_RES_STR_TRANS_IN),    
                            m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                            m_pdm->res3(CG_RES_STR_STEP_IN)<<_T(" '")<<pBox->text<<_T("'"));                       
                }
                pTrans->box.sfcError = true;
                pBox->sfcError = true;
                ret = false;
            }
            else 
            {
                anzPred++;
                pPredStepNrs->add(elem.elemNr);
            }
            break;
        case SFC_TRANS:
            if (elem.elemNr == transNr)
                continue;
            pBox = &(theTransitions.get(elem.elemNr)->box);
            boxEdp(&ediPos, pBox->ext);

            if (!pTrans->box.sfcError && !pBox->sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                    m_pdm->res1(CG_RES_STR_TRANS),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_TRANS_IN));
            }
            pTrans->box.sfcError = true;
            pBox->sfcError = true;
            ret = false;
            break;
        case SFC_ACTBLK:
            pBox = &(theActionBlocks.get(elem.elemNr)->box);
            boxEdp(&ediPos, pBox->ext);

            if (!pTrans->box.sfcError && !pBox->sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                    m_pdm->res1(CG_RES_STR_ACTBL),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_TRANS));
            }
            pTrans->box.sfcError = true;
            pBox->sfcError = true;
            ret = false;
            break;
        default:
            assert(elem.type == SFC_BOX 
                   ||elem.type == SFC_STEP
                   ||elem.type == SFC_TRANS
                   ||elem.type == SFC_ACTBLK);
            break;
        }
    }
    // multiple preds?    
    if (anzPred>1 
        || (anzPred>=1 && reachedConDivs.getUsed()>=1)
        || (reachedConDivs.getUsed()>1))
    {
        pBox = &(pTrans->box);
        boxEdp(&ediPos, pBox->ext);

        if (!pTrans->box.sfcError)
        {
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                m_pdm->res1(CG_RES_STR_MULTI_STEP),
                _T(""), _T(""));
        }
        pTrans->box.sfcError = true;
        ret = false;
    }
    
    // one pred condiv
    if (ret && reachedConDivs.getUsed()==1)
    {
        //int conDivNr = reachedConDivs.get(0);
        SFCConDivElement* pConDiv = reachedConDivs.get(0);

        FC_CGArray<int> predElemNrs;
        SFC_ELEM_TYPE predElemType;
        FC_CGArray<int> succElemNrs;
        SFC_ELEM_TYPE succElemType;

        bool cdRet;

        predElemType = SFC_STEP;
        succElemType = SFC_TRANS;

        if (startOnConDiv || (conDivPinPos.get(0)==BOTTOM))
        {
            cdRet = getConDivPredSucc(pConDiv, 
                                      &predElemNrs, 
                                      &predElemType,
                                      &succElemNrs,
                                      &succElemType);
            // no error
            if (cdRet)
            {
                (*pPredType) = pConDiv->type;
                for (int psNr=0; psNr<predElemNrs.getUsed(); psNr++)
                {
                    pPredStepNrs->add(predElemNrs.get(psNr));
                }
                for (int stNr=0; stNr<succElemNrs.getUsed(); stNr++)
                {
                    SFCTransitionElement* pPrioTrans = theTransitions.get(succElemNrs.get(stNr));
                    pPrioTrans->priority = stNr;
                }
            }
        }
        else    // trans in -> condiv in
        {
            boxEdp(&ediPos, pTrans->box.ext);

            if (!pTrans->box.sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                    m_pdm->res1(CG_RES_STR_TRANS_IN),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_CONV_DIV_IN));
            }
            pTrans->box.sfcError = true;
            ret = false;
        }

    }

    return ret;
}


bool SFC_LDFBD_SignalHash::getTransSucc(int transNr,
                                        FC_CGArray<int>* pSuccStepNrs,
                                        SFC_CONDIV_TYPE* pSuccType)
{
    bool ret = true;

    Position start;
   
    SFCTransitionElement* pTrans; 

    FC_CGArray<LineElement*> reachedLines;
    FC_CGArray<PinElement*>  reachedPins;
    FC_CGArray<SFCConDivElement*> reachedConDivs;
    FC_CGArray<PINPOS>       conDivPinPos;
    Extension            maxSignalExtension;
    bool                 startOnConDiv;

    CG_Edipos ediPos;
    BoxElement* pBox;   

    int anzSucc = 0;
    assert(pSuccStepNrs && pSuccType);
    *pSuccType = SFC_NONE_CONDIV;
    pSuccStepNrs->resetUsed();

    // ****************** Succ *******************
    pTrans = theTransitions.get(transNr);
 
    getPinPos(BOTTOM, &pTrans->box, &start);
    GetExtensionFrom2Pos(start, start, &maxSignalExtension);

    getSFCSignalAndWarn(start, 
                        &reachedLines,
                        &reachedPins,
                        &reachedConDivs,
                        &conDivPinPos,
                        &maxSignalExtension,
                        &startOnConDiv);
    // succ of a trans:
    // ok: step, condiv - test from there if it is the right end
    // error: actblk, box(fbd)
    // if succ is top of trans error, if succ is bottom of trans ok
    for (int pNr=0; pNr<reachedPins.getUsed(); pNr++)
    {
        // pin already reached from somewhere else
        if (reachedPins.get(pNr)->nrSFCCon<0)
        {
            continue;
        }
        // else look at reached element
        SFCElem elem = theSFCCons.get(reachedPins.get(pNr)->nrSFCCon);
        switch (elem.type)
        {
        case SFC_BOX:
            pBox = theBoxes.get(elem.elemNr);
            boxEdp(&ediPos, pBox->ext);

            if (!pTrans->box.sfcError && !pBox->sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                    m_pdm->res1(CG_RES_STR_NONSFCELEM),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_TRANS));
            }
            pTrans->box.sfcError = true;
            pBox->sfcError = true;
            ret = false;
            break;
        case SFC_STEP:
            pBox = &(theSteps.get(elem.elemNr)->box);
            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos)) == BOTTOM)
            {
                // output -> output
                boxEdp(&ediPos, pBox->ext);

                if (!pTrans->box.sfcError && !pBox->sfcError)
                {
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                        m_pdm->res1(CG_RES_STR_TRANS_OUT),    
                        m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                        m_pdm->res3(CG_RES_STR_STEP_OUT)<<_T(" '")<<pBox->text<<_T("'"));                       
                }
                pTrans->box.sfcError = true;
                pBox->sfcError = true;
                ret = false;
            }
            else 
            {
                anzSucc++;
                pSuccStepNrs->add(elem.elemNr);
            }
            break;
        case SFC_TRANS:
            if (elem.elemNr == transNr)
                continue;
            pBox = &(theTransitions.get(elem.elemNr)->box);
            if (isWhichPin(pBox, &(reachedPins.get(pNr)->pos)) == TOP)
            {
                boxEdp(&ediPos, pBox->ext); 

                if (!pTrans->box.sfcError && !pBox->sfcError)
                {
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos,
                        m_pdm->res1(CG_RES_STR_TRANS),    
                        m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                        m_pdm->res3(CG_RES_STR_TRANS_IN));
                }
                pTrans->box.sfcError = true;
                pBox->sfcError = true;
                ret = false;
            }
            break;
        case SFC_ACTBLK:
            pBox = &(theActionBlocks.get(elem.elemNr)->box);
            boxEdp(&ediPos, pBox->ext);

            if (!pTrans->box.sfcError && !pBox->sfcError)
            {
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                    m_pdm->res1(CG_RES_STR_ACTBL),    
                    m_pdm->res2(CG_RES_STR_CONNECTED_WITH),
                    m_pdm->res3(CG_RES_STR_TRANS));
            }
            pTrans->box.sfcError = true;
            pBox->sfcError = true;
            ret = false;
            break;
        default:
            assert(elem.type == SFC_BOX 
                   ||elem.type == SFC_STEP
                   ||elem.type == SFC_TRANS
                   ||elem.type == SFC_ACTBLK);
            break;
        }
    }
    // multiple preds?
    int anzFollCD = 0;
    for (int cdcount = 0; cdcount<reachedConDivs.getUsed(); cdcount++)
    {
        if (conDivPinPos.get(cdcount)==TOP)
            anzFollCD++;
    }
    if (anzSucc>1 
        || (anzSucc>=1 && anzFollCD>=1)
        || (anzFollCD>1))
    {
        pBox = &(pTrans->box);
        boxEdp(&ediPos, pBox->ext);

        if (!pTrans->box.sfcError)
        {
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &ediPos, 
                m_pdm->res1(CG_RES_STR_MULTI_STEP),    
                _T(""), _T(""));
        }
        pTrans->box.sfcError = true;
        ret = false;
    }
    
    // one succ condiv
    if (ret && reachedConDivs.getUsed()==1)
    {
        //int conDivNr = reachedConDis.get(0);
        SFCConDivElement* pConDiv = reachedConDivs.get(0);

        FC_CGArray<int> predElemNrs;
        SFC_ELEM_TYPE predElemType;
        FC_CGArray<int> succElemNrs;
        SFC_ELEM_TYPE succElemType;

        bool cdRet;

        predElemType = SFC_TRANS;
        succElemType = SFC_STEP;

        if (startOnConDiv || (conDivPinPos.get(0)==TOP))
        {
            cdRet = getConDivPredSucc(pConDiv, 
                                      &predElemNrs, 
                                      &predElemType,
                                      &succElemNrs,
                                      &succElemType);
            // no error
            if (cdRet)
            {
                (*pSuccType) = pConDiv->type;
                for (int psNr=0; psNr<succElemNrs.getUsed(); psNr++)
                {
                    pSuccStepNrs->add(succElemNrs.get(psNr));
                }
            }
        }
        else    // trans out -> condiv out
        {
            // no error
        }

    }

    return ret;
}







bool SFC_LDFBD_SignalHash::getPinPos(PINPOS pinpos, 
                                     BoxElement* pBox, 
                                     Position *ppos)
{
    bool ret = true;
    int pinNr = -1;
    int left,right,top,bottom;

    left = pBox->ext.left;
    right = pBox->ext.right;
    top = pBox->ext.top;
    bottom = pBox->ext.bottom;

    for (int pNr=0; pNr<pBox->pins.getUsed(); pNr++)
    {
        PinElement* pPin = pBox->pins.get(pNr);
        int x,y;
        x = pPin->pos.x;
        y = pPin->pos.y;

        switch (pinpos)
        {
        case TOP:
            if (y<=top)
            {
                if (pinNr>-1)
                    ret = false;
                ppos->x = x;
                ppos->y = y;
                pinNr = pNr;
            }
            break;
        case BOTTOM:
            if (y>=bottom)
            {
                if (pinNr>-1)
                    ret = false;
                ppos->x = x;
                ppos->y = y;
                pinNr = pNr;
            }
            break;
        case LEFT:
            if (x<=left)
            {
                if (pinNr>-1)
                    ret = false;
                ppos->x = x;
                ppos->y = y;
                pinNr = pNr;
            }
            break;
        case RIGHT:
            if (x>=right)
            {
                if (pinNr>-1)
                    ret = false;
                ppos->x = x;
                ppos->y = y;
                pinNr = pNr;
            }
            break;
        default:
            assert(pinpos==TOP || pinpos==BOTTOM || pinpos==LEFT || pinpos==RIGHT);
            break;
        }
    }

    if (pinNr == -1)
        ret = false;

    return ret;
}


SFC_LDFBD_SignalHash::PINPOS SFC_LDFBD_SignalHash::isWhichPin(const BoxElement* pBox, const Position *pPos)
{
    PINPOS pinpos;

    int left,right,top,bottom;
    int x,y;

    left = pBox->ext.left;
    right = pBox->ext.right;
    top = pBox->ext.top;
    bottom = pBox->ext.bottom;    

    x = pPos->x;
    y = pPos->y;

    if (x<=left)
        pinpos = LEFT;
    else if (x>=right)
        pinpos = RIGHT;
    else if (y<=top)
        pinpos = TOP;
    else if (y>=bottom)
        pinpos = BOTTOM;

    return pinpos;
}



BoxElement* SFC_LDFBD_SignalHash::getBoxOfSFCElementPin(const PinElement *pPin)
{
    assert(pPin->nrSFCCon>-1);

    SFCElem elem = theSFCCons.get(pPin->nrSFCCon);

    

    switch (elem.type)
    {
    case SFC_STEP:
        return &(theSteps.get(elem.elemNr)->box);
        break;
    case SFC_TRANS:
        return &(theTransitions.get(elem.elemNr)->box);
        break;
    case SFC_ACTBLK:
        return &(theActionBlocks.get(elem.elemNr)->box);
        break;
    default:
        assert(elem.type == SFC_STEP 
              || elem.type == SFC_TRANS
              || elem.type == SFC_ACTBLK);
        break;
    }

    return NULL;
}





void SFC_LDFBD_SignalHash::getSFCSignalAndWarn(Position              start, 
                             FC_CGArray<LineElement*>* pReachedLines,
                             FC_CGArray<PinElement*>*  pReachedPins,
                             FC_CGArray<SFCConDivElement*>* pReachedConDivs,
                             FC_CGArray<PINPOS>*       pConDivPinPos,
                             Extension*            pMaxSignalExtension,
                             bool*                 pStartOnConDiv)
{
    int iRet;
    
    iRet = getSFCSignal(start, pReachedLines, pReachedPins, 
                        pReachedConDivs, pConDivPinPos,
                        pMaxSignalExtension, pStartOnConDiv);

    //iRet: 0=ok, 1=signal cyclic, 2=unconnected line ends 3=both
    if(iRet!=0)
    {
        //problem: don't dump same error twice for same signal.
        //solution: call setSignalFlag, return is true if flag was 
        //not set for at least one line of the signal.
        if(setSFCSignalFlag(start, LINE_FLAG_UNCONNECT_WARN))
        {
            if(iRet & 1)
                signalDiag0(CG_W_SIGNAL_CYCLIC, start);
            if(iRet & 2)
                signalDiag0(CG_W_UNCONNECTED_LINE, start);
        }
    }
}
bool SFC_LDFBD_SignalHash::setSFCSignalFlag(Position start, long lFlag)
{
    int                  n;
    LineElement*         pLine;
    bool                 bFlagWasNotSetInAllLines = false;
    FC_CGArray<LineElement*> lines;

    assert(lFlag);

    bool b;
    getSFCSignal(start, &lines, NULL, NULL, NULL, NULL, &b);
    
    n=lines.getUsed();
    if(n<=0)
    {   
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



bool SFC_LDFBD_SignalHash::checkTransNetElement(const BoxElement* pBox)
{

    CG_Edipos ediPos;

    boxEdp(&ediPos, pBox->ext);

    if (pBox->boxType == BOX_TYPE_VARWRITE)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_VARWRITE));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_VARFEEDBACK)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_FEEDBACK));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_CONNECTOR)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_CONNECTOR));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_CONTINUATION)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_CONTINUATION));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_COIL)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_COIL));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_FB)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_FB));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_LABEL)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_LABEL));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_JUMP)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_JUMP));
        return false;
    }
    else if (pBox->boxType == BOX_TYPE_RETURN)
    {
        m_pdm->msg1(CG_E_SFC_TRANS_WRONG_ELEM, &ediPos, m_pdm->res1(CG_RES_STR_RETURN));
        return false;
    }
   
    return true;
}

