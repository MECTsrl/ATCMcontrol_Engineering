
#ifndef __LDFBD_H_
#define __LDFBD_H_


//----  Aggregate Includes:   --------------------------------------*
#include "st_ParseContext.h"
//----  Forward Class Definitions:   -------------------------------*
class LDFBD_SignalHash;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


CG_STStmt* ST_LDFBD_To_CBody(
    st_ParseContext*      pParse,     //context
    SFC_LDFBD_SignalHash& grElements, //full 4gr body also if SFC,FBD,LD
    long                  codeGenOpt  //CG_LOADBDY_* bits
);




#endif // __LDFBD_H_

