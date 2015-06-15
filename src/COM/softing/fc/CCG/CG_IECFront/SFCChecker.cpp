



//----  Local Defines:   -------------------------------------------*
#define SFC_Q_N  0x0001
#define SFC_Q_R  0x0002
#define SFC_Q_S  0x0004
#define SFC_Q_P  0x0008
#define SFC_Q_P0 0x0010
#define SFC_Q_P1 0x0020
#define SFC_Q_L  0x0040
#define SFC_Q_D  0x0080
#define SFC_Q_SD 0x0100
#define SFC_Q_DS 0x0200
#define SFC_Q_SL 0x0400

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "SFCChecker.h"
#include "resource.h"

//----  Static Initializations:   ----------------------------------*




// =================================================
// help function
// =================================================



inline long GetBoxXFromEdp(const CG_Edipos* pEdp)
{
	if (pEdp->typ==CG_EDP_ST)
		return pEdp->lc.c;

    assert(pEdp->typ==CG_EDP_4GR_BOX && pEdp->gr.x>=0);
    return pEdp->gr.x;
}
inline long GetBoxYFromEdp(const CG_Edipos* pEdp)
{
	if (pEdp->typ==CG_EDP_ST)
		return pEdp->lc.l;

    assert(pEdp->typ==CG_EDP_4GR_BOX && pEdp->gr.y>=0);
    return pEdp->gr.y;
}



// ======================================================================
// class SFC
// ======================================================================







// description of a transition
//  in textual representation
//  this is used to hold all strings in linear arrays
//  here only the offset in these arrays and other simple parameter are stored
struct SFCTransition
{
    int fromStartOffset;    // the range in the fromList-Array
    int fromEndOffset;      //     all steps proceeding a transition
    bool fromSim;           //  are these steps connected via simultaneous link
    int toStartOffset;      // the range in the toList-Array
    int toEndOffset;        //     all steps following a transition
    bool toSim;             //  are these steps connected via simultaneous link
    int prio;               // the priority of the transition
};


// the types of the nodes in the SFC-graph
enum SFCElementTyp 
{
    SFCtStep, SFCtTrans, SFCtAltCon, SFCtAltDiv, SFCtSimCon, SFCtSimDiv
};

// description of an element in the SFC-graph
struct SFCElement
{
    SFCElementTyp typ;  // type of the element
    // position         // graphical position
    int nr;             // nr of the step or transition
    int pred;           // pointer to the (one) predecessor of the node
    int succ;           // pointer to the (one) successer of the node
    int predLeft;       // all pred. and succ. of one node are coupled
    int predRight;      // via  linked list
    int succLeft;
    int succRight;
};


// struct to color SFC Graph
struct SFCColor
{
    int simDepth;
    int simBranch;
};

enum SFCColorReturnTyp
{
    ColFinalNode, ColSimConNode, ColSimDivNode, ColAltConNode, ColAltDivNode,
    ColSameColNode, ColDifColNode, ColNextNode, ColErrorNode, ColOpenNode, ColOpenSimNode, ColSimLoops
};

struct SFCColorReturn 
{
    int nodeNr;
    SFCColorReturnTyp nodeTyp;
};



//define SFC_DEBUGCODE to enable some debug routine for standalone test 
//#define SFC_DEBUGCODE

class SFC
{
public:
    SFC(CGT_CDiagMsg* pdm)
    {
        m_pdm = pdm;
    };
    
    // give a name to the SFC
    bool addSFCName(const TCHAR* name,
                    const CG_Edipos* ppos);
    // add an initial step
    bool addInitialStep(const TCHAR* name,
                        const CG_Edipos* ppos);
    // add a normal step
    bool addStep(const TCHAR* name,
                 const CG_Edipos* ppos);
    // add a transition
    //   prio     : priority
    //   fromList : all steps from which the transition can be reached in the sfc
    //   toList   : all steps that can be reached from the transition
    //   fromPar  : are the fromList-steps connected via a simultaneous link
    //   toPar    : are the toList-steps connected via a simultaneous link
    bool addTransition(int prio, 
                       const FC_CStrList* fromList, 
                       const FC_CStrList* toList,
                       bool fromSim, 
                       bool toSim,
                       const CG_Edipos* ppos,
                       FC_CGArray<CG_Edipos> *pfromPos,
                       FC_CGArray<CG_Edipos> *ptoPos);
    bool addTransitionName(const TCHAR* name);
    bool addTransitionNoName();
    // add an actionControlBlock
    //    stepName   : the name of the step the acb is associated to
    //    actionName : the name of the action the acb is associated to
    //    qualifier  : the qualifier set
    //    timeParam  : the optional time parameter
    bool addActionBlock(const TCHAR* stepName, 
                        const TCHAR* actionName, 
                        const TCHAR* qualifier,
                        const long timeParam,
                        const CG_Edipos* ppos);
    // add an action
    bool addAction(const TCHAR* name,
                   const CG_Edipos* ppos);

    // create the sfc graph to do some checkings
    bool createGraph(const CG_Edipos* pEndOfFile);
    // check the sfc graph (structure, safe, unsafe, unreachable)
    bool check();
    // generate the java program text for the sfc-interpreter class
    bool codeGen();
    int  getQualVal(const TCHAR* pName, const CG_Edipos* pEdiPos);

    int getAnzSteps()   { return m_numSteps;};
    int getAnzTrans()   { return m_numTrans;};
    int getAnzActions() { return m_numActions;};
    int getInitStep()   { return m_initialStep;};

    int getAnzNextSteps() { return m_nextSteps.getUsed(); };
    int getAnzPrevSteps() { return m_prevSteps.getUsed(); };
    int getAnzStepAction(){ return m_stepActionBlocks.getUsed(); };

    int  getFollowTrans                 (int nr) { return m_followTrans.get(nr);};
    int  getAltTrans                    (int nr) { return m_altTrans.get(nr);};
    int  getNextSteps                   (int nr) { return m_nextSteps.get(nr);};
    int  getNextStepsOffset             (int nr) { return m_nextStepsOffset.get(nr);};
    int  getPrevSteps                   (int nr) { return m_prevSteps.get(nr);};
    int  getPrevStepsOffset             (int nr) { return m_prevStepsOffset.get(nr);};
    int  getStepActionBlockOffset       (int nr) { return m_stepActionBlockOffset.get(nr);};
    int  getStepActionBlocks            (int nr) { return m_stepActionBlocks.get(nr);};
    int  getStepActionBlockQualifier    (int nr) { return m_stepActionBlockQualifier.get(nr);};
    long getStepActionBlockTimeParameter(int nr) { return m_stepActionBlockTimeParameter.get(nr);};



    int getFollowTransArray                 (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_followTrans.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_followTrans.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getAltTransArray                    (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_altTrans.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_altTrans.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getNextStepsArray                   (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_nextSteps.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_nextSteps.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getNextStepsOffsetArray             (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_nextStepsOffset.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_nextStepsOffset.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getPrevStepsArray                   (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_prevSteps.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_prevSteps.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getPrevStepsOffsetArray             (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_prevStepsOffset.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_prevStepsOffset.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getStepActionBlockOffsetArray       (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_stepActionBlockOffset.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_stepActionBlockOffset.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getStepActionBlocksArray            (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_stepActionBlocks.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_stepActionBlocks.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getStepActionBlockQualifierArray    (long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_stepActionBlockQualifier.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_stepActionBlockQualifier.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }
    int getStepActionBlockTimeParameterArray(long* pArr/*[nElems]*/, int nElems)
    {
        int n = m_stepActionBlockTimeParameter.getUsed();
        if(n<=nElems)
            memcpy(pArr, m_stepActionBlockTimeParameter.getCElements(), sizeof(long)*n);
        return n<=nElems;
    }






    int  isStep   (const TCHAR* pName);
    int  isAction (const TCHAR* pName);
    
    const TCHAR*     getStepName     (int nr);
    const TCHAR*     getActionName   (int nr);  
    const TCHAR*     getTransName    (int nr);
    const CG_Edipos* getStepEdiPos   (int nr);
    const CG_Edipos* getActionEdiPos (int nr);
    const CG_Edipos* getTransEdiPos  (int nr);

#if defined(DEBUG) && defined(SFC_DEBUGCODE)
    // some output, just for debugging
    void output();
	void outputGraph();
    void outputInterpreter();
    void outputColor();
#endif

private:
    // search a string in an string list
    int searchStringList(const TCHAR *name, FC_CStrList* sl);
    int countPresOfStringInList(const TCHAR *name, FC_CStrList *sl, int endPos = -1);

    // fill a part of a sfc graph with color (simDepth, simBranch)
    SFCColorReturn colorInit(int nodeNr, 
                             SFCColor color);
    SFCColorReturn colorStraight(int nodeNr,
                                 SFCColor color);
    SFCColorReturn colorAlt(int nodeNr,
                            SFCColor color,
                            bool simCall);
    SFCColorReturn colorSim(int nodeNr,
                            SFCColor color);

private:
    // textual representation of the sfc
    CGT_CDiagMsg*   m_pdm;              //error message sink

    FC_CStr         m_sfcName;          // name of the sfc  
    FC_CStrList     m_initialStepName;  // name of the initial step
    FC_CStrList     m_stepNames;        // name of all steps (including init step)

    FC_CGArray <SFCTransition> m_transition; // the parameter to all the transitions
    FC_CStrList m_transFromList;    // the fromLists of all Transitions packed in one array
    FC_CStrList m_transToList;      // the toLists of all Transitions packed in one array
    FC_CStrList m_transitionNames;  // names of transitions
    FC_CGArray <bool> m_isTransitionNamed;
    
    FC_CStrList m_actionNames;      // names of all actions

    FC_CStrList m_aBlockStepName;   // to each action block the name of the associated step
    FC_CStrList m_aBlockAction;     // the name of the associated action
    FC_CStrList m_aBlockQualifier;  // the qualifier set
    FC_CGArray <long> m_aBlockTimePara;  // and the optional time qualifier

    FC_CGArray <CG_Edipos> m_ediPosStep;
    FC_CGArray <CG_Edipos> m_ediPosTransition;
    FC_CGArray <CG_Edipos> m_ediPosAction;
    FC_CGArray <CG_Edipos> m_ediPosActionBlock;
    FC_CGArray <CG_Edipos> m_ediPosFromList;
    FC_CGArray <CG_Edipos> m_ediPosToList;

    CG_Edipos m_endOfFile;

    // representation to do some checks
    int m_initialStep;                  // number of the init step
    FC_CGArray <SFCElement> m_sfcGraph;     // graph representation (numbers) of the sfc
    int m_numSteps;                     // number of steps
    int m_numTrans;                     // number of transitions
    int m_numActions;

    FC_CGArray <SFCColor> m_sfcColor;       // array to color sfc graph

    int m_nextSimColor;                 // color to use after next SimDiv

    // number representation of the sfc
    FC_CGArray <int> m_followTrans;         // one transition following a step 
    FC_CGArray <int> m_altTrans;            // to each transition a alternative transition
    FC_CGArray <int> m_nextSteps;           // all steps following the transitions
    FC_CGArray <int> m_nextStepsOffset;     // offset in the array
    FC_CGArray <int> m_prevSteps;           // all steps proceeding the transitions
    FC_CGArray <int> m_prevStepsOffset;     // offset in the array
    FC_CGArray <int> m_stepActionBlockOffset;
    FC_CGArray <int> m_stepActionBlocks;
    FC_CGArray <int> m_stepActionBlockQualifier;
    FC_CGArray <long> m_stepActionBlockTimeParameter;
    
};





// ==================================================
//  Implementation
// ==================================================

// addSFCName
bool SFC::addSFCName(const TCHAR* name,
                     const CG_Edipos* ppos)
{
    m_sfcName.load(name);
    
    return true;
}



// addInitialStep
bool SFC::addInitialStep(const TCHAR* name,
                         const CG_Edipos* ppos)
{
    m_initialStepName.add(name);
    addStep(name,ppos);
    
    return true;
}



// addStep
bool SFC::addStep(const TCHAR* name,
                  const CG_Edipos* ppos)
{
    m_stepNames.add(name);
    m_ediPosStep.add(*ppos);
    
    return true;
}



// addTransition
bool SFC::addTransition(int prio, 
                        const FC_CStrList* fromList, 
                        const FC_CStrList* toList,
                        bool fromSim, 
                        bool toSim,
                        const CG_Edipos* ppos,
                        FC_CGArray<CG_Edipos> *pfromPos,
                        FC_CGArray<CG_Edipos> *ptoPos)
{
    SFCTransition trans;

    trans.prio = prio;
    trans.fromSim = fromSim;
    trans.fromStartOffset = m_transFromList.getUsed();
    trans.fromEndOffset = m_transFromList.getUsed()+fromList->getUsed();
    trans.toSim = toSim;
    trans.toStartOffset = m_transToList.getUsed();
    trans.toEndOffset = m_transToList.getUsed()+toList->getUsed();

    m_transition.add(trans);
    m_transFromList.addStrList(fromList);
    m_transToList.addStrList(toList);

    m_ediPosTransition.add(*ppos);

    for (int i=0; i<pfromPos->getUsed(); i++)
    {
        m_ediPosFromList.add(pfromPos->get(i));
    }
    for (i=0; i<ptoPos->getUsed(); i++)
    {
        m_ediPosToList.add(ptoPos->get(i));
    }

    return true;
}



bool SFC::addTransitionName(const TCHAR* name)
{
    m_transitionNames.add(name);
    m_isTransitionNamed.add(true);
    return true;
}

bool SFC::addTransitionNoName()
{
    m_transitionNames.add(_T(""));
    m_isTransitionNamed.add(false);
    return true;
}



// addActionBlock
bool SFC::addActionBlock(const TCHAR* stepName, 
                         const TCHAR* actionName, 
                         const TCHAR* qualifier,
                         const long timeParam,
                         const CG_Edipos* ppos)
{
    m_aBlockStepName.add(stepName);
    if (!_tcscmp(_T(""), qualifier))
        m_aBlockQualifier.add(_T("N"));
    else
        m_aBlockQualifier.add(qualifier);
    m_aBlockAction.add(actionName);
    m_aBlockTimePara.add(timeParam);
    assert( (m_aBlockStepName.getUsed() == m_aBlockAction.getUsed()) &&
              (m_aBlockAction.getUsed() == m_aBlockQualifier.getUsed()) &&
              (m_aBlockQualifier.getUsed() == m_aBlockTimePara.getUsed()) );
    
    m_ediPosActionBlock.add(*ppos);

    return true;
}



// add Action
bool SFC::addAction(const TCHAR* name,
                    const CG_Edipos* ppos)
{
    m_actionNames.add(name);
    m_ediPosAction.add(*ppos);
    
    return true;
}



// searchStringList
int SFC::searchStringList(const TCHAR *name, FC_CStrList *sl)
{
    int pos = 0;
    
    while (pos<sl->getUsed())
    {
        const TCHAR* psz;
        psz = sl->get(pos);
        if (!_tcsicmp(psz, name))
            return pos;
        pos++;
    }
    
    return -1;
}


// count how often a string exists in a string list
int SFC::countPresOfStringInList(const TCHAR *name, FC_CStrList *sl, int endPos)
{
    int anz = 0;
    int pos = 0;

    if (endPos == -1)
        endPos = sl->getUsed();
    
    while ((pos<sl->getUsed()) && (pos<endPos))
    {
        const TCHAR* psz;
        psz = sl->get(pos);
        if (!_tcsicmp(psz, name))
            anz++;
        pos++;
    }
    
    return anz;
}



// createGraph
bool SFC::createGraph(const CG_Edipos *pEndOfFile)
{
    int i;
    int pos;

    int cerror = 0;
    
    FC_CGArray<int> succStepsNr;
    FC_CGArray<int> predStepsNr;

    m_endOfFile = *pEndOfFile;

    if (m_initialStepName.getUsed()==0)
    {
        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_endOfFile,
            m_pdm->res1(CG_RES_STR_SFC_NO_INISTEP),
            _T(""), _T(""));
        cerror++;
    }
    else 
    {
        pos = searchStringList(m_initialStepName.get(0), &m_stepNames);
        if (m_initialStepName.getUsed()>1)
        {
            for (int mis = 1; mis<m_initialStepName.getUsed(); mis++)
            {
                // multiple init steps: error or probably only not supported
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER,
                    &m_ediPosStep.get(searchStringList(m_initialStepName.get(mis),&m_stepNames)),
                    m_pdm->res1(CG_RES_STR_SFC_MULTI_INISTEP),
                    _T(""), _T(""));
                cerror++;
            }
        }
    }

    // check if all stepNames ActionNames and TransitionNames exist only one time
    for (i=0; i<m_stepNames.getUsed(); i++)
    {
        if (!m_stepNames.get(i)[0])
        {
            m_pdm->msg1(CG_E_SYNTAX_MISSING, &m_ediPosStep.get(i), 
                m_pdm->res1(CG_RES_STR_STEP_NAME));
            cerror++;
        }
        else
        {
            if (countPresOfStringInList(m_stepNames.get(i), &m_stepNames, i) > 0)
            {
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosStep.get(i), 
                    m_pdm->res1(CG_RES_STR_STEP)<<_T(": '")<<m_stepNames.get(i)<<_T("'"));
                cerror++;
            }
        }
    }
    for (i=0; i<m_transitionNames.getUsed(); i++)
    {
        if (m_isTransitionNamed.get(i))
        {
            if (countPresOfStringInList(m_transitionNames.get(i), &m_transitionNames, i) > 0)
            {   
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosTransition.get(i),
                     m_pdm->res1(CG_RES_STR_TRANS)<<_T(": '")<<m_transitionNames.get(i)<<_T("'"));
                cerror++;
            }
            if (countPresOfStringInList(m_transitionNames.get(i), &m_stepNames, -1) > 0)
            {
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosTransition.get(i), 
                    m_pdm->res1(CG_RES_STR_TRANS)<<_T(": '")<<m_transitionNames.get(i)<<
                    _T("' ")<<m_pdm->res2(CG_RES_STR_STEP_ALREADY_DEF));
                cerror++;
            }
        }
    }
    for (i=0; i<m_actionNames.getUsed(); i++)
    {
        if (!m_actionNames.get(i)[0])
        {
            m_pdm->msg1(CG_E_SYNTAX_MISSING, &m_ediPosAction.get(i), 
                m_pdm->res1(CG_RES_STR_ACTBL_NAME));
            cerror++;
        }
        else
        {
            if (countPresOfStringInList(m_actionNames.get(i), &m_actionNames, i) > 0)
            {
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosAction.get(i), 
                     m_pdm->res1(CG_RES_STR_ACTBL)<<_T(": '")<<m_actionNames.get(i)<<_T("'"));
                cerror++;
            }
            if (countPresOfStringInList(m_actionNames.get(i), &m_stepNames, -1) > 0)
            {
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosAction.get(i), 
                    m_pdm->res1(CG_RES_STR_ACTBL)<<_T(": '")<<m_actionNames.get(i)<< 
                    _T("' ")<<m_pdm->res2(CG_RES_STR_STEP_ALREADY_DEF));
                cerror++;
            }
            if (countPresOfStringInList(m_actionNames.get(i), &m_transitionNames, -1) > 0)
            {
                m_pdm->msg1(CG_E_SFC_REDEF, &m_ediPosAction.get(i), 
                    m_pdm->res1(CG_RES_STR_ACTBL)<<_T(": '")<<m_actionNames.get(i)<< 
                       _T("' ")<<m_pdm->res2(CG_RES_STR_TRANS_ALREADY_DEF));
                cerror++;
            }
        }
    }


    // initialize graph
    // fill with nodes with no connection
    // first all steps
    m_sfcGraph.resetUsed();
    m_numSteps = m_stepNames.getUsed();

    for (i=0; i<m_numSteps; i++)
    {
        SFCElement elem;
        elem.nr        = i;
        elem.typ       = SFCtStep;
        elem.pred      = -1;
        elem.succ      = -1;
        elem.predLeft  = -1;
        elem.predRight = -1;
        elem.succLeft  = -1;
        elem.succRight = -1;
        m_sfcGraph.add(elem);
    }
    // and all transitions
    m_numTrans = m_transition.getUsed();
    for (i=0; i<m_numTrans; i++)
    {
        SFCElement elem;
        elem.nr        = i;
        elem.typ       = SFCtTrans;
        elem.pred      = -1;
        elem.succ      = -1;
        elem.predLeft  = -1;
        elem.predRight = -1;
        elem.succLeft  = -1;
        elem.succRight = -1;
        m_sfcGraph.add(elem);
    }

    int lerror = 0;

    // add links between nodes
    for (i=0; i<m_numTrans; i++)
    {
        SFCTransition trans = m_transition.get(i);
        int transGraphPos   = i + m_numSteps;
        int predAnz         = trans.fromEndOffset-trans.fromStartOffset;
        int succAnz         = trans.toEndOffset-trans.toStartOffset;
        lerror = cerror;
        
        // error cases
        if (predAnz == 0)   
        {
            //printErrorMsg("A transition must have at least one successor and "
            //    "one predecessor step: Transition ", i);
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosTransition.get(i),
                m_pdm->res1(CG_RES_STR_TRANS_NOPRED), _T(""), _T(""));
            cerror++;
        }
        if (succAnz == 0)
        {
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosTransition.get(i),
                m_pdm->res1(CG_RES_STR_TRANS_NOSUCC), _T(""), _T(""));
            cerror++;
        }
        if ((predAnz == 1) && trans.fromSim)
        {
            //printErrorMsg("Transition has only one predecessor "
            //    "step connected via simultaneous link: Transition ", i);
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosTransition.get(i),
                m_pdm->res1(CG_RES_STR_SIMCONV_ONEPRED), _T(""), _T(""));
            cerror++;
        }
        if ((succAnz > 1) && !trans.toSim)
        {
            //printErrorMsg("Transition has multiple successor "
            //    "steps connected via alternative link: Transition ", i);
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER,&m_ediPosTransition.get(i),
                m_pdm->res1(CG_RES_STR_ALTDIV_MULTSUCC), _T(""), _T(""));
            cerror++;
        }
        if ((succAnz == 1) && trans.toSim)
        {
            //printErrorMsg("Transition has only one successor "
            //    "step connected via simultaneous link: Transition ", i);
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER,&m_ediPosTransition.get(i),
                m_pdm->res1(CG_RES_STR_SIMDIV_ONESUCC), _T(""), _T(""));
            cerror++;
        }
        // remove this to allow special altcon with transition after altcon
        if ((predAnz > 1) && !trans.fromSim)
        {
            m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosTransition.get(i), 
                m_pdm->res1(CG_RES_STR_ALTCONV_MULTPRED), _T(""), _T(""));
            cerror++;
        }

        // error continue with next transition
        if (cerror>lerror)
            continue;

        // find the nr of succ and pred steps
        predStepsNr.resetUsed();
        succStepsNr.resetUsed();

        for (int pi=trans.fromStartOffset; pi<trans.fromEndOffset; pi++)
        {
            int sNr = searchStringList(m_transFromList.get(pi), &m_stepNames);
            if (sNr == -1)
            {
                m_pdm->msg1(CG_E_SFC_UNDEF, &m_ediPosFromList.get(pi),
                          m_pdm->res1(CG_RES_STR_STEP)<<_T(": '")<<m_transFromList.get(pi)<<_T("'"));
                cerror++;
            }
            else
            {
                for (int ii=0; ii<predStepsNr.getUsed(); ii++)
                {
                    if (predStepsNr.get(ii) == sNr)
                    {
                        m_pdm->msg1(CG_E_SFC_STEP_MULT_REF,&m_ediPosFromList.get(pi),
                                    m_transFromList.get(pi));
                        cerror++;
                    }
                }
                predStepsNr.add(sNr);
            }
        }
        for (int si=trans.toStartOffset; si<trans.toEndOffset; si++)
        {
            int sNr = searchStringList(m_transToList.get(si), &m_stepNames);
            if (sNr == -1)
            {
                m_pdm->msg1(CG_E_SFC_UNDEF, &m_ediPosToList.get(si),
                    m_pdm->res1(CG_RES_STR_STEP)<<_T(": '")<<m_transToList.get(si)<<_T("'"));
                cerror++;
            }
            else
            {
                for (int ii=0; ii<succStepsNr.getUsed(); ii++)
                {
                    if (succStepsNr.get(ii) == sNr)
                    {
                        m_pdm->msg1(CG_E_SFC_STEP_MULT_REF, &m_ediPosToList.get(si),
                            m_transToList.get(si));
                        cerror++;
                    }
                }
                succStepsNr.add(sNr);
            }

        }

        // error in this transition look at next transition
        if (cerror > lerror)
            continue;

        // predecessor simultaneous convergence (9)
        if (trans.fromSim)
        {            
            SFCElement transElem;
            SFCElement simCon;
            int        simConPos;

            transElem = m_sfcGraph.get(transGraphPos);

            // add sim convergence node to graph (succ = trans, preds = all pred steps of trans)
            simConPos        = m_sfcGraph.getUsed();
            simCon.nr        = simConPos;
            simCon.typ       = SFCtSimCon;
            simCon.succ      = transGraphPos;
            simCon.pred      = predStepsNr.get(0);
            simCon.predLeft  = -1;
            simCon.predRight = -1;
            simCon.succLeft  = -1;
            simCon.succRight = -1;

            m_sfcGraph.add(simCon);

            // correct pred steps entries
            for (int pst=0; pst<predStepsNr.getUsed(); pst++) 
            {
                SFCElement step = m_sfcGraph.get(predStepsNr.get(pst));
                if (step.succ!=-1)
                {
                    //printErrorMsg("Step has multiple successors, step nr: ", step.nr);
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER,
                        &m_ediPosFromList.get(trans.fromStartOffset+pst), 
                        m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<m_transFromList.get(trans.fromStartOffset+pst)<<_T("' "),
                        m_pdm->res2(CG_RES_STR_ALREADY_PRED), _T(""));
                    cerror++;
                    continue;
                }
                step.succ = simConPos;
                if (pst>0)
                    step.succLeft = predStepsNr.get(pst-1);
                if (pst<predStepsNr.getUsed()-1)
                    step.succRight = predStepsNr.get(pst+1);
                m_sfcGraph.set(predStepsNr.get(pst),step);
            }

            // correct trans entries
            transElem.pred = simConPos;
            m_sfcGraph.set(transGraphPos, transElem);
        }

        // successor simultaneous divergence (10)
        if (trans.toSim)
        {
            SFCElement transElem;
            SFCElement simDiv;
            int        simDivPos;

            transElem = m_sfcGraph.get(transGraphPos);

            // add sim divergence node to graph (pred = trans, succ = all succ steps of trans)
            simDivPos        = m_sfcGraph.getUsed();
            simDiv.nr        = simDivPos;
            simDiv.typ       = SFCtSimDiv;
            simDiv.succ      = succStepsNr.get(0);
            simDiv.pred      = transGraphPos;
            simDiv.predLeft  = -1;
            simDiv.predRight = -1;
            simDiv.succLeft  = -1;
            simDiv.succRight = -1;

            m_sfcGraph.add(simDiv);

            // correct succ steps entries
            for (int sst=0; sst<succStepsNr.getUsed(); sst++) {
                SFCElement step = m_sfcGraph.get(succStepsNr.get(sst));
                //assert(step.pred==-1);
                if (step.pred != -1)
                {
                    //printErrorMsg("Step has multiple predecessors, step nr: ",step.nr);
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, 
                        &m_ediPosToList.get(trans.toStartOffset+sst), 
                        m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<m_transToList.get(trans.toStartOffset+sst)<<_T("' "),
                        m_pdm->res2(CG_RES_STR_ALREADY_SUCC), _T("")); //succStepsNr.get(sst)
                    cerror++;
                    continue;
                }
                step.pred = simDivPos;
                if (sst>0)
                    step.predLeft = succStepsNr.get(sst-1);
                if (sst<succStepsNr.getUsed()-1)
                    step.predRight = succStepsNr.get(sst+1);
                m_sfcGraph.set(succStepsNr.get(sst),step);
            }

            // correct trans entries
            transElem.succ = simDivPos;
            m_sfcGraph.set(transGraphPos, transElem);   
        }

        //  one or more predecessor steps via direct or alternative link (1,3,5)
        if (!trans.fromSim)
        {
            SFCElement transElem;
            transElem = m_sfcGraph.get(transGraphPos);

            SFCElement step = m_sfcGraph.get(predStepsNr.get(0));

            // has transition multiple pred steps -> 5 
            if (predStepsNr.getUsed()>1)
            {
                // has there already been inserted a special trans-step sequence
                if (step.succ != -1) {
                    // get the special step after the inserted spec. alt conv
                    step = m_sfcGraph.get(
                            m_sfcGraph.get(
                                m_sfcGraph.get(
                                    step.succ           // spec trans
                                ).succ                  // altCon
                            ).succ);                    // spec step

                    // is it a AltCon
                    if (m_sfcGraph.get(m_sfcGraph.get(step.succ).succ).typ != SFCtAltCon)
                    {
                        m_pdm->msg3(CG_E_SFC_ELEM_ORDER, 
                            &m_ediPosFromList.get(trans.fromStartOffset+0), 
                            m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<m_transFromList.get(trans.fromStartOffset+0)<<_T("' "),
                            m_pdm->res2(CG_RES_STR_ALREADY_PRED), _T(""));
                        cerror++;
                        continue;
                    }
                }
                else  // if not insert alt conv and special trans and step
                {
                    SFCElement altCon;
                    SFCElement altTrans;
                    SFCElement speStep;

                    int altConPos = m_sfcGraph.getUsed();
                    
                    m_sfcGraph.add(altCon);

                    // for all pred steps a new spec trans
                    for (int pst=0; pst<predStepsNr.getUsed(); pst++)
                    {
                        altTrans.nr        = m_sfcGraph.getUsed();
                        altTrans.typ       = SFCtTrans;
                        altTrans.pred      = predStepsNr.get(pst);
                        altTrans.succ      = altConPos;
                        altTrans.predLeft  = -1;
                        altTrans.predRight = -1;
                        altTrans.succLeft  = -1;
                        altTrans.succRight = -1;
                        if (pst>0)
                            altTrans.succLeft = altTrans.nr - 1;
                        if (pst<predStepsNr.getUsed()-1)
                            altTrans.succRight = altTrans.nr + 1;

                        m_sfcGraph.add(altTrans);

                        SFCElement preStep = m_sfcGraph.get(predStepsNr.get(pst));
                        preStep.succ = altTrans.nr;
                        m_sfcGraph.set(predStepsNr.get(pst), preStep);
                    }

                    speStep.nr        = m_sfcGraph.getUsed();
                    speStep.typ       = SFCtStep;
                    speStep.pred      = altConPos;
                    speStep.succ      = -1; //transGraphPos;
                    speStep.succLeft  = -1;
                    speStep.succRight = -1;
                    speStep.predLeft  = -1;
                    speStep.predRight = -1;

                    m_sfcGraph.add(speStep);

                    altCon.nr        = altConPos;
                    altCon.typ       = SFCtAltCon;
                    altCon.pred      = altTrans.nr;
                    altCon.succ      = speStep.nr;
                    altCon.succLeft  = -1;
                    altCon.succRight = -1;
                    altCon.predLeft  = -1;
                    altCon.predRight = -1;

                    m_sfcGraph.set(altConPos, altCon);

                    step = speStep;
                }
            }

            // step is pointing to exactly one pred step of the trans
            
            // three cases: type of succ of the step -1 , trans, altdiv
            // succ of step: -1
            if (step.succ == -1)
            {
                step.succ = transGraphPos;
                m_sfcGraph.set(step.nr, step);

                transElem.pred = step.nr;
                m_sfcGraph.set(transGraphPos, transElem);
            }
            // succ of step: trans
            else if (m_sfcGraph.get(step.succ).typ == SFCtTrans)
            {
                SFCElement altTrans = m_sfcGraph.get(step.succ);
                SFCElement altDiv;
                int altDivPos = m_sfcGraph.getUsed();

                altDiv.nr        = altDivPos;
                altDiv.typ       = SFCtAltDiv;
                altDiv.pred      = step.nr;
                altDiv.succ      = altTrans.nr + m_numSteps;
                altDiv.predLeft  = -1;
                altDiv.predRight = -1;
                altDiv.succLeft  = -1;
                altDiv.succRight = -1;

                m_sfcGraph.add(altDiv);

                step.succ = altDivPos;
                m_sfcGraph.set(step.nr, step);

                altTrans.pred = altDivPos;
                altTrans.predRight = transGraphPos;

                m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);

                transElem.pred = altDivPos;
                transElem.predLeft = altTrans.nr + m_numSteps;

                m_sfcGraph.set(transGraphPos, transElem);

            }
            // succ of step: altDiv
            else if (m_sfcGraph.get(step.succ).typ == SFCtAltDiv)
            {
                SFCElement altTrans = m_sfcGraph.get(m_sfcGraph.get(step.succ).succ);
                transElem.predLeft = altTrans.nr + m_numSteps;
                transElem.predRight = altTrans.predRight;
                transElem.pred = step.succ;
                altTrans.predRight = transGraphPos;

                m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);

                if (transElem.predRight != -1) 
                {
                    altTrans = m_sfcGraph.get(transElem.predRight);
                    altTrans.predLeft = transGraphPos;

                    m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);
                }

                m_sfcGraph.set(transGraphPos, transElem);
            }
            else 
            {
                //printErrorMsg("Something wrong after filled in special step near transition ", 
                //    transElem.nr);
                m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosFromList.get(trans.fromStartOffset+0), 
                    m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<m_transFromList.get(trans.fromStartOffset+0)<<_T("' "),
                    m_pdm->res2(CG_RES_STR_ALREADY_PRED), _T(""));
                cerror++;
            }

        }

        // successor step of a transition via direct link (2,4)
        if (!trans.toSim)
        {
            SFCElement transElem;
            transElem = m_sfcGraph.get(transGraphPos);

            assert(succStepsNr.getUsed()==1);
            
            SFCElement step = m_sfcGraph.get(succStepsNr.get(0));
            
            // (2) there is no other pred transition of the step 
            if (step.pred == -1)
            {
                step.pred = transGraphPos;
                transElem.succ = step.nr;                
            }
            else   // (4) the step has already another pred trans -> alt con
            {
                // if succ of step is already alt con -> just add trans to alt con
                if (m_sfcGraph.get(step.pred).typ == SFCtAltCon)
                {
                    SFCElement altTrans = m_sfcGraph.get(m_sfcGraph.get(step.pred).pred);
                    
                    transElem.succLeft = altTrans.nr + m_numSteps;
                    transElem.succRight = altTrans.succRight;
                    altTrans.succRight = transElem.nr + m_numSteps;

                    m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);

                    altTrans = m_sfcGraph.get(transElem.succRight);
                    altTrans.succLeft = transGraphPos;
                    
                    m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);

                    transElem.succ = m_sfcGraph.get(step.pred).nr;

                }  // else add new alt conv
                else if (m_sfcGraph.get(step.pred).typ == SFCtTrans)
                {
                    SFCElement altTrans = m_sfcGraph.get(step.pred);
                    SFCElement altCon;
                    int altConPos;

                    altConPos        = m_sfcGraph.getUsed();
                    altCon.nr        = altConPos;
                    altCon.typ       = SFCtAltCon;
                    altCon.pred      = altTrans.nr + m_numSteps;
                    altCon.succ      = step.nr;
                    altCon.predLeft  = -1;
                    altCon.predRight = -1;
                    altCon.succLeft  = -1;
                    altCon.succRight = -1;

                    m_sfcGraph.add(altCon);

                    altTrans.succ = altConPos;
                    altTrans.succRight = transGraphPos;
                    
                    m_sfcGraph.set(altTrans.nr + m_numSteps, altTrans);

                    step.pred = altConPos;
                    transElem.succ = altConPos;
                    transElem.succLeft = altTrans.nr + m_numSteps;

                }
                else if (m_sfcGraph.get(step.pred).typ == SFCtSimDiv)
                {
                    SFCElement altCon;
                    int altConPos = -1;

                    for (int sacd=0; sacd<m_sfcGraph.getUsed(); sacd++)
                    {
                        if (m_sfcGraph.get(sacd).typ == SFCtAltCon && 
                            m_sfcGraph.get(sacd).succ == step.nr)
                        {
                            altConPos = sacd;
                            break;
                        }
                    }

                    if (altConPos == -1)
                    {
                        altConPos        = m_sfcGraph.getUsed();
                        altCon.nr        = altConPos;
                        altCon.typ       = SFCtAltCon;
                        altCon.pred      = transGraphPos;
                        altCon.succ      = step.nr;
                        altCon.predLeft  = -1;
                        altCon.predRight = -1;
                        altCon.succLeft  = -1;
                        altCon.succRight = -1;

                        m_sfcGraph.add(altCon);

                        transElem.succ = altConPos;
                    }
                    else
                    {
                        int rm;
                        altCon = m_sfcGraph.get(altConPos);
                        SFCElement altTrans = m_sfcGraph.get(altCon.pred);
                        
                        rm = altTrans.succRight;
                        altTrans.succLeft = transGraphPos;
                        m_sfcGraph.set(altCon.pred, altTrans);
                        transElem.succLeft = altCon.pred;
                        transElem.succRight = rm;
                        if (rm!=-1)
                        {
                            altTrans = m_sfcGraph.get(rm);
                            altTrans.succLeft = transGraphPos;
                            m_sfcGraph.set(rm, altTrans);
                        }

                        m_sfcGraph.set(altConPos, altCon);
                        transElem.succ = altConPos;
                    }
                }
                else 
                {
                    //printErrorMsg("Step has already another pred, stepNr: ",step.nr);
                    m_pdm->msg3(CG_E_SFC_ELEM_ORDER, &m_ediPosToList.get(trans.toStartOffset+0), 
                            m_pdm->res1(CG_RES_STR_STEP)<<_T(" '")<<m_transToList.get(trans.toStartOffset+0)<<_T("' "),
                            m_pdm->res2(CG_RES_STR_ALREADY_SUCC), _T(""));
                    cerror++;
                    continue;
                }

            }

            m_sfcGraph.set(succStepsNr.get(0), step);
            m_sfcGraph.set(transGraphPos, transElem);
        }
    }

    if (cerror == 0)
        return true;
    else 
        return false;
}






// codeGen
bool SFC::codeGen()
{
    int cerror = 0;

    m_followTrans.resetUsed();
    m_altTrans.resetUsed();
    m_nextStepsOffset.resetUsed();
    m_prevStepsOffset.resetUsed();
    m_nextSteps.resetUsed();
    m_prevSteps.resetUsed();
    m_stepActionBlockOffset.resetUsed();
    m_stepActionBlocks.resetUsed();
    m_stepActionBlockQualifier.resetUsed();
    m_stepActionBlockTimeParameter.resetUsed();


    m_numActions = m_actionNames.getUsed();
    
    assert(m_initialStepName.getUsed() == 1);

    m_initialStep = searchStringList(m_initialStepName.get(0), &m_stepNames);
    
    for (int i1=0; i1<m_numSteps; i1++)
    {
        m_followTrans.add(-1);
    }
    for (int i2=0; i2<m_numTrans; i2++)
    {
        m_altTrans.add(-1);
        m_nextStepsOffset.add(0);
        m_prevStepsOffset.add(0);
    }
    m_prevSteps.add(-1);
    m_nextSteps.add(-1);

    // fill followTrans and altTrans
    for (int sti=0; sti<m_numSteps; sti++)
    {
        SFCElement step = m_sfcGraph.get(sti);
        if (step.succ == -1) continue;
        SFCElement follow = m_sfcGraph.get(step.succ);

        // 4 cases: followT is altdiv, trans, spec trans, simcon
        assert((follow.typ == SFCtTrans) 
              || (follow.typ == SFCtAltDiv)
              || (follow.typ == SFCtSimCon));
        // trans
        if ((follow.typ == SFCtTrans) && (follow.nr <= m_numSteps+m_numTrans))
        {
            m_followTrans.set(sti, follow.nr);
        }
        // simCon
        else if (follow.typ == SFCtSimCon)
        {
            m_followTrans.set(sti, m_sfcGraph.get(m_sfcGraph.get(follow.nr).succ).nr);
        }
        else      // altdiv , spec trans
        {
            // special sequence, skip to altdiv after that spec
            // and continue in the same way for both cases
            if (follow.typ != SFCtAltDiv)
            {
               follow = m_sfcGraph.get(
                      m_sfcGraph.get(
                          follow.succ           // spec alt con
                      ).succ);                   // spec step
               if (m_sfcGraph.get(follow.succ).typ == SFCtAltDiv)    // spec alt div ?
               {
                   follow = m_sfcGraph.get(follow.succ);
               }
            }
            
            // construct two array with all following transitions (nr, prio)
            FC_CGArray<int> fTransNr;
            //FC_CGArray<int> fTransPrio;
            int nextT = -1;

            fTransNr.resetUsed();
            //fTransPrio.resetUsed();

            SFCElement at = m_sfcGraph.get(follow.succ);
            assert(at.typ == SFCtTrans);

            fTransNr.add(at.nr);
            //fTransPrio.add(m_transition.get(at.nr).prio);
            
            nextT = at.predRight;
            while (nextT != -1)
            {
                fTransNr.add(m_sfcGraph.get(nextT).nr);
                //fTransPrio.add(m_transition.get(m_sfcGraph.get(nextT).nr).prio);
                nextT = m_sfcGraph.get(nextT).predRight;
            }

            // sort follow transitions (priority, left to right, top to bottom)
            for(int s1=0; s1<fTransNr.getUsed()-1; s1++)
            {
                for (int s2=s1+1; s2<fTransNr.getUsed(); s2++)
                {
                    int s1Nr = fTransNr.get(s1);
                    int s2Nr = fTransNr.get(s2);
                    
                    int s1Prio = m_transition.get(s1Nr).prio;
                    int s2Prio = m_transition.get(s2Nr).prio;

                    if (s2Prio<s1Prio)
                    {
                        fTransNr.set(s1, s2Nr);
                        fTransNr.set(s2, s1Nr);
                    }
                    else if (s2Prio==s1Prio)
                    {
                        // look at edi pos
                        long ediPos1x = GetBoxXFromEdp(m_ediPosTransition.getAddrOf(s1Nr));
                        long ediPos1y = GetBoxYFromEdp(m_ediPosTransition.getAddrOf(s1Nr));
                        long ediPos2x = GetBoxXFromEdp(m_ediPosTransition.getAddrOf(s2Nr));
                        long ediPos2y = GetBoxYFromEdp(m_ediPosTransition.getAddrOf(s2Nr));
                        if (ediPos2y<ediPos1y
                            || (ediPos2y==ediPos1y && ediPos2x<ediPos1x))
                        {
                            fTransNr.set(s1, s2Nr);
                            fTransNr.set(s2, s1Nr);
                        }
                    }
                }
            }

            int lastT = -1;
            nextT = 0;
            m_followTrans.set(sti, fTransNr.get(0));
            lastT = fTransNr.get(0);
            for (int i=1; i<fTransNr.getUsed(); i++)
            {
                nextT = fTransNr.get(i);
                m_altTrans.set(lastT, nextT);
                lastT = nextT;
            }       
        }  // end altdiv
    }



    // fill next and prev steps arrays
    for (int tri=0; tri<m_numTrans; tri++)
    {
        SFCElement trans = m_sfcGraph.get(tri+m_numSteps);
   
        // next arrays
     
        SFCElement follow = m_sfcGraph.get(trans.succ);
        m_nextStepsOffset.set(tri, m_nextSteps.getUsed());

        // 4 cases : follow = step, simdiv, altcon
        if (follow.typ == SFCtStep)
        {
            m_nextSteps.add(follow.nr);    
        }
        else if (follow.typ == SFCtAltCon)
        {
            m_nextSteps.add(m_sfcGraph.get(m_sfcGraph.get(follow.nr).succ).nr);
        }
        else   // simdiv
        {
            SFCElement fst = m_sfcGraph.get(follow.succ);
            m_nextSteps.add(fst.nr);
            while (fst.predLeft != -1)
            {
                fst = m_sfcGraph.get(fst.predLeft);
                m_nextSteps.add(fst.nr);
            }
            fst = m_sfcGraph.get(follow.succ);
            while (fst.predRight != -1)
            {
                fst = m_sfcGraph.get(fst.predRight);
                m_nextSteps.add(fst.nr);
            }
        }
        m_nextSteps.add(-1);

        // prev arrays

        SFCElement prev = m_sfcGraph.get(trans.pred);
        // this is only interesting, if there is a simcon before the transition
        if (prev.typ == SFCtSimCon)
        {
            m_prevStepsOffset.set(tri, m_prevSteps.getUsed());
            SFCElement pst = m_sfcGraph.get(prev.pred);
            m_prevSteps.add(pst.nr);
            while (pst.succRight != -1)
            {
                pst = m_sfcGraph.get(pst.succRight);
                m_prevSteps.add(pst.nr);
            }
            m_prevSteps.add(-1);
        }      
        else if (prev.typ == SFCtStep)
        {
            m_prevStepsOffset.set(tri, m_prevSteps.getUsed());
            m_prevSteps.add(prev.nr);
            m_prevSteps.add(-1);
        }
        else if (prev.typ == SFCtAltDiv)
        {
            m_prevStepsOffset.set(tri, m_prevSteps.getUsed());
            m_prevSteps.add(m_sfcGraph.get(prev.pred).nr);
            m_prevSteps.add(-1);
        }
    }

    // fill Action Block Array
    int acbOffset = 0;
    m_stepActionBlocks.add(-1);
    m_stepActionBlockQualifier.add(-1);
    m_stepActionBlockTimeParameter.add(-1);
    acbOffset++;

    FC_CGArray <bool> actionUsed;
    for (int i=0; i<m_numActions; i++)
    {
        actionUsed.add(false);
    }

    for (i=0; i<m_numSteps; i++)
    {
        bool found = false;
        for (int j=0; j<m_aBlockStepName.getUsed(); j++)
        {
            if (!_tcsicmp(m_stepNames.get(i), m_aBlockStepName.get(j)))
            {
                if (!found)
                {
                    m_stepActionBlockOffset.add(acbOffset);
                }
                found = true;

                CG_Edipos* pEdiPos = &(m_ediPosActionBlock.get(j)); 

                int actNr = searchStringList(m_aBlockAction.get(j), &m_actionNames);
                if (actNr == -1)
                {
                    m_pdm->msg1(CG_E_SFC_UNDEF, pEdiPos, 
                        m_pdm->res1(CG_RES_STR_ACTBL)<<_T(": '")<<m_aBlockAction.get(j)<<_T("'"));
                    cerror++;
                }
                else
                {
                    actionUsed.set(actNr, true);
                }
                
                int qualNr = getQualVal(m_aBlockQualifier.get(j), pEdiPos);
                if (qualNr == -1)
                {
                    m_pdm->msg1(CG_E_SFC_UNDEF, pEdiPos, 
                        m_pdm->res1(CG_RES_STR_QUALIFIER)<<_T(": '")<<m_aBlockQualifier.get(j)<<_T("'"));
                    cerror++;
                }

                int timePar = m_aBlockTimePara.get(j);

                m_stepActionBlocks.add(actNr);
                m_stepActionBlockQualifier.add(qualNr);
                m_stepActionBlockTimeParameter.add(timePar);
                acbOffset++;
            }
        }
        if (!found)
        {
            m_stepActionBlockOffset.add(0);
        }
        else 
        {
            m_stepActionBlocks.add(-1);
            m_stepActionBlockQualifier.add(-1);
            m_stepActionBlockTimeParameter.add(-1);
            acbOffset++;
        }
    }

    // check for unused actions
    for (i=0; i<m_numActions; i++)
    {
        if (!actionUsed.get(i))
        {
            const CG_Edipos* pEdiPos = &(m_ediPosAction.get(i));
            m_pdm->msg1(CG_W_UNUSED_ACTION, pEdiPos, m_actionNames.get(i));
        }
    }

    if (cerror>0)
        return false;
    else
        return true;
}


int  SFC::getQualVal(const TCHAR* pName, const CG_Edipos* pEdiPos)
{
    int ret = -1;
    bool timeQ = false;

    if (!_tcsicmp(pName, _T("N")))
        ret = SFC_Q_N;
    else if (!_tcsicmp(pName, _T("S")))
        ret = SFC_Q_S;
    else if (!_tcsicmp(pName, _T("R")))
        ret = SFC_Q_R;
    else if (!_tcsicmp(pName, _T("P")))
        ret = SFC_Q_P;
    else if (!_tcsicmp(pName, _T("P0")))
        ret = SFC_Q_P0;
    else if (!_tcsicmp(pName, _T("P1")))
        ret = SFC_Q_P1;
    else if (!_tcsicmp(pName, _T("D")))
    {
        ret = SFC_Q_D;
        timeQ = true;
    }
    else if (!_tcsicmp(pName, _T("L")))
    {
        ret = SFC_Q_L;
        timeQ = true;
    }
    else if (!_tcsicmp(pName, _T("SD")))
    {
        ret = SFC_Q_SD;
        timeQ = true;
    }
    else if (!_tcsicmp(pName, _T("DS")))
    {
        ret = SFC_Q_DS;
        timeQ = true;
    }
    else if (!_tcsicmp(pName, _T("SL")))
    {
        ret = SFC_Q_SL;
        timeQ = true;
    }

    if (timeQ)
    {
        /**** not supported ****/
        m_pdm->msg1(CG_E_NOT_SUPPORTED, pEdiPos, 
            m_pdm->res1(CG_RES_STR_TIME_QUALIFIER));
    }

    return ret;
}


SFCColorReturn SFC::colorInit(int nodeNr, 
                              SFCColor color)
{
    /*
     node already colored -> return
     node of type: step,trans ->   color -> sfcColor(nodeNr)
                                falls (node.succ == -1) -> return (nodeNr, ColFinalStep)
                                sonst                   -> color(node.succ, color)
                altDiv     ->   colorAlt(nodeNr, color)
                altCon     ->   color -> sfcColor(nodeNr)
                                color(node.succ, color)
                simDiv     ->   colorSim(nodeNr, color)
                simCon     ->   Fehler!
                                return (nodeNr, ColErrorNode)
    */
    SFCColor*   sfcColor = m_sfcColor.getElements();
    SFCElement* sfcGraph = m_sfcGraph.getElements();
    SFCColorReturn ret = {-1, ColErrorNode};

    if (nodeNr < 0)
        return ret;
    
    // node already colored -> return 
    if (sfcColor[nodeNr].simDepth > 0)
    {
        ret.nodeNr = 0;
        ret.nodeTyp = ColNextNode;
        return ret;
    }
    // node of type:
    switch (sfcGraph[nodeNr].typ) {
    case SFCtStep:
    case SFCtTrans:
        if (sfcGraph[nodeNr].succ == -1)
        {
            sfcColor[nodeNr] = color;
            ret.nodeNr = nodeNr;
            ret.nodeTyp = ColFinalNode;
            return ret;
        }
        else 
        {
            sfcColor[nodeNr] = color;
            ret = colorInit(sfcGraph[nodeNr].succ, color);
            return ret;
        }
        break;
    case SFCtAltDiv:
        ret = colorAlt(nodeNr, color, false);
        if ((ret.nodeTyp == ColNextNode)
            || (ret.nodeTyp == ColOpenNode)
            || (ret.nodeTyp == ColFinalNode))
        {
            if (ret.nodeNr == -1 && ret.nodeTyp == ColFinalNode)
                return ret;
            ret = colorInit(ret.nodeNr, color);
        }
        else if (ret.nodeTyp == ColOpenSimNode)
        {
            // EEEE 1
            //printErrorMsg("EEEE 1: SimCon directly reached from initialStep, simCon nr: ", ret.nodeNr);
            m_pdm->msg1(CG_E_SFC_UNSAFE,
                        &(m_ediPosTransition.get( sfcGraph[sfcGraph[ret.nodeNr].succ].nr )), 
                        m_pdm->res1(CG_RES_STR_OPEN_SIMCON));
            ret.nodeTyp = ColErrorNode;
            return ret;
        }
        return ret;
        break;
    case SFCtAltCon:
        sfcColor[nodeNr] = color;
        ret = colorInit(sfcGraph[nodeNr].succ, color);
        return ret;
        break;
    case SFCtSimDiv:
        ret = colorSim(nodeNr, color);
        if (ret.nodeTyp == ColNextNode)
        {
            ret = colorInit(ret.nodeNr, color);
        }
        return ret;
        break;
    case SFCtSimCon:
        // EEEE 2
        m_pdm->msg1(CG_E_SFC_UNSAFE,
                    &m_ediPosTransition.get( sfcGraph[sfcGraph[nodeNr].succ].nr), 
                    m_pdm->res1(CG_RES_STR_OPEN_SIMCON));
        ret.nodeNr = nodeNr;
        ret.nodeTyp = ColErrorNode;
        return ret;
        break;
    default:
        //printErrorMsg("Strange nodeTyp of node nr: ", nodeNr);
        ret.nodeNr = nodeNr;
        ret.nodeTyp = ColErrorNode;
        return ret;
    }
    return ret;
}



SFCColorReturn SFC::colorStraight(int nodeNr,
                                  SFCColor color)
{
    /*
     falls node already colored -> falls (color == sfcColor) -> return (nodeNr, ColSameColNode)
                                   sonst                     -> return (nodeNr, ColDifColNode)
     sonst -> node of type: step, trans -> color -> sfcColor(nodeNr)
                                           falls (node.succ == -1) -> return (nodeNr, ColFinalStep)
                                           sonst                   -> colorStraight(node.succ, color)
                            sonst       -> return(nodeNr, nodeTyp)
    */       

    SFCColor*   sfcColor = m_sfcColor.getElements();
    SFCElement* sfcGraph = m_sfcGraph.getElements();
    SFCColorReturn ret = {-1, ColErrorNode};

    if (nodeNr < 0)
    {
        ret.nodeNr = -1;
        ret.nodeTyp = ColErrorNode;
        return ret;
    }

    // node already colored
    if (sfcColor[nodeNr].simDepth > 0) 
    {
        // same color
        if ((sfcColor[nodeNr].simDepth == color.simDepth)
            && (sfcColor[nodeNr].simBranch == color.simBranch))
        {
            ret.nodeNr = nodeNr;
            //if (sfcGraph[nodeNr].typ == SFCtAltCon)
            //    ret.nodeTyp = ColAltConNode;
            //else
                ret.nodeTyp = ColSameColNode;
            return ret;
        }
        // different color
        else
        {
            // EEEE 3 = sfc12
            m_pdm->msg1(CG_E_SFC_UNSAFE,&m_endOfFile, 
                m_pdm->res1(CG_RES_STR_UNSAFE_LOOP));
            
            ret.nodeNr = nodeNr;
            ret.nodeTyp = ColDifColNode;
            return ret;
        }
    }
    // node not yet colored
    else
    {
        ret.nodeNr = nodeNr;

        switch (sfcGraph[nodeNr].typ)
        {
        case SFCtStep:
        case SFCtTrans:
            if (sfcGraph[nodeNr].succ == -1)
            {
                sfcColor[nodeNr] = color;
                ret.nodeNr = nodeNr;
                ret.nodeTyp = ColFinalNode;
                return ret;
            }
            else 
            {
                sfcColor[nodeNr] = color;
                ret = colorStraight(sfcGraph[nodeNr].succ, color);
                return ret;
            }
            break;
        case SFCtAltDiv:
            ret.nodeTyp = ColAltDivNode;
            return ret;
            break;
        case SFCtAltCon:
            ret.nodeTyp = ColAltConNode;
            return ret;
            break;
        case SFCtSimDiv:
            ret.nodeTyp = ColSimDivNode;
            return ret;
            break;
        case SFCtSimCon:
            ret.nodeTyp = ColSimConNode;
            return ret;
            break;
        default:
            //printErrorMsg("Strange nodeTyp of node nr: ", nodeNr);
            ret.nodeTyp = ColErrorNode;
            return ret;
        }
    }

    return ret;
}



SFCColorReturn SFC::colorAlt(int nodeNr,
                             SFCColor color, 
                             bool simCall)
{
    /*
 altDiv <- nodeNr
  für jeden Ast unter altDiv: (nextAltNode)
        ret <- colorStraight(nextAltNode, color)
        ret of type:    ColAltDivNode:  Alle Äste unter ret.NodeNr unter altDiv hängen
                                        nächsten Ast bearbeiten
                        ColAltConNode:  falls alle Vorgänger gefärbt ->
                                            alle die gleiche Farbe -> 
                                                (nextAltNode == letzter Ast unter altDiv) und (openNodes leer) ->
                                                           return(ret.NodeNr.succ, ColNextNode)
                                                 sonst -> 
                                                        sfcColor(ret.NodeNr) <- color
                                                        ret.NodeNr aus openNodes streichen
                                                        ret.NodeNr.succ unter altDiv hängen
                                                        nächsten Ast bearbeiten
                                            sonst                  -> return (ret.NodeNr, ColErrorNode)
                                        sonst ->    openNodes <- ret.NodeNr
                                                    nächsten Ast bearbeiten
                        ColSimDivNode:  colorSim(ret.NodeNr, color)
                        ColSimConNode:  Fehler!
                                        evtl. openNodes ausgeben
                                        return(nodeNr, ColErrorNode)
                        ColNextNode:    retNode.NodeNr unter altDiv hängen
                                        nächsten Ast bearbeiten


    */
    SFCColor*   sfcColor = m_sfcColor.getElements();
    SFCElement* sfcGraph = m_sfcGraph.getElements();
    SFCColorReturn ret = {-1, ColErrorNode};

    FC_CGArray<int> openNodes;
    FC_CGArray<int> finalNodes;
    FC_CGArray<int> openSimNodes;

    int altDiv = nodeNr;
    int nextAltNode = sfcGraph[altDiv].succ;
    int lastCon = -1;

    while (nextAltNode != -1)
    {
        SFCColorReturn straightRet = colorStraight(nextAltNode, color);
        
        switch (straightRet.nodeTyp)
        {
        case ColErrorNode:
            return straightRet;
            break;
        case ColSameColNode:
            // nothing to do, do next branch
            break;
        case ColDifColNode:
            return straightRet;
            break;
        case ColFinalNode:
            finalNodes.add(straightRet.nodeNr);
            break;
        case ColAltDivNode:
        {
            // push up all branches under ret.altDiv to altDiv
            int nAltDiv = straightRet.nodeNr;
            int nextAltBranch = sfcGraph[nAltDiv].succ;
            int lastRightNode = sfcGraph[nextAltNode].predRight;
            int lastLeftNode = nextAltNode;
            while (nextAltBranch != -1)
            {
                sfcGraph[lastLeftNode].predRight = nextAltBranch;
                sfcGraph[nextAltBranch].predLeft = lastLeftNode;
                lastLeftNode = nextAltBranch;
                nextAltBranch = sfcGraph[nextAltBranch].predRight;
            }
            sfcGraph[lastLeftNode].predRight = lastRightNode;
            if (lastRightNode != -1)
            {
                sfcGraph[lastRightNode].predLeft = lastLeftNode;
            }
            sfcColor[nAltDiv] = color;
            break;
        }
        case ColAltConNode:
            /*
                 ColAltConNode:  falls alle Vorgänger gefärbt ->
                                            alle die gleiche Farbe -> 
                                                (nextAltNode == letzter Ast unter altDiv) und (openNodes leer) ->
                                                           return(ret.NodeNr.succ, ColNextNode)
                                                 sonst -> 
                                                        sfcColor(ret.NodeNr) <- color
                                                        ret.NodeNr aus openNodes streichen
                                                        ret.NodeNr.succ unter altDiv hängen
                                                        nächsten Ast bearbeiten
                                            sonst                  -> return (ret.NodeNr, ColErrorNode)
                                        sonst ->    openNodes <- ret.NodeNr
                                                    nächsten Ast bearbeiten
            */
            {
                int altCon = straightRet.nodeNr;
                int predNode = sfcGraph[altCon].pred;
                bool allCol = true;
                while (predNode != -1)
                {
                    if (sfcColor[predNode].simDepth == 0)
                    {
                        allCol = false;
                        break;
                    }
                    if ((sfcColor[predNode].simDepth != color.simDepth)
                        || (sfcColor[predNode].simBranch != color.simBranch))
                    {
                        // EEEE 4 ----
                        m_pdm->msg1(CG_E_SFC_UNSAFE,
                                     &m_ediPosTransition.get( sfcGraph[sfcGraph[altCon].pred].nr),
                                     m_pdm->res1(CG_RES_STR_TOKEN_JUMP));
                        ret.nodeNr = altCon;
                        ret.nodeTyp = ColDifColNode;
                        return ret;
                    }
                    predNode = sfcGraph[predNode].succRight;
                }

                // all pred nodes colored
                if (allCol)
                {
                    // close node, count open nodes
                    int anzOpenNodes = 0;
                    for (int i = 0; i<openNodes.getUsed(); i++)
                    {
                        if (openNodes.get(i) == altCon)
                            openNodes.set(i, -1);
                        if (openNodes.get(i) != -1)
                            anzOpenNodes++;
                    }
                    sfcColor[altCon] = color;

                    // is this the last node under altDiv 
                    // and no more open nodes under altDiv
                    if ((sfcGraph[nextAltNode].predRight == -1)
                        && (anzOpenNodes == 0))
                    {
                        // altDiv finished
                        lastCon = altCon;
                        break;
                    }
                    // else push up the node under altCon under altDiv
                    else
                    {
                        
                        int pushUpNode = sfcGraph[altCon].succ;
                        if (sfcColor[pushUpNode].simDepth == 0)
                        {
                            if (sfcColor[pushUpNode].simDepth == 0)
                            {
                                // is node already in list
                                int rightNode = sfcGraph[nextAltNode].predRight;
                                int inList = false;
                                while (rightNode != -1)
                                {
                                    if (rightNode == pushUpNode)
                                    {
                                        inList = true;
                                        break;
                                    }
                                    rightNode = sfcGraph[rightNode].predRight;
                                }

                                if (!inList)
                                {
                                    rightNode = sfcGraph[nextAltNode].predRight;
                                    sfcGraph[nextAltNode].predRight = pushUpNode;
                                    sfcGraph[pushUpNode].predLeft = nextAltNode;
                                    sfcGraph[pushUpNode].predRight = rightNode;
                                    if (rightNode != -1)
                                    {
                                        sfcGraph[rightNode].predLeft = pushUpNode;
                                    }
                                }
                            }
                        }
                        
                    }

                }
                // not all pred nodes colored
                else
                {
                    bool foundDouble = false;
                    for (int doubleOpenNodes=0; doubleOpenNodes<openNodes.getUsed(); doubleOpenNodes++)
                    {
                        if (openNodes.get(doubleOpenNodes) == altCon)
                            foundDouble = true;
                    }
                    if (!foundDouble)
                    {
                        openNodes.add(altCon);
                    }
                    // --------------------------
                    /*
                    int pushUpNode = sfcGraph[altCon].succ;
                    if (sfcColor[pushUpNode].simDepth == 0)
                    {
                        int rightNode = sfcGraph[nextAltNode].predRight;
                        sfcGraph[nextAltNode].predRight = pushUpNode;
                        sfcGraph[pushUpNode].predLeft = nextAltNode;
                        sfcGraph[pushUpNode].predRight = rightNode;
                        if (rightNode != -1)
                        {
                            sfcGraph[rightNode].predLeft = pushUpNode;
                        }
                    }
                    */
                    // ---------------------------
                }

                break;
            }
        case ColSimDivNode:
            SFCColorReturn retSim = colorSim(straightRet.nodeNr, color);
            
            if (retSim.nodeTyp == ColNextNode)
            {
                int pushUpNode = retSim.nodeNr;
                int rightNode = sfcGraph[nextAltNode].predRight;
                sfcGraph[nextAltNode].predRight = pushUpNode;
                sfcGraph[pushUpNode].predLeft = nextAltNode;
                sfcGraph[pushUpNode].predRight = rightNode;
                if (rightNode != -1)
                {
                    sfcGraph[rightNode].predLeft = pushUpNode;
                }
            }
            else if (retSim.nodeTyp == ColSimLoops)
            {
                break;
            }
            else
            {
                return retSim;
            }

            break;
        case ColSimConNode:
            //ret.nodeNr = straightRet.nodeNr;
            //ret.nodeTyp = ColOpenSimNode;
            //return ret;
            openSimNodes.add(straightRet.nodeNr);
            break;
        default:
            //printErrorMsg("StraightColor returned strange nodeTyp for node nr: ", 
            //    straightRet.nodeNr);
            ret.nodeNr = straightRet.nodeNr;
            ret.nodeTyp = ColErrorNode;
            return ret;
        }

        // next branch
        nextAltNode = sfcGraph[nextAltNode].predRight;

        if (sfcColor[nextAltNode].simDepth>0)
        {
            nextAltNode = -1;
        }
        
        // if nextAltNode == -1 but there are openNodes, push up all successors 
        // of the open nodes 
        int lastPushUpNode = -1;
        if ((nextAltNode == -1) && (openNodes.getUsed()>0))
        {
            for (int i=0; i<openNodes.getUsed(); i++)
            {
                if (openNodes.get(i) != -1)
                {
                    int nodeToPushUp = sfcGraph[openNodes.get(i)].succ;
                    //sfcColor[openNodes.get(i)] = color;
                    if (sfcColor[nodeToPushUp].simDepth == 0)
                    {
                        if (nextAltNode == -1)
                        {
                            nextAltNode = nodeToPushUp;
                        }
                        if (lastPushUpNode != -1)
                        {
                            sfcGraph[lastPushUpNode].predRight = nodeToPushUp;
                        }
                        lastPushUpNode = nodeToPushUp;
                    }
                }
            }
        }
    }

    sfcColor[altDiv] = color;
    
    if ((lastCon == -1) && (openSimNodes.getUsed()==0) && !simCall)
    {
        ret.nodeNr = -1;
        ret.nodeTyp = ColFinalNode;
        return ret;
    }
    else 
    {
      if (simCall)
      {
        for (int i=0; i<finalNodes.getUsed(); i++)
        {
            if (finalNodes.get(i) != -1)
            {
                ret.nodeNr = finalNodes.get(i);
                ret.nodeTyp = ColFinalNode;
                return ret;
            }
        }
        for (i=0; i<openNodes.getUsed(); i++)
        {
            if (openNodes.get(i) != -1)
            {
                ret.nodeNr = openNodes.get(i);
                ret.nodeTyp = ColOpenNode;
                return ret;
            }
        }
      }

      if (openSimNodes.getUsed()>1)
      {
          //printErrorMsg("Multiple simCons under altDiv, altDiv nr: ", altDiv);
          m_pdm->msg1(CG_E_SFC_UNSAFE, &m_endOfFile, m_pdm->res1(CG_RES_STR_OPEN_ALTDIV));
          ret.nodeNr = altDiv;
          ret.nodeTyp = ColOpenNode;
          return ret;
      }
      if (openSimNodes.getUsed()==1)
      {
          ret.nodeNr = openSimNodes.get(0);
          ret.nodeTyp = ColOpenSimNode;
          return ret;
      }
      
      //SFCColor col0 = {0,0};
      //colorStraight(sfcGraph[lastCon].succ, col0);
      ret.nodeNr = sfcGraph[lastCon].succ;
      ret.nodeTyp = ColNextNode;
      return ret;
    }

    return ret;
}



SFCColorReturn SFC::colorSim(int nodeNr,
                             SFCColor color)
{
   /*
  simDiv <- nodeNr
  actColor <- (nextSimColor, 0)
  für jeden Ast unter simDiv: (nextSimNode)
    actColor.simBranch ++
    ret <- colorStraight(nextSimNode, color)
    ret of type:    ColAltDivNode:  colorAlt(ret.NodeNr, actColor)
                    ColAltConNode:  Fehler!
                                    return(ret.NodeNr, ColErrorNode)
                    ColSimDivNode:  Alle Äste unter ret.NodeNr unter simDiv hängen
                                    nächsten Ast bearbeiten
                    ColSimConNode:  falls alle Vorgänger gefärbt ->
                                        (nextSimNode == letzterAst unter simDiv) und (openNodes leer) ->
                                                return(ret.NodeNr.succ, ColNextNode)
                                        sonst ->
                                                sfcColor(ret.NodeNr) <- actColor   
                                                ret.Nodenr aus openNodes streichen
                                                ret.NodeNr.succ unter simDiv hängen
                                                nächstenAt bearbeiten
                                    sonst ->    openNodes <- ret.NodeNr
                                                nächsten Ast bearbeiten
                    ColNextNode:    retNode.NodeNr unter simDiv hängen
                                    nächstenAst bearbeiten
                    ColSameColNode: Fehler!
                                    return(ret.nodeNr, ColErrorNode)
  */
    
    SFCColor*   sfcColor = m_sfcColor.getElements();
    SFCElement* sfcGraph = m_sfcGraph.getElements();
    SFCColorReturn ret = {-1, ColErrorNode};

    FC_CGArray<int> openNodes;
    FC_CGArray<int> finalNodes;
    FC_CGArray<int> openAltNodes;

    int simDiv = nodeNr;
    SFCColor actColor;
    
    actColor.simDepth = m_nextSimColor;
    m_nextSimColor++;
    actColor.simBranch = 0;

    int nextSimNode = sfcGraph[simDiv].succ;
    int lastCon = -1;

    while (nextSimNode != -1)
    {
        actColor.simBranch++;

        SFCColorReturn straightRet = colorStraight(nextSimNode, actColor);

        switch (straightRet.nodeTyp)
        {
        case ColErrorNode:
            return straightRet;
            break;
        case ColSameColNode:
            // do nothing
            break;
        case ColDifColNode:
            return straightRet;
            break;
        case ColFinalNode:
            //printErrorMsg("Final node under open simDiv, node nr: ", straightRet.nodeNr);
            m_pdm->msg1(CG_E_SFC_UNSAFE,
                        m_ediPosStep.getAddrOf(straightRet.nodeNr),
                        m_pdm->res1(CG_RES_STR_UNSAFE_FINAL));
            ret.nodeNr = straightRet.nodeNr;
            ret.nodeTyp = ColErrorNode;
            return ret;
            break;
        case ColAltDivNode:
            // call colorAlt
            SFCColorReturn altRet = colorAlt(straightRet.nodeNr, actColor, true);

            // closed altDiv -> push up node under last altCon
            if (altRet.nodeTyp == ColNextNode)
            {
                 int pushUpNode = altRet.nodeNr;
                 int rightNode = sfcGraph[nextSimNode].predRight;
                 sfcGraph[nextSimNode].predRight = pushUpNode;
                 sfcGraph[pushUpNode].predLeft = nextSimNode;
                 sfcGraph[pushUpNode].predRight = rightNode;
                 if (rightNode != -1)
                 {
                      sfcGraph[rightNode].predLeft = pushUpNode;
                 }
            }
            else if (altRet.nodeTyp == ColFinalNode)
            {
                //printErrorMsg("Final Node under SimDiv: node Nr: ",altRet.nodeNr);
                m_pdm->msg1(CG_E_SFC_UNSAFE, m_ediPosStep.getAddrOf(altRet.nodeNr),
                            m_pdm->res1(CG_RES_STR_UNSAFE_FINAL));
                ret.nodeNr = altRet.nodeNr;
                ret.nodeTyp = ColErrorNode;
                return ret;
            }
            else if (altRet.nodeTyp == ColOpenNode)
            {
                //printErrorMsg("Open altDiv branches under simDiv: altDiv nr: ",altRet.nodeNr);
                m_pdm->msg1(CG_E_SFC_UNSAFE, &m_endOfFile, m_pdm->res1(CG_RES_STR_OPEN_ALTDIV));
                ret.nodeNr = altRet.nodeNr;
                ret.nodeTyp = ColErrorNode;
                return ret;
            }
            else if (altRet.nodeTyp == ColOpenSimNode)
            {
                int pushUpNode = sfcGraph[altRet.nodeNr].pred; 
                int rightNode = sfcGraph[nextSimNode].predRight;
                sfcGraph[nextSimNode].predRight = pushUpNode;
                sfcGraph[pushUpNode].predLeft = nextSimNode;
                sfcGraph[pushUpNode].predRight = rightNode;
                if (rightNode != -1)
                {
                sfcGraph[rightNode].predLeft = pushUpNode;
                }
                sfcColor[pushUpNode].simDepth = 0;
                sfcColor[pushUpNode].simBranch = 0;
            }
            else
            {
                return altRet;
            }
            break;
        case ColAltConNode:
            {
                openAltNodes.add(straightRet.nodeNr);
                
                int pushUpNode = sfcGraph[straightRet.nodeNr].succ; 
                int rightNode = sfcGraph[nextSimNode].predRight;
                sfcGraph[nextSimNode].predRight = pushUpNode;
                sfcGraph[pushUpNode].predLeft = nextSimNode;
                sfcGraph[pushUpNode].predRight = rightNode;
                if (rightNode != -1)
                {
                    sfcGraph[rightNode].predLeft = pushUpNode;
                }

                // color next branch (-> just pushed up node) in same color
                actColor.simBranch--;
                
                break;
            }
        case ColSimDivNode:
            {
                // push up all branches under ret.altDiv to altDiv
                int nSimDiv = straightRet.nodeNr;
                int nextSimBranch = sfcGraph[nSimDiv].succ;
                int lastRightNode = sfcGraph[nextSimNode].predRight;
                int lastLeftNode = nextSimNode;
                while (nextSimBranch != -1)
                {
                    sfcGraph[lastLeftNode].predRight = nextSimBranch;
                    sfcGraph[nextSimBranch].predLeft = lastLeftNode;
                    lastLeftNode = nextSimBranch;
                    nextSimBranch = sfcGraph[nextSimBranch].predRight;
                }
                sfcGraph[lastLeftNode].predRight = lastRightNode;
                if (lastRightNode != -1)
                {
                    sfcGraph[lastRightNode].predLeft = lastLeftNode;
                }
                sfcColor[nSimDiv] = actColor;

                break;
            }
        case ColSimConNode:
            /*
                          ColSimConNode:  falls alle Vorgänger gefärbt ->
                                       (nextSimNode == letzterAst unter simDiv) und (openNodes leer) ->
                                                return(ret.NodeNr.succ, ColNextNode)
                                        sonst ->
                                                sfcColor(ret.NodeNr) <- actColor   
                                                ret.Nodenr aus openNodes streichen
                                                ret.NodeNr.succ unter simDiv hängen
                                                nächstenAt bearbeiten
                                    sonst ->    openNodes <- ret.NodeNr
                                                nächsten Ast bearbeiten
            */
            {
                int simCon = straightRet.nodeNr;
                int predNode = sfcGraph[simCon].pred;
                bool allCol = true;
                while (predNode != -1)
                {
                    if (sfcColor[predNode].simDepth == 0)
                    {
                        allCol = false;
                        break;
                    }
                    if (sfcColor[predNode].simDepth != actColor.simDepth)
                    {
                        //printErrorMsg("Different colored nodes (different sim depths) in simCon nr: ", 
                        //    simCon);
                        m_pdm->msg1(CG_E_SFC_UNSAFE, &m_endOfFile, m_pdm->res1(CG_RES_STR_TOKEN_JUMP));
                        ret.nodeNr = simCon;
                        ret.nodeTyp = ColDifColNode;
                        return ret;
                    }
                    predNode = sfcGraph[predNode].succRight;
                }

                // all pred nodes colored
                if (allCol)
                {
                    // close node, count open nodes
                    int anzOpenNodes = 0;
                    for (int i = 0; i<openNodes.getUsed(); i++)
                    {
                        if (openNodes.get(i) == simCon)
                            openNodes.set(i, -1);
                        if (openNodes.get(i) != -1)
                            anzOpenNodes++;
                    }
                    sfcColor[simCon].simDepth = actColor.simDepth;
                    sfcColor[simCon].simBranch = 0;

                    // is this the last node under altDiv 
                    // and no more open nodes under altDiv
                    if ((sfcGraph[nextSimNode].predRight == -1)
                        && (anzOpenNodes == 0))
                    {
                        // altDiv finished
                        lastCon = simCon;
                        break;
                    }
                    // else push up the node under altCon under altDiv
                    else
                    {
                        int pushUpNode = sfcGraph[simCon].succ;
                        int rightNode = sfcGraph[nextSimNode].predRight;
                        sfcGraph[nextSimNode].predRight = pushUpNode;
                        sfcGraph[pushUpNode].predLeft = nextSimNode;
                        sfcGraph[pushUpNode].predRight = rightNode;
                        if (rightNode != -1)
                        {
                            sfcGraph[rightNode].predLeft = pushUpNode;
                        }
                    }
                }
                // not all pred nodes colored
                else
                {
                    openNodes.add(simCon);
                }
                break;
            }
        default:
            //printErrorMsg("StraightColor returned strange nodeTyp for node nr: ", 
            //    straightRet.nodeNr);
            ret.nodeNr = straightRet.nodeNr;
            ret.nodeTyp = ColErrorNode;
            return ret;
        }
        // next branch
        nextSimNode = sfcGraph[nextSimNode].predRight;
    }

    sfcColor[simDiv].simDepth = actColor.simDepth;
    sfcColor[simDiv].simBranch = 0;
    
    for (int i=0; i<openNodes.getUsed(); i++)
    {
        if (openNodes.get(i) != -1)
        {
            //printErrorMsg("Open simCon under simDiv, simCon nr: ", openNodes.get(i));
            m_pdm->msg1(CG_E_SFC_UNSAFE, &m_endOfFile, m_pdm->res1(CG_RES_STR_OPEN_SIMCON));  
            ret.nodeNr = openNodes.get(i);
            ret.nodeTyp = ColErrorNode;
            return ret;
        }
    }
    for (i=0; i<openAltNodes.getUsed(); i++)
    {
        int allCol = true;
        int prev = sfcGraph[openAltNodes.get(i)].pred;
        SFCColor c = sfcColor[prev];
        while (prev!=-1)
        {
            if ((sfcColor[prev].simDepth != c.simDepth) 
                ||(sfcColor[prev].simBranch != c.simBranch))
            {
                allCol = false;
                break;
            }
            prev = sfcGraph[prev].succRight;
        }
        if (!allCol)
        {
            //printErrorMsg("Open or different colored altCon under simDiv, altCon nr: ", 
            //    openAltNodes.get(i));
            m_pdm->msg1(CG_E_SFC_UNSAFE, &m_endOfFile, m_pdm->res1(CG_RES_STR_TOKEN_JUMP));
            ret.nodeNr = openAltNodes.get(i);
            ret.nodeNr = ColErrorNode;
            return ret;
        }
    }

    if (lastCon!=-1)
    {
        ret.nodeNr = sfcGraph[lastCon].succ;
        ret.nodeTyp = ColNextNode;
        return ret;
    }
    else
    {
        ret.nodeNr = -1;
        ret.nodeTyp = ColSimLoops;
        return ret;
    }
    
    return ret;
}



// check
bool SFC::check()
{
    m_sfcColor.resetUsed(); 
   
    SFCColor col = {0,0};

    for(int i=0; i<m_sfcGraph.getUsed(); i++)
    {
        m_sfcColor.add(col);
    }

    col.simDepth = 1;
    col.simBranch = 0;
    m_nextSimColor = 2;
    
    assert(m_initialStep>=0);

    SFCColorReturn ret = colorInit(m_initialStep, col);
    if ((ret.nodeTyp == ColErrorNode) || (ret.nodeTyp == ColDifColNode))
        return false;

    for (i=0; i<m_numSteps; i++)
    {
        SFCColor col = m_sfcColor.get(i);
        if (col.simBranch==0 && col.simDepth==0)
        {
            m_pdm->msg1(CG_E_SFC_UNSAFE, m_ediPosStep.getAddrOf(i),
                m_pdm->res1(CG_RES_STR_STEPNOTREACHED)<<_T("'")<<getStepName(i)<<_T("'"));
        }
    }

    return true;
}


int  SFC::isStep   (const TCHAR* pName)
{
    return searchStringList(pName, &m_stepNames);
}

int  SFC::isAction (const TCHAR* pName)
{
    return searchStringList(pName, &m_actionNames);
}


    
const TCHAR*     SFC::getStepName     (int nr)
{
    return m_stepNames.get(nr);
}

const TCHAR*     SFC::getActionName   (int nr)
{
    return m_actionNames.get(nr);
}

const TCHAR*     SFC::getTransName   (int nr)
{
    return m_transitionNames.get(nr);
}
 
const CG_Edipos* SFC::getStepEdiPos   (int nr)
{
    return (&m_ediPosStep.get(nr));
}

const CG_Edipos* SFC::getActionEdiPos (int nr)
{
    return (&m_ediPosAction.get(nr));
}

const CG_Edipos* SFC::getTransEdiPos (int nr)
{
    return (&m_ediPosTransition.get(nr));
}


// =================================================================================
// C Interface
// =================================================================================


SFC_CHECKER* ST_SFC_CreateSFCChecker(CGT_CDiagMsg* pdm)
{
    SFC *sfc = new SFC(pdm);
    return (SFC_CHECKER*)sfc;
}



void  ST_SFC_FreeSFCChecker (SFC_CHECKER* pHdl)
{
    delete (SFC*)pHdl;
}



bool  ST_SFC_AddSFCName    (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        return (sfc->addSFCName(pszName, ppos));
    }
    else
        return false;
}



bool  ST_SFC_AddInitialStep(SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        return (sfc->addInitialStep(pszName, ppos));
    }
    else
        return false;
}



bool  ST_SFC_AddStep       (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        return (sfc->addStep(pszName, ppos));
    }
    else
        return false;
}



bool  ST_SFC_AddTransition (SFC_CHECKER* pHdl,
                            int prio, 
                            const FC_CStrList* fromList, 
                            const FC_CStrList* toList,
                            bool fromSim, 
                            bool toSim,
                            const CG_Edipos* ppos,
                            FC_CGArray<CG_Edipos> *pfromPos,
                            FC_CGArray<CG_Edipos> *ptoPos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        sfc->addTransitionNoName();
        return (sfc->addTransition(prio, fromList, toList, fromSim, toSim, 
                                   ppos, pfromPos, ptoPos));
    }
    else
        return false;
}



bool  ST_SFC_AddTransitionNamed (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            int prio, 
                            const FC_CStrList* fromList, 
                            const FC_CStrList* toList,
                            bool fromSim, 
                            bool toSim,
                            const CG_Edipos* ppos,
                            FC_CGArray<CG_Edipos> *pfromPos,
                            FC_CGArray<CG_Edipos> *ptoPos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        sfc->addTransitionName(pszName);
        return (sfc->addTransition(prio, fromList, toList, fromSim, toSim, 
                                   ppos, pfromPos, ptoPos));
    }
    else
        return false;
}



bool  ST_SFC_AddActionBlock(SFC_CHECKER* pHdl,
                            const TCHAR* stepName, 
                            const TCHAR* actionName, 
                            const TCHAR* qualifier,
                            const long timeParam,
                            const CG_Edipos* ppos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        return (sfc->addActionBlock(stepName, actionName, qualifier, timeParam, ppos));
    }
    else 
        return false;
}



bool  ST_SFC_AddAction     (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos)
{
    SFC* sfc = (SFC*)pHdl;
    if (sfc!=NULL)
    {
        return (sfc->addAction(pszName, ppos));
    }
    else
        return false;
}



bool  ST_SFC_Check(
    SFC_CHECKER*     pHdl,
    const CG_Edipos* pEndOfFile
)
{
    SFC* sfc = (SFC*)pHdl;

    if (sfc!=NULL)
    {
        if (!sfc->createGraph(pEndOfFile))
        {
            return false;
        }

        if (!sfc->codeGen())
        {
            return false;
        }
        
        bool ret = sfc->check();
        if (!ret)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}




int ST_SFC_GetAnzSteps   (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzSteps();
}



int ST_SFC_GetAnzTrans   (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzTrans();
}



int ST_SFC_GetAnzActions (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzActions();
}

int ST_SFC_GetInitStep   (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getInitStep();
}






int   ST_SFC_GetAnzNextSteps (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzNextSteps();
}

int   ST_SFC_GetAnzPrevSteps (SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzPrevSteps();
}

int   ST_SFC_GetAnzStepAction(SFC_CHECKER* pHdl)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAnzStepAction();
}


int   ST_SFC_GetFollowTrans                   (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getFollowTrans(nr);
}

int   ST_SFC_GetAltTrans                      (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getAltTrans(nr);
}

int   ST_SFC_GetNextSteps                     (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getNextSteps(nr);
}

int   ST_SFC_GetNextStepsOffset               (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getNextStepsOffset(nr);
}

int   ST_SFC_GetPrevSteps                     (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getPrevSteps(nr);
}

int   ST_SFC_GetPrevStepsOffset               (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getPrevStepsOffset(nr);
}

int   ST_SFC_GetStepActionBlockOffset         (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getStepActionBlockOffset(nr);
}

int   ST_SFC_GetStepActionBlocks              (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getStepActionBlocks(nr);
}

int   ST_SFC_GetStepActionBlockQualifier      (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getStepActionBlockQualifier(nr);
}

int   ST_SFC_GetStepActionBlockTimeParameter  (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->getStepActionBlockTimeParameter(nr);
}





//next functions return 0 if array to short:
int ST_SFC_GetFollowTransArray                 (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getFollowTransArray(pArr, nElems);
}
int ST_SFC_GetAltTransArray                    (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getAltTransArray(pArr, nElems);
}
int ST_SFC_GetNextStepsArray                   (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getNextStepsArray(pArr, nElems);
}
int ST_SFC_GetNextStepsOffsetArray             (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getNextStepsOffsetArray(pArr, nElems);
}
int ST_SFC_GetPrevStepsArray                   (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getPrevStepsArray(pArr, nElems);
}
int ST_SFC_GetPrevStepsOffsetArray             (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getPrevStepsOffsetArray(pArr, nElems);
}
int ST_SFC_GetStepActionBlockOffsetArray       (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getStepActionBlockOffsetArray(pArr, nElems);
}
int ST_SFC_GetStepActionBlocksArray            (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getStepActionBlocksArray(pArr, nElems);
}
int ST_SFC_GetStepActionBlockQualifierArray    (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getStepActionBlockQualifierArray(pArr, nElems);
}
int ST_SFC_GetStepActionBlockTimeParameterArray(SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems)
{
    return ((SFC*)pHdl)->getStepActionBlockTimeParameterArray(pArr, nElems);
}






int   ST_SFC_IsStep(SFC_CHECKER* pHdl, const TCHAR* pName)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->isStep(pName);
}

int   ST_SFC_IsAction(SFC_CHECKER* pHdl, const TCHAR* pName)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);
    
    return sfc->isAction(pName);
}


const TCHAR* ST_SFC_GetStepName               (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getStepName(nr);
}

const TCHAR* ST_SFC_GetActionName             (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getActionName(nr);
}

const TCHAR* ST_SFC_GetTransName              (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getTransName(nr);
}


const CG_Edipos* ST_SFC_GetStepEdiPos         (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getStepEdiPos(nr);
}

const CG_Edipos* ST_SFC_GetActionEdiPos       (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getActionEdiPos(nr);
}

const CG_Edipos* ST_SFC_GetTransEdiPos        (SFC_CHECKER* pHdl, int nr)
{
    SFC* sfc = (SFC*)pHdl;

    assert(sfc!=NULL);

    return sfc->getTransEdiPos(nr);
}






#if defined(DEBUG) && defined(SFC_DEBUGCODE)
#include <iostream.h>
#include <iomanip.h>
// output, only for debugging
// printErrorMsg
// just for use in stand alone mode to show some error messages
static void printErrorMsg(const TCHAR* str, int i = -10000)
{
    printf("SFC code generator ERROR: %s", str);
    
    if (i != -10000)
        printf("%d", i);

    printf(" \n");
}

bool  ST_SFC_CheckDebug    (SFC_CHECKER* pHdl,
                            CG_Edipos* pEndOfFile)
{
    SFC* sfc = (SFC*)pHdl;

    if (sfc!=NULL)
    {
        sfc->output(); 
        flush(cout);

        if (!sfc->createGraph(pEndOfFile))
        {
            printErrorMsg("Error creating sfc graph");
            return false;
        }
        sfc->outputGraph();
        flush(cout);

        if (!sfc->codeGen())
        {
            printErrorMsg("Error generating sfc code");
            return false;
        }
        sfc->outputInterpreter();
        flush(cout);
        
        cout << " \nChecking SFC: \n";
        bool ret = sfc->check();
        sfc->outputColor();
        flush(cout);
        if (!ret)
        {
            printErrorMsg("Error checking sfc");
            return false;
        }
        else
        {
            flush(cout);
            return true;
        }
    }
    else
    {
        return false;
    }
}


void SFC::output()
{
    int i,j;
    cout << "SFC: " << (const char *)m_sfcName << " \n";

    cout << "  INITIAL_STEP: ";
    for (i=0; i<m_initialStepName.getUsed(); i++)
    {
        cout << m_initialStepName.get(i);
    }
    cout << " \n";
    cout << "  STEPS: \n";
    for (i=0; i<m_stepNames.getUsed(); i++)
    {
        cout << "     " << m_stepNames.get(i) << " \n";
    }

    cout << "  TRANSITIONS: \n";
    for (i=0; i<m_transition.getUsed(); i++)
    {
        cout << "    PRIO: " << m_transition.get(i).prio << "  ";
        if (m_transition.get(i).fromSim)
            cout << "    FROM=: ";
        else
            cout << "    FROM-: ";
        for (j=m_transition.get(i).fromStartOffset; j<m_transition.get(i).fromEndOffset; j++)
        {
            cout << m_transFromList.get(j) << "  ";
        }
        if (m_transition.get(i).toSim)
            cout << "    TO=: ";
        else
            cout << "    TO-: ";
        for (j=m_transition.get(i).toStartOffset; j<m_transition.get(i).toEndOffset; j++)
        {
            cout << m_transToList.get(j) << "  ";
        }
        cout << " \n";
    }

    cout << "  ACTIONS: \n";
    for (i=0; i<m_actionNames.getUsed(); i++)
    {
        cout << "    " << m_actionNames.get(i) << " \n";
    }
    
    cout << "  ACTION_BLOCKS: \n";
    for (i=0; i<m_aBlockStepName.getUsed(); i++)
    {
        cout << "    " << m_aBlockStepName.get(i) << "  " 
                       << m_aBlockQualifier.get(i) << "  "
                       << m_aBlockTimePara.get(i) << "  "
                       << m_aBlockAction.get(i) << " \n";
    }
    cout << " \n";
    flush(cout);
}


// outputGraph
void SFC::outputGraph()
{
    int i;

    cout << "SFC Graph: \n";

    for (i=0; i<m_sfcGraph.getUsed(); i++)
    {
        SFCElement elem;
        elem = m_sfcGraph.get(i);

        cout << setw(3) << i << " : ";
        switch (elem.typ) 
        {
        case SFCtStep: 
            cout << "Step   ";
            break;
        case SFCtTrans:
            cout << "Trans  ";
            break;
        case SFCtAltDiv:
            cout << "AltDiv ";
            break;
        case SFCtAltCon:
            cout << "AltCon ";
            break;
        case SFCtSimDiv:
            cout << "SimDiv ";
            break;
        case SFCtSimCon:
            cout << "SimCon ";
            break;
        default:
            cout << "unknown";
        }

        cout << setw(3) << elem.nr << "   -- ";

        cout << "  pred: " << setw(4) << elem.pred;
        cout << "  succ: " << setw(4) << elem.succ;
        cout << "  pl  : " << setw(4) << elem.predLeft;
        cout << "  pr  : " << setw(4) << elem.predRight;
        cout << "  sl  : " << setw(4) << elem.succLeft;
        cout << "  sr  : " << setw(4) << elem.succRight;

        cout << " \n";
    }
}



void SFC::outputInterpreter()
{
    int i;
    const w = 4;

    cout << " \nSFCInterpreter: \n\n";

    cout << "                  ";
    for (i=0; i<40; i++)
    {
        cout << setw(w) << i;
    }
    cout << " \n";

    cout << "FollowTrans     : ";
    for (i=0; i<m_followTrans.getUsed(); i++) 
    {
        cout << setw(w) << m_followTrans.get(i);    
    }
    cout << " \n";

    cout << "AltTrans        : ";
    for (i=0; i<m_altTrans.getUsed(); i++)
    {
        cout << setw(w) << m_altTrans.get(i);
    }
    cout << " \n";

    cout << "NextStepsOffset : ";
    for (i=0; i<m_nextStepsOffset.getUsed(); i++)
    {
        cout << setw(w) << m_nextStepsOffset.get(i);
    }
    cout << " \n";

    cout << "NextSteps       : ";
    for (i=0; i<m_nextSteps.getUsed(); i++)
    {
        cout << setw(w) << m_nextSteps.get(i);
    }
    cout << " \n";

    cout << "PrevStepsOffset : ";
    for (i=0; i<m_prevStepsOffset.getUsed(); i++)
    {
        cout << setw(w) << m_prevStepsOffset.get(i);
    }
    cout << " \n";

    cout << "PrevSteps       : ";
    for (i=0; i<m_prevSteps.getUsed(); i++)
    {
        cout << setw(w) << m_prevSteps.get(i);
    }
    cout << " \n";

    cout << "ActionBlockOffs : ";
    for (i=0; i<m_stepActionBlockOffset.getUsed(); i++)
    {
        cout << setw(w) << m_stepActionBlockOffset.get(i);
    }
    cout << " \n";

    cout << "ActBlockAction  : ";
    for (i=0; i<m_stepActionBlocks.getUsed(); i++)
    {
        cout << setw(w) << m_stepActionBlocks.get(i);
    }
    cout << " \n";

    cout << "ActBlockQual    : ";
    for (i=0; i<m_stepActionBlockQualifier.getUsed(); i++)
    {
        cout << setw(w) << m_stepActionBlockQualifier.get(i);
    }
    cout << " \n";

    cout << "ActBlockTimePara: ";
    for (i=0; i<m_stepActionBlockTimeParameter.getUsed(); i++)
    {
        cout << setw(w) << m_stepActionBlockTimeParameter.get(i);
    }
    cout << " \n";

}

void SFC::outputColor()
{
    for (int i = 0; i<m_sfcColor.getUsed(); i++)
    {
        cout << i << "  : (" 
             << m_sfcColor.get(i).simDepth << " , " 
             << m_sfcColor.get(i).simBranch << ") \n";
    }
}
#endif //DEBUG


