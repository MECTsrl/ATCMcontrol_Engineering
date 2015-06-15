#ifndef _GrEditorBase_H
#define _GrEditorBase_H

// for automatic correction of old errors on load/store ..
#define  GR_AUTO_CORRECTION
// V1.20 can no time qualifier in sfc !
#define  V120_SFC_NOTIMEQUALIFIER


// scale factor between font and grid
// -> font uses FontToGrid % of one grid step (grid step is 100 %)
#define  FontToGrid      75

#define  GRID_BASE        8         // size of grid in pixel

#define  MAX_XGRID      800         // max x-grid size 
#define  MAX_YGRID      800         // max y-grid size 

#define  MAX_XPAGE        2         // max x-grid size 
#define  MAX_YPAGE        1         // max y-grid size 

#define  CP_RAD           3         // connect point radius
#define  SP_RAD           3         // select point radius

#define  LIGHTPIN_RANGE   3         // range in grid coords for hight light pins at edit line

#define  PIN_LENGTH       1         // length of pins in grid

#define  MAX_EXT_INOUT   64         // max. number of extensible in/out at functions

#define  SERIAL_VERSION  22         // for serialization

// frame at border of view in grids for automatic scrolling
#define SCROLL_FRAME         ( 8 * m_iGrid )

// common typedefs 
typedef enum 
{
   PT_IN,          // (left)       input pin
   PT_OUT,         // (right)      output pin
   PT_INOUT,       // (...)        inoutput pin

   PT_LAST
} PIN_TYPE;

typedef enum 
{
   COT_IN=0,       // connector input  (left)
   COT_OUT,        //           output (right)

   COT_LAST
} CON_TYPE;

typedef enum 
{
   LT_JUMP=0,      // label jump
   LT_DEST,        // label destination

   LT_LAST
} LAB_TYPE;

typedef enum 
{
   VT_IN=0,        // var input
   VT_OUT,         //     output
   VT_INOUT,       //     inoutput

   VT_LAST
} VAR_TYPE;

typedef enum 
{
   IT_ALTDIV=0,    // divergence alternativ
   IT_SIMDIV,      //            simultan

   IT_LAST
} DIV_TYPE;

typedef enum 
{
   RS_NW,          // north - west
   RS_N,           // north
   RS_NE,          // north - east
   RS_E,           // east
   RS_SE,          // south - east
   RS_S,           // south
   RS_SW,          // south - west
   RS_W,           // west

   RS_LAST
} RS_TYPE;

typedef enum       // enabled languages for editor 
{
   EL_FBD,         //  only fbd
   EL_LD,          //  only ld
   EL_ALL,         //  all 

   EL_LAST         //  
} EDIT_LANG;

typedef enum       // state of editor 
{
   EM_NORMAL,      //  normal
   EM_LINE_START,  //  line start (choose start point)
   EM_LINE,        //  line       (choose line point)
   EM_SELECT,      //  select       (choose end point)
   EM_INSERT,      //  insert mode of elements
   EM_MOVE,        //  move mode of elements, lines
   EM_DRAG_MOVE,   //  drag or move mode -> move with/without control key
   EM_RESIZE_START,//  resize start (choose element)
   EM_RESIZE_DIR,  //  resize dir selection 
   EM_RESIZE,      //  resize mode of elements
   EM_EDITPROP,    //  edit property
   EM_INSCOL,      //  insert columns
   EM_INSCOL1,     //  insert columns (disabled)
   EM_DELCOL,      //  delete columns
   EM_DELCOL1,     //  delete columns (disabled)
   EM_INSROW,      //  insert rows
   EM_INSROW1,     //  insert rows (disabled)
   EM_DELROW,      //  delete rows
   EM_DELROW1,     //  delete rows (disabled)
                   //  further edit modi ..

   EM_MONITOR,     //  monitoring is active
                   //  further monitor modi ..

   EM_LAST
} EDI_MODE;

typedef enum       // handles for cursor
{
   HC_INSCOL=0,    //  
   HC_INSCOL1,     //  
   HC_DELCOL,      //  
   HC_DELCOL1,     //  
   HC_INSROW,      //  
   HC_INSROW1,     //  
   HC_DELROW,      //  
   HC_DELROW1,     //  
   HC_PENCUR,      // 
   HC_PENCUR1,     // 
   HC_SIZEALL,     //
   HC_ARROW,       //
   HC_SIZEWE,      //
   HC_SIZENS,      //
   HC_SIZENWSE,    //
   HC_SIZENESW,    //
   HC_CROSS,       //

   HC_LAST
} HCURSOR_TYPE;

                   // types of colors
#define COL_UNUSED   RGB(255, 0, 255)  // unused color for transparent bitmaps

typedef enum 
{
   CO_NORMAL=0,    //  element normal 
   CO_SELECT,      //  element selected 
   CO_ERROR,       //  element error
   CO_COMMENT,     //  element comment
   CO_RUBBER,      //  line in rubberband mode
   CO_EDITLINE,    //  line in edit mode
   CO_GRID,        //  grid
   CO_BACKGD,      //  background of view
   CO_ELEMBK,      //  background of element CST,VAR,LAB,CMT,CON,RET
   CO_FBLKBK,      //  background of element FB/FCT/PRO
   CO_LADDBK,      //  background of element CONTACT,COIL (active)
   CO_EDGE,        //  edge of paper sheets
   CO_LIGHTPIN,    //  highlight pin
   CO_BRKREC,      //  received breakpoint (stop point)
   CO_BRKDIS,      //  inactive breakpoint (disabled)
   CO_BRKACT,      //  active, connected breakpoint  
   CO_BRKNOC,      //  active, not connected breakpoint
   CO_UNUSED,      //  unused color for transparent bitmaps
   CO_BACKGD_SFC,  //  background of view in sfc manual mode
   CO_ELEMBK_SFC,  //  background of element CST,VAR,LAB,CMT,CON,RET in sfc manual mode
   CO_LADDBK_SFC,  //  background of element CST,VAR,LAB,CMT,CON,RET in sfc manual mode
   CO_FBLKBK_SFC,  //  background of element FB/FCT/PRO in sfc manual mode
   CO_ACTIVE_SFC,  //  active sfc element
   CO_FORCED_SFC,  //  forced sfc element
   CO_BLOCKD_SFC,  //  blocked sfc element

   CO_LAST
} COLOR_TYPE; 

typedef enum 
{                  // brushs
   BT_RUBBER,
   BT_BACKGD,      // 
   BT_SELECT,
   BT_ELEMBK,
   BT_FBLKBK,
   BT_LADDBK,      // active ladder element 
   BT_LIGHTPIN,
   BT_BRKREC,      // breakpoint received, stop
   BT_BRKDIS,      //
   BT_BRKACT,      //
   BT_BRKNOC,      //
   BT_BACKGD_SFC,  //
   BT_ELEMBK_SFC,  //
   BT_LADDBK_SFC,  //
   BT_FBLKBK_SFC,  // 
   BT_ACTIVE_SFC,  //

   BT_LAST
} BRUSH_TYPE; 

typedef enum 
{
   PE_NORMAL,      // pens
   PE_SELECT,
   PE_ERROR,
   PE_COMMENT,
   PE_LABEL,
   PE_GRID,
   PE_PAGE,
   PE_SHEET,
   PE_RUBBER,
   PE_EDITLINE,
   PE_FORCED,
   PE_BLOCKED,

   PE_LAST
} PEN_TYPE; 

typedef enum       // raster steps, grid display
{
   GT_NO=0,        //  no grid display
   GT_POINT,       //  with points
   GT_CROSS,       //  with small cross
   GT_LINES,       //  with (crossed) lines
   GT_LAST
} GRID_TYPE;

typedef enum       // line draw mode
{
   LDM_ORTHO=0,    //  orthogonal
   LDM_RUBB,       //  rubberband
 
   LDM_LAST
} LINE_TYPE;

typedef enum       // move type for elements
{
   MT_SEL=0,       //  move selected elements
   MT_DX,          //  move elements above ax with dx
   MT_DY,          //  move elements above ay with dy
 
   MT_LAST
} MOVE_TYPE;

typedef enum       // sequence type for action(block)
{
   QT_none=0,      //     Non-stored (null qualifier)
   QT_N,           //  N  Non-stored
   QT_R,           //  R  overriding Reset
   QT_S,           //  S  Set (Stored)
   QT_L,           //  L  time Limited
   QT_D,           //  D  time Delayed
   QT_P,           //  P  Pulse
   QT_SD,          //  SD Stored and time Delayed
   QT_DS,          //  DS Delayed and Stored
   QT_SL,          //  SL Stored and time Limited
   QT_P0,          //  P0 Pulse, Falling Edge
   QT_P1,          //  P1 Pulse, Rising Edge

   QT_LAST
} QUAL_TYPE;

typedef enum       // ladder element type
{                  // contact coil
   LD_NORMAL,      //    x     x  normal    
   LD_CLOSED,      //    x        closed    
   LD_NEGATIVE=    //          x  negative  
     LD_CLOSED,    
   LD_POSTRANS,    //    x     x  positive transition sensing
   LD_NEGTRANS,    //    x     x  negative transition sensing
   LD_SET,         //          x  set       
   LD_RESET,       //          x   reset     

   LD_LAST
} LADDER_TYPE;

typedef enum       // element types for undo/redo
{
   ET_ACTION,      //  sfc action 
   ET_ACTIONBLOCK, //  sfc actionblock
   ET_COMMENT,     //      comment 
   ET_CONIN,       //  fbd connector
   ET_CONOUT,      //  fbd continuation
   ET_CONST,       //  fbd constant -> var read / expression
   ET_ALTDIV,      //  sfc alternativ div/convergence
   ET_SIMDIV,      //  sfc simultaneous div/convergence
   ET_FB,          //  fbd FB
   ET_FUNCTION,    //  fbd FCT
   ET_PROCEDURE,   //  fbd PRO
   ET_LABEL,       //  fbd label
   ET_JUMP,        //  fbd jump
   ET_LINE,        //      line
   ET_RETURN,      //  fbd return
   ET_STEP,        //  sfc step
   ET_TRANSITION,  //  sfc transition
   ET_VARIN,       //  fbd variable read /expression
   ET_VAROUT,      //  fbd variable write
   ET_VARINOUT,    //  fbd variable feedback
 
   ET_POWERRAIL,   //  ld  power
   ET_CONTACT,     //  ld  contact
   ET_COIL,        //  ld  coil

   ET_LAST
} ELEM_TYPE;
#endif                       // _GrEditorBase_H
