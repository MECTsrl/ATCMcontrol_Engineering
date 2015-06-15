#ifndef __RESINIT_H_
#define __RESINIT_H_





//----  Forward Class Definitions:   -------------------------------*
class CGlobCtx;
class CTarget;


//----  Prototypes:    ---------------------------------------------*
extern "C" void PC_DumpRetainFile(
    CGlobCtx* pgctx,
    CTarget*  pTrg
);

extern "C" void PC_DumpTargetInitFile(
    CGlobCtx* pgctx,
    CTarget*  pTrg
);




#endif // __RESINIT_H_


