/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/Parser.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Parser.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/Parser.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Parser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const unsigned short    ASM_STORE   = 0xFF00;      // neuer Opcode != allen IpCodes
const unsigned short    ASM_RESTORE = 0xFF01;      // neuer Opcode != allen IpCodes
const unsigned short    ASM_POPV    = 0xFF02;      // neuer Opcode != allen IpCodes
const unsigned short    ASM_MOVE    = 0xFF03;      // neuer Opcode != allen IpCodes
const unsigned short    ASM_REMOVE  = 0xFF04;      // neuer Opcode != allen IpCodes
const unsigned short    ASM_I2F     = 0xFF05;      // neuer Opcode != allen IpCodes

const unsigned long SEGMENT     = 0xFFFF0000;   // Segmentmaske

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CParser::CParser()
{
    urlnumber = 0;
    m_iList              = 0;
    m_iDebugNames        = 0;
	m_iTrashBreakpoints  = 0;
    m_IP_LoadAddress     = 0; 
    m_NC_LoadAddress     = 0; // is relocable
    m_StackAlignment     = CCodec::stackAlignment;
    m_InstAlignment      = CCodec::instAlignment;
    m_FirmwareVersion    = 0; 
	m_b16BitProc		 = FALSE;
    m_CodeSegSize        = 0; 

    m_max_calls          = 0;
    m_max_index_entries  = 0;
    m_max_object_entries = 0;
}

CParser::~CParser()
{
   
}

void CParser::pars(CFile& NccFile, CStdioFile& ListFile, XDownHeader& DownHeader, unsigned char *pBuffer, Cxxx_dbp& xxx_dbp)
{
    int            size, index, index_0, change;
    unsigned long  ulSize;

    CCodec::stackAlignment  = m_StackAlignment;
    CCodec::instAlignment   = m_InstAlignment;
    CCodec::firmwareVersion = m_FirmwareVersion;
    CCodec::iList           = m_iList;
    if (m_max_calls) 
        CCodec::maxCalls         = m_max_calls;
    if (m_max_index_entries) 
        CCodec::maxIndexEntries  = m_max_index_entries;
    if (m_max_object_entries) 
        CCodec::maxObjectEntries = m_max_object_entries;
    // Init
    IpCodeList.RemoveAll();
    IpCodeList.SetSize(0, 100);
    NccCodeList.RemoveAll();
    NccCodeList.SetSize(0, 100);
    MapIpOffset.RemoveAll();
    MapIpLabel.RemoveAll();
    MapNccLabel.RemoveAll();

    // create IpCodeList 
    if( m_iList & option_IpList ) {
        ListFile.WriteString(CString("========== IP ============================================\n") );
    }
    scanner(ListFile, DownHeader, pBuffer, xxx_dbp);
    // Init Codec
    Codec.Init();

	if(bitsizeVM < 32) // normalize segmented addresses
		m_NC_LoadAddress = 0;

    if( m_iList & option_NccList ) {
        ListFile.WriteString(CString("========== NC ============================================\n") );
    }
    // strip in Code-Blocks 1 for check
    size = IpCodeList.GetSize();
    do {
        index_0 = 0;
        index   = 0;
        change  = 0;
        while( index+1 < size ) {
            // Block beginnt auf Haltepunkt und endet einen Befehl davor
            while( (index+1 < size) && (IpCodeList[index+1].BpIndex == -1) ) index++;
            // check IpCodeList
            if( checkBlock(index_0, index) ) 
                {change = 1; break;}  //ASM_RETN ist IP
            index_0 = ++index;
        }
        // check IpCodeList (Rest )
        if( index < size ) {
            if( checkBlock(index_0, index) ) 
                change = 1;  //ASM_RETN ist IP
        }
        change |= checkJmp();
    } while( change );
    
    // strip in Code-Blocks 2 for generate
    index_0 = 0;
    index   = 0;
    size    = IpCodeList.GetSize();
    while( index+1 < size ) {
        // Block beginnt auf Haltepunkt und endet einen Befehl davor
        while( (index+1 < size) && (IpCodeList[index+1].BpIndex == -1) ) index++;
        // generate NccCodeList
        generateBlock(index_0, index);
        index_0 = ++index;
    }
    // generate NccCodeList (Rest )
    if( index < size )
        generateBlock(index_0, index);
   
    // link modul
    DownHeader.ulSize = linkNCC(ListFile, xxx_dbp);
   
    // store modul
    XDownHeader DownHeaderB;
    if( big_endian == 1 ) 
    {
       DownHeaderB.uIndex   = ((DownHeader.uIndex   & 0x00FF) << 8) | ((DownHeader.uIndex   & 0xFF00) >> 8);
       DownHeaderB.uSegment = ((DownHeader.uSegment & 0x00FF) << 8) | ((DownHeader.uSegment & 0xFF00) >> 8);
       DownHeaderB.ulSize   = 
          ((DownHeader.ulSize & 0x000000FF) << 24) | 
          ((DownHeader.ulSize & 0x0000FF00) <<  8) |
          ((DownHeader.ulSize & 0x00FF0000) >>  8) |
          ((DownHeader.ulSize & 0xFF000000) >> 24);
       DownHeaderB.ulOffset   = 
          ((DownHeader.ulOffset & 0x000000FF) << 24) | 
          ((DownHeader.ulOffset & 0x0000FF00) <<  8) |
          ((DownHeader.ulOffset & 0x00FF0000) >>  8) |
          ((DownHeader.ulOffset & 0xFF000000) >> 24);
    } else {
       DownHeaderB = DownHeader;
    }
    NccFile.Write(&DownHeaderB, sizeof(DownHeaderB));
    size = NccCodeList.GetSize();
    for(index = ulSize = 0; index < size; index++) 
    {
      NccFile.Write(NccCodeList[index].NccCode, NccCodeList[index].nNccCode);
      ulSize += NccCodeList[index].nNccCode;

      if( NccCodeList[index].pSTRING )
      {
         NccFile.Write(NccCodeList[index].pSTRING, NccCodeList[index].pSTRING[1]+2);
         ulSize += NccCodeList[index].pSTRING[1]+2;
      }
      if( NccCodeList[index].seg_size ) {
         if( NccCodeList[index].isNative == IP_CODE) {
            unsigned char asm_nop = ASM_NOP;
            for(unsigned long i = 0; i < NccCodeList[index].seg_size; i++)
               NccFile.Write(&asm_nop, 1);
            ulSize += NccCodeList[index].seg_size;
         } else {
            int nopsize = CCodec::ASM_NOP().size;
            for(unsigned long i = 0; i < NccCodeList[index].seg_size / nopsize; i++)
               NccFile.Write(CCodec::ASM_NOP().buffer, nopsize );
            ulSize += NccCodeList[index].seg_size;
         }
      }
    }
    ASSERT(DownHeader.ulSize == ulSize);
}

int CParser::scanner(CStdioFile& ListFile, XDownHeader& DownHeader, unsigned char *pNcc, Cxxx_dbp& xxx_dbp)
{
    unsigned char*   pend    = pNcc + DownHeader.ulSize;
    unsigned char*   pNcc_1  = pNcc;
    unsigned char*   pNcc_0  = pNcc;
    unsigned short   op;
    COpcode          OpcodeElem;
    int              i, index, size;
    unsigned long    labelCounter = 0;
    CString          line;
    char             BreakPointType;
   
    // IP-Code byteweise resssemblieren
    while( pNcc < pend ) {
        // Init
        CIpCodeElem     IpCodeElem;

        // Position 
        IpCodeElem.offset = pNcc - pNcc_0;
        IpCodeElem.label  = ++labelCounter;

        // Präfix ?
        switch(*pNcc) {
        case  PREF_WIDE8:       IpCodeElem.ParSize  =  1; IpCodeElem.Praefix = *pNcc++; goto MainTable;
        case  PREF_WIDE16:      IpCodeElem.ParSize  =  2; IpCodeElem.Praefix = *pNcc++; goto MainTable;
        case  PREF_WIDE32:      IpCodeElem.ParSize  =  4; IpCodeElem.Praefix = *pNcc++; goto MainTable;
        case  PREF_WIDE64:      IpCodeElem.ParSize  =  8; IpCodeElem.Praefix = *pNcc++; goto MainTable;
 
        case  PREF_CONV:       
            IpCodeElem.OpCode   = *pNcc++;
            IpCodeElem.OpCode <<= 8;
            IpCodeElem.OpCode  += *pNcc++;
            OpcodeElem          = ASM_getOpcode(IpCodeElem.OpCode);
            break;

        case  PREF_ARITH:      
            IpCodeElem.OpCode   = *pNcc++;
            IpCodeElem.OpCode <<= 8;
            op                  = *pNcc++;
            IpCodeElem.OpCode  += op;
            OpcodeElem          = ASM_getOpcode(IpCodeElem.OpCode);
            break;
        default:    
MainTable:
            IpCodeElem.OpCode = op = *pNcc++;
			op += (IpCodeElem.Praefix << 8);
            OpcodeElem          = ASM_getOpcode(op);
        }
        if(OpcodeElem.dataCount == 0) {
            OpcodeElem.dataCount = IpCodeElem.ParSize * 8;
        }
        
		// COpcode auswerten
        if( OpcodeElem.live == live_disable ) {
            CString str;
            str.Format("Ipcode Code: 0x%4x Adr: %05d", IpCodeElem.OpCode, IpCodeElem.offset);
            ASSERT(_ASSERT_); 
            throw CFormatException(str);
        }
        IpCodeElem.OpcodeString   = OpcodeElem.OpcodeString;
        IpCodeElem.type           = OpcodeElem.type;
        IpCodeElem.live           = OpcodeElem.live;
        IpCodeElem.dataCount      = OpcodeElem.dataCount;
        IpCodeElem.parCount       = OpcodeElem.parCount;
  
        // Parameter auswerten
        // Sonderfall Stringkonstante
        if( IpCodeElem.OpCode == ASM_PSHC_TX ) 
        {
            IpCodeElem.parCount = 0;
            op = *pNcc+2;   // len
            IpCodeElem.pSTRING = new unsigned char[op];
            memcpy(IpCodeElem.pSTRING, pNcc, op);
            pNcc += op;
        }

        if( IpCodeElem.parCount > 0 ) {
            if( big_endian == 0 ) {
                memcpy(IpCodeElem.Par1.uc8, pNcc, IpCodeElem.ParSize);
                pNcc += IpCodeElem.ParSize;
            } else {
                for( i = 0; i < IpCodeElem.ParSize; i++ ) {
                    IpCodeElem.Par1.uc8[IpCodeElem.ParSize - 1 - i] = *pNcc++;
                }
            }
        }

        if( IpCodeElem.parCount > 1 ) {
            if( big_endian == 0 ) {
                memcpy(IpCodeElem.Par2.uc8, pNcc, IpCodeElem.ParSize);
                pNcc += IpCodeElem.ParSize;
            } else {
                for( i = 0; i < IpCodeElem.ParSize; i++ ) {
                    IpCodeElem.Par2.uc8[IpCodeElem.ParSize - 1 - i] = *pNcc++;
                }
            }
        }

        if( IpCodeElem.parCount > 2 ) {
            if( big_endian == 0 ) {
                memcpy(IpCodeElem.Par3.uc8, pNcc, IpCodeElem.ParSize);
                pNcc += IpCodeElem.ParSize;
            } else {
                for( i = 0; i < IpCodeElem.ParSize; i++ ) {
                    IpCodeElem.Par3.uc8[IpCodeElem.ParSize - 1 - i] = *pNcc++;
                }
            }
        }

        // Breakpoint suchen
        index = xxx_dbp.isBreakpoint(pNcc_1 - pNcc_0);
        IpCodeElem.BpIndex = index;
        if( index != -1 ) {
            IpCodeElem.BpNumber = xxx_dbp[index].stLineNumber;
            BreakPointType      = xxx_dbp[index].cTyp;
        }
        // Sprünge berechnen
        {
            unsigned short OpCode = IpCodeElem.OpCode;
            switch( OpCode ) {
                case ASM_JMP:
                case ASM_JMPC:
                case ASM_JMPCN:
                ASSERT(IpCodeElem.parCount == 1);
                // nur relative Sprünge im Modul !
                switch (IpCodeElem.ParSize) {
                    case 1:  IpCodeElem.jmpto = IpCodeElem.Par1.i1; break;
                    case 2:  IpCodeElem.jmpto = IpCodeElem.Par1.i2; break;
                    case 4:  IpCodeElem.jmpto = IpCodeElem.Par1.i4; break;
                    case 8:  ASSERT(_ASSERT_); break;
                    default: ASSERT(_ASSERT_); break;
                }
            }
        }
  
        // NccCode kopieren
        if((*pNcc_1 == ASM_BREAK || *pNcc_1 == ASM_ACTIVE_BREAK) && m_iTrashBreakpoints) {
            IpCodeElem.nNccCode = 0;
        } else if( !IpCodeElem.pSTRING ) {
            IpCodeElem.nNccCode = pNcc - pNcc_1;
        } else {
            if (IpCodeElem.OpCode & 0xFF00) {
                IpCodeElem.nNccCode = 2;
            } else {
                IpCodeElem.nNccCode = 1;  // OpCode in Main table  !
            }
        }
  
        memcpy(&IpCodeElem.NccCode, pNcc_1, IpCodeElem.nNccCode);
  
        // in Liste
        IpCodeList.Add( IpCodeElem );
        // MapIpOffset mit Key = Offset und Value = Index in IpCodeList
        MapIpOffset.SetAt((void*)IpCodeElem.offset, (void*)IpCodeList.GetUpperBound());

        // Listing
        if( m_iList & option_IpList ) 
        {
            line = IpCodeElem.Format(m_IP_LoadAddress, BreakPointType) + CString('\n');
            ListFile.WriteString( line );
        }
        pNcc_1 = pNcc;
    }

    // Sprungziele in Label umrechnen
    size  = IpCodeList.GetSize();
    for(index = 0; index < size; index++) {
        if( IpCodeList[index].jmpto != 0 ) {
            void*   IpOffset;   
            bool    found = false;
            int     dest  = IpCodeList[index].offset + IpCodeList[index].jmpto;

            if (MapIpOffset.Lookup((void*)dest, IpOffset)) {
                IpCodeList[index].jmpto = IpCodeList[(int)IpOffset].label;
                // MapIpLabel mit Key = Label und Value = Index in IpCodeList
                MapIpLabel.SetAt((void*)(IpCodeList[(int)IpOffset].label), IpOffset);

            } else {
                CString str;
                str.Format("Ipcode Jmp : 0x%04x Adr: %05d", IpCodeList[index].OpCode, IpCodeList[index].offset);
                ASSERT(_ASSERT_);
                throw CFormatException(str);
            }
        }
    }

    return 1;
}

int CParser::skipEmptyIpBlock(int  index)
{
    int size = IpCodeList.GetSize();
    // 1. Block != BREAK suchen
    while( index < size && !IpCodeList[index].nNccCode ) index++;   // BREAK übergehen
    return index;
}

int CParser::skipEmptyNccBlock(int  index)
{
    int size = NccCodeList.GetSize();
    // 1. Block != BREAK suchen
    while( index < size && !NccCodeList[index].nNccCode ) index++;   // BREAK übergehen
    return index;
}

int CParser::checkBlock(int index_b, int index_e)
{
    bool    live = true;
    int     index;
    int     stack_depth;
    int     change;

   
    stack_depth = 0;
    change      = 0;
    for(index = skipEmptyIpBlock(index_b), live = (index<=index_e); index <= index_e; index++) 
    {
      switch(IpCodeList[index].type)
      {                 /* vorher = ?, nachher = ? */
      case type_push    : stack_depth += 1;     break; /* 0, 1 */
      case type_op1     : stack_depth += 0;     break; /* 1, 1    ; z.B. NOT aber auch alle PUSH-Vektor-Befehle*/
      case type_op2     : stack_depth -= 1;     break; /* 2, 1    ; z.B. ADD */
      case type_pop     : stack_depth -= 1;     break; /* 1, 0 */
      case type_statm   : stack_depth += 0;     break; /* 0, 0 */
      case type_cc      : stack_depth -= 1;     break; /* 1, 0    ; bedingte Sprünge ...*/
      case type_par     : stack_depth += 1;     break; /* 0, 1    ; formaler Parameter*/
      case type_op3     : stack_depth -= 2;     break; /* 3, 1    ; z.B. SEL, MUX -> stets IP*/
      case type_popv    : stack_depth -= 2;     break; /* 2, 0    ; POP Vektor Befehle*/
      case type_fct     : stack_depth += 100;   break; /* N, 1 */
      default: ASSERT(_ASSERT_); break;
      }
      if ((IpCodeList[index].live != live_nc) ||
          ((IpCodeList[index].OpCode == ASM_JMP) && 
           (stack_depth != 0)))
      {
            live = false;
            break;
      }
      if ((IpCodeList[index].OpCode == ASM_JMP  ) ||
          (IpCodeList[index].OpCode == ASM_JMPC ) ||
          (IpCodeList[index].OpCode == ASM_JMPCN))
      {
        if ((IpCodeList[index].jmpto > +0x1000) ||
             (IpCodeList[index].jmpto < -0x1000))
         {
               live = false;
               break;
         }
      }
      else if(IpCodeList[index].dataCount > bitsizeVM) 
      {
            live = false;
            break;
      }
    }
/*
	// There is no relation betwenn start and end of the POU
	// this is handeled right on cRETURN function (implemented in control)

	bool    ret;
    bool    live_1;
	int     findex;

    // Der letzte Block muss ASM_RETN enthalten, er ist NC, wenn auch der erste Block NC ist.
    ret     = false;
    for(index = index_b; index <= index_e; index++) {
      if ( IpCodeList[index].OpCode == ASM_RETN  ) {
          // letzten Block gefunden
          ret = true;
      }
    }
    if( ret ) {        // Returnblock gefunden
        // 1. Block != BREAK suchen
        findex   = skipEmptyIpBlock(0);
        live_1  = IpCodeList[findex].isNative == NC_CODE;
        // Wenn der Block mit IP beginnt, muss er auch als IP-Enden!
        if( live_1 == false ) live = false;
        else if( live == false ) {
            if( ASM_getOpcode(ASM_RETN).live == live_nc ) {
                ASSERT(_ASSERT_); 
                throw CFormatException("ASM_RETN must be native");
            } 
        }
    }
*/
    // set IP_ oder Native
    for(index = index_b; index <= index_e; index++) 
    {
        IpCodeList[index].isNative = live ? NC_CODE : IP_CODE;
    }

    return change;
}


int CParser::checkJmp(void)
{
    int     size, index, lindex, findex, label, mode, change;
    void*   pvalue;

    size    = IpCodeList.GetSize();
    change  = 0;
    findex  = skipEmptyIpBlock(0);
    for(index = 0; index < size; index++) {
        if( (label=IpCodeList[index].jmpto) != 0 ) {
            if( !MapIpLabel.Lookup((void*)label, pvalue)) {
                ASSERT(_ASSERT_);
            }
            lindex  =  skipEmptyIpBlock((int)pvalue);
            mode    =  IpCodeList[index ].isNative + 
                       IpCodeList[lindex].isNative * 2;
            switch( mode ) {
            case 0: // IP -> IP: OK
                        break;
            case 1: // NC -> IP: Set Source to IP, not if [index-1] is NC
                        if( (lindex == 0) || !IpCodeList[lindex-1].isNative ) {
                            IpCodeList[index].live = live_ip;

if ((index == size-1) && (IpCodeList[index].OpCode == ASM_RETN))
{
   IpCodeList[findex].live = live_ip;
}
                            change = 1;
                        }
                        break;
            case 2: // IP -> NC: Set Dest   to IP
                        IpCodeList[lindex].live = live_ip;;
if ((lindex == size-1) && (IpCodeList[lindex].OpCode == ASM_RETN))
{
   IpCodeList[findex].live = live_ip;
}
                        change = 1;
                        break;
            case 3: // NC -> NC: OK
                        break;
            }
        }
    }
    return change;
}

int CParser::generateBlock(int index_b, int index_e)
{
    int index;
    
    if( !IpCodeList[index_b].isNative ) {
        // copy code
        for(index = index_b; index <= index_e; index++) {
            CNccCodeElem        NccCodeElem;
            
            NccCodeElem = IpCodeList[index];
            NccCodeElem.isNative = IP_CODE;
            NccCodeList.Add(NccCodeElem);
            // MapNccLabel mit Key = Label und Value = Index in NccCodeList
            MapNccLabel.SetAt((void*)(NccCodeElem.label), (void*)(NccCodeList.GetUpperBound()));
            
        }
    } else {
        // generate nc
        CNccCodeElem NccCodeElem;
        NccCodeElem.nNccCode = 0;
        NccCodeElem.live = live_nc;
        NccCodeElem.isNative = 1;
        bool checkExec = FALSE;

        for(index = index_b; checkExec == FALSE && index <= index_e; index++) {
            CNccCodeElem elem;
            elem = IpCodeList[index];
            switch(elem.OpCode) {
                case ASM_JMP:  case ASM_JMPC:   case ASM_JMPCN:
                    if(elem.Par1.ui4 & (0x80 << 8 * (elem.ParSize-1)))
                        checkExec = TRUE;
            }
        }
        NccCodeElem.AddOpcode(Codec.OnStatement(checkExec));
        NccCodeList.Add(NccCodeElem);
               
        CNccCodeList CodeList;          
        parsRPN(index_b, index_e, CodeList);
        generateNCC(CodeList);       
    }
    
    return 1;
}

/*
 Arbeit mit Labels:
 Das 1. Element einer Codesequenz kann nur vom Typ type_push oder type_statm sein,
 da nur diese kein Element entkellern.
 Beim 1. generierten NccCodeElement wird das gespeicherte Label eingetragen.
*/
void CParser::add(CNccCodeList& CodeList, CNccCodeElem& elem)
{
   if( index_j != -1 ) {
      // Label in ersten generierten Block einsetzen
      elem.label   = IpCodeList[index_j].label;
      elem.BpIndex = IpCodeList[index_j].BpIndex;
      elem.BpNumber= IpCodeList[index_j].BpNumber;
      index_j = -1;
   } else {
      // Label löschen
      elem.label   = -1;
      elem.BpIndex = -1;
      elem.BpNumber= 0;
   }
   CodeList.Add(elem);
}

int CParser::parsRPN(int index_b, int index_e, CNccCodeList& CodeList)
{
    // 276f
    CNccCodeList        Keller;                         // Symbolkeller
    CNccCodeElem        symbol,                         // akt. Symbol aus Eingabestrom
                        operand1, operand2,             // temp. Operanden
                        ergebnis,                       // Symbol des Akku-Inhaltes
                        tmpelem;                      
    int                 i, index, hmax = 0, kp = 0, fdc;
    int                 rflag = 0, datacount, size, tkp, pcount;
    int                 o1ftype, o2ftype, oftype;       // old ftype; force Vector ?

    index        = index_b;
    index_j      = -1;
    ergebnis.url = undefined;
    Keller.SetSize(100, 100);
    while( index <= index_e ) {
	  rflag          = 0;
      symbol         = IpCodeList[index];
      symbol.h       = -1;
      // Labelindex speichern
      if( ( (symbol.type == type_push )  || 
            (symbol.type == type_statm)  || 
            (index_b     == index_e   )   ) && 
          (index_j == -1)                   && 
          (symbol.label != -1)                 ) 
      {
        index_j = index;
      }
      // Label löschen
      symbol.label   = -1;
      symbol.BpIndex = -1;
      symbol.BpNumber=  0;

      switch( symbol.type  ) {
      case type_push:     
         symbol.url   = getURL();
         symbol.OperatorCode = ASM_NOP;
         symbol.ftype = 0;
         Keller[kp++] = symbol;
         break;
      case type_op1:      
          if(--kp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
         operand1       = Keller[kp];
         o1ftype        = operand1.ftype;
         operand1.ftype = ASM_getFType(symbol.OpCode, ftin1);
         if( ergebnis.url == undefined ) {
            add(CodeList, operand1); // load
         } else {
            if( ergebnis.url != operand1.url ) {
               // save old operand1 in Hilfspeicher[hmax]
               ergebnis.OperatorCode   = 0;
               ergebnis.OpCode         = ASM_STORE;
               ergebnis.Par1.ui4       = hmax;
               add(CodeList, ergebnis);
               add(CodeList, operand1); // load
            }
         }
         // evtl. Akku tauschen; Vektorzugriff
         if( exchangeTest(o1ftype, operand1.ftype) ) {
            // Festkomma-Akku in Gleitkomma-Akku umladen
            tmpelem.OperatorCode   = 0;
            tmpelem.ftype          = operand1.ftype;
            tmpelem.OpCode         = ASM_I2F;
            add(CodeList, tmpelem);
         }
         // Operation anweisen
         symbol.OperatorCode = symbol.OpCode;
         symbol.OpCode = 0;
         add(CodeList, symbol);
         // Hilfsspeicherverwaltung
         if( operand1.h != -1 ) hmax--;
         hmax++;
         ergebnis.h             = hmax;
         ergebnis.url           = getURL();
         ergebnis.offset        = -1;
         ergebnis.OperatorCode  = 0;
         ergebnis.OpCode        = ASM_RESTORE;
         ergebnis.Par1.ui4      = hmax;
         ergebnis.dataCount     = symbol.dataCount;
         ergebnis.ftype         = ASM_getFType(symbol.OperatorCode, ftout);
         Keller[kp++]           = ergebnis;
         break;
      case type_op2: 
         if(--kp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
         operand2       = Keller[kp];
         o2ftype        = operand2.ftype;
         operand2.ftype = ASM_getFType(symbol.OpCode, ftin2);
         if(--kp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
         operand1       = Keller[kp];
         o1ftype        = operand1.ftype;
         operand1.ftype = ASM_getFType(symbol.OpCode, ftin1);
         // Operanden laden
         if( ergebnis.url == undefined ) {
            // *** Fall 1: Akku leer -> Lade op1 in Akku, Lade Op2 in Register
            add(CodeList, operand1); // load
            // evtl. Akku tauschen; Vektorzugriff
            if( exchangeTest(o1ftype, operand1.ftype) ) {
                // Festkomma-Akku in Gleitkomma-Akku umladen
                tmpelem.OperatorCode   = 0;
                tmpelem.ftype          = operand1.ftype;
                tmpelem.OpCode         = ASM_I2F;
                add(CodeList, tmpelem);
                o1ftype                = 0;
            }
         } else {
            if( ergebnis.url == operand2.url ) {
            // *** Fall 2: Akku == Op2 -> Akku nach Register, Lade op1 in Akku
               // Optimiert zum Stack- und Zeitsparen

                // evtl. Akku tauschen; Vektorzugriff
                if( exchangeTest(o2ftype, operand2.ftype) ) {
                    // Festkomma-Akku in Gleitkomma-Akku umladen
                    tmpelem.OperatorCode   = 0;
                    tmpelem.ftype          = operand1.ftype;
                    tmpelem.OpCode         = ASM_I2F;
                    add(CodeList, tmpelem);
                    o2ftype                = 0;
                }
                // swap op1 -> op2
                ergebnis.ftype = operand1.ftype; oftype  = o2ftype;
                operand2 = operand1;             o2ftype = o1ftype;
                operand1 = ergebnis;             o1ftype = oftype;
                if( !isKommutativ(symbol.OpCode) ) {
                    operand2.swapFlag = 1;
                    operand2.swapftype= operand1.ftype;
                }
            } else if( ergebnis.url != operand1.url ) {
                // *** Fall 3: Akku belegt -> Akku kellern, Lade op1 in Akku, Lade Op2 in Register
                // evtl. Akku tauschen; Vektorzugriff
                // save old operand1 in Hilfspeicher[hmax]
                ergebnis.OperatorCode   = 0;
                ergebnis.OpCode         = ASM_STORE;
                ergebnis.Par1.ui4       = hmax;
                add(CodeList, ergebnis);
                add(CodeList, operand1); // load
                // evtl. Akku tauschen; Vektorzugriff
                if( exchangeTest(o1ftype, operand1.ftype) ) {
                    // Festkomma-Akku in Gleitkomma-Akku umladen
                    tmpelem.OperatorCode   = 0;
                    tmpelem.ftype          = operand1.ftype;
                    tmpelem.OpCode         = ASM_I2F;
                    add(CodeList, tmpelem);
                    o1ftype                = 0;
                }
            }
            else {
                // else *** Fall4: Akku = Op1 -> Lade Op2 in Register
                // evtl. Akku tauschen; Vektorzugriff
                if( exchangeTest(o1ftype, operand1.ftype) ) {
                    // Festkomma-Akku in Gleitkomma-Akku umladen
                    tmpelem.OperatorCode   = 0;
                    tmpelem.ftype          = operand1.ftype;
                    tmpelem.OpCode         = ASM_I2F;
                    add(CodeList, tmpelem);
                    o1ftype                = 0;
                }
            }
         }
         // Op1 steht im Akku
         operand2.OperatorCode  = symbol.OpCode;
         operand2.OperatorParam = symbol.Par1;
         operand2.exchange      = exchangeTest(o2ftype, operand2.ftype);
		 if(symbol.OpCode == ASM_POPO_ST_32 && operand2.ftype == 0)
			 operand2.ftype = o1ftype;

         // Operation mit Operand2 anweisen
         add(CodeList, operand2);
         // Hilfsspeicherverwaltung
         if( operand1.h != -1 ) hmax--;
         if( operand2.h != -1 ) hmax--;
         hmax++;
         ergebnis.h             = hmax;
         ergebnis.url           = getURL();
         ergebnis.offset        = -1;
         ergebnis.OperatorCode  = 0;
         ergebnis.OpCode        = ASM_RESTORE;
         ergebnis.Par1.ui4      = hmax;
         ergebnis.dataCount     = symbol.dataCount;
         ergebnis.ftype         = ASM_getFType(symbol.OpCode, ftout);
         Keller[kp++]           = ergebnis;
         break;
      case type_popv:     
         if(--kp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
         operand2 = Keller[kp];      // Op2 = Array-Index
         if(--kp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
         operand1 = Keller[kp];      // Op1 = Value
         if( ergebnis.url == undefined ) {
            add(CodeList, operand1); // op1: Akku = Value
         } else {
            if( ergebnis.url == operand2.url ) {
               // save old operand1 in Hilfspeicher[hmax]
               ergebnis.OperatorCode   = 0;
               ergebnis.OpCode         = ASM_STORE;
               ergebnis.Par1.ui4       = hmax;
               add(CodeList, ergebnis);
               add(CodeList, operand1); // load
            } else if( ergebnis.url != operand1.url ) {
               // save old operand1 in Hilfspeicher[hmax]
               ergebnis.OperatorCode   = 0;
               ergebnis.OpCode         = ASM_STORE;
               ergebnis.Par1.ui4       = hmax;
               add(CodeList, ergebnis);
               add(CodeList, operand1); // load
            }
         }
         operand2.OperatorCode = ASM_POPV;
         // op2: Reg = Index + dummy-Operator ASM_POPV
         add(CodeList, operand2);
         // Speicheroperation anweisen
         symbol.OperatorCode = 0;
         hmax = 0;
         ergebnis.url = undefined;
         symbol.ftype = operand1.ftype;
         add(CodeList, symbol);
         break;
      case type_cc:
      case type_pop:
         if( kp > 0 ) {
            operand1 = Keller[--kp];
            if( operand1.h == -1 ) {
               add(CodeList, operand1); // load
            }
         }
         // Speicheroperation anweisen
         symbol.OperatorCode= 0;
         hmax               = 0;
         ergebnis.url       = undefined;
         symbol.ftype       = operand1.ftype;
         add(CodeList, symbol);
         break;
      case type_statm:
         // Operation anweisen
         symbol.OperatorCode = 0;
         add(CodeList, symbol);
         ergebnis.url = undefined;
         hmax = 0;
         break;
      case type_fct:
         // Funktionsaufruf generieren
         // par1 = Index, par2 = Länge Lokaldaten
         // par3 = parameter stack size
         // zunächst par3 - Parameter entkellern,
         // bis Länge Bytes entkellert


          if (symbol.parCount == 3)
              i = symbol.Par3.i4;
          else if (symbol.parCount == 2)
              i = symbol.Par2.i4;
          else
          {
              ASSERT(_ASSERT_); throw CFormatException("Invalid function parameter number");
          }

         // Anzahl der Parameter und evt. Returnparameter berechnen
         for(tkp = kp, size = pcount = 0; (size < i) && pcount < kp; ) {
            if(--tkp < 0 ) {ASSERT(_ASSERT_); throw CFormatException("Kellerpointer");}
            datacount = Keller[tkp].dataCount/8;      // Hoffentlich stimmt datacount immer!!!
            size += max(datacount, CCodec::stackAlignment); // size in Byte , datacount in Bit
            pcount++;
         }

         // Returnparameter vorhanden, wenn tkp > 0 ist
         if(tkp > 0) {
             pcount++; 
             rflag  = 1;
             fdc = Keller[tkp-1].dataCount;  // first dataCount für Return sichern
         }

         if (symbol.OpCode == ASM_CALF_PR)
         {
             // save the registers on stack
             for(tkp = 0; tkp < kp-pcount; tkp++ ) {
                operand1 = Keller[tkp];
                if( ergebnis.url == undefined ) {
                    add(CodeList, operand1); // load
                } else {
                    if( ergebnis.url != operand1.url ) {
                       // save old operand1 in Hilfspeicher[hmax]
                       ergebnis.OperatorCode   = 0;
                       ergebnis.OpCode         = ASM_STORE;
                       ergebnis.Par1.ui4       = hmax;
                       add(CodeList, ergebnis);
                       add(CodeList, operand1); // load
                    }
                }
                // Parameter kellern von Akku nach ((--SP))
                operand1.OpCode = ASM_MOVE;
                operand1.OperatorCode = 0;
                add(CodeList, operand1);
                ergebnis.url = undefined;
                // Hilfsspeicherverwaltung
                if( operand1.h != -1 ) hmax--;
             }
         }
         
         // Parameter in umgekehrter Reihenfolge umspeichern
         for(tkp = (kp -= pcount), i = 0; i < pcount; i++, tkp++ ) {
            operand1 = Keller[tkp];
            if( ergebnis.url == undefined ) {
                add(CodeList, operand1); // load
            } else {
                if( ergebnis.url != operand1.url ) {
                   // save old operand1 in Hilfspeicher[hmax]
                   ergebnis.OperatorCode   = 0;
                   ergebnis.OpCode         = ASM_STORE;
                   ergebnis.Par1.ui4       = hmax;
                   add(CodeList, ergebnis);
                   add(CodeList, operand1); // load
                }
            }
            // Parameter kellern von Akku nach ((--SP))
            operand1.OpCode = ASM_MOVE;
            add(CodeList, operand1);
            ergebnis.url = undefined;
            // Hilfsspeicherverwaltung
            if( operand1.h != -1 ) hmax--;
         }

         tkp = kp - 1;

         // Funktionsaufruf anweisen
         symbol.OperatorCode = 0;
         add(CodeList, symbol);
         // Falls Returnparameter gekellert, zurückholen
         if( rflag && ASM_CLST != IpCodeList[index_e].OpCode) {
             operand1.OpCode = ASM_REMOVE;
             operand1.dataCount = fdc;
             switch(fdc) {
             case 64:   operand1.ftype         = cLREAL;    break;
             case 32:   operand1.ftype         = cVECT32;   break;
             default:
                operand1.ftype         = 0;
             }
             add(CodeList, operand1);
             
             ergebnis.h             = ++hmax;
             ergebnis.url           = getURL();
             ergebnis.offset        = -1;
             ergebnis.OperatorCode  = 0;
             ergebnis.OpCode        = ASM_RESTORE;
             ergebnis.Par1.ui4      = hmax;
             ergebnis.dataCount     = fdc;
             // the datatype of length 32 is unknown
             // the datatype of length 64 is cLREAL, because L... not supported
             switch(fdc) {
             case 32:   ergebnis.ftype         = cVECT32;   break;
             case 64:   ergebnis.ftype         = cLREAL;    break;
             default:
                ergebnis.ftype         = 0;
             }
             Keller[kp++]           = ergebnis;

             if(tkp >= 0 && symbol.OpCode == ASM_CALF_PR) {
                  // save old operand1 in Hilfspeicher[hmax]
                  ergebnis.url           = getURL();
                  ergebnis.OpCode         = ASM_STORE;
                  add(CodeList, ergebnis);
                  ergebnis.url = undefined;
             }
         } else {
            ergebnis.url = undefined;
            hmax = 0;
         }

         if (symbol.OpCode == ASM_CALF_PR)
         {
              /* restore registers */
              for(; tkp >=0; tkp--) {
                operand1 = Keller[tkp];
                operand1.h = ++hmax;
              if(ergebnis.url != undefined && ergebnis.url != operand1.url) {
                  // save old operand1 in Hilfspeicher[hmax]
                  ergebnis.url           = getURL();
                  ergebnis.OpCode        = ASM_STORE;
                  add(CodeList, ergebnis);
              }
                // Parameter kellern von Akku nach ((--SP))
                operand1.OpCode = ASM_REMOVE;
                operand1.OperatorCode = 0;
                add(CodeList, operand1);
                // Hilfsspeicherverwaltung
                ergebnis.h             = operand1.h;
                ergebnis.url           = getURL();
                ergebnis.offset        = -1;
                ergebnis.OperatorCode  = 0;
                ergebnis.OpCode        = ASM_RESTORE;
                ergebnis.Par1.ui4      = operand1.h;
                ergebnis.dataCount     = operand1.dataCount;
                ergebnis.ftype         = operand1.ftype;
                Keller[tkp]            = ergebnis;
             }
         }
         
         break;
      default:
         ASSERT(_ASSERT_);
         throw CFormatException("IpCode not allow");
     
        } // switch
        index++;
    } // while

    return 1;
}


int CParser::ASM_getFType(int opcode, int pos)
{
    for (int i=0; i<sizeof(FType) / sizeof(FType[0]); i++)
    {
        if (opcode == FType[i].op)
        {
            switch(pos) { 
            case ftin1: return FType[i].in1;
            case ftin2: return FType[i].in2;
            case ftout: return FType[i].out;
            }
       }
    }
    return 0;  /* ohne float */
}

bool    CParser::isKommutativ(unsigned short OpCode)
{
   switch( OpCode ) {
   case ASM_ADD_SINT:
   case ASM_ADD_INT:
   case ASM_ADD_DINT:
   case ASM_ADD_LINT:
   case ASM_ADD_USINT:
   case ASM_ADD_UINT:
   case ASM_ADD_UDINT:
   case ASM_ADD_ULINT:
   case ASM_ADD_REAL:
   case ASM_ADD_LREAL:
      
   case ASM_MUL_SINT:
   case ASM_MUL_INT:
   case ASM_MUL_DINT:
   case ASM_MUL_LINT:
   case ASM_MUL_USINT:
   case ASM_MUL_UINT:
   case ASM_MUL_UDINT:
   case ASM_MUL_ULINT:
   case ASM_MUL_REAL:
   case ASM_MUL_LREAL:
      
   case ASM_EQ_SINT:
   case ASM_EQ_INT:
   case ASM_EQ_DINT:
   case ASM_EQ_LINT:
   case ASM_EQ_USINT:
   case ASM_EQ_UINT:
   case ASM_EQ_UDINT:
   case ASM_EQ_ULINT:
   case ASM_EQ_REAL:
   case ASM_EQ_LREAL:
   case ASM_EQ_BOOL:
      
   case ASM_NE_SINT:
   case ASM_NE_INT:
   case ASM_NE_DINT:
   case ASM_NE_LINT:
   case ASM_NE_USINT:
   case ASM_NE_UINT:
   case ASM_NE_UDINT:
   case ASM_NE_ULINT:
   case ASM_NE_REAL:
   case ASM_NE_LREAL:
   case ASM_NE_BOOL:
      
   case ASM_AND_BOOL:
   case ASM_AND_BYTE:
   case ASM_AND_WORD:
   case ASM_AND_DWORD:
   case ASM_AND_LWORD:
      
   case ASM_OR_BOOL:
   case ASM_OR_BYTE:
   case ASM_OR_WORD:
   case ASM_OR_DWORD:
   case ASM_OR_LWORD:
      
   case ASM_XOR_BOOL:
   case ASM_XOR_BYTE:
   case ASM_XOR_WORD:
   case ASM_XOR_DWORD:
   case ASM_XOR_LWORD:
      
      return true;
   }
   return false;
}

bool CParser::exchangeTest(int ft1, int ft2)
{
    return ( ft1 == cVECT32 && ft2 == cREAL ) || ( ft1 == cREAL && ft2 == cVECT32 );
}

void CParser::generateLS(unsigned char dtype, unsigned short segment, CNccCodeElem& NccCodeElem, int LsType)
{
   unsigned long     ul4;
   unsigned __int64  ul8; 

   if( (segment == SEG_GLOBAL) && (m_iList & option_Praef64) ) {
      if( dtype == cBOOL ) {
         memcpy(&ul8, NccCodeElem.Par1.uc8, 8);
         ul4 = ul8 & 7;
         ul8 = ul8 >> 3;
      } else {
         ul8 = NccCodeElem.Par1.ui4;
         ul4 = 0;
      }
      NccCodeElem.AddOpcode(Codec.LoadSelector(ul8 >> 16, ul8 & 0x0000FFFFL)); 
   } else {
      ul4 = NccCodeElem.Par1.ui4;
      NccCodeElem.AddOpcode(Codec.LoadReference(segment)); 
   }
   switch( LsType ) {
   case LSloadAkku:    NccCodeElem.AddOpcode(Codec.LoadAkkuAdrReg (dtype, ul4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSloadReg:     NccCodeElem.AddOpcode(Codec.LoadRegAdrReg  (dtype, ul4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, ul4, NccCodeElem.ParSize, NccCodeElem.ftype)); 
       if (segment == SEG_OUTPUT && m_iList && option_WriteIOFlags)
       {
           Parameter p;
           p.ui4 = 0xffffffff;
           // HACK: I know that WriteFlags member is next to pSeg
           NccCodeElem.AddOpcode(Codec.LoadReference(MAX_SEGMENTS));
           NccCodeElem.AddOpcode(Codec.LoadAkkuIm(p, NccCodeElem.ParSize, NccCodeElem.ftype));
           NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, ul4, NccCodeElem.ParSize, NccCodeElem.ftype)); 
       }
       break;
   }
}

void CParser::generateLSI(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType)
{
   NccCodeElem.AddOpcode(Codec.LoadInstanceReference(SEG_INSTANCE, NccCodeElem.Par1.ui4));
   switch( LsType ) {
   case LSloadAkku:    NccCodeElem.AddOpcode(Codec.LoadAkkuAdrReg (dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSloadReg:     NccCodeElem.AddOpcode(Codec.LoadRegAdrReg  (dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   }
}

void CParser::generateLSII(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType)
{
	if(dtype ==  cBOOL)
	{
		switch(LsType) {
		case LSloadAkku:    NccCodeElem.AddOpcode(Codec.LoadBitAdr(NccCodeElem.Par1.ui4, true));	break;
		case LSloadReg:     NccCodeElem.AddOpcode(Codec.LoadBitAdr(NccCodeElem.Par1.ui4, false));	break;
		case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreBitAdr(NccCodeElem.Par1.ui4, true));	break;
		}
		return;
	}

	NccCodeElem.AddOpcode(Codec.LoadAdrRegReference(NccCodeElem.Par1.ui4));
	switch(LsType) {
	case LSloadAkku:    NccCodeElem.AddOpcode(Codec.LoadAkkuAdrReg (dtype, 0, 0, NccCodeElem.ftype));	break;
	case LSloadReg:     NccCodeElem.AddOpcode(Codec.LoadRegAdrReg  (dtype, 0, 0, NccCodeElem.ftype));	break;
	case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, 0, 0, NccCodeElem.ftype));	break;
	}
}

void CParser::generateLSV(unsigned char dtype, unsigned short segment, CNccCodeElem& NccCodeElem, int LsType)
{
	switch( LsType ) {
	case LSloadAkku:	NccCodeElem.AddOpcode(Codec.LoadVectorReference(segment, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.dataCount, true)); 
						NccCodeElem.AddOpcode(Codec.LoadAkkuAdrRegV(dtype, NccCodeElem.ftype));
						break;
	case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.LoadVectorReference(segment, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.dataCount, false)); 
						NccCodeElem.AddOpcode(Codec.StoreAkkuAdrRegV(dtype, NccCodeElem.ftype));
						break;
	}
}

void CParser::generateLSS(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType)
{
	switch( LsType ) {
	case LSloadAkku:	NccCodeElem.AddOpcode(Codec.StoreAdr(0, true)); 
						NccCodeElem.AddOpcode(Codec.LoadAkkuAdrReg(dtype, NccCodeElem.Par1.ui4, 0, NccCodeElem.ftype));
						break;
	case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreAdr(0, false));
						NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, NccCodeElem.OperatorParam.ui4, 0, NccCodeElem.ftype));
						break;
	}
}

void CParser::generateLSID(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType)
{
   NccCodeElem.AddOpcode(Codec.SelectInstance(NccCodeElem.Par1.ui4)); 
   switch( LsType ) {
   case LSloadAkku:    NccCodeElem.AddOpcode(Codec.LoadAkkuAdrReg (dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSloadReg:     NccCodeElem.AddOpcode(Codec.LoadRegAdrReg  (dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   case LSstoreAkku:   NccCodeElem.AddOpcode(Codec.StoreAkkuAdrReg(dtype, NccCodeElem.Par2.ui4, NccCodeElem.ParSize, NccCodeElem.ftype)); break;
   }
}


void CParser::generateLEAID(CNccCodeElem& NccCodeElem)
{
    switch(NccCodeElem.OpCode)
	{
	case ASM_LEAD_GI:
	case ASM_LEAD_GI_1:
		NccCodeElem.AddOpcode(Codec.LoadReference(SEG_INPUT));				break;
	case ASM_LEAD_GX:
	case ASM_LEAD_GX_1:
		NccCodeElem.AddOpcode(Codec.LoadReference(SEG_OUTPUT));				break;
	case ASM_LEAD_GM:
	case ASM_LEAD_GM_1:
		NccCodeElem.AddOpcode(Codec.LoadReference(SEG_GLOBAL));				break;
	case ASM_LEAO_ID:
	case ASM_LEAO_ID_1:
	case ASM_LEAO_XD:
		NccCodeElem.AddOpcode(Codec.SelectInstance(NccCodeElem.Par1.ui4));	break;
	default:
        ASSERT(_ASSERT_);
        throw CFormatException("Internal Error (#101)");
	}
	
    switch(NccCodeElem.OpCode)
	{
	case ASM_LEAD_GI:
	case ASM_LEAD_GX:
	case ASM_LEAD_GM:
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par1.ui4, false));	break;
	case ASM_LEAD_GI_1:
	case ASM_LEAD_GX_1:
	case ASM_LEAD_GM_1:
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par1.ui4, true));	break;
	case ASM_LEAO_ID:	
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par2.ui4, false));	break;
	case ASM_LEAO_ID_1:	
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par2.ui4, true));	break;
	case ASM_LEAO_XD:	
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, 0, false));						break;
	default:
        ASSERT(_ASSERT_);
        throw CFormatException("Internal Error (#102)");
	}

	if(option_WriteIOFlags)
	{
		Parameter p;
		p.ui4 = 0xffffffff;
		switch(NccCodeElem.OpCode)
		{
		case ASM_LEAD_GX_1:
			NccCodeElem.AddOpcode(Codec.LoadReference(MAX_SEGMENTS));
			// HACK: I know that WriteFlags member is next to pSeg
			NccCodeElem.AddOpcode(Codec.LoadRegIm(p, NccCodeElem.ParSize, NccCodeElem.ftype));
			NccCodeElem.AddOpcode(Codec.StoreRegAdrReg(cBOOL, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0)); 
			break;
		case ASM_LEAD_GX:
			if (NccCodeElem.Par2.ui4 > 0)
			{
				unsigned copy_4bytes = NccCodeElem.Par2.ui4/4;
				bool copy_2bytes = (NccCodeElem.Par2.ui4%4 >= 2);
				bool copy_1byte = (NccCodeElem.Par2.ui4%2 == 1);
				unsigned offset = 0;

				NccCodeElem.AddOpcode(Codec.LoadReference(MAX_SEGMENTS));
				// HACK: I know that WriteFlags member is next to pSeg
				NccCodeElem.AddOpcode(Codec.LoadRegIm(p, NccCodeElem.ParSize, NccCodeElem.ftype));

				for(; copy_4bytes; copy_4bytes--, offset+=4)
					NccCodeElem.AddOpcode(Codec.StoreRegAdrReg(cDINT, NccCodeElem.Par1.ui4+offset, NccCodeElem.ParSize, 0));
				if(copy_2bytes)
				{
					NccCodeElem.AddOpcode(Codec.StoreRegAdrReg(cINT, NccCodeElem.Par1.ui4+offset, NccCodeElem.ParSize, 0));
					offset+=2;
				}
				if(copy_1byte)
					NccCodeElem.AddOpcode(Codec.StoreRegAdrReg(cBYTE, NccCodeElem.Par1.ui4+offset, NccCodeElem.ParSize, 0));
			}
			break;
		}
	}
}

void CParser::generateLEAI(CNccCodeElem& NccCodeElem)
{
    switch(NccCodeElem.OpCode)
	{
	case ASM_LEAD_IN:
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_INSTANCE, NccCodeElem.Par1.ui4, false));
		return;
	case ASM_LEAD_IN_1:	
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_INSTANCE, NccCodeElem.Par1.ui4, true));
		return;
	case ASM_LEAO_IN:
		NccCodeElem.AddOpcode(Codec.LoadInstanceReference(SEG_INSTANCE, NccCodeElem.Par1.ui4));
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par2.ui4, false));
		return;
	case ASM_LEAO_IN_1:	
		NccCodeElem.AddOpcode(Codec.LoadInstanceReference(SEG_INSTANCE, NccCodeElem.Par1.ui4));
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, NccCodeElem.Par2.ui4, true));
		return;
	case ASM_LEAO_XN:
		NccCodeElem.AddOpcode(Codec.LoadInstanceReference(SEG_INSTANCE, NccCodeElem.Par1.ui4));
		NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, 0, false));
		return;
	case 0:
		switch(NccCodeElem.OperatorCode)
		{
		case ASM_LEAI_ID:
			NccCodeElem.AddOpcode(Codec.LoadInstanceReference(SEG_GLOBALDATA, NccCodeElem.Par1.ui4));
			NccCodeElem.AddOpcode(Codec.LoadAkkuAdr(SEG_GLOBALDATA, 0, false));
			return;
		case ASM_LEAO_ST:
			NccCodeElem.AddOpcode(Codec.Swap(32, 0));
			NccCodeElem.AddOpcode(Codec.LoadAkkuIm(NccCodeElem.Par1, 4, 0));
			NccCodeElem.AddOpcode(Codec.Add(cDINT));
			return;
/*		case ASM_LEAO_ST_1:
			NccCodeElem.AddOpcode(Codec.Swap(64, cLREAL));
			NccCodeElem.AddOpcode(Codec.LoadAkkuIm(NccCodeElem.Par1, 8, cLREAL));
			NccCodeElem.AddOpcode(Codec.Add(cLREAL));
			return;*/
		}
	default:
        ASSERT(_ASSERT_);
        throw CFormatException("Internal Error (#104)");
	}
}


bool CParser::generateLoadAkku(CNccCodeElem& NccCodeElem)
{
    Parameter   lPar;

    switch( NccCodeElem.OpCode ) {
            // ==========  Load  Const  ==========
         case ASM_PUSHC:     NccCodeElem.AddOpcode(Codec.LoadAkkuIm(NccCodeElem.Par1, NccCodeElem.ParSize, NccCodeElem.ftype));                 break;
         case ASM_PUSHC_0:   memset(&lPar, 0, sizeof(lPar)); //rw lPar.ui4 = 0;
                             NccCodeElem.AddOpcode(Codec.LoadAkkuIm(lPar, NccCodeElem.ParSize, NccCodeElem.ftype));                                    break;
         case ASM_PUSHC_1:   memset(&lPar, 0, sizeof(lPar)); lPar.ui4 = 1; //rw lPar.ui4 = 1;
                             NccCodeElem.AddOpcode(Codec.LoadAkkuIm(lPar, NccCodeElem.ParSize, 0));                                    break;
            //rw            case ASM_PUSHI1_0:  NccCodeElem.AddOpcode(Codec.LoadAkkuIm(0, 1));                                                      break;
            //rw            case ASM_PUSHI1_1:  NccCodeElem.AddOpcode(Codec.LoadAkkuIm(1, 1));                                                      break;
            

            // ==========  Load  BOOL  ==========
         case ASM_PSHD_IN_1:   NccCodeElem.AddOpcode(Codec.LoadAkkuInd(cBOOL, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));         break;
         case ASM_PSHD_GR_1:  generateLS(cBOOL, SEG_RETAIN,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GI_1:  generateLS(cBOOL, SEG_INPUT,        NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GO_1:  generateLS(cBOOL, SEG_OUTPUT,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GM_1:  generateLS(cBOOL, SEG_GLOBAL,       NccCodeElem, LSloadAkku);                                       break;
            // Instance
         case ASM_PSHO_IN_1: generateLSI(cBOOL,                NccCodeElem, LSloadAkku);                                 break;
		 case ASM_PSHI_IN_1: generateLSII(cBOOL,			   NccCodeElem, LSloadAkku);									   break;
		 case ASM_PSHO_ID_1: generateLSID(cBOOL,               NccCodeElem, LSloadAkku);                                       break;
            
            // ==========  Load  BYTE  ==========
         case ASM_PSHD_IN_8:   NccCodeElem.AddOpcode(Codec.LoadAkkuInd(cBYTE, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));         break;
         case ASM_PSHD_GR_8:  generateLS(cBYTE, SEG_RETAIN,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GI_8:  generateLS(cBYTE, SEG_INPUT,        NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GO_8:  generateLS(cBYTE, SEG_OUTPUT,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GM_8:  generateLS(cBYTE, SEG_GLOBAL,       NccCodeElem, LSloadAkku);                                       break;
            // Instance
         case ASM_PSHO_IN_8: generateLSI(cBYTE,                NccCodeElem, LSloadAkku);                                 break;
		 case ASM_PSHI_IN_8: generateLSII(cBYTE,			   NccCodeElem, LSloadAkku);									   break;
		 case ASM_PSHO_ID_8: generateLSID(cBYTE,               NccCodeElem, LSloadAkku);                                       break;
            
            // ==========  Load  WORD  ==========
         case ASM_PSHD_IN_16:  NccCodeElem.AddOpcode(Codec.LoadAkkuInd(cWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));         break;
         case ASM_PSHD_GR_16: generateLS(cWORD, SEG_RETAIN,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GI_16: generateLS(cWORD, SEG_INPUT,        NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GO_16: generateLS(cWORD, SEG_OUTPUT,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GM_16: generateLS(cWORD, SEG_GLOBAL,       NccCodeElem, LSloadAkku);                                       break;
            // Instance
         case ASM_PSHO_IN_16: generateLSI(cWORD,               NccCodeElem, LSloadAkku);                                       break;
		 case ASM_PSHI_IN_16: generateLSII(cWORD,			   NccCodeElem, LSloadAkku);									   break;
		 case ASM_PSHO_ID_16: generateLSID(cWORD,              NccCodeElem, LSloadAkku);                                       break;
            
            // ==========  Load  DWORD  ==========
         case ASM_PSHD_IN_32:  NccCodeElem.AddOpcode(Codec.LoadAkkuInd(cDWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.ftype));        break;
         case ASM_PSHD_GR_32: generateLS(cDWORD, SEG_RETAIN,      NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GI_32: generateLS(cDWORD, SEG_INPUT,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GO_32: generateLS(cDWORD, SEG_OUTPUT,      NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GM_32: generateLS(cDWORD, SEG_GLOBAL,      NccCodeElem, LSloadAkku);                                       break;
            // Instance
         case ASM_PSHO_IN_32: generateLSI(cDWORD,              NccCodeElem, LSloadAkku);                                       break;
		 case ASM_PSHI_IN_32: generateLSII(cDWORD,			   NccCodeElem, LSloadAkku);									   break;
		 case ASM_PSHO_ID_32: generateLSID(cDWORD,             NccCodeElem, LSloadAkku);                                       break;

			 // ==========  Load  LWORD  ==========
         case ASM_PSHD_IN_64:  NccCodeElem.AddOpcode(Codec.LoadAkkuInd(cLWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.ftype));        break;
         case ASM_PSHD_GR_64: generateLS(cLWORD, SEG_RETAIN,      NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GI_64: generateLS(cLWORD, SEG_INPUT,       NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GO_64: generateLS(cLWORD, SEG_OUTPUT,      NccCodeElem, LSloadAkku);                                       break;
         case ASM_PSHD_GM_64: generateLS(cLWORD, SEG_GLOBAL,      NccCodeElem, LSloadAkku);                                       break;
            // Instance
         case ASM_PSHO_IN_64: generateLSI(cLWORD,              NccCodeElem, LSloadAkku);                                       break;
		 case ASM_PSHI_IN_64: generateLSII(cLWORD,			   NccCodeElem, LSloadAkku);									   break;
		 case ASM_PSHO_ID_64: generateLSID(cLWORD,             NccCodeElem, LSloadAkku);                                       break;
         
		 default: 
             return false;
    }
    return true;
}

bool CParser::generateLoadReg (CNccCodeElem& NccCodeElem, int ftype)
{
    Parameter   lPar;
    switch( NccCodeElem.OpCode ) {
             // ==========  Load  BOOL  ==========
          case ASM_PSHD_IN_1:  NccCodeElem.AddOpcode(Codec.LoadRegInd( cBOOL, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
          case ASM_PSHD_GR_1:  generateLS(cBOOL, SEG_RETAIN,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GI_1:  generateLS(cBOOL, SEG_INPUT,        NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GO_1:  generateLS(cBOOL, SEG_OUTPUT,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GM_1:  generateLS(cBOOL, SEG_GLOBAL,       NccCodeElem, LSloadReg);                                    break;
             // Instance
          case ASM_PSHO_IN_1:  generateLSI(cBOOL,                NccCodeElem, LSloadReg);                                 break;
		  case ASM_PSHI_IN_1:  generateLSII(cBOOL,				 NccCodeElem, LSloadReg);									   break;
		  case ASM_PSHO_ID_1:  generateLSID(cBOOL,               NccCodeElem, LSloadReg);                                       break;
             
             // ==========  Load  BYTE  ==========
          case ASM_PSHD_IN_8:  NccCodeElem.AddOpcode(Codec.LoadRegInd( cBYTE, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
          case ASM_PSHD_GR_8:  generateLS(cBYTE, SEG_RETAIN,       NccCodeElem, LSloadReg);                                        break;
          case ASM_PSHD_GI_8:  generateLS(cBYTE, SEG_INPUT,        NccCodeElem, LSloadReg);                                        break;
          case ASM_PSHD_GO_8:  generateLS(cBYTE, SEG_OUTPUT,       NccCodeElem, LSloadReg);                                        break;
          case ASM_PSHD_GM_8:  generateLS(cBYTE, SEG_GLOBAL,       NccCodeElem, LSloadReg);                                        break;
             // Instance
          case ASM_PSHO_IN_8:  generateLSI(cBYTE,                NccCodeElem, LSloadReg);                                 break;
		  case ASM_PSHI_IN_8:  generateLSII(cBYTE,				 NccCodeElem, LSloadReg);									   break;
		  case ASM_PSHO_ID_8:  generateLSID(cBYTE,               NccCodeElem, LSloadReg);                                       break;
             
             // ==========  Load  WORD  ==========
          case ASM_PSHD_IN_16: NccCodeElem.AddOpcode(Codec.LoadRegInd( cWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
          case ASM_PSHD_GR_16: generateLS(cWORD, SEG_RETAIN,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GI_16: generateLS(cWORD, SEG_INPUT,        NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GO_16: generateLS(cWORD, SEG_OUTPUT,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GM_16: generateLS(cWORD, SEG_GLOBAL,       NccCodeElem, LSloadReg);                                    break;
             // Instance
          case ASM_PSHO_IN_16: generateLSI(cWORD,               NccCodeElem, LSloadReg);                                    break;
		  case ASM_PSHI_IN_16: generateLSII(cWORD,			    NccCodeElem, LSloadReg);									   break;
		  case ASM_PSHO_ID_16: generateLSID(cWORD,              NccCodeElem, LSloadReg);                                       break;
             
             // ==========  Load  DWORD  ==========
          case ASM_PSHD_IN_32: NccCodeElem.AddOpcode(Codec.LoadRegInd( cDWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, ftype));     break;
          case ASM_PSHD_GR_32: generateLS(cDWORD, SEG_RETAIN,      NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GI_32: generateLS(cDWORD, SEG_INPUT,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GO_32: generateLS(cDWORD, SEG_OUTPUT,      NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GM_32: generateLS(cDWORD, SEG_GLOBAL,      NccCodeElem, LSloadReg);                                    break;
             // Instance
          case ASM_PSHO_IN_32: generateLSI(cDWORD,              NccCodeElem, LSloadReg);                                    break;
		  case ASM_PSHI_IN_32: generateLSII(cDWORD,			    NccCodeElem, LSloadReg);									   break;
 		  case ASM_PSHO_ID_32: generateLSID(cDWORD,             NccCodeElem, LSloadReg);                                       break;
            
             // ==========  Load  LWORD  ==========
          case ASM_PSHD_IN_64: NccCodeElem.AddOpcode(Codec.LoadRegInd( cLWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, ftype));     break;
          case ASM_PSHD_GR_64: generateLS(cLWORD, SEG_RETAIN,      NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GI_64: generateLS(cLWORD, SEG_INPUT,       NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GO_64: generateLS(cLWORD, SEG_OUTPUT,      NccCodeElem, LSloadReg);                                    break;
          case ASM_PSHD_GM_64: generateLS(cLWORD, SEG_GLOBAL,      NccCodeElem, LSloadReg);                                    break;
             // Instance
          case ASM_PSHO_IN_64: generateLSI(cLWORD,              NccCodeElem, LSloadReg);                                    break;
		  case ASM_PSHI_IN_64: generateLSII(cLWORD,			    NccCodeElem, LSloadReg);									   break;
		  case ASM_PSHO_ID_64: generateLSID(cLWORD,             NccCodeElem, LSloadReg);                                       break;
             
             
             // ==========  Load  Const  ==========
          case ASM_PUSHC:     NccCodeElem.AddOpcode(Codec.LoadRegIm(  NccCodeElem.Par1, NccCodeElem.ParSize, ftype));         break;
          case ASM_PUSHC_0:   memset(&lPar, 0, sizeof(lPar)); //rw lPar.ui4 = 0;
                              NccCodeElem.AddOpcode(Codec.LoadRegIm(lPar, NccCodeElem.ParSize, ftype));                       break;
          case ASM_PUSHC_1:   memset(&lPar, 0, sizeof(lPar)); lPar.ui4 = 1; //rw 
                              NccCodeElem.AddOpcode(Codec.LoadRegIm(lPar, NccCodeElem.ParSize, 0));                           break;
             //rw                case ASM_PUSHI1_0:  NccCodeElem.AddOpcode(Codec.LoadRegIm(0, 1));                                                       break;
             //rw                case ASM_PUSHI1_1:  NccCodeElem.AddOpcode(Codec.LoadRegIm(1, 1));                                                       break;
         default: 
             return false;
    }
    return true;
}

int CParser::generateNCC(CNccCodeList& CodeList)
{
   int              index, size, tmpftype;
   unsigned short   OpCode;  //rw
   bool             akku;
   
   size = CodeList.GetSize();
   for(index = 0; index < size; index++) {
      CNccCodeElem        NccCodeElem;
      
      NccCodeElem = CodeList[index];
      NccCodeElem.nNccCode = 0;
      NccCodeElem.isNative = NC_CODE;

      if( NccCodeElem.OperatorCode == 0 ) {  
         // Anweisungen, Laden, Speichern, JMP
         OpCode = NccCodeElem.OpCode;
         if( !generateLoadAkku(NccCodeElem) )
         switch ( OpCode ) 
         {
         case ASM_BREAK:																											break;
         case ASM_RESTORE:		NccCodeElem.AddOpcode(Codec.Restore(NccCodeElem.dataCount, NccCodeElem.Par1.ui4, false, NccCodeElem.ftype));           break;
         case ASM_STORE:		NccCodeElem.AddOpcode(Codec.Store  (NccCodeElem.dataCount, NccCodeElem.Par1.ui4, NccCodeElem.ftype));                  break;
         case ASM_MOVE:			NccCodeElem.AddOpcode(Codec.Move   (NccCodeElem.dataCount, NccCodeElem.ftype));                     break;
         case ASM_REMOVE:		NccCodeElem.AddOpcode(Codec.ReMove (NccCodeElem.dataCount, NccCodeElem.ftype));                     break;
         case ASM_I2F:			NccCodeElem.AddOpcode(Codec.i2f    (NccCodeElem.ftype, true));                                      break;
            // ==========  Store BOOL  ==========
         case ASM_POPD_IN_1:    NccCodeElem.AddOpcode(Codec.StoreAkkuInd(cBOOL, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
         case ASM_POPD_GR_1:	generateLS(cBOOL, SEG_RETAIN,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GI_1:	generateLS(cBOOL, SEG_INPUT,        NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GX_1:	generateLS(cBOOL, SEG_OUTPUT,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GM_1:	generateLS(cBOOL, SEG_GLOBAL,       NccCodeElem, LSstoreAkku);                                      break;
            // Vektor
         case ASM_POPV_IN_1:	generateLSV(cBOOL, SEG_INSTANCE,    NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPV_ID_1:    generateLSV(cBOOL, SEG_GLOBALDATA,  NccCodeElem, LSstoreAkku);                                      break;
            // Instance
         case ASM_POPO_IN_1:	generateLSI(cBOOL,					NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPI_IN_1:	generateLSII(cBOOL,					NccCodeElem, LSstoreAkku);										break;
		 case ASM_POPO_ID_1:	generateLSID(cBOOL,					NccCodeElem, LSstoreAkku);                                      break;
            
            // ==========  Store BYTE  ==========
         case ASM_POPD_IN_8:    NccCodeElem.AddOpcode(Codec.StoreAkkuInd(cBYTE, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
         case ASM_POPD_GR_8:	generateLS(cBYTE, SEG_RETAIN,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GI_8:	generateLS(cBYTE, SEG_INPUT,        NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GX_8:	generateLS(cBYTE, SEG_OUTPUT,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GM_8:	generateLS(cBYTE, SEG_GLOBAL,       NccCodeElem, LSstoreAkku);                                      break;
            // Vektor
         case ASM_POPV_IN_8:	generateLSV(cBYTE, SEG_INSTANCE,    NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPV_ID_8:    generateLSV(cBYTE, SEG_GLOBALDATA,  NccCodeElem, LSstoreAkku);                                      break;
            // Instance
         case ASM_POPO_IN_8:	generateLSI(cBYTE,					NccCodeElem, LSstoreAkku);										break;
         case ASM_POPI_IN_8:	generateLSII(cBYTE,					NccCodeElem, LSstoreAkku);										break;
		 case ASM_POPO_ID_8:	generateLSID(cBYTE,					NccCodeElem, LSstoreAkku);                                      break;
            
            // ==========  Store WORD  ==========
         case ASM_POPD_IN_16:   NccCodeElem.AddOpcode(Codec.StoreAkkuInd(cWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, 0));     break;
         case ASM_POPD_GR_16:	generateLS(cWORD, SEG_RETAIN,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GI_16:	generateLS(cWORD, SEG_INPUT,        NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GX_16:	generateLS(cWORD, SEG_OUTPUT,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GM_16:	generateLS(cWORD, SEG_GLOBAL,       NccCodeElem, LSstoreAkku);                                      break;
            // Vektor
         case ASM_POPV_IN_16:	generateLSV(cWORD, SEG_INSTANCE,    NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPV_ID_16:   generateLSV(cWORD, SEG_GLOBALDATA,  NccCodeElem, LSstoreAkku);                                      break;
            // Instance
         case ASM_POPO_IN_16:	generateLSI(cWORD,					NccCodeElem, LSstoreAkku);									break;
         case ASM_POPI_IN_16:	generateLSII(cWORD,					NccCodeElem, LSstoreAkku);										break;
		 case ASM_POPO_ID_16:	generateLSID(cWORD,					NccCodeElem, LSstoreAkku);                                      break;
            
            // ==========  Store DWORD  ==========
         case ASM_POPD_IN_32:   NccCodeElem.AddOpcode(Codec.StoreAkkuInd(cDWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.ftype));        break;
         case ASM_POPD_GR_32:	generateLS(cDWORD, SEG_RETAIN,      NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GI_32:	generateLS(cDWORD, SEG_INPUT,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GX_32:	generateLS(cDWORD, SEG_OUTPUT,      NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GM_32:	generateLS(cDWORD, SEG_GLOBAL,      NccCodeElem, LSstoreAkku);                                      break;
            // Vektor
         case ASM_POPV_IN_32:	generateLSV(cDWORD, SEG_INSTANCE,   NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPV_ID_32:   generateLSV(cDWORD, SEG_GLOBALDATA, NccCodeElem, LSstoreAkku);                                      break;
            // Instance
         case ASM_POPO_IN_32:	generateLSI(cDWORD,					NccCodeElem, LSstoreAkku);										break;
         case ASM_POPI_IN_32:	generateLSII(cDWORD,				NccCodeElem, LSstoreAkku);										break;
		 case ASM_POPO_ID_32:	generateLSID(cDWORD,				NccCodeElem, LSstoreAkku);                                      break;
            
            // ==========  Store LWORD  ==========
         case ASM_POPD_IN_64:   NccCodeElem.AddOpcode(Codec.StoreAkkuInd(cLWORD, NccCodeElem.Par1.ui4, NccCodeElem.ParSize, NccCodeElem.ftype));        break;
         case ASM_POPD_GR_64:	generateLS(cLWORD, SEG_RETAIN,      NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GI_64:	generateLS(cLWORD, SEG_INPUT,       NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GX_64:	generateLS(cLWORD, SEG_OUTPUT,      NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPD_GM_64:	generateLS(cLWORD, SEG_GLOBAL,      NccCodeElem, LSstoreAkku);                                      break;
            // Vektor
         case ASM_POPV_IN_64:	generateLSV(cLWORD, SEG_INSTANCE,   NccCodeElem, LSstoreAkku);                                      break;
         case ASM_POPV_ID_64:   generateLSV(cLWORD, SEG_GLOBALDATA, NccCodeElem, LSstoreAkku);                                      break;
            // Instance
         case ASM_POPO_IN_64:	generateLSI(cLWORD,					NccCodeElem, LSstoreAkku);										break;
         case ASM_POPI_IN_64:	generateLSII(cLWORD,				NccCodeElem, LSstoreAkku);										break;
		 case ASM_POPO_ID_64:	generateLSID(cLWORD,				NccCodeElem, LSstoreAkku);                                      break;
            		 
            // ==========  Jmp         ==========
         case ASM_JMP:			NccCodeElem.AddOpcode(Codec.Jmp(JMP_UC));                                                           break;
         case ASM_JMPC:			NccCodeElem.AddOpcode(Codec.Jmp(JMP_C));                                                            break;
         case ASM_JMPCN:		NccCodeElem.AddOpcode(Codec.Jmp(JMP_CN));                                                           break;

            // ==========  LEA         ==========
		 case ASM_LEAD_GI:
		 case ASM_LEAD_GI_1:
		 case ASM_LEAD_GX:
		 case ASM_LEAD_GX_1:
         case ASM_LEAD_GM:
         case ASM_LEAD_GM_1:
         case ASM_LEAO_ID:
         case ASM_LEAO_ID_1:
         case ASM_LEAO_XD:		generateLEAID(NccCodeElem);																			break;
         case ASM_LEAD_IN:
         case ASM_LEAD_IN_1:
         case ASM_LEAO_IN:
         case ASM_LEAO_IN_1:
         case ASM_LEAO_XN:		generateLEAI(NccCodeElem);																			break;

            // ==========  Call        ==========
         case ASM_CALF_PR:		NccCodeElem.AddOpcode(Codec.calF   (fn,      NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALF_CL:		NccCodeElem.AddOpcode(Codec.calFSM (fnm,     NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALF_SL:		NccCodeElem.AddOpcode(Codec.calFSM (fnsys,   NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_PI:		NccCodeElem.AddOpcode(Codec.calFB  (fb,      NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_PN:		NccCodeElem.AddOpcode(Codec.calFB  (fbid,    NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_CI:		NccCodeElem.AddOpcode(Codec.calFBSM(fbm,     NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_CN:		NccCodeElem.AddOpcode(Codec.calFBSM(fbmid,   NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_SI:		NccCodeElem.AddOpcode(Codec.calFBSM(fbsys,   NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_CALB_SN:		NccCodeElem.AddOpcode(Codec.calFBSM(fbsysid, NccCodeElem.Par1.ui4, NccCodeElem.Par2.ui4));          break;
         case ASM_RETN:			NccCodeElem.AddOpcode(Codec.Return());                                                              break;
         case ASM_CLST:			NccCodeElem.AddOpcode(Codec.Leave (NccCodeElem.Par1.ui4));                                          break;
         default:
            ASSERT(_ASSERT_);
            throw CFormatException("Internal Error (#105)");
            }
        } else if( NccCodeElem.OpCode == 0) {
           // 1 Operand und PUSHV...
           switch( NccCodeElem.OperatorCode ) {
           case ASM_ABS_SINT:	NccCodeElem.AddOpcode(Codec.Abs(cSINT));                                                            break;
           case ASM_ABS_INT:	NccCodeElem.AddOpcode(Codec.Abs(cINT));                                                             break;
           case ASM_ABS_DINT:	NccCodeElem.AddOpcode(Codec.Abs(cDINT));                                                            break;
           case ASM_NEG_SINT:	NccCodeElem.AddOpcode(Codec.Neg(cSINT));                                                            break;
           case ASM_NEG_INT:	NccCodeElem.AddOpcode(Codec.Neg(cINT));                                                             break;
           case ASM_NEG_DINT:	NccCodeElem.AddOpcode(Codec.Neg(cDINT));                                                            break;
           case ASM_NOT_BOOL:	NccCodeElem.AddOpcode(Codec.Not(cBOOL));                                                            break;
           case ASM_NOT_BYTE:	NccCodeElem.AddOpcode(Codec.Not(cBYTE));                                                            break;
           case ASM_NOT_WORD:	NccCodeElem.AddOpcode(Codec.Not(cWORD));                                                            break;
           case ASM_NOT_DWORD:	NccCodeElem.AddOpcode(Codec.Not(cDWORD));                                                           break;
              
           case ASM_ABS_REAL:	NccCodeElem.AddOpcode(Codec.Abs(cREAL));                                                            break;
           case ASM_NEG_REAL:	NccCodeElem.AddOpcode(Codec.Neg(cREAL));                                                            break;

           case ASM_ABS_LREAL:	NccCodeElem.AddOpcode(Codec.Abs(cLREAL));                                                           break;
           case ASM_NEG_LREAL:	NccCodeElem.AddOpcode(Codec.Neg(cLREAL));                                                           break;

              // ==========  Load  BOOL  ==========
              // Vektor
           case ASM_PSHV_IN_1:  generateLSV(cBOOL, SEG_INSTANCE,    NccCodeElem, LSloadAkku);                                       break;  
           case ASM_PSHV_ID_1:  generateLSV(cBOOL, SEG_GLOBALDATA,  NccCodeElem, LSloadAkku);                                       break;
		   case ASM_PSHO_ST_1:  generateLSS(cBOOL,					NccCodeElem, LSloadAkku);                                       break;
              // ==========  Load  BYTE  ==========
              // Vektor
           case ASM_PSHV_IN_8:	generateLSV(cBYTE, SEG_INSTANCE,    NccCodeElem, LSloadAkku);                                       break;  
           case ASM_PSHV_ID_8:  generateLSV(cBYTE, SEG_GLOBALDATA,  NccCodeElem, LSloadAkku);                                       break;
		   case ASM_PSHO_ST_8:  generateLSS(cBYTE,					NccCodeElem, LSloadAkku);                                       break;
              // ==========  Load  WORD  ==========
              // Vektor
           case ASM_PSHV_IN_16: generateLSV(cWORD, SEG_INSTANCE,    NccCodeElem, LSloadAkku);                                       break;  
           case ASM_PSHV_ID_16: generateLSV(cWORD, SEG_GLOBALDATA,  NccCodeElem, LSloadAkku);                                       break;
		   case ASM_PSHO_ST_16: generateLSS(cWORD,					NccCodeElem, LSloadAkku);                                       break;
              // ==========  Load DWORD  ==========
              // Vektor
           case ASM_PSHV_IN_32: generateLSV(cDWORD, SEG_INSTANCE,   NccCodeElem, LSloadAkku);                                       break;  
           case ASM_PSHV_ID_32: generateLSV(cDWORD, SEG_GLOBALDATA, NccCodeElem, LSloadAkku);                                       break;
		   case ASM_PSHO_ST_32: generateLSS(cDWORD,					NccCodeElem, LSloadAkku);                                       break;
              // ==========  Load LWORD  ==========
              // Vektor
           case ASM_PSHV_IN_64: generateLSV(cLWORD, SEG_INSTANCE,   NccCodeElem, LSloadAkku);                                       break;  
           case ASM_PSHV_ID_64: generateLSV(cLWORD, SEG_GLOBALDATA, NccCodeElem, LSloadAkku);                                       break;
		   case ASM_PSHO_ST_64: generateLSS(cLWORD,					NccCodeElem, LSloadAkku);                                       break;
              // ==========  Konvert  ==========
           case ASM_BOOL_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cUSINT   ));										break;
           case ASM_BOOL_TO_SINT    : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cSINT    ));                                        break;
           case ASM_BOOL_TO_UINT    : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cUINT    ));                                        break; 
           case ASM_BOOL_TO_INT     : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cINT     ));                                        break;
           case ASM_BOOL_TO_UDINT   : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cUDINT   ));                                        break;
           case ASM_BOOL_TO_DINT    : NccCodeElem.AddOpcode(Codec.ConvBOOL2INTS(cDINT    ));                                        break;
           case ASM_BOOL_TO_REAL    : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cBOOL, cREAL));                                      break;
           case ASM_BOOL_TO_LREAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cBOOL, cLREAL));                                     break;
           case ASM_USINT_TO_BOOL   : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cUSINT));                                           break;
           case ASM_USINT_TO_SINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cSINT    ));                                break;
           case ASM_USINT_TO_BYTE   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cBYTE    ));                                break;
           case ASM_USINT_TO_UINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cUINT    ));                                break;
           case ASM_USINT_TO_INT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cINT     ));                                break;
           case ASM_USINT_TO_UDINT  : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cUDINT   ));                                break;
           case ASM_USINT_TO_DINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUSINT, cDINT    ));                                break;
           case ASM_USINT_TO_REAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUSINT, cREAL));                                     break;
           case ASM_USINT_TO_LREAL  : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUSINT, cLREAL));                                    break;
           case ASM_SINT_TO_BOOL    : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cSINT));                                            break;
           case ASM_SINT_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cSINT,  cUSINT   ));                                break;
           case ASM_SINT_TO_UINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cSINT,  cUINT    ));                                break;
           case ASM_SINT_TO_INT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cSINT,  cINT     ));                                break;
           case ASM_SINT_TO_UDINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cSINT,  cUDINT   ));                                break;
           case ASM_SINT_TO_DINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cSINT,  cDINT    ));                                break;
           case ASM_SINT_TO_REAL    : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cSINT, cREAL));                                      break;
           case ASM_SINT_TO_LREAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cSINT, cLREAL));                                     break;
           case ASM_BYTE_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cBYTE,  cUSINT   ));                                break;
           case ASM_UINT_TO_BOOL    : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cUINT));                                            break;
           case ASM_UINT_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cUSINT   ));                                break;
           case ASM_UINT_TO_SINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cSINT    ));                                break;
           case ASM_UINT_TO_INT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cINT     ));                                break;
           case ASM_UINT_TO_WORD    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cWORD    ));                                break;
           case ASM_UINT_TO_UDINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cUDINT   ));                                break;
           case ASM_UINT_TO_DINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUINT,  cDINT    ));                                break;
           case ASM_UINT_TO_REAL    : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUINT, cREAL));                                      break;
           case ASM_UINT_TO_LREAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUINT, cLREAL));                                     break;
           case ASM_INT_TO_BOOL     : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cINT));                                             break;
           case ASM_INT_TO_USINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cINT,   cUSINT   ));                                break;
           case ASM_INT_TO_SINT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cINT,   cSINT    ));                                break;
           case ASM_INT_TO_UINT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cINT,   cUINT    ));                                break;
           case ASM_INT_TO_UDINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cINT,   cUDINT   ));                                break;
           case ASM_INT_TO_DINT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cINT,   cDINT    ));                                break;
           case ASM_INT_TO_REAL     : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cINT, cREAL));                                       break;
           case ASM_INT_TO_LREAL    : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cINT, cLREAL));                                      break;
           case ASM_WORD_TO_UINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cWORD,  cUINT    ));                                break;
           case ASM_UDINT_TO_BOOL   : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cUDINT));                                           break;
           case ASM_UDINT_TO_USINT  : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUDINT, cUSINT   ));                                break;
           case ASM_UDINT_TO_SINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUDINT, cSINT    ));                                break;
           case ASM_UDINT_TO_UINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUDINT, cUINT    ));                                break;
           case ASM_UDINT_TO_INT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUDINT, cINT     ));                                break;
           case ASM_UDINT_TO_DINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cUDINT, cDINT    ));                                break;
           case ASM_UDINT_TO_REAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUDINT, cREAL));                                     break;
           case ASM_UDINT_TO_LREAL  : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cUDINT, cLREAL));                                    break;
           case ASM_DINT_TO_BOOL    : NccCodeElem.AddOpcode(Codec.ConvINTS2BOOL(cDINT));                                            break;
           case ASM_DINT_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDINT,  cUSINT   ));                                break;
           case ASM_DINT_TO_SINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDINT,  cSINT    ));                                break;
           case ASM_DINT_TO_UINT    : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDINT,  cUINT    ));                                break;
           case ASM_DINT_TO_INT     : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDINT,  cINT     ));                                break;
           case ASM_DINT_TO_UDINT   : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDINT,  cUDINT   ));                                break;
           case ASM_DINT_TO_REAL    : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cDINT, cREAL));                                      break;
           case ASM_DINT_TO_LREAL   : NccCodeElem.AddOpcode(Codec.ConvALL2REAL(cDINT, cLREAL));                                     break;
           case ASM_DWORD_TO_UDINT  : NccCodeElem.AddOpcode(Codec.ConvINTS2INTS(cDWORD, cUDINT   ));                                break;
           case ASM_REAL_TO_BOOL    : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cBOOL     ));                                break;
           case ASM_REAL_TO_USINT   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cUSINT    ));                                break;
           case ASM_REAL_TO_SINT    : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cSINT     ));                                break;
           case ASM_REAL_TO_UINT    : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cUINT     ));                                break;
           case ASM_REAL_TO_INT     : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cINT      ));                                break;
           case ASM_REAL_TO_UDINT   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cUDINT    ));                                break;
           case ASM_REAL_TO_DINT    : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cREAL,  cDINT     ));                                break;
           case ASM_LREAL_TO_BOOL   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cBOOL     ));                                break;
           case ASM_LREAL_TO_USINT  : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cUSINT    ));                                break;
           case ASM_LREAL_TO_SINT   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cSINT     ));                                break;
           case ASM_LREAL_TO_UINT   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cUINT     ));                                break;
           case ASM_LREAL_TO_INT    : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cINT      ));                                break;
           case ASM_LREAL_TO_UDINT  : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cUDINT    ));                                break;
           case ASM_LREAL_TO_DINT   : NccCodeElem.AddOpcode(Codec.ConvREAL2ALL(cLREAL, cDINT     ));                                break;
           case ASM_REAL_TO_LREAL   : NccCodeElem.AddOpcode(Codec.ConvREAL2REAL(cREAL, cLREAL    ));                                break;
           case ASM_LREAL_TO_REAL   : NccCodeElem.AddOpcode(Codec.ConvREAL2REAL(cLREAL,cREAL     ));                                break;

            // ==========  LEA         ==========
	       case ASM_LEAI_ID:		generateLEAI(NccCodeElem);																		break;
	       case ASM_LEAO_ST:		generateLEAI(NccCodeElem);																		break;
           default:
              ASSERT(_ASSERT_);
              throw CFormatException("Internal Error (#106)");
           }
        } else {
            // 2 Operanden
            // Beim Laden des 2.Operanden muss ftype exchange beachten
            if( NccCodeElem.exchange ) 
                tmpftype = cVECT32;
            else
                tmpftype = NccCodeElem.ftype;

            if( NccCodeElem.h != -1 ) {
                // Op2 aus Zwischenergebnis
                if( NccCodeElem.swapFlag ) {
                    // Op2 := Op1
                    NccCodeElem.AddOpcode(Codec.Swap(NccCodeElem.dataCount, NccCodeElem.swapftype));
                    // Op1 := Zwischenergebnis   
                    akku=true;
                    NccCodeElem.AddOpcode(Codec.Restore(NccCodeElem.dataCount, NccCodeElem.Par1.ui4, false, tmpftype));
                } else {
                    // Op2 := Zwischenergebnis   
                    akku=false;
                    NccCodeElem.AddOpcode(Codec.Restore(NccCodeElem.dataCount, NccCodeElem.Par1.ui4, true, tmpftype));
                }
            } else {
                // Op2 aus Speicherzelle
                if( NccCodeElem.swapFlag ) {
                    // Op2 := Op1
                    NccCodeElem.AddOpcode(Codec.Swap(NccCodeElem.dataCount, NccCodeElem.swapftype));
                    // Op1 = Konstante, Speicherzelle
                    akku = true;
                    if( !generateLoadAkku(NccCodeElem) ) {
                        ASSERT(_ASSERT_);
                        throw CFormatException("Internal Error (#107)");
                    }
                } else { 
                    // Op2 = Konstante, Speicherzelle
                    akku = false;
                    if( !generateLoadReg(NccCodeElem, tmpftype) ) {
                        ASSERT(_ASSERT_);
                        throw CFormatException("Internal Error (#108)");
                    }
                }
            }

           // evtl. Reg. tauschen; Vektorzugriffe
           if( NccCodeElem.exchange )
                NccCodeElem.AddOpcode(Codec.i2f(NccCodeElem.ftype, akku));

           switch( NccCodeElem.OperatorCode ) {
              // ==========  logische Operationen  ==========  
           case ASM_POPV:                                                                                                          break;
		   case ASM_POPO_ST_1:  generateLSS(cBOOL,  NccCodeElem, LSstoreAkku);													   break;
		   case ASM_POPO_ST_8:  generateLSS(cBYTE,  NccCodeElem, LSstoreAkku);													   break;
		   case ASM_POPO_ST_16: generateLSS(cWORD,  NccCodeElem, LSstoreAkku);													   break;
		   case ASM_POPO_ST_32: generateLSS(cDWORD, NccCodeElem, LSstoreAkku);													   break;
		   case ASM_POPO_ST_64: generateLSS(cLWORD, NccCodeElem, LSstoreAkku);													   break;
           case ASM_AND_BOOL:  NccCodeElem.AddOpcode(Codec.And(cBOOL));                                                            break;
           case ASM_AND_BYTE:  NccCodeElem.AddOpcode(Codec.And(cBYTE));                                                            break;
           case ASM_AND_WORD:  NccCodeElem.AddOpcode(Codec.And(cWORD));                                                            break;
           case ASM_AND_DWORD: NccCodeElem.AddOpcode(Codec.And(cDWORD));                                                           break;
           case ASM_OR_BOOL:   NccCodeElem.AddOpcode(Codec.Or (cBOOL));                                                            break;
           case ASM_OR_BYTE:   NccCodeElem.AddOpcode(Codec.Or(cBYTE));                                                             break;
           case ASM_OR_WORD:   NccCodeElem.AddOpcode(Codec.Or(cWORD));                                                             break;
           case ASM_OR_DWORD:  NccCodeElem.AddOpcode(Codec.Or(cDWORD));                                                            break;
           case ASM_ROL_BYTE:  NccCodeElem.AddOpcode(Codec.Rol(cBYTE));                                                            break;
           case ASM_ROL_WORD:  NccCodeElem.AddOpcode(Codec.Rol(cWORD));                                                            break;
           case ASM_ROL_DWORD: NccCodeElem.AddOpcode(Codec.Rol(cDWORD));                                                           break;
           case ASM_ROR_BYTE:  NccCodeElem.AddOpcode(Codec.Ror(cBYTE));                                                            break;
           case ASM_ROR_WORD:  NccCodeElem.AddOpcode(Codec.Ror(cWORD));                                                            break;
           case ASM_ROR_DWORD: NccCodeElem.AddOpcode(Codec.Ror(cDWORD));                                                           break;
           case ASM_SHL_BYTE:  NccCodeElem.AddOpcode(Codec.Shl(cBYTE));                                                            break;
           case ASM_SHL_WORD:  NccCodeElem.AddOpcode(Codec.Shl(cWORD));                                                            break;
           case ASM_SHL_DWORD: NccCodeElem.AddOpcode(Codec.Shl(cDWORD));                                                           break;
           case ASM_SHR_BYTE:  NccCodeElem.AddOpcode(Codec.Shr(cBYTE));                                                            break;
           case ASM_SHR_WORD:  NccCodeElem.AddOpcode(Codec.Shr(cWORD));                                                            break;
           case ASM_SHR_DWORD: NccCodeElem.AddOpcode(Codec.Shr(cDWORD));                                                           break;
           case ASM_XOR_BOOL:  NccCodeElem.AddOpcode(Codec.Xor(cBOOL));                                                            break;
           case ASM_XOR_BYTE:  NccCodeElem.AddOpcode(Codec.Xor(cBYTE));                                                            break;
           case ASM_XOR_WORD:  NccCodeElem.AddOpcode(Codec.Xor(cWORD));                                                            break;
           case ASM_XOR_DWORD: NccCodeElem.AddOpcode(Codec.Xor(cDWORD));                                                           break;
              // ==========  arithmetische Operationen  ==========  
           case ASM_ADD_SINT:  NccCodeElem.AddOpcode(Codec.Add(cSINT));                                                            break;
           case ASM_ADD_USINT: NccCodeElem.AddOpcode(Codec.Add(cUSINT));                                                           break;
           case ASM_ADD_INT:   NccCodeElem.AddOpcode(Codec.Add(cINT));                                                             break;
           case ASM_ADD_UINT:  NccCodeElem.AddOpcode(Codec.Add(cUINT));                                                            break;
           case ASM_ADD_DINT:  NccCodeElem.AddOpcode(Codec.Add(cDINT));                                                             break;
           case ASM_ADD_UDINT: NccCodeElem.AddOpcode(Codec.Add(cUDINT));                                                            break;
           case ASM_DIV_SINT:  NccCodeElem.AddOpcode(Codec.Div(cSINT));                                                            break;
           case ASM_DIV_USINT: NccCodeElem.AddOpcode(Codec.Div(cUSINT));                                                           break;
           case ASM_DIV_INT:   NccCodeElem.AddOpcode(Codec.Div(cINT));                                                             break;
           case ASM_DIV_UINT:  NccCodeElem.AddOpcode(Codec.Div(cUINT));                                                            break;
           case ASM_DIV_DINT:  NccCodeElem.AddOpcode(Codec.Div(cDINT));                                                             break;
           case ASM_DIV_UDINT: NccCodeElem.AddOpcode(Codec.Div(cUDINT));                                                            break;
           case ASM_MOD_SINT:  NccCodeElem.AddOpcode(Codec.Mod(cSINT));                                                            break;
           case ASM_MOD_USINT: NccCodeElem.AddOpcode(Codec.Mod(cUSINT));                                                           break;
           case ASM_MOD_INT:   NccCodeElem.AddOpcode(Codec.Mod(cINT));                                                             break;
           case ASM_MOD_UINT:  NccCodeElem.AddOpcode(Codec.Mod(cUINT));                                                            break;
           case ASM_MOD_DINT:  NccCodeElem.AddOpcode(Codec.Mod(cDINT));                                                             break;
           case ASM_MOD_UDINT: NccCodeElem.AddOpcode(Codec.Mod(cUDINT));                                                            break;
           case ASM_MUL_SINT:  NccCodeElem.AddOpcode(Codec.Mul(cSINT));                                                            break;
           case ASM_MUL_USINT: NccCodeElem.AddOpcode(Codec.Mul(cUSINT));                                                           break;
           case ASM_MUL_INT:   NccCodeElem.AddOpcode(Codec.Mul(cINT));                                                             break;
           case ASM_MUL_UINT:  NccCodeElem.AddOpcode(Codec.Mul(cUINT));                                                            break;
           case ASM_MUL_DINT:  NccCodeElem.AddOpcode(Codec.Mul(cDINT));                                                             break;
           case ASM_MUL_UDINT: NccCodeElem.AddOpcode(Codec.Mul(cUDINT));                                                            break;
           case ASM_SUB_SINT:  NccCodeElem.AddOpcode(Codec.Sub(cSINT));                                                            break;
           case ASM_SUB_USINT: NccCodeElem.AddOpcode(Codec.Sub(cUSINT));                                                           break;
           case ASM_SUB_INT:   NccCodeElem.AddOpcode(Codec.Sub(cINT));                                                             break;
           case ASM_SUB_UINT:  NccCodeElem.AddOpcode(Codec.Sub(cUINT));                                                            break;
           case ASM_SUB_DINT:  NccCodeElem.AddOpcode(Codec.Sub(cDINT));                                                             break;
           case ASM_SUB_UDINT: NccCodeElem.AddOpcode(Codec.Sub(cUDINT));                                                            break;

           case ASM_ADD_REAL:  NccCodeElem.AddOpcode(Codec.Add(cREAL));                                                            break;
           case ASM_DIV_REAL:  NccCodeElem.AddOpcode(Codec.Div(cREAL));                                                            break;
           case ASM_MOD_REAL:  NccCodeElem.AddOpcode(Codec.Mod(cREAL));                                                            break;
           case ASM_MUL_REAL:  NccCodeElem.AddOpcode(Codec.Mul(cREAL));                                                            break;
           case ASM_SUB_REAL:  NccCodeElem.AddOpcode(Codec.Sub(cREAL));                                                            break;
           case ASM_POW_REAL_REAL:  NccCodeElem.AddOpcode(Codec.Pow(cREAL,cREAL));                                                            break;
           case ASM_POW_REAL_INT:   NccCodeElem.AddOpcode(Codec.Pow(cREAL,cINT));                                                            break;
           case ASM_POW_REAL_DINT:  NccCodeElem.AddOpcode(Codec.Pow(cREAL,cDINT));                                                            break;

           case ASM_ADD_LREAL:  NccCodeElem.AddOpcode(Codec.Add(cLREAL));                                                            break;
           case ASM_DIV_LREAL:  NccCodeElem.AddOpcode(Codec.Div(cLREAL));                                                            break;
           case ASM_MOD_LREAL:  NccCodeElem.AddOpcode(Codec.Mod(cLREAL));                                                            break;
           case ASM_MUL_LREAL:  NccCodeElem.AddOpcode(Codec.Mul(cLREAL));                                                            break;
           case ASM_SUB_LREAL:  NccCodeElem.AddOpcode(Codec.Sub(cLREAL));                                                            break;
           case ASM_POW_LREAL_LREAL: NccCodeElem.AddOpcode(Codec.Pow(cLREAL,cLREAL));                                                            break;
           case ASM_POW_LREAL_INT:   NccCodeElem.AddOpcode(Codec.Pow(cLREAL,cINT));                                                            break;
           case ASM_POW_LREAL_DINT:  NccCodeElem.AddOpcode(Codec.Pow(cLREAL,cDINT));                                                            break;

              // ==========  Vergleichsoperationen  ==========  
           case ASM_GT_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_GT));                                                    break;
           case ASM_GE_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_GE));                                                    break;
           case ASM_EQ_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_EQ));                                                    break;
           case ASM_LE_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_LE));                                                    break;
           case ASM_LT_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_LT));                                                    break;
           case ASM_NE_BOOL:   NccCodeElem.AddOpcode(Codec.Cmp(cBOOL, CMP_NE));                                                    break;
           case ASM_GT_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_GT));                                                    break;
           case ASM_GE_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_GE));                                                    break;
           case ASM_EQ_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_EQ));                                                    break;
           case ASM_LE_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_LE));                                                    break;
           case ASM_LT_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_LT));                                                    break;
           case ASM_NE_USINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUSINT,CMP_NE));                                                    break;
           case ASM_GT_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_GT));                                                    break;
           case ASM_GE_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_GE));                                                    break;
           case ASM_EQ_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_EQ));                                                    break;
           case ASM_LE_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_LE));                                                    break;
           case ASM_LT_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_LT));                                                    break;
           case ASM_NE_SINT:   NccCodeElem.AddOpcode(Codec.Cmp(cSINT, CMP_NE));                                                    break;
           case ASM_GT_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_GT));                                                    break;
           case ASM_GE_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_GE));                                                    break;
           case ASM_EQ_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_EQ));                                                    break;
           case ASM_LE_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_LE));                                                    break;
           case ASM_LT_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_LT));                                                    break;
           case ASM_NE_UINT:   NccCodeElem.AddOpcode(Codec.Cmp(cUINT, CMP_NE));                                                    break;
           case ASM_GT_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_GT));                                                    break;
           case ASM_GE_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_GE));                                                    break;
           case ASM_EQ_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_EQ));                                                    break;
           case ASM_LE_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_LE));                                                    break;
           case ASM_LT_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_LT));                                                    break;
           case ASM_NE_INT:    NccCodeElem.AddOpcode(Codec.Cmp(cINT,  CMP_NE));                                                    break;
           case ASM_GT_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_GT));                                                    break;
           case ASM_GE_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_GE));                                                    break;
           case ASM_EQ_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_EQ));                                                    break;
           case ASM_LE_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_LE));                                                    break;
           case ASM_LT_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_LT));                                                    break;
           case ASM_NE_UDINT:  NccCodeElem.AddOpcode(Codec.Cmp(cUDINT,CMP_NE));                                                    break;
           case ASM_GT_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_GT));                                                    break;
           case ASM_GE_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_GE));                                                    break;
           case ASM_EQ_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_EQ));                                                    break;
           case ASM_LE_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_LE));                                                    break;
           case ASM_LT_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_LT));                                                    break;
           case ASM_NE_DINT:   NccCodeElem.AddOpcode(Codec.Cmp(cDINT, CMP_NE));                                                    break;
           case ASM_GT_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_GT));                                                    break;
           case ASM_GE_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_GE));                                                    break;
           case ASM_EQ_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_EQ));                                                    break;
           case ASM_LE_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_LE));                                                    break;
           case ASM_LT_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_LT));                                                    break;
           case ASM_NE_REAL:   NccCodeElem.AddOpcode(Codec.Cmp(cREAL, CMP_NE));                                                    break;
           case ASM_GT_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_GT));                                                    break;
           case ASM_GE_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_GE));                                                    break;
           case ASM_EQ_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_EQ));                                                    break;
           case ASM_LE_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_LE));                                                    break;
           case ASM_LT_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_LT));                                                    break;
           case ASM_NE_LREAL:  NccCodeElem.AddOpcode(Codec.Cmp(cLREAL,CMP_NE));                                                    break;
           default:
              ASSERT(_ASSERT_);
              throw CFormatException("Internal Error (#109)");
           }
        }
        
        NccCodeList.Add(NccCodeElem);
        // MapNccLabel mit Key = Label und Value = Index in NccCodeList
        MapNccLabel.SetAt((void*)(NccCodeElem.label), (void*)(NccCodeList.GetUpperBound()));
    }
    
    return 1;
}


void CParser::segmentedNCC(int index, unsigned long& ulsize)
{
   unsigned long  ulNewSize, ulLastSize, jmp_size;
   unsigned long  Adr, lastAdr;

   if (m_b16BitProc == FALSE)
   {
	   return; // the follwing code doesn't align corectily the code on segment limit jump
               // this will affect only 16 bit targets (OSAI)
   }

   jmp_size = Codec.Jmp(JMP_UC).size;

   if( index + 1 == NccCodeList.GetSize() )
      // letzter Block
      return;

   // Überschreitet der nächste Block die Segmentgrenze ?
   ulLastSize = ulsize;
   ulNewSize  = ulsize + NccCodeList[index+1].nNccCode;
   if (NccCodeList[index+1].pSTRING)
   {
      ulNewSize += NccCodeList[index+1].pSTRING[1] + 2;
   }

   lastAdr  = m_NC_LoadAddress + ulLastSize;
   Adr      = m_NC_LoadAddress + ulNewSize + jmp_size + 2;
   if( (lastAdr & SEGMENT) == (Adr & SEGMENT) ) 
      return;

   // JA -> Sprung einfügen
   Adr      &= SEGMENT;
   if( (NccCodeList[index].isNative == IP_CODE) || (NccCodeList[index+1].isNative == IP_CODE)) {
      // IP-NOPs einfügen, da IP-Code bzw. gerade Umschaltung zu IP-Code
    
      switch(code_allignment ) {    
      case 1:   break;
      case 2:   if(lastAdr & 1) Adr++;
                break;
      case 4:   if(lastAdr & 3) Adr += 4 - (lastAdr & 3);
                break;
      }

      NccCodeList[index].seg_size = Adr - lastAdr;
   } else {
      if(NccCodeList[index-1].isNative == IP_CODE)
      {
          switch(code_allignment ) {    
          case 1:   break;
          case 2:   if(lastAdr & 1) Adr++;
                    break;
          case 4:   if(lastAdr & 3) Adr += 4 - (lastAdr & 3);
                    break;
          }

          NccCodeList[index-1].seg_size += Adr - lastAdr;
      }
      else
      {
        if(CCodec::jmp_relative)
            NccCodeList[index].AddOpcode(Codec.Jmp(JMP_UC, Adr - lastAdr));
        else
            NccCodeList[index].AddOpcode(Codec.Jmp(JMP_UC, Adr));
       // NC-NOPs einfügen
       NccCodeList[index].seg_size = Adr - lastAdr - jmp_size;
      }
   }
   // Codeverlängerung
   ulsize += Adr - lastAdr;
}

int CParser::linkNCC(CStdioFile& ListFile, Cxxx_dbp& xxx_dbp)
{
   unsigned long    ulSize;
   int              index, size, isNative, xxx_index;
   int              pos, count, i, j;
   void*            pvalue;
   CBlob            blobai, blobia;
   
   // insert IP-NC-switching
   blobai = CCodec::ASM_IP();
   blobia = CCodec::IP_ASM();
   size = NccCodeList.GetSize();
   ulSize = 0;
   isNative = IP_CODE; // startcode
   for( index = skipEmptyNccBlock(0); index < size; index = skipEmptyNccBlock(index+1) ) {
      NccCodeList[index].nNccIpAsm = 0;
      if( isNative != NccCodeList[index].isNative ) {
         // append ASM_IP after last Code-Bytes
         if( NccCodeList[index].isNative == IP_CODE ) {
             for (j = index - 1; NC_CODE != NccCodeList[j].isNative && j >= 0; j--)
                 ;
             ASSERT(j >=0 );

            pos = NccCodeList[j].nNccCode;
            for(count = 0; count < blobai.size; count++)
               NccCodeList[j].NccCode[pos++] = blobai.buffer[count];
            NccCodeList[j].nNccCode += blobai.size;
            ulSize += blobai.size;
         }
         // insert ASM_NATIVE before the first Code-Byte
         else if( NccCodeList[index].isNative == NC_CODE ) {
            // IP_ASM()
            for(count = 0; count < blobia.size; count++) {
              memmove(NccCodeList[index].NccCode+1, NccCodeList[index].NccCode, NccCodeList[index].nNccCode);
              NccCodeList[index].NccCode[0] = blobia.buffer[blobia.size - count - 1];
              NccCodeList[index].nNccCode++;
              NccCodeList[index].nNccIpAsm++;
              NccCodeList[index].OpcodeString = "IP_ASM";
            }
            // ASM_NATIVE
            memmove(NccCodeList[index].NccCode+1, NccCodeList[index].NccCode, NccCodeList[index].nNccCode);
            NccCodeList[index].NccCode[0] = ASM_NATIVE;
            NccCodeList[index].nNccCode++;
            NccCodeList[index].nNccIpAsm++;
            // code_allignment
            switch( code_allignment ) {
            case 1:
                break;
            case 2:
               if( (ulSize+1) & 1 ) {
                  // even Address !
                  memmove(NccCodeList[index].NccCode+1, NccCodeList[index].NccCode, NccCodeList[index].nNccCode);
                  NccCodeList[index].NccCode[0] = ASM_NOP;
                  NccCodeList[index].nNccCode++;
                  NccCodeList[index].nNccIpAsm++;
                  NccCodeList[index].OpcodeString = "NOP";
               }
               break;
            case 4:
               if( (ulSize+1) & 3 ) {
                  // even Address !
                   for(i = 0; i < 4 - ((ulSize+1) & 3); i++) {
                      memmove(NccCodeList[index].NccCode+1, NccCodeList[index].NccCode, NccCodeList[index].nNccCode);
                      NccCodeList[index].NccCode[0] = ASM_NOP;
                      NccCodeList[index].nNccCode++;
                      NccCodeList[index].nNccIpAsm++;
                      NccCodeList[index].OpcodeString = "NOP";
                  }
               }
               break;
            }
         }
      }
      isNative = NccCodeList[index].isNative;
      ulSize += NccCodeList[index].nNccCode;
      if (NccCodeList[index].pSTRING)
      {
         ulSize += NccCodeList[index].pSTRING[1] + 2;
      }
   }
   /*
   // Modul must end with IP !
   // Now in ASM_RETN-Codec
   if( isNative == NC_CODE ) {
      pos = NccCodeList[size-1].nNccCode;
      for(count = 0; count < blob.size; count++)
         NccCodeList[size-1].NccCode[pos++] = blob.buffer[count];
      NccCodeList[size-1].nNccCode += blob.size;
   }
   */ 
   // Calculate offset and check segment
   ulSize = 0;
   for(index = 0; index < size; index++) {
      NccCodeList[index].offset = ulSize;
      ulSize += NccCodeList[index].nNccCode;
      if (NccCodeList[index].pSTRING)
      {
         ulSize += NccCodeList[index].pSTRING[1] + 2;
      }
      segmentedNCC(index, ulSize);
   }

    // Modul-Alignment
    // code_allignment
    switch( code_allignment ) {
    case 1:
        break;
    case 2:
        if( ulSize & 1 ) {
            // even Address !
            CNccCodeElem    NccCodeElem;

            NccCodeElem.OpCode      = ASM_NOP;
            NccCodeElem.NccCode[0]  = 0;    // nie ausgeführt
            NccCodeElem.nNccCode    = 1;
            NccCodeElem.offset      = ulSize;
            NccCodeElem.OpcodeString = "NOP";

            ulSize++;
            NccCodeList.Add(NccCodeElem);
            size = NccCodeList.GetSize();
        }
        break;
    case 4:
       if( ulSize & 3 ) {
            CNccCodeElem    NccCodeElem;

            NccCodeElem.OpCode      = ASM_NOP;
            NccCodeElem.NccCode[0]  = 0;    // nie ausgeführt
            NccCodeElem.nNccCode    = 1;
            NccCodeElem.offset      = ulSize;
            NccCodeElem.OpcodeString = "NOP";
            // even Address !
            j = 4 - (ulSize & 3);
            for (i = 0; i < j; i++) 
            {
                ulSize++;
                NccCodeList.Add(NccCodeElem);
            }
            size = NccCodeList.GetSize();
       }
       break;
    }

    // Check CodeSize
    if( !CCodec::checkSize(ulSize, m_NC_LoadAddress) ) {
        CString str, pou_name;
        pou_name = xxx_dbp.getFileName();
        pou_name = pou_name.Right(pou_name.GetLength() - pou_name.ReverseFind('\\') - 1);
        pou_name = pou_name.Left(pou_name.ReverseFind('.'));
        str.Format("POU '%s' code size %dKB exceeds 64KB", pou_name, ulSize>>10);
        throw CFormatException(str);
    }
   
   // Calculate jmps
   for(index = 0; index < size; index++) {
        if( NccCodeList[index].jmpto != 0 ) {
            // search label
            long           label = NccCodeList[index].jmpto;
            long           diff;     // Sprungweite
            int            dindex;   // Index Sprungziel
            unsigned long  abs_adr;  // Absolute Adresse

            if( !MapNccLabel.Lookup((void*)label, pvalue) ) {
                CString str;
                str.Format("NcCode Jmp : 0x%04x Adr: %05d", IpCodeList[index].OpCode, IpCodeList[index].offset);
                ASSERT(_ASSERT_);
                throw CFormatException(str);
            }
            dindex  = (int)pvalue;
            dindex  = skipEmptyNccBlock(dindex);
            diff    = NccCodeList[dindex].offset - NccCodeList[index].offset;
            abs_adr = NccCodeList[dindex].offset + m_NC_LoadAddress;

            if( NccCodeList[index].isNative == IP_CODE ) {
                // update IP-Jmp
                // check parcount
                switch( NccCodeList[index].ParSize ) {
                case 1:
                    if( (diff < -128) || (diff > 127) )
                        throw CFormatException("Internal Error (#110)");
                case 2: 
                    if( (diff < -32768) || (diff > 32767) )
                        throw CFormatException("Internal Error (#111)");
                }
                NccCodeList[index].Par1.ui4 = diff;

                // jmps have 2 byte OpCodes
                if( big_endian == 0 )
                {
                    memcpy(NccCodeList[index].NccCode+2, &diff, NccCodeList[index].ParSize);
                } else {
                    for( int i = 0; i < NccCodeList[index].ParSize; i++ ) 
                    {
                        NccCodeList[index].NccCode[2 + NccCodeList[index].ParSize-1 - i] = 
                        *((unsigned char *)&diff + i);
                    }
                }
            } else {
              // update NC-Jmp
              // Dest is IP ?
              if( NccCodeList[dindex].isNative == IP_CODE ) 
              {
                  ASSERT((dindex>0) && (NccCodeList[dindex-1].isNative != IP_CODE));
                  // sizeof(ASM_IP) subtrahieren
                  // + die Füllbytes für den Segmentwechsel in IP-Code
                  diff     -= CCodec::sizeAsmIp + NccCodeList[dindex-1].seg_size;
                  abs_adr  -= CCodec::sizeAsmIp + NccCodeList[dindex-1].seg_size;
              }
              // Absprung- oder Zielblock mit IP->NC Umschaltung ?
              diff     -= NccCodeList[index].nNccIpAsm;
              diff     += NccCodeList[dindex].nNccIpAsm;
              abs_adr  += NccCodeList[dindex].nNccIpAsm;
              // Sprungtype ist im Codec bekannt
              if( CCodec::jmp_relative ) {
                 // Länge der Codesequenze einrechnen
                 if( (diff < -CCodec::maxRelJmp) || (diff > CCodec::maxRelJmp) )
                 {
                     CString str;
                     str.Format("Relative jump exceeds %dK", (CCodec::maxRelJmp+1)/1024);
                     ASSERT(_ASSERT_); 
                     throw CFormatException(str);
                 }
                 if( NccCodeList[index].type == type_statm )       // unconditional jmp
                 {
                    diff -= CCodec::StartJmpCode;
                    CCodec::setJmpAdr(NccCodeList[index].NccCode +
                                      CCodec::sizeJmpCode - 
                                      CCodec::JmpOffset + 
                                      NccCodeList[index].nNccIpAsm, 
                                      diff, (bool)(index > dindex));
                 } 
                 else // conditional jmp
                 {
                    diff -= CCodec::StartJmpCCode;
                    CCodec::setJmpAdr(NccCodeList[index].NccCode + 
                                      CCodec::sizeJmpCCode - 
                                      CCodec::JmpOffset + 
                                      NccCodeList[index].nNccIpAsm, 
                                      diff, (bool)(index > dindex));
                 }
              } 
              else 
              { 
                 // jmp_absolute
                 if( NccCodeList[index].type == type_statm ) 
                 {
                    CCodec::setJmpAdr(NccCodeList[index].NccCode + 
                                      CCodec::sizeJmpCode - 
                                      CCodec::JmpOffset + 
                                      NccCodeList[index].nNccIpAsm, 
                                      abs_adr, (bool)(index > dindex));
        
                 } 
                 else 
                 {
                    CCodec::setJmpAdr(NccCodeList[index].NccCode + 
                                      CCodec::sizeJmpCCode - 
                                      CCodec::JmpOffset + 
                                      NccCodeList[index].nNccIpAsm, 
                                   abs_adr, (bool)(index > dindex));
                 }
              }
            }
        } // if
   } // for
   
   
   // Calculate breakpointlist
   for(index = 0; index < size; index++) 
   {
      if( (xxx_index=NccCodeList[index].BpIndex) != -1 ) 
      {  // Line with breakpoint
         xxx_dbp[xxx_index].ipLineNumber = NccCodeList[index].offset;
         xxx_dbp[xxx_index].isNative     = NccCodeList[index].isNative;
      }
   }
   
   // Listing
   if( m_iList & option_NccList ) 
   {
      isNative = 0; 
      for(index = 0; index < size; index++) 
      {
         // NCC-Source nicht für Alignment-Byte µµ??
         ListFile.WriteString( 
            NccCodeList[index].Format( (m_iList & option_ReAsm) > 0, 
                                       isNative < NccCodeList[index].isNative, 
                                       m_NC_LoadAddress) 
            + CString('\n'));
         if( NccCodeList[index].nNccCode )
            // ignore empty blocks
            isNative = NccCodeList[index].isNative;
      }
   }
   return ulSize;
}



//   Type                  i1      i2       o 	
CParser::CFType CParser::FType[92]  =
{
    {ASM_NOP              , 0,      0,      0}, 
    {ASM_BOOL_TO_REAL     , 0,      0,      cREAL}, 
    {ASM_USINT_TO_REAL    , 0,      0,      cREAL},
    {ASM_BYTE_TO_REAL     , 0,      0,      cREAL},
    {ASM_SINT_TO_REAL     , 0,      0,      cREAL},
    {ASM_UINT_TO_REAL     , 0,      0,      cREAL},
    {ASM_WORD_TO_REAL     , 0,      0,      cREAL},
    {ASM_INT_TO_REAL      , 0,      0,      cREAL},
    {ASM_UDINT_TO_REAL    , 0,      0,      cREAL},
    {ASM_DWORD_TO_REAL    , 0,      0,      cREAL},
    {ASM_DINT_TO_REAL     , 0,      0,      cREAL},
    {ASM_ULINT_TO_REAL    , 0,      0,      cREAL},
    {ASM_LWORD_TO_REAL    , 0,      0,      cREAL},
    {ASM_LINT_TO_REAL     , 0,      0,      cREAL},

    {ASM_REAL_TO_BOOL     , cREAL,  0,      0},
    {ASM_REAL_TO_USINT    , cREAL,  0,      0},
    {ASM_REAL_TO_BYTE     , cREAL,  0,      0},
    {ASM_REAL_TO_SINT     , cREAL,  0,      0},
    {ASM_REAL_TO_UINT     , cREAL,  0,      0},
    {ASM_REAL_TO_WORD     , cREAL,  0,      0},
    {ASM_REAL_TO_INT      , cREAL,  0,      0},
    {ASM_REAL_TO_UDINT    , cREAL,  0,      0},
    {ASM_REAL_TO_DWORD    , cREAL,  0,      0},
    {ASM_REAL_TO_DINT     , cREAL,  0,      0},
    {ASM_REAL_TO_ULINT    , cREAL,  0,      0},
    {ASM_REAL_TO_LWORD    , cREAL,  0,      0},
    {ASM_REAL_TO_LINT     , cREAL,  0,      0},

    {ASM_BOOL_TO_LREAL    , 0,      0,      cLREAL}, 
    {ASM_USINT_TO_LREAL   , 0,      0,      cLREAL},
    {ASM_BYTE_TO_LREAL    , 0,      0,      cLREAL},
    {ASM_SINT_TO_LREAL    , 0,      0,      cLREAL},
    {ASM_UINT_TO_LREAL    , 0,      0,      cLREAL},
    {ASM_WORD_TO_LREAL    , 0,      0,      cLREAL},
    {ASM_INT_TO_LREAL     , 0,      0,      cLREAL},
    {ASM_UDINT_TO_LREAL   , 0,      0,      cLREAL},
    {ASM_DWORD_TO_LREAL   , 0,      0,      cLREAL},
    {ASM_DINT_TO_LREAL    , 0,      0,      cLREAL},
    {ASM_REAL_TO_LREAL    , cREAL,  0,      cLREAL},
    {ASM_ULINT_TO_LREAL   , 0,      0,      cLREAL},
    {ASM_LWORD_TO_LREAL   , 0,      0,      cLREAL},
    {ASM_LINT_TO_LREAL    , 0,      0,      cLREAL},

    {ASM_LREAL_TO_BOOL    , cLREAL, 0,      0},
    {ASM_LREAL_TO_USINT   , cLREAL, 0,      0},
    {ASM_LREAL_TO_BYTE    , cLREAL, 0,      0},
    {ASM_LREAL_TO_SINT    , cLREAL, 0,      0},
    {ASM_LREAL_TO_UINT    , cLREAL, 0,      0},
    {ASM_LREAL_TO_WORD    , cLREAL, 0,      0},
    {ASM_LREAL_TO_INT     , cLREAL, 0,      0},
    {ASM_LREAL_TO_UDINT   , cLREAL, 0,      0},
    {ASM_LREAL_TO_DWORD   , cLREAL, 0,      0},
    {ASM_LREAL_TO_DINT    , cLREAL, 0,      0},
    {ASM_LREAL_TO_REAL    , cLREAL, 0,      cREAL},
    {ASM_LREAL_TO_ULINT   , cLREAL, 0,      0},
    {ASM_LREAL_TO_LWORD   , cLREAL, 0,      0},
    {ASM_LREAL_TO_LINT    , cLREAL, 0,      0},

    {ASM_NEG_REAL         , cREAL,  0,      cREAL},
    {ASM_ABS_REAL         , cREAL,  0,      cREAL},
    {ASM_ADD_REAL         , cREAL,  cREAL,  cREAL},
    {ASM_SUB_REAL         , cREAL,  cREAL,  cREAL},
    {ASM_MUL_REAL         , cREAL,  cREAL,  cREAL},
    {ASM_DIV_REAL         , cREAL,  cREAL,  cREAL},
    {ASM_MOD_REAL         , cREAL,  cREAL,  cREAL},
    {ASM_GT_REAL          , cREAL,  cREAL,  0},
    {ASM_GE_REAL          , cREAL,  cREAL,  0},
    {ASM_EQ_REAL          , cREAL,  cREAL,  0},
    {ASM_LE_REAL          , cREAL,  cREAL,  0},
    {ASM_LT_REAL          , cREAL,  cREAL,  0},
    {ASM_NE_REAL          , cREAL,  cREAL,  0},

    {ASM_NEG_LREAL        , cLREAL, 0,      cLREAL},
    {ASM_ABS_LREAL        , cLREAL, 0,      cLREAL},
    {ASM_ADD_LREAL        , cLREAL, cLREAL, cLREAL},
    {ASM_SUB_LREAL        , cLREAL, cLREAL, cLREAL},
    {ASM_MUL_LREAL        , cLREAL, cLREAL, cLREAL},
    {ASM_DIV_LREAL        , cLREAL, cLREAL, cLREAL},
    {ASM_MOD_LREAL        , cLREAL, cLREAL, cLREAL},
    {ASM_GT_LREAL         , cLREAL, cLREAL, 0},
    {ASM_GE_LREAL         , cLREAL, cLREAL, 0},
    {ASM_EQ_LREAL         , cLREAL, cLREAL, 0},
    {ASM_LE_LREAL         , cLREAL, cLREAL, 0},
    {ASM_LT_LREAL         , cLREAL, cLREAL, 0},
    {ASM_NE_LREAL         , cLREAL, cLREAL, 0},

    {ASM_POW_REAL_REAL    , cREAL,  cREAL,  cREAL},
    {ASM_POW_REAL_INT     , cREAL,  0,      cREAL},
    {ASM_POW_REAL_DINT    , cREAL,  0,      cREAL},
    {ASM_POW_LREAL_LREAL  , cLREAL, cLREAL, cLREAL},
    {ASM_POW_LREAL_INT    , cLREAL, 0,      cLREAL},
    {ASM_POW_LREAL_DINT   , cLREAL, 0,      cLREAL},

    // Sonderbehandlung von Vektoren; Kennung: i1
    {ASM_PSHV_IN_32       , 0,      0,      cVECT32},
    {ASM_PSHV_ID_32       , 0,      0,      cVECT32},
    {ASM_POPV_IN_32       , 0,      0,      cVECT32},
    {ASM_POPV_ID_32       , 0,      0,      cVECT32},
};

/* ---------------------------------------------------------------------------- */

