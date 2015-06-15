#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#define ST_RES_STR_PROCEDURE         1
#define ST_RES_STR_INPUT             2 
#define ST_RES_STR_OUTPUT            3 
#define ST_RES_STR_INOUT             4

#define ST_RES_STR_NUMBER            5
#define ST_RES_STR_IDENTIFIER        6 
#define ST_RES_STR_VARIABLE          7
#define ST_RES_STR_LABEL             8 
#define ST_RES_STR_NAMED_CONST       9
#define ST_RES_STR_FILENAME          10
#define ST_RES_STR_CONF_NAME         11
#define ST_RES_STR_LIB_NAME          12

#define CG_RES_STR_VARREAD           13  //"Var Read"
#define CG_RES_STR_VARWRITE          14  //"Var Write"
#define CG_RES_STR_FEEDBACK          15  //"Var Feedback"
#define CG_RES_STR_FEEDBACK_L        16  //"Var Feedback (left pin)"
#define CG_RES_STR_FEEDBACK_R        17  //"Var Feedback (right pin)"
#define CG_RES_STR_CONNECTOR         18  //"Connector"
#define CG_RES_STR_CONTINUATION      19  //"Continuation"
#define CG_RES_STR_POWERRAIL         20  //"Powerrail"
#define CG_RES_STR_COIL              21  //"Coil"
#define CG_RES_STR_CONTACT           22  //"Contact"
#define CG_RES_STR_WIREDOR           23  //"Wiredor"
#define CG_RES_STR_FB                24  //"Function Block"
#define CG_RES_STR_FCT               25  //"Function"
#define CG_RES_STR_LABEL             26  //"Label"
#define CG_RES_STR_JUMP              27  //"Jump"
#define CG_RES_STR_RETURN            28  //"Return"
#define CG_RES_STR_TRANS_RET         29  //"Transition return"
#define CG_RES_STR_FCT_RES           30  //"Function result"
  
#define CG_RES_STR_MIX_INOUT         31  //"mixing of inputs/outputs in function interface, all inputs must preceed all outputs or vice versa"  
#define CG_RES_STR_MIX_INOUT_EXT     32  //"mixing of inputs/outputs and extended inputs in function interface, all other inputs and outputs must preceed the extended input"  
#define CG_RES_STR_INLINE_FB         33  //"inline{...} syntax for function blocks"
#define CG_RES_STR_INLINE_LOC_VAR    34  //"inline{...} functions with local variables"

#define CG_RES_STR_LIB               35  //" in library "
#define CG_RES_STR_PRECOMPILED       36  //"    was called from precompiled code of "

#define CG_RES_STR_CONNECTED_WITH    38  //" connected with "
#define CG_RES_STR_ACTBL_ALLOWED     39  //", only actionblocks allowed"

#define CG_RES_STR_SFCELEM           40  //"sfc element"
#define CG_RES_STR_NONSFCELEM        41  //"non sfc element"
#define CG_RES_STR_WRONGSFCELEM      42  //"wrong sfc element"
#define CG_RES_STR_FDBELEM           43  //"FBD element"
#define CG_RES_STR_STEP              44  //"step"
#define CG_RES_STR_STEP_IN           45  //"input of step"
#define CG_RES_STR_STEP_OUT          46  //"output of step"
#define CG_RES_STR_STEP_INOUT        47  //"input or output of step"
#define CG_RES_STR_STEPX_OUT         48  //"step.X output"
#define CG_RES_STR_ACTBL             49  //"actionblock"
#define CG_RES_STR_TRANS             50  //"transition"
#define CG_RES_STR_TRANS_IN          51  //"input of transition"
#define CG_RES_STR_TRANS_OUT         52  //"output of transition"
#define CG_RES_STR_QUALIFIER         53  //"qualifier"
#define CG_RES_STR_CONV_DIV          54  //"convergence or divergence"
#define CG_RES_STR_CONV_DIV_IN       55  //"input of convergence or divergence"
#define CG_RES_STR_CONV_DIV_OUT      56  //"output of convergence or divergence"

#define CG_RES_STR_PREDS             57  //"predecessors"
#define CG_RES_STR_SUCCS             58  //"successors"
#define CG_RES_STR_PRED              59  //"predecessor"
#define CG_RES_STR_SUCC              60  //"successor"

#define CG_RES_STR_MULTI_STEP        61  //"transition has multiple successor steps connected via direct link"

#define CG_RES_STR_NOOVERLOAD        62  //"no overload takes "
#define CG_RES_STR_PIN_IN            63  //" input pins"
#define CG_RES_STR_PIN_OUT           64  //" output pins"

#define CG_RES_STR_NONSIMPLEDTYPE    65  //"LD/FBD: non simple datatype as function output (forcing a temporary variable)"

#define CG_RES_STR_OR_NOEFFECT       66  //"Wired Or: has no effect (one input is power rail)"
#define CG_RES_STR_OR_BADRSIG        67  //"Wired Or: looks like FDB: signal source is a "

#define CG_RES_STR_BADLEFTSIGNAL     68  //"bad left signal source"
#define CG_RES_STR_OPENRIGHTSIDE     69  //"right side must be open or (right) powerrail"
#define CG_RES_STR_EN_NOTNEGATED     70  //"EN/ENO cannot be negated"
#define CG_RES_STR_ENO_NOTSPLIT      71  //"ENO signal must not be split"
#define CG_RES_STR_ENO_NOTCONNECT    72  //"ENO not directly connected to next EN, jump or return"
#define CG_RES_STR_EN_SIGNALSPLIT    73  //"signal split at in/output of EN/ENO box not allowed"));
#define CG_RES_STR_EN_BADINPUT       74  //"bad input signal: neither VarRead nor previous EN/ENO output"
#define CG_RES_STR_EN_BADOUTPUT      75  //"bad output signal: neither VarWrite nor input of next EN/ENO"
#define CG_RES_STR_EN_NOTCONNECT     76  //"EN not connected to ENO (better use second network?)"
#define CG_RES_STR_OR_NOINPUT        77  //"Wired Or without inputs"
#define CG_RES_STR_OR_NOOUTPUT       78  //"Wired Or without outputs"
#define CG_RES_STR_BAD_POWERRAIL     79  //"powerrail connected from both sides"
#define CG_RES_STR_REFFEEDBACK       80  //"Var Feedback not supported in LD language"

#define CG_RES_STR_NONAME            81  //"'function result pin' should have no name in layout"
#define CG_RES_STR_IN_PIN            82  //"input pin"
#define CG_RES_STR_OUT_PIN           83  //"output pin"
#define CG_RES_STR_NOTIN             84  //"is not an input"
#define CG_RES_STR_NOTOUT            85  //"is not an output"
#define CG_RES_STR_WRONGPOS          86  //"is at wrong vertical position"

#define CG_RES_STR_OPEN_SIMCON       87  //"Not all input branches of a simultaneous convergence reached."
#define CG_RES_STR_UNSAFE_FINAL      88  //"Final node under simultaneous divergence."
#define CG_RES_STR_UNSAFE_LOOP       89  //"Profileration of tokens. Loop of one branch of a simultaneous divergence."
#define CG_RES_STR_OPEN_ALTDIV       90  //"Convergence missing under alternative divergence."
#define CG_RES_STR_TOKEN_JUMP        91  //"Token entering a branch of a simultaneous sequence from outside that branch."
#define CG_RES_STR_STEPNOTREACHED    92  //"Step is never reached: "

#define CG_RES_STR_SFC_NO_INISTEP    93  //"SFC has no initial step"
#define CG_RES_STR_SFC_MULTI_INISTEP 94  //"SFC has multiple initial steps"
#define CG_RES_STR_STEP_ALREADY_DEF  95  //"is already defined as step"
#define CG_RES_STR_TRANS_ALREADY_DEF 96  //"is already defined as transition"

#define CG_RES_STR_TRANS_NOPRED      97  //"transition has no predecessor step"
#define CG_RES_STR_TRANS_NOSUCC      98  //"transition has no successor step"
#define CG_RES_STR_SIMCONV_ONEPRED   99  //"simultaneous convergence has only one predecessor step"
#define CG_RES_STR_ALTCONV_MULTPRED  100 //"alternative convergence has multiple predecessor steps"
#define CG_RES_STR_SIMDIV_ONESUCC    101 //"simultaneous divergence has only one successor step"
#define CG_RES_STR_ALTDIV_MULTSUCC   102 //"alternative divergence has multiple successor steps"

#define CG_RES_STR_ALREADY_PRED      103 //"is already predecessor of another transition"
#define CG_RES_STR_ALREADY_SUCC      104 //"is already successor of another transition"

#define CG_RES_STR_TIME_QUALIFIER    105 //"time qualifier"

#define CG_RES_STR_DECL_RETAIN       106 //" cannot be declared [NON_]RETAIN"
#define CG_RES_STR_PRG_USING         107 //"programs using "  eg
#define CG_RES_STR_ARRAY_BASE        108 //"arrays of base type"
#define CG_RES_STR_DADDR_WOUT_NAME   109 //"direct address without symbolic name"
#define CG_RES_STR_DADDR_WITH_MEM    110 //"direct address with memory location 'M'"
#define CG_RES_STR_DADDR_VARCONFIG   111 //"incomplete direct address in VAR_CONFIG style"
#define CG_RES_STR_DADDR_OPC         112 //"incomplete direct OPC address using '""<field>""'"

#define CG_RES_STR_DATATYPE          113 //"data type "
#define CG_RES_STR_USEDLIT           114 //" used literal "
#define CG_RES_STR_LONGERTHAN        115 //" longer than "
#define CG_RES_STR_CHARS             116 //" characters"

#define CG_RES_STR_NOFORMAT          117 //"no [FORMAT] section found"
#define CG_RES_STR_NOKEY             118 //"no 'format=' key found"
#define CG_RES_STR_UNEXPKEY          119 //"unexpected second format key"
#define CG_RES_STR_FCT_WRONG_PARAM   120 //"function parameter passing using <formal>:=<actual>"

#define CG_RES_STR_RESATTR1          121 //"\n   resource attributes (*[[.. ]]*) after resource name\n"
#define CG_RES_STR_RESATTR2          122 //"\n  (*[[.. ]]*)\n  RESOURCE <name> ON ...."

#define CG_RES_STR_NL                123 //"<new line>"
#define CG_RES_STR_EOF               124 //"<end of file>"
#define CG_RES_STR_EMPTYHDR          125 //"<empty header>"
#define CG_RES_STR_IDENTIFIER        126 //"<identifier>"
#define CG_RES_STR_SFC_LANG          127 //"SFC language"
#define CG_RES_STR_FBD_LANG          128 //"FBD language"
#define CG_RES_STR_LADDER_LANG       129 //"Ladder language"
#define CG_RES_STR_USEDCONST         130 //" (used CONST "
#define CG_RES_STR_FROM              131 //" from "
#define CG_RES_STR_ALREADY_DEF       132 //", already defined in "
#define CG_RES_STR_GLOBALINRES       133 //"VAR_GLOBAL in resource, use #import ""<file>.gvl"" instead"

#define CG_RES_STR_STEP_NAME         134 //"step-name"
#define CG_RES_STR_ACTBL_NAME        135 //"action block-name"

#define CG_RES_STR_ST1               136 //"white space before/after '.' in system comment"
#define CG_RES_STR_ST2               137 //"VAR_GLOBAL in configuration scope"
#define CG_RES_STR_ST3               138 //"RESOURCE declaration in .con file, use #import ""<file>.res"" instead"
#define CG_RES_STR_ST4               139 //"program configuration elements following the program type name"
#define CG_RES_STR_ST5               140 //"'leading NATIVE block' "
#define CG_RES_STR_ST6               141 //"overloads: using '|' for non basic types"
#define CG_RES_STR_ST7               142 //"<identifier>"
#define CG_RES_STR_ST8               143 //"\n     inline is now a keyword, don't need #pragma"
#define CG_RES_STR_ST9               144 //"'#pragma overload <type list>'"
#define CG_RES_STR_ST10              145 //"\n    'overload <type list>'"
#define CG_RES_STR_ST11              146 //"\n    overload is now a keyword, don't need #pragma"
#define CG_RES_STR_ST12              147 //"declaration keyword inside body?"
#define CG_RES_NS_COMPL_TYP_FUN_OUT  148 //"array or structure as function output parameter"
#define CG_RES_NS_AT_SYSVAR          149 //"'AT <sys var>' syntax: no system variables defined"
#define CG_RES_STR_GT_VAR_EXT        150 //EQ,NE,LT,LE,GT,GE for more than 2 arguments

#define CG_RES_LD_VARW_BAD_USE       250 //"Var Read(Var Write) unexpected here (looks like FBD), use Contact(Coil) instead"

#endif // __RESOURCE_H_


